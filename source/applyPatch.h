#pragma once
#include "pch.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include "Resource.h"
#include "Window.h"
#include "patchProcessor.h"
#include "fileEditor.h"

class Window;
class patchProcessor;
class fileEditor;

class applyPatch
{
	// Global methods
public:
	applyPatch(Window* win, int discNum, patchProcessor* processor);
	~applyPatch();
	bool patch();
	void prepareFiles();
	void initialise();
	void createTemp();
	void iterateTemp();
	void applyFMV();
	void backupROM();
	void executeBat();
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
	Window* pWin;
	int num;
	patchProcessor* pp;
	std::ofstream list_file;
	std::ofstream batch_file;
	std::ofstream cue_stream;
	fileEditor* pFE;
};