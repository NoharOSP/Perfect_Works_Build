#pragma once
#include "pch.h"
#include <string>
#include <iostream>
#include <fstream>
#include "Window.h"
#include "patchProcessor.h"

class patchProcessor;
class Window;

class makeSLUS
{
	// Global methods
public:
	makeSLUS(std::string romFile, int num, patchProcessor* pp, Window* pWin);
	~makeSLUS();
};

