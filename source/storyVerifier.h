#pragma once
#include "Window.h"
#include "patchProcessor.h"

class Window;
class patchProcessor;

class storyVerifier
{
	// Global methods
public:
	storyVerifier();
	~storyVerifier();
	bool fastVerify(Window* pWin, patchProcessor* pp, int num);
	bool scriptVerify(Window* pWin, patchProcessor* pp, int num);
};

