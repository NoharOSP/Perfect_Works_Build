#pragma once
#include "pch.h"
#include <string>
#include <iostream>
#include <fstream>
#include "Resource.h"
#include "patchProcessor.h"
#include "Window.h"
#include "makeSLUS.h"
#include "partyStatEditor.h"
#include "helper/gameFileTools.h"

class patchProcessor;
class Window;

class fileEditor
{
	// Global methods
public:
	fileEditor(patchProcessor* processor, Window* window, int discNum, std::string temp);
	~fileEditor();
	void exeEdits(std::string file);
	void editSLUS(std::string romFile);
	void editTextSpeed(std::string file);
	void expRateEdits(std::string file);

	// Global variables
public:
	patchProcessor* pp;
	int num;
	Window* pWin;
	std::string tempDir;
};

