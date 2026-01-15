#pragma once
#include "pch.h"
#include <string>
#include <iostream>
#include <fstream>
#include "Resource.h"
#include "patchProcessor.h"
#include "Window.h"

class patchProcessor;
class Window;

class fileEditor
{
	// Global methods
public:
	fileEditor(patchProcessor* processor, Window* window, int discNum, std::string temp);
	~fileEditor();
	void monsterEdits(std::string file);
	void iterateMonster(std::string file);
	void exeEdits(std::string file);
	void battleExeEdits(std::string file);
	void editSLUS(std::string romFile);
	void setMonsterData(int data[], bool gear);
	void editExp(std::fstream* fileContents, int i, int nextpos, wchar_t buffer, int data[]);
	void editGold(std::fstream* fileContents, int i, int nextpos, wchar_t buffer, int data[]);
	void makeSLUS(std::string romFile);
	std::string fileTrim(std::string file);

	// Global variables
public:
	patchProcessor* pp;
	int num;
	Window* pWin;
	std::string tempDir;
};

