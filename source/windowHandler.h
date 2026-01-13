#pragma once
#include "Window.h"
#include "handleScript.h"
#include "handleGameplay.h"
#include "handleGraphics.h"

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
	void checkFMV();
	void checkVoice();
	void checkStoryMode();

	// Global variables
public:
	Window* pWin;
	bool ticked = false;
};

