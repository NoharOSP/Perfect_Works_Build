#include "pch.h"
#include "fileEditor.h"

void fileEditor::graphicEdits() {
	if (windowHandler::flashesticked == BST_CHECKED) {
		// Remove battle flashes
		graphicalEditor::battleExeEdits();
	}
}

void fileEditor::gameplayEdits() {
	if (patchProcessor::expName != "" || patchProcessor::goldName != "") {
		// Iterate through enemy files to apply exp or gold changes
		monsterEditor::verifyFiles();
	}
	if (windowHandler::deathblowsticked == BST_CHECKED) {
		Window::log_file << "Applying ability learning level changes." << std::endl;
		for (const auto& entry : std::filesystem::directory_iterator(applyPatch::temp)) {
			expRateEdits(entry.path().string());
		}
	}
}

void fileEditor::scriptEdits() {
	if (windowHandler::fastticked == BST_CHECKED) {
		if (!windowHandler::fmvticked == BST_CHECKED) {
			Window::log_file << "Applying text speed change to game's executable." << std::endl;
			for (const auto& entry : std::filesystem::directory_iterator(applyPatch::temp)) {
				exeEdits(entry.path().string());
			}
		}
	}
}

void fileEditor::audioEdits() {
	if (windowHandler::musicticked == BST_CHECKED) {
		if (windowHandler::scriptticked == BST_CHECKED || windowHandler::fastticked == BST_CHECKED || (windowHandler::basicarenaticked || windowHandler::expertarenaticked)) {
			Window::log_file << "Applying music placement edits." << std::endl;
			std::vector<std::string> musicFileList;
			for (const auto& entry : std::filesystem::directory_iterator(musicpath)) {
				musicFileList.emplace_back(entry);
			}
			for (const auto& entry : std::filesystem::directory_iterator(applyPatch::temp)) {
				if (std::find(musicFileList.begin(), musicFileList.end(), entry) != musicFileList.end()) {
					audioEditor::musicEdits(entry.path().string());
				}
			}
		}
	}
}

void fileEditor::modeEdits() {
	if (windowHandler::jpnticked == BST_CHECKED) {
		Window::log_file << "Applying Japanese control changes." << std::endl;
		for (const auto& entry : std::filesystem::directory_iterator(applyPatch::temp)) {
			controlEditor::addImage(entry.path().string());
			if (!windowHandler::fmvticked == BST_CHECKED) {
				controlEditor::editExecutable(entry.path().string());
			}
			controlEditor::editBattleExe(entry.path().string());
		}
	}
}

void fileEditor::editSLUS(std::string romFile) {
	// Insert new SLUS
	if (patchProcessor::fastName != "" || patchProcessor::jpnName != "") {
		// Add fast text to softsubs SLUS
		std::filesystem::current_path(patchProcessor::gamefilePath);
		if (patchProcessor::num == 1) {
			std::filesystem::copy(patchProcessor::slusDisc1, applyPatch::temp, std::filesystem::copy_options::update_existing);
		}
		if (patchProcessor::num == 2) {
			std::filesystem::copy(patchProcessor::slusDisc2, applyPatch::temp, std::filesystem::copy_options::update_existing);
		}
		if (patchProcessor::jpnName != "") {
			Window::log_file << "Applying control edits to game's executable." << std::endl;
			for (const auto& entry : std::filesystem::directory_iterator(applyPatch::temp)) {
				controlEditor::editExecutable(entry.path().string());
			}
		}
		if (patchProcessor::fastName != "") {
			Window::log_file << "Applying text speed change to game's executable." << std::endl;
			for (const auto& entry : std::filesystem::directory_iterator(applyPatch::temp)) {
				exeEdits(entry.path().string());
			}
		}
		std::filesystem::current_path(Window::home);
	}
	// Create batch file to make a new SLUS
	makeSLUS(romFile);
}

void fileEditor::makeSLUS(std::string romFile) {
	std::ofstream batch_file2;
	Window::log_file << "Creating new SLUS file." << std::endl;
	batch_file2.open("commands2.cmd", std::ios::trunc);
	if (patchProcessor::num == 1) {
		if (patchProcessor::fastName != "" || patchProcessor::jpnName != "") {
			batch_file2 << "Tools\\Xeno_slus_ins.exe " + romFile + " gamefiles\\temp\\SLUS_006.64" << std::endl;
		}
		else {
			batch_file2 << "Tools\\Xeno_slus_ins.exe " + romFile + " gamefiles\\sub_executable\\disc1\\SLUS_006.64" << std::endl;
		}
	}
	if (patchProcessor::num == 2) {
		if (patchProcessor::fastName != "") {
			batch_file2 << "Tools\\Xeno_slus_ins.exe " + romFile + " gamefiles\\temp\\SLUS_006.69" << std::endl;
		}
		else {
			batch_file2 << "Tools\\Xeno_slus_ins.exe " + romFile + " gamefiles\\sub_executable\\disc2\\SLUS_006.69" << std::endl;
		}
	}
	batch_file2.close();
	int batch_exit_code = system("cmd.exe /c commands2.cmd");
	Window::log_file << "Remove new SLUS command file." << std::endl;
	remove("commands2.cmd");
}

void fileEditor::exeEdits(std::string file) {
	std::string trimfile = gameFileTools::fileTrim(file);
	if (patchProcessor::fmvName == "") {
		if (patchProcessor::num == 1) {
			// Check if filename is 0022
			if (trimfile != "0022") {
				return;
			}
		}
		if (patchProcessor::num == 2) {
			// Check if filename is 0017
			if (trimfile != "0017") {
				return;
			}
		}
	}
	else {
		if (patchProcessor::num == 1) {
			// Check if filename is SLUS_006.64
			if (trimfile != "SLUS_006.64") {
				return;
			}
		}
		if (patchProcessor::num == 2) {
			// Check if filename is SLUS_006.69
			if (trimfile != "SLUS_006.69") {
				return;
			}
		}
	}
	editTextSpeed(file);
}

void fileEditor::editTextSpeed(std::string file) {
	// Open file
	std::fstream fileContents;
	fileContents.open(file, std::ios::in | std::ios::out | std::ios::binary);
	// Find the position of the text speed value
	fileContents.seekp(151908, std::ios_base::beg);
	int speed = 0x05;
	fileContents.write(reinterpret_cast <char*>(&speed), 2);
	fileContents.seekp(151911, std::ios_base::beg);
	int nextval = 0x34;
	fileContents.write(reinterpret_cast <char*>(&nextval), 2);
	// Close file
	fileContents.close();
}

void fileEditor::expRateEdits(std::string file) {
	std::string trimfile = gameFileTools::fileTrim(file);
	// Check if filename is 2607
	if (trimfile != "2607.unk4") {
		return;
	}
	std::filesystem::current_path(Window::home);
	std::filesystem::current_path(patchProcessor::gamefilePath);
	std::filesystem::current_path(applyPatch::temp);
	partyStatEditor pse;
	pse.deathblowLevels();
	std::filesystem::current_path("..\\");
}