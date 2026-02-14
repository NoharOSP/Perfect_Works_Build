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
};

