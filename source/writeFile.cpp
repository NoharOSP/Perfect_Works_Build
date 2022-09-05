#include "pch.h"
#include "writeFile.h"



writeFile::writeFile(HWND hWnd, std::string home, std::string cd, int num, bool itemsspells, bool script, bool stats)
{
	dir = home;
	discName = cd;
	discNum = num;
	wind = hWnd;
	std::filesystem::current_path(dir);
	if (std::filesystem::exists("\patches")) {
		std::filesystem::current_path("\patches");
		if (std::filesystem::exists("\shared_files")) {
			std::filesystem::current_path("\shared_files");
		}
		else {
			MessageBox(wind, L"Could not find directory for 'shared_files'.", L"Error", MB_ICONERROR);
		}
	}
	else {
		MessageBox(wind, L"Could not find directory for 'patches'.", L"Error", MB_ICONERROR);
	}
	if (itemsspells && script && stats) {
		allTrue();
	}
	else if (itemsspells && !script && stats) {
		noScript();
	}
	else if (!itemsspells && script && stats) {
		noItems();
	}
}


writeFile::~writeFile()
{

}

void writeFile::allTrue() {
	if (std::filesystem::exists("\items_script_stats")) {
		std::filesystem::current_path("\items_script_stats");
	}
	else {
		MessageBox(wind, L"Could not find directory for 'items_script_stats'.", L"Error", MB_ICONERROR);
	}
	preprocess();
}

void writeFile::noScript() {
	if (std::filesystem::exists("\items_stats")) {
		std::filesystem::current_path("\items_stats");
	}
	else {
		MessageBox(wind, L"Could not find directory for 'items_stats'.", L"Error", MB_ICONERROR);
	}
	preprocess();
}

void writeFile::noItems() {
	if (std::filesystem::exists("\script_stats")) {
		std::filesystem::current_path("\script_stats");
	}
	else {
		MessageBox(wind, L"Could not find directory for 'script_stats'.", L"Error", MB_ICONERROR);
	}
	preprocess();
}

bool writeFile::ReadVirtualTOC(CFile* romFile)
{
	BYTE pBuffer[2048 * 16];
	UINT count;
	byte entry[7];
	TOCEntry FInfo;


	ReadSector(romFile, pBuffer, 24, 16);


	for (count = 0; count < 2048 * 16 / 7; count++) {
		memcpy(entry, pBuffer + 7 * count, 7);


		FInfo.dwStartSector = entry[0] | (entry[1] << 8) | (entry[2] << 16);
		FInfo.nLength = entry[3] | (entry[4] << 8) | (entry[5] << 16) | (entry[6] << 24);
		if (FInfo.dwStartSector == 0xffffff) {
			break;
		}
	    arTOC.Add(FInfo);
	}

	return true;
}

bool writeFile::process(CFile *romFile, CFile *statsFile, UINT nPosition) {
	byte pBuffer[2048 * 16];
	UINT nNum = 0;
	ReadSector(romFile, pBuffer, 24, 16);
	DWORD fileLength = statsFile->GetLength();
	memcpy(pBuffer + (7 * nPosition) + 3, &fileLength, 4);
	WriteSector(romFile, pBuffer, 24, 16);
	ReadSector(romFile, pBuffer, 108607, 16);
	memcpy(pBuffer + 4 + (7 * nPosition) + 3, &fileLength, 4);
	WriteSector(romFile, pBuffer, 108607, 16);
	byte FileBuff[32 * 2048];
	UINT i = 0;
	DWORD LastOff = 0;
	while (statsFile->GetPosition() + 32 * 2048 < fileLength) {
		memset(FileBuff, 0, 32 * 2048);
		statsFile->Read(FileBuff, 32 * 2048);
		WriteSector(romFile, FileBuff, arTOC[nPosition].dwStartSector + i * 32, 32);
		i++;
	}
	LastOff = statsFile->GetPosition();
	if (LastOff < fileLength) {
		memset(FileBuff, 0, 32 * 2048);
		statsFile->Read(FileBuff, fileLength - LastOff);

		UINT nRemSect = (fileLength - LastOff) / 2048;
		if ((fileLength - LastOff) % 2048 != 0) {
			nRemSect++;
		}
		WriteSector(romFile, FileBuff, arTOC[nPosition].dwStartSector + i * 32, nRemSect);
	}
	return true;
}

bool writeFile::ReadSector(CFile *romFile, byte *pBuffer, DWORD dwStart, UINT NumOfSectors) {
	byte bySector[2048];
	UINT i = 0;

	for (i = 0; i < NumOfSectors; i++) {
		romFile->Seek((dwStart + i) * 2352 + 24, CFile::begin); 
		romFile->Read(bySector, 2048);
		memcpy(pBuffer + i * 2048, bySector, 2048);
	}
	return true;
}

bool writeFile::WriteSector(CFile *romFile, byte *pBuffer, DWORD dwStart, UINT NumOfSectors) {
	romFile->Seek(dwStart * 2352 + 24, CFile::begin);
	for (UINT i = 0; i < NumOfSectors; i++) {
		romFile->Write(pBuffer + (i * 2048), 2048);
		romFile->Seek(304, CFile::current);
	}
	return true;
}

void writeFile::preprocess() {
	CString iso;
	CString file;
	UINT nPos = std::stoi("2595");
	if (discNum == 2) {
		nPos -= 5;
	}
	iso = discName.c_str();
	file = "2595";
	CFile romFile;
	CFile statsFile;
	romFile.Open(iso, CFile::modeReadWrite);
	ReadVirtualTOC(&romFile);
	statsFile.Open(file, CFile::modeRead);
	if ((statsFile.GetLength() / 2048) != (arTOC[nPos].nLength / 2048)) {
		MessageBox(wind, L"Invalid file.", L"Error", MB_ICONERROR);
	}
	else {
		process(&romFile, &statsFile, nPos);
	}
	romFile.Close();
	statsFile.Close();
}