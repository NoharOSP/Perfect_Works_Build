#include "pch.h"
#include "romFinder.h"

romFinder::romFinder(Window* win) {
	pWin = win;
}

romFinder::~romFinder() {

}

void romFinder::browseFiles() {
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = pWin->winHwnd;
	ofn.lpstrFilter = "Bin File (*.bin)\0*.bin\0";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_ENABLESIZING | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
	ofn.nMaxFile = MAX_PATH;
	char szFile[MAX_PATH];
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';
	ofn.nFilterIndex = 1;
	if (GetOpenFileNameA(&ofn)) {
		pWin->log_file << "File selected." << std::endl;
		std::string path = ofn.lpstrFile;
		// Check for Xenogears bin files
		searchCD(path);
		if (getFound()) {
			pWin->log_file << "Xenogears has been found. Determine disc number." << std::endl;
			pWin->discNum = getDisc();
			if (discNum == 1 || discNum == 2) {
				setPathText(path);
			}
			else {
				romErrorMsg();
			}
			if (pWin->pathFound1 || pWin->pathFound2) {
				pWin->checkboxLock();
			}
		}
		else {
			romErrorMsg();
		}
	}
}

void romFinder::setPathText(std::string path) {
	if (discNum == 1) {
		pWin->log_file << "Disc 1 found." << std::endl;
		pWin->pathFound1 = true;
		pWin->log_file << "Determine disc 1 path." << std::endl;
		pWin->path1 = path;
	}
	if (discNum == 2) {
		pWin->log_file << "Disc 2 found." << std::endl;
		pWin->pathFound2 = true;
		pWin->log_file << "Determine disc 2 path." << std::endl;
		pWin->path2 = path;
	}
	std::wstring wpath = std::wstring(path.begin(), path.end());
	LPCWSTR lpath = wpath.c_str();
	if (discNum == 1) {
		pWin->log_file << "Put disc 1 path in path window." << std::endl;
		SetWindowText(pWin->cd1path, lpath);
	}
	if (discNum == 2) {
		pWin->log_file << "Put disc 2 path in path window." << std::endl;
		SetWindowText(pWin->cd2path, lpath);
	}
}

void romFinder::romErrorMsg() {
	pWin->log_file << "The selected file is not a valid Xenogears ROM." << std::endl;
	MessageBox(pWin->winHwnd, L"The bin is not valid.", L"Error", MB_ICONERROR);
}

void romFinder::searchCD(std::string path) {
	std::ifstream file;
	int byte = -1;
	file.open(path, std::ios::binary);
	while (!file.bad()) {
		file.read(reinterpret_cast<char*>(&buffer), sizeof(buffer));
		byte += 1;
		// Check to see if the function has passed the header
		if (file.eof() || byte > 37704) {
			break;
		}
		// Find "XENOGEARS" in the ROM header
		if (buffer == 'X') {
			xenoFound = true;
			while (!discFound) {
				file.read(reinterpret_cast<char*>(&buffer), sizeof(buffer));
				byte += 1;
				if (byte == 37736) {
					findDiscNum(path);
					break;
				}
			}
		}
	}
	file.close();
}

void romFinder::findDiscNum(std::string path) {
	int val = (int)buffer;
	// Determine disc number through the first file difference
	if (val == 178) {
		discNum = 1;
		discFound = true;
		fileSize = std::filesystem::file_size(path);
		if (fileSize != 718738272) {
			xenoFound = false;
		}
	}
	else if (val == 207) {
		discNum = 2;
		discFound = true;
		fileSize = std::filesystem::file_size(path);
		if (fileSize != 688700880) {
			xenoFound = false;
		}
	}
}

bool romFinder::getFound() {
	return xenoFound;
}

int romFinder::getDisc() {
	return discNum;
}
