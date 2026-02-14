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
	std::filesystem::current_path(Window::home);
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
	std::filesystem::current_path(Window::home);
}