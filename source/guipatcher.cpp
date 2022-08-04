// guipatcher.cpp : Defines the entry point for the application.
//

// Includes:

#include "pch.h"
#include "framework.h"
#include "guipatcher.h"

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
int discNum;
int winX = 640;
int winY = 380;
bool pathFound1 = false;
bool pathFound2 = false;
bool p_encounters = false;
bool p_exp_gold = false;
bool p_fastold = false;
bool p_fastnew = false;
bool p_items_spells = false;
bool p_monsters = false;
bool p_script = false;
bool p_stats = false;
bool patchPathValid = false;
std::string home = "";
std::string path1 = "";
std::string path2 = "";
std::string patchPath = "\patches";
std::string encountersName1 = "";
std::string encountersName2 = "";
std::string expgoldName1 = "";
std::string expgoldName2 = "";
std::string fastName1 = "";
std::string fastName2 = "";
std::string itemspellsName1 = "";
std::string itemspellsName2 = "";
std::string monsterName1 = "";
std::string monsterName2 = "";
std::string scriptName1 = "";
std::string scriptName2 = "";
std::string statName1 = "";
std::string statName2 = "";
std::vector<HWND> windList;
std::vector<std::string> patchList;
HWND cd1path;
HWND cd2path;
HWND browsebutton1;
HWND browsebutton2;
HWND aboutbutton;
HWND patchbutton;
HWND encounters;
HWND fasttext;
HWND expgold;
HWND itemspells;
HWND monsters;
HWND stats;
HWND script;
RECT rcWindow;
TOOLINFO toolInfo;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void initialiseButtonList();
void initialisePatchList();
void checkboxLock();
void patchBoxLock();
void relock();
void reinitialisePatches();
void clearText();
void initialiseWindows(HWND hWnd);
void tooltipTextMaker(HWND hWnd);
HWND CreateToolTip(HWND hParent, HWND hText, HINSTANCE hInst, PTSTR pszText);
HWND toolGenerator(char* text, HWND hWnd, HWND hText);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_GUIPATCHER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GUIPATCHER));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GUIPATCHER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_GUIPATCHER);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	// Find USEDDEFAULT value

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - Create main window variables
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		home = std::filesystem::current_path().string();
		initialiseWindows(hWnd);
		initialiseButtonList();
		for (int i = 0; i < windList.size(); i++) {
			SendMessage(windList[i], WM_SETFONT, (LPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
		}
		checkboxLock();
		patchBoxLock();
		tooltipTextMaker(hWnd);
		break;
	}
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_OPEN:
		{
			OPENFILENAMEA ofn;
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = "Bin File (*.bin)\0*.bin\0";
			ofn.Flags = OFN_DONTADDTORECENT | OFN_ENABLESIZING | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
			ofn.nMaxFile = MAX_PATH;
			char szFile[MAX_PATH];
			ofn.lpstrFile = szFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nFilterIndex = 1;
			if (GetOpenFileNameA(&ofn)) {
				std::string path = ofn.lpstrFile;
				romFinder rf;
				rf.searchCD(path);
				if (rf.getFound()) {
					discNum = rf.getDisc();
					if (discNum == 1) {
						pathFound1 = true;
						path1 = path;
						std::wstring wpath = std::wstring(path1.begin(), path1.end());
						LPCWSTR lpath = wpath.c_str();
						SetWindowText(cd1path, lpath);
					}
					if (discNum == 2) {
						pathFound2 = true;
						path2 = path;
						std::wstring wpath = std::wstring(path2.begin(), path2.end());
						LPCWSTR lpath = wpath.c_str();
						SetWindowText(cd2path, lpath);
					}
					if (pathFound1 && pathFound2) {
						checkboxLock();
					}
				}
				else {
					MessageBox(hWnd, L"The bin is not valid.", L"Error", MB_ICONERROR);
				}
			}
		}
		    break;

		case IDM_CHOOSE_PATCH:
		{
			LRESULT scriptticked = SendMessage(script, BM_GETCHECK, NULL, NULL);
			if (scriptticked == BST_CHECKED) {
				p_script = true;
				if (p_fastold) {
					p_fastold = false;
					p_fastnew = true;
				}
			}
			else {
				p_script = false;
			}
			LRESULT encticked = SendMessage(encounters, BM_GETCHECK, NULL, NULL);
			if (encticked == BST_CHECKED) {
				p_encounters = true;
			}
			else {
				p_encounters = false;
			}
			LRESULT fastticked = SendMessage(fasttext, BM_GETCHECK, NULL, NULL);
			if (fastticked == BST_CHECKED) {
				if (p_script == false) {
					p_fastold = true;
				}
				else {
					p_fastnew = true;
				}
			}
			else {
				p_fastold = false;
				p_fastnew = false;
			}
			LRESULT expgoldticked = SendMessage(expgold, BM_GETCHECK, NULL, NULL);
			if (expgoldticked == BST_CHECKED) {
				p_exp_gold = true;
			}
			else {
				p_exp_gold = false;
			}
			LRESULT itemspellsticked = SendMessage(itemspells, BM_GETCHECK, NULL, NULL);
			if (itemspellsticked == BST_CHECKED) {
				p_items_spells = true;
			}
			else {
				p_items_spells = false;
			}
			LRESULT monstersticked = SendMessage(monsters, BM_GETCHECK, NULL, NULL);
			if (monstersticked == BST_CHECKED) {
				p_monsters = true;
			}
			else {
				p_monsters = false;
			}
			LRESULT statsticked = SendMessage(stats, BM_GETCHECK, NULL, NULL);
			if (statsticked == BST_CHECKED) {
				p_stats = true;
			}
			else {
				p_stats = false;
			}
			patchBoxLock();
		}
		    break;

		case IDM_APPLY_PATCH:
		{
			std::filesystem::current_path(home);
			if (std::filesystem::exists(patchPath)) {
				std::filesystem::current_path(patchPath);
				patchPathValid = true;
			}
			if (patchPathValid) {
				if (p_encounters) {
					encountersName1 = "cd1_encounters.ppf";
					encountersName2 = "cd2_encounters.ppf";
				}
				if (p_exp_gold) {
					expgoldName1 = "cd1_exp_gold.ppf";
					expgoldName2 = "cd2_exp_gold.ppf";
				}
				if (p_fastnew) {
					fastName1 = "cd1_fast_text_new_script.ppf";
					fastName2 = "cd2_fast_text_new_script.ppf";
				}
				if (p_fastold) {
					fastName1 = "cd1_fast_text_old_script.ppf";
					fastName2 = "cd2_fast_text_old_script.ppf";
				}
				if (p_items_spells) {
					itemspellsName1 = "cd1_items_spells.ppf";
					itemspellsName2 = "cd2_items_spells.ppf";
				}
				if (p_monsters) {
					monsterName1 = "cd1_monster_stats.ppf";
					monsterName2 = "cd2_monster_stats.ppf";
				}
				if (p_script) {
					scriptName1 = "cd1_script.ppf";
					scriptName2 = "cd2_script.ppf";
				}
				if (p_stats) {
					statName1 = "cd1_statchanges.ppf";
					statName2 = "cd2_statchanges.ppf";
				}
				initialisePatchList();
				for (int i = 0; i < patchList.size(); i++) {
					if (patchList[i] != "") {
						PatchProcess pp(hWnd, path1, patchList[i]);
					}
				}
				for (int i = 0; i < patchList.size(); i++) {
					if (patchList[i] != "") {
						PatchProcess pp(hWnd, path2, patchList[i]);
					}
				}
				MessageBox(hWnd, L"Patch was completed successfully. Use ECCRegen to see if the bin file needs to be regenerated", L"Success", MB_ICONASTERISK);
				relock();
				reinitialisePatches();
				clearText();
			}
			else {
				MessageBox(hWnd, L"Could not find directory for 'Patches'.", L"Error", MB_ICONERROR);
			}
		}
		    break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	    break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		SetBkMode(hdc, TRANSPARENT);
		// TODO: Add any drawing code that uses hdc here...
		SelectObject(hdc, hFont);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW));
		GetClientRect(hWnd, &rcWindow);
		RECT rc1, rc2;
		rc1, rc2 = rcWindow;
		rc1.top = winY / 50;
		rc1.left = winX / 50;
		rc1.right = winX - (rc1.left * 2.3);
		rc1.bottom = winY * 0.35;
		rc2.top = winY * 0.375;
		rc2.left = rc1.left;
		rc2.right = rc1.right;
		rc2.bottom = winY * 0.79;
		Rectangle(hdc, rc1.left, rc1.top, rc1.right, rc1.bottom);
		Rectangle(hdc, rc2.left, rc2.top, rc2.right, rc2.bottom);
		FillRect(hdc, &rc1, (HBRUSH)(COLOR_WINDOW));
		FillRect(hdc, &rc2, (HBRUSH)(COLOR_WINDOW));
		FrameRect(hdc, &rc1, CreateSolidBrush(RGB(220, 220, 220)));
		FrameRect(hdc, &rc2, CreateSolidBrush(RGB(220, 220, 220)));
		TCHAR cd1text[256];
		swprintf_s(cd1text, 256, L"CD1 File:");
		TextOut(hdc, winX * 0.0325, winY * 0.06, cd1text, wcslen(cd1text));
		TCHAR cd2text[256];
		swprintf_s(cd2text, 256, L"CD2 File:");
		TextOut(hdc, winX * 0.0325, winY * 0.16, cd2text, wcslen(cd2text));
		TCHAR qoltext[256];
		swprintf_s(qoltext, 256, L"QoL:");
		TextOut(hdc, winX * 0.0325, winY * 0.4, qoltext, wcslen(qoltext));
		TCHAR balancetext[256];
		swprintf_s(balancetext, 256, L"Balance:");
		TextOut(hdc, winX * 0.35, winY * 0.4, balancetext, wcslen(balancetext));
		TCHAR storytext[256];
		swprintf_s(storytext, 256, L"Story:");
		TextOut(hdc, winX * 0.75, winY * 0.4, storytext, wcslen(storytext));
		EndPaint(hWnd, &ps);
	}
	    break;
	case WM_GETMINMAXINFO:
	{
		LPMINMAXINFO pMMI = (LPMINMAXINFO)lParam;
		pMMI->ptMaxTrackSize.x = winX;
		pMMI->ptMaxTrackSize.y = winY;
		break;
	}
	    break;
	case TTN_SHOW:
	{

	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void initialiseButtonList() {
	windList.emplace_back(cd1path);
	windList.emplace_back(cd2path);
	windList.emplace_back(browsebutton1);
	windList.emplace_back(browsebutton2);
	windList.emplace_back(aboutbutton);
	windList.emplace_back(patchbutton);
	windList.emplace_back(encounters);
	windList.emplace_back(fasttext);
	windList.emplace_back(expgold);
	windList.emplace_back(itemspells);
	windList.emplace_back(monsters);
	windList.emplace_back(stats);
	windList.emplace_back(script);
}

void initialisePatchList() {
	patchList.emplace_back(encountersName1);
	patchList.emplace_back(encountersName2);
	patchList.emplace_back(fastName1);
	patchList.emplace_back(fastName2);
	patchList.emplace_back(expgoldName1);
	patchList.emplace_back(expgoldName2);
	patchList.emplace_back(itemspellsName1);
	patchList.emplace_back(itemspellsName2);
	patchList.emplace_back(monsterName1);
	patchList.emplace_back(monsterName2);
	patchList.emplace_back(statName1);
	patchList.emplace_back(statName2);
	patchList.emplace_back(scriptName1);
	patchList.emplace_back(scriptName2);
}

void checkboxLock() {
	bool found;
	if (pathFound1 && pathFound2) {
		found = TRUE;
	}
	else {
		found = FALSE;
		for (int i = 6; i < windList.size(); i++) {
			LRESULT untick = SendMessage(windList[i], BM_SETCHECK, BST_UNCHECKED, NULL);
		}
	}
	for (int i = 6; i < windList.size(); i++) {
		EnableWindow(windList[i], found);
	}
}

void patchBoxLock() {
	bool checkfound = false;
	for (int i = 6; i < windList.size(); i++) {
		LRESULT boxticked = SendMessage(windList[i], BM_GETCHECK, NULL, NULL);
		if (boxticked == BST_CHECKED) {
			checkfound = true;
			EnableWindow(patchbutton, TRUE);
			break;
		}
	}
	if (!checkfound) {
		EnableWindow(patchbutton, FALSE);
	}
}

void relock() {
	path1 = "";
	path2 = "";
	pathFound1 = false;
	pathFound2 = false;
	checkboxLock();
	patchBoxLock();
}

void reinitialisePatches () {
	for (int i = 0; i < patchList.size(); i++) {
		patchList[i] = "";
	}
}

void clearText() {
	SetWindowText(cd1path, L"");
	SetWindowText(cd2path, L"");
}

HWND CreateToolTip(HWND hParent, HWND hText, HINSTANCE hInst, PTSTR pszText)
{
	if (!hParent || !hText || !pszText)
	{
		return NULL;
	}

	HWND hwndTip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
		WS_POPUP | TTS_ALWAYSTIP,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		hParent, NULL,
		hInst, NULL);

	if (!hwndTip)
	{
		return NULL;
	}

	toolInfo.cbSize = sizeof(toolInfo);
	toolInfo.hwnd = hParent;
	toolInfo.hinst = hInst;
	toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
	toolInfo.uId = (UINT_PTR)hText;
	toolInfo.lpszText = pszText;
	GetClientRect(hParent, &toolInfo.rect);
	SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);
	SendMessage(hwndTip, TTM_SETMAXTIPWIDTH, 0, 255);
	return hwndTip;
}

HWND toolGenerator(char* text, HWND hWnd, HWND hText) {	
	wchar_t wtext[256];
	mbstowcs(wtext, text, strlen(text) + 1);
	LPWSTR ptr = wtext;
	HWND hWndTT = CreateToolTip(hWnd, hText, hInst, ptr);
	return hWndTT;
}

void initialiseWindows(HWND hWnd) {
	cd1path = CreateWindow(L"EDIT", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, (int)(winX * 0.15), (int)(winY * 0.05), 400, 25, hWnd, NULL, hInst, NULL);
	cd2path = CreateWindow(L"EDIT", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, (int)(winX * 0.15), (int)(winY * 0.15), 400, 25, hWnd, NULL, hInst, NULL);
	browsebutton1 = CreateWindow(L"BUTTON", L"Browse", WS_BORDER | WS_CHILD | WS_VISIBLE, (int)(winX * 0.825), (int)(winY * 0.05), 70, 25, hWnd, (HMENU)9001, hInst, NULL);
	browsebutton2 = CreateWindow(L"BUTTON", L"Browse", WS_BORDER | WS_CHILD | WS_VISIBLE, (int)(winX * 0.825), (int)(winY * 0.15), 70, 25, hWnd, (HMENU)9001, hInst, NULL);
	aboutbutton = CreateWindow(L"BUTTON", L"About", WS_BORDER | WS_CHILD | WS_VISIBLE, (int)(winX * 0.848), (int)(winY * 0.815), 70, 25, hWnd, (HMENU)104, hInst, NULL);
	patchbutton = CreateWindow(L"BUTTON", L"Patch", WS_BORDER | WS_CHILD | WS_VISIBLE, (int)(winX * 0.825), (int)(winY * 0.25), 70, 25, hWnd, (HMENU)9003, hInst, NULL);
	encounters = CreateWindow(L"BUTTON", L"Half encounters", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.0325), (int)(winY * 0.45), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	fasttext = CreateWindow(L"BUTTON", L"Fast text", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.0325), (int)(winY * 0.53), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	expgold = CreateWindow(L"BUTTON", L"Double exp/gold", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.35), (int)(winY * 0.45), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	itemspells = CreateWindow(L"BUTTON", L"Rebalanced items and spells", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.35), (int)(winY * 0.53), 160, 25, hWnd, (HMENU)9002, hInst, NULL);
	monsters = CreateWindow(L"BUTTON", L"Rebalanced monsters", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.35), (int)(winY * 0.61), 130, 25, hWnd, (HMENU)9002, hInst, NULL);
	stats = CreateWindow(L"BUTTON", L"Rebalanced party stats", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.35), (int)(winY * 0.69), 130, 25, hWnd, (HMENU)9002, hInst, NULL);
	script = CreateWindow(L"BUTTON", L"Retranslated script", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.75), (int)(winY * 0.45), 115, 25, hWnd, (HMENU)9002, hInst, NULL);
}

void tooltipTextMaker(HWND hWnd) {
	char text_encounters[] =
		"Lowers encounter rate for all areas except\n"
		"for the end of disc dungeons, key grinding\n"
		"spots and places where you'd be dependent\n"
		"on encounter drops.";
	HWND tt_encounters = toolGenerator(text_encounters, hWnd, encounters);
	char text_fast[] =
		"Text scrolls instantly.\n"
		"WARNING!! This has a history of crashing\n"
		"scripted scenes. Use at your own risk.";
	HWND tt_fast = toolGenerator(text_fast, hWnd, fasttext);
	char text_expgold[] =
		"Doubles rewards from battle. This is\n"
		"recommended with the half encounter patch\n"
		"to keep a consistent level curve with the game.";
	HWND tt_expgold = toolGenerator(text_expgold, hWnd, expgold);
	char text_itemspells[] =
		"Spells changed to rebalance the party as well\n"
		"as item uses/stats and shop inventories.";
	HWND tt_itemspellss = toolGenerator(text_itemspells, hWnd, itemspells);
	char text_monsters[] =
		"Enemies and bosses will be altered to give\n"
		"a bigger challenge. Recommended for second\n"
		"playthroughs.";
	HWND tt_monsters = toolGenerator(text_monsters, hWnd, monsters);
	char text_stats[] =
		"The party's base stats are altered to let the\n"
		"weaker characters stand closer to the top tiers,\n"
		"but also weakens the more exploitable\n"
		"characters to overshadow the cast significantly\n"
		"less.";
	HWND tt_stats = toolGenerator(text_stats, hWnd, stats);
	char text_script[] =
		"Retranslates names, locations, and key words to\n"
		"be more accurate to what they should be and\n"
		"relocalizes the script to have less grammatical\n"
		"errors, confusing lines, and awkward sentencing\n"
		"in important scenes.";
	HWND tt_script = toolGenerator(text_script, hWnd, script);
}