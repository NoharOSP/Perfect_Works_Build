#pragma once
#include <string>
#include <filesystem>
#include <iostream>
#include <fstream> 
#include <sstream>

typedef struct {
	DWORD	dwStartSector;
	UINT	nLength;
}TOCEntry;

typedef CArray<TOCEntry, TOCEntry> CTOCArray;

class writeFile
{
public:
	writeFile(std::string home, std::string cd, int num, bool itemsspells, bool script, bool stats);
	virtual ~writeFile();
	CTOCArray arTOC;
	void allTrue();
	void noScript();
	bool process(std::string discNum, std::string gameFile, UINT nPosition);
	bool ReadSector(char *pBuffer, DWORD dwStart, UINT NumOfSectors);
	bool WriteSector(char *pBuffer, DWORD dwStart, UINT NumOfSectors);
public:
	std::string dir;
	std::string discName;
	int discNum;
	std::fstream romFile;
	std::ifstream statsFile;
};

