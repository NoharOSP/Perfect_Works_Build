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
	graphicsVerifier grav;
	gameplayVerifier gamv;
	storyVerifier sv;
	if (!storyModeVerify()) {
		if (!gamv.monsterVerify(pWin, pp)) {
			gamv.expVerify(pWin, pp);
			gamv.goldVerify(pWin, pp);
		}
		arenaVerifier av(pWin, pp);
		av.~arenaVerifier();
		gamv.encounterVerify(pWin, pp, num);
		gamv.itemSpellVerify(pWin, pp, num);
		gamv.deathblowVerify(pWin, pp);
	}
	sv.fastVerify(pWin, pp, num);
	sv.scriptVerify(pWin, pp, num);
	grav.resizeVerify(pWin, pp);
	grav.portraitsVerify(pWin, pp);
	voiceVerify();
	grav.roniVerify(pWin, pp);
	grav.cafeVerify(pWin, pp, num);
	bugVerify();
	titleVerify();
	grav.~graphicsVerifier();
	gamv.~gameplayVerifier();
	sv.~storyVerifier();
}

bool patchVerifier::storyModeVerify() {
	if (pWin->p_story_mode) {
		pWin->log_file << "Story mode directory found." << std::endl;
		if (!pWin->p_script) {
			if (num == 1) {
				pp->storyModeName = "storyfiles_cd1";
			}
			if (num == 2) {
				pp->storyModeName = "storyfiles_cd2";
			}
		}
		else {
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

bool patchVerifier::voiceVerify() {
	if (pWin->p_voice) {
		pWin->log_file << "Undub in-game voice directory found." << std::endl;
		pp->voiceName = "voice";
	}
	return pWin->p_voice;
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