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
	if (fmvName1 != "" || fmvName2 != "") {
		fmvVerify();
	}
	if (gamefileVerify()) {
		// Check for ticked boxes
		tickedBoxes();
	}
	initialisePatchLists();
	if (filePathValid) {
		pWin->log_file << "Check if the patcher is inside OneDrive." << std::endl;;
		if (oneDriveCheck()) {
			start();
		}
		// Restore defaults
		reinitialisePatches();
		clearPatchLists();
		pWin->restoreDefaults();
	}
	else {
		MessageBox(patchWnd, L"Could not find directory for 'gamefiles'. Check repo for latest version.", L"Error", MB_ICONERROR);
	}
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

void patchProcessor::fmvVerify() {
	// Access directory for patches if FMVs has been ticked
	pWin->log_file << "Check if 'patches' directory exists." << std::endl;
	if (std::filesystem::exists(patchPath)) {
		std::filesystem::current_path(patchPath);
		pWin->log_file << "'patches' directory is valid." << std::endl;
		patchPathValid = true;
		if (pWin->p_fmv) {
			pWin->log_file << "FMV patch file found." << std::endl;
			if (num == 1) {
				fmvPatch1 = "cd1_fmvs.xdelta";
				fmvName1 = "fmv1";
			}
			if (num == 2) {
				fmvPatch2 = "cd2_fmvs.xdelta";
				fmvName2 = "fmv2";
			}
		}
	}
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

void patchProcessor::tickedBoxes() {
	if (!storyModeVerify()) {
		if (!monsterVerify()) {
			expVerify();
			goldVerify();
			arenaVerify();
		}
		encounterVerify();
		itemSpellVerify();
		fastVerify();
		scriptVerify();
		portraitsVerify();
		graphicsVerify();
		voiceVerify();
		roniVerify();
		cafeVerify();
		bugVerify();
		titleVerify();
	}
}

bool patchProcessor::storyModeVerify() {
	if (pWin->p_story_mode) {
		if (!pWin->p_script) {
			pWin->log_file << "Story mode directory found." << std::endl;
			if (num == 1) {
				storyModeName1 = "storyfiles_cd1";
			}
			if (num == 2) {
				storyModeName2 = "storyfiles_cd2";
			}
		}
		else {
			pWin->log_file << "Story mode/retranslated script directory found." << std::endl;
			if (num == 1) {
				storyModeName1 = "storyfiles_script_cd1";
			}
			if (num == 2) {
				storyModeName2 = "storyfiles_script_cd2";
			}
		}
	}
	return pWin->p_story_mode;
}

bool patchProcessor::monsterVerify() {
	if (pWin->p_monsters) {
		// Check if items/spells and script patches aren't selected to avoid compatibility issues.
		if (!pWin->p_script && !pWin->p_items_spells) {
			pWin->log_file << "Rebalanced monsters directory found." << std::endl;
			monsterName = "Monsters";
		}
		else if (pWin->p_items_spells && !pWin->p_script) {
			pWin->log_file << "Rebalanced monsters and party directory found." << std::endl;
			monsterName = "monsters_items";
		}
		else if (!pWin->p_items_spells && pWin->p_script) {
			pWin->log_file << "Rebalanced monsters/retranslated script directory found." << std::endl;
			monsterName = "monsters_script";
		}
		else if (pWin->p_items_spells && pWin->p_script) {
			pWin->log_file << "Rebalanced monsters and party/retranslated script directory found." << std::endl;
			monsterName = "monsters_both";
		}
	}
	return pWin->p_monsters;
}

bool patchProcessor::expVerify() {
	bool exp = false;
	if (pWin->p_expone || pWin->p_exptwo) {
		pWin->log_file << "Experience directory found." << std::endl;
		expName = "og_monsters";
		exp = true;
	}
	return exp;
}

bool patchProcessor::goldVerify() {
	bool gold = false;
	if (pWin->p_goldone || pWin->p_goldtwo) {
		pWin->log_file << "Gold directory found." << std::endl;
		goldName = "og_monsters";
		gold = true;
	}
	return gold;
}

bool patchProcessor::encounterVerify() {
	if (pWin->p_encounters) {
		// Check if script patches aren't selected to avoid compatibility issues. Otherwise, a merged folder will be used
		if (!pWin->p_script) {
			pWin->log_file << "Half encounters directory found." << std::endl;
			if (num == 1) {
				encountersName1 = "encounter_rate_1";
			}
			if (num == 2) {
				encountersName2 = "encounter_rate_2";
			}
		}
		else {
			pWin->log_file << "Half encounters/retranslated script directory found." << std::endl;
			if (num == 1) {
				encountersName1 = "encounterone_script";
			}
			if (num == 2) {
				encountersName2 = "encountertwo_script";
			}
		}
	}
	return pWin->p_encounters;
}

bool patchProcessor::itemSpellVerify() {
	if (pWin->p_items_spells) {
		// Check if the items/script hybrid patch needs to be applied
		if (pWin->p_script) {
			pWin->log_file << "Rebalanced party/retranslated script directory found." << std::endl;
			if (num == 1) {
				itemspellsName1 = "Script_items";
			}
			if (num == 2) {
				itemspellsName2 = "Script_items2";
			}
		}
		else {
			pWin->log_file << "Rebalanced party directory found." << std::endl;
			if (num == 1) {
				itemspellsName1 = "items1";
			}
			if (num == 2) {
				itemspellsName2 = "items2";
			}
		}
	}
	return pWin->p_items_spells;
}

bool patchProcessor::fastVerify() {
	bool fast = false;
	if (pWin->p_fastnew) {
		pWin->log_file << "Retranslated fast text directory found." << std::endl;
		if (num == 1) {
			fastName1 = "text_cd1";
		}
		if (num == 2) {
			// Disc 2 will use the same patch regardless of whether the script patch has been applied as it has no cutscenes with auto-advance
			fastName2 = "text_cd2";
		}
		fast = true;

	}
	if (pWin -> p_fastold) {
		pWin->log_file << "Fast text directory found." << std::endl;
		if (num == 1) {
			fastName1 = "text_old1";
		}
		if (num == 2) {
			fastName2 = "text_cd2";
		}
		fast = true;
	}
	return fast;
}

bool patchProcessor::scriptVerify() {
	if (pWin->p_script) {
		// Check if items/spells and script hybrid won't be applied
		if (!pWin->p_items_spells) {
			pWin->log_file << "Retranslated script directory found." << std::endl;
			if (num == 1) {
				scriptName1 = "script1";
			}
			if (num == 2) {
				scriptName2 = "script2";
			}
		}
		else {
			pWin->log_file << "Retranslated script/rebalanced party directory found." << std::endl;
			if (num == 1) {
				scriptName1 = "Script_items";
			}
			else {
				scriptName2 = "Script_items2";
			}
		}
	}
	return pWin->p_script;
}

bool patchProcessor::arenaVerify() {
	bool arena = false;
	if (pWin->p_barena) {
		if (!pWin->p_script) {
			pWin->log_file << "Basic arena directory found." << std::endl;
			arenaName = "filesbasic";
		}
		else {
			pWin->log_file << "Basic arena/retranslated script directory found." << std::endl;
			arenaName = "filesbasic_script";
		}
		arena = true;
	}
	if (pWin->p_earena) {
		if (!pWin->p_script) {
			pWin->log_file << "Expert arena directory found." << std::endl;
			arenaName = "filesexpert";
		}
		else {
			pWin->log_file << "Expert arena/retranslated script directory found." << std::endl;
			arenaName = "filesexpert_script";
		}
		arena = true;
	}
	return arena;
}

bool patchProcessor::portraitsVerify() {
	if (pWin->p_portraits) {
		pWin->log_file << "Resized portraits directory found." << std::endl;
		portraitsName = "portraits";
	}
	return pWin->p_portraits;
}

bool patchProcessor::graphicsVerify() {
	bool graphics = false;
	if (pWin->p_graphics) {
		// Graphics edits outside of portraits are not applied to the items/spells or script patch here as they would cause crashes.
		if (!pWin->p_items_spells && !pWin->p_script) {
			if (!pWin->p_portraits) {
				pWin->log_file << "Graphical fix directory found." << std::endl;
				graphicsName = "graphics";
			}
			else {
				pWin->log_file << "Graphical fix without portraits directory found." << std::endl;
				graphicsName = "graphics_no_portraits";
			}
			graphics = true;
		}
		else if (!pWin->p_portraits) {
			pWin->log_file << "Fixed portraits directory found." << std::endl;
			graphicsName = "graphics_portraits";
			graphics = true;
		}
	}
	return graphics;
}

bool patchProcessor::voiceVerify() {
	if (pWin->p_voice) {
		pWin->log_file << "Undub in-game voice directory found." << std::endl;
		voiceName = "voice";
	}
	return pWin->p_voice;
}

bool patchProcessor::roniVerify() {
	if (pWin->p_roni) {
		if (!pWin->p_portraits) {
			pWin->log_file << "Non-resized Roni directory found." << std::endl;
			roniName = "roni_pw\\default";
		}
		else {
			pWin->log_file << "Resized Roni directory found." << std::endl;
			roniName = "roni_pw\\resized";
		}
	}
	return pWin->p_roni;
}

bool patchProcessor::cafeVerify() {
	bool cafe = false;
	if (pWin->p_cafe) {
		if (num == 1) {
			if (!pWin->p_script) {
				pWin->log_file << "Original script Emeralda diner fix directory found." << std::endl;
				cafeName = "emeralda_fix\\og_script";
			}
			else {
				pWin->log_file << "New script Emeralda diner fix directory found." << std::endl;
				cafeName = "emeralda_fix\\new_script";
			}
			cafe = true;
		}
	}
	return cafe;
}

bool patchProcessor::bugVerify() {
	bool bug = false;
	// Bug patch is not applied with items/spells and script as they already have it applied.
	if (!pWin->p_items_spells && !pWin->p_script) {
		pWin->log_file << "Bug fix directory found." << std::endl;
		if (num == 1) {
			bugName1 = "bug_fix1";
		}
		if (num == 2) {
			bugName2 = "bug_fix2";
		}
		bug = true;
	}
	return bug;
}

void patchProcessor::titleVerify() {
	pWin->log_file << "Title screen directory found." << std::endl;
	titleName = "title_screen";
}

// Initialise patch list
void patchProcessor::initialisePatchLists() {
	pWin->log_file << "Initialise patch names." << std::endl;
	if (num == 1) {
		patchList.emplace_back(encountersName1);
		patchList.emplace_back(itemspellsName1);
		patchList.emplace_back(fastName1);
		patchList.emplace_back(scriptName1);
		patchList.emplace_back(fmvName1);
		patchList.emplace_back(fmvPatch1);
		patchList.emplace_back(storyModeName1);
		patchList.emplace_back(bugName1);
	}
	if (num == 2) {
		patchList.emplace_back(encountersName2);
		patchList.emplace_back(itemspellsName2);
		patchList.emplace_back(fastName2);
		patchList.emplace_back(scriptName2);
		patchList.emplace_back(fmvName2);
		patchList.emplace_back(fmvPatch2);
		patchList.emplace_back(storyModeName2);
		patchList.emplace_back(bugName2);
	}
	patchList.emplace_back(portraitsName);
	patchList.emplace_back(graphicsName);
	patchList.emplace_back(expName);
	patchList.emplace_back(goldName);
	patchList.emplace_back(monsterName);
	patchList.emplace_back(arenaName);
	patchList.emplace_back(voiceName);
	patchList.emplace_back(titleName);
	patchList.emplace_back(roniName);
	patchList.emplace_back(cafeName);
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
	if (applyPatch()) {
		pWin->log_file << "xenoiso process successful." << std::endl;
		successMessage = true;
	}
	else {
		pWin->log_file << "xenoiso process failed." << std::endl;
		successMessage = false;
	}
	finish();
}


bool patchProcessor::applyPatch() {
	std::string fileName;
	std::string oldPath;
	std::string cueName;
	std::string cdName;
	bool patched = false;
	pWin->log_file << "Initialise patched ROM name and disc number to pass to xenoiso." << std::endl;
	// Disc 1 data
	if (num == 1) {
		fileName = "Xenogears_PW_CD1.bin";
		cueName = "Xenogears_PW_CD1.cue";
		if (space) {
			oldPath = tempPath;
		}
		else {
			oldPath = filePath;
		}
		cdName = "cd1";
	}
	// Disc 2 data
	if (num == 2) {
		fileName = "Xenogears_PW_CD2.bin";
		cueName = "Xenogears_PW_CD2.cue";
		if (space) {
			oldPath = tempPath;
		}
		else {
			oldPath = filePath;
		}
		cdName = "cd2fix";
	}
	// Return to home directory
	std::filesystem::current_path(pWin->home);
	// Detect if the patched ROM already exists.
	pWin->log_file << "Check if a patched ROM already exists." << std::endl;
	if (std::filesystem::exists(fileName)) {
		pWin->log_file << "Deleting existing patched ROM." << std::endl;
		remove(fileName.c_str());
	}
	// TODO: Move to function for creating files
	// Create text file for xenoiso
	std::ofstream list_file;
	pWin->log_file << "Creating list file for xenoiso." << std::endl;
	list_file.open("list.txt", std::ios::trunc);
	// Create batch file for xdelta commands
	std::ofstream batch_file;
	pWin->log_file << "Creating xdelta command file." << std::endl;
	batch_file.open("commands.cmd", std::ios::trunc);
	if (!patchList.empty()) {
		// Create ROMs using xenoiso
		pWin->log_file << "Create temporary directory." << std::endl;
		std::filesystem::current_path(gamefilePath);
		std::filesystem::create_directory(temp);
		// TODO: Move to function
		pWin->log_file << "Copy files from each selected option into the temporary directory." << std::endl;
		if (expName != "") {
			std::filesystem::copy(expName, temp, std::filesystem::copy_options::update_existing);
		}
		if (goldName != "") {
			std::filesystem::copy(goldName, temp, std::filesystem::copy_options::update_existing);
		}
		if (scriptName1 != "") {
			std::filesystem::copy(scriptName1, temp, std::filesystem::copy_options::overwrite_existing);
		}
		if (scriptName2 != "") {
			std::filesystem::copy(scriptName2, temp, std::filesystem::copy_options::overwrite_existing);
		}
		if (encountersName1 != "") {
			std::filesystem::copy(encountersName1, temp, std::filesystem::copy_options::update_existing);
		}
		if (encountersName2 != "") {
			std::filesystem::copy(encountersName2, temp, std::filesystem::copy_options::update_existing);
		}
		if (fastName1 != "" || fastName2 != "") {
			// Copy executable to temp
			if (!pWin->p_fmv) {
				std::filesystem::copy(exeName, temp, std::filesystem::copy_options::update_existing);
				pWin->log_file << "Applying text speed change to game's executable." << std::endl;
				for (const auto& entry : std::filesystem::directory_iterator(temp)) {
					exeEdits(entry.path().string());
				}
			}
			if (num == 1) {
				std::filesystem::copy(fastName1, temp, std::filesystem::copy_options::update_existing);
			}
			if (num == 2) {
				std::filesystem::copy(fastName2, temp, std::filesystem::copy_options::update_existing);
			}
		}
		if (portraitsName != "") {
			std::filesystem::copy(portraitsName, temp, std::filesystem::copy_options::update_existing);
		}
		if (graphicsName != "") {
			std::filesystem::copy(graphicsName, temp, std::filesystem::copy_options::update_existing);
		}
		if (arenaName != "") {
			std::filesystem::copy(arenaName, temp, std::filesystem::copy_options::update_existing);
		}
		if (itemspellsName1 != "") {
			std::filesystem::copy(itemspellsName1, temp, std::filesystem::copy_options::update_existing);
		}
		if (itemspellsName2 != "") {
			std::filesystem::copy(itemspellsName2, temp, std::filesystem::copy_options::update_existing);
		}
		if (monsterName != "") {
			std::filesystem::copy(monsterName, temp, std::filesystem::copy_options::update_existing);
		}
		if (voiceName != "") {
			std::filesystem::copy(voiceName, temp, std::filesystem::copy_options::update_existing);
		}
		if (roniName != "") {
			std::filesystem::copy(roniName, temp, std::filesystem::copy_options::update_existing);
		}
		if (cafeName != "") {
			std::filesystem::copy(cafeName, temp, std::filesystem::copy_options::update_existing);
		}
		if (storyModeName1 != "") {
			std::filesystem::copy(storyModeName1, temp, std::filesystem::copy_options::overwrite_existing);
		}
		if (storyModeName2 != "") {
			std::filesystem::copy(storyModeName2, temp, std::filesystem::copy_options::overwrite_existing);
		}
		if (bugName1 != "") {
			std::filesystem::copy(bugName1, temp, std::filesystem::copy_options::update_existing);
		}
		if (bugName2 != "") {
			std::filesystem::copy(bugName2, temp, std::filesystem::copy_options::overwrite_existing);
		}
		if (titleName != "") {
			std::filesystem::copy(titleName, temp, std::filesystem::copy_options::update_existing);
		}
		if (expName != "" || goldName != "") {
			// Iterate through enemy files to apply exp or gold changes
			if (expName != "") {
				pWin->log_file << "Applying exp changes." << std::endl;
			}
			if (goldName != "") {
				pWin->log_file << "Applying gold changes." << std::endl;
			}
			for (const auto& entry : std::filesystem::directory_iterator(temp)) {
				monsterEdits(entry.path().string());
			}
		}
		if (fmvName1 != "") {
			std::filesystem::copy(fmvName1, temp, std::filesystem::copy_options::update_existing);
		}
		if (fmvName2 != "") {
			std::filesystem::copy(fmvName2, temp, std::filesystem::copy_options::update_existing);
		}
		if (pWin->p_flashes) {
			pWin->log_file << "Applying battle executable changes." << std::endl;
			for (const auto& entry : std::filesystem::directory_iterator(temp)) {
				battleExeEdits(entry.path().string());
			}
		}
		std::filesystem::current_path(pWin->home);
		changed = true;
		if (fmvName1 != "" || fmvName2 != "") {
			// TODO: Move to function
			pWin->log_file << "Applying FMV undub." << std::endl;
			std::filesystem::current_path(pWin->home);
			std::string patchName;
			if (num == 1) {
				patchName = fmvPatch1;
			}
			if (num == 2) {
				patchName = fmvPatch2;
			}
			pWin->log_file << "Creating batch file commands." << std::endl;
			if (patched) {
				// Create copy of bin file to stack patches
				pWin->log_file << "Make backup of the ROM if it has already been patched." << std::endl;
				batch_file << "copy \"" + fileName + "\" backup.bin \n" << std::endl;
				batch_file << "del \"" + fileName + "\" \n" << std::endl;
				pWin->log_file << "Write command for xdelta to apply the FMV patches." << std::endl;
				batch_file << "Tools\\xdelta3-3.0.11-i686.exe -d  -s backup.bin patches\\" + patchName + " \"" + fileName + "\" \n" << std::endl;
				if (num == 1) {
					batch_file << "Tools\\Insert_pWin->log_file.exe " + fileName + " Tools\\xenocd1_softmod_files_extended.txt -new" << std::endl;
				}
				if (num == 2) {
					batch_file << "Tools\\Insert_pWin->log_file.exe " + fileName + " Tools\\xenocd2_softmod_files_extended.txt -new" << std::endl;
				}
			}
			else {
				// Apply patches
				pWin->log_file << "Write command for xdelta to apply the FMV patches." << std::endl;
				changed = true;
				batch_file << "Tools\\xdelta3-3.0.11-i686.exe -d  -s \"" + oldPath + "\" patches\\" + patchName + " \"" + fileName + "\" \n" << std::endl;
				if (num == 1) {
					batch_file << "Tools\\Insert_pWin->log_file.exe " + fileName + " Tools\\xenocd1_softmod_files_extended.txt -new" << std::endl;
				}
				if (num == 2) {
					batch_file << "Tools\\Insert_pWin->log_file.exe " + fileName + " Tools\\xenocd2_softmod_files_extended.txt -new" << std::endl;
				}
				patched = true;
			}
		}
		if (patched == true) {
			// TODO: Move to function
			pWin->log_file << "Create a backup ROM if the FMV patch has been applied." << std::endl;
			oldPath = "backup.bin";
			batch_file << "copy \"" + fileName + "\" " + oldPath + "\n" << std::endl;
			batch_file << "del \"" + fileName + "\" \n" << std::endl;
		}
		pWin->log_file << "Write list file for xenoiso." << std::endl;
		list_file << cdName << "\n" << oldPath << "\n" << fileName << "\n" << "-1,.\\gamefiles\\temp" << std::flush;
		pWin->log_file << "Check if FMVs have been ticked." << std::endl;
		if (patched != true) {
			patched = true;
		}
	}
	list_file.close();
	batch_file.close();
	// Execute patch file
	if (fmvName1 != "" || fmvName2 != "") {
		pWin->log_file << "Executing batch file. Applying FMV patch and rewriting the file table." << std::endl;
		int batch_exit_code = system("cmd.exe /c commands.cmd");
	}
	pWin->log_file << "Execute xenoiso." << std::endl;
	try {
		// Detect errors with xenoiso
		int batch_exit_code = system("cmd.exe /c Tools\\xenoiso list.txt");
	}
	catch (const std::system_error& error) {
		// Catch errors when opening xenoiso
		pWin->log_file << &error << std::endl;
	}
	// TODO: Move to function
	// Insert new SLUS
	if (fmvName1 != "" || fmvName2 != "") {
		if (fastName1 != "" || fastName2 != "") {
			// Add fast text to softsubs SLUS
			std::filesystem::current_path(gamefilePath);
			if (num == 1) {
				std::filesystem::copy(slusDisc1, temp, std::filesystem::copy_options::update_existing);
			}
			if (num == 2) {
				std::filesystem::copy(slusDisc2, temp, std::filesystem::copy_options::update_existing);
			}
			pWin->log_file << "Applying text speed change to game's executable." << std::endl;
			for (const auto& entry : std::filesystem::directory_iterator(temp)) {
				exeEdits(entry.path().string());
			}
			std::filesystem::current_path(pWin->home);
		}
		// Create batch file to make a new SLUS
		std::ofstream batch_file2;
		pWin->log_file << "Creating new SLUS file." << std::endl;
		batch_file2.open("commands2.cmd", std::ios::trunc);
		if (num == 1) {
			if (fastName1 == "") {
				batch_file2 << "Tools\\Xeno_slus_ins.exe " + fileName + " gamefiles\\temp\\SLUS_006.64" << std::endl;
			}
			else {
				batch_file2 << "Tools\\Xeno_slus_ins.exe " + fileName + " gamefiles\\sub_executable\\disc1\\SLUS_006.64" << std::endl;
			}
		}
		if (num == 2) {
			if (fastName2 == "") {
				batch_file2 << "Tools\\Xeno_slus_ins.exe " + fileName + " gamefiles\\temp\\SLUS_006.69" << std::endl;
			}
			else {
				batch_file2 << "Tools\\Xeno_slus_ins.exe " + fileName + " gamefiles\\sub_executable\\disc2\\SLUS_006.69" << std::endl;
			}
		}
		batch_file2.close();
		int batch_exit_code = system("cmd.exe /c commands2.cmd");
		pWin->log_file << "Remove new SLUS command file." << std::endl;
		remove("commands2.cmd");
	}
	// TODO: Move to function
	// Remove batch and backup bin
	pWin->log_file << "Remove command file." << std::endl;
	remove("commands.cmd");
	pWin->log_file << "Remove backup ROM." << std::endl;
	remove("backup.bin");
	pWin->log_file << "Remove xenoiso list." << std::endl;
	remove("list.txt");
	std::filesystem::current_path(gamefilePath);
	pWin->log_file << "Remove temp directory." << std::endl;
	std::filesystem::remove_all("temp");
	pWin->log_file << "Remove renamed ROMs." << std::endl;
	std::filesystem::current_path(pWin->home);
	if (num == 1) {
		std::filesystem::remove("Xenogears1.bin");
	}
	if (num == 2) {
		std::filesystem::remove("Xenogears2.bin");
	}
	// Create cue file
	pWin->log_file << "Create cue file." << std::endl;
	cue_stream.open(cueName, std::ios::out);
	cue_stream << "FILE \"" + fileName + "\" BINARY" << "\n";
	cue_stream << "  TRACK 01 MODE2/2352" << "\n";
	cue_stream << "    INDEX 01 00:00:00" << "\n";
	cue_stream.close();
	pWin->log_file << "Check filesize to determine if xenoiso executed successfully." << std::endl;
	int fileSize = std::filesystem::file_size(fileName);
	if (fileSize == 0) {
		pWin->log_file << "Failure to execute xenoiso. Returning error." << std::endl;
		return false;
	}
	else {
		pWin->log_file << "xenoiso executed successfully. Returning completion message." << std::endl;
		return true;
	}


}

void patchProcessor::exeEdits(std::string file) {
	// Apply fast text changes
	std::string trimfile = file;
	trimfile.erase(0, 5);
	if (fmvName1 == "" || fmvName2 == "") {
		// Check if filename is 0022
		if (trimfile != "0022") {
			return;
		}
	}
	else {
		if (num == 1) {
			// Check if filename is SLUS_006.64
			if (trimfile != "SLUS_006.64") {
				return;
			}
		}
		if (num == 2) {
			// Check if filename is SLUS_006.69
			if (trimfile != "SLUS_006.69") {
				return;
			}
		}
	}
	// Open file
	std::fstream fileContents;
	fileContents.open(file, std::ios::in | std::ios::out | std::ios::binary);
	// Find the position of the text speed value
	fileContents.seekp(151908, std::ios_base::beg);
	int speed = 0x05;
	fileContents.write(reinterpret_cast <char*>(&speed), 2);
	// Apply additional FMV version edits
	if (fmvName1 != "" || fmvName2 != "") {
		fileContents.seekp(151911, std::ios_base::beg);
		int nextval = 0x34;
		fileContents.write(reinterpret_cast <char*>(&nextval), 2);
	}
	// Close file
	fileContents.close();
}

void patchProcessor::monsterEdits(std::string file) {
	// Apply 1.5 exp or 1.5 gold changes
	// Check if filename is between 2618 and 2768
	std::string trimfile = file;
	trimfile.erase(0, 5);
	int fileNum = 0;
	if (trimfile[0] == '2') {
		if (trimfile[1] == '6' || trimfile[1] == '7') {
			if (trimfile[4] != '.') {
				fileNum = std::stoi(trimfile);
				if (fileNum < 2618) {
					return;
				}
			}
			else {
				return;
			}
		}
		else {
			return;
		}
	}
	else {
		return;
	}
	// Open file
	std::fstream fileContents;
	fileContents.open(file, std::ios::in | std::ios::out | std::ios::binary);
	wchar_t buffer;
	buffer = 0;
	// Set length using first two bytes of a monster file
	fileContents.seekp(0, std::ios_base::beg);
	fileContents.read(reinterpret_cast<char*>(&buffer), 2);
	int length = buffer;
	// Iterate through each monster
	for (int i = 126; i < length; i = i + 368) {
		// Read HP
		fileContents.seekp(i, std::ios_base::beg);
		fileContents.read(reinterpret_cast<char*>(&buffer), 2);
		uint64_t hp = buffer;
		// Read max HP
		int nextpos = i + 2;
		fileContents.seekp(nextpos, std::ios_base::beg);
		fileContents.read(reinterpret_cast<char*>(&buffer), 2);
		uint64_t mhp = buffer;
		bool gear;
		// Check if the monster is a gear
		if (hp == 0 || mhp == 0) {
			gear = true;
		}
		else {
			gear = false;
		}
		// Establish an array which determines the position i jumps to
		int data[4];
		if (gear) {
			data[0] = 0xb8;
			data[1] = 0xbc;
			data[2] = 0x100;
			data[3] = 0x10a;
		}
		else {
			data[0] = 0;
			data[1] = 2;
			data[2] = 0x100;
			data[3] = 0x10a;
		}
		if (expName != "") {
			// Find experience
			nextpos = i + data[2];
			fileContents.seekp(nextpos, std::ios_base::beg);
			fileContents.read(reinterpret_cast<char*>(&buffer), 4);
			uint64_t exp = buffer;
			if (num == 1) {
				exp = exp * 1.5;
			}
			if (num == 2) {
				exp = exp * 2;
			}
			nextpos = i + data[2];
			fileContents.seekp(nextpos, std::ios_base::beg);
			fileContents.write(reinterpret_cast<char*>(&exp), 4);
		}
		if (goldName != "") {
			// Find gold
			nextpos = i + data[3];
			fileContents.seekp(nextpos, std::ios_base::beg);
			fileContents.read(reinterpret_cast<char*>(&buffer), 2);
			uint64_t gold = buffer;
			if (num == 1) {
				gold = gold * 1.5;
			}
			if (num == 2) {
				gold = gold * 2;
			}
			nextpos = i + data[3];
			fileContents.seekp(nextpos, std::ios_base::beg);
			fileContents.write(reinterpret_cast<char*>(&gold), 2);
		}
	}
	// Close file
	fileContents.close();
}

// Remove battle flashes
void patchProcessor::battleExeEdits(std::string file) {
	std::string trimfile = file;
	trimfile.erase(0, 5);
	// Check if filename is 0038
	if (trimfile != "0038") {
		return;
	}
	// Decompress file
	std::filesystem::current_path(pWin->home);
	int batch_decompress = system("Tools\\xenocomp.exe -d gamefiles\\temp\\0038 gamefiles\\temp\\0038.dec");
	std::filesystem::current_path(gamefilePath);
	std::filesystem::current_path(temp);
	std::string decomp = "0038.dec";
	// Open file
	std::fstream fileContents;
	fileContents.open(decomp, std::ios::in | std::ios::out | std::ios::binary);
	// Find the position of the text speed value
	fileContents.seekp(278032, std::ios_base::beg);
	int flash = 0x00;
	fileContents.write(reinterpret_cast <char*>(&flash), 2);
	// Close file
	fileContents.close();
	// Recompress file
	std::filesystem::current_path(pWin->home);
	int batch_compress = system("Tools\\xenocomp.exe -c gamefiles\\temp\\0038.dec gamefiles\\temp\\0038");
	// Remove decompressed file
	std::filesystem::current_path(filePath);
	std::filesystem::current_path(temp);
	remove("0038.dec");
}

void patchProcessor::finish() {
	SetWindowText(patchWnd, pWin->title);
	if (successMessage) {
		pWin->log_file << "Show success message." << std::endl;
		MessageBox(patchWnd, L"Patch was completed successfully.", L"Success", MB_ICONASTERISK);
	}
	else {
		pWin->log_file << "Show failure message." << std::endl;
		MessageBox(patchWnd, L"An error occurred with xenoiso. View pw_log for details.", L"Error", MB_ICONASTERISK);
	}
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