#include "pch.h"
#include "audioEditor.h"

void audioEditor::musicEdits(std::string file) {
	// Get file name without path info
	std::string trimfile = gameFileTools::fileTrim(file);
	// Find subfile
	std::string subFile;
	for (const auto& entry : std::filesystem::directory_iterator(musicSubFiles)) {
		std::string subfileName = entry.path().string();
		if (subfileName.contains(trimfile)) {
			subFile = subfileName;
		}
	}
	// TODO: Obtain subfile contents
	// Open file
	std::fstream subFileContents;
	// Close file
	subFileContents.close();
}