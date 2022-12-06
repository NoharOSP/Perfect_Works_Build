#pragma once
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream> 
#include <sstream>
#include <iostream>
#include <bitset>

class patchChecker
{
public:
	patchChecker();
	virtual ~patchChecker();
	bool encountersCheck(std::string path);
	bool expgoldCheck(std::string path);
	bool fastTextCheck(std::string path, std::string version);
	bool itemsCheck(std::string path, std::string version);
	bool monstersCheck(std::string path);
	bool scriptCheck(std::string path);
	bool statsCheck(std::string path);
	void writeByte(int num);
	void initialise();
public:
	unsigned char buffer;
	bool found;
	int maxByte = 0;
	std::fstream file;
	int byte = -1;
};

