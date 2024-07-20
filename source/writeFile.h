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
	// Global methods
public:
	writeFile(HWND hWnd, std::string home, std::string cd, int num, bool itemsspells, bool script, bool expgold, bool monsters, bool encounters, bool fastnew, bool barena, bool earena, bool portraits, bool fastold, bool graphics);
	virtual ~writeFile();
	CTOCArray arTOC;
	void doExpGold();
	void doMonsters();
	void doEncounters();
	void doFast();
	void doArena();
	void doPortraits();
	void doExpGoldMonsters();
	void doGraphics();
	void preprocess(std::string fileName);
	void goHome();
	bool process(CFile *romFile, CFile *statsFile, UINT nPosition);
	bool ReadSector(CFile *romFile, byte *pBuffer, DWORD dwStart, UINT NumOfSectors);
	bool WriteSector(CFile *romFile, byte *pBuffer, DWORD dwStart, UINT NumOfSectors);
	bool ReadVirtualTOC(CFile* romFile);
	// Global variables
public:
	std::string dir;
	std::string discName;
	int discNum;
	HWND wind;
	bool wf_itemspells;
	bool wf_script;
	bool wf_expgold;
	bool wf_monsters;
	bool wf_encounters;
	bool wf_fast;
	bool wf_barena;
	bool wf_earena;
	bool wf_portraits;
	bool wf_fastold;
	bool wf_graphics;
};

