#include "pch.h"
#include "windowHandler.h"

windowHandler::windowHandler(Window* win) {
	pWin = win;
}

windowHandler::~windowHandler() {

}

bool windowHandler::check() {
	checkScript();
	checkEnc();
	checkFast();
	checkExpOne();
	checkExpTwo();
	checkGoldOne();
	checkGoldTwo();
	checkItemsParty();
	checkMonsters();
	checkNormArena();
	checkBasicArena();
	checkExpArena();
	checkPortraits();
	checkFMV();
	checkGraphics();
	checkVoice();
	checkFlash();
	checkRoni();
	checkCafe();
	checkStoryMode();
	return ticked;
}

void windowHandler::checkScript() {
	LRESULT scriptticked = SendMessage(pWin->script, BM_GETCHECK, NULL, NULL);
	if (scriptticked == BST_CHECKED) {
		pWin->log_file << "Script changes ticked." << std::endl;
		pWin->p_script = true;
		if (pWin->p_fastold) {
			pWin->log_file << "Ensure the version of fast text is used that supports the updated script." << std::endl;
			pWin->p_fastold = false;
			pWin->p_fastnew = true;
		}
		ticked = true;
	}
	else {
		pWin->p_script = false;
	}
}

void windowHandler::checkEnc() {
	LRESULT encticked = SendMessage(pWin->encounters, BM_GETCHECK, NULL, NULL);
	if (encticked == BST_CHECKED) {
		pWin->log_file << "Half encounters ticked." << std::endl;
		pWin->p_encounters = true;
		if (pWin->p_story_mode == true) {
			pWin->log_file << "Unticking story mode." << std::endl;
			LRESULT smuntick = SendMessage(pWin->storyMode, BM_SETCHECK, BST_UNCHECKED, NULL);
			pWin->p_story_mode = false;
		}
		else {
			ticked = true;
		}
	}
	else {
		pWin->p_encounters = false;
	}
}

void windowHandler::checkFast() {
	LRESULT fastticked = SendMessage(pWin->fasttext, BM_GETCHECK, NULL, NULL);
	if (fastticked == BST_CHECKED) {
		// Check if the fast text patch should support the new translation
		pWin->log_file << "Fast text ticked." << std::endl;
		if (pWin->p_script == false) {
			pWin->log_file << "Ensure the version of fast text supporting the original translation is used." << std::endl;
			pWin->p_fastold = true;
		}
		else {
			pWin->log_file << "Ensure the version of fast text is used that supports the updated script." << std::endl;
			pWin->p_fastnew = true;
		}
		ticked = true;
	}
	else {
		pWin->p_fastold = false;
		pWin->p_fastnew = false;
	}
}

void windowHandler::checkExpOne() {
	LRESULT exponeticked = SendMessage(pWin->experience1, BM_GETCHECK, NULL, NULL);
	if (exponeticked == BST_CHECKED) {
		pWin->log_file << "1.5x exp ticked." << std::endl;
		pWin->p_expone = true;
		if (pWin->p_exptwo == true) {
			pWin->log_file << "Unticking 2x exp." << std::endl;
			LRESULT exptwountick = SendMessage(pWin->experience2, BM_SETCHECK, BST_UNCHECKED, NULL);
			pWin->p_exptwo = false;
		}
		if (pWin->p_story_mode == true) {
			pWin->log_file << "Unticking story mode." << std::endl;
			LRESULT smuntick = SendMessage(pWin->storyMode, BM_SETCHECK, BST_UNCHECKED, NULL);
			pWin->p_story_mode = false;
		}
		ticked = true;
	}
	else {
		pWin->p_expone = false;
	}
}

void windowHandler::checkExpTwo() {
	LRESULT exptwoticked = SendMessage(pWin->experience2, BM_GETCHECK, NULL, NULL);
	if (exptwoticked == BST_CHECKED) {
		pWin->log_file << "2x exp ticked." << std::endl;
		pWin->p_exptwo = true;
		if (pWin->p_expone == true) {
			pWin->log_file << "Unticking 1.5x exp." << std::endl;
			LRESULT exponeuntick = SendMessage(pWin->experience1, BM_SETCHECK, BST_UNCHECKED, NULL);
			pWin->p_expone = false;
		}
		if (pWin->p_story_mode == true) {
			pWin->log_file << "Unticking story mode." << std::endl;
			LRESULT smuntick = SendMessage(pWin->storyMode, BM_SETCHECK, BST_UNCHECKED, NULL);
			pWin->p_story_mode = false;
		}
		ticked = true;
	}
	else {
		pWin->p_exptwo = false;
	}
}

void windowHandler::checkGoldOne() {
	LRESULT goldoneticked = SendMessage(pWin->gold1, BM_GETCHECK, NULL, NULL);
	if (goldoneticked == BST_CHECKED) {
		pWin->log_file << "1.5x gold ticked." << std::endl;
		pWin->p_goldone = true;
		if (pWin->p_goldtwo == true) {
			pWin->log_file << "Unticking 2x gold." << std::endl;
			LRESULT goldtwountick = SendMessage(pWin->gold2, BM_SETCHECK, BST_UNCHECKED, NULL);
			pWin->p_goldtwo = false;
		}
		if (pWin->p_story_mode == true) {
			pWin->log_file << "Unticking story mode." << std::endl;
			LRESULT smuntick = SendMessage(pWin->storyMode, BM_SETCHECK, BST_UNCHECKED, NULL);
			pWin->p_story_mode = false;
		}
		ticked = true;
	}
	else {
		pWin->p_goldone = false;
	}
}

void windowHandler::checkGoldTwo() {
	LRESULT goldtwoticked = SendMessage(pWin->gold2, BM_GETCHECK, NULL, NULL);
	if (goldtwoticked == BST_CHECKED) {
		pWin->log_file << "2x gold ticked." << std::endl;
		pWin->p_goldtwo = true;
		if (pWin->p_goldone == true) {
			pWin->log_file << "Unticking 1.5x gold." << std::endl;
			LRESULT goldoneuntick = SendMessage(pWin->gold1, BM_SETCHECK, BST_UNCHECKED, NULL);
			pWin->p_goldone = false;
		}
		if (pWin->p_story_mode == true) {
			pWin->log_file << "Unticking story mode." << std::endl;
			LRESULT smuntick = SendMessage(pWin->storyMode, BM_SETCHECK, BST_UNCHECKED, NULL);
			pWin->p_story_mode = false;
		}
		ticked = true;
	}
	else {
		pWin->p_goldtwo = false;
	}
}

void windowHandler::checkItemsParty() {
	LRESULT itemspellsticked = SendMessage(pWin->itemspells, BM_GETCHECK, NULL, NULL);
	if (itemspellsticked == BST_CHECKED) {
		pWin->log_file << "Rebalanced party/items ticked." << std::endl;
		pWin->p_items_spells = true;
		if (pWin->p_story_mode == true) {
			pWin->log_file << "Unticking story mode." << std::endl;
			LRESULT smuntick = SendMessage(pWin->storyMode, BM_SETCHECK, BST_UNCHECKED, NULL);
			pWin->p_story_mode = false;
		}
		ticked = true;
	}
	else {
		pWin->p_items_spells = false;
	}
}

void windowHandler::checkMonsters() {
	LRESULT monstersticked = SendMessage(pWin->monsters, BM_GETCHECK, NULL, NULL);
	if (monstersticked == BST_CHECKED) {
		pWin->log_file << "Rebalanced monsters ticked." << std::endl;
		pWin->p_monsters = true;
		if (pWin->p_story_mode == true) {
			pWin->log_file << "Unticking story mode." << std::endl;
			LRESULT smuntick = SendMessage(pWin->storyMode, BM_SETCHECK, BST_UNCHECKED, NULL);
			pWin->p_story_mode = false;
		}
		ticked = true;
	}
	else {
		pWin->p_monsters = false;
	}
}

void windowHandler::checkNormArena() {
	LRESULT normalarenaticked = SendMessage(pWin->normalarena, BM_GETCHECK, NULL, NULL);
	if (normalarenaticked == BST_CHECKED) {
		pWin->log_file << "Normal arena selected." << std::endl;
		pWin->p_barena = false;
		pWin->p_earena = false;
		ticked = true;
	}
}

void windowHandler::checkBasicArena() {
	LRESULT basicarenaticked = SendMessage(pWin->basicarena, BM_GETCHECK, NULL, NULL);
	if (basicarenaticked == BST_CHECKED) {
		pWin->log_file << "Basic arena selected." << std::endl;
		pWin->p_barena = true;
		if (pWin->p_story_mode == true) {
			pWin->log_file << "Unticking story mode." << std::endl;
			LRESULT smuntick = SendMessage(pWin->storyMode, BM_SETCHECK, BST_UNCHECKED, NULL);
			pWin->p_story_mode = false;
		}
		ticked = true;
	}
	else {
		pWin->p_barena = false;
	}
}

void windowHandler::checkExpArena() {
	LRESULT expertarenaticked = SendMessage(pWin->expertarena, BM_GETCHECK, NULL, NULL);
	if (expertarenaticked == BST_CHECKED) {
		pWin->log_file << "Expert arena selected." << std::endl;
		pWin->p_earena = true;
		if (pWin->p_story_mode == true) {
			pWin->log_file << "Unticking story mode." << std::endl;
			LRESULT smuntick = SendMessage(pWin->storyMode, BM_SETCHECK, BST_UNCHECKED, NULL);
			pWin->p_story_mode = false;
		}
		ticked = true;
	}
	else {
		pWin->p_earena = false;
	}
}

void windowHandler::checkPortraits() {
	LRESULT portraitsticked = SendMessage(pWin->portraits, BM_GETCHECK, NULL, NULL);
	if (portraitsticked == BST_CHECKED) {
		pWin->log_file << "Resized portraits ticked." << std::endl;
		pWin->p_portraits = true;
		ticked = true;
	}
	else {
		pWin->p_portraits = false;
	}
}

void windowHandler::checkFMV() {
	LRESULT fmvticked = SendMessage(pWin->fmvs, BM_GETCHECK, NULL, NULL);
	if (fmvticked == BST_CHECKED) {
		pWin->log_file << "FMV undub ticked." << std::endl;
		pWin->p_fmv = true;
		ticked = true;
	}
	else {
		pWin->p_fmv = false;
	}
}

void windowHandler::checkGraphics() {
	LRESULT graphicsticked = SendMessage(pWin->graphics, BM_GETCHECK, NULL, NULL);
	if (graphicsticked == BST_CHECKED) {
		pWin->log_file << "Graphical fixes ticked." << std::endl;
		pWin->p_graphics = true;
		ticked = true;
	}
	else {
		pWin->p_graphics = false;
	}
}

void windowHandler::checkVoice() {
	LRESULT voiceticked = SendMessage(pWin->voice, BM_GETCHECK, NULL, NULL);
	if (voiceticked == BST_CHECKED) {
		pWin->log_file << "Battle undub ticked." << std::endl;
		pWin->p_voice = true;
		ticked = true;
	}
	else {
		pWin->p_voice = false;
	}
}

void windowHandler::checkFlash() {
	LRESULT flashesticked = SendMessage(pWin->flashes, BM_GETCHECK, NULL, NULL);
	if (flashesticked == BST_CHECKED) {
		pWin->log_file << "No battle flashes ticked." << std::endl;
		pWin->p_flashes = true;
		ticked = true;
	}
	else {
		pWin->p_flashes = false;
	}
}

void windowHandler::checkRoni() {
	LRESULT roniticked = SendMessage(pWin->roni, BM_GETCHECK, NULL, NULL);
	if (roniticked == BST_CHECKED) {
		pWin->log_file << "Perfect Works Roni ticked." << std::endl;
		pWin->p_roni = true;
		ticked = true;
	}
	else {
		pWin->p_roni = false;
	}
}

void windowHandler::checkCafe() {
	LRESULT cafeticked = SendMessage(pWin->cafe, BM_GETCHECK, NULL, NULL);
	if (cafeticked == BST_CHECKED) {
		pWin->log_file << "Emeralda diner fix ticked." << std::endl;
		pWin->p_cafe = true;
		ticked = true;
	}
	else {
		pWin->p_cafe = false;
	}
}

void windowHandler::checkStoryMode() {
	LRESULT storymodeticked = SendMessage(pWin->storyMode, BM_GETCHECK, NULL, NULL);
	if (storymodeticked == BST_CHECKED) {
		pWin->log_file << "Story mode ticked." << std::endl;
		pWin->p_story_mode = true;
		pWin->log_file << "Unticking incompatible patches." << std::endl;
		pWin->p_encounters = false;
		LRESULT enctick = SendMessage(pWin->encounters, BM_SETCHECK, BST_UNCHECKED, NULL);
		pWin->p_expone = false;
		LRESULT exponeuntick = SendMessage(pWin->experience1, BM_SETCHECK, BST_UNCHECKED, NULL);
		pWin->p_exptwo = false;
		LRESULT exptwountick = SendMessage(pWin->experience2, BM_SETCHECK, BST_UNCHECKED, NULL);
		pWin->p_goldone = false;
		LRESULT goldoneuntick = SendMessage(pWin->gold1, BM_SETCHECK, BST_UNCHECKED, NULL);
		pWin->p_goldtwo = false;
		LRESULT goldtwountick = SendMessage(pWin->gold2, BM_SETCHECK, BST_UNCHECKED, NULL);
		pWin->p_monsters = false;
		LRESULT monuntick = SendMessage(pWin->monsters, BM_SETCHECK, BST_UNCHECKED, NULL);
		pWin->p_items_spells = false;
		LRESULT itemuntick = SendMessage(pWin->itemspells, BM_SETCHECK, BST_UNCHECKED, NULL);
		pWin->p_barena = false;
		pWin->p_earena = false;
		LRESULT normtick = SendMessage(pWin->normalarena, BM_SETCHECK, BST_CHECKED, NULL);
		LRESULT basicuntick = SendMessage(pWin->basicarena, BM_SETCHECK, BST_UNCHECKED, NULL);
		LRESULT expertuntick = SendMessage(pWin->expertarena, BM_SETCHECK, BST_UNCHECKED, NULL);
		ticked = true;
	}
	else {
		pWin->p_story_mode = false;
	}
}