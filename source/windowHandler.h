#pragma once
#include "Window.h"

class Window;

class windowHandler
{
	// Global methods
public:
	windowHandler(Window* win);
	~windowHandler();
	bool check();
	void checkScript();
	void checkEnc();
	void checkFast();
	void checkExpOne();
	void checkExpTwo();
	void checkGoldOne();
	void checkGoldTwo();
	void checkItemsParty();
	void checkMonsters();
	void checkNormArena();
	void checkBasicArena();
	void checkExpArena();
	void checkPortraits();
	void checkFMV();
	void checkGraphics();
	void checkVoice();
	void checkRoni();
	void checkFlash();
	void checkCafe();
	void checkStoryMode();

	// Global variables
public:
	Window* pWin;
	bool ticked;
};

