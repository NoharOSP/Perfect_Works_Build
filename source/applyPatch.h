#pragma once
#include "pch.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include "Resource.h"
#include "Window.h"
#include "patchProcessor.h"

class Window;
class patchProcessor;

class applyPatch
{
	// Global methods
public:
	applyPatch(Window* win, int discNum, patchProcessor* processor);
	~applyPatch();
	bool patch();
	bool verifyPatch();
	void initialise();
	void checkFile();
	void createFiles();
	void popTemp();
	void exeEdits(std::string file);
	void monsterEdits(std::string file);
	void battleExeEdits(std::string file);
	void applyFMV();
	void executeBat();
	void editSLUS();
	void cleanup();
	void makeCue();

	// Global variables
public:
	std::string fileName;
	std::string oldPath;
	std::string cueName;
	std::string cdName;
	std::string temp = "temp";
	bool patched = false;
	bool changed = false;
	Window* pWin;
	int num;
	patchProcessor* pp;
	std::ofstream list_file;
	std::ofstream batch_file;
	std::ofstream cue_stream;
};

