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

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


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
		// Create home directory, font and buttons
		home = std::filesystem::current_path().string();
		initialiseGlobalWindows(hWnd);
		initialiseGlobalButtonList();
		initialiseGlobalFont();
		checkboxLock();
		patchBoxLock();
		tooltipTextMaker(hWnd);
		break;
	}
	case WM_SIZE:
	{
		// Change window position and dimensions
		HWND tc = reinterpret_cast<HWND>(static_cast<LONG_PTR>(GetWindowLongPtr(hWnd, GWLP_USERDATA)));
		MoveWindow(tc, 2, 2, LOWORD(lParam) - 4, LOWORD(lParam) - 4, TRUE);
	}

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_OPEN:
		{
			// Open file browser
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
				// Check for Xenogears bin files
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
					if (pathFound1 || pathFound2) {
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
			// Check for ticked boxes
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
				// Check if the fast text patch should support the new translation
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
			LRESULT basicarenaticked = SendMessage(basicarena, BM_GETCHECK, NULL, NULL);
			if (basicarenaticked == BST_CHECKED) {
				p_barena = true;
			}
			else {
				p_barena = false;
			}
			LRESULT expertarenaticked = SendMessage(expertarena, BM_GETCHECK, NULL, NULL);
			if (expertarenaticked == BST_CHECKED) {
				p_earena = true;
			}
			else {
				p_earena = false;
			}
			LRESULT portraitsticked = SendMessage(portraits, BM_GETCHECK, NULL, NULL);
			if (portraitsticked == BST_CHECKED) {
				p_portraits = true;
			}
			else {
				p_portraits = false;
			}
			LRESULT fmvticked = SendMessage(fmvs, BM_GETCHECK, NULL, NULL);
			if (fmvticked == BST_CHECKED) {
				p_fmv = true;
			}
			else {
				p_fmv = false;
			}
			LRESULT graphicsticked = SendMessage(graphics, BM_GETCHECK, NULL, NULL);
			if (graphicsticked == BST_CHECKED) {
				p_graphics = true;
			}
			else {
				p_graphics = false;
			}
			LRESULT voiceticked = SendMessage(voice, BM_GETCHECK, NULL, NULL);
			if (voiceticked == BST_CHECKED) {
				p_voice = true;
			}
			else {
				p_voice = false;
			}
			LRESULT allticked = SendMessage(all, BM_GETCHECK, NULL, NULL);
			if (allticked == BST_CHECKED) {
				p_script = true;
				p_fastnew = true;
				p_encounters = true;
				p_exp_gold = true;
				p_items_spells = true;
				p_monsters = true;
				p_barena = true;
				p_portraits = true;
				p_graphics = true;
				p_voice = true;
				p_fmv = true;
			}
			// Unlock patch button
			patchBoxLock();
		}
		break;

		case IDM_APPLY_PATCH:
		{
			SetWindowText(hWnd, L"Preparing...");
			changed = false;
			// Work around path names with whitespace.
			if (pathFound1) {
				if (path1.find(' ') != std::string::npos) {
					SetWindowText(hWnd, L"Copying files...");
					std::filesystem::current_path(home);
					std::ifstream  src(path1, std::ios::binary);
					std::ofstream  dst(tempcd1, std::ios::binary);
					dst << src.rdbuf();
					space = true;
				}
			}
			if (pathFound2) {
				if (path2.find(' ') != std::string::npos) {
					SetWindowText(hWnd, L"Copying files...");
					std::filesystem::current_path(home);
					std::ifstream  src(path2, std::ios::binary);
					std::ofstream  dst(tempcd2, std::ios::binary);
					dst << src.rdbuf();
					space = true;
				}
			}
			// Return to home directory
			std::filesystem::current_path(home);
			// Access directory for patches if FMVs has been ticked
			if (std::filesystem::exists(patchPath)) {
				std::filesystem::current_path(patchPath);
				patchPathValid = true;
				if (p_fmv) {
					if (pathFound1) {
						fmvName1 = "cd1_fmvs.xdelta";
					}
					if (pathFound2) {
						fmvName2 = "cd2_fmvs.xdelta";
					}
				}
			}
			// Access directory for files
			std::filesystem::current_path(home);
			if (std::filesystem::exists(filePath)) {
				std::filesystem::current_path(filePath);
				filePathValid = true;
			}
			// Check for ticked boxes
			if (filePathValid) {
				if (p_encounters) {
					// Check if script patches aren't selected to avoid compatibility issues. Otherwise, a merged folder will be used
					if (!p_script) {
						if (pathFound1) {
							encountersName1 = "encounter_rate_1";
						}
						if (pathFound2) {
							encountersName2 = "encounter_rate_2";
						}
					}
					else {
						if (pathFound1) {
							encountersName1 = "encounterone_script";
						}
						if (pathFound2) {
							encountersName2 = "encountertwo_script";
						}
					}
				}
				if (p_items_spells) {
					// Check if the items/script hybrid patch needs to be applied
					if (p_script) {
						if (pathFound1) {
							itemspellsName1 = "Script_items";
						}
						if (pathFound2) {
							itemspellsName2 = "Script_items2";
						}
					}
					else {
						if (pathFound1) {
							itemspellsName1 = "items1";
						}
						if (pathFound2) {
							itemspellsName2 = "items2";
						}
					}
				}
				if (p_exp_gold) {
					// Check if items/spells and script patches aren't selected to avoid compatibility issues.
					if (!p_script && !p_items_spells && !p_monsters) {
						if (pathFound1) {
							expgoldName1 = "boost";
						}
						if (pathFound2) {
							expgoldName2 = "boost";
						}
					}
					else if (p_items_spells && !p_script && !p_monsters) {
						if (pathFound1) {
							expgoldName1 = "exp_gold_items";
						}
						if (pathFound2) {
							expgoldName2 = "exp_gold_items";
						}
					}
					else if (p_script && !p_items_spells && !p_monsters) {
						if (pathFound1) {
							expgoldName1 = "exp_gold_script";
						}
						if (pathFound2) {
							expgoldName2 = "exp_gold_script";
						}
					}
					else if (p_items_spells && p_script && !p_monsters) {
						if (pathFound1) {
							expgoldName1 = "exp_gold_both";
						}
						if (pathFound2) {
							expgoldName2 = "exp_gold_both";
						}
					}
					else if (p_monsters && !p_items_spells && !p_script) {
						if (pathFound1) {
							expgoldName1 = "exp_monster";
						}
						if (pathFound2) {
							expgoldName2 = "exp_monster";
						}
					}
					else if (p_monsters && p_items_spells && !p_script) {
						if (pathFound1) {
							expgoldName1 = "exp_monster_items";
						}
						if (pathFound2) {
							expgoldName2 = "exp_monster_items";
						}
					}
					else if (p_monsters && !p_items_spells && p_script) {
						if (pathFound1) {
							expgoldName1 = "exp_monster_script";
						}
						if (pathFound2) {
							expgoldName2 = "exp_monster_script";
						}
					}
					else {
						if (pathFound1) {
							expgoldName1 = "exp_monster_both";
						}
						if (pathFound2) {
							expgoldName2 = "exp_monster_both";
						}
					}
				}
				if (p_fastnew) {
					if (pathFound1) {
						fastName1 = "text_cd1";
					}
					if (pathFound2) {
						// Disc 2 will use the same patch regardless of whether the script patch has been applied as it has no cutscenes with auto-advance
						fastName2 = "text_cd2";
					}

				}
				if (p_fastold) {
					if (pathFound1) {
						fastName1 = "text_old1";
					}
					if (pathFound2) {
						fastName2 = "text_cd2";
					}
				}
				if (p_monsters) {
					// Check if items/spells and script patches aren't selected to avoid compatibility issues.
					if (!p_script && !p_items_spells && !p_exp_gold) {
						if (pathFound1) {
							monsterName1 = "Monsters";
						}
						if (pathFound2) {
							monsterName2 = "Monsters";
						}
					}
					else if (p_items_spells && !p_script) {
						if (pathFound1) {
							monsterName1 = "monsters_items";
						}
						if (pathFound2) {
							monsterName2 = "monsters_items";
						}
					}
					else if (!p_items_spells && p_script) {
						if (pathFound1) {
							monsterName1 = "monsters_script";
						}
						if (pathFound2) {
							monsterName2 = "monsters_script";
						}
					}
					else if (p_items_spells && p_script) {
						if (pathFound1) {
							monsterName1 = "monsters_both";
						}
						if (pathFound2) {
							monsterName2 = "monsters_both";
						}
					}
				}
				if (p_script) {
					if (pathFound1) {
						// Check if items/spells and script hybrid won't be applied
						if (!p_items_spells) {
							scriptName1 = "script1";
						}
						else {
							scriptName1 = "Script_items";
						}
					}
					if (pathFound2) {
						// Check if script hybrid with items/spells won't be applied
						if (!p_items_spells) {
							scriptName2 = "script2";
						}
						else {
							scriptName2 = "Script_items2";
						}
					}
				}
				if (p_barena) {
					if (pathFound1) {
						if (!p_script) {
							arenaName1 = "filesbasic";
						}
						else {
							arenaName1 = "filesbasic_script";
						}
					}
					if (pathFound2) {
						if (!p_script) {
							arenaName2 = "filesbasic";
						}
						else {
							arenaName2 = "filesbasic_script";
						}
					}
				}
				if (p_earena) {
					if (pathFound1) {
						if (!p_script) {
							arenaName1 = "filesexpert";
						}
						else {
							arenaName1 = "filesexpert_script";
						}
					}
					if (pathFound2) {
						if (!p_script) {
							arenaName2 = "filesexpert";
						}
						else {
							arenaName2 = "filesexpert_script";
						}
					}
				}
				if (p_portraits) {
					if (pathFound1) {
						portraitsName1 = "portraits";
					}
					if (pathFound2) {
						portraitsName2 = "portraits";
					}
				}
				// Graphics edits outside of portraits are not applied to the items/spells or script patch here as they would cause crashes.
				if (p_graphics) {
					if (pathFound1) {
						if (!p_items_spells && !p_script) {
							if (!p_portraits) {
								graphicsName1 = "graphics";
							}
							else {
								graphicsName1 = "graphics_no_portraits";
							}
						}
						else if (!p_portraits) {
							graphicsName1 = "graphics_portraits";
						}
					}
					if (pathFound2) {
						if (!p_items_spells && !p_script) {
							if (!p_portraits) {
								graphicsName2 = "graphics";
							}
							else {
								graphicsName2 = "graphics_no_portraits";
							}
						}
						else if (!p_portraits) {
							graphicsName2 = "graphics_portraits";
						}
					}
				}
				if (p_voice) {
					if (pathFound1) {
						voiceName1 = "voice";
					}
					if (pathFound2) {
						voiceName2 = "voice";
					}
				}
				// Bug patch is not applied with items/spells as they already have it applied.
				if (!p_items_spells) {
					if (pathFound1) {
						bugName1 = "bug_fix";

					}
					if (pathFound2) {
						bugName2 = "bug_fix";
					}
				}
				if (pathFound1) {
					titleName1 = "title_screen";

				}
				if (pathFound2) {
					titleName2 = "title_screen";
				}
				initialisePatchLists();
				SetWindowText(hWnd, L"Patching...");
				SetCursor(LoadCursor(NULL, IDC_WAIT));
				// Apply disc 1 patches
				if (pathFound1) {
					applyPatch(1);
				}
				// Apply disc 2 patches
				if (pathFound2) {
					applyPatch(2);
				}
				SetWindowText(hWnd, szTitle);
				MessageBox(hWnd, L"Patch was completed successfully. Use ECCRegen to see if the bin file needs to be regenerated", L"Success", MB_ICONASTERISK);
				// Restore defaults
				relock();
				reinitialisePatches();
				clearText();
				}
				else {
					MessageBox(hWnd, L"Could not find directory for 'patches'. Check repo for latest version.", L"Error", MB_ICONERROR);
				}
			}
			break;
		case IDM_ABOUT:
			// Create "About" dialog
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			// Close patcher
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
		hdc = BeginPaint(hWnd, &ps);
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
		rc1.right = winX - (rc1.left * 2);
		rc1.bottom = winY * 0.325;
		rc2.top = winY * 0.34;
		rc2.left = rc1.left;
		rc2.right = rc1.right;
		rc2.bottom = winY * 0.725;
		Rectangle(hdc, rc1.left, rc1.top, rc1.right, rc1.bottom);
		Rectangle(hdc, rc2.left, rc2.top, rc2.right, rc2.bottom);
		FillRect(hdc, &rc1, (HBRUSH)(COLOR_WINDOW));
		FillRect(hdc, &rc2, (HBRUSH)(COLOR_WINDOW));
		FrameRect(hdc, &rc1, CreateSolidBrush(RGB(220, 220, 220)));
		FrameRect(hdc, &rc2, CreateSolidBrush(RGB(220, 220, 220)));
		drawGlobalText();
		drawGUIText();
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
	case WM_NOTIFY:
		
	case WM_CTLCOLORSTATIC:
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

// Initialise global button list
void initialiseGlobalButtonList() {
	globalWindList.emplace_back(cd1path);
	globalWindList.emplace_back(cd2path);
	globalWindList.emplace_back(browsebutton1);
	globalWindList.emplace_back(browsebutton2);
	globalWindList.emplace_back(aboutbutton);
	globalWindList.emplace_back(patchbutton);
	globalWindList.emplace_back(encounters);
	globalWindList.emplace_back(fasttext);
	globalWindList.emplace_back(portraits);
	globalWindList.emplace_back(basicarena);
	globalWindList.emplace_back(expertarena);
	globalWindList.emplace_back(expgold);
	globalWindList.emplace_back(itemspells);
	globalWindList.emplace_back(monsters);
	globalWindList.emplace_back(script);
	globalWindList.emplace_back(fmvs);
	globalWindList.emplace_back(graphics);
	globalWindList.emplace_back(voice);
	globalWindList.emplace_back(all);
}

// Initialise patch list
void initialisePatchLists() {
	patchList1.emplace_back(encountersName1);
	patchList2.emplace_back(encountersName2);
	patchList1.emplace_back(fastName1);
	patchList2.emplace_back(fastName2);
	patchList1.emplace_back(portraitsName1);
	patchList2.emplace_back(portraitsName2);
	patchList1.emplace_back(fmvName1);
	patchList2.emplace_back(fmvName2);
	patchList1.emplace_back(arenaName1);
	patchList2.emplace_back(arenaName2);
	patchList1.emplace_back(expgoldName1);
	patchList2.emplace_back(expgoldName2);
	patchList1.emplace_back(itemspellsName1);
	patchList2.emplace_back(itemspellsName2);
	patchList1.emplace_back(monsterName1);
	patchList2.emplace_back(monsterName2);
	patchList1.emplace_back(scriptName1);
	patchList2.emplace_back(scriptName2);
	patchList1.emplace_back(graphicsName1);
	patchList2.emplace_back(graphicsName2);
	patchList1.emplace_back(bugName1);
	patchList2.emplace_back(bugName2);
	patchList1.emplace_back(titleName1);
	patchList2.emplace_back(titleName2);
	patchList1.emplace_back(voiceName1);
	patchList2.emplace_back(voiceName2);
}

// Lock checkboxes until a bin file has been found
void checkboxLock() {
	bool found;
	if (pathFound1 || pathFound2) {
		found = TRUE;
	}
	else {
		found = FALSE;
		for (int i = 4; i < globalWindList.size(); i++) {
			LRESULT untick = SendMessage(globalWindList[i], BM_SETCHECK, BST_UNCHECKED, NULL);
		}
	}
	for (int i = 4; i < globalWindList.size(); i++) {
		EnableWindow(globalWindList[i], found);
	}
}

// Lock patch button until a box has been ticked
void patchBoxLock() {
	bool checkfound = false;
	for (int i = 0; i < globalWindList.size(); i++) {
		LRESULT boxticked = SendMessage(globalWindList[i], BM_GETCHECK, NULL, NULL);
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

// Locks both patches and buttons
void relock() {
	path1 = "";
	path2 = "";
	pathFound1 = false;
	pathFound2 = false;
	checkboxLock();
	patchBoxLock();
}

// Removes patch names
void reinitialisePatches () {
	arenaName1 = "";
	arenaName2 = "";
	encountersName1 = "";
	encountersName2 = "";
	expgoldName1 = "";
	expgoldName2 = "";
	fastName1 = "";
	fastName2 = "";
	portraitsName1 = "";
	portraitsName2 = "";
	fmvName1 = "";
	fmvName2 = "";
	itemspellsName1 = "";
	itemspellsName2 = "";
	monsterName1 = "";
	monsterName2 = "";
	scriptName1 = "";
	scriptName2 = "";
	graphicsName1 = "";
	graphicsName2 = "";
	bugName1 = "";
	bugName2 = "";
	titleName1 = "";
	titleName2 = "";
	voiceName1 = "";
	voiceName2 = "";
	patchList1.clear();
	patchList2.clear();
}

// Removes file paths for bins
void clearText() {
	SetWindowText(cd1path, L"");
	SetWindowText(cd2path, L"");
}


// Create tool tip
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

// Generate tool tip
HWND toolGenerator(char* text, HWND hWnd, HWND hText) {	
	wchar_t wtext[256];
	mbstowcs(wtext, text, strlen(text) + 1);
	LPWSTR ptr = wtext;
	HWND hWndTT = CreateToolTip(hWnd, hText, hInst, ptr);
	return hWndTT;
}

// Define tool tips for each checkbox
void tooltipTextMaker(HWND hWnd) {
	char text_encounters[] =
		"Lowers encounter rate for all areas except\n"
		"for the end of disc dungeons, key grinding\n"
		"spots and places where you'd be dependent\n"
		"on encounter drops.";
	HWND tt_encounters = toolGenerator(text_encounters, hWnd, encounters);
	char text_fast[] =
		"Text scrolls instantly. There are noticeable\n"
		"bugs with specific scenes, so the patch is\n"
		"still a work in progress\n";
	HWND tt_fast = toolGenerator(text_fast, hWnd, fasttext);
	char text_expgold[] =
		"Increases rewards from battle by 50%.\n"
		"This is recommended with the half encounter\n"
		"patch to keep a consistent level curve with\n"
		"the game.";
	HWND tt_expgold = toolGenerator(text_expgold, hWnd, expgold);
	char text_itemspells[] =
		"WARNING: Incompatible with pre-0.4 saves.\n"
		"The game is rebalanced; with items, spells,\n"
		"stats and the damage formula receiving\n"
		"adjustment. Several of these features are\n"
		"receiving fine tuning, meaning some bugs\n"
		"are to be expected.";
	HWND tt_itemspellss = toolGenerator(text_itemspells, hWnd, itemspells);
	char text_monsters[] =
		"Enemies and bosses will be altered to give\n"
		"a bigger challenge. Recommended for second\n"
		"playthroughs.";
	HWND tt_monsters = toolGenerator(text_monsters, hWnd, monsters);
	char text_script[] =
		"Retranslates names, locations, and key words to\n"
		"be more accurate to what they should be and\n"
		"relocalizes the script to have less grammatical\n"
		"errors, confusing lines, and awkward sentencing\n"
		"in important scenes.\n"
		"This process may take a while.";
	HWND tt_script = toolGenerator(text_script, hWnd, script);
	char text_barena[] =
		"Only the essentials of the rebalance\n"
		"for casual play and fighting the CPU.";
	HWND tt_barena = toolGenerator(text_barena, hWnd, basicarena);
	char text_earena[] =
		"Additional tweaks tailored to \n"
		"improve the PVP experience in 2P mode.\n";
	HWND tt_earena = toolGenerator(text_earena, hWnd, expertarena);
	char text_all[] =
		"Selects all patches.";
	HWND tt_all = toolGenerator(text_all, hWnd, all);
	char text_portraits[] =
		"Corrects the proportions of all character\n"
		"portraits when running the game at its\n"
		"native aspect ratio. If your emulator or\n"
		"scaler is already applying a correction\n"
		"to the game's overall aspect ratio or\n"
		"using a texture hack, you may NOT need\n"
		"this fix.";
	HWND tt_portraits = toolGenerator(text_portraits, hWnd, portraits);
	char text_fmvs[] =
		"Changes the FMVs so that they use\n"
		"the Japanese audio and subtitles\n"
		"which are closer to the original\n"
		"script.";
	HWND tt_fmvs = toolGenerator(text_fmvs, hWnd, fmvs);
	char text_graphics[] =
		"Fixes graphical bugs with\n"
		"portraits and the battle UI. If\n" 
		"you are using a texture hack, you\n" 
		"may NOT need this fix.\n";
	HWND tt_graphics = toolGenerator(text_graphics, hWnd, graphics);
	char text_voices[] =
		"Switches to the Japanese voices\n"
		"used in battle and the Speed\n"
		"minigame.";
	HWND tt_voice = toolGenerator(text_voices, hWnd, voice);
}

// Initialise common controls for parsing large files
void StartCommonControls(DWORD flags) {
	INITCOMMONCONTROLSEX iccx;
	iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccx.dwICC = flags;
	InitCommonControlsEx(&iccx);
}

// Define global windows
void initialiseGlobalWindows(HWND hWnd) {
	cd1path = CreateWindow(L"EDIT", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, (int)(winX * 0.15), (int)(winY * 0.05), 500, 25, hWnd, NULL, hInst, NULL);
	cd2path = CreateWindow(L"EDIT", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, (int)(winX * 0.15), (int)(winY * 0.15), 500, 25, hWnd, NULL, hInst, NULL);
	browsebutton1 = CreateWindow(L"BUTTON", L"Browse", WS_BORDER | WS_CHILD | WS_VISIBLE, (int)(winX * 0.85), (int)(winY * 0.05), 70, 25, hWnd, (HMENU)9001, hInst, NULL);
	browsebutton2 = CreateWindow(L"BUTTON", L"Browse", WS_BORDER | WS_CHILD | WS_VISIBLE, (int)(winX * 0.85), (int)(winY * 0.15), 70, 25, hWnd, (HMENU)9001, hInst, NULL);
	aboutbutton = CreateWindow(L"BUTTON", L"About", WS_BORDER | WS_CHILD | WS_VISIBLE, (int)(winX * 0.85), (int)(winY * 0.75), 70, 25, hWnd, (HMENU)104, hInst, NULL);
	patchbutton = CreateWindow(L"BUTTON", L"Patch", WS_BORDER | WS_CHILD | WS_VISIBLE, (int)(winX * 0.85), (int)(winY * 0.25), 70, 25, hWnd, (HMENU)9003, hInst, NULL);
	encounters = CreateWindow(L"BUTTON", L"Half encounters", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.0325), (int)(winY * 0.40), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	portraits = CreateWindow(L"BUTTON", L"Readjusted portraits", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.0325), (int)(winY * 0.47), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	graphics = CreateWindow(L"BUTTON", L"Graphical fixes", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.0325), (int)(winY * 0.54), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	expgold = CreateWindow(L"BUTTON", L"1.5x exp/gold", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.19), (int)(winY * 0.40), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	monsters = CreateWindow(L"BUTTON", L"Rebalanced monsters", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.19), (int)(winY * 0.47), 125, 25, hWnd, (HMENU)9002, hInst, NULL);
	basicarena = CreateWindow(L"BUTTON", L"Basic rebalance", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, (int)(winX * 0.35), (int)(winY * 0.40), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	expertarena = CreateWindow(L"BUTTON", L"Expert rebalance", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, (int)(winX * 0.35), (int)(winY * 0.47), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	fmvs = CreateWindow(L"BUTTON", L"FMV changes", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.51), (int)(winY * 0.40), 125, 25, hWnd, (HMENU)9002, hInst, NULL);
	script = CreateWindow(L"BUTTON", L"Script/name changes", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.51), (int)(winY * 0.47), 125, 25, hWnd, (HMENU)9002, hInst, NULL);
	voice = CreateWindow(L"BUTTON", L"Japanese voices", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.51), (int)(winY * 0.54), 130, 25, hWnd, (HMENU)9002, hInst, NULL);
	fasttext = CreateWindow(L"BUTTON", L"Fast text", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.67), (int)(winY * 0.40), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	itemspells = CreateWindow(L"BUTTON", L"Rebalanced items/characters", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.67), (int)(winY * 0.47), 160, 25, hWnd, (HMENU)9002, hInst, NULL);
	all = CreateWindow(L"BUTTON", L"All patches", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.83), (int)(winY * 0.40), 100, 25, hWnd, (HMENU)9002, hInst, NULL);
}

// Define font for global windows
void initialiseGlobalFont() {
	for (int i = 0; i < globalWindList.size(); i++) {
		SendMessage(globalWindList[i], WM_SETFONT, (LPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
	}
}

void drawGUIText() {
	HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SelectObject(hdc, hFont);
	swprintf_s(qoltext, 256, L"QoL:      ");
	swprintf_s(balancetext, 256, L"Balance:  ");
	swprintf_s(arenatext, 256, L"Arena:  ");
	swprintf_s(storytext, 256, L"Story:    ");
	swprintf_s(exptext, 256, L"Experimental:    ");
	swprintf_s(misctext, 256, L"Misc:    ");
	TextOut(hdc, winX * 0.0325, winY * 0.35, qoltext, wcslen(qoltext));
	TextOut(hdc, winX * 0.19, winY * 0.35, balancetext, wcslen(balancetext));
	TextOut(hdc, winX * 0.35, winY * 0.35, arenatext, wcslen(arenatext));
	TextOut(hdc, winX * 0.51, winY * 0.35, storytext, wcslen(storytext));
	TextOut(hdc, winX * 0.67, winY * 0.35, exptext, wcslen(exptext));
	TextOut(hdc, winX * 0.83, winY * 0.35, misctext, wcslen(misctext));
}

void drawGlobalText() {
	HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SelectObject(hdc, hFont);
	swprintf_s(cd1text, 256, L"CD1 File:");
	swprintf_s(cd2text, 256, L"CD2 File:");
	TextOut(hdc, winX * 0.0325, winY * 0.05, cd1text, wcslen(cd1text));
	TextOut(hdc, winX * 0.0325, winY * 0.15, cd2text, wcslen(cd2text));
}

void applyPatch(int discNum) {
	std::vector<std::string> patchList;
	std::string fileName;
	std::string oldPath;
	std::string cueName;
	std::string cdName;
	// Disc 1 data
	if (discNum == 1) {
		fileName = "Xenogears_PW_CD1.bin";
		cueName = "Xenogears_PW_CD1.cue";
		patchList = patchList1;
		if (space) {
			oldPath = tempcd1;
		}
		else {
			oldPath = path1;
		}
		cdName = "cd1";
	}
	// Disc 2 data
	if (discNum == 2) {
		fileName = "Xenogears_PW_CD2.bin";
		cueName = "Xenogears_PW_CD2.cue";
	    patchList = patchList2;
		if (space) {
			oldPath = tempcd2;
		}
		else {
			oldPath = path2;
		}
		cdName = "cd2fix";
	}
	bool patched = false;
	// Return to home directory
	std::filesystem::current_path(home);
	// Create text file for xenoiso
	std::ofstream list_file;
	list_file.open("list.txt", std::ios::trunc);
	// Create batch file for xdelta commands
	std::ofstream batch_file;
	batch_file.open("commands.cmd", std::ios::trunc);
	if (!patchList.empty()) {
		// Create ROMs using xenoiso
		std::string temp = "temp";
		std::filesystem::current_path(filePath);
		std::filesystem::create_directory(temp);
		if (p_script) {
			if (discNum == 1) {
				if (scriptName1 != "") {
					std::filesystem::copy(scriptName1, temp, std::filesystem::copy_options::update_existing);
				}
			}
			if (discNum == 2) {
				if (scriptName2 != "") {
					std::filesystem::copy(scriptName2, temp, std::filesystem::copy_options::update_existing);
				}
			}
		}
		if (p_encounters) {
			if (discNum == 1) {
				std::filesystem::copy(encountersName1, temp, std::filesystem::copy_options::update_existing);
			}
			if (discNum == 2) {
				std::filesystem::copy(encountersName2, temp, std::filesystem::copy_options::update_existing);
			}
		}
		if (p_fastnew || p_fastold) {
			if (discNum == 1) {
				std::filesystem::copy(fastName1, temp, std::filesystem::copy_options::update_existing);
			}
			if (discNum == 2) {
				std::filesystem::copy(fastName2, temp, std::filesystem::copy_options::update_existing);
			}
		}
		if (p_portraits) {
			if (discNum == 1) {
				std::filesystem::copy(portraitsName1, temp, std::filesystem::copy_options::update_existing);
			}
			if (discNum == 2) {
				std::filesystem::copy(portraitsName2, temp, std::filesystem::copy_options::update_existing);
			}
		}
		if (p_graphics) {
			if (discNum == 1) {
				if (graphicsName1 != "") {
					std::filesystem::copy(graphicsName1, temp, std::filesystem::copy_options::update_existing);
				}
			}
			if (discNum == 2) {
				if (graphicsName2 != "") {
					std::filesystem::copy(graphicsName2, temp, std::filesystem::copy_options::update_existing);
				}
			}
		}
		if (p_barena || p_earena) {
			if (discNum == 1) {
				std::filesystem::copy(arenaName1, temp, std::filesystem::copy_options::update_existing);
			}
			if (discNum == 2) {
				std::filesystem::copy(arenaName2, temp, std::filesystem::copy_options::update_existing);
			}
		}
		if (p_items_spells) {
			if (discNum == 1) {
				std::filesystem::copy(itemspellsName1, temp, std::filesystem::copy_options::update_existing);
			}
			if (discNum == 2) {
				std::filesystem::copy(itemspellsName2, temp, std::filesystem::copy_options::update_existing);
			}
		}
		if (p_exp_gold) {
			if (discNum == 1) {
				std::filesystem::copy(expgoldName1, temp, std::filesystem::copy_options::update_existing);
			}
			if (discNum == 2) {
				std::filesystem::copy(expgoldName2, temp, std::filesystem::copy_options::update_existing);
			}
		}
		if (p_monsters) {
			if (discNum == 1) {
				if (monsterName1 != "") {
					std::filesystem::copy(monsterName1, temp, std::filesystem::copy_options::update_existing);
				}
			}
			if (discNum == 2) {
				if (monsterName2 != "") {
					std::filesystem::copy(monsterName2, temp, std::filesystem::copy_options::update_existing);
				}
			}
		}
		if (p_voice) {
			if (discNum == 1) {
				std::filesystem::copy(voiceName1, temp, std::filesystem::copy_options::update_existing);
			}
			if (discNum == 2) {
				std::filesystem::copy(voiceName2, temp, std::filesystem::copy_options::update_existing);
			}
		}
		if (discNum == 1) {
			if (bugName1 != "") {
				std::filesystem::copy(bugName1, temp, std::filesystem::copy_options::update_existing);
			}
			if (titleName1 != "") {
				std::filesystem::copy(titleName1, temp, std::filesystem::copy_options::update_existing);
			}
		}
		if (discNum == 2) {
			if (bugName2 != "") {
				std::filesystem::copy(bugName2, temp, std::filesystem::copy_options::overwrite_existing);
			}
			if (titleName2 != "") {
				std::filesystem::copy(titleName2, temp, std::filesystem::copy_options::overwrite_existing);
			}
		}
		std::filesystem::current_path(home);
		changed = true;
		list_file << cdName << "\n" << oldPath << "\n" << fileName << "\n" << "-1,.\\gamefiles\\temp" << std::flush;
		batch_file << "xenoiso list.txt\n" << std::endl;
		patched = true;
		if (p_fmv) {
			std::filesystem::current_path(home);
			std::string patchName;
			if (discNum == 1) {
				patchName = fmvName1;
			}
			if (discNum == 2) {
				patchName = fmvName2;
			}
			if (patched) {
				// Create copy of bin file to stack patches
				batch_file << "copy \"" + fileName + "\" backup.bin \n" << std::endl;
				batch_file << "del \"" + fileName + "\" \n" << std::endl;
				batch_file << "xdelta3-3.0.11-i686.exe -d  -s backup.bin patches\\" + patchName + " \"" + fileName + "\" \n" << std::endl;
			}
			else {
				// Apply patches
				changed = true;
				batch_file << "xdelta3-3.0.11-i686.exe -d  -s \"" + oldPath + "\" patches\\" + patchName + " \"" + fileName + "\" \n" << std::endl;
				patched = true;
			}
		}
	}
	list_file.close();
	batch_file.close();
	// Execute patch file
	int batch_exit_code = system("cmd.exe /c commands.cmd");
	// Remove batch and backup bin
	remove("commands.cmd");
	remove("backup.bin");
	remove("list.txt");
	std::filesystem::current_path(filePath);
	std::filesystem::remove_all("temp");
	std::filesystem::current_path(home);
	if (discNum == 1) {
		std::filesystem::remove("Xenogears1.bin");
	}
	if (discNum == 2) {
		std::filesystem::remove("Xenogears2.bin");
	}
	// Create cue file
	cue_stream.open(cueName, std::ios::out);
	cue_stream << "FILE \"" + fileName + "\" BINARY" << "\n";
	cue_stream << "  TRACK 01 MODE2/2352" << "\n";
	cue_stream << "    INDEX 01 00:00:00" << "\n";
	cue_stream.close();
}