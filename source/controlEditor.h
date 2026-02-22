#pragma once
#include "pch.h"
#include "helper/gameFileTools.h"
#include "Window.h"
#include <string>
#include <filesystem>

class Window;

class controlEditor
{
	// Global methods
public:
	static void addImage(std::string file);
	static void editData(std::string trimfile);
	static void editBattleFile(std::string trimfile);
	static void editExecutable(std::string file);

	// Global variables
public:
	inline static std::string exediff = "data\\controls\\0022.csv";
};

