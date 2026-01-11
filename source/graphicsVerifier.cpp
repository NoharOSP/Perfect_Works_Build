#include "pch.h"
#include "graphicsVerifier.h"

graphicsVerifier::graphicsVerifier() {

}

graphicsVerifier::~graphicsVerifier() {

}

bool graphicsVerifier::resizeVerify(Window* pWin, patchProcessor* pp) {
	if (pWin->p_resize) {
		pWin->log_file << "Resized portraits directory found." << std::endl;
		pp->resizeName = "resized_portraits";
	}
	return pWin->p_resize;
}

bool graphicsVerifier::portraitsVerify(Window* pWin, patchProcessor* pp) {
	if (pWin->p_portraits) {
		pWin->log_file << "Portrait directory found." << std::endl;
		pp->portraitsName = "portraits";
	}
	return pWin->portraits;
}

bool graphicsVerifier::roniVerify(Window* pWin, patchProcessor* pp) {
	if (pWin->p_roni) {
		pWin->log_file << "Roni directory found." << std::endl;
		if (!pWin->p_resize) {
			pp->roniName = "roni_pw\\default";
		}
		else {
			pp->roniName = "roni_pw\\resized";
		}
	}
	return pWin->p_roni;
}

bool graphicsVerifier::cafeVerify(Window* pWin, patchProcessor* pp, int num) {
	bool cafe = false;
	if (pWin->p_cafe) {
		pWin->log_file << "Emeralda diner fix directory found." << std::endl;
		if (num == 1) {
			if (!pWin->p_script) {
				pp->cafeName = "emeralda_fix\\og_script";
			}
			else {
				pp->cafeName = "emeralda_fix\\new_script";
			}
			cafe = true;
		}
	}
	return cafe;
}