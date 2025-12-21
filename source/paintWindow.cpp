#include "pch.h"
#include "paintWindow.h"

paintWindow::paintWindow(Window* win) {
	pWin = win;
}

paintWindow::~paintWindow() {
}

void paintWindow::paint() {
	PAINTSTRUCT ps;
	pWin->log_file << "Begin drawing window." << std::endl;
	hdc = BeginPaint(pWin->winHwnd, &ps);
	HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SetBkMode(hdc, TRANSPARENT);
	pWin->log_file << "Set font." << std::endl;
	SelectObject(hdc, hFont);
	pWin->log_file << "Draw window." << std::endl;
	FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW));
	GetClientRect(pWin->winHwnd, &rcWindow);
	RECT rc1, rc2;
	rc1, rc2 = rcWindow;
	rc1.top = pWin->winY * 0.025;
	rc1.left = pWin->winX / 50;
	rc1.right = pWin->winX - (rc1.left * 2);
	rc1.bottom = pWin->winY * 0.27;
	rc2.top = pWin->winY * 0.30;
	rc2.left = rc1.left;
	rc2.right = rc1.right;
	rc2.bottom = pWin->winY * 0.85;
	Rectangle(hdc, rc1.left, rc1.top, rc1.right, rc1.bottom);
	Rectangle(hdc, rc2.left, rc2.top, rc2.right, rc2.bottom);
	pWin->log_file << "Draw upper rectangle." << std::endl;
	FillRect(hdc, &rc1, (HBRUSH)(COLOR_WINDOW));
	pWin->log_file << "Draw lower rectangle." << std::endl;
	FillRect(hdc, &rc2, (HBRUSH)(COLOR_WINDOW));
	FrameRect(hdc, &rc1, CreateSolidBrush(RGB(220, 220, 220)));
	FrameRect(hdc, &rc2, CreateSolidBrush(RGB(220, 220, 220)));
	drawText();
	EndPaint(pWin->winHwnd, &ps);
}

void paintWindow::drawText() {
	// Set font
	HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SelectObject(hdc, hFont);
	// Create text
	swprintf_s(cd1text, 256, L"CD1 File:");
	swprintf_s(cd2text, 256, L"CD2 File:");
	swprintf_s(graphicstext, 256, L"Graphics:      ");
	swprintf_s(gameplaytext, 256, L"Gameplay:  ");
	swprintf_s(arenatext, 256, L"Arena:  ");
	swprintf_s(storytext, 256, L"Story:    ");
	swprintf_s(audiotext, 256, L"Audio:    ");
	swprintf_s(modestext, 256, L"Modes:    ");
	// Output text
	TextOut(hdc, pWin->winX * 0.0325, pWin->winY * 0.05, cd1text, wcslen(cd1text));
	TextOut(hdc, pWin->winX * 0.0325, pWin->winY * 0.12, cd2text, wcslen(cd2text));
	TextOut(hdc, pWin->winX * pWin->graphicsx, pWin->winY * 0.32, graphicstext, wcslen(graphicstext));
	TextOut(hdc, pWin->winX * pWin->gameplayx, pWin->winY * 0.32, gameplaytext, wcslen(gameplaytext));
	TextOut(hdc, pWin->winX * pWin->arenax, pWin->winY * 0.32, arenatext, wcslen(arenatext));
	TextOut(hdc, pWin->winX * pWin->storyx, pWin->winY * 0.32, storytext, wcslen(storytext));
	TextOut(hdc, pWin->winX * pWin->audiox, pWin->winY * 0.32, audiotext, wcslen(audiotext));
	TextOut(hdc, pWin->winX * pWin->smx, pWin->winY * 0.32, modestext, wcslen(modestext));
}