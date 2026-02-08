#include "gameFileTools.h"

std::string gameFileTools::fileTrim(std::string file) {
	std::string trimfile = file;
	trimfile.erase(0, 5);
	return trimfile;
}