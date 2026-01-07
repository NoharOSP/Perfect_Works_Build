#pragma once
#include "windowHandler.h"
#include "Window.h"

class windowHandler;
class Window;

class handleScript
{
	// Global methods
public:
	handleScript();
	~handleScript();
	void checkScript(Window* pWin, windowHandler* wh);
	void checkFast(Window* pWin, windowHandler* wh);
};

