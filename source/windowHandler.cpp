#include "pch.h"
#include "windowHandler.h"

windowHandler::windowHandler(Window* win) {
	pWin = win;
}

windowHandler::~windowHandler() {

}

bool windowHandler::check() {
	// TODO: Make arena, graphics and audio handlers
	handleScript hs;
	handleGameplay hg;
	hs.checkScript(pWin, this);
	hg.checkEnc(pWin, this);
	hs.checkFast(pWin, this);
	hg.checkExpOne(pWin, this);
	hg.checkExpTwo(pWin, this);
	hg.checkGoldOne(pWin, this);
	hg.checkGoldTwo(pWin, this);
	hg.checkItemsParty(pWin, this);
	hg.checkMonsters(pWin, this);
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