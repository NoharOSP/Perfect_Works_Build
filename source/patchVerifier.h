#pragma once
#include "patchProcessor.h"

class patchVerifier
{
	// Global methods
public:
	static void verify();
	static void graphicsVerify();
	static void gameplayVerify();
	static void monsterFileVerify();
	static void arenaVerify();
	static void storyVerify();
	static void audioVerify();
	static void modeVerify();
	static void bugVerify();
	static void titleVerify();

	// Global variables
public:
	inline static std::string patchPath = "\patches";
	inline static bool patchPathValid = false;
};

