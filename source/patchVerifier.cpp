#include "pch.h"
#include "patchVerifier.h"

patchVerifier::patchVerifier(Window* win, patchProcessor* process, int discNum) {
	pWin = win;
	pp = process;
	num = discNum;
}

patchVerifier::~patchVerifier() {
}

void patchVerifier::fmvVerify() {
	// Access directory for patches if FMVs has been ticked
	pWin->log_file << "Check if 'patches' directory exists." << std::endl;
	if (std::filesystem::exists(patchPath)) {
		std::filesystem::current_path(patchPath);
		pWin->log_file << "'patches' directory is valid." << std::endl;
		patchPathValid = true;
		if (pWin->p_fmv) {
			pWin->log_file << "FMV patch file found." << std::endl;
			if (num == 1) {
				pp->fmvPatch = "cd1_fmvs.xdelta";
				pp->fmvName = "fmv1";
			}
			if (num == 2) {
				pp->fmvPatch = "cd2_fmvs.xdelta";
				pp->fmvName = "fmv2";
			}
		}
	}
}

void patchVerifier::verify() {
	if (!storyModeVerify()) {
		if (!monsterVerify()) {
			expVerify();
			goldVerify();
			arenaVerify();
		}
		encounterVerify();
		itemSpellVerify();
	}
	fastVerify();
	scriptVerify();
	portraitsVerify();
	graphicsVerify();
	voiceVerify();
	roniVerify();
	cafeVerify();
	bugVerify();
	titleVerify();
}

bool patchVerifier::storyModeVerify() {
	if (pWin->p_story_mode) {
		if (!pWin->p_script) {
			pWin->log_file << "Story mode directory found." << std::endl;
			if (num == 1) {
				pp->storyModeName = "storyfiles_cd1";
			}
			if (num == 2) {
				pp->storyModeName = "storyfiles_cd2";
			}
		}
		else {
			pWin->log_file << "Story mode/retranslated script directory found." << std::endl;
			if (num == 1) {
				pp->storyModeName = "storyfiles_script_cd1";
			}
			if (num == 2) {
				pp->storyModeName = "storyfiles_script_cd2";
			}
		}
	}
	return pWin->p_story_mode;
}

bool patchVerifier::monsterVerify() {
	if (pWin->p_monsters) {
		// Check if items/spells and script patches aren't selected to avoid compatibility issues.
		if (!pWin->p_script && !pWin->p_items_spells) {
			pWin->log_file << "Rebalanced monsters directory found." << std::endl;
			pp->monsterName = "Monsters";
		}
		else if (pWin->p_items_spells && !pWin->p_script) {
			pWin->log_file << "Rebalanced monsters and party directory found." << std::endl;
			pp->monsterName = "monsters_items";
		}
		else if (!pWin->p_items_spells && pWin->p_script) {
			pWin->log_file << "Rebalanced monsters/retranslated script directory found." << std::endl;
			pp->monsterName = "monsters_script";
		}
		else if (pWin->p_items_spells && pWin->p_script) {
			pWin->log_file << "Rebalanced monsters and party/retranslated script directory found." << std::endl;
			pp->monsterName = "monsters_both";
		}
	}
	return pWin->p_monsters;
}

bool patchVerifier::expVerify() {
	bool exp = false;
	if (pWin->p_expone || pWin->p_exptwo) {
		pWin->log_file << "Experience directory found." << std::endl;
		pp->expName = "og_monsters";
		exp = true;
	}
	return exp;
}

bool patchVerifier::goldVerify() {
	bool gold = false;
	if (pWin->p_goldone || pWin->p_goldtwo) {
		pWin->log_file << "Gold directory found." << std::endl;
		pp->goldName = "og_monsters";
		gold = true;
	}
	return gold;
}

bool patchVerifier::encounterVerify() {
	if (pWin->p_encounters) {
		// Check if script patches aren't selected to avoid compatibility issues. Otherwise, a merged folder will be used
		if (!pWin->p_script) {
			pWin->log_file << "Half encounters directory found." << std::endl;
			if (num == 1) {
				pp->encountersName = "encounter_rate_1";
			}
			if (num == 2) {
				pp->encountersName = "encounter_rate_2";
			}
		}
		else {
			pWin->log_file << "Half encounters/retranslated script directory found." << std::endl;
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

bool patchVerifier::itemSpellVerify() {
	if (pWin->p_items_spells) {
		// Check if the items/script hybrid patch needs to be applied
		if (pWin->p_script) {
			pWin->log_file << "Rebalanced party/retranslated script directory found." << std::endl;
			if (num == 1) {
				pp->itemspellsName = "Script_items";
			}
			if (num == 2) {
				pp->itemspellsName = "Script_items2";
			}
		}
		else {
			pWin->log_file << "Rebalanced party directory found." << std::endl;
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

bool patchVerifier::fastVerify() {
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
		pWin->log_file << "Fast text directory found." << std::endl;
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

bool patchVerifier::scriptVerify() {
	if (pWin->p_script) {
		// Check if items/spells and script hybrid won't be applied
		if (!pWin->p_items_spells) {
			pWin->log_file << "Retranslated script directory found." << std::endl;
			if (num == 1) {
				pp->scriptName = "script1";
			}
			if (num == 2) {
				pp->scriptName = "script2";
			}
		}
		else {
			pWin->log_file << "Retranslated script/rebalanced party directory found." << std::endl;
			if (num == 1) {
				pp->scriptName = "Script_items";
			}
			else {
				pp->scriptName = "Script_items2";
			}
		}
	}
	return pWin->p_script;
}

bool patchVerifier::arenaVerify() {
	bool arena = false;
	if (pWin->p_barena) {
		if (!pWin->p_script) {
			pWin->log_file << "Basic arena directory found." << std::endl;
			pp->arenaName = "filesbasic";
		}
		else {
			pWin->log_file << "Basic arena/retranslated script directory found." << std::endl;
			pp->arenaName = "filesbasic_script";
		}
		arena = true;
	}
	if (pWin->p_earena) {
		if (!pWin->p_script) {
			pWin->log_file << "Expert arena directory found." << std::endl;
			pp->arenaName = "filesexpert";
		}
		else {
			pWin->log_file << "Expert arena/retranslated script directory found." << std::endl;
			pp->arenaName = "filesexpert_script";
		}
		arena = true;
	}
	return arena;
}

bool patchVerifier::portraitsVerify() {
	if (pWin->p_portraits) {
		pWin->log_file << "Resized portraits directory found." << std::endl;
		pp->portraitsName = "portraits";
	}
	return pWin->p_portraits;
}

bool patchVerifier::graphicsVerify() {
	bool graphics = false;
	if (pWin->p_graphics) {
		// Graphics edits outside of portraits are not applied to the items/spells or script patch here as they would cause crashes.
		if (!pWin->p_items_spells && !pWin->p_script) {
			if (!pWin->p_portraits) {
				pWin->log_file << "Graphical fix directory found." << std::endl;
				pp->graphicsName = "graphics";
			}
			else {
				pWin->log_file << "Graphical fix without portraits directory found." << std::endl;
				pp->graphicsName = "graphics_no_portraits";
			}
			graphics = true;
		}
		else if (!pWin->p_portraits) {
			pWin->log_file << "Fixed portraits directory found." << std::endl;
			pp->graphicsName = "graphics_portraits";
			graphics = true;
		}
	}
	return graphics;
}

bool patchVerifier::voiceVerify() {
	if (pWin->p_voice) {
		pWin->log_file << "Undub in-game voice directory found." << std::endl;
		pp->voiceName = "voice";
	}
	return pWin->p_voice;
}

bool patchVerifier::roniVerify() {
	if (pWin->p_roni) {
		if (!pWin->p_portraits) {
			pWin->log_file << "Non-resized Roni directory found." << std::endl;
			pp->roniName = "roni_pw\\default";
		}
		else {
			pWin->log_file << "Resized Roni directory found." << std::endl;
			pp->roniName = "roni_pw\\resized";
		}
	}
	return pWin->p_roni;
}

bool patchVerifier::cafeVerify() {
	bool cafe = false;
	if (pWin->p_cafe) {
		if (num == 1) {
			if (!pWin->p_script) {
				pWin->log_file << "Original script Emeralda diner fix directory found." << std::endl;
				pp->cafeName = "emeralda_fix\\og_script";
			}
			else {
				pWin->log_file << "New script Emeralda diner fix directory found." << std::endl;
				pp->cafeName = "emeralda_fix\\new_script";
			}
			cafe = true;
		}
	}
	return cafe;
}

bool patchVerifier::bugVerify() {
	bool bug = false;
	// Bug patch is not applied with items/spells and script as they already have it applied.
	if (!pWin->p_items_spells && !pWin->p_script) {
		pWin->log_file << "Bug fix directory found." << std::endl;
		if (num == 1) {
			pp->bugName = "bug_fix1";
		}
		if (num == 2) {
			pp->bugName = "bug_fix2";
		}
		bug = true;
	}
	return bug;
}

void patchVerifier::titleVerify() {
	pWin->log_file << "Title screen directory found." << std::endl;
	pp->titleName = "title_screen";
}