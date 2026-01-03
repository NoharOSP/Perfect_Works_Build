#include "pch.h"
#include "storyVerifier.h"

storyVerifier::storyVerifier() {

}

storyVerifier::~storyVerifier() {

}

bool storyVerifier::fastVerify(Window* pWin, patchProcessor* pp, int num) {
	bool fast = false;
	if (pWin->p_fastnew) {
		pWin->log_file << "Retranslated fast text directory found." << std::endl;
		if (num == 1) {
			pp->fastName = "text_cd1";
		}
		if (num == 2) {
			// Disc 2 will use the same patch regardless of whether the script patch has been applied as it has no cutscenes with auto-advance
			pp->fastName = "text_cd2";
		}
		fast = true;

	}
	if (pWin->p_fastold) {
		pWin->log_file << "Retranslated fast text directory found." << std::endl;
		if (num == 1) {
			pp->fastName = "text_old1";
		}
		if (num == 2) {
			pp->fastName = "text_cd2";
		}
		fast = true;
	}
	return fast;
}

bool storyVerifier::scriptVerify(Window* pWin, patchProcessor* pp, int num) {
	if (pWin->p_script) {
		pWin->log_file << "Retranslated script directory found." << std::endl;
		// Check if items/spells and script hybrid won't be applied
		if (!pWin->p_items_spells) {
			if (num == 1) {
				pp->scriptName = "script1";
			}
			if (num == 2) {
				pp->scriptName = "script2";
			}
		}
	}
	return pWin->p_script;
}