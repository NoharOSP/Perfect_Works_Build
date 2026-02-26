#include "pch.h"
#include "controlEditor.h"

void controlEditor::addImage(std::string file) {
	std::string trimfile = gameFileTools::fileTrim(file);
	// Check if filename is 2593 or 3958
	if ((trimfile == "2593.unk8") && (trimfile == "3958.unk8")) {
		editData(trimfile);
	}
	else if (trimfile == "2614") {
		editBattleFile(trimfile);
	}
	else {
		return;
	}
}

void controlEditor::editData(std::string trimfile) {
	std::filesystem::current_path(Window::home);
	std::filesystem::current_path(patchProcessor::gamefilePath);
	std::filesystem::current_path(applyPatch::temp);
	// Decompress file
	if (trimfile == "2593.unk8") {
		int batch_decompress = system("..\\..\\Tools\\xenopack.exe -u 2593.unk8");
	}
	if (trimfile == "3958.unk8") {
		int batch_decompress = system("..\\..\\Tools\\xenopack.exe -u 3958.unk8");
	}
	// Copy Japanese control image
	std::filesystem::copy("..\\jpn_controls\\2593_3958\\file1", "file1", std::filesystem::copy_options::overwrite_existing);
	// Recompress file
	if (trimfile == "2593.unk8") {
		int batch_recompress = system("..\\..\\Tools\\xenopack.exe -p 2593.unk8");
	}
	if (trimfile == "3958.unk8") {
		int batch_recompress = system("..\\..\\Tools\\xenopack.exe -p 3958.unk8");
	}
	// Remove decompressed files
	remove("file0"), remove("file1"), remove("file2"), remove("file3"), remove("file4"), remove("file5"), remove("file6"), remove("file7");
	std::filesystem::current_path("..\\");
}

void controlEditor::editBattleFile(std::string trimfile) {
	std::filesystem::current_path(Window::home);
	std::filesystem::current_path(patchProcessor::gamefilePath);
	std::filesystem::current_path(applyPatch::temp);
	// Decompress file
	int batch_decompress = system("..\\..\\Tools\\xenopack.exe -u 2614");
	// Copy Japanese control images
	std::filesystem::copy("..\\jpn_controls\\2614\\file0", "file0", std::filesystem::copy_options::overwrite_existing);
	std::filesystem::copy("..\\jpn_controls\\2614\\file1", "file1", std::filesystem::copy_options::overwrite_existing);
	// Recompress file
	int batch_recompress = system("..\\..\\Tools\\xenopack.exe -p 2614");
	// Remove decompressed files
	for (int i = 0; i < 38; i++) {
		std::string num = std::to_string(i);
		std::string fileName = "file" + num;
		remove(fileName.c_str());
	}
	std::filesystem::current_path("..\\");
}

void controlEditor::editExecutable(std::string file) {
	std::string trimfile = gameFileTools::fileTrim(file);
	// Check if filename is 0022 or uses a SLUS prefix
	if (trimfile == "0022" || (trimfile == "SLUS_006.64" || trimfile == "SLUS_006.69")) {
		std::filesystem::current_path(Window::home);
		// Read data documenting control differences and put it into vectors
		std::vector<int> offsets;
		std::vector<int> values;
		std::string line;
		std::fstream exedata;
		exedata.open(exediff);
		while (getline(exedata, line)) {
			int pos = line.find(",");
			std::string offset = line.substr(0, pos);
			offsets.emplace_back(stoi(line));
			std::string value = line.substr(pos + 1);
			values.emplace_back(stoi(value, nullptr, 16));
		}
		exedata.close();
		// Open file
		std::filesystem::current_path(patchProcessor::gamefilePath);
		std::filesystem::current_path(applyPatch::temp);
		std::fstream fileContents;
		fileContents.open(trimfile, std::ios::in | std::ios::out | std::ios::binary);
		// Edit file
		for (int i = 0; i < offsets.size(); i++) {
			fileContents.seekp(offsets[i], std::ios_base::beg);
			fileContents.write(reinterpret_cast <char*>(&values[i]), 2);
		}
		std::filesystem::current_path("..\\");
	}
	else {
		return;
	}
}