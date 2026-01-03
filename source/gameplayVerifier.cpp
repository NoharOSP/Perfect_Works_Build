#include "pch.h"
#include "gameplayVerifier.h"

gameplayVerifier::gameplayVerifier() {

}

gameplayVerifier::~gameplayVerifier() {

}

bool gameplayVerifier::monsterVerify(Window* pWin, patchProcessor* pp) {
	if (pWin->p_monsters) {
		pWin->log_file << "Rebalanced monsters directory found." << std::endl;
		// Check if items/spells and script patches aren't selected to avoid compatibility issues.
		if (!pWin->p_script && !pWin->p_items_spells) {
			pp->monsterName = "Monsters";
		}
		else if (pWin->p_items_spells && !pWin->p_script) {
			pp->monsterName = "monsters_items";
		}
		else if (!pWin->p_items_spells && pWin->p_script) {
			pp->monsterName = "monsters_script";
		}
		else if (pWin->p_items_spells && pWin->p_script) {
			pp->monsterName = "monsters_both";
		}
	}
	return pWin->p_monsters;
}

bool gameplayVerifier::expVerify(Window* pWin, patchProcessor* pp) {
	bool exp = false;
	if (pWin->p_expone || pWin->p_exptwo) {
		pWin->log_file << "Experience directory found." << std::endl;
		pp->expName = "og_monsters";
		exp = true;
	}
	return exp;
}

bool gameplayVerifier::goldVerify(Window* pWin, patchProcessor* pp) {
	bool gold = false;
	if (pWin->p_goldone || pWin->p_goldtwo) {
		pWin->log_file << "Gold directory found." << std::endl;
		pp->goldName = "og_monsters";
		gold = true;
	}
	return gold;
}

bool gameplayVerifier::encounterVerify(Window* pWin, patchProcessor* pp, int num) {
	if (pWin->p_encounters) {
		pWin->log_file << "Half encounters directory found." << std::endl;
		// Check if script patches aren't selected to avoid compatibility issues. Otherwise, a merged folder will be used
		if (!pWin->p_script) {
			if (num == 1) {
				pp->encountersName = "encounter_rate_1";
			}
			if (num == 2) {
				pp->encountersName = "encounter_rate_2";
			}
		}
		else {
			if (num == 1) {
				pp->encountersName = "encounterone_script";
			}
			if (num == 2) {
				pp->encountersName = "encountertwo_script";
			}
		}
	}
	return pWin->p_encounters;
}

bool gameplayVerifier::itemSpellVerify(Window* pWin, patchProcessor* pp, int num) {
	if (pWin->p_items_spells) {
		pWin->log_file << "Rebalanced items/party directory found." << std::endl;
		// Check if the items/script hybrid patch needs to be applied
		if (pWin->p_script) {
			if (num == 1) {
				pp->itemspellsName = "Script_items";
			}
			if (num == 2) {
				pp->itemspellsName = "Script_items2";
			}
		}
		else {
			if (num == 1) {
				pp->itemspellsName = "items1";
			}
			if (num == 2) {
				pp->itemspellsName = "items2";
			}
		}
	}
	return pWin->p_items_spells;
}