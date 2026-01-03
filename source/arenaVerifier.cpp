#include "pch.h"
#include "arenaVerifier.h"

arenaVerifier::arenaVerifier(Window* pWin, patchProcessor* pp) {
	bool arena = false;
	if (basicVerify(pWin, pp) || expertVerify(pWin, pp)) {
		arena = true;
	}
	arenaFound(arena);
}

arenaVerifier::~arenaVerifier() {

}

bool arenaVerifier::basicVerify(Window* pWin, patchProcessor* pp) {
	bool basic = false;
	if (pWin->p_barena) {
		pWin->log_file << "Basic arena directory found." << std::endl;
		if (!pWin->p_script) {
			pp->arenaName = "filesbasic";
		}
		else {
			pp->arenaName = "filesbasic_script";
		}
		basic = true;
	}
	return basic;
}

bool arenaVerifier::expertVerify(Window* pWin, patchProcessor* pp) {
	bool expert = false;
	if (pWin->p_earena) {
		pWin->log_file << "Expert arena directory found." << std::endl;
		if (!pWin->p_script) {
			pp->arenaName = "filesexpert";
		}
		else {
			pp->arenaName = "filesexpert_script";
		}
		expert = true;
	}
	return expert;
}

bool arenaVerifier::arenaFound(bool arena) {
	return arena;
}