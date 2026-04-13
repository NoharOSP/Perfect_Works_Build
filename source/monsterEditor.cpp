#include "pch.h"
#include "monsterEditor.h"

void monsterEditor::verifyFiles() {
	if (patchProcessor::expName != "") {
		Window::log_file << "Applying exp changes." << std::endl;
	}
	if (patchProcessor::goldName != "") {
		Window::log_file << "Applying gold changes." << std::endl;
	}
	for (const auto& entry : std::filesystem::directory_iterator(applyPatch::temp)) {
		// Check if filename is between 2618 and 2768
		std::string trimfile = gameFileTools::fileTrim(entry.path().string());
		int fileNum = std::stoi(trimfile);
		if (fileNum < 2618) {
			continue;
		}
		if (fileNum > 2768) {
			continue;
		}
		// Edit monster file
		editMonster(entry.path().string());
	}
}

void monsterEditor::editMonster(std::string file) {
	// Open file
	std::fstream fileContents;
	fileContents.open(file, std::ios::in | std::ios::out | std::ios::binary);
	wchar_t buffer = 0;
	// Set length using first two bytes of a monster file
	fileContents.seekp(0, std::ios_base::beg);
	fileContents.read(reinterpret_cast<char*>(&buffer), 2);
	int length = buffer;
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
		// Establish an array which determines the position i jumps to
		int data[4];
		setMonsterData(data, gear);
		if (patchProcessor::expName != "") {
			editExp(&fileContents, i, nextpos, buffer, data);
		}
		if (patchProcessor::goldName != "") {
			editGold(&fileContents, i, nextpos, buffer, data);
		}
	}
	// Close file
	fileContents.close();
}

void monsterEditor::setMonsterData(int data[], bool gear) {
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
}

void monsterEditor::editExp(std::fstream* fileContents, int i, int nextpos, wchar_t buffer, int data[]) {
	// Find experience
	nextpos = i + data[2];
	fileContents->seekp(nextpos, std::ios_base::beg);
	fileContents->read(reinterpret_cast<char*>(&buffer), 4);
	uint64_t exp = buffer;
	if (windowHandler::expticked == BST_CHECKED) {
		exp = exp * 1.5;
	}
	if (windowHandler::expticked == BST_CHECKED) {
		exp = exp * 2;
	}
	nextpos = i + data[2];
	fileContents->seekp(nextpos, std::ios_base::beg);
	fileContents->write(reinterpret_cast<char*>(&exp), 4);
}

void monsterEditor::editGold(std::fstream* fileContents, int i, int nextpos, wchar_t buffer, int data[]) {
	// Find gold
	nextpos = i + data[3];
	fileContents->seekp(nextpos, std::ios_base::beg);
	fileContents->read(reinterpret_cast<char*>(&buffer), 2);
	uint64_t gold = buffer;
	if (windowHandler::goldticked == BST_CHECKED) {
		gold = gold * 1.5;
	}
	if (windowHandler::goldticked == BST_CHECKED) {
		gold = gold * 2;
	}
	nextpos = i + data[3];
	fileContents->seekp(nextpos, std::ios_base::beg);
	fileContents->write(reinterpret_cast<char*>(&gold), 2);
}
