#include "pch.h"
#include "audioEditor.h"

void audioEditor::musicEdits(std::string file) {
	// Check if filename is between 0606 and 2064
	std::string trimfile = gameFileTools::fileTrim(file);
	int fileNum = std::stoi(trimfile);
	if (fileNum >= 0606 && fileNum <= 2064) {
		// TODO: Add functionality
		// Open file
		std::fstream fileContents;
		// Close file
		fileContents.close();
	}
}