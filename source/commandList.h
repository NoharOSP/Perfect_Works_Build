#pragma once
#include "windows.h"
#include <iostream>

typedef struct {
	int num;
	char path[100];
}FILE_ENTRY;

class commandList
{
public:
	int m_nCdType;
	FILE_ENTRY* getNextFile(FILE_ENTRY* file);
	int setList(std::string filename, int cdnum);
	commandList();
	virtual ~commandList();
private:
	FILE* list;
	char curpath[100];
protected:
	int m_bFix;
	int m_inDir;
public:
	WIN32_FIND_DATA fd;
	HANDLE hFind;
};
