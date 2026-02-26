#include "pch.h"
#include "applyPatch.h"

applyPatch::applyPatch(Window* win, int discNum, patchProcessor* processor) {
	pWin = win;
	num = discNum;
	pp = processor;
	pFE = new fileEditor(pp, pWin, num, temp);
	initialise();
}

applyPatch::~applyPatch() {
	pFE->~fileEditor();
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
	prepareFiles();
	executeBat();
	if (pp->fmvName != "") {
		pFE->editSLUS(fileName);
	}
	cleanup();
	makeCue();
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

void applyPatch::prepareFiles() {
	// Detect if the patched ROM already exists.
	pWin->log_file << "Check if a patched ROM already exists." << std::endl;
	if (std::filesystem::exists(fileName)) {
		pWin->log_file << "Deleting existing patched ROM." << std::endl;
		remove(fileName.c_str());
	}
	pWin->log_file << "Creating list file for xenoiso." << std::endl;
	list_file.open("list.txt", std::ios::trunc);
	pWin->log_file << "Creating xdelta command file." << std::endl;
	batch_file.open("commands.cmd", std::ios::trunc);
	if (!pp->patchList.empty()) {
		createTemp();
		std::filesystem::current_path(pWin->home);
		if (pp->fmvName != "") {
			applyFMV();
		}
		if (patched) {
			backupROM();
		}
		else {
			patched = true;
		}
		pWin->log_file << "Write list file for xenoiso." << std::endl;
		list_file << cdName << "\n" << oldPath << "\n" << fileName << "\n" << "-1,.\\gamefiles\\temp" << std::flush;
	}
	list_file.close();
	batch_file.close();
}

void applyPatch::createTemp() {
	// Create ROMs using xenoiso
	pWin->log_file << "Create temporary directory." << std::endl;
	std::filesystem::current_path(pp->gamefilePath);
	std::filesystem::create_directory(temp);
	pWin->log_file << "Copy files from each selected option into the temporary directory." << std::endl;
	for (int i = 0; i < pp->patchList.size(); i++) {
		if (pp->patchList[i] != "" && pp->patchList[i] != pp->fmvPatch) {
			if (pp->patchList[i] == pp->fastName) {
				// Copy executable to temp
				if (!pWin->p_fmv) {
					if (num == 1) {
						std::filesystem::copy(patchProcessor::exeName1, temp, std::filesystem::copy_options::update_existing);
					}
					if (num == 2) {
						std::filesystem::copy(patchProcessor::exeName2, temp, std::filesystem::copy_options::update_existing);
					}
					pWin->log_file << "Applying text speed change to game's executable." << std::endl;
					for (const auto& entry : std::filesystem::directory_iterator(temp)) {
						pFE->exeEdits(entry.path().string());
					}
				}
			}
			std::filesystem::copy(pp->patchList[i], temp, std::filesystem::copy_options::update_existing);
		}
	}
	iterateTemp();
}

void applyPatch::iterateTemp() {
	if (pp->expName != "" || pp->goldName != "") {
		// Iterate through enemy files to apply exp or gold changes
		if (pp->expName != "") {
			pWin->log_file << "Applying exp changes." << std::endl;
		}
		if (pp->goldName != "") {
			pWin->log_file << "Applying gold changes." << std::endl;
		}
		for (const auto& entry : std::filesystem::directory_iterator(temp)) {
			pFE->monsterEdits(entry.path().string());
		}
	}
	if (pWin->p_flashes) {
		pWin->log_file << "Applying battle executable changes." << std::endl;
		for (const auto& entry : std::filesystem::directory_iterator(temp)) {
			pFE->battleExeEdits(entry.path().string());
		}
	}
}

void applyPatch::applyFMV() {
	pWin->log_file << "Applying FMV undub." << std::endl;
	std::filesystem::current_path(pWin->home);
	pWin->log_file << "Creating batch file commands." << std::endl;
	if (patched) {
		// Create copy of bin file to stack patches
		pWin->log_file << "Make backup of the ROM if it has already been patched." << std::endl;
		batch_file << "copy \"" + fileName + "\" backup.bin \n" << std::endl;
		batch_file << "del \"" + fileName + "\" \n" << std::endl;
		batch_file << "Tools\\xdelta3-3.0.11-i686.exe -d  -s backup.bin patches\\" + pp->fmvPatch + " \"" + fileName + "\" \n" << std::endl;
	}
	else {
		batch_file << "Tools\\xdelta3-3.0.11-i686.exe -d  -s \"" + oldPath + "\" patches\\" + pp->fmvPatch + " \"" + fileName + "\" \n" << std::endl;
		patched = true;
	}
	if (num == 1) {
		batch_file << "Tools\\Insert_log_file.exe " + fileName + " Tools\\xenocd1_softmod_files_extended.txt -new" << std::endl;
	}
	if (num == 2) {
		batch_file << "Tools\\Insert_log_file.exe " + fileName + " Tools\\xenocd2_softmod_files_extended.txt -new" << std::endl;
	}
}

void applyPatch::backupROM() {
	pWin->log_file << "Create a backup ROM if the FMV patch has been applied." << std::endl;
	oldPath = "backup.bin";
	batch_file << "copy \"" + fileName + "\" " + oldPath + "\n" << std::endl;
	batch_file << "del \"" + fileName + "\" \n" << std::endl;
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
	