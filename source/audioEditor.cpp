#include "pch.h"
#include "audioEditor.h"

// TODO: Split into methods
void audioEditor::musicEdits(std::string file) {
	// Get file name without path info
	std::string trimfile = gameFileTools::fileTrim(file);
	// Find subfile
	std::string subFile;
	for (const auto& entry : std::filesystem::directory_iterator(musicSubFiles)) {
		std::string subfileName = entry.path().string();
		if (subfileName.contains(trimfile)) {
			subFile = subfileName;
			break;
		}
	}
	// Open subfile
	std::fstream subFileContents;
	subFileContents.open(subFile, std::ios::in | std::ios::out | std::ios::binary);
	// Get length
	int subLength = std::filesystem::file_size(subFile);
	// Create array
	std::vector<char> contents;
	// Copy contents
	unsigned char buffer;
	for (int i = 0; i < subLength; i++) {
		subFileContents.read(reinterpret_cast<char*>(&buffer), sizeof(buffer));
		contents.push_back(buffer);
	}
	// Close subfile
	subFileContents.close();
	// Open room
	char32_t offbuffer = 0;
	std::fstream room;
	room.open(file, std::ios::in | std::ios::out | std::ios::binary);
	room.seekp(324, std::ios_base::beg);
	room.read(reinterpret_cast<char*>(&offbuffer), 4);
	uint64_t scriptoffset = offbuffer;
	room.seekp(328, std::ios_base::beg);
	room.read(reinterpret_cast<char*>(&offbuffer), 4);
	uint64_t spriteoffset = offbuffer;
	uint64_t total = scriptoffset + subLength;
	// Check subfile length
	if (total != spriteoffset) {
		if (total > spriteoffset) {
			while ((scriptoffset + contents.size()) != spriteoffset) {
				contents.erase(contents.end() - 1);
			}
		}
		if (total < spriteoffset) {
			while ((scriptoffset + contents.size()) != spriteoffset) {
				int filler = 0;
				contents.push_back((char)filler);
			}
		}
	}
	// Write to file
	for (int i = 0; (i + scriptoffset) != spriteoffset; i++) {
		room.seekp((i + scriptoffset), std::ios_base::beg);
		room.write(reinterpret_cast<char*>(&contents[i]), 1);
	}
	room.close();
}