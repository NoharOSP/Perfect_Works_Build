#pragma once
#include "Window.h"
#include <shellapi.h>
#include <commdlg.h>
#include <iostream>
#include <filesystem>
#include <fstream> 
#include <sstream>
#include <string>
#include <bitset>

class Window;

class romFinder
{
	// Global methods
public:
	romFinder(Window* win);
	~romFinder();
	void browseFiles();
	void setPathText(std::string path);
	void romErrorMsg();
	void searchCD(std::string path);
	void findDiscNum(std::string path);
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
	Window* pWin;
};

