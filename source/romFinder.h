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
	// Global methods
public:
	romFinder();
	~romFinder();
	void searchCD(std::string path);
	bool getFound();
	int getDisc();
	// Private variables
private:
	std::string name;
	// Global variables
public:
	OPENFILENAMEA ofn;
	bool xenoFound = false;
	bool discFound = false;
	int discNum = 0;
	int fileSize = 0;
	unsigned char buffer;
};

