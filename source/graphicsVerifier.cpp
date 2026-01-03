#include "pch.h"
#include "graphicsVerifier.h"

graphicsVerifier::graphicsVerifier() {

}

graphicsVerifier::~graphicsVerifier() {

}

bool graphicsVerifier::portraitsVerify(Window* pWin, patchProcessor* pp) {
	if (pWin->p_portraits) {
		pWin->log_file << "Resized portraits directory found." << std::endl;
		pp->portraitsName = "portraits";
	}
	return pWin->p_portraits;
}

bool graphicsVerifier::graphicsVerify(Window* pWin, patchProcessor* pp) {
	bool graphics = false;
	if (pWin->p_graphics) {
		pWin->log_file << "Graphical fix directory found." << std::endl;
		// Graphics edits outside of portraits are not applied to the items/spells or script patch here as they would cause crashes.
		if (!pWin->p_items_spells && !pWin->p_script) {
			if (!pWin->p_portraits) {
				pp->graphicsName = "graphics";
			}
			else {
				pp->graphicsName = "graphics_no_portraits";
			}
			graphics = true;
		}
		else if (!pWin->p_portraits) {
			pp->graphicsName = "graphics_portraits";
			graphics = true;
		}
	}
	return graphics;
}

bool graphicsVerifier::roniVerify(Window* pWin, patchProcessor* pp) {
	if (pWin->p_roni) {
		pWin->log_file << "Roni directory found." << std::endl;
		if (!pWin->p_portraits) {
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