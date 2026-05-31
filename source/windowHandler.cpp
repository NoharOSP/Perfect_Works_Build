#include "pch.h"
#include "windowHandler.h"

// TODO: Fix checkbox logic for bug fix only
void windowHandler::checkGraphics() {
	resizeticked = SendMessage(Window::resize, BM_GETCHECK, NULL, NULL);
	if (resizeticked == BST_CHECKED) {
		if (portraitsticked == BST_CHECKED) {
			SendMessage(Window::portraits, BM_SETCHECK, BST_UNCHECKED, NULL);
		}
		if (bugticked == BST_CHECKED) {
			SendMessage(Window::bugFix, BM_SETCHECK, BST_UNCHECKED, NULL);
		}
	}
	portraitsticked = SendMessage(Window::portraits, BM_GETCHECK, NULL, NULL);
	if (portraitsticked == BST_CHECKED) {
		if (resizeticked) {
			SendMessage(Window::resize, BM_SETCHECK, BST_UNCHECKED, NULL);
		}
		if (bugticked == BST_CHECKED) {
			SendMessage(Window::bugFix, BM_SETCHECK, BST_UNCHECKED, NULL);
		}
	}
	flashesticked = SendMessage(Window::flashes, BM_GETCHECK, NULL, NULL);
	if (flashesticked == BST_CHECKED && bugticked == BST_CHECKED) {
		SendMessage(Window::bugFix, BM_SETCHECK, BST_UNCHECKED, NULL);
	}
	roniticked = SendMessage(Window::roni, BM_GETCHECK, NULL, NULL);
	if (roniticked == BST_CHECKED && bugticked == BST_CHECKED) {
		SendMessage(Window::bugFix, BM_SETCHECK, BST_UNCHECKED, NULL);
	}
	cafeticked = SendMessage(Window::cafe, BM_GETCHECK, NULL, NULL);
	if (cafeticked == BST_CHECKED && bugticked == BST_CHECKED) {
		SendMessage(Window::bugFix, BM_SETCHECK, BST_UNCHECKED, NULL);
	}
}

void windowHandler::checkGameplay() {
	encticked = SendMessage(Window::encounters, BM_GETCHECK, NULL, NULL);
	if (encticked == BST_CHECKED && bugticked == BST_CHECKED) {
		SendMessage(Window::bugFix, BM_SETCHECK, BST_UNCHECKED, NULL);
	}
	expticked = SendMessage(Window::experience, BM_GETCHECK, NULL, NULL);
	if (expticked) {
		EnableWindow(Window::expList, true);
		SendMessage(Window::bugFix, BM_SETCHECK, BST_UNCHECKED, NULL);
	}
	else {
		EnableWindow(Window::expList, false);
	}
	goldticked = SendMessage(Window::gold, BM_GETCHECK, NULL, NULL);
	if (goldticked) {
		EnableWindow(Window::goldList, true);
		SendMessage(Window::bugFix, BM_SETCHECK, BST_UNCHECKED, NULL);
	}
	else {
		EnableWindow(Window::goldList, false);
	}
	itemspellsticked = SendMessage(Window::itemspells, BM_GETCHECK, NULL, NULL);
	if (itemspellsticked) {
		SendMessage(Window::bugFix, BM_SETCHECK, BST_UNCHECKED, NULL);
	}
	monstersticked = SendMessage(Window::monsters, BM_GETCHECK, NULL, NULL);
	if (monstersticked) {
		SendMessage(Window::bugFix, BM_SETCHECK, BST_UNCHECKED, NULL);
	}
	deathblowsticked = SendMessage(Window::deathblows, BM_GETCHECK, NULL, NULL);
	if (deathblowsticked) {
		SendMessage(Window::bugFix, BM_SETCHECK, BST_UNCHECKED, NULL);
	}
	capticked = SendMessage(Window::cap, BM_GETCHECK, NULL, NULL);
	if (capticked) {
		SendMessage(Window::bugFix, BM_SETCHECK, BST_UNCHECKED, NULL);
	}
}

void windowHandler::checkArena() {
	normalarenaticked = SendMessage(Window::normalarena, BM_GETCHECK, NULL, NULL);
	basicarenaticked = SendMessage(Window::basicarena, BM_GETCHECK, NULL, NULL);
	if (basicarenaticked) {
		SendMessage(Window::bugFix, BM_SETCHECK, BST_UNCHECKED, NULL);
	}
	expertarenaticked = SendMessage(Window::expertarena, BM_GETCHECK, NULL, NULL);
	if (expertarenaticked) {
		SendMessage(Window::bugFix, BM_SETCHECK, BST_UNCHECKED, NULL);
	}
	for (int i = 0; i < Window::gameplayWindList.size(); i++) {
		LRESULT boxticked = SendMessage(Window::gameplayWindList[i], BM_GETCHECK, NULL, NULL);
		if (boxticked == BST_CHECKED && storymodeticked == BST_CHECKED) {
			LRESULT smuntick = SendMessage(Window::storyMode, BM_SETCHECK, BST_UNCHECKED, NULL);
		}
	}
}

void windowHandler::checkStory() {
	scriptticked = SendMessage(Window::script, BM_GETCHECK, NULL, NULL);
	if (scriptticked) {
		SendMessage(Window::bugFix, BM_SETCHECK, BST_UNCHECKED, NULL);
	}
	fastticked = SendMessage(Window::fasttext, BM_GETCHECK, NULL, NULL);
	if (fastticked == BST_CHECKED && instantticked == BST_CHECKED) {
		SendMessage(Window::instant, BM_SETCHECK, BST_UNCHECKED, NULL);
		SendMessage(Window::bugFix, BM_SETCHECK, BST_UNCHECKED, NULL);
	}
	instantticked = SendMessage(Window::instant, BM_GETCHECK, NULL, NULL);
	if (fastticked == BST_CHECKED && instantticked == BST_CHECKED) {
		SendMessage(Window::fasttext, BM_SETCHECK, BST_UNCHECKED, NULL);
		SendMessage(Window::bugFix, BM_SETCHECK, BST_UNCHECKED, NULL);
	}
}

void windowHandler::checkAudio() {
	fmvticked = SendMessage(Window::fmvs, BM_GETCHECK, NULL, NULL);
	if (fmvticked) {
		SendMessage(Window::bugFix, BM_SETCHECK, BST_UNCHECKED, NULL);
	}
	voiceticked = SendMessage(Window::voice, BM_GETCHECK, NULL, NULL);
	if (voiceticked) {
		SendMessage(Window::bugFix, BM_SETCHECK, BST_UNCHECKED, NULL);
	}
	musicticked = SendMessage(Window::music, BM_GETCHECK, NULL, NULL);
	if (musicticked) {
		SendMessage(Window::bugFix, BM_SETCHECK, BST_UNCHECKED, NULL);
	}
}

void windowHandler::checkModes() {
	storymodeticked = SendMessage(Window::storyMode, BM_GETCHECK, NULL, NULL);
	if (storymodeticked == BST_CHECKED) {
		for (int i = 0; i < Window::gameplayWindList.size(); i++) {
			LRESULT boxticked = SendMessage(Window::gameplayWindList[i], BM_GETCHECK, NULL, NULL);
			if (boxticked == BST_CHECKED) {
				boxticked = SendMessage(Window::gameplayWindList[i], BM_SETCHECK, BST_UNCHECKED, NULL);
			}
		}
		SendMessage(Window::bugFix, BM_SETCHECK, BST_UNCHECKED, NULL);
	}
	jpnticked = SendMessage(Window::jpnControls, BM_GETCHECK, NULL, NULL);
	if (jpnticked) {
		SendMessage(Window::bugFix, BM_SETCHECK, BST_UNCHECKED, NULL);
	}
	bugticked = SendMessage(Window::bugFix, BM_GETCHECK, NULL, NULL);
	if (bugticked == BST_CHECKED) {
		for (int i = 0; i < Window::windList.size(); i++) {
			LRESULT boxticked = SendMessage(Window::windList[i], BM_GETCHECK, NULL, NULL);
			if (Window::windList[i] != Window::normalarena && Window::windList[i] != Window::bugFix) {
				if (boxticked == BST_CHECKED) {
					boxticked = SendMessage(Window::windList[i], BM_SETCHECK, BST_UNCHECKED, NULL);
				}
			}
		}
	}
}