#pragma once
#include "pch.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include "Resource.h"
#include "Window.h"
#include "patchVerifier.h"

class Window;

class patchProcessor
{
	// Global methods
public:
	patchProcessor(Window* win, HWND hWnd, int discNum, std::string path);
	~patchProcessor();
	void prepare();
	void removeWhitespace();
	void titleVerify();
	void start();
	void initialisePatchLists();
	void finish();
	void exeEdits(std::string file);
	void monsterEdits(std::string file);
	void battleExeEdits(std::string file);
	void reinitialisePatches();
	void clearPatchLists();
	bool gamefileVerify();
	bool oneDriveCheck();
	bool applyPatch();

	// Global variables
public:
	HWND patchWnd;
	bool changed = false;
	bool space = false;
	bool filePathValid = false;
	bool successMessage = false;
	int num;
	std::string tempPath;
	std::string filePath;
	std::string gamefilePath = "\gamefiles";
	std::string encountersName = "";
	std::string expName = "";
	std::string fastName = "";
	std::string fmvName = "";
	std::string roniName = "";
	std::string itemspellsName = "";
	std::string monsterName = "";
	std::string portraitsName = "";
	std::string scriptName = "";
	std::string arenaName = "";
	std::string graphicsName = "";
	std::string bugName = "";
	std::string titleName = "";
	std::string voiceName = "";
	std::string flashesName = "";
	std::string storyModeName = "";
	std::string goldName = "";
	std::string fmvPatch = "";
	std::string cafeName = "";
	std::string temp = "temp";
	std::string exeName = "executable";
	std::string slusDisc1 = "sub_executable\\disc1";
	std::string slusDisc2 = "sub_executable\\disc2";
	Window* pWin;
	std::vector<std::string> patchList;
	std::ofstream cue_stream;
};

