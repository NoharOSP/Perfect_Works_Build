#include "pch.h"
#include "handleGameplay.h"

handleGameplay::handleGameplay() {

}

handleGameplay::~handleGameplay() {

}

void handleGameplay::checkEnc(Window* pWin, windowHandler* wh) {
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
			wh->ticked = true;
		}
	}
	else {
		pWin->p_encounters = false;
	}
}

void handleGameplay::checkExpOne(Window* pWin, windowHandler* wh) {
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
		wh->ticked = true;
	}
	else {
		pWin->p_expone = false;
	}
}

void handleGameplay::checkExpTwo(Window* pWin, windowHandler* wh) {
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
		wh->ticked = true;
	}
	else {
		pWin->p_exptwo = false;
	}
}

void handleGameplay::checkGoldOne(Window* pWin, windowHandler* wh) {
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
		wh->ticked = true;
	}
	else {
		pWin->p_goldone = false;
	}
}

void handleGameplay::checkGoldTwo(Window* pWin, windowHandler* wh) {
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
		wh->ticked = true;
	}
	else {
		pWin->p_goldtwo = false;
	}
}

void handleGameplay::checkItemsParty(Window* pWin, windowHandler* wh) {
	LRESULT itemspellsticked = SendMessage(pWin->itemspells, BM_GETCHECK, NULL, NULL);
	if (itemspellsticked == BST_CHECKED) {
		pWin->log_file << "Rebalanced party/items ticked." << std::endl;
		pWin->p_items_spells = true;
		if (pWin->p_story_mode == true) {
			pWin->log_file << "Unticking story mode." << std::endl;
			LRESULT smuntick = SendMessage(pWin->storyMode, BM_SETCHECK, BST_UNCHECKED, NULL);
			pWin->p_story_mode = false;
		}
		wh->ticked = true;
	}
	else {
		pWin->p_items_spells = false;
	}
}

void handleGameplay::checkMonsters(Window* pWin, windowHandler* wh) {
	LRESULT monstersticked = SendMessage(pWin->monsters, BM_GETCHECK, NULL, NULL);
	if (monstersticked == BST_CHECKED) {
		pWin->log_file << "Rebalanced monsters ticked." << std::endl;
		pWin->p_monsters = true;
		if (pWin->p_story_mode == true) {
			pWin->log_file << "Unticking story mode." << std::endl;
			LRESULT smuntick = SendMessage(pWin->storyMode, BM_SETCHECK, BST_UNCHECKED, NULL);
			pWin->p_story_mode = false;
		}
		wh->ticked = true;
	}
	else {
		pWin->p_monsters = false;
	}
}

void handleGameplay::checkDeathblows(Window* pWin, windowHandler* wh) {
	LRESULT deathblowsticked = SendMessage(pWin->deathblows, BM_GETCHECK, NULL, NULL);
	if (deathblowsticked == BST_CHECKED) {
		pWin->log_file << "No Deathblow levels ticked." << std::endl;
		pWin->p_deathblow = true;
		if (pWin->p_story_mode == true) {
			pWin->log_file << "Unticking story mode." << std::endl;
			LRESULT smuntick = SendMessage(pWin->storyMode, BM_SETCHECK, BST_UNCHECKED, NULL);
			pWin->p_story_mode = false;
		}
		wh->ticked = true;
	}
	else {
		pWin->p_deathblow = false;
	}
}