#pragma once
#include "Window.h"
#include "patchProcessor.h"

class Window;
class patchProcessor;

class patchVerifier
{
	// Global methods
public:
	patchVerifier(Window* win, patchProcessor* process, int discNum);
	~patchVerifier();
	void fmvVerify();
	void verify();
	void titleVerify();
	bool storyModeVerify();
	bool monsterVerify();
	bool expVerify();
	bool goldVerify();
	bool encounterVerify();
	bool itemSpellVerify();
	bool fastVerify();
	bool scriptVerify();
	bool arenaVerify();
	bool basicVerify();
	bool expertVerify();
	bool portraitsVerify();
	bool graphicsVerify();
	bool voiceVerify();
	bool roniVerify();
	bool cafeVerify();
	bool bugVerify();

	// Global variables
public:
	Window* pWin;
	std::string patchPath = "\patches";
	bool patchPathValid = false;
	bool item = false;
	int num;
	patchProcessor* pp;
};

