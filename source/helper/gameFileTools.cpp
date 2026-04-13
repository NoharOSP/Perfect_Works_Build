#include "gameFileTools.h"

std::string gameFileTools::fileTrim(std::string file) {
	std::string trimfile = file;
	trimfile.erase(0, 5);
	return trimfile;
}

std::string gameFileTools::musicfileTrim(std::string file) {
	std::string trimfile = file;
	trimfile.erase(0, 6);
	return trimfile;
}