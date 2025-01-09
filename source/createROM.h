#pragma once
#include <iostream>
#include <tchar.h>
#include "commandList.h"
#include "isoManager.h"
#include "archiveIndex.h"
#include <sstream>

class createROM
{
public:
	createROM(HWND hWnd, int num, std::string oldPath, std::string newPath, std::string listFile);
	int process(commandList cmd, std::string filename);
	unsigned long FindEndOfIndex(unsigned char* index, unsigned long maxsize);
	unsigned long SizeToSectors(unsigned long size, int secsize);
public:
#define TRUE -1;
#define FALSE 0;
	int glb_VideoFiles;
	int glb_FirstNonVideoSector;
	int glb_IndexSector2;
	int glb_debug;
	FILE* log;
	int cdnum;
	std::string discfile;
	std::string destfile;
	HWND window;
};

