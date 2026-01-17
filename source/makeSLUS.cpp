#include "pch.h"
#include "makeSLUS.h"

makeSLUS::makeSLUS(std::string romFile, int num, patchProcessor* pp, Window* pWin) {
	std::ofstream batch_file2;
	pWin->log_file << "Creating new SLUS file." << std::endl;
	batch_file2.open("commands2.cmd", std::ios::trunc);
	if (num == 1) {
		if (pp->fastName != "") {
			batch_file2 << "Tools\\Xeno_slus_ins.exe " + romFile + " gamefiles\\temp\\SLUS_006.64" << std::endl;
		}
		else {
			batch_file2 << "Tools\\Xeno_slus_ins.exe " + romFile + " gamefiles\\sub_executable\\disc1\\SLUS_006.64" << std::endl;
		}
	}
	if (num == 2) {
		if (pp->fastName != "") {
			batch_file2 << "Tools\\Xeno_slus_ins.exe " + romFile + " gamefiles\\temp\\SLUS_006.69" << std::endl;
		}
		else {
			batch_file2 << "Tools\\Xeno_slus_ins.exe " + romFile + " gamefiles\\sub_executable\\disc2\\SLUS_006.69" << std::endl;
		}
	}
	batch_file2.close();
	int batch_exit_code = system("cmd.exe /c commands2.cmd");
	pWin->log_file << "Remove new SLUS command file." << std::endl;
	remove("commands2.cmd");
}

makeSLUS::~makeSLUS() {

}