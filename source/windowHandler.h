#pragma once
#include "Window.h"
#include "handleScript.h"
#include "handleGameplay.h"

class Window;

class windowHandler
{
	// Global methods
public:
	windowHandler(Window* win);
	~windowHandler();
	bool check();
	void checkNormArena();
	void checkBasicArena();
	void checkExpArena();
	void checkResize();
	void checkFMV();
	void checkPortraits();
	void checkVoice();
	void checkRoni();
	void checkFlash();
	void checkCafe();
	void checkStoryMode();

	// Global variables
public:
	Window* pWin;
	bool ticked = false;
};

