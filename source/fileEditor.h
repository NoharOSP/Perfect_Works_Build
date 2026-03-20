#pragma once
#include "pch.h"
#include <string>
#include <iostream>
#include <fstream>
#include "applyPatch.h"
#include "Resource.h"
#include "partyStatEditor.h"
#include "helper/gameFileTools.h"

class fileEditor
{
	// Global methods
public:
	static void editSLUS(std::string romFile);
	static void makeSLUS(std::string romFile);
	static void exeEdits(std::string file);
	static void editTextSpeed(std::string file);
	static void expRateEdits(std::string file);
};

