#include "pch.h"
#include "writeFile.h"


writeFile::writeFile(std::string home, std::string cd, int num, bool itemsspells, bool script, bool stats)
{
	dir = home;
	discName = cd;
	discNum = num;
	std::filesystem::current_path(dir);
	std::filesystem::current_path("\patches");
	std::filesystem::current_path("\shared_files");
	if (itemsspells && script && stats) {
		allTrue();
	}
	else if (itemsspells && stats) {
		noScript();
	}
}


writeFile::~writeFile()
{

}

void writeFile::allTrue() {
	std::filesystem::current_path("\items_script_stats");
	if (discNum == 1) {
		process(discName, "2595", 2595);
	}
	else if (discNum == 2) {
		process(discName, "2595", 2595 - 5);
	}
}

void writeFile::noScript() {
	std::filesystem::current_path("\items_stats");
	if (discNum == 1) {
		process(discName, "2595", 2595);
	}
	else if (discNum == 2) {
		process(discName, "2595", 2595 - 5);
	}
}

bool writeFile::process(std::string discNum, std::string gameFile, UINT nPosition) {
	romFile.open(discNum, std::ios::binary);
	statsFile.open(gameFile, std::ios::binary);
	char pBuffer[2048 * 16];
	UINT nNum = 0;
	ReadSector(pBuffer, 24, 16);
	DWORD fileLength = statsFile.tellg();
	memcpy(pBuffer + (7 * nPosition) + 3, &fileLength, 4);
	WriteSector(pBuffer, 24, 16);
	char FileBuff[32 * 2048];
	UINT i = 0;
	DWORD LastOff = 0;
	int filePos = statsFile.tellg();
	while (filePos + 32 * 2048 < fileLength) {
		memset(FileBuff, 0, 32 * 2048);
		statsFile.read(FileBuff, 32 * 2048);
		WriteSector(FileBuff, arTOC[nPosition].dwStartSector + i * 32, 32);
		i++;
		filePos = statsFile.tellg();
	}
	LastOff = statsFile.tellg();
	if (LastOff < fileLength) {
		memset(FileBuff, 0, 32 * 2048);
		statsFile.read(FileBuff, fileLength - LastOff);

		UINT nRemSect = (fileLength - LastOff) / 2048;
		if ((fileLength - LastOff) % 2048 != 0)
			nRemSect++;
		WriteSector(FileBuff, arTOC[nPosition].dwStartSector + i * 32, nRemSect);
	}
}

bool writeFile::ReadSector(char *pBuffer, DWORD dwStart, UINT NumOfSectors) {
	char bySector[2048];
	UINT i = 0;

	for (i = 0; i < NumOfSectors; i++) {
		romFile.seekg((dwStart + i) * 2352 + 24, romFile.beg); 
		romFile.read(bySector, 2048);
		memcpy(pBuffer + i * 2048, bySector, 2048);
	}
	return true;
}

bool writeFile::WriteSector(char *pBuffer, DWORD dwStart, UINT NumOfSectors) {
	romFile.seekg(dwStart * 2352 + 24, romFile.beg);
	for (UINT i = 0; i < NumOfSectors; i++) {
		romFile.write(pBuffer + (i * 2048), 2048);
		romFile.seekg(304, romFile.cur);
	}
	return true;
}