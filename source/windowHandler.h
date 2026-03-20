#pragma once
#include "Window.h"

class windowHandler
{
	// Global methods
public:
	static void checkGraphics();
	static void checkGameplay();
	static void checkArena();
	static void checkStory();
	static void checkAudio();
	static void checkModes();
	inline static LRESULT resizeticked;
	inline static LRESULT portraitsticked;
	inline static LRESULT flashesticked;
	inline static LRESULT roniticked;
	inline static LRESULT cafeticked;
	inline static LRESULT encticked;
	inline static LRESULT exponeticked;
	inline static LRESULT exptwoticked;
	inline static LRESULT goldoneticked;
	inline static LRESULT goldtwoticked;
	inline static LRESULT itemspellsticked;
	inline static LRESULT monstersticked;
	inline static LRESULT deathblowsticked;
	inline static LRESULT normalarenaticked;
	inline static LRESULT basicarenaticked;
	inline static LRESULT expertarenaticked;
	inline static LRESULT scriptticked;
	inline static LRESULT fastticked;
	inline static LRESULT fmvticked;
	inline static LRESULT voiceticked;
	inline static LRESULT musicticked;
	inline static LRESULT storymodeticked;
	inline static LRESULT jpnticked;
};

