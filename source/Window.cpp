#include "pch.h"
#include "Window.h"

Window::Window(HWND hWnd, HINSTANCE hInst, int axisX, int axisY, LPWSTR szTitle) {
	// Initialise variables
	winHwnd = hWnd;
	winInst = hInst;
	winX = axisX;
	winY = axisY;
	title = szTitle;
	// Initialise home directory
	home = std::filesystem::current_path().string();
	std::filesystem::current_path(home);
	// Create log
	log_file.open("pw_log.txt", std::ios::trunc);
	// Create window
	log_file << "Creating windows." << std::endl;
	initialise();
}

Window::~Window() {
}

void Window::initialise() {
	// Initialise windows
	initialiseWindows();
	initialiseFont();
	checkboxLock();
	patchBoxLock();
	tooltipTextMaker();
}

void Window::initialiseWindows() {
	// Define windows
	createPaths();
	createButtons();
	graphicWindows();
	gameplayWindows();
	arenaWindows();
	storyWindows();
	audioWindows();
	modeWindows();
}

void Window::initialiseFont() {
	log_file << "Applying font to windows." << std::endl;
	// Apply font to paths
	for (int i = 0; i < pathList.size(); i++) {
		SendMessage(pathList[i], WM_SETFONT, (LPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
	}
	// Apply font to buttons
	for (int i = 0; i < buttonList.size(); i++) {
		SendMessage(buttonList[i], WM_SETFONT, (LPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
	}
	// Apply font to each window in windList
	for (int i = 0; i < windList.size(); i++) {
		SendMessage(windList[i], WM_SETFONT, (LPARAM)GetStockObject(DEFAULT_GUI_FONT), NULL);
	}

}


void Window::checkboxLock() {
	log_file << "Check if ROMs have been found." << std::endl;
	// Lock checkboxes until a bin file has been found
	if (pathFound1 || pathFound2) {
		found = TRUE;
	}
	else {
		log_file << "ROM file has not been found. Lock checkboxes." << std::endl;
		found = FALSE;
		for (int i = 0; i < windList.size(); i++) {
			LRESULT untick = SendMessage(windList[i], BM_SETCHECK, BST_UNCHECKED, NULL);
		}
	}
	// Enable the patch window if a ROM has been found
	for (int i = 0; i < windList.size(); i++) {
		EnableWindow(windList[i], found);
		if (windList[i] == normalarena) {
			LRESULT tick = SendMessage(windList[i], BM_SETCHECK, BST_CHECKED, NULL);
		}
	}
}

void Window::patchBoxLock() {
	// Lock patch button until a box has been ticked
	// Check if a path is available first
	if (pathFound1 || pathFound2) {
		log_file << "Checking if an option has been ticked." << std::endl;
		for (int i = 0; i < windList.size(); i++) {
			LRESULT boxticked = SendMessage(windList[i], BM_GETCHECK, NULL, NULL);
			if (boxticked == BST_CHECKED) {
				log_file << "An option has been ticked. Unlocking patch button." << std::endl;
				checkfound = true;
			}
		}
		if (!checkfound) {
			log_file << "No options have been ticked. Locking patch button." << std::endl;
			checkfound = false;
		}
	}
	else {
		checkfound = false;
	}
	// Enable patch button if an option has been ticked
	EnableWindow(patchbutton, checkfound);
}

// TODO: Move to new class?
void Window::tooltipTextMaker() {
	// Define tool tips for each checkbox
	log_file << "Defining tooltip messages." << std::endl;
	tipDesc tips;
	// Graphics
	HWND tt_portraits = toolGenerator(tips.text_portraits, winHwnd, portraits, winInst).hWndTT;
	HWND tt_graphics = toolGenerator(tips.text_graphics, winHwnd, graphics, winInst).hWndTT;
	HWND tt_flashes = toolGenerator(tips.text_flashes, winHwnd, flashes, winInst).hWndTT;
	HWND tt_roni = toolGenerator(tips.text_roni, winHwnd, roni, winInst).hWndTT;
	HWND tt_cafe = toolGenerator(tips.text_cafe, winHwnd, cafe, winInst).hWndTT;
	// Gameplay
	HWND tt_encounters = toolGenerator(tips.text_encounters, winHwnd, encounters, winInst).hWndTT;
	HWND tt_expone = toolGenerator(tips.text_expone, winHwnd, experience1, winInst).hWndTT;
	HWND tt_exptwo = toolGenerator(tips.text_exptwo, winHwnd, experience2, winInst).hWndTT;
	HWND tt_goldone = toolGenerator(tips.text_goldone, winHwnd, gold1, winInst).hWndTT;
	HWND tt_goldtwo = toolGenerator(tips.text_goldtwo, winHwnd, gold2, winInst).hWndTT;
	HWND tt_itemspells = toolGenerator(tips.text_itemspells, winHwnd, itemspells, winInst).hWndTT;
	HWND tt_monsters = toolGenerator(tips.text_monsters, winHwnd, monsters, winInst).hWndTT;
	// Arena
	HWND tt_barena = toolGenerator(tips.text_barena, winHwnd, basicarena, winInst).hWndTT;
	HWND tt_earena = toolGenerator(tips.text_earena, winHwnd, expertarena, winInst).hWndTT;
	HWND tt_narena = toolGenerator(tips.text_narena, winHwnd, normalarena, winInst).hWndTT;
	// Story
	HWND tt_fast = toolGenerator(tips.text_fast, winHwnd, fasttext, winInst).hWndTT;
	HWND tt_script = toolGenerator(tips.text_script, winHwnd, script, winInst).hWndTT;
	// Audio
	HWND tt_fmvs = toolGenerator(tips.text_fmvs, winHwnd, fmvs, winInst).hWndTT;
	HWND tt_voice = toolGenerator(tips.text_voices, winHwnd, voice, winInst).hWndTT;
	// Mode
	HWND tt_story_mode = toolGenerator(tips.text_story_mode, winHwnd, storyMode, winInst).hWndTT;
}

void Window::createPaths() {
	// Create windows for ROM paths
	cd1path = CreateWindow(L"EDIT", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, (int)(winX * 0.15), (int)(winY * 0.05), 500, 25, winHwnd, NULL, winInst, NULL);
	cd2path = CreateWindow(L"EDIT", NULL, WS_BORDER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, (int)(winX * 0.15), (int)(winY * 0.12), 500, 25, winHwnd, NULL, winInst, NULL);
	// Put in window list
	pathList.emplace_back(cd1path);
	pathList.emplace_back(cd2path);
}

void Window::createButtons() {
	// Create buttons
	browsebutton1 = CreateWindow(L"BUTTON", L"Browse", WS_CHILD | WS_VISIBLE, (int)(winX * 0.85), (int)(winY * 0.05), 70, 25, winHwnd, (HMENU)9001, winInst, NULL);
	browsebutton2 = CreateWindow(L"BUTTON", L"Browse", WS_CHILD | WS_VISIBLE, (int)(winX * 0.85), (int)(winY * 0.12), 70, 25, winHwnd, (HMENU)9001, winInst, NULL);
	aboutbutton = CreateWindow(L"BUTTON", L"About", WS_CHILD | WS_VISIBLE, (int)(winX * 0.85), (int)(winY * 0.87), 70, 25, winHwnd, (HMENU)104, winInst, NULL);
	patchbutton = CreateWindow(L"BUTTON", L"Patch", WS_CHILD | WS_VISIBLE, (int)(winX * 0.85), (int)(winY * 0.20), 70, 25, winHwnd, (HMENU)9003, winInst, NULL);
	// Put in window list
	buttonList.emplace_back(browsebutton1);
	buttonList.emplace_back(browsebutton2);
	buttonList.emplace_back(aboutbutton);
	buttonList.emplace_back(patchbutton);
}

void Window::graphicWindows() {
	// Create graphic patch windows
	portraits = CreateWindow(L"BUTTON", L"Resized portraits", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * graphicsx), (int)(winY * 0.36), 100, 25, winHwnd, (HMENU)9002, winInst, NULL);
	graphics = CreateWindow(L"BUTTON", L"Graphical fixes", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * graphicsx), (int)(winY * 0.43), 100, 25, winHwnd, (HMENU)9002, winInst, NULL);
	flashes = CreateWindow(L"BUTTON", L"No battle flashes", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * graphicsx), (int)(winY * 0.50), 100, 25, winHwnd, (HMENU)9002, winInst, NULL);
	roni = CreateWindow(L"BUTTON", L"PW Roni", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * graphicsx), (int)(winY * 0.57), 100, 25, winHwnd, (HMENU)9002, winInst, NULL);
	cafe = CreateWindow(L"BUTTON", L"Emeralda cafe fix", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * graphicsx), (int)(winY * 0.64), 100, 25, winHwnd, (HMENU)9002, winInst, NULL);
	// Put in window list
	windList.emplace_back(portraits);
	windList.emplace_back(graphics);
	windList.emplace_back(flashes);
	windList.emplace_back(roni);
	windList.emplace_back(cafe);
}

void Window::gameplayWindows() {
	// Create gameplay patch windows
	encounters = CreateWindow(L"BUTTON", L"1/2 encounters", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * gameplayx), (int)(winY * 0.36), 90, 25, winHwnd, (HMENU)9002, winInst, NULL);
	experience1 = CreateWindow(L"BUTTON", L"1.5x exp", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * gameplayx), (int)(winY * 0.43), 110, 25, winHwnd, (HMENU)9002, winInst, NULL);
	experience2 = CreateWindow(L"BUTTON", L"2x exp", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * gameplayx), (int)(winY * 0.50), 110, 25, winHwnd, (HMENU)9002, winInst, NULL);
	gold1 = CreateWindow(L"BUTTON", L"1.5x gold", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * gameplayx), (int)(winY * 0.57), 110, 25, winHwnd, (HMENU)9002, winInst, NULL);
	gold2 = CreateWindow(L"BUTTON", L"2x gold", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * gameplayx), (int)(winY * 0.64), 110, 25, winHwnd, (HMENU)9002, winInst, NULL);
	monsters = CreateWindow(L"BUTTON", L"Rebalanced enemies", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * gameplayx), (int)(winY * 0.71), 120, 25, winHwnd, (HMENU)9002, winInst, NULL);
	itemspells = CreateWindow(L"BUTTON", L"Rebalanced party/items", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * gameplayx), (int)(winY * 0.78), 160, 25, winHwnd, (HMENU)9002, winInst, NULL);
	// Put in window list
	windList.emplace_back(encounters);
	windList.emplace_back(experience1);
	windList.emplace_back(experience2);
	windList.emplace_back(gold1);
	windList.emplace_back(gold2);
	windList.emplace_back(itemspells);
	windList.emplace_back(monsters);
}

void Window::arenaWindows() {
	// Create arena patch windows
	normalarena = CreateWindow(L"BUTTON", L"Normal", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, (int)(winX * arenax), (int)(winY * 0.36), 110, 25, winHwnd, (HMENU)9002, winInst, NULL);
	basicarena = CreateWindow(L"BUTTON", L"Basic mode", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, (int)(winX * arenax), (int)(winY * 0.43), 110, 25, winHwnd, (HMENU)9002, winInst, NULL);
	expertarena = CreateWindow(L"BUTTON", L"Expert mode", WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON, (int)(winX * arenax), (int)(winY * 0.50), 110, 25, winHwnd, (HMENU)9002, winInst, NULL);
	// Put in window list
	windList.emplace_back(normalarena);
	windList.emplace_back(basicarena);
	windList.emplace_back(expertarena);
}

void Window::storyWindows() {
	// Create story patch windows
	script = CreateWindow(L"BUTTON", L"Script/name changes", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * storyx), (int)(winY * 0.36), 120, 25, winHwnd, (HMENU)9002, winInst, NULL);
	fasttext = CreateWindow(L"BUTTON", L"Fast text", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * storyx), (int)(winY * 0.43), 110, 25, winHwnd, (HMENU)9002, winInst, NULL);
	// Put in window list
	windList.emplace_back(fasttext);
	windList.emplace_back(script);
}

void Window::audioWindows() {
	// Create audio patch windows
	fmvs = CreateWindow(L"BUTTON", L"FMV undub", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * audiox), (int)(winY * 0.36), 100, 25, winHwnd, (HMENU)9002, winInst, NULL);
	voice = CreateWindow(L"BUTTON", L"Battle undub", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * audiox), (int)(winY * 0.43), 90, 25, winHwnd, (HMENU)9002, winInst, NULL);
	// Put in window list
	windList.emplace_back(fmvs);
	windList.emplace_back(voice);
}

void Window::modeWindows() {
	// Create mode patch windows
	storyMode = CreateWindow(L"BUTTON", L"Story mode", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, (int)(winX * smx), (int)(winY * 0.36), 85, 25, winHwnd, (HMENU)9002, winInst, NULL);
	// Put in window list
	windList.emplace_back(storyMode);
}

// TODO: Consider moving to a new class (romFinder?)
void Window::openFile(HWND hWnd) {
	// Open ROM files
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
			else if (discNum == 2) {
				log_file << "Disc 2 found." << std::endl;
				pathFound2 = true;
				log_file << "Determine disc 2 path." << std::endl;
				path2 = path;
				std::wstring wpath = std::wstring(path2.begin(), path2.end());
				LPCWSTR lpath = wpath.c_str();
				log_file << "Put disc 2 path in path window." << std::endl;
				SetWindowText(cd2path, lpath);
			}
			else {
				log_file << "The selected file is not a valid Xenogears ROM." << std::endl;
				MessageBox(hWnd, L"The bin is not valid.", L"Error", MB_ICONERROR);
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

// TODO: Move to new class
// Handle window selection process
void Window::windowSelect() {
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
		if (p_story_mode == true) {
			log_file << "Unticking story mode." << std::endl;
			LRESULT smuntick = SendMessage(storyMode, BM_SETCHECK, BST_UNCHECKED, NULL);
			p_story_mode = false;
		}
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
	LRESULT exponeticked = SendMessage(experience1, BM_GETCHECK, NULL, NULL);
	if (exponeticked == BST_CHECKED) {
		log_file << "1.5x exp ticked." << std::endl;
		p_expone = true;
		if (p_exptwo == true) {
			log_file << "Unticking 2x exp." << std::endl;
			LRESULT exptwountick = SendMessage(experience2, BM_SETCHECK, BST_UNCHECKED, NULL);
			p_exptwo = false;
		}
		if (p_story_mode == true) {
			log_file << "Unticking story mode." << std::endl;
			LRESULT smuntick = SendMessage(storyMode, BM_SETCHECK, BST_UNCHECKED, NULL);
			p_story_mode = false;
		}
	}
	else {
		log_file << "1.5x exp unticked." << std::endl;
		p_expone = false;
	}
	LRESULT exptwoticked = SendMessage(experience2, BM_GETCHECK, NULL, NULL);
	if (exptwoticked == BST_CHECKED) {
		log_file << "2x exp ticked." << std::endl;
		p_exptwo = true;
		if (p_expone == true) {
			log_file << "Unticking 1.5x exp." << std::endl;
			LRESULT exponeuntick = SendMessage(experience1, BM_SETCHECK, BST_UNCHECKED, NULL);
			p_expone = false;
		}
		if (p_story_mode == true) {
			log_file << "Unticking story mode." << std::endl;
			LRESULT smuntick = SendMessage(storyMode, BM_SETCHECK, BST_UNCHECKED, NULL);
			p_story_mode = false;
		}
	}
	else {
		log_file << "2x exp unticked." << std::endl;
		p_exptwo = false;
	}
	LRESULT goldoneticked = SendMessage(gold1, BM_GETCHECK, NULL, NULL);
	if (goldoneticked == BST_CHECKED) {
		log_file << "1.5x gold ticked." << std::endl;
		p_goldone = true;
		if (p_goldtwo == true) {
			log_file << "Unticking 2x gold." << std::endl;
			LRESULT goldtwountick = SendMessage(gold2, BM_SETCHECK, BST_UNCHECKED, NULL);
			p_goldtwo = false;
		}
		if (p_story_mode == true) {
			log_file << "Unticking story mode." << std::endl;
			LRESULT smuntick = SendMessage(storyMode, BM_SETCHECK, BST_UNCHECKED, NULL);
			p_story_mode = false;
		}
	}
	else {
		log_file << "1.5x gold unticked." << std::endl;
		p_goldone = false;
	}
	LRESULT goldtwoticked = SendMessage(gold2, BM_GETCHECK, NULL, NULL);
	if (goldtwoticked == BST_CHECKED) {
		log_file << "2x gold ticked." << std::endl;
		p_goldtwo = true;
		if (p_goldone == true) {
			log_file << "Unticking 1.5x gold." << std::endl;
			LRESULT goldoneuntick = SendMessage(gold1, BM_SETCHECK, BST_UNCHECKED, NULL);
			p_goldone = false;
		}
		if (p_story_mode == true) {
			log_file << "Unticking story mode." << std::endl;
			LRESULT smuntick = SendMessage(storyMode, BM_SETCHECK, BST_UNCHECKED, NULL);
			p_story_mode = false;
		}
	}
	else {
		log_file << "2x gold unticked." << std::endl;
		p_goldtwo = false;
	}
	LRESULT itemspellsticked = SendMessage(itemspells, BM_GETCHECK, NULL, NULL);
	if (itemspellsticked == BST_CHECKED) {
		log_file << "Rebalanced party/items ticked." << std::endl;
		p_items_spells = true;
		if (p_story_mode == true) {
			log_file << "Unticking story mode." << std::endl;
			LRESULT smuntick = SendMessage(storyMode, BM_SETCHECK, BST_UNCHECKED, NULL);
			p_story_mode = false;
		}
	}
	else {
		log_file << "Rebalanced party/items unticked." << std::endl;
		p_items_spells = false;
	}
	LRESULT monstersticked = SendMessage(monsters, BM_GETCHECK, NULL, NULL);
	if (monstersticked == BST_CHECKED) {
		log_file << "Rebalanced monsters ticked." << std::endl;
		p_monsters = true;
		if (p_story_mode == true) {
			log_file << "Unticking story mode." << std::endl;
			LRESULT smuntick = SendMessage(storyMode, BM_SETCHECK, BST_UNCHECKED, NULL);
			p_story_mode = false;
		}
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
		if (p_story_mode == true) {
			log_file << "Unticking story mode." << std::endl;
			LRESULT smuntick = SendMessage(storyMode, BM_SETCHECK, BST_UNCHECKED, NULL);
			p_story_mode = false;
		}
	}
	else {
		log_file << "Basic arena deselected." << std::endl;
		p_barena = false;
	}
	LRESULT expertarenaticked = SendMessage(expertarena, BM_GETCHECK, NULL, NULL);
	if (expertarenaticked == BST_CHECKED) {
		log_file << "Expert arena selected." << std::endl;
		p_earena = true;
		if (p_story_mode == true) {
			log_file << "Unticking story mode." << std::endl;
			LRESULT smuntick = SendMessage(storyMode, BM_SETCHECK, BST_UNCHECKED, NULL);
			p_story_mode = false;
		}
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
	LRESULT roniticked = SendMessage(roni, BM_GETCHECK, NULL, NULL);
	if (roniticked == BST_CHECKED) {
		log_file << "Perfect Works Roni ticked." << std::endl;
		p_roni = true;
	}
	else {
		log_file << "Perfect Works Roni unticked." << std::endl;
		p_roni = false;
	}
	LRESULT cafeticked = SendMessage(cafe, BM_GETCHECK, NULL, NULL);
	if (cafeticked == BST_CHECKED) {
		log_file << "Emeralda diner fix ticked." << std::endl;
		p_cafe = true;
	}
	else {
		log_file << "Emeralda diner fix unticked." << std::endl;
		p_cafe = false;
	}
	LRESULT storymodeticked = SendMessage(storyMode, BM_GETCHECK, NULL, NULL);
	if (storymodeticked == BST_CHECKED) {
		log_file << "Story mode ticked." << std::endl;
		p_story_mode = true;
		log_file << "Unticking incompatible patches." << std::endl;
		p_encounters = false;
		LRESULT enctick = SendMessage(encounters, BM_SETCHECK, BST_UNCHECKED, NULL);
		p_expone = false;
		LRESULT exponeuntick = SendMessage(experience1, BM_SETCHECK, BST_UNCHECKED, NULL);
		p_exptwo = false;
		LRESULT exptwountick = SendMessage(experience2, BM_SETCHECK, BST_UNCHECKED, NULL);
		p_goldone = false;
		LRESULT goldoneuntick = SendMessage(gold1, BM_SETCHECK, BST_UNCHECKED, NULL);
		p_goldtwo = false;
		LRESULT goldtwountick = SendMessage(gold2, BM_SETCHECK, BST_UNCHECKED, NULL);
		p_monsters = false;
		LRESULT monuntick = SendMessage(monsters, BM_SETCHECK, BST_UNCHECKED, NULL);
		p_items_spells = false;
		LRESULT itemuntick = SendMessage(itemspells, BM_SETCHECK, BST_UNCHECKED, NULL);
		p_barena = false;
		p_earena = false;
		LRESULT normtick = SendMessage(normalarena, BM_SETCHECK, BST_CHECKED, NULL);
		LRESULT basicuntick = SendMessage(basicarena, BM_SETCHECK, BST_UNCHECKED, NULL);
		LRESULT expertuntick = SendMessage(expertarena, BM_SETCHECK, BST_UNCHECKED, NULL);
	}
	else {
		log_file << "Story mode unticked." << std::endl;
		p_story_mode = false;
	}
	// Unlock patch button
	patchBoxLock();
}

void Window::process() {
	// Check if both discs have paths available
	log_file << "Preparing to patch." << std::endl;
	if (pathFound1) {
		patchProcessor pp1(this, winHwnd, 1, path1);
	}
	if (pathFound2) {
		patchProcessor pp2(this, winHwnd, 2, path2);
	}
}

// Restores default patcher settings
void Window::restoreDefaults() {
	relock();
	clearText();
}

// Locks both patches and buttons
void Window::relock() {
	log_file << "Resetting file paths." << std::endl;
	path1 = "";
	path2 = "";
	pathFound1 = false;
	pathFound2 = false;
	checkboxLock();
	patchBoxLock();
}

// Removes file paths for bins
void Window::clearText() {
	log_file << "Removing paths from windows." << std::endl;
	SetWindowText(cd1path, L"");
	SetWindowText(cd2path, L"");
}

// TODO: Shorten or move to new class
// Draw window
void Window::paintProcess(HWND hWnd) {
	PAINTSTRUCT ps;
	log_file << "Begin drawing window." << std::endl;
	hdc = BeginPaint(hWnd, &ps);
	HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
	SetBkMode(hdc, TRANSPARENT);
	log_file << "Set font." << std::endl;
	SelectObject(hdc, hFont);
	log_file << "Draw window." << std::endl;
	FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW));
	GetClientRect(hWnd, &rcWindow);
	RECT rc1, rc2;
	rc1, rc2 = rcWindow;
	rc1.top = winY * 0.025;
	rc1.left = winX / 50;
	rc1.right = winX - (rc1.left * 2);
	rc1.bottom = winY * 0.27;
	rc2.top = winY * 0.30;
	rc2.left = rc1.left;
	rc2.right = rc1.right;
	rc2.bottom = winY * 0.85;
	Rectangle(hdc, rc1.left, rc1.top, rc1.right, rc1.bottom);
	Rectangle(hdc, rc2.left, rc2.top, rc2.right, rc2.bottom);
	log_file << "Draw upper rectangle." << std::endl;
	FillRect(hdc, &rc1, (HBRUSH)(COLOR_WINDOW));
	log_file << "Draw lower rectangle." << std::endl;
	FillRect(hdc, &rc2, (HBRUSH)(COLOR_WINDOW));
	FrameRect(hdc, &rc1, CreateSolidBrush(RGB(220, 220, 220)));
	FrameRect(hdc, &rc2, CreateSolidBrush(RGB(220, 220, 220)));
	drawText();
	EndPaint(hWnd, &ps);
}

void Window::drawText() {
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
	TextOut(hdc, winX * 0.0325, winY * 0.05, cd1text, wcslen(cd1text));
	TextOut(hdc, winX * 0.0325, winY * 0.12, cd2text, wcslen(cd2text));
	TextOut(hdc, winX * graphicsx, winY * 0.32, graphicstext, wcslen(graphicstext));
	TextOut(hdc, winX * gameplayx, winY * 0.32, gameplaytext, wcslen(gameplaytext));
	TextOut(hdc, winX * arenax, winY * 0.32, arenatext, wcslen(arenatext));
	TextOut(hdc, winX * storyx, winY * 0.32, storytext, wcslen(storytext));
	TextOut(hdc, winX * audiox, winY * 0.32, audiotext, wcslen(audiotext));
	TextOut(hdc, winX * smx, winY * 0.32, modestext, wcslen(modestext));
}