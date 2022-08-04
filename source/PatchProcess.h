#pragma once
#include <string>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <filesystem>


class PatchProcess
{
public:
	PatchProcess(HWND hWnd, std::string file1, std::string file2);
	~PatchProcess();
	void ApplyPatch(int ppf, int bin, char mode);
	int ShowFileId(int ppf);
	int OpenFiles(std::string file1, std::string file2);
public:
	int ppf;
	int bin;
	char mode;
	char binblock[1024];
	char ppfblock[1024];
	unsigned char ppfmem[512];
	HWND wind;
#define APPLY 1
#define UNDO 2
};

