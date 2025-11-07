#pragma once
class toolGenerator
{
	// Global methods
public:
	toolGenerator(char* text, HWND hWnd, HWND hText, HINSTANCE hInst);
	~toolGenerator();
	HWND CreateToolTip(HWND hParent, HWND hText, HINSTANCE hInst, PTSTR pszText);

	// Global variables
public:
	TOOLINFO toolInfo;
	HWND hWndTT;
};

