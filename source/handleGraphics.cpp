#include "pch.h"
#include "handleGraphics.h"

handleGraphics::handleGraphics() {

}

handleGraphics::~handleGraphics() {

}

void handleGraphics::checkResize(Window* pWin, windowHandler* wh) {
	LRESULT resizeticked = SendMessage(pWin->resize, BM_GETCHECK, NULL, NULL);
	if (resizeticked == BST_CHECKED) {
		pWin->log_file << "Resized portraits ticked." << std::endl;
		pWin->p_resize = true;
		if (pWin->p_portraits == true) {
			pWin->log_file << "Unticking portrait fixes." << std::endl;
			LRESULT portraituntick = SendMessage(pWin->portraits, BM_SETCHECK, BST_UNCHECKED, NULL);
			pWin->p_portraits = false;
		}
		wh->ticked = true;
	}
	else {
		pWin->p_resize = false;
	}
}

void handleGraphics::checkPortraits(Window* pWin, windowHandler* wh) {
	LRESULT portraitsticked = SendMessage(pWin->portraits, BM_GETCHECK, NULL, NULL);
	if (portraitsticked == BST_CHECKED) {
		pWin->log_file << "Portraits ticked." << std::endl;
		pWin->p_portraits = true;
		if (pWin->p_resize == true) {
			pWin->log_file << "Unticking resized portraits." << std::endl;
			LRESULT resizeuntick = SendMessage(pWin->resize, BM_SETCHECK, BST_UNCHECKED, NULL);
			pWin->p_resize = false;
		}
		wh->ticked = true;
	}
	else {
		pWin->p_portraits = false;
	}
}

void handleGraphics::checkFlash(Window* pWin, windowHandler* wh) {
	LRESULT flashesticked = SendMessage(pWin->flashes, BM_GETCHECK, NULL, NULL);
	if (flashesticked == BST_CHECKED) {
		pWin->log_file << "No battle flashes ticked." << std::endl;
		pWin->p_flashes = true;
		wh->ticked = true;
	}
	else {
		pWin->p_flashes = false;
	}
}

void handleGraphics::checkRoni(Window* pWin, windowHandler* wh) {
	LRESULT roniticked = SendMessage(pWin->roni, BM_GETCHECK, NULL, NULL);
	if (roniticked == BST_CHECKED) {
		pWin->log_file << "Perfect Works Roni ticked." << std::endl;
		pWin->p_roni = true;
		wh->ticked = true;
	}
	else {
		pWin->p_roni = false;
	}
}

void handleGraphics::checkCafe(Window* pWin, windowHandler* wh) {
	LRESULT cafeticked = SendMessage(pWin->cafe, BM_GETCHECK, NULL, NULL);
	if (cafeticked == BST_CHECKED) {
		pWin->log_file << "Emeralda diner fix ticked." << std::endl;
		pWin->p_cafe = true;
		wh->ticked = true;
	}
	else {
		pWin->p_cafe = false;
	}
}