#pragma once
#include "windowHandler.h"
#include "Window.h"

class windowHandler;
class Window;

class handleGraphics
{
	// Global methods
public:
	handleGraphics();
	~handleGraphics();
	void checkResize(Window* pWin, windowHandler* wh);
	void checkPortraits(Window* pWin, windowHandler* wh);
	void checkRoni(Window* pWin, windowHandler* wh);
	void checkFlash(Window* pWin, windowHandler* wh);
	void checkCafe(Window* pWin, windowHandler* wh);
};

