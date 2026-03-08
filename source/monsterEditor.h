#pragma once
#include "pch.h"
#include "helper/gameFileTools.h"
#include "patchProcessor.h"
#include <string>
#include <fstream>

class monsterEditor
{
	// Public methods
public:
	static void verifyFiles();
	static void editMonster(std::string file);
	static void setMonsterData(int data[], bool gear);
	static void editExp(std::fstream* fileContents, int i, int nextpos, wchar_t buffer, int data[]);
	static void editGold(std::fstream* fileContents, int i, int nextpos, wchar_t buffer, int data[]);
};

