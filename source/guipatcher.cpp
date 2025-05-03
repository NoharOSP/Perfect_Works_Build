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
			// Draw text
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
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
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
		// Create log
		log_file.open("pw_log.txt", std::ios::trunc);
		break;
	}
	case WM_SIZE:
	{
		// Change window position and dimensions
		HWND tc = reinterpret_cast<HWND>(static_cast<LONG_PTR>(GetWindowLongPtr(hWnd, GWLP_USERDATA)));
		MoveWindow(tc, 2, 2, LOWORD(lParam) - 4, LOWORD(lParam) - 4, true);
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
			log_file << "Open file browser." << std::endl;
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
				log_file << "File selected." << std::endl;
				std::string path = ofn.lpstrFile;
				romFinder rf;
				// Check for Xenogears bin files
				log_file << "Check if selected bin is Xenogears." << std::endl;
				rf.searchCD(path);
				if (rf.getFound()) {
					log_file << "Xenogears has been found. Determine disc number." << std::endl;
					discNum = rf.getDisc();
					if (discNum == 1) {
						log_file << "Disc 1 found." << std::endl;
						pathFound1 = true;
						log_file << "Determine disc 1 path." << std::endl;
						path1 = path;
						std::wstring wpath = std::wstring(path1.begin(), path1.end());
						LPCWSTR lpath = wpath.c_str();
						log_file << "Put disc 1 path in path window." << std::endl;
						SetWindowText(cd1path, lpath);
					}
					if (discNum == 2) {
						log_file << "Disc 2 found." << std::endl;
						pathFound2 = true;
						log_file << "Determine disc 2 path." << std::endl;
						path2 = path;
						std::wstring wpath = std::wstring(path2.begin(), path2.end());
						LPCWSTR lpath = wpath.c_str();
						log_file << "Put disc 2 path in path window." << std::endl;
						SetWindowText(cd2path, lpath);
					}
					if (pathFound1 || pathFound2) {
						checkboxLock();
					}
				}
				else {
					log_file << "The selected file is not a valid Xenogears ROM." << std::endl;
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
				log_file << "Script changes ticked." << std::endl;
				p_script = true;
				if (p_fastold) {
					log_file << "Ensure the version of fast text is used that supports the updated script." << std::endl;
					p_fastold = false;
					p_fastnew = true;
				}
			}
			else {
				log_file << "Script changes unticked." << std::endl;
				p_script = false;
			}
			LRESULT encticked = SendMessage(encounters, BM_GETCHECK, NULL, NULL);
			if (encticked == BST_CHECKED) {
				log_file << "Half encounters ticked." << std::endl;
				p_encounters = true;
			}
			else {
				log_file << "Half encounters unticked." << std::endl;
				p_encounters = false;
			}
			LRESULT fastticked = SendMessage(fasttext, BM_GETCHECK, NULL, NULL);
			if (fastticked == BST_CHECKED) {
				// Check if the fast text patch should support the new translation
				log_file << "Fast text ticked." << std::endl;
				if (p_script == false) {
					log_file << "Ensure the version of fast text supporting the original translation is used." << std::endl;
					p_fastold = true;
				}
				else {
					log_file << "Ensure the version of fast text is used that supports the updated script." << std::endl;
					p_fastnew = true;
				}
			}
			else {
				log_file << "Fast text unticked." << std::endl;
				p_fastold = false;
				p_fastnew = false;
			}
			LRESULT expgoldticked = SendMessage(expgold, BM_GETCHECK, NULL, NULL);
			if (expgoldticked == BST_CHECKED) {
				log_file << "1.5 exp/gold ticked." << std::endl;
				p_exp_gold = true;
			}
			else {
				log_file << "1.5 exp/gold unticked." << std::endl;
				p_exp_gold = false;
			}
			LRESULT itemspellsticked = SendMessage(itemspells, BM_GETCHECK, NULL, NULL);
			if (itemspellsticked == BST_CHECKED) {
				log_file << "Rebalanced party/items ticked." << std::endl;
				p_items_spells = true;
			}
			else {
				log_file << "Rebalanced party/items unticked." << std::endl;
				p_items_spells = false;
			}
			LRESULT monstersticked = SendMessage(monsters, BM_GETCHECK, NULL, NULL);
			if (monstersticked == BST_CHECKED) {
				log_file << "Rebalanced monsters ticked." << std::endl;
				p_monsters = true;
			}
			else {
				log_file << "Rebalanced party/items unticked." << std::endl;
				p_monsters = false;
			}
			LRESULT normalarenaticked = SendMessage(normalarena, BM_GETCHECK, NULL, NULL);
			if (normalarenaticked == BST_CHECKED) {
				log_file << "Normal arena selected." << std::endl;
				p_barena = false;
				p_earena = false;
			}
			LRESULT basicarenaticked = SendMessage(basicarena, BM_GETCHECK, NULL, NULL);
			if (basicarenaticked == BST_CHECKED) {
				log_file << "Basic arena selected." << std::endl;
				p_barena = true;
			}
			else {
				log_file << "Basic arena deselected." << std::endl;
				p_barena = false;
			}
			LRESULT expertarenaticked = SendMessage(expertarena, BM_GETCHECK, NULL, NULL);
			if (expertarenaticked == BST_CHECKED) {
				log_file << "Expert arena selected." << std::endl;
				p_earena = true;
			}
			else {
				log_file << "Expert arena deselected." << std::endl;
				p_earena = false;
			}
			LRESULT portraitsticked = SendMessage(portraits, BM_GETCHECK, NULL, NULL);
			if (portraitsticked == BST_CHECKED) {
				log_file << "Resized portraits ticked." << std::endl;
				p_portraits = true;
			}
			else {
				log_file << "Resized portraits unticked." << std::endl;
				p_portraits = false;
			}
			LRESULT fmvticked = SendMessage(fmvs, BM_GETCHECK, NULL, NULL);
			if (fmvticked == BST_CHECKED) {
				log_file << "FMV undub ticked." << std::endl;
				p_fmv = true;
			}
			else {
				log_file << "FMV undub unticked." << std::endl;
				p_fmv = false;
			}
			LRESULT graphicsticked = SendMessage(graphics, BM_GETCHECK, NULL, NULL);
			if (graphicsticked == BST_CHECKED) {
				log_file << "Graphical fixes ticked." << std::endl;
				p_graphics = true;
			}
			else {
				log_file << "Graphical fixes unticked." << std::endl;
				p_graphics = false;
			}
			LRESULT voiceticked = SendMessage(voice, BM_GETCHECK, NULL, NULL);
			if (voiceticked == BST_CHECKED) {
				log_file << "Battle undub ticked." << std::endl;
				p_voice = true;
			}
			else {
				log_file << "Battle undub unticked." << std::endl;
				p_voice = false;
			}
			LRESULT flashesticked = SendMessage(flashes, BM_GETCHECK, NULL, NULL);
			if (flashesticked == BST_CHECKED) {
				log_file << "No battle flashes ticked." << std::endl;
				p_flashes = true;
			}
			else {
				log_file << "No battle flashes unticked." << std::endl;
				p_flashes = false;
			}
			LRESULT storymodeticked = SendMessage(storyMode, BM_GETCHECK, NULL, NULL);
			if (storymodeticked == BST_CHECKED) {
				log_file << "Story mode ticked." << std::endl;
				p_story_mode = true;
			}
			else {
				log_file << "Story mode unticked." << std::endl;
				p_story_mode = false;
			}
			// Unlock patch button
			patchBoxLock();
		}
		break;

		case IDM_APPLY_PATCH:
		{
			log_file << "Preparing to patch." << std::endl;
			SetWindowText(hWnd, L"Preparing...");
			changed = false;
			// Work around path names with whitespace.
			if (pathFound1) {
				log_file << "Check if disc 1 filename has whitespace characters." << std::endl;
				if (path1.find(' ') != std::string::npos) {
					log_file << "Whitespace characters found. Creating a copy of the ROM inside the home directory." << std::endl;
					SetWindowText(hWnd, L"Copying files...");
					std::filesystem::current_path(home);
					std::ifstream  src(path1, std::ios::binary);
					std::ofstream  dst(tempcd1, std::ios::binary);
					dst << src.rdbuf();
					log_file << "Copying completed." << std::endl;
					space = true;
				}
			}
			if (pathFound2) {
				log_file << "Check if disc 2 filename has whitespace characters." << std::endl;
				if (path2.find(' ') != std::string::npos) {
					log_file << "Whitespace characters found. Creating a copy of the ROM inside the home directory." << std::endl;
					SetWindowText(hWnd, L"Copying files...");
					std::filesystem::current_path(home);
					std::ifstream  src(path2, std::ios::binary);
					std::ofstream  dst(tempcd2, std::ios::binary);
					dst << src.rdbuf();
					log_file << "Copying completed." << std::endl;
					space = true;
				}
			}
			// Return to home directory
			std::filesystem::current_path(home);
			// Access directory for patches if FMVs has been ticked
			log_file << "Check if 'patches' directory exists." << std::endl;
			if (std::filesystem::exists(patchPath)) {
				std::filesystem::current_path(patchPath);
				log_file << "'patches' directory is valid." << std::endl;
				patchPathValid = true;
				if (p_fmv) {
					if (pathFound1) {
						log_file << "Disc 1 FMV patch file found." << std::endl;
						fmvName1 = "cd1_fmvs.xdelta";
					}
					if (pathFound2) {
						log_file << "Disc 2 FMV patch file found." << std::endl;
						fmvName2 = "cd2_fmvs.xdelta";
					}
				}
			}
			// Access directory for files
			std::filesystem::current_path(home);
			log_file << "Check if 'gamefiles' directory is valid." << std::endl;
			if (std::filesystem::exists(filePath)) {
				std::filesystem::current_path(filePath);
				log_file << "'gamefiles' directory is valid." << std::endl;
				filePathValid = true;
			}
			// Check for ticked boxes
			if (filePathValid) {
				if (p_encounters) {
					// Check if story mode hasn't been ticked
					if (!p_story_mode) {
						// Check if script patches aren't selected to avoid compatibility issues. Otherwise, a merged folder will be used
						if (!p_script) {
							if (pathFound1) {
								log_file << "Disc 1 half encounters directory found." << std::endl;
								encountersName1 = "encounter_rate_1";
							}
							if (pathFound2) {
								log_file << "Disc 2 half encounters directory found." << std::endl;
								encountersName2 = "encounter_rate_2";
							}
						}
						else {
							if (pathFound1) {
								log_file << "Disc 1 half encounters/retranslated script directory found." << std::endl;
								encountersName1 = "encounterone_script";
							}
							if (pathFound2) {
								log_file << "Disc 2 half encounters/retranslated script directory found." << std::endl;
								encountersName2 = "encountertwo_script";
							}
						}
					}
				}
				if (p_items_spells) {
					// Check if story mode hasn't been ticked
					if (!p_story_mode) {
						// Check if the items/script hybrid patch needs to be applied
						if (p_script) {
							if (pathFound1) {
								log_file << "Disc 1 rebalanced party/retranslated script directory found." << std::endl;
								itemspellsName1 = "Script_items";
							}
							if (pathFound2) {
								log_file << "Disc 2 rebalanced party/retranslated script directory found." << std::endl;
								itemspellsName2 = "Script_items2";
							}
						}
						else {
							if (pathFound1) {
								log_file << "Disc 1 rebalanced party directory found." << std::endl;
								itemspellsName1 = "items1";
							}
							if (pathFound2) {
								log_file << "Disc 1 rebalanced party directory found." << std::endl;
								itemspellsName2 = "items2";
							}
						}
					}
				}
				if (p_exp_gold) {
					// Check if story mode hasn't been ticked
					if (!p_story_mode) {
						// Check if items/spells and script patches aren't selected to avoid compatibility issues.
						if (!p_script && !p_items_spells && !p_monsters) {
							if (pathFound1) {
								log_file << "Disc 1 1.5x exp/gold directory found." << std::endl;
								expgoldName1 = "boost";
							}
							if (pathFound2) {
								log_file << "Disc 2 1.5x exp/gold directory found." << std::endl;
								expgoldName2 = "boost";
							}
						}
						else if (p_items_spells && !p_script && !p_monsters) {
							if (pathFound1) {
								log_file << "Disc 1 1.5x exp/gold/rebalanced party directory found." << std::endl;
								expgoldName1 = "exp_gold_items";
							}
							if (pathFound2) {
								log_file << "Disc 2 1.5x exp/gold/rebalanced party directory found." << std::endl;
								expgoldName2 = "exp_gold_items";
							}
						}
						else if (p_script && !p_items_spells && !p_monsters) {
							if (pathFound1) {
								log_file << "Disc 1 1.5x exp/gold/retranslated script directory found." << std::endl;
								expgoldName1 = "exp_gold_script";
							}
							if (pathFound2) {
								log_file << "Disc 2 1.5x exp/gold/retranslated script directory found." << std::endl;
								expgoldName2 = "exp_gold_script";
							}
						}
						else if (p_items_spells && p_script && !p_monsters) {
							if (pathFound1) {
								log_file << "Disc 1 1.5x exp/gold/retranslated script/rebalanced party directory found." << std::endl;
								expgoldName1 = "exp_gold_both";
							}
							if (pathFound2) {
								log_file << "Disc 2 1.5x exp/gold/retranslated script/rebalanced party directory found." << std::endl;
								expgoldName2 = "exp_gold_both";
							}
						}
						else if (p_monsters && !p_items_spells && !p_script) {
							if (pathFound1) {
								log_file << "Disc 1 1.5x exp/gold/rebalanced monsters directory found." << std::endl;
								expgoldName1 = "exp_monster";
							}
							if (pathFound2) {
								log_file << "Disc 2 1.5x exp/gold/rebalanced monsters directory found." << std::endl;
								expgoldName2 = "exp_monster";
							}
						}
						else if (p_monsters && p_items_spells && !p_script) {
							if (pathFound1) {
								log_file << "Disc 1 1.5x exp/gold/rebalanced monsters and party directory found." << std::endl;
								expgoldName1 = "exp_monster_items";
							}
							if (pathFound2) {
								log_file << "Disc 2 1.5x exp/gold/rebalanced monsters and party directory found." << std::endl;
								expgoldName2 = "exp_monster_items";
							}
						}
						else if (p_monsters && !p_items_spells && p_script) {
							if (pathFound1) {
								log_file << "Disc 1 1.5x exp/gold/rebalanced monsters/retranslated script directory found." << std::endl;
								expgoldName1 = "exp_monster_script";
							}
							if (pathFound2) {
								log_file << "Disc 2 1.5x exp/gold/rebalanced monsters/retranslated script directory found." << std::endl;
								expgoldName2 = "exp_monster_script";
							}
						}
						else {
							if (pathFound1) {
								log_file << "Disc 1 1.5x exp/gold/rebalanced monsters and party/retranslated script directory found." << std::endl;
								expgoldName1 = "exp_monster_both";
							}
							if (pathFound2) {
								log_file << "Disc 2 1.5x exp/gold/rebalanced monsters and party/retranslated script directory found." << std::endl;
								expgoldName2 = "exp_monster_both";
							}
						}
					}
				}
				if (p_fastnew) {
					if (pathFound1) {
						log_file << "Disc 1 retranslated fast text directory found." << std::endl;
						fastName1 = "text_cd1";
					}
					if (pathFound2) {
						// Disc 2 will use the same patch regardless of whether the script patch has been applied as it has no cutscenes with auto-advance
						log_file << "Disc 2 retranslated fast text directory found." << std::endl;
						fastName2 = "text_cd2";
					}

				}
				if (p_fastold) {
					if (pathFound1) {
						log_file << "Disc 1 fast text directory found." << std::endl;
						fastName1 = "text_old1";
					}
					if (pathFound2) {
						log_file << "Disc 2 fast text directory found." << std::endl;
						fastName2 = "text_cd2";
					}
				}
				if (p_monsters) {
					// Check if story mode hasn't been ticked
					if (!p_story_mode) {
						// Check if items/spells and script patches aren't selected to avoid compatibility issues.
						if (!p_script && !p_items_spells && !p_exp_gold) {
							if (pathFound1) {
								log_file << "Disc 1 rebalanced monsters directory found." << std::endl;
								monsterName1 = "Monsters";
							}
							if (pathFound2) {
								log_file << "Disc 2 rebalanced monsters directory found." << std::endl;
								monsterName2 = "Monsters";
							}
						}
						else if (p_items_spells && !p_script) {
							if (pathFound1) {
								log_file << "Disc 1 rebalanced monsters and party directory found." << std::endl;
								monsterName1 = "monsters_items";
							}
							if (pathFound2) {
								log_file << "Disc 2 rebalanced monsters and party directory found." << std::endl;
								monsterName2 = "monsters_items";
							}
						}
						else if (!p_items_spells && p_script) {
							if (pathFound1) {
								log_file << "Disc 1 rebalanced monsters/retranslated script directory found." << std::endl;
								monsterName1 = "monsters_script";
							}
							if (pathFound2) {
								log_file << "Disc 2 rebalanced monsters/retranslated script directory found." << std::endl;
								monsterName2 = "monsters_script";
							}
						}
						else if (p_items_spells && p_script) {
							if (pathFound1) {
								log_file << "Disc 1 rebalanced monsters and party/retranslated script directory found." << std::endl;
								monsterName1 = "monsters_both";
							}
							if (pathFound2) {
								log_file << "Disc 2 rebalanced monsters and party/retranslated script directory found." << std::endl;
								monsterName2 = "monsters_both";
							}
						}
					}
				}
				if (p_script) {
					// TODO: Merge with story mode
					if (pathFound1) {
						// Check if items/spells and script hybrid won't be applied
						if (!p_items_spells) {
							log_file << "Disc 1 retranslated script directory found." << std::endl;
							scriptName1 = "script1";
						}
						else {
							log_file << "Disc 1 retranslated script/rebalanced party directory found." << std::endl;
							scriptName1 = "Script_items";
						}
					}
					if (pathFound2) {
						// Check if script hybrid with items/spells won't be applied
						if (!p_items_spells) {
							log_file << "Disc 2 retranslated script directory found." << std::endl;
							scriptName2 = "script2";
						}
						else {
							log_file << "Disc 2 retranslated script/rebalanced party directory found." << std::endl;
							scriptName2 = "Script_items2";
						}
					}
				}
				if (p_barena) {
					if (pathFound1) {
						if (!p_story_mode) {
							if (!p_script) {
								log_file << "Disc 1 basic arena directory found." << std::endl;
								arenaName1 = "filesbasic";
							}
							else {
								log_file << "Disc 1 basic arena/retranslated script directory found." << std::endl;
								arenaName1 = "filesbasic_script";
							}
						}
					}
					if (pathFound2) {
						if (!p_story_mode) {
							if (!p_script) {
								log_file << "Disc 2 basic arena directory found." << std::endl;
								arenaName2 = "filesbasic";
							}
							else {
								log_file << "Disc 2 basic arena/retranslated script directory found." << std::endl;
								arenaName2 = "filesbasic_script";
							}
						}
					}
				}
				if (p_earena) {
					if (pathFound1) {
						if (!p_story_mode) {
							if (!p_script) {
								log_file << "Disc 1 expert arena directory found." << std::endl;
								arenaName1 = "filesexpert";
							}
							else {
								log_file << "Disc 1 expert arena/retranslated script directory found." << std::endl;
								arenaName1 = "filesexpert_script";
							}
						}
					}
					if (pathFound2) {
						if (!p_story_mode) {
							if (!p_script) {
								log_file << "Disc 2 expert arena directory found." << std::endl;
								arenaName2 = "filesexpert";
							}
							else {
								log_file << "Disc 2 expert arena/retranslated script directory found." << std::endl;
								arenaName2 = "filesexpert_script";
							}
						}
					}
				}
				if (p_portraits) {
					if (pathFound1) {
						log_file << "Disc 1 resized portraits directory found." << std::endl;
						portraitsName1 = "portraits";
					}
					if (pathFound2) {
						log_file << "Disc 2 resized portraits directory found." << std::endl;
						portraitsName2 = "portraits";
					}
				}
				// Graphics edits outside of portraits are not applied to the items/spells or script patch here as they would cause crashes.
				if (p_graphics) {
					if (pathFound1) {
						if (!p_items_spells && !p_script) {
							if (!p_portraits) {
								log_file << "Disc 1 graphical fix directory found." << std::endl;
								graphicsName1 = "graphics";
							}
							else {
								log_file << "Disc 1 graphical fix without portraits directory found." << std::endl;
								graphicsName1 = "graphics_no_portraits";
							}
						}
						else if (!p_portraits) {
							log_file << "Disc 1 fixed portraits directory found." << std::endl;
							graphicsName1 = "graphics_portraits";
						}
					}
					if (pathFound2) {
						if (!p_items_spells && !p_script) {
							if (!p_portraits) {
								log_file << "Disc 2 graphical fix directory found." << std::endl;
								graphicsName2 = "graphics";
							}
							else {
								log_file << "Disc 2 graphical fix without portraits directory found." << std::endl;
								graphicsName2 = "graphics_no_portraits";
							}
						}
						else if (!p_portraits) {
							log_file << "Disc 2 fixed portraits directory found." << std::endl;
							graphicsName2 = "graphics_portraits";
						}
					}
				}
				if (p_voice) {
					if (pathFound1) {
						log_file << "Disc 1 undub battle voices directory found." << std::endl;
						voiceName1 = "voice";
					}
					if (pathFound2) {
						log_file << "Disc 2 undub battle voices directory found." << std::endl;
						voiceName2 = "voice";
					}
				}
				if (p_flashes) {
					if (pathFound1) {
						if (!p_items_spells) {
							log_file << "Disc 1 no battle flashes directory found." << std::endl;
							flashesName1 = "flashes";
						}
						else {
							log_file << "Disc 1 no battle flashes/rebalanced characters directory found." << std::endl;
							flashesName1 = "flashes_items";
						}
					}
					if (pathFound2) {
						if (!p_items_spells) {
							log_file << "Disc 2 no battle flashes directory found." << std::endl;
							flashesName2 = "flashes";
						}
						else {
							log_file << "Disc 2 no battle flashes/rebalanced characters directory found." << std::endl;
							flashesName2 = "flashes_items";
						}
					}
				}
				if (p_story_mode) {
					if (pathFound1) {
						if (!p_script) {
							log_file << "Disc 1 story mode directory found." << std::endl;
							storyModeName1 = "storyfiles_cd1";
						}
						else {
							log_file << "Disc 1 story mode/retranslated script directory found." << std::endl;
							storyModeName1 = "storyfiles_script_cd1";
						}
					}
					if (pathFound2) {
						if (!p_script) {
							log_file << "Disc 2 story mode directory found." << std::endl;
							storyModeName2 = "storyfiles_cd2";
						}
						else {
							log_file << "Disc 2 story mode/retranslated script directory found." << std::endl;
							storyModeName2 = "storyfiles_script_cd2";
						}
					}
				}
				// Bug patch is not applied with items/spells and battle flashes as they already have it applied.
				if (!p_items_spells && !p_flashes) {
					if (pathFound1) {
						log_file << "Disc 1 bug fix directory found." << std::endl;
						bugName1 = "bug_fix";

					}
					if (pathFound2) {
						log_file << "Disc 2 bug fix directory found." << std::endl;
						bugName2 = "bug_fix";
					}
				}
				if (pathFound1) {
					log_file << "Disc 1 title screen directory found." << std::endl;
					titleName1 = "title_screen";

				}
				if (pathFound2) {
					log_file << "Disc 2 title screen directory found." << std::endl;
					titleName2 = "title_screen";
				}
				initialisePatchLists();
				log_file << "Starting patch process." << std::endl;
				SetWindowText(hWnd, L"Patching...");
				log_file << "Changing cursor to reflect loading." << std::endl;
				SetCursor(LoadCursor(NULL, IDC_WAIT));
				// Apply disc 1 patches
				if (pathFound1) {
					log_file << "Applying disc 1 patches." << std::endl;
					if (applyPatch(1)) {
						log_file << "xenoiso process successful." << std::endl;
						successMessage = true;
					}
					else {
						log_file << "xenoiso process failed." << std::endl;
						successMessage = false;
					}
				}
				// Apply disc 2 patches
				if (pathFound2) {
					log_file << "Applying disc 2 patches." << std::endl;
					if (applyPatch(2)) {
						successMessage = true;
					}
					else {
						log_file << "xenoiso process failed." << std::endl;
						successMessage = false;
					}
				}
				SetWindowText(hWnd, szTitle);
				if (successMessage) {
					log_file << "Show success message." << std::endl;
					MessageBox(hWnd, L"Patch was completed successfully.", L"Success", MB_ICONASTERISK);
				}
				else {
					log_file << "Show failure message." << std::endl;
					MessageBox(hWnd, L"An error occurred with xenoiso. View pw_log for details.", L"Error", MB_ICONASTERISK);
				}
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
			log_file << "Open 'About' dialog." << std::endl;
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			// Close patcher
			log_file << "Closing the patcher." << std::endl;
			log_file << "Destroy window." << std::endl;
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
		log_file << "Begin drawing window." << std::endl;
		hdc = BeginPaint(hWnd, &ps);
		HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
		SetBkMode(hdc, TRANSPARENT);
		// TODO: Add any drawing code that uses hdc here...
		log_file << "Set font." << std::endl;
		SelectObject(hdc, hFont);
		log_file << "Draw window." << std::endl;
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
		log_file << "Draw upper rectangle." << std::endl;
		FillRect(hdc, &rc1, (HBRUSH)(COLOR_WINDOW));
		log_file << "Draw lower rectangle." << std::endl;
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
		log_file << "Exit program." << std::endl;
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
	log_file << "Initialise buttons." << std::endl;
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
	globalWindList.emplace_back(normalarena);
	globalWindList.emplace_back(storyMode);
	globalWindList.emplace_back(flashes);
}

// Initialise patch list
void initialisePatchLists() {
	log_file << "Initialise patch names." << std::endl;
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
	patchList1.emplace_back(flashesName1);
	patchList2.emplace_back(flashesName2);
}

// Lock checkboxes until a bin file has been found
void checkboxLock() {
	bool found;
	log_file << "Check if ROMs have been found." << std::endl;
	if (pathFound1 || pathFound2) {
		log_file << "ROM file has been found. Unlocking checkboxes." << std::endl;
		found = TRUE;
	}
	else {
		log_file << "ROM file has not been found. Lock checkboxes." << std::endl;
		found = FALSE;
		for (int i = 6; i < globalWindList.size(); i++) {
			LRESULT untick = SendMessage(globalWindList[i], BM_SETCHECK, BST_UNCHECKED, NULL);
		}
	}
	for (int i = 6; i < globalWindList.size(); i++) {
		EnableWindow(globalWindList[i], found);
	}
}

// Lock patch button until a box has been ticked
void patchBoxLock() {
	bool checkfound = false;
	log_file << "Checking if an option has been ticked." << std::endl;
	for (int i = 0; i < globalWindList.size(); i++) {
		LRESULT boxticked = SendMessage(globalWindList[i], BM_GETCHECK, NULL, NULL);
		if (boxticked == BST_CHECKED) {
			log_file << "An option has been ticked. Unlocking patch button." << std::endl;
			checkfound = true;
			EnableWindow(patchbutton, TRUE);
			break;
		}
	}
	if (!checkfound) {
		log_file << "No options have been ticked. Locking patch button." << std::endl;
		EnableWindow(patchbutton, FALSE);
	}
}

// Locks both patches and buttons
void relock() {
	log_file << "Resetting file paths." << std::endl;
	path1 = "";
	path2 = "";
	pathFound1 = false;
	pathFound2 = false;
	checkboxLock();
	patchBoxLock();
}

// Removes patch names
void reinitialisePatches() {
	log_file << "Clearing patch names." << std::endl;
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
	flashesName1 = "";
	flashesName2 = "";
	storyModeName1 = "";
	storyModeName2 = "";
	log_file << "Clearing patch lists." << std::endl;
	patchList1.clear();
	patchList2.clear();
}

// Removes file paths for bins
void clearText() {
	log_file << "Removing paths from windows." << std::endl;
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
	log_file << "Creating tooltip rectangle." << std::endl;
	GetClientRect(hParent, &toolInfo.rect);
	SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);
	SendMessage(hwndTip, TTM_SETMAXTIPWIDTH, 0, 255);
	return hwndTip;
}

// Generate tool tip
HWND toolGenerator(char* text, HWND hWnd, HWND hText) {
	wchar_t wtext[256];
	log_file << "Adding text to tooltip." << std::endl;
	mbstowcs(wtext, text, strlen(text) + 1);
	LPWSTR ptr = wtext;
	HWND hWndTT = CreateToolTip(hWnd, hText, hInst, ptr);
	return hWndTT;
}

// Define tool tips for each checkbox
void tooltipTextMaker(HWND hWnd) {
	log_file << "Defining tooltip messages." << std::endl;
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
		"using a texture hack, you may not need\n"
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
	char text_narena[] =
		"Uses the Battle Arena balancing\n"
		"from the base game.\n";
	HWND tt_narena = toolGenerator(text_narena, hWnd, normalarena);
	char text_story_mode[] =
		"Turns off random encounters entirely"
		"and ensures all enemies in the game\n"
		"can be killed in one hit.\n";
	HWND tt_story_mode = toolGenerator(text_story_mode, hWnd, storyMode);
	char text_flashes[] =
		"Turns off flashes during battle.";
	HWND tt_flashes = toolGenerator(text_flashes, hWnd, flashes);
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
	log_file << "Creating windows." << std::endl;
	cd1path = CreateWindow(L"EDIT", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, (int)(winX * 0.15), (int)(winY * 0.05), 500, 25, hWnd, NULL, hInst, NULL);
	cd2path = CreateWindow(L"EDIT", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, (int)(winX * 0.15), (int)(winY * 0.15), 500, 25, hWnd, NULL, hInst, NULL);
	browsebutton1 = CreateWindow(L"BUTTON", L"Browse", WS_CHILD | WS_VISIBLE, (int)(winX * 0.85), (int)(winY * 0.05), 70, 25, hWnd, (HMENU)9001, hInst, NULL);
	browsebutton2 = CreateWindow(L"BUTTON", L"Browse", WS_CHILD | WS_VISIBLE, (int)(winX * 0.85), (int)(winY * 0.15), 70, 25, hWnd, (HMENU)9001, hInst, NULL);
	aboutbutton = CreateWindow(L"BUTTON", L"About", WS_CHILD | WS_VISIBLE, (int)(winX * 0.85), (int)(winY * 0.75), 70, 25, hWnd, (HMENU)104, hInst, NULL);
	patchbutton = CreateWindow(L"BUTTON", L"Patch", WS_CHILD | WS_VISIBLE, (int)(winX * 0.85), (int)(winY * 0.25), 70, 25, hWnd, (HMENU)9003, hInst, NULL);
	encounters = CreateWindow(L"BUTTON", L"1/2 encounters", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * gameplayx), (int)(winY * 0.40), 90, 25, hWnd, (HMENU)9002, hInst, NULL);
	portraits = CreateWindow(L"BUTTON", L"Resized portraits", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * graphicsx), (int)(winY * 0.40), 100, 25, hWnd, (HMENU)9002, hInst, NULL);
	graphics = CreateWindow(L"BUTTON", L"Graphical fixes", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * graphicsx), (int)(winY * 0.47), 100, 25, hWnd, (HMENU)9002, hInst, NULL);
	expgold = CreateWindow(L"BUTTON", L"1.5x exp/gold", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * gameplayx), (int)(winY * 0.47), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	monsters = CreateWindow(L"BUTTON", L"Rebalanced enemies", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * gameplayx), (int)(winY * 0.54), 120, 25, hWnd, (HMENU)9002, hInst, NULL);
	normalarena = CreateWindow(L"BUTTON", L"Normal", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, (int)(winX * arenax), (int)(winY * 0.40), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	basicarena = CreateWindow(L"BUTTON", L"Basic mode", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, (int)(winX * arenax), (int)(winY * 0.47), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	expertarena = CreateWindow(L"BUTTON", L"Expert mode", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, (int)(winX * arenax), (int)(winY * 0.54), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	fmvs = CreateWindow(L"BUTTON", L"FMV undub", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * audiox), (int)(winY * 0.40), 100, 25, hWnd, (HMENU)9002, hInst, NULL);
	script = CreateWindow(L"BUTTON", L"Script/name changes", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * storyx), (int)(winY * 0.40), 120, 25, hWnd, (HMENU)9002, hInst, NULL);
	voice = CreateWindow(L"BUTTON", L"Battle undub", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * audiox), (int)(winY * 0.47), 90, 25, hWnd, (HMENU)9002, hInst, NULL);
	fasttext = CreateWindow(L"BUTTON", L"Fast text", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * storyx), (int)(winY * 0.47), 110, 25, hWnd, (HMENU)9002, hInst, NULL);
	itemspells = CreateWindow(L"BUTTON", L"Rebalanced party/items", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * gameplayx), (int)(winY * 0.61), 160, 25, hWnd, (HMENU)9002, hInst, NULL);
	storyMode = CreateWindow(L"BUTTON", L"Story mode", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * smx), (int)(winY * 0.40), 85, 25, hWnd, (HMENU)9002, hInst, NULL);
	flashes = CreateWindow(L"BUTTON", L"No battle flashes", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * graphicsx), (int)(winY * 0.54), 100, 25, hWnd, (HMENU)9002, hInst, NULL);
}

// Define font for global windows
void initialiseGlobalFont() {
	log_file << "Applying font to windows." << std::endl;
	for (int i = 0; i < globalWindList.size(); i++) {
		SendMessage(globalWindList[i], WM_SETFONT, (LPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
	}
}

void drawGUIText() {
	HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SelectObject(hdc, hFont);
	swprintf_s(graphicstext, 256, L"Graphics:      ");
	swprintf_s(gameplaytext, 256, L"Gameplay:  ");
	swprintf_s(arenatext, 256, L"Arena:  ");
	swprintf_s(storytext, 256, L"Story:    ");
	swprintf_s(audiotext, 256, L"Audio:    ");
	swprintf_s(modestext, 256, L"Modes:    ");
	TextOut(hdc, winX * graphicsx, winY * 0.35, graphicstext, wcslen(graphicstext));
	TextOut(hdc, winX * gameplayx, winY * 0.35, gameplaytext, wcslen(gameplaytext));
	TextOut(hdc, winX * arenax, winY * 0.35, arenatext, wcslen(arenatext));
	TextOut(hdc, winX * storyx, winY * 0.35, storytext, wcslen(storytext));
	TextOut(hdc, winX * audiox, winY * 0.35, audiotext, wcslen(audiotext));
	TextOut(hdc, winX * smx, winY * 0.35, modestext, wcslen(modestext));
	
}

void drawGlobalText() {
	HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SelectObject(hdc, hFont);
	swprintf_s(cd1text, 256, L"CD1 File:");
	swprintf_s(cd2text, 256, L"CD2 File:");
	TextOut(hdc, winX * 0.0325, winY * 0.05, cd1text, wcslen(cd1text));
	TextOut(hdc, winX * 0.0325, winY * 0.15, cd2text, wcslen(cd2text));
}

bool applyPatch(int discNum) {
	std::vector<std::string> patchList;
	std::string fileName;
	std::string oldPath;
	std::string cueName;
	std::string cdName;
	// Disc 1 data
	log_file << "Initialise patched ROM name and disc number to pass to xenoiso." << std::endl;
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
	// Detect if the patched ROM already exists.
	log_file << "Check if a patched ROM already exists." << std::endl;
	if (std::filesystem::exists(fileName)) {
		log_file << "Deleting existing patched ROM." << std::endl;
		remove(fileName.c_str());
	}
	// Create text file for xenoiso
	std::ofstream list_file;
	log_file << "Creating list file for xenoiso." << std::endl;
	list_file.open("list.txt", std::ios::trunc);
	// Create batch file for xdelta commands
	std::ofstream batch_file;
	log_file << "Creating xdelta command file." << std::endl;
	batch_file.open("commands.cmd", std::ios::trunc);
	if (!patchList.empty()) {
		// Create ROMs using xenoiso
		log_file << "Create temporary directory." << std::endl;
		std::string temp = "temp";
		std::filesystem::current_path(filePath);
		std::filesystem::create_directory(temp);
		log_file << "Copy files from each selected option into the temporary directory." << std::endl;
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
				if (encountersName1 != "") {
					std::filesystem::copy(encountersName1, temp, std::filesystem::copy_options::update_existing);
				}
			}
			if (discNum == 2) {
				if (encountersName2 != "") {
					std::filesystem::copy(encountersName2, temp, std::filesystem::copy_options::update_existing);
				}
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
				if (arenaName1 != "") {
					std::filesystem::copy(arenaName1, temp, std::filesystem::copy_options::update_existing);
				}
			}
			if (discNum == 2) {
				if (arenaName2 != "") {
					std::filesystem::copy(arenaName2, temp, std::filesystem::copy_options::update_existing);
				}
			}
		}
		if (p_items_spells) {
			if (discNum == 1) {
				if (itemspellsName1 != "") {
					std::filesystem::copy(itemspellsName1, temp, std::filesystem::copy_options::update_existing);
				}
			}
			if (discNum == 2) {
				if (itemspellsName2 != "") {
					std::filesystem::copy(itemspellsName2, temp, std::filesystem::copy_options::update_existing);
				}
			}
		}
		if (p_exp_gold) {
			if (discNum == 1) {
				if (expgoldName1 != "") {
					std::filesystem::copy(expgoldName1, temp, std::filesystem::copy_options::update_existing);
				}
			}
			if (discNum == 2) {
				if (expgoldName2 != "") {
					std::filesystem::copy(expgoldName2, temp, std::filesystem::copy_options::update_existing);
				}
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
		if (p_flashes) {
			if (discNum == 1) {
				std::filesystem::copy(flashesName1, temp, std::filesystem::copy_options::update_existing);
			}
			if (discNum == 2) {
				std::filesystem::copy(flashesName2, temp, std::filesystem::copy_options::update_existing);
			}
		}
		if (p_story_mode) {
			if (discNum == 1) {
				if (storyModeName1 != "") {
					std::filesystem::copy(storyModeName1, temp, std::filesystem::copy_options::update_existing);
				}
			}
			if (discNum == 2) {
				if (storyModeName2 != "") {
					std::filesystem::copy(storyModeName2, temp, std::filesystem::copy_options::update_existing);
				}
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
		if (p_fmv) {
			log_file << "Applying FMV undub." << std::endl;
			std::filesystem::current_path(home);
			std::string patchName;
			if (discNum == 1) {
				patchName = fmvName1;
			}
			if (discNum == 2) {
				patchName = fmvName2;
			}
			log_file << "Creating batch file commands." << std::endl;
			if (patched) {
				// Create copy of bin file to stack patches
				log_file << "Make backup of the ROM if it has already been patched." << std::endl;
				batch_file << "copy \"" + fileName + "\" backup.bin \n" << std::endl;
				batch_file << "del \"" + fileName + "\" \n" << std::endl;
				log_file << "Write command for xdelta to apply the FMV patches." << std::endl;
				batch_file << "xdelta3-3.0.11-i686.exe -d  -s backup.bin patches\\" + patchName + " \"" + fileName + "\" \n" << std::endl;
			}
			else {
				// Apply patches
				log_file << "Write command for xdelta to apply the FMV patches." << std::endl;
				changed = true;
				batch_file << "xdelta3-3.0.11-i686.exe -d  -s \"" + oldPath + "\" patches\\" + patchName + " \"" + fileName + "\" \n" << std::endl;
				patched = true;
			}
		}
		if (patched == true) {
			log_file << "Create a backup ROM if the FMV patch has been applied." << std::endl;
			oldPath = "backup.bin";
			batch_file << "copy \"" + fileName + "\" " + oldPath + "\n" << std::endl;
			batch_file << "del \"" + fileName + "\" \n" << std::endl;
		}
		log_file << "Write list file for xenoiso." << std::endl;
		list_file << cdName << "\n" << oldPath << "\n" << fileName << "\n" << "-1,.\\gamefiles\\temp" << std::flush;
		if (patched != true) {
			patched = true;
		}
	}
	list_file.close();
	batch_file.close();
	// Execute patch file
	if (p_fmv) {
		log_file << "Executing batch file. Applying FMV patch." << std::endl;
		int batch_exit_code = system("cmd.exe /c commands.cmd");
	}
	log_file << "Execute xenoiso." << std::endl;
	system("cmd.exe /c xenoiso list.txt -d");
	// Copy xenoiso log contents to pw_log
	std::ifstream xenoisoLog("log.txt");
	log_file << xenoisoLog.rdbuf() << "\n";
	// Remove batch and backup bin
	log_file << "Remove command file." << std::endl;
	remove("commands.cmd");
	log_file << "Remove backup ROM." << std::endl;
	remove("backup.bin");
	log_file << "Remove xenoiso list." << std::endl;
	remove("list.txt");
	std::filesystem::current_path(filePath);
	log_file << "Remove temp directory." << std::endl;
	std::filesystem::remove_all("temp");
	log_file << "Remove renamed ROMs." << std::endl;
	std::filesystem::current_path(home);
	if (discNum == 1) {
		std::filesystem::remove("Xenogears1.bin");
	}
	if (discNum == 2) {
		std::filesystem::remove("Xenogears2.bin");
	}
	// Create cue file
	log_file << "Create cue file." << std::endl;
	cue_stream.open(cueName, std::ios::out);
	cue_stream << "FILE \"" + fileName + "\" BINARY" << "\n";
	cue_stream << "  TRACK 01 MODE2/2352" << "\n";
	cue_stream << "    INDEX 01 00:00:00" << "\n";
	cue_stream.close();
	log_file << "Check filesize to determine if xenoiso executed successfully." << std::endl;
	int fileSize = std::filesystem::file_size(fileName);
	if (fileSize == 0) {
		log_file << "Failure to execute xenoiso. Returning error." << std::endl;
		return false;
	}
	else {
		log_file << "xenoiso executed successfully. Returning completion message." << std::endl;
		return true;
	}
}


