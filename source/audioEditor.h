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

	// Global variables
public:
	inline static std::string musicSubFiles = "music_subfiles";
};

