#pragma once
#include "Window.h"
#include "patchProcessor.h"

class Window;
class patchProcessor;

class gameplayVerifier
{
	// Global methods
public:
	gameplayVerifier();
	~gameplayVerifier();
	bool monsterVerify(Window* pWin, patchProcessor* pp);
	bool expVerify(Window* pWin, patchProcessor* pp);
	bool goldVerify(Window* pWin, patchProcessor* pp);
	bool encounterVerify(Window* pWin, patchProcessor* pp, int num);
	bool itemSpellVerify(Window* pWin, patchProcessor* pp, int num);
	bool deathblowVerify(Window* pWin, patchProcessor* pp);
};

