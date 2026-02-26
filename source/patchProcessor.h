#pragma once
#include "pch.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include "Resource.h"
#include "Window.h"
#include "patchVerifier.h"
#include "applyPatch.h"

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
	void reinitialisePatches();
	void clearPatchLists();
	bool gamefileVerify();
	bool oneDriveCheck();

	// Global variables
public:
	HWND patchWnd;
	bool space = false;
	bool filePathValid = false;
	bool successMessage = false;
	int num;
	std::string tempPath;
	std::string filePath;
	std::string encountersName = "";
	std::string expName = "";
	std::string fastName = "";
	std::string fmvName = "";
	std::string roniName = "";
	std::string itemspellsName = "";
	std::string monsterName = "";
	std::string resizeName = "";
	std::string scriptName = "";
	std::string arenaName = "";
	std::string portraitsName = "";
	std::string bugName = "";
	std::string titleName = "";
	std::string voiceName = "";
	std::string flashesName = "";
	std::string storyModeName = "";
	std::string goldName = "";
	std::string fmvPatch = "";
	std::string cafeName = "";
	inline static std::string exeName1 = "executable\\cd1";
	inline static std::string exeName2 = "executable\\cd2";
	std::string deathblowName = "";
	std::string jpnName = "";
	std::string slusDisc1 = "sub_executable\\disc1";
	std::string slusDisc2 = "sub_executable\\disc2";
	inline static std::string gamefilePath = "\gamefiles";
	Window* pWin;
	std::vector<std::string> patchList;
};

