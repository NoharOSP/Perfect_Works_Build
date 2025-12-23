#include "pch.h"
#include "Window.h"

Window::Window(HWND hWnd, HINSTANCE hInst, int axisX, int axisY, LPWSTR szTitle) {
	// Initialise variables
	winHwnd = hWnd;
	winInst = hInst;
	winX = axisX;
	winY = axisY;
	title = szTitle;
	pHandle = new windowHandler(this);
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

void Window::openFile(HWND hWnd) {
	// Open ROM files
	log_file << "Open file browser." << std::endl;
	romFinder rf(this);
	rf.browseFiles();
}

// Handle window selection process
void Window::windowSelect() {
	if (pHandle->check()) {
		patchBoxLock();
	}
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

// Draw window
void Window::paintProcess() {
	paintWindow pw(this);
	pw.paint();
}