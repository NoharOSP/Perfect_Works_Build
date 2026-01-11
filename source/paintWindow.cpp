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
	graphicWindows();
	gameplayWindows();
	arenaWindows();
	storyWindows();
	audioWindows();
	modeWindows();
	initialiseFont();
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

void paintWindow::initialiseFont() {
	pWin->log_file << "Applying font to windows." << std::endl;
	// Apply font to paths
	for (int i = 0; i < pWin->pathList.size(); i++) {
		SendMessage(pWin->pathList[i], WM_SETFONT, (LPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
	}
	// Apply font to buttons
	for (int i = 0; i < pWin->buttonList.size(); i++) {
		SendMessage(pWin->buttonList[i], WM_SETFONT, (LPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
	}
	// Apply font to each window in windList
	for (int i = 0; i < pWin->windList.size(); i++) {
		SendMessage(pWin->windList[i], WM_SETFONT, (LPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
	}

}

void paintWindow::graphicWindows() {
	// Create graphic patch windows
	pWin->resize = CreateWindow(L"BUTTON", L"Resized portraits", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(pWin->winX * pWin->graphicsx), (int)(pWin->winY * 0.36), 100, 25, pWin->winHwnd, (HMENU)9002, pWin->winInst, NULL);
	pWin->portraits = CreateWindow(L"BUTTON", L"Portrait fixes", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(pWin->winX * pWin->graphicsx), (int)(pWin->winY * 0.43), 100, 25, pWin->winHwnd, (HMENU)9002, pWin->winInst, NULL);
	pWin->flashes = CreateWindow(L"BUTTON", L"No battle flashes", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(pWin->winX * pWin->graphicsx), (int)(pWin->winY * 0.50), 100, 25, pWin->winHwnd, (HMENU)9002, pWin->winInst, NULL);
	pWin->roni = CreateWindow(L"BUTTON", L"PW Roni", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(pWin->winX * pWin->graphicsx), (int)(pWin->winY * 0.57), 100, 25, pWin->winHwnd, (HMENU)9002, pWin->winInst, NULL);
	pWin->cafe = CreateWindow(L"BUTTON", L"Emeralda cafe fix", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(pWin->winX * pWin->graphicsx), (int)(pWin->winY * 0.64), 100, 25, pWin->winHwnd, (HMENU)9002, pWin->winInst, NULL);
	// Put in window list
	pWin->windList.emplace_back(pWin->resize);
	pWin->windList.emplace_back(pWin->portraits);
	pWin->windList.emplace_back(pWin->flashes);
	pWin->windList.emplace_back(pWin->roni);
	pWin->windList.emplace_back(pWin->cafe);
}

void paintWindow::gameplayWindows() {
	// Create gameplay patch windows
	pWin->encounters = CreateWindow(L"BUTTON", L"1/2 encounters", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(pWin->winX * pWin->gameplayx), (int)(pWin->winY * 0.36), 90, 25, pWin->winHwnd, (HMENU)9002, pWin->winInst, NULL);
	pWin->experience1 = CreateWindow(L"BUTTON", L"1.5x exp", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(pWin->winX * pWin->gameplayx), (int)(pWin->winY * 0.43), 110, 25, pWin->winHwnd, (HMENU)9002, pWin->winInst, NULL);
	pWin->experience2 = CreateWindow(L"BUTTON", L"2x exp", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(pWin->winX * pWin->gameplayx), (int)(pWin->winY * 0.50), 110, 25, pWin->winHwnd, (HMENU)9002, pWin->winInst, NULL);
	pWin->gold1 = CreateWindow(L"BUTTON", L"1.5x gold", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(pWin->winX * pWin->gameplayx), (int)(pWin->winY * 0.57), 110, 25, pWin->winHwnd, (HMENU)9002, pWin->winInst, NULL);
	pWin->gold2 = CreateWindow(L"BUTTON", L"2x gold", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(pWin->winX * pWin->gameplayx), (int)(pWin->winY * 0.64), 110, 25, pWin->winHwnd, (HMENU)9002, pWin->winInst, NULL);
	pWin->monsters = CreateWindow(L"BUTTON", L"Rebalanced enemies", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(pWin->winX * pWin->gameplayx), (int)(pWin->winY * 0.71), 120, 25, pWin->winHwnd, (HMENU)9002, pWin->winInst, NULL);
	pWin->itemspells = CreateWindow(L"BUTTON", L"Rebalanced party/items", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(pWin->winX * pWin->gameplayx), (int)(pWin->winY * 0.78), 160, 25, pWin->winHwnd, (HMENU)9002, pWin->winInst, NULL);
	// Put in window list
	pWin->windList.emplace_back(pWin->encounters);
	pWin->windList.emplace_back(pWin->experience1);
	pWin->windList.emplace_back(pWin->experience2);
	pWin->windList.emplace_back(pWin->gold1);
	pWin->windList.emplace_back(pWin->gold2);
	pWin->windList.emplace_back(pWin->itemspells);
	pWin->windList.emplace_back(pWin->monsters);
}

void paintWindow::arenaWindows() {
	// Create arena patch windows
	pWin->normalarena = CreateWindow(L"BUTTON", L"Normal", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, (int)(pWin->winX * pWin->arenax), (int)(pWin->winY * 0.36), 110, 25, pWin->winHwnd, (HMENU)9002, pWin->winInst, NULL);
	pWin->basicarena = CreateWindow(L"BUTTON", L"Basic mode", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, (int)(pWin->winX * pWin->arenax), (int)(pWin->winY * 0.43), 110, 25, pWin->winHwnd, (HMENU)9002, pWin->winInst, NULL);
	pWin->expertarena = CreateWindow(L"BUTTON", L"Expert mode", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, (int)(pWin->winX * pWin->arenax), (int)(pWin->winY * 0.50), 110, 25, pWin->winHwnd, (HMENU)9002, pWin->winInst, NULL);
	// Put in window list
	pWin->windList.emplace_back(pWin->normalarena);
	pWin->windList.emplace_back(pWin->basicarena);
	pWin->windList.emplace_back(pWin->expertarena);
}

void paintWindow::storyWindows() {
	// Create story patch windows
	pWin->script = CreateWindow(L"BUTTON", L"Script/name changes", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(pWin->winX * pWin->storyx), (int)(pWin->winY * 0.36), 120, 25, pWin->winHwnd, (HMENU)9002, pWin->winInst, NULL);
	pWin->fasttext = CreateWindow(L"BUTTON", L"Fast text", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(pWin->winX * pWin->storyx), (int)(pWin->winY * 0.43), 110, 25, pWin->winHwnd, (HMENU)9002, pWin->winInst, NULL);
	// Put in window list
	pWin->windList.emplace_back(pWin->fasttext);
	pWin->windList.emplace_back(pWin->script);
}

void paintWindow::audioWindows() {
	// Create audio patch windows
	pWin->fmvs = CreateWindow(L"BUTTON", L"FMV undub", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(pWin->winX * pWin->audiox), (int)(pWin->winY * 0.36), 100, 25, pWin->winHwnd, (HMENU)9002, pWin->winInst, NULL);
	pWin->voice = CreateWindow(L"BUTTON", L"Battle undub", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(pWin->winX * pWin->audiox), (int)(pWin->winY * 0.43), 90, 25, pWin->winHwnd, (HMENU)9002, pWin->winInst, NULL);
	// Put in window list
	pWin->windList.emplace_back(pWin->fmvs);
	pWin->windList.emplace_back(pWin->voice);
}

void paintWindow::modeWindows() {
	// Create mode patch windows
	pWin->storyMode = CreateWindow(L"BUTTON", L"Story mode", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(pWin->winX * pWin->smx), (int)(pWin->winY * 0.36), 85, 25, pWin->winHwnd, (HMENU)9002, pWin->winInst, NULL);
	// Put in window list
	pWin->windList.emplace_back(pWin->storyMode);
}