#pragma once
#include <shellapi.h>
#include <commdlg.h>
#include <iostream>
#include <filesystem>
#include <fstream> 
#include <sstream>
#include <string>
#include <bitset>

class romFinder
{
public:
	romFinder();
	~romFinder();
	void searchCD(std::string path);
	bool getFound();
	int getDisc();
private:
	std::string name;
public:
	OPENFILENAMEA ofn;
	bool xenoFound = false;
	bool discFound = false;
	int discNum = 0;
	unsigned char buffer;
};

