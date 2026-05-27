#pragma once
#include "Window.h"

class Window;

class windowPainter
{
	// Global methods
public:
	static void createWindows(Window* win);
	static void initialiseFont();
	static void graphicWindows();
	static void gameplayWindows();
	static void arenaWindows();
	static void storyWindows();
	static void audioWindows();
	static void modeWindows();
	static void paint();
	static void drawText();
	static void drawDropdown(std::string option);
	
	// Global variables
public:
	inline static Window* pWin;
	inline static HDC hdc;
	inline static RECT rcWindow;
	inline static TCHAR cd1text[256];
	inline static TCHAR cd2text[256];
	inline static TCHAR graphicstext[256];
	inline static TCHAR gameplaytext[256];
	inline static TCHAR storytext[256];
	inline static TCHAR arenatext[256];
	inline static TCHAR audiotext[256];
	inline static TCHAR modestext[256];
	inline static TCHAR dummy[256];
};

