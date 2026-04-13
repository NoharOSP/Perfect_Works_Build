#include "pch.h"
#include "windowHandler.h"

void windowHandler::checkGraphics() {
	resizeticked = SendMessage(Window::resize, BM_GETCHECK, NULL, NULL);
	if (resizeticked == BST_CHECKED && portraitsticked == BST_CHECKED) {
		SendMessage(Window::portraits, BM_SETCHECK, BST_UNCHECKED, NULL);
	}
	portraitsticked = SendMessage(Window::portraits, BM_GETCHECK, NULL, NULL);
	if (resizeticked == BST_CHECKED && portraitsticked == BST_CHECKED) {
		SendMessage(Window::resize, BM_SETCHECK, BST_UNCHECKED, NULL);
	}
	flashesticked = SendMessage(Window::flashes, BM_GETCHECK, NULL, NULL);
	roniticked = SendMessage(Window::roni, BM_GETCHECK, NULL, NULL);
	cafeticked = SendMessage(Window::cafe, BM_GETCHECK, NULL, NULL);
}

void windowHandler::checkGameplay() {
	encticked = SendMessage(Window::encounters, BM_GETCHECK, NULL, NULL);
	expticked = SendMessage(Window::experience, BM_GETCHECK, NULL, NULL);
	goldticked = SendMessage(Window::gold, BM_GETCHECK, NULL, NULL);
	itemspellsticked = SendMessage(Window::itemspells, BM_GETCHECK, NULL, NULL);
	monstersticked = SendMessage(Window::monsters, BM_GETCHECK, NULL, NULL);
	deathblowsticked = SendMessage(Window::deathblows, BM_GETCHECK, NULL, NULL);
}

void windowHandler::checkArena() {
	normalarenaticked = SendMessage(Window::normalarena, BM_GETCHECK, NULL, NULL);
	basicarenaticked = SendMessage(Window::basicarena, BM_GETCHECK, NULL, NULL);
	expertarenaticked = SendMessage(Window::expertarena, BM_GETCHECK, NULL, NULL);
	for (int i = 0; i < Window::gameplayWindList.size(); i++) {
		LRESULT boxticked = SendMessage(Window::gameplayWindList[i], BM_GETCHECK, NULL, NULL);
		if (boxticked == BST_CHECKED && storymodeticked == BST_CHECKED) {
			LRESULT smuntick = SendMessage(Window::storyMode, BM_SETCHECK, BST_UNCHECKED, NULL);
		}
	}
}

void windowHandler::checkStory() {
	scriptticked = SendMessage(Window::script, BM_GETCHECK, NULL, NULL);
	fastticked = SendMessage(Window::fasttext, BM_GETCHECK, NULL, NULL);
}

void windowHandler::checkAudio() {
	fmvticked = SendMessage(Window::fmvs, BM_GETCHECK, NULL, NULL);
	voiceticked = SendMessage(Window::voice, BM_GETCHECK, NULL, NULL);
	musicticked = SendMessage(Window::music, BM_GETCHECK, NULL, NULL);
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
	}
	jpnticked = SendMessage(Window::jpnControls, BM_GETCHECK, NULL, NULL);
}