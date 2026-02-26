#pragma once
#include "Window.h"
#include "patchProcessor.h"
#include "graphicsVerifier.h"
#include "gameplayVerifier.h"
#include "arenaVerifier.h"
#include "storyVerifier.h"

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
	void jpnVerify();
	bool storyModeVerify();
	bool voiceVerify();
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

