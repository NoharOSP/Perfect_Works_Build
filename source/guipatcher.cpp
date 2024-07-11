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
			drawGlobalText();
			drawGUIText();
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

	// Create tabs

	rc = { 0, 0, 0, 0 };
	StartCommonControls(ICC_TAB_CLASSES);
	tc = CreateTabController(hWnd, hInst, TCS_FIXEDWIDTH, rc, IDC_TAB);
	SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(tc));
	InsertTab(tc, _T("General"), 0, 0, TCIF_TEXT | TCIF_IMAGE);
	InsertTab(tc, _T("Modes"), 1, 1, TCIF_TEXT | TCIF_IMAGE);
	SendMessage(tc, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), 0);
	tabNo = 1;
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
		generalButtonCustomiser(hWnd);
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
				p_music = true;
			}
			LRESULT easyticked = SendMessage(easy, BM_GETCHECK, NULL, NULL);
			if (easyticked == BST_CHECKED) {
				p_encounters = true;
				p_exp_gold = true;
			}
			LRESULT hardticked = SendMessage(hard, BM_GETCHECK, NULL, NULL);
			if (hardticked == BST_CHECKED) {
				p_items_spells = true;
				p_monsters = true;
				p_earena = true;
			}
			LRESULT portraitsticked = SendMessage(portraits, BM_GETCHECK, NULL, NULL);
			if (portraitsticked == BST_CHECKED) {
				p_portraits = true;
			}
			else {
				p_portraits = false;
			}
			LRESULT musicticked = SendMessage(music, BM_GETCHECK, NULL, NULL);
			if (musicticked == BST_CHECKED) {
				p_music = true;
			}
			else {
				p_music = false;
			}
			LRESULT fmvticked = SendMessage(fmvs, BM_GETCHECK, NULL, NULL);
			if (fmvticked == BST_CHECKED) {
				p_fmv = true;
			}
			else {
				p_fmv = false;
			}
			// Unlock patch button
			patchBoxLock();
		}
		    break;

		case IDM_APPLY_PATCH:
		{
			changed = false;
			// Return to home directory
			std::filesystem::current_path(home);
			// Access directory for patches
			if (std::filesystem::exists(patchPath)) {
				std::filesystem::current_path(patchPath);
				patchPathValid = true;
			}
			SetWindowText(hWnd, L"Preparing...");
			// Check for previous script edits
			bool scriptExists = false;
			if (pathFound1) {
				if (pc1.scriptVerify(path1)) {
					scriptExists = true;
				}
			}
			if (pathFound2) {
				if (pc2.scriptVerify(path2)) {
					scriptExists = true;
				}
			}
			// Check for ticked boxes
			if (patchPathValid) {
				if (p_encounters) {
					// Check if items/spells and script patches aren't selected to avoid compatibility issues. Otherwise, writeFile will apply encounter files
					if (!p_items_spells && !p_script) {
						if (!scriptExists) {
							if (pathFound1) {
								encountersName1 = "cd1_encounters.xdelta";
							}
							if (pathFound2) {
								encountersName2 = "cd2_encounters.xdelta";
							}
						}
					}
				}
				if (p_exp_gold) {
					// Check if items/spells and script patches aren't selected to avoid compatibility issues. Otherwise, writeFile will apply exp/gold files
					if (!p_script && !p_items_spells && !p_monsters) {
						if (!scriptExists) {
							if (pathFound1) {
								expgoldName1 = "cd1_exp_gold.xdelta";
							}
							if (pathFound2) {
								expgoldName2 = "cd2_exp_gold.xdelta";
							}
						}
					}
				}
				if (p_fastnew) {
					if (pathFound1) {
						fastName1 = "cd1_fast_text_new_script.xdelta";
					}
					if (pathFound2) {
						// Disc 2 will use the same patch regardless of whether the script patch has been applied as it has no cutscenes with auto-advance
						fastName2 = "cd2_fast_text.xdelta";
					}
					
				}
				if (p_fastold) {
					if (pathFound1) {
						fastName1 = "cd1_fast_text_old_script.xdelta";
					}
					if (pathFound2) {
						fastName2 = "cd2_fast_text.xdelta";
					}
				}
				if (p_items_spells) {
					// Check if the items/script hybrid patch needs to be applied
					if (p_script || scriptExists) {
						if (pathFound1) {
							itemspellsName1 = "cd1_items_script.xdelta";
						}
						if (pathFound2) {
							// Check if the item/script/arena hybrid patch exclusive to disc 2 needs to be added
							if (!p_barena && !p_earena) {
								itemspellsName2 = "cd2_items_script.xdelta";
							}
							else {
								if (p_barena) {
									itemspellsName2 = "cd2_script_item_barena.xdelta";
								}
								else if (p_earena) {
									itemspellsName2 = "cd2_script_item_earena.xdelta";
								}
							}
						}
					}
					else {
						if (pathFound1) {
							itemspellsName1 = "cd1_items_spells.xdelta";
						}
						if (pathFound2) {
							// Check if items/arena hybrid patch exclusive to disc 2 needs to be added
							if (!p_barena && !p_earena)
							{
								itemspellsName2 = "cd2_items_spells.xdelta";
							}
							else {
								if (p_barena) {
									itemspellsName2 = "cd2_items_barena.xdelta";
								}
								else if (p_earena) {
									itemspellsName2 = "cd2_items_earena.xdelta";
								}
							}
						}
					}
				}
				if (p_monsters) {
					// Check if items/spells and script patches aren't selected to avoid compatibility issues. Otherwise, writeFile will apply monster files
					if (!p_script && !p_items_spells && !p_exp_gold) {
						if (!scriptExists) {
							if (pathFound1) {
								monsterName1 = "cd1_monster_stats.xdelta";
							}
							if (pathFound2) {
								monsterName2 = "cd2_monster_stats.xdelta";
							}
						}
					}
				}
				if (p_script) {
					if (pathFound1) {
						// Check if items/spells and script hybrid patch won't be applied
						if (!p_items_spells) {
							scriptName1 = "cd1_script.xdelta";
						}
					}
					if (pathFound2) {
						// Check if script hybrid patches with items/spells and/or arena won't be applied
						if (!p_items_spells && (!p_barena && !p_earena)) {
							scriptName2 = "cd2_script.xdelta";
						}
					}
				}
				if (p_barena) {
					if (pathFound1) {
						// Check if items/spells and script patches aren't selected to avoid compatibility issues. Otherwise, writeFile will apply arena files
						if (!p_items_spells && !p_script) {
							if (!scriptExists) {
								arenaName1 = "cd1_battling_barena.xdelta";
							}
						}
					}
					if (pathFound2) {
						if (!p_items_spells) {
							// Check if arena/script hybrid patch needs to be added
							if (!p_script && !scriptExists) {
								arenaName2 = "cd2_battling_barena.xdelta";
							}
							else {
								arenaName2 = "cd2_script_barena.xdelta";
							}
						}
					}	
				}
				if (p_earena) {
					if (pathFound1) {
						// Check if items/spells and script patches aren't selected to avoid compatibility issues. Otherwise, writeFile will apply arena files
						if (!p_items_spells && !p_script) {
							if (!scriptExists) {
								arenaName1 = "cd1_battling_earena.xdelta";
							}
						}
					}
					if (pathFound2) {
						if (!p_items_spells) {
							// Check if arena/script hybrid patch needs to be added
							if (!p_script && !scriptExists) {
								arenaName2 = "cd2_battling_earena.xdelta";
							}
							else {
								arenaName2 = "cd2_script_earena.xdelta";
							}
						}
					}
				}
				if (p_portraits) {
					if (pathFound1) {
						/* Check if script patches aren't selected to avoid compatibility issues. Otherwise, writeFile will apply portrait files. 
						This mainly applies to disc 1 due to an error with the Solaris emergence cutscene*/
						if (!p_script) {
							portraitsName1 = "cd1_portraits.xdelta";
						}
					}
					if (pathFound2) {
						portraitsName2 = "cd2_portraits.xdelta";
					}
				}
				if (p_music) {
					if (pathFound1) {
						if (!p_fastold && !p_script && !p_items_spells) {
							musicName1 = "cd1_music.xdelta";
						}
					}
					if (pathFound2) {
						if (!p_script) {
							musicName2 = "cd2_music.xdelta";
						}
					}
				}
				if (p_fmv) {
					if (pathFound1) {
						fmvName1 = "cd1_fmvs.xdelta";
					}
					if (pathFound2) {
						fmvName2 = "cd2_fmvs.xdelta";
					}
				}
				// Graphics edits are not applied to the items/spells or script patch as they already have it applied.
				if (!p_items_spells && !p_script) {
					if (pathFound1) {
						graphicsName1 = "cd1_graphics.xdelta";
					}
					if (pathFound2) {
						graphicsName2 = "cd2_graphics.xdelta";
					}
				}
				// Bug patch is not applied to the items/spells, arena and script patches as they already have it applied.
				if (!p_items_spells && !p_script && (!p_barena && !p_earena)) {
					if (pathFound1) {
						bugName1 = "cd1_bug_fixes.xdelta";
						
					}
					if (pathFound2) {
						bugName2 = "cd2_bug_fixes.xdelta";
					}
				}
				// Title is ignored for items and script as they already have it applied.
				if (!p_items_spells && !p_script) {
					if (pathFound1) {
						titleName1 = "cd1_title.xdelta";

					}
					if (pathFound2) {
						titleName2 = "cd2_title.xdelta";
					}
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
				// Check if the script patch has been applied and what patches require direct file inserts for compatibility
				if (scriptExists) {
					p_script = true;
				}
				if (p_exp_gold || p_monsters || p_encounters || p_fastnew || p_barena || p_earena || p_portraits || p_music) {
					if ((p_script || p_items_spells || p_fastold) || (p_exp_gold && p_monsters)) {
						SetWindowText(hWnd, L"Finishing...");
						if (changed == false) {
							changed = true;
						}
						if (pathFound1) {
							writeFile wf1(hWnd, home, newPath1, 1, p_items_spells, p_script, p_exp_gold, p_monsters, p_encounters, p_fastnew, p_barena, p_earena, p_portraits, p_music, p_fastold);
						}
						if (pathFound2) {
							writeFile wf2(hWnd, home, newPath2, 2, p_items_spells, p_script, p_exp_gold, p_monsters, p_encounters, p_fastnew, p_barena, p_earena, p_portraits, p_music, p_fastold);
						}
					}
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
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		GetClientRect(hWnd, &rcWindow);
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
		// Close tabs
		DestroyTabs(hWnd);
		PostQuitMessage(0);
		break;
	case WM_NOTIFY:
		// Manage tabs
		if (((LPNMHDR)lParam)->code == TCN_SELCHANGE)
		{
			int tabID = TabCtrl_GetCurSel(tc);
			switch (tabID)
			{
			case 0:
				removeMiscButtons();
				generalButtonCustomiser(hWnd);
				tabNo = 1;
				break;
			case 1:
				removeGeneralButtons();
				miscButtonCustomiser(hWnd);
				tabNo = 2;
				break;
			default:
				break;
			}
		}
	case WM_CTLCOLORSTATIC:
		// Colour tab windows
		if (std::find(generalWindList.begin(), generalWindList.end(), (HWND)lParam) != generalWindList.end()) {
			return (LONG)GetStockObject(WHITE_BRUSH);
		}
		if (std::find(miscWindList.begin(), miscWindList.end(), (HWND)lParam) != miscWindList.end()) {
			return (LONG)GetStockObject(WHITE_BRUSH);
		}
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

// Initialise global button list
void initialiseGlobalButtonList() {
	globalWindList.emplace_back(cd1path);
	globalWindList.emplace_back(cd2path);
	globalWindList.emplace_back(browsebutton1);
	globalWindList.emplace_back(browsebutton2);
	globalWindList.emplace_back(aboutbutton);
	globalWindList.emplace_back(patchbutton);
}

//  Initialise general button list
void initialiseGeneralButtonList() {
	generalWindList.emplace_back(encounters);
	generalWindList.emplace_back(fasttext);
	generalWindList.emplace_back(portraits);
	generalWindList.emplace_back(basicarena);
	generalWindList.emplace_back(expertarena);
	generalWindList.emplace_back(expgold);
	generalWindList.emplace_back(itemspells);
	generalWindList.emplace_back(monsters);
	generalWindList.emplace_back(script);
	generalWindList.emplace_back(music);
	generalWindList.emplace_back(fmvs);
}

// Initialise misc button list
void initialiseMiscButtonList() {
	miscWindList.emplace_back(all);
	miscWindList.emplace_back(easy);
	miscWindList.emplace_back(hard);
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
	patchList1.emplace_back(musicName1);
	patchList2.emplace_back(musicName2);
	patchList1.emplace_back(graphicsName1);
	patchList2.emplace_back(graphicsName2);
	patchList1.emplace_back(bugName1);
	patchList2.emplace_back(bugName2);
	patchList1.emplace_back(titleName1);
	patchList2.emplace_back(titleName2);
}

// Lock checkboxes until a bin file has been found
void checkboxLock() {
	bool found;
	if (pathFound1 || pathFound2) {
		found = TRUE;
	}
	else {
		found = FALSE;
		for (int i = 0; i < generalWindList.size(); i++) {
			LRESULT untick = SendMessage(generalWindList[i], BM_SETCHECK, BST_UNCHECKED, NULL);
		}
		for (int i = 0; i < miscWindList.size(); i++) {
			LRESULT untick = SendMessage(miscWindList[i], BM_SETCHECK, BST_UNCHECKED, NULL);
		}
	}
	for (int i = 0; i < generalWindList.size(); i++) {
		EnableWindow(generalWindList[i], found);
	}
	for (int i = 0; i < miscWindList.size(); i++) {
		EnableWindow(miscWindList[i], found);
	}
}

// Lock patch button until a box has been ticked
void patchBoxLock() {
	bool checkfound = false;
	for (int i = 0; i < generalWindList.size(); i++) {
		LRESULT boxticked = SendMessage(generalWindList[i], BM_GETCHECK, NULL, NULL);
		if (boxticked == BST_CHECKED) {
			checkfound = true;
			EnableWindow(patchbutton, TRUE);
			break;
		}
	}
	for (int i = 0; i < miscWindList.size(); i++) {
		LRESULT boxticked = SendMessage(miscWindList[i], BM_GETCHECK, NULL, NULL);
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
	musicName1 = "";
	musicName2 = "";
	graphicsName1 = "";
	graphicsName2 = "";
	bugName1 = "";
	bugName2 = "";
	titleName1 = "";
	titleName2 = "";
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

// Define text boxes for the general tab
void initialiseGeneralWindows(HWND hWnd) {
	encounters = CreateWindow(L"BUTTON", L"Half encounters", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.0325), (int)(winY * 0.45), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	fasttext = CreateWindow(L"BUTTON", L"Fast text", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.0325), (int)(winY * 0.53), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	portraits = CreateWindow(L"BUTTON", L"Readjusted portraits", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.0325), (int)(winY * 0.61), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	basicarena = CreateWindow(L"BUTTON", L"Basic rebalance", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, (int)(winX * 0.50), (int)(winY * 0.45), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	expertarena = CreateWindow(L"BUTTON", L"Expert rebalance", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, (int)(winX * 0.50), (int)(winY * 0.53), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	expgold = CreateWindow(L"BUTTON", L"1.5x exp/gold", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.25), (int)(winY * 0.45), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	itemspells = CreateWindow(L"BUTTON", L"Rebalanced items/characters", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.25), (int)(winY * 0.53), 160, 25, hWnd, (HMENU)9002, hInst, NULL);
	monsters = CreateWindow(L"BUTTON", L"Rebalanced monsters", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.25), (int)(winY * 0.61), 130, 25, hWnd, (HMENU)9002, hInst, NULL);
	fmvs = CreateWindow(L"BUTTON", L"FMV changes", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.75), (int)(winY * 0.45), 130, 25, hWnd, (HMENU)9002, hInst, NULL);
	music = CreateWindow(L"BUTTON", L"Music changes", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.75), (int)(winY * 0.53), 130, 25, hWnd, (HMENU)9002, hInst, NULL);
	script = CreateWindow(L"BUTTON", L"Script/name changes", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.75), (int)(winY * 0.61), 130, 25, hWnd, (HMENU)9002, hInst, NULL);
}

// Define text boxes for the misc tab
void initialiseMiscWindows(HWND hWnd) {
	all = CreateWindow(L"BUTTON", L"All patches", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.0325), (int)(winY * 0.45), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	easy = CreateWindow(L"BUTTON", L"Easy mode", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.25), (int)(winY * 0.45), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	hard = CreateWindow(L"BUTTON", L"Hard mode", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.25), (int)(winY * 0.53), 160, 25, hWnd, (HMENU)9002, hInst, NULL);
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
		"Spells changed to rebalance the party as well\n"
		"as item uses/stats and shop inventories. The\n" 
		"party's base stats have also been altered to\n"
		"let the weaker characters stand closer to the\n" 
		"top tiers.";
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
	char text_easy[] =
		"Allows for an easier playthrough. The encounter\n"
		"rate is halved and the experience and gold\n"
		"dropped by enemies is increased by 50%.\n"
	    "Suitable for players who mainly want to\n"
		"experience the story.";
	HWND tt_easy = toolGenerator(text_easy, hWnd, easy);
	char text_hard[] =
		"A mode dedicated to veterans. Specific\n"
		"items and equipment have been readjusted,\n"
		"character spells and stats have been\n"
	    "rebalanced, and several of the bosses\n"
	    "have received buffs.";
	HWND tt_hard = toolGenerator(text_hard, hWnd, hard);
	char text_portraits[] =
		"Corrects the proportions of all character\n"
		"portraits when running the game at its\n"
		"native aspect ratio. If your emulator or\n"
		"scaler is already applying a correction\n"
		"to the game's overall aspect ratio or\n" 
		"using a texture hack, you may NOT need\n" 
		"this fix.";
	HWND tt_portraits = toolGenerator(text_portraits, hWnd, portraits);
	char text_music[] =
		"Changes the songs used in specific\n"
		"sequences so that they fit better,\n"
		"prevent overuse, or allow for leitmotif\n"
		"consistency.";
	HWND tt_music = toolGenerator(text_music, hWnd, music);
	char text_fmvs[] =
		"Changes the FMVs so that they use\n"
		"the Japanese audio and subtitles\n"
		"which are closer to the original\n"
		"script.";
	HWND tt_fmvs = toolGenerator(text_fmvs, hWnd, fmvs);
}


HWND CreateTabController(HWND hParent, HINSTANCE hInst, DWORD dwStyle, const RECT& rc, const int id)
{
	dwStyle |= WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS;
	return CreateWindowEx(0, WC_TABCONTROL, 0, dwStyle, rc.left, rc.top, rc.right, rc.bottom, hParent, 
		reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)), hInst, 0);
}

// Initialise common controls for parsing large files
void StartCommonControls(DWORD flags) {
	INITCOMMONCONTROLSEX iccx;
	iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
	iccx.dwICC = flags;
	InitCommonControlsEx(&iccx);
}

int InsertTab(HWND TabController, const ustring& txt, int item_index, int image_index, UINT mask) {
	std::vector<TCHAR> tmp(txt.begin(), txt.end());
	tmp.push_back(_T('\0'));
	TCITEM tabPage = { 0 };
	tabPage.mask = mask;
	tabPage.pszText = &tmp[0];
	tabPage.cchTextMax = static_cast<int>(txt.length());
	tabPage.iImage = image_index;
	return static_cast<int>(SendMessage(TabController, TCM_INSERTITEM, item_index, reinterpret_cast<LPARAM>(&tabPage)));
}

void DestroyTabs(const HWND hWnd) {
	HWND tc = reinterpret_cast<HWND>(static_cast<LONG_PTR>(GetWindowLongPtr(hWnd, GWLP_USERDATA)));
	HIMAGELIST hImages = reinterpret_cast<HIMAGELIST>(SendMessage(tc,TCM_GETIMAGELIST, 0, 0));
	ImageList_Destroy(hImages);
}

// Define global windows
void initialiseGlobalWindows(HWND hWnd) {
	cd1path = CreateWindow(L"EDIT", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, (int)(winX * 0.15), (int)(winY * 0.10), 400, 25, hWnd, NULL, hInst, NULL);
	cd2path = CreateWindow(L"EDIT", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, (int)(winX * 0.15), (int)(winY * 0.20), 400, 25, hWnd, NULL, hInst, NULL);
	browsebutton1 = CreateWindow(L"BUTTON", L"Browse", WS_BORDER | WS_CHILD | WS_VISIBLE, (int)(winX * 0.838), (int)(winY * 0.10), 70, 25, hWnd, (HMENU)9001, hInst, NULL);
	browsebutton2 = CreateWindow(L"BUTTON", L"Browse", WS_BORDER | WS_CHILD | WS_VISIBLE, (int)(winX * 0.838), (int)(winY * 0.20), 70, 25, hWnd, (HMENU)9001, hInst, NULL);
	aboutbutton = CreateWindow(L"BUTTON", L"About", WS_BORDER | WS_CHILD | WS_VISIBLE, (int)(winX * 0.838), (int)(winY * 0.825), 70, 25, hWnd, (HMENU)104, hInst, NULL);
	patchbutton = CreateWindow(L"BUTTON", L"Patch", WS_BORDER | WS_CHILD | WS_VISIBLE, (int)(winX * 0.838), (int)(winY * 0.30), 70, 25, hWnd, (HMENU)9003, hInst, NULL);
}

// Define font for global windows
void initialiseGlobalFont() {
	for (int i = 0; i < globalWindList.size(); i++) {
		SendMessage(globalWindList[i], WM_SETFONT, (LPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
	}
}

// Define font for general windows
void initialiseGeneralFont() {
	for (int i = 0; i < generalWindList.size(); i++) {
		SendMessage(generalWindList[i], WM_SETFONT, (LPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
	}
}

// Define font for misc windows
void initialiseMiscFont() {
	for (int i = 0; i < miscWindList.size(); i++) {
		SendMessage(miscWindList[i], WM_SETFONT, (LPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
	}
}

// Initialise settings for general tab
void generalButtonCustomiser(HWND hWnd) {
	initialiseGeneralWindows(hWnd);
	initialiseGeneralButtonList();
	initialiseGeneralFont();
	checkboxLock();
	patchBoxLock();
	tooltipTextMaker(hWnd);
}

// Initialise settings for misc tab
void miscButtonCustomiser(HWND hWnd) {
	initialiseMiscWindows(hWnd);
	initialiseMiscButtonList();
	initialiseMiscFont();
	checkboxLock();
	patchBoxLock();
	tooltipTextMaker(hWnd);
}

// Hide general buttons
void removeGeneralButtons() {
	ShowWindow(encounters, SW_HIDE);
	ShowWindow(fasttext, SW_HIDE);
	ShowWindow(portraits, SW_HIDE);
	ShowWindow(basicarena, SW_HIDE);
	ShowWindow(expertarena, SW_HIDE);
	ShowWindow(expgold, SW_HIDE);
	ShowWindow(itemspells, SW_HIDE);
	ShowWindow(monsters, SW_HIDE);
	ShowWindow(script, SW_HIDE);
	ShowWindow(music, SW_HIDE);
	ShowWindow(fmvs, SW_HIDE);
}

// Hide misc buttons
void removeMiscButtons() {
	ShowWindow(all, SW_HIDE);
	ShowWindow(easy, SW_HIDE);
	ShowWindow(hard, SW_HIDE);
}

void drawGUIText() {
	hdc = GetDC(tc);
	HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SelectObject(hdc, hFont);
	// Draw text for general tab
	if (tabNo == 1) {
		swprintf_s(qoltext, 256, L"QoL:      ");
		swprintf_s(balancetext, 256, L"Balance:  ");
		swprintf_s(arenatext, 256, L"Arena:  ");
		swprintf_s(storytext, 256, L"Story:    ");
		TextOut(hdc, winX * 0.0325, winY * 0.4, qoltext, wcslen(qoltext));
		TextOut(hdc, winX * 0.25, winY * 0.4, balancetext, wcslen(balancetext));
		TextOut(hdc, winX * 0.50, winY * 0.4, arenatext, wcslen(arenatext));
		TextOut(hdc, winX * 0.75, winY * 0.4, storytext, wcslen(storytext));
	}
	// Draw text for misc tab
	if (tabNo == 2) {
		swprintf_s(misctext, 256, L"Misc:     ");
		swprintf_s(modetext, 256, L"Mode:     ");
		swprintf_s(dummy, 256, L"          ");
		TextOut(hdc, winX * 0.0325, winY * 0.4, misctext, wcslen(misctext));
		TextOut(hdc, winX * 0.25, winY * 0.4, modetext, wcslen(modetext));
		TextOut(hdc, winX * 0.50, winY * 0.4, dummy, wcslen(dummy));
		TextOut(hdc, winX * 0.75, winY * 0.4, dummy, wcslen(dummy));
	}
	ReleaseDC(tc, hdc);
}

void drawGlobalText() {
	hdc = GetDC(tc);
	HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SelectObject(hdc, hFont);
	swprintf_s(cd1text, 256, L"CD1 File:");
	swprintf_s(cd2text, 256, L"CD2 File:");
	TextOut(hdc, winX * 0.0325, winY * 0.10, cd1text, wcslen(cd1text));
	TextOut(hdc, winX * 0.0325, winY * 0.20, cd2text, wcslen(cd2text));
	ReleaseDC(tc, hdc);
}

void applyPatch(int discNum) {
	std::vector<std::string> patchList;
	std::string fileName;
	std::string oldPath;
	std::string cueName;
	// Disc 1 data
	if (discNum == 1) {
		fileName = "Xenogears_PW_CD1.bin";
		cueName = "Xenogears_PW_CD1.cue";
		patchList = patchList1;
		oldPath = path1;
	}
	// Disc 2 data
	if (discNum == 2) {
		fileName = "Xenogears_PW_CD2.bin";
		cueName = "Xenogears_PW_CD2.cue";
	    patchList = patchList2;
		oldPath = path2;
	}
	bool patched = false;
	// Return to home directory
	std::filesystem::current_path(home);
	// Create batch file for xdelta commands
	std::ofstream batch_file;
	batch_file.open("commands.cmd", std::ios::trunc);
	for (int i = 0; i < patchList.size(); i++) {
		if (patchList[i] != "") {
			// Create copy of bin file to stack patches
			if (patched) {
				batch_file << "copy \"" + fileName + "\" backup.bin \n" << std::endl;
				batch_file << "del \"" + fileName + "\" \n" << std::endl;
				batch_file << "xdelta3-3.0.11-i686.exe -d  -s backup.bin patches\\" + patchList[i] + " \"" + fileName + "\" \n" << std::endl;
			}
			else {
				// Apply patches
				changed = true;
				batch_file << "xdelta3-3.0.11-i686.exe -d  -s \"" + oldPath + "\" patches\\" + patchList[i] + " \"" + fileName + "\" \n" << std::endl;
				patched = true;
			}
		}
	}
	//batch_file << "timeout /T 5" << std::endl;
	batch_file.close();
	// Execute patch file
	int batch_exit_code = system("cmd.exe /c commands.cmd");
	// Remove batch and backup bin
	remove("commands.cmd");
	remove("backup.bin");
	// Create cue file
	cue_stream.open(cueName, std::ios::out);
	cue_stream << "FILE \"" + fileName + "\" BINARY" << "\n";
	cue_stream << "  TRACK 01 MODE2/2352" << "\n";
	cue_stream << "    INDEX 01 00:00:00" << "\n";
	cue_stream.close();
	if (discNum == 1) {
		newPath1 = home + "/" + fileName;
		// Mark version at the start of the bin
		pc1.markVersion(newPath1);
		pc1.markSubVersion(newPath1);
	}
	if (discNum == 2) {
		newPath2 = home + "/" + fileName;
		pc2.markVersion(newPath2);
		pc2.markSubVersion(newPath2);
	}
}