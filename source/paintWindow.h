#pragma once
#include "Window.h"

class Window;
class paintWindow
{
	// Global methods
public:
	paintWindow(Window* win);
	~paintWindow();
	void paint();
	void drawText();

	// Global variables
public:
	Window* pWin;
	HDC hdc;
	RECT rcWindow;
	TCHAR cd1text[256];
	TCHAR cd2text[256];
	TCHAR graphicstext[256];
	TCHAR gameplaytext[256];
	TCHAR storytext[256];
	TCHAR arenatext[256];
	TCHAR audiotext[256];
	TCHAR modestext[256];
	TCHAR dummy[256];
};

