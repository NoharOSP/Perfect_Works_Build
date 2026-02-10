#include "pch.h"
#include "patchProcessor.h"

patchProcessor::patchProcessor(Window* win, HWND hWnd, int discNum, std::string path) {
	// Initialise variables
	patchWnd = hWnd;
	num = discNum;
	filePath = path;
	pWin = win;
	prepare();
}

patchProcessor::~patchProcessor() {
	
}

void patchProcessor::prepare() {
	SetWindowText(patchWnd, L"Preparing...");
	// Work around path names with whitespace.
	pWin->log_file << "Check if disc filename has whitespace characters." << std::endl;
	if (filePath.find(' ') != std::string::npos) {
		pWin->log_file << "Whitespace characters found. Creating a copy of the ROM inside the home directory." << std::endl;
		removeWhitespace();
	}
	std::filesystem::current_path(pWin->home);
	patchVerifier pv(pWin, this, num);
	if (pWin->p_fmv) {
		pv.fmvVerify();
		std::filesystem::current_path(pWin->home);
	}
	if (gamefileVerify()) {
		// Check for ticked boxes
		pv.verify();
	}
	initialisePatchLists();
	if (filePathValid) {
		pWin->log_file << "Check if the patcher is inside OneDrive." << std::endl;;
		if (oneDriveCheck()) {
			start();
		}
	}
	else {
		MessageBox(patchWnd, L"Could not find directory for 'gamefiles'. Check repo for latest version.", L"Error", MB_ICONERROR);
	}
	pv.~patchVerifier();
}

void patchProcessor::removeWhitespace() {
	SetWindowText(patchWnd, L"Copying files...");
	std::filesystem::current_path(pWin->home);
	std::string curPath;
	if (num == 1) {
		tempPath = "Xenogears1.bin";
	}
	if (num == 2) {
		tempPath = "Xenogears2.bin";
	}
	std::ifstream src(filePath, std::ios::binary);
	std::ofstream dst(tempPath, std::ios::binary);
	dst << src.rdbuf();
	pWin->log_file << "Copying completed." << std::endl;
	space = true;
}

bool patchProcessor::gamefileVerify() {
	// Access directory for files
	pWin->log_file << "Check if 'gamefiles' directory is valid." << std::endl;
	if (std::filesystem::exists(gamefilePath)) {
		pWin->log_file << "'gamefiles' directory is valid." << std::endl;
		std::filesystem::current_path(gamefilePath);
		filePathValid = true;
	}
	return filePathValid;
}

// Initialise patch list
void patchProcessor::initialisePatchLists() {
	pWin->log_file << "Initialise patch names." << std::endl;
	patchList.emplace_back(encountersName);
	patchList.emplace_back(itemspellsName);
	patchList.emplace_back(fastName);
	patchList.emplace_back(scriptName);
	patchList.emplace_back(fmvName);
	patchList.emplace_back(fmvPatch);
	patchList.emplace_back(storyModeName);
	patchList.emplace_back(bugName);
	patchList.emplace_back(resizeName);
	patchList.emplace_back(portraitsName);
	patchList.emplace_back(expName);
	patchList.emplace_back(goldName);
	patchList.emplace_back(monsterName);
	patchList.emplace_back(arenaName);
	patchList.emplace_back(voiceName);
	patchList.emplace_back(titleName);
	patchList.emplace_back(roniName);
	patchList.emplace_back(cafeName);
	patchList.emplace_back(deathblowName);
	patchList.emplace_back(jpnName);
}

bool patchProcessor::oneDriveCheck() {
	bool safeDrive = false;
	if (pWin->home.contains("OneDrive")) {
		pWin->log_file << "Display OneDrive error." << std::endl;
		MessageBox(patchWnd, L"The patcher is in the OneDrive. It cannot be used.", L"Error", MB_ICONASTERISK);
		pWin->log_file << "Abort patching process." << std::endl;
		SetWindowText(patchWnd, pWin->title);
	}
	else {
		pWin->log_file << "OneDrive is not in use. Resume execution." << std::endl;
		safeDrive = true;
	}
	return safeDrive;
}

void patchProcessor::start() {
	pWin->log_file << "Starting patch process." << std::endl;
	SetWindowText(patchWnd, L"Patching...");
	pWin->log_file << "Changing cursor to reflect loading." << std::endl;
	SetCursor(LoadCursor(NULL, IDC_WAIT));
	// Apply patches
	pWin->log_file << "Applying patches." << std::endl;
	applyPatch ap(pWin, num, this);
	if (ap.patch()) {
		pWin->log_file << "xenoiso process successful." << std::endl;
		successMessage = true;
	}
	else {
		pWin->log_file << "xenoiso process failed." << std::endl;
		successMessage = false;
	}
	if (num == 1) {
		if (!pWin->pathFound2) {
			finish();
		}
	}
	else {
		finish();
	}
}

void patchProcessor::finish() {
	SetWindowText(patchWnd, pWin->title);
	if (successMessage) {
		pWin->log_file << "Show success message." << std::endl;
		MessageBox(patchWnd, L"Patch was completed successfully. The completed ROM will be available as Xenogears_PW_CD1 or Xenogears_PW_CD2.", L"Success", MB_ICONASTERISK);
	}
	else {
		pWin->log_file << "Show failure message." << std::endl;
		MessageBox(patchWnd, L"An error occurred with xenoiso. View pw_log for details.", L"Error", MB_ICONASTERISK);
	}
	// Restore defaults
	reinitialisePatches();
	clearPatchLists();
	pWin->restoreDefaults();
}

// Removes patch names
void patchProcessor::reinitialisePatches() {
	pWin->log_file << "Clearing patch names." << std::endl;
	for (int i = 0; i < patchList.size(); i = i + 1) {
		patchList[i] = "";
	}
}

// Clear patch lists
void patchProcessor::clearPatchLists() {
	pWin->log_file << "Clearing patch lists." << std::endl;
	patchList.clear();
}