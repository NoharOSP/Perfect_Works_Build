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
		home = std::filesystem::current_path().string();
		initialiseGlobalWindows(hWnd);
		initialiseGlobalButtonList();
		initialiseGlobalFont();
		generalButtonCustomiser(hWnd);
		break;
	}
	case WM_SIZE:
	{
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
			LRESULT arenaticked = SendMessage(arena, BM_GETCHECK, NULL, NULL);
			if (arenaticked == BST_CHECKED) {
				p_arena = true;
			}
			else {
				p_arena = false;
			}
			LRESULT allticked = SendMessage(all, BM_GETCHECK, NULL, NULL);
			if (allticked == BST_CHECKED) {
				p_script = true;
				p_fastnew = true;
				p_encounters = true;
				p_exp_gold = true;
				p_items_spells = true;
				p_monsters = true;
				p_stats = true;
				p_arena = true;
				p_portraits = true;
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
				p_stats = true;
				p_arena = true;
			}
			LRESULT portraitsticked = SendMessage(portraits, BM_GETCHECK, NULL, NULL);
			if (portraitsticked == BST_CHECKED) {
				p_portraits = true;
			}
			else {
				p_portraits = false;
			}
			patchBoxLock();
		}
		    break;

		case IDM_APPLY_PATCH:
		{
			patchChecker pc1;
			patchChecker pc2;
			bool changed = false;
			std::filesystem::current_path(home);
			if (std::filesystem::exists(patchPath)) {
				std::filesystem::current_path(patchPath);
				patchPathValid = true;
			}
			SetWindowText(hWnd, L"Preparing...");
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
			if (patchPathValid) {
				if (p_encounters) {
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
					if (!p_script && !p_items_spells) {
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
						fastName2 = "cd2_fast_text_new_script.xdelta";
					}
					
				}
				if (p_fastold) {
					if (pathFound1) {
						fastName1 = "cd1_fast_text_old_script.xdelta";
					}
					if (pathFound2) {
						fastName2 = "cd2_fast_text_old_script.xdelta";
					}
				}
				if (p_items_spells) {
					if (p_script || scriptExists) {
						if (pathFound1) {
							itemspellsName1 = "cd1_items_script.xdelta";
							if (!pc1.undubCheck(path1)) {
								fmvName1 = "cd1_fmvs.xdelta";
							}
						}
						if (pathFound2) {
							itemspellsName2 = "cd2_items_script.xdelta";
							if (!pc1.undubCheck(path2)) {
								fmvName1 = "cd2_fmvs.xdelta";
							}
						}
					}
					else {
						if (pathFound1) {
							itemspellsName1 = "cd1_items_spells.xdelta";
						}
						if (pathFound2) {
							itemspellsName2 = "cd2_items_spells.xdelta";
						}
					}
				}
				if (p_monsters) {
					if (!p_script && !p_items_spells) {
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
					if (!p_items_spells) {
						if (pathFound1) {
							scriptName1 = "cd1_script.xdelta";
							if (!pc1.undubCheck(path1)) {
								fmvName1 = "cd1_fmvs.xdelta";
							}
						}
						if (pathFound2) {
							scriptName2 = "cd2_script.xdelta";
							if (!pc1.undubCheck(path2)) {
								fmvName2 = "cd2_fmvs.xdelta";
							}
						}
					}
				}
				if (p_stats) {
					if (!p_items_spells && !p_script) {
						if (!scriptExists) {
							if (pathFound1) {
								statName1 = "cd1_statchanges.xdelta";
							}
							if (pathFound2) {
								statName2 = "cd2_statchanges.xdelta";
							}
						}
					}
				}
				if (p_arena) {
					if (!p_script && !p_items_spells) {
						if (!scriptExists) {
							if (pathFound1) {
								arenaName1 = "cd1_battling_arena.xdelta";
							}
							if (pathFound2) {
								arenaName2 = "cd2_battling_arena.xdelta";
							}
						}
					}
				}
				if (p_portraits) {
					if (pathFound1) {
						portraitsName1 = "cd1_portraits.xdelta";
					}
					if (pathFound2) {
						portraitsName2 = "cd2_portraits.xdelta";
					}
				}
				initialisePatchLists();
				SetWindowText(hWnd, L"Patching...");
				SetCursor(LoadCursor(NULL, IDC_WAIT));
				if (pathFound1) {
					bool patched = false;
					std::filesystem::current_path(home);
					std::ofstream batch_file;
					batch_file.open("commands.cmd", std::ios::trunc);
					const auto filename = std::filesystem::path{ path1 }.filename().string();
					for (int i = 0; i < patchList1.size(); i++) {
						if (patchList1[i] != "") {
							if (patched) {
								batch_file << "copy \"Xenogears_PW_CD1.bin\" backup.bin \n" << std::endl;
								batch_file << "del \"Xenogears_PW_CD1.bin\" \n" << std::endl;
								batch_file << "xdelta.exe -d  -s backup.bin patches\\" + patchList1[i] + " \"Xenogears_PW_CD1.bin\" \n" << std::endl;
							}
							else {
								changed = true;
								batch_file << "xdelta.exe -d  -s \"" + filename + "\" patches\\" + patchList1[i] + " \"Xenogears_PW_CD1.bin\" \n" << std::endl;
								patched = true;
							}
						}
					}
					batch_file.close();
					int batch_exit_code = system("cmd.exe /c commands.cmd");
					remove("commands.cmd");
					remove("backup.bin");
					disc1_cue.open("Xenogears_PW_CD1.cue", std::ios::out);
					disc1_cue << "FILE \"Xenogears_PW_CD1.bin\" BINARY" << "\n";
					disc1_cue << "  TRACK 01 MODE2/2352" << "\n";
					disc1_cue << "    INDEX 01 00:00:00" << "\n";
					disc1_cue.close();
					std::string newName = "Xenogears_PW_CD1.bin";
					size_t start_pos = path1.find(filename);
					path1.replace(start_pos, newName.length(), newName);
					size_t erasePoint = path1.find(".bin");
					path1.erase(erasePoint + 4, path1.size() - (erasePoint + 4));
					pc1.markVersion(path1);
					pc1.markSubVersion(path1);
				}
				if (pathFound2) {
					bool patched = false;
					std::filesystem::current_path(home);
					std::ofstream batch_file;
					batch_file.open("commands.cmd", std::ios::trunc);
					const auto filename = std::filesystem::path{ path2 }.filename().string();
					for (int i = 0; i < patchList2.size(); i++) {
						if (patchList2[i] != "") {
							if (patched) {
								batch_file << "copy \"Xenogears_PW_CD2.bin\" backup.bin \n" << std::endl;
								batch_file << "del \"Xenogears_PW_CD2.bin\" \n" << std::endl;
								batch_file << "xdelta.exe -d  -s backup.bin patches\\" + patchList2[i] + " \"Xenogears_PW_CD2.bin\" \n" << std::endl;
							}
							else {
								changed = true;
								batch_file << "xdelta.exe -d  -s \"" + filename + "\" patches\\" + patchList2[i] + " \"Xenogears_PW_CD2.bin\" \n" << std::endl;
								patched = true;
							}
						}
					}
					batch_file.close();
					int batch_exit_code = system("cmd.exe /c commands.cmd");
					remove("commands.cmd");
					disc2_cue.open("Xenogears_PW_CD2.cue", std::ios::out);
					disc2_cue << "FILE \"Xenogears_PW_CD2.bin\" BINARY" << "\n";
					disc2_cue << "  TRACK 01 MODE2/2352" << "\n";
					disc2_cue << "    INDEX 01 00:00:00" << "\n";
					disc2_cue.close();
					std::string newName = "Xenogears_PW_CD2.bin";
					size_t start_pos = path2.find(filename);
					path2.replace(start_pos, newName.length(), newName);
					size_t erasePoint = path2.find(".bin");
					path2.erase(erasePoint + 4, path2.size() - (erasePoint + 4));
					pc2.markVersion(path2);
					pc2.markSubVersion(path2);
				}
				if (scriptExists) {
					p_script = true;
				}
				if (p_stats || p_exp_gold || p_monsters || p_encounters || p_fastnew || p_arena) {
					if (p_script || p_items_spells) {
						SetWindowText(hWnd, L"Finishing...");
						if (changed == false) {
							changed = true;
						}
						if (pathFound1) {
							writeFile wf1(hWnd, home, path1, 1, p_items_spells, p_script, p_stats, p_exp_gold, p_monsters, p_encounters, p_fastnew, p_arena);
						}
						if (pathFound2) {
							writeFile wf2(hWnd, home, path2, 2, p_items_spells, p_script, p_stats, p_exp_gold, p_monsters, p_encounters, p_fastnew, p_arena);
						}
					}
				}
				SetWindowText(hWnd, szTitle);
				MessageBox(hWnd, L"Patch was completed successfully. Use ECCRegen to see if the bin file needs to be regenerated", L"Success", MB_ICONASTERISK);
				relock();
				reinitialisePatches();
				clearText();
			}
			else {
				MessageBox(hWnd, L"Could not find directory for 'patches'.", L"Error", MB_ICONERROR);
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
		DestroyTabs(hWnd);
		PostQuitMessage(0);
		break;
	case WM_NOTIFY:
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

void initialiseGlobalButtonList() {
	globalWindList.emplace_back(cd1path);
	globalWindList.emplace_back(cd2path);
	globalWindList.emplace_back(browsebutton1);
	globalWindList.emplace_back(browsebutton2);
	globalWindList.emplace_back(aboutbutton);
	globalWindList.emplace_back(patchbutton);
}

void initialiseGeneralButtonList() {
	generalWindList.emplace_back(encounters);
	generalWindList.emplace_back(fasttext);
	generalWindList.emplace_back(portraits);
	generalWindList.emplace_back(arena);
	generalWindList.emplace_back(expgold);
	generalWindList.emplace_back(itemspells);
	generalWindList.emplace_back(monsters);
	generalWindList.emplace_back(stats);
	generalWindList.emplace_back(script);
}

void initialiseMiscButtonList() {
	miscWindList.emplace_back(all);
	miscWindList.emplace_back(easy);
	miscWindList.emplace_back(hard);
}

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
	patchList1.emplace_back(statName1);
	patchList2.emplace_back(statName2);
	patchList1.emplace_back(scriptName1);
	patchList2.emplace_back(scriptName2);
}

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

void relock() {
	path1 = "";
	path2 = "";
	pathFound1 = false;
	pathFound2 = false;
	checkboxLock();
	patchBoxLock();
}

void reinitialisePatches () {
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
	statName1 = "";
	statName2 = "";
	patchList1.clear();
	patchList2.clear();
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

void initialiseGeneralWindows(HWND hWnd) {
	encounters = CreateWindow(L"BUTTON", L"Half encounters", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.0325), (int)(winY * 0.45), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	fasttext = CreateWindow(L"BUTTON", L"Fast text", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.0325), (int)(winY * 0.53), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	portraits = CreateWindow(L"BUTTON", L"Readjusted portraits", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.0325), (int)(winY * 0.61), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	arena = CreateWindow(L"BUTTON", L"Rebalanced battle arena", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.35), (int)(winY * 0.45), 175, 25, hWnd, (HMENU)9002, hInst, NULL);
	expgold = CreateWindow(L"BUTTON", L"1.5x exp/gold", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.35), (int)(winY * 0.53), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	itemspells = CreateWindow(L"BUTTON", L"Rebalanced items and spells", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.35), (int)(winY * 0.61), 160, 25, hWnd, (HMENU)9002, hInst, NULL);
	monsters = CreateWindow(L"BUTTON", L"Rebalanced monsters", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.35), (int)(winY * 0.69), 130, 25, hWnd, (HMENU)9002, hInst, NULL);
	stats = CreateWindow(L"BUTTON", L"Rebalanced party stats", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.35), (int)(winY * 0.77), 130, 25, hWnd, (HMENU)9002, hInst, NULL);
	script = CreateWindow(L"BUTTON", L"Script/name changes", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.75), (int)(winY * 0.45), 130, 25, hWnd, (HMENU)9002, hInst, NULL);
}

void initialiseMiscWindows(HWND hWnd) {
	all = CreateWindow(L"BUTTON", L"All patches", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.0325), (int)(winY * 0.45), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	easy = CreateWindow(L"BUTTON", L"Easy mode", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.35), (int)(winY * 0.45), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	hard = CreateWindow(L"BUTTON", L"Hard mode", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * 0.35), (int)(winY * 0.53), 160, 25, hWnd, (HMENU)9002, hInst, NULL);
}

void tooltipTextMaker(HWND hWnd) {
	char text_encounters[] =
		"Lowers encounter rate for all areas except\n"
		"for the end of disc dungeons, key grinding\n"
		"spots and places where you'd be dependent\n"
		"on encounter drops.";
	HWND tt_encounters = toolGenerator(text_encounters, hWnd, encounters);
	char text_fast[] =
		"Text scrolls instantly.";
	HWND tt_fast = toolGenerator(text_fast, hWnd, fasttext);
	char text_expgold[] =
		"Increases rewards from battle by 50%.\n"
		"This is recommended with the half encounter\n"
		"patch to keep a consistent level curve with\n"
		"the game.";
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
		"in important scenes.\n"
		"This process may take a while.";
	HWND tt_script = toolGenerator(text_script, hWnd, script);
	char text_arena[] =
		"Rebalances the gears in the arena. Please\n"
		"refer to the interactive tutorial for\n"
		"instructions.";
	HWND tt_arena = toolGenerator(text_arena, hWnd, arena);
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
		"Allows for a harder playthrough. Specific\n"
		"items and equipment have been readjusted,\n"
		"character spells and stats have been\n"
	    "rebalanced, and several of the bosses\n"
	    "have received buffs.";
	HWND tt_hard = toolGenerator(text_hard, hWnd, hard);
	char text_portraits[] =
		"Corrects the proportions of all character\n"
		"portraits when running the game at its\n"
		"native aspect ratio. If your emulator or\n"
		"scaler is already applying some kind of\n"
		"correction to the game's overall aspect\n"
		"ratio, you may NOT need this fix on top\n"
		"of that.";
	HWND tt_portraits = toolGenerator(text_portraits, hWnd, portraits);
}

HWND CreateTabController(HWND hParent, HINSTANCE hInst, DWORD dwStyle, const RECT& rc, const int id)
{
	dwStyle |= WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS;
	return CreateWindowEx(0, WC_TABCONTROL, 0, dwStyle, rc.left, rc.top, rc.right, rc.bottom, hParent, 
		reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)), hInst, 0);
}

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

void initialiseGlobalWindows(HWND hWnd) {
	cd1path = CreateWindow(L"EDIT", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, (int)(winX * 0.15), (int)(winY * 0.10), 400, 25, hWnd, NULL, hInst, NULL);
	cd2path = CreateWindow(L"EDIT", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, (int)(winX * 0.15), (int)(winY * 0.20), 400, 25, hWnd, NULL, hInst, NULL);
	browsebutton1 = CreateWindow(L"BUTTON", L"Browse", WS_BORDER | WS_CHILD | WS_VISIBLE, (int)(winX * 0.838), (int)(winY * 0.10), 70, 25, hWnd, (HMENU)9001, hInst, NULL);
	browsebutton2 = CreateWindow(L"BUTTON", L"Browse", WS_BORDER | WS_CHILD | WS_VISIBLE, (int)(winX * 0.838), (int)(winY * 0.20), 70, 25, hWnd, (HMENU)9001, hInst, NULL);
	aboutbutton = CreateWindow(L"BUTTON", L"About", WS_BORDER | WS_CHILD | WS_VISIBLE, (int)(winX * 0.838), (int)(winY * 0.815), 70, 25, hWnd, (HMENU)104, hInst, NULL);
	patchbutton = CreateWindow(L"BUTTON", L"Patch", WS_BORDER | WS_CHILD | WS_VISIBLE, (int)(winX * 0.838), (int)(winY * 0.30), 70, 25, hWnd, (HMENU)9003, hInst, NULL);
}

void initialiseGlobalFont() {
	for (int i = 0; i < globalWindList.size(); i++) {
		SendMessage(globalWindList[i], WM_SETFONT, (LPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
	}
}

void initialiseGeneralFont() {
	for (int i = 0; i < generalWindList.size(); i++) {
		SendMessage(generalWindList[i], WM_SETFONT, (LPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
	}
}

void initialiseMiscFont() {
	for (int i = 0; i < miscWindList.size(); i++) {
		SendMessage(miscWindList[i], WM_SETFONT, (LPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
	}
}

void generalButtonCustomiser(HWND hWnd) {
	initialiseGeneralWindows(hWnd);
	initialiseGeneralButtonList();
	initialiseGeneralFont();
	checkboxLock();
	patchBoxLock();
	tooltipTextMaker(hWnd);
}

void miscButtonCustomiser(HWND hWnd) {
	initialiseMiscWindows(hWnd);
	initialiseMiscButtonList();
	initialiseMiscFont();
	checkboxLock();
	patchBoxLock();
	tooltipTextMaker(hWnd);
}

void removeGeneralButtons() {
	ShowWindow(encounters, SW_HIDE);
	ShowWindow(fasttext, SW_HIDE);
	ShowWindow(portraits, SW_HIDE);
	ShowWindow(arena, SW_HIDE);
	ShowWindow(expgold, SW_HIDE);
	ShowWindow(itemspells, SW_HIDE);
	ShowWindow(monsters, SW_HIDE);
	ShowWindow(stats, SW_HIDE);
	ShowWindow(script, SW_HIDE);
}

void removeMiscButtons() {
	ShowWindow(all, SW_HIDE);
	ShowWindow(easy, SW_HIDE);
	ShowWindow(hard, SW_HIDE);
}

void drawGUIText() {
	hdc = GetDC(tc);
	HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SelectObject(hdc, hFont);
	if (tabNo == 1) {
		swprintf_s(qoltext, 256, L"QoL:      ");
		swprintf_s(balancetext, 256, L"Balance:  ");
		swprintf_s(storytext, 256, L"Story:    ");
		TextOut(hdc, winX * 0.0325, winY * 0.4, qoltext, wcslen(qoltext));
		TextOut(hdc, winX * 0.35, winY * 0.4, balancetext, wcslen(balancetext));
		TextOut(hdc, winX * 0.75, winY * 0.4, storytext, wcslen(storytext));
	}
	if (tabNo == 2) {
		swprintf_s(misctext, 256, L"Misc:     ");
		swprintf_s(modetext, 256, L"Mode:     ");
		swprintf_s(dummy, 256, L"          ");
		TextOut(hdc, winX * 0.0325, winY * 0.4, misctext, wcslen(misctext));
		TextOut(hdc, winX * 0.35, winY * 0.4, modetext, wcslen(modetext));
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

BOOL ParseALargeFile(HWND hWnd, LPTSTR lpszFileName)
{
	RECT rcClient;  
	int cyVScroll;  
	HWND hwndPB;    
	HANDLE hFile;   
	DWORD cb;       
	LPCH pch;       
	LPCH pchTmp;    


	InitCommonControls();

	GetClientRect(hWnd, &rcClient);

	cyVScroll = GetSystemMetrics(SM_CYVSCROLL);

	hwndPB = CreateWindowEx(0, PROGRESS_CLASS, (LPTSTR)NULL,
		WS_CHILD | WS_VISIBLE, rcClient.left,
		rcClient.bottom - cyVScroll,
		rcClient.right, cyVScroll,
		hWnd, (HMENU)0, hInst, NULL);

	hFile = CreateFile(lpszFileName, GENERIC_READ, FILE_SHARE_READ,
		(LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);

	if (hFile == (HANDLE)INVALID_HANDLE_VALUE)
		return FALSE;

	cb = GetFileSize(hFile, (LPDWORD)NULL);

	SendMessage(hwndPB, PBM_SETRANGE, 0, MAKELPARAM(0, cb / 2048));

	SendMessage(hwndPB, PBM_SETSTEP, (WPARAM)1, 0);

	pch = (LPCH)LocalAlloc(LPTR, sizeof(char) * 2048);

	pchTmp = pch;

	do {
		ReadFile(hFile, pchTmp, sizeof(char) * 2048, &cb, (LPOVERLAPPED)NULL);

		SendMessage(hwndPB, PBM_STEPIT, 0, 0);

	} while (cb);

	CloseHandle((HANDLE)hFile);

	DestroyWindow(hwndPB);

	return TRUE;
}


