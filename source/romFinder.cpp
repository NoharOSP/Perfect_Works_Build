#include "pch.h"
#include "romFinder.h"

romFinder::romFinder() {
}

romFinder::~romFinder() {

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
			file.read(reinterpret_cast<char*>(&buffer), sizeof(buffer));
			byte += 1;
			if (buffer == 'E') {
				file.read(reinterpret_cast<char*>(&buffer), sizeof(buffer));
				byte += 1;
				if (buffer == 'N') {
					file.read(reinterpret_cast<char*>(&buffer), sizeof(buffer));
					byte += 1;
					if (buffer == 'O') {
						file.read(reinterpret_cast<char*>(&buffer), sizeof(buffer));
						byte += 1;
						if (buffer == 'G') {
							file.read(reinterpret_cast<char*>(&buffer), sizeof(buffer));
							byte += 1;
							if (buffer == 'E') {
								file.read(reinterpret_cast<char*>(&buffer), sizeof(buffer));
								byte += 1;
								if (buffer == 'A') {
									file.read(reinterpret_cast<char*>(&buffer), sizeof(buffer));
									byte += 1;
									if (buffer == 'R') {
										file.read(reinterpret_cast<char*>(&buffer), sizeof(buffer));
										byte += 1;
										if (buffer == 'S') {
											xenoFound = true;
											while (!discFound) {
												file.read(reinterpret_cast<char*>(&buffer), sizeof(buffer));
												byte += 1;
												if (byte == 37736) {
													int val = (int)buffer;
													// Determine disc number through the first file difference
													if (val == 178) {
														discNum = 1;
														discFound = true;
													}
													else if (val == 207) {
														discNum = 2;
														discFound = true;
													}
													break;
												}
											}
											break;
										}
									}
								}
							}
						}
					}
				}

			}
		}
	}
	file.close();
}

bool romFinder::getFound() {
	return xenoFound;
}

int romFinder::getDisc() {
	return discNum;
}
