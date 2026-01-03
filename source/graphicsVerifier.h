#pragma once
#include "Window.h"
#include "patchProcessor.h"

class Window;
class patchProcessor;

class graphicsVerifier
{
	// Global methods
public:
	graphicsVerifier();
	~graphicsVerifier();
	bool portraitsVerify(Window* pWin, patchProcessor* pp);
	bool graphicsVerify(Window* pWin, patchProcessor* pp);
	bool roniVerify(Window* pWin, patchProcessor* pp);
	bool cafeVerify(Window* pWin, patchProcessor* pp, int num);
};

