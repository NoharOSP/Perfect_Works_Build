#include "pch.h"
#include "fileEditor.h"

fileEditor::fileEditor(patchProcessor* processor, Window* window, int discNum, std::string temp) {
	pp = processor;
	num = discNum;
	pWin = window;
	tempDir = temp;
}

fileEditor::~fileEditor() {

}

void fileEditor::monsterEdits(std::string file) {
	// Check if filename is between 2618 and 2768
	std::string trimfile = fileTrim(file);
	int fileNum = std::stoi(trimfile);
	if (fileNum < 2618) {
		return;
	}
	if (fileNum > 2768) {
		return;
	}
	// Iterate through each monster
	iterateMonster(file);
}

std::string fileEditor::fileTrim(std::string file) {
	std::string trimfile = file;
	trimfile.erase(0, 5);
	return trimfile;
}

void fileEditor::iterateMonster(std::string file) {
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
		if (pp->expName != "") {
			editExp(&fileContents, i, nextpos, buffer, data);
		}
		if (pp->goldName != "") {
			editGold(&fileContents, i, nextpos, buffer, data);
		}
	}
	// Close file
	fileContents.close();
}

void fileEditor::setMonsterData(int data[], bool gear) {
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

void fileEditor::editExp(std::fstream* fileContents, int i, int nextpos, wchar_t buffer, int data[]) {
	// Find experience
	nextpos = i + data[2];
	fileContents->seekp(nextpos, std::ios_base::beg);
	fileContents->read(reinterpret_cast<char*>(&buffer), 4);
	uint64_t exp = buffer;
	if (num == 1) {
		exp = exp * 1.5;
	}
	if (num == 2) {
		exp = exp * 2;
	}
	nextpos = i + data[2];
	fileContents->seekp(nextpos, std::ios_base::beg);
	fileContents->write(reinterpret_cast<char*>(&exp), 4);
}

void fileEditor::editGold(std::fstream* fileContents, int i, int nextpos, wchar_t buffer, int data[]) {
	// Find gold
	nextpos = i + data[3];
	fileContents->seekp(nextpos, std::ios_base::beg);
	fileContents->read(reinterpret_cast<char*>(&buffer), 2);
	uint64_t gold = buffer;
	if (num == 1) {
		gold = gold * 1.5;
	}
	if (num == 2) {
		gold = gold * 2;
	}
	nextpos = i + data[3];
	fileContents->seekp(nextpos, std::ios_base::beg);
	fileContents->write(reinterpret_cast<char*>(&gold), 2);
}

void fileEditor::exeEdits(std::string file) {
	std::string trimfile = fileTrim(file);
	if (pp->fmvName == "") {
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
	if (pp->fmvName != "") {
		fileContents.seekp(151911, std::ios_base::beg);
		int nextval = 0x34;
		fileContents.write(reinterpret_cast <char*>(&nextval), 2);
	}
	// Close file
	fileContents.close();
}

// Remove battle flashes
void fileEditor::battleExeEdits(std::string file) {
	std::string trimfile = fileTrim(file);
	// Check if filename is 0038
	if (trimfile != "0038") {
		return;
	}
	// Decompress file
	std::filesystem::current_path(pWin->home);
	int batch_decompress = system("Tools\\xenocomp.exe -d gamefiles\\temp\\0038 gamefiles\\temp\\0038.dec");
	std::filesystem::current_path(pp->gamefilePath);
	std::filesystem::current_path(tempDir);
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
	std::filesystem::current_path(pp->gamefilePath);
	std::filesystem::current_path(tempDir);
	remove("0038.dec");
}

void fileEditor::editSLUS(std::string romFile) {
	// Insert new SLUS
	if (pp->fastName != "") {
		// Add fast text to softsubs SLUS
		std::filesystem::current_path(pp->gamefilePath);
		if (num == 1) {
			std::filesystem::copy(pp->slusDisc1, tempDir, std::filesystem::copy_options::update_existing);
		}
		if (num == 2) {
			std::filesystem::copy(pp->slusDisc2, tempDir, std::filesystem::copy_options::update_existing);
		}
		pWin->log_file << "Applying text speed change to game's executable." << std::endl;
		for (const auto& entry : std::filesystem::directory_iterator(tempDir)) {
			exeEdits(entry.path().string());
		}
		std::filesystem::current_path(pWin->home);
	}
	// Create batch file to make a new SLUS
	makeSLUS(romFile);
}

void fileEditor::makeSLUS(std::string romFile) {
	std::ofstream batch_file2;
	pWin->log_file << "Creating new SLUS file." << std::endl;
	batch_file2.open("commands2.cmd", std::ios::trunc);
	if (num == 1) {
		if (pp->fastName != "") {
			batch_file2 << "Tools\\Xeno_slus_ins.exe " + romFile + " gamefiles\\temp\\SLUS_006.64" << std::endl;
		}
		else {
			batch_file2 << "Tools\\Xeno_slus_ins.exe " + romFile + " gamefiles\\sub_executable\\disc1\\SLUS_006.64" << std::endl;
		}
	}
	if (num == 2) {
		if (pp->fastName != "") {
			batch_file2 << "Tools\\Xeno_slus_ins.exe " + romFile + " gamefiles\\temp\\SLUS_006.69" << std::endl;
		}
		else {
			batch_file2 << "Tools\\Xeno_slus_ins.exe " + romFile + " gamefiles\\sub_executable\\disc2\\SLUS_006.69" << std::endl;
		}
	}
	batch_file2.close();
	int batch_exit_code = system("cmd.exe /c commands2.cmd");
	pWin->log_file << "Remove new SLUS command file." << std::endl;
	remove("commands2.cmd");
}