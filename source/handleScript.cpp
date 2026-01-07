#include "pch.h"
#include "handleScript.h"

handleScript::handleScript() {

}

handleScript::~handleScript() {

}

void handleScript::checkScript(Window* pWin, windowHandler* wh) {
	LRESULT scriptticked = SendMessage(pWin->script, BM_GETCHECK, NULL, NULL);
	if (scriptticked == BST_CHECKED) {
		pWin->log_file << "Script changes ticked." << std::endl;
		pWin->p_script = true;
		if (pWin->p_fastold) {
			pWin->log_file << "Ensure the version of fast text is used that supports the updated script." << std::endl;
			pWin->p_fastold = false;
			pWin->p_fastnew = true;
		}
		wh->ticked = true;
	}
	else {
		pWin->p_script = false;
	}
}

void handleScript::checkFast(Window* pWin, windowHandler* wh) {
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
		wh->ticked = true;
	}
	else {
		pWin->p_fastold = false;
		pWin->p_fastnew = false;
	}
}