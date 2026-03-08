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

void fileEditor::exeEdits(std::string file) {
	std::string trimfile = gameFileTools::fileTrim(file);
	if (pp->fmvName == "") {
		if (num == 1) {
			// Check if filename is 0022
			if (trimfile != "0022") {
				return;
			}
		}
		if (num == 2) {
			// Check if filename is 0017
			if (trimfile != "0017") {
				return;
			}
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

// Remove battle flashes
void fileEditor::battleExeEdits(std::string file) {
	std::string trimfile = gameFileTools::fileTrim(file);
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
	std::filesystem::current_path("..\\");
}

void fileEditor::editSLUS(std::string romFile) {
	// Insert new SLUS
	if (pp->fastName != "" || pp->jpnName != "") {
		// Add fast text to softsubs SLUS
		std::filesystem::current_path(pp->gamefilePath);
		if (num == 1) {
			std::filesystem::copy(pp->slusDisc1, tempDir, std::filesystem::copy_options::update_existing);
		}
		if (num == 2) {
			std::filesystem::copy(pp->slusDisc2, tempDir, std::filesystem::copy_options::update_existing);
		}
		if (pp->jpnName != "") {
			pWin->log_file << "Applying control edits to game's executable." << std::endl;
			for (const auto& entry : std::filesystem::directory_iterator(tempDir)) {
				controlEditor::editExecutable(entry.path().string());
			}
		}
		if (pp->fastName != "") {
			pWin->log_file << "Applying text speed change to game's executable." << std::endl;
			for (const auto& entry : std::filesystem::directory_iterator(tempDir)) {
				exeEdits(entry.path().string());
			}
		}
		std::filesystem::current_path(pWin->home);
	}
	// Create batch file to make a new SLUS
	makeSLUS ms(romFile, num, pp, pWin);
}

void fileEditor::expRateEdits(std::string file) {
	std::string trimfile = gameFileTools::fileTrim(file);
	// Check if filename is 2607
	if (trimfile != "2607.unk4") {
		return;
	}
	std::filesystem::current_path(pWin->home);
	std::filesystem::current_path(pp->gamefilePath);
	std::filesystem::current_path(tempDir);
	partyStatEditor pse;
	pse.deathblowLevels();
	std::filesystem::current_path("..\\");
}