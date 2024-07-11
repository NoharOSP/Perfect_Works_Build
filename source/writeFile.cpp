#include "pch.h"
#include "writeFile.h"



writeFile::writeFile(HWND hWnd, std::string home, std::string cd, int num, bool itemsspells, bool script, bool exp_gold, bool monsters, bool encounters, bool fastnew, bool barena, bool earena, bool portraits, bool music, bool fastold)
{
	dir = home;
	discName = cd;
	discNum = num;
	wind = hWnd;
	wf_itemspells = itemsspells;
	wf_script = script;
	wf_expgold = exp_gold;
	wf_monsters = monsters;
	wf_encounters = encounters;
	wf_fast = fastnew;
	wf_barena = barena;
	wf_earena = earena;
	wf_portraits = portraits;
	wf_music = music;
	wf_fastold = fastold;
	// Move to the "shared_files" directory
	std::filesystem::current_path(dir);
	if (std::filesystem::exists("\patches")) {
		std::filesystem::current_path("\patches");
		if (std::filesystem::exists("\shared_files")) {
			std::filesystem::current_path("\shared_files");
		}
		else {
			MessageBox(wind, L"Could not find directory for 'shared_files'. Check repo for latest version.", L"Error", MB_ICONERROR);
		}
	}
	else {
		MessageBox(wind, L"Could not find directory for 'patches'. Check repo for latest version.", L"Error", MB_ICONERROR);
	}
	// Determine if both exp/gold and monsters have been ticked.
	if (wf_expgold && wf_monsters) {
		doExpGoldMonsters();
	}
	else {
		// Determine if the exp/gold patch hasn't been ticked
		if (wf_expgold) {
			doExpGold();
		}
		// Determine if the monsters patch has been ticked
		if (wf_monsters) {
			doMonsters();
		}
	}
	// Determine if the encounters patch has been ticked
	if (wf_encounters) {
		doEncounters();
	}
	// Determine if the fast text patch has been ticked
	if (wf_fast) {
		doFast();
	}
	// Determine if the arena patch has been ticked
	if (wf_barena || wf_earena) {
		doArena();
	}
	// Determine if the portraits patch has been ticked
	if (wf_portraits) {
		doPortraits();
	}
	// Determine if the music patch has been ticked
	if (wf_music) {
		doMusic();
	}
}


writeFile::~writeFile()
{

}

void writeFile::doExpGold() {
	// Find shared files for the items/spells and exp/gold patches
	if (wf_itemspells) {
		goHome();
		if (std::filesystem::exists("\exp_gold_items")) {
			std::filesystem::current_path("\exp_gold_items");
		}
		else {
			MessageBox(wind, L"Could not find directory for 'exp_gold_items'. Check repo for latest version.", L"Error", MB_ICONERROR);
		}
		for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
			std::string fileName = entry.path().string();
			std::string finalName = fileName.substr(fileName.find_last_of("/\\") + 1);
			preprocess(finalName);
		}
	}
	// Find shared files for the script and exp/gold patches
	if (wf_script) {
		goHome();
		if (std::filesystem::exists("\exp_gold_script")) {
			std::filesystem::current_path("\exp_gold_script");
		}
		else {
			MessageBox(wind, L"Could not find directory for 'exp_gold_script'. Check repo for latest version.", L"Error", MB_ICONERROR);
		}
		for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
			std::string fileName = entry.path().string();
			std::string finalName = fileName.substr(fileName.find_last_of("/\\") + 1);
			preprocess(finalName);
		}
	}
	// Find shared files for the script, items/spells and exp/gold patches
	if (wf_script && wf_itemspells) {
		goHome();
		if (std::filesystem::exists("\exp_gold_both")) {
			std::filesystem::current_path("\exp_gold_both");
		}
		else {
			MessageBox(wind, L"Could not find directory for 'exp_gold_both'. Check repo for latest version.", L"Error", MB_ICONERROR);
		}
		for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
			std::string fileName = entry.path().string();
			std::string finalName = fileName.substr(fileName.find_last_of("/\\") + 1);
			preprocess(finalName);
		}
	}
}

void writeFile::doMonsters() {
	// Find shared files for the items/spells and monster patches
	if (wf_itemspells) {
		goHome();
		if (std::filesystem::exists("\monsters_items")) {
			std::filesystem::current_path("\monsters_items");
		}
		else {
			MessageBox(wind, L"Could not find directory for 'monsters_items'. Check repo for latest version.", L"Error", MB_ICONERROR);
		}
		for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
			std::string fileName = entry.path().string();
			std::string finalName = fileName.substr(fileName.find_last_of("/\\") + 1);
			preprocess(finalName);
		}
	}
	// Find shared files for the script and monster patches
	if (wf_script) {
		goHome();
		if (std::filesystem::exists("\monsters_script")) {
			std::filesystem::current_path("\monsters_script");
		}
		else {
			MessageBox(wind, L"Could not find directory for 'monsters_script'. Check repo for latest version.", L"Error", MB_ICONERROR);
		}
		for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
			std::string fileName = entry.path().string();
			std::string finalName = fileName.substr(fileName.find_last_of("/\\") + 1);
			preprocess(finalName);
		}
	}
	// Find shared files for the script, items/spells and monster patches
	if (wf_script && wf_itemspells) {
		goHome();
		if (std::filesystem::exists("\monsters_both")) {
			std::filesystem::current_path("\monsters_both");
		}
		else {
			MessageBox(wind, L"Could not find directory for 'monsters_both'. Check repo for latest version.", L"Error", MB_ICONERROR);
		}
		for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
			std::string fileName = entry.path().string();
			std::string finalName = fileName.substr(fileName.find_last_of("/\\") + 1);
			preprocess(finalName);
		}
	}
}

void writeFile::doFast() {
	// Check for shared files between the script and fast text patches
	if (wf_script) {
		// Mainly applies to disc 1 as disc 2 lacks scenes with auto-advance 
		if (discNum == 1) {
			goHome();
			if (std::filesystem::exists("\speed_one")) {
				std::filesystem::current_path("\speed_one");
				for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
					std::string fileName = entry.path().string();
					std::string finalName = fileName.substr(fileName.find_last_of("/\\") + 1);
					preprocess(finalName);
				}
			}
			else {
				MessageBox(wind, L"Could not find directory for 'speed_one'. Check repo for latest version.", L"Error", MB_ICONERROR);
			}
		}
	}
}

void writeFile::doEncounters() {
	// Check for shared files between the script and encounter rate patches
	if (wf_script) {
		// Check disc number
		if (discNum == 1) {
			goHome();
			if (std::filesystem::exists("\encounterone_script")) {
				std::filesystem::current_path("\encounterone_script");
			}
			else {
				MessageBox(wind, L"Could not find directory for 'encounterone_script'. Check repo for latest version.", L"Error", MB_ICONERROR);
			}
		}
		else if (discNum == 2) {
			goHome();
			if (std::filesystem::exists("\encountertwo_script")) {
				std::filesystem::current_path("\encountertwo_script");
			}
			else {
				MessageBox(wind, L"Could not find directory for 'encountertwo_script'. Check repo for latest version.", L"Error", MB_ICONERROR);
			}
		}
		for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
			std::string fileName = entry.path().string();
			std::string finalName = fileName.substr(fileName.find_last_of("/\\") + 1);
			preprocess(finalName);
		}
	}
}

void writeFile::doArena() {
	// Mainly applies to disc 1. THIS WILL NOT WORK FOR DISC 2
	if (discNum == 1) {
		// Apply data for individual gears
		if (wf_script || wf_itemspells) {
			goHome();
			if (std::filesystem::exists("\kislev_battle")) {
				std::filesystem::current_path("\kislev_battle");
				// Check for shared files between the script and arena patches
				if (wf_script) {
					if (std::filesystem::exists("\Script_gear")) {
						std::filesystem::current_path("\Script_gear");
						for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
							std::string fileName = entry.path().string();
							std::string finalName = fileName.substr(fileName.find_last_of("/\\") + 1);
							preprocess(finalName);
						}
					}
					else {
						MessageBox(wind, L"Could not find directory for 'Script_gear'. Check repo for latest version.", L"Error", MB_ICONERROR);
					}
					goHome();
					std::filesystem::current_path("\kislev_battle");
					if (wf_barena) {
						if (std::filesystem::exists("\Misc_script_basic")) {
							std::filesystem::current_path("\Misc_script_basic");
							for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
								std::string fileName = entry.path().string();
								std::string finalName = fileName.substr(fileName.find_last_of("/\\") + 1);
								preprocess(finalName);
							}
						}
						else {
							MessageBox(wind, L"Could not find directory for 'Misc_script_basic'. Check repo for latest version.", L"Error", MB_ICONERROR);
						}
					}
					else {
						if (std::filesystem::exists("\Misc_script_expert")) {
							std::filesystem::current_path("\Misc_script_expert");
							for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
								std::string fileName = entry.path().string();
								std::string finalName = fileName.substr(fileName.find_last_of("/\\") + 1);
								preprocess(finalName);
							}
						}
						else {
							MessageBox(wind, L"Could not find directory for 'Misc_script_expert'. Check repo for latest version.", L"Error", MB_ICONERROR);
						}
					}
				}
				// Check for shared files between the items/spells and arena patches
				else {
					if (std::filesystem::exists("\Item_gear")) {
						std::filesystem::current_path("\Item_gear");
						for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
							std::string fileName = entry.path().string();
							std::string finalName = fileName.substr(fileName.find_last_of("/\\") + 1);
							preprocess(finalName);
						}
					}
					else {
						MessageBox(wind, L"Could not find directory for 'Item_gear'. Check repo for latest version.", L"Error", MB_ICONERROR);
					}
					goHome();
					std::filesystem::current_path("\kislev_battle");
					if (wf_barena) {
						if (std::filesystem::exists("\Misc_item_basic")) {
							std::filesystem::current_path("\Misc_item_basic");
							for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
								std::string fileName = entry.path().string();
								std::string finalName = fileName.substr(fileName.find_last_of("/\\") + 1);
								preprocess(finalName);
							}
						}
						else {
							MessageBox(wind, L"Could not find directory for 'Misc_item_basic'. Check repo for latest version.", L"Error", MB_ICONERROR);
						}
					}
					else {
						if (std::filesystem::exists("\Misc_item_expert")) {
							std::filesystem::current_path("\Misc_item_expert");
							for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
								std::string fileName = entry.path().string();
								std::string finalName = fileName.substr(fileName.find_last_of("/\\") + 1);
								preprocess(finalName);
							}
						}
						else {
							MessageBox(wind, L"Could not find directory for 'Misc_item_expert'. Check repo for latest version.", L"Error", MB_ICONERROR);
						}
					}
				}
			}
			else {
				MessageBox(wind, L"Could not find directory for 'kislev_battle'. Check repo for latest version.", L"Error", MB_ICONERROR);
			}
		}
	}
}

void writeFile::doPortraits() {
	// Check for shared files between the script and portrait patches
	if (wf_script) {
		// Check disc number
		if (discNum == 1) {
			goHome();
			if (std::filesystem::exists("\portraits")) {
				std::filesystem::current_path("\portraits");
				for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
					std::string fileName = entry.path().string();
					std::string finalName = fileName.substr(fileName.find_last_of("/\\") + 1);
					preprocess(finalName);
				}
			}
			else {
				MessageBox(wind, L"Could not find directory for 'portraits'. Check repo for latest version.", L"Error", MB_ICONERROR);
			}
		}
	}
}

void writeFile::doMusic() {
	// Check for shared files between the music and original script fast text patches
	if (wf_fastold) {
		// Check disc number
		if (discNum == 1) {
			goHome();
			if (std::filesystem::exists("\music_fastold")) {
				std::filesystem::current_path("\music_fastold");
				for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
					std::string fileName = entry.path().string();
					std::string finalName = fileName.substr(fileName.find_last_of("/\\") + 1);
					preprocess(finalName);
				}
			}
			else {
				MessageBox(wind, L"Could not find directory for 'music_fastold'. Check repo for latest version.", L"Error", MB_ICONERROR);
			}
		}
	}
	// Check for shared files between the music and script patches
	if (wf_script) {
		if (discNum == 1) {
			goHome();
			if (std::filesystem::exists("\music_script1")) {
				std::filesystem::current_path("\music_script1");
				for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
					std::string fileName = entry.path().string();
					std::string finalName = fileName.substr(fileName.find_last_of("/\\") + 1);
					preprocess(finalName);
				}
			}
			else {
				MessageBox(wind, L"Could not find directory for 'music_script1'. Check repo for latest version.", L"Error", MB_ICONERROR);
			}
		}
		if (discNum == 2) {
			goHome();
			if (std::filesystem::exists("\music_script2")) {
				std::filesystem::current_path("\music_script2");
				for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
					std::string fileName = entry.path().string();
					std::string finalName = fileName.substr(fileName.find_last_of("/\\") + 1);
					preprocess(finalName);
				}
			}
			else {
				MessageBox(wind, L"Could not find directory for 'music_script2'. Check repo for latest version.", L"Error", MB_ICONERROR);
			}
		}
	}
	// Implement music files directly if items are applied.
	if (wf_itemspells && !wf_script) {
		if (discNum == 1) {
			goHome();
			if (std::filesystem::exists("\music_items")) {
				std::filesystem::current_path("\music_items");
				for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
					std::string fileName = entry.path().string();
					std::string finalName = fileName.substr(fileName.find_last_of("/\\") + 1);
					preprocess(finalName);
				}
			}
			else {
				MessageBox(wind, L"Could not find directory for 'music_items'. Check repo for latest version.", L"Error", MB_ICONERROR);
			}
		}
	}
}

void writeFile::doExpGoldMonsters() {
	// Without items or script
	if (!wf_itemspells && !wf_script) {
		goHome();
		if (std::filesystem::exists("\exp_monster")) {
			std::filesystem::current_path("\exp_monster");
		}
		else {
			MessageBox(wind, L"Could not find directory for 'exp_monste'. Check repo for latest version.", L"Error", MB_ICONERROR);
		}
		for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
			std::string fileName = entry.path().string();
			std::string finalName = fileName.substr(fileName.find_last_of("/\\") + 1);
			preprocess(finalName);
		}
	}
	// Find shared files for the exp/gold, monster and item patches
	if (wf_itemspells) {
		goHome();
		if (std::filesystem::exists("\exp_monster_items")) {
			std::filesystem::current_path("\exp_monster_items");
		}
		else {
			MessageBox(wind, L"Could not find directory for 'exp_monster_items'. Check repo for latest version.", L"Error", MB_ICONERROR);
		}
		for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
			std::string fileName = entry.path().string();
			std::string finalName = fileName.substr(fileName.find_last_of("/\\") + 1);
			preprocess(finalName);
		}
	}
	// Find shared files for the script, monster and exp/gold patches
	if (wf_script) {
		goHome();
		if (std::filesystem::exists("\exp_monster_script")) {
			std::filesystem::current_path("\exp_monster_script");
		}
		else {
			MessageBox(wind, L"Could not find directory for 'exp_monster_script'. Check repo for latest version.", L"Error", MB_ICONERROR);
		}
		for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
			std::string fileName = entry.path().string();
			std::string finalName = fileName.substr(fileName.find_last_of("/\\") + 1);
			preprocess(finalName);
		}
	}
	// Find shared files for the script, items/spells, monster and exp/gold patches
	if (wf_script && wf_itemspells) {
		goHome();
		if (std::filesystem::exists("\exp_monster_both")) {
			std::filesystem::current_path("\exp_monster_both");
		}
		else {
			MessageBox(wind, L"Could not find directory for 'exp_monster_both'. Check repo for latest version.", L"Error", MB_ICONERROR);
		}
		for (const auto& entry : std::filesystem::directory_iterator(std::filesystem::current_path())) {
			std::string fileName = entry.path().string();
			std::string finalName = fileName.substr(fileName.find_last_of("/\\") + 1);
			preprocess(finalName);
		}
	}
}

// Find TOC in the ROM
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

// Processes files to read and write their sectors
bool writeFile::process(CFile *romFile, CFile *statsFile, UINT nPosition) {
	byte pBuffer[2048 * 16];
	UINT nNum = 0;
	// Write the index
	ReadSector(romFile, pBuffer, 24, 16);
	DWORD fileLength = statsFile->GetLength();
	memcpy(pBuffer + (7 * nPosition) + 3, &fileLength, 4);
	WriteSector(romFile, pBuffer, 24, 16);
	// Write the index into the exe
	ReadSector(romFile, pBuffer, 108607, 16);
	memcpy(pBuffer + 4 + (7 * nPosition) + 3, &fileLength, 4);
	WriteSector(romFile, pBuffer, 108607, 16);
	// Write the file
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
		// Write the last section of the file
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
	// Puts sectors from dwStart to NumOfSectors into pBuffer
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

// Validate and prepare file
void writeFile::preprocess(std::string fileName) {
	CString iso;
	CString file;
	UINT nPos = std::stoi(fileName);
	if (discNum == 2) {
		nPos -= 5;
	}
	iso = discName.c_str();
	file = fileName.c_str();
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

// Return to "shared_files" directory
void writeFile::goHome() {
	std::filesystem::current_path(dir);
	if (std::filesystem::exists("\patches")) {
		std::filesystem::current_path("\patches");
		if (std::filesystem::exists("\shared_files")) {
			std::filesystem::current_path("\shared_files");
		}
		else {
			MessageBox(wind, L"Could not find directory for 'shared_files'. Check repo for latest version.", L"Error", MB_ICONERROR);
		}
	}
}