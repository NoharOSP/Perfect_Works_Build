#include "pch.h"
#include "applyPatch.h"

applyPatch::applyPatch(Window* win, int discNum, patchProcessor* processor) {
	pWin = win;
	num = discNum;
	pp = processor;
	initialise();
}

applyPatch::~applyPatch() {

}

void applyPatch::initialise() {
	pWin->log_file << "Initialise patched ROM name and disc number to pass to xenoiso." << std::endl;
	// Disc 1 data
	if (num == 1) {
		fileName = "Xenogears_PW_CD1.bin";
		cueName = "Xenogears_PW_CD1.cue";
		cdName = "cd1";
	}
	// Disc 2 data
	if (num == 2) {
		fileName = "Xenogears_PW_CD2.bin";
		cueName = "Xenogears_PW_CD2.cue";
		cdName = "cd2fix";
	}
	if (pp->space) {
		oldPath = pp->tempPath;
	}
	else {
		oldPath = pp->filePath;
	}
	// Return to home directory
	std::filesystem::current_path(pWin->home);
}

bool applyPatch::patch() {
	checkFile();
	createFiles();
	if (!pp->patchList.empty()) {
		// Create ROMs using xenoiso
		pWin->log_file << "Create temporary directory." << std::endl;
		std::filesystem::current_path(pp->gamefilePath);
		std::filesystem::create_directory(temp);
		popTemp();
		if (pp->expName != "" || pp->goldName != "") {
			// Iterate through enemy files to apply exp or gold changes
			if (pp->expName != "") {
				pWin->log_file << "Applying exp changes." << std::endl;
			}
			if (pp->goldName != "") {
				pWin->log_file << "Applying gold changes." << std::endl;
			}
			for (const auto& entry : std::filesystem::directory_iterator(temp)) {
				monsterEdits(entry.path().string());
			}
		}
		if (pWin->p_flashes) {
			pWin->log_file << "Applying battle executable changes." << std::endl;
			for (const auto& entry : std::filesystem::directory_iterator(temp)) {
				battleExeEdits(entry.path().string());
			}
		}
		std::filesystem::current_path(pWin->home);
		changed = true;
		if (pp->fmvName != "") {
			applyFMV();
		}
		if (patched == true) {
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
	executeBat();
	if (pp->fmvName != "") {
		editSLUS();
	}
	cleanup();
	makeCue();
	return verifyPatch();
}
	
void applyPatch::checkFile() {
	// Detect if the patched ROM already exists.
	pWin->log_file << "Check if a patched ROM already exists." << std::endl;
	if (std::filesystem::exists(fileName)) {
		pWin->log_file << "Deleting existing patched ROM." << std::endl;
		remove(fileName.c_str());
	}
}

void applyPatch::createFiles() {
	pWin->log_file << "Creating list file for xenoiso." << std::endl;
	list_file.open("list.txt", std::ios::trunc);
	pWin->log_file << "Creating xdelta command file." << std::endl;
	batch_file.open("commands.cmd", std::ios::trunc);
}

void applyPatch::popTemp() {
	pWin->log_file << "Copy files from each selected option into the temporary directory." << std::endl;
	for (int i = 0; i < pp->patchList.size(); i++) {
		if (pp->patchList[i] != "" && pp->patchList[i] != pp->fmvPatch) {
			if (pp->patchList[i] == pp->fastName) {
				// Copy executable to temp
				if (!pWin->p_fmv) {
					std::filesystem::copy(pp->exeName, temp, std::filesystem::copy_options::update_existing);
					pWin->log_file << "Applying text speed change to game's executable." << std::endl;
					for (const auto& entry : std::filesystem::directory_iterator(temp)) {
						exeEdits(entry.path().string());
					}
				}
			}
			std::filesystem::copy(pp->patchList[i], temp, std::filesystem::copy_options::update_existing);
		}
	}
}

void applyPatch::exeEdits(std::string file) {
	// Apply fast text changes
	std::string trimfile = file;
	trimfile.erase(0, 5);
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

void applyPatch::monsterEdits(std::string file) {
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
		if (pp->expName != "") {
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
		if (pp->goldName != "") {
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
void applyPatch::battleExeEdits(std::string file) {
	std::string trimfile = file;
	trimfile.erase(0, 5);
	// Check if filename is 0038
	if (trimfile != "0038") {
		return;
	}
	// Decompress file
	std::filesystem::current_path(pWin->home);
	int batch_decompress = system("Tools\\xenocomp.exe -d gamefiles\\temp\\0038 gamefiles\\temp\\0038.dec");
	std::filesystem::current_path(pp->gamefilePath);
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
	std::filesystem::current_path(pp->gamefilePath);
	std::filesystem::current_path(temp);
	remove("0038.dec");
}

void applyPatch::applyFMV() {
	pWin->log_file << "Applying FMV undub." << std::endl;
	std::filesystem::current_path(pWin->home);
	std::string patchName;
	patchName = pp->fmvPatch;
	pWin->log_file << "Creating batch file commands." << std::endl;
	if (patched) {
		// Create copy of bin file to stack patches
		pWin->log_file << "Make backup of the ROM if it has already been patched." << std::endl;
		batch_file << "copy \"" + fileName + "\" backup.bin \n" << std::endl;
		batch_file << "del \"" + fileName + "\" \n" << std::endl;
		pWin->log_file << "Write command for xdelta to apply the FMV patches." << std::endl;
		batch_file << "Tools\\xdelta3-3.0.11-i686.exe -d  -s backup.bin patches\\" + patchName + " \"" + fileName + "\" \n" << std::endl;
		if (num == 1) {
			batch_file << "Tools\\Insert_log_file.exe " + fileName + " Tools\\xenocd1_softmod_files_extended.txt -new" << std::endl;
		}
		if (num == 2) {
			batch_file << "Tools\\Insert_log_file.exe " + fileName + " Tools\\xenocd2_softmod_files_extended.txt -new" << std::endl;
		}
	}
	else {
		// Apply patches
		pWin->log_file << "Write command for xdelta to apply the FMV patches." << std::endl;
		changed = true;
		batch_file << "Tools\\xdelta3-3.0.11-i686.exe -d  -s \"" + oldPath + "\" patches\\" + patchName + " \"" + fileName + "\" \n" << std::endl;
		if (num == 1) {
			batch_file << "Tools\\Insert_log_file.exe " + fileName + " Tools\\xenocd1_softmod_files_extended.txt -new" << std::endl;
		}
		if (num == 2) {
			batch_file << "Tools\\Insert_log_file.exe " + fileName + " Tools\\xenocd2_softmod_files_extended.txt -new" << std::endl;
		}
		patched = true;
	}
}

void applyPatch::executeBat() {
	// Execute patch file
	if (pp->fmvName != "") {
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
}

void applyPatch::editSLUS() {
	// Insert new SLUS
	if (pp->fastName != "") {
		// Add fast text to softsubs SLUS
		std::filesystem::current_path(pp->gamefilePath);
		if (num == 1) {
			std::filesystem::copy(pp->slusDisc1, temp, std::filesystem::copy_options::update_existing);
		}
		if (num == 2) {
			std::filesystem::copy(pp->slusDisc2, temp, std::filesystem::copy_options::update_existing);
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
		if (pp->fastName != "") {
			batch_file2 << "Tools\\Xeno_slus_ins.exe " + fileName + " gamefiles\\temp\\SLUS_006.64" << std::endl;
		}
		else {
			batch_file2 << "Tools\\Xeno_slus_ins.exe " + fileName + " gamefiles\\sub_executable\\disc1\\SLUS_006.64" << std::endl;
		}
	}
	if (num == 2) {
		if (pp->fastName != "") {
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

void applyPatch::cleanup() {
	// Remove batch and backup bin
	pWin->log_file << "Remove command file." << std::endl;
	remove("commands.cmd");
	pWin->log_file << "Remove backup ROM." << std::endl;
	remove("backup.bin");
	pWin->log_file << "Remove xenoiso list." << std::endl;
	remove("list.txt");
	std::filesystem::current_path(pp->gamefilePath);
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
}

void applyPatch::makeCue() {
	// Create cue file
	pWin->log_file << "Create cue file." << std::endl;
	cue_stream.open(cueName, std::ios::out);
	cue_stream << "FILE \"" + fileName + "\" BINARY" << "\n";
	cue_stream << "  TRACK 01 MODE2/2352" << "\n";
	cue_stream << "    INDEX 01 00:00:00" << "\n";
	cue_stream.close();
}
	
bool applyPatch::verifyPatch() {
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
	