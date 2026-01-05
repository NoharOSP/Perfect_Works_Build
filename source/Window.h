#pragma once
#include "pch.h"
#include <vector>
#include <string>
#include <iostream>
#include "toolGenerator.h"
#include "Resource.h"
#include "romFinder.h"
#include "patchProcessor.h"
#include "paintWindow.h"
#include "windowHandler.h"

struct tipDesc {
	char text_encounters[255] =
		"Lowers encounter rate for all areas except\n"
		"for the end of disc dungeons, key grinding\n"
		"spots and places where you'd be dependent\n"
		"on encounter drops.";
	char text_fast[255] =
		"Text scrolls instantly. There are noticeable\n"
		"bugs with specific scenes, so the patch is\n"
		"still a work in progress\n";
	char text_expone[255] =
		"Increases experience from battle by 50%.\n";
	char text_exptwo[255] =
		"Increases experience from battle by 100%.\n"
		"If half encounters is selected, it is\n"
		"recommended to choose this setting.";
	char text_goldone[255] =
		"Increases money from battle by 50%.\n";
	char text_goldtwo[255] =
		"Increases gold from battle by 100%.\n"
		"If half encounters is selected, it is\n"
		"recommended to choose this setting.";
	char text_itemspells[255] =
		"WARNING: Incompatible with pre-0.4 saves.\n"
		"The game is rebalanced; with items,\n" 
		"characters and the damage formula receiving\n"
		"adjustment. Other significant overhauls are\n"
		"included. Several features need fine tuning,\n"
		"meaning some bugs are to be expected.";
	char text_monsters[255] =
		"Enemies and bosses will be altered to give\n"
		"a bigger challenge. Recommended for second\n"
		"playthroughs.";
	char text_script[255] =
		"Retranslates names, locations, and keywords to\n"
		"better align with the Japanese version and\n"
		"relocalizes the script to have fewer\n"
		"grammatical errors, confusing lines, and\n"
		"less awkward phrasing in important scenes.\n"
		"This process may take a while.";
	char text_barena[255] =
		"Only the essentials of the rebalance\n"
		"for casual play and fighting the CPU.";
	char text_earena[255] =
		"Additional tweaks tailored to \n"
		"improve the PVP experience in 2P mode.\n";
	char text_portraits[255] =
		"Corrects the proportions of all character\n"
		"portraits when running the game at its\n"
		"native aspect ratio (approx 3:2).\n"
		"If your emulator or scaler stretches\n"
		"the game's overall aspect ratio to 4:3,\n"
		"or you're using a texture hack, you do\n"
		"not need this fix.";
	char text_fmvs[255] =
		"Changes the FMVs so that they use\n"
		"the Japanese audio and subtitles\n"
		"which are closer to the original\n"
		"script.";
	char text_graphics[255] =
		"Fixes graphical bugs with\n"
		"portraits and the battle UI. If\n"
		"you are using a texture hack, you\n"
		"may NOT need this fix.\n";
	char text_voices[255] =
		"Switches to the Japanese voices\n"
		"used in battle and the Speed\n"
		"minigame.";
	char text_narena[255] =
		"Uses the Battle Arena balancing\n"
		"from the base game.\n";
	char text_story_mode[255] =
		"Turns off random encounters entirely\n"
		"and ensures all enemies in the game\n"
		"can be killed in one hit.\n";
	char text_flashes[255] =
		"Turns off flashes during battle.\n"
		"A side effect is that the victory\n"
		"screen won't dim when experience\n"
		"and gold are given. Certain scenes\n"
		"will also have notable graphical\n"
		"bugs.";
	char text_roni[255] =
		"Swaps Roni's portrait for his\n"
		"Perfect Works design.\n";
	char text_cafe[255] =
		"Emeralda now uses a sitting\n"
		"sprite over her nanoreactor\n"
		"one when eating at the Aveh\n"
		"cafe.\n";
};

class windowHandler;

class Window
{
	// Global mathods
public:
	Window(HWND hWnd, HINSTANCE hInst, int axisX, int axisY, LPWSTR szTitle);
	~Window();
	void initialise();
	void paintProcess();
	void checkboxLock();
	void patchBoxLock();
	void tooltipTextMaker();
	void openFile(HWND hWnd);
	void windowSelect();
	void process();
	void restoreDefaults();

	// Global variables
public:
	HWND winHwnd;
	HWND cd1path;
	HWND cd2path;
	HWND browsebutton1;
	HWND browsebutton2;
	HWND aboutbutton;
	HWND patchbutton;
	HWND portraits;
	HWND graphics;
	HWND flashes;
	HWND roni;
	HWND cafe;
	HWND encounters;
	HWND experience1;
	HWND experience2;
	HWND gold1;
	HWND gold2;
	HWND itemspells;
	HWND monsters;
	HWND normalarena;
	HWND basicarena;
	HWND expertarena;
	HWND fasttext;
	HWND script;
	HWND fmvs;
	HWND voice;
	HWND storyMode;
	HINSTANCE winInst;
	int winX;
	int winY;
	int discNum;
	float graphicsx = 0.03;
	float gameplayx = 0.18;
	float arenax = 0.36;
	float storyx = 0.51;
	float audiox = 0.69;
	float smx = 0.84;
	std::vector<HWND> pathList;
	std::vector<HWND> buttonList;
	std::vector<HWND> windList;
	bool pathFound1 = false;
	bool pathFound2 = false;
	bool found = false;
	bool checkfound = false;
	bool p_encounters = false;
	bool p_expone = false;
	bool p_exptwo = false;
	bool p_fastold = false;
	bool p_fastnew = false;
	bool p_items_spells = false;
	bool p_monsters = false;
	bool p_portraits = false;
	bool p_script = false;
	bool p_barena = false;
	bool p_earena = false;
	bool p_fmv = false;
	bool p_graphics = false;
	bool p_voice = false;
	bool p_flashes = false;
	bool p_story_mode = false;
	bool p_goldone = false;
	bool p_goldtwo = false;
	bool p_roni = false;
	bool p_cafe = false;
	std::ofstream log_file;
	std::string path1 = "";
	std::string path2 = "";
	std::string home;
	LPWSTR title;
	windowHandler* pHandle;
};

