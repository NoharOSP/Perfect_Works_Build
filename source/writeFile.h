#pragma once
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream> 
#include <sstream>
#include "framework.h"

typedef struct {
	DWORD	dwStartSector;
	UINT	nLength;
}TOCEntry;

typedef CArray<TOCEntry, TOCEntry> CTOCArray;

class writeFile
{
public:
	writeFile(HWND hWnd, std::string home, std::string cd, int num, bool itemsspells, bool script, bool stats, bool expgold, bool monsters, bool encounters);
	virtual ~writeFile();
	CTOCArray arTOC;
	void allTrue();
	void noScript();
	void noItems();
	void doExpGold();
	void doMonsters();
	void doEncounters();
	void preprocess(std::string fileName);
	bool process(CFile *romFile, CFile *statsFile, UINT nPosition);
	bool ReadSector(CFile *romFile, byte *pBuffer, DWORD dwStart, UINT NumOfSectors);
	bool WriteSector(CFile *romFile, byte *pBuffer, DWORD dwStart, UINT NumOfSectors);
	bool ReadVirtualTOC(CFile* romFile);
public:
	std::string dir;
	std::string discName;
	int discNum;
	HWND wind;
};

