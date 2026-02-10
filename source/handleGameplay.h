#pragma once
#include "windowHandler.h"
#include "Window.h"

class windowHandler;
class Window;

class handleGameplay
{
	// Global methods
public:
	handleGameplay();
	~handleGameplay();
	void checkEnc(Window* pWin, windowHandler* wh);
	void checkExpOne(Window* pWin, windowHandler* wh);
	void checkExpTwo(Window* pWin, windowHandler* wh);
	void checkGoldOne(Window* pWin, windowHandler* wh);
	void checkGoldTwo(Window* pWin, windowHandler* wh);
	void checkItemsParty(Window* pWin, windowHandler* wh);
	void checkMonsters(Window* pWin, windowHandler* wh);
	void checkDeathblows(Window* pWin, windowHandler* wh);
};

