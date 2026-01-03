#pragma once
#include "Window.h"
#include "patchProcessor.h"

class arenaVerifier
{
	// Global methods
public:
	arenaVerifier(Window* pWin, patchProcessor* pp);
	~arenaVerifier();
	bool basicVerify(Window* pWin, patchProcessor* pp);
	bool expertVerify(Window* pWin, patchProcessor* pp);
	bool arenaFound(bool arena);
};

