#pragma once
#include "pch.h"
#include "Resource.h"
#include <filesystem>
#include <iostream>
#include <fstream>

class partyStatEditor
{
	// Global methods
public:
	partyStatEditor();
	~partyStatEditor();
	void deathblowLevels();
	void popData(int dbData[]);
};

