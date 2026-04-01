#pragma once
#include <string>
#include <fstream>
#include <filesystem>
#include "helper/gameFileTools.h"

class audioEditor
{
	// Global methods
public:
	static void musicEdits(std::string file);
	static void writeEdits(std::string file, int subLength, std::vector<char> contents);
	// Global variables
public:
	inline static std::string musicSubFiles = "music_subfiles";
};

