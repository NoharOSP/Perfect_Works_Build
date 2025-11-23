#pragma once
#include "pch.h"
#include <filesystem>
#include <iostream>
#include <fstream>
#include <string>
#include "Resource.h"
#include "Window.h"

class Window;

class patchProcessor
{
	// Global methods
public:
	patchProcessor(Window* win, HWND hWnd, int discNum, std::string path);
	~patchProcessor();
	void prepare();
	void removeWhitespace();
	void fmvVerify();
	void tickedBoxes();
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
	bool storyModeVerify();
	bool monsterVerify();
	bool expVerify();
	bool goldVerify();
	bool encounterVerify();
	bool itemSpellVerify();
	bool fastVerify();
	bool scriptVerify();
	bool arenaVerify();
	bool portraitsVerify();
	bool graphicsVerify();
	bool voiceVerify();
	bool roniVerify();
	bool cafeVerify();
	bool bugVerify();
	bool oneDriveCheck();
	bool applyPatch();

	// Global variables
public:
	HWND patchWnd;
	bool changed = false;
	bool space = false;
	bool patchPathValid = false;
	bool filePathValid = false;
	bool successMessage = false;
	int num;
	std::string tempPath;
	std::string filePath;
	std::string patchPath = "\patches";
	std::string gamefilePath = "\gamefiles";
	std::string encountersName1 = "";
	std::string encountersName2 = "";
	std::string expName = "";
	std::string fastName1 = "";
	std::string fastName2 = "";
	std::string fmvName1 = "";
	std::string fmvName2 = "";
	std::string roniName = "";
	std::string itemspellsName1 = "";
	std::string itemspellsName2 = "";
	std::string monsterName = "";
	std::string portraitsName = "";
	std::string scriptName1 = "";
	std::string scriptName2 = "";
	std::string arenaName = "";
	std::string graphicsName = "";
	std::string bugName1 = "";
	std::string bugName2 = "";
	std::string titleName = "";
	std::string voiceName = "";
	std::string flashesName1 = "";
	std::string flashesName2 = "";
	std::string storyModeName1 = "";
	std::string storyModeName2 = "";
	std::string goldName = "";
	std::string fmvPatch1 = "";
	std::string fmvPatch2 = "";
	std::string cafeName = "";
	std::string temp = "temp";
	std::string exeName = "executable";
	std::string slusDisc1 = "sub_executable\\disc1";
	std::string slusDisc2 = "sub_executable\\disc2";
	Window* pWin;
	std::vector<std::string> patchList;
	std::ofstream cue_stream;
};

