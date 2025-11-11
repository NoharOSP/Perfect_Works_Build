#pragma once

#include "resource.h"
#include <shellapi.h>
#include <commdlg.h>
#include <iostream>
#include "romFinder.h"
#include <winuser.h>
#include <shobjidl.h> 
#include <commctrl.h>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <direct.h>
#include "toolGenerator.h"
#include "Window.h"

#define MAX_LOADSTRING 100
#define BUF_SIZE 300
#define WM_UPDATEPROGRESS WM_USER + 1

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

typedef std::basic_string<TCHAR> ustring;

enum {
	IDC_TAB = 200,
};


// Global variables
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

int discNum;
int winX = 800;
int winY = 600;
int id;
int workingDisc;
float graphicsx = 0.03;
float gameplayx = 0.18;
float arenax = 0.36;
float storyx = 0.51;
float audiox = 0.69;
float smx = 0.84;
bool pathFound1 = false;
bool pathFound2 = false;
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
bool patchPathValid = false;
bool filePathValid = false;
bool changed;
bool space = false;
bool guidrawn = false;
bool globdrawn = false;
bool successMessage = false;
bool safeDrive = false;
bool found = false;
bool checkfound = false;
std::string home = std::filesystem::current_path().string();
std::string path1 = "";
std::string path2 = "";
std::string newPath1 = "";
std::string newPath2 = "";
std::string patchPath = "\patches";
std::string filePath = "\gamefiles";
std::string tempcd1 = "Xenogears1.bin";
std::string tempcd2 = "Xenogears2.bin";
std::string tempPath;
std::string encountersName1 = "";
std::string encountersName2 = "";
std::string expName1 = "";
std::string expName2 = "";
std::string fastName1 = "";
std::string fastName2 = "";
std::string fmvName1 = "";
std::string fmvName2 = "";
std::string roniName1 = "";
std::string roniName2 = "";
std::string itemspellsName1 = "";
std::string itemspellsName2 = "";
std::string monsterName1 = "";
std::string monsterName2 = "";
std::string portraitsName1 = "";
std::string portraitsName2 = "";
std::string scriptName1 = "";
std::string scriptName2 = "";
std::string arenaName1 = "";
std::string arenaName2 = "";
std::string graphicsName1 = "";
std::string graphicsName2 = "";
std::string bugName1 = "";
std::string bugName2 = "";
std::string titleName1 = "";
std::string titleName2 = "";
std::string voiceName1 = "";
std::string voiceName2 = "";
std::string flashesName1 = "";
std::string flashesName2 = "";
std::string storyModeName1 = "";
std::string storyModeName2 = "";
std::string goldName1 = "";
std::string goldName2 = "";
std::string fmvPatch1 = "";
std::string fmvPatch2 = "";
std::string exeName = "executable";
std::string temp = "temp";
std::string slusDisc1 = "sub_executable\\disc1";
std::string slusDisc2 = "sub_executable\\disc2";
std::vector<HWND> pathList;
std::vector<HWND> buttonList;
std::vector<HWND> windList;
std::vector<std::string> patchList1;
std::vector<std::string> patchList2;
HWND window;
HWND cd1path;
HWND cd2path;
HWND browsebutton1;
HWND browsebutton2;
HWND aboutbutton;
HWND patchbutton;
HWND encounters;
HWND fasttext;
HWND basicarena;
HWND expertarena;
HWND experience1;
HWND experience2;
HWND itemspells;
HWND monsters;
HWND portraits;
HWND script;
HWND all;
HWND fmvs;
HWND graphics;
HWND voice;
HWND storyMode;
HWND flashes;
HWND roni;
HWND hwndPB;
HWND normalarena;
HWND gold1;
HWND gold2;
HWND tc;
RECT rcWindow;
RECT rc;
HDC hdc;
HDC hdc_new;
TCHAR cd1text[256];
TCHAR cd2text[256];
TCHAR graphicstext[256];
TCHAR gameplaytext[256];
TCHAR storytext[256];
TCHAR arenatext[256];
TCHAR audiotext[256];
TCHAR modestext[256];
TCHAR dummy[256];
HBRUSH hBrush;
std::ofstream cue_stream;
std::ofstream log_file;
char text_encounters[] =
"Lowers encounter rate for all areas except\n"
"for the end of disc dungeons, key grinding\n"
"spots and places where you'd be dependent\n"
"on encounter drops.";
char text_fast[] =
"Text scrolls instantly. There are noticeable\n"
"bugs with specific scenes, so the patch is\n"
"still a work in progress\n";
char text_expone[] =
"Increases experience from battle by 50%.\n";
char text_exptwo[] =
"Increases experience from battle by 100%.\n"
"If half encounters is selected, it is\n"
"recommended to choose this setting.";
char text_goldone[] =
"Increases money from battle by 50%.\n";
char text_goldtwo[] =
"Increases gold from battle by 100%.\n"
"If half encounters is selected, it is\n"
"recommended to choose this setting.";
char text_itemspells[] =
"WARNING: Incompatible with pre-0.4 saves.\n"
"The game is rebalanced; with items, spells,\n"
"stats and the damage formula receiving\n"
"adjustment. Several of these features are\n"
"receiving fine tuning, meaning some bugs\n"
"are to be expected.";
char text_monsters[] =
"Enemies and bosses will be altered to give\n"
"a bigger challenge. Recommended for second\n"
"playthroughs.";
char text_script[] =
"Retranslates names, locations, and keywords to\n"
"better align with the Japanese version and\n"
"relocalizes the script to have fewer\n"
"grammatical errors, confusing lines, and\n"
"less awkward phrasing in important scenes.\n"
"This process may take a while.";
char text_barena[] =
"Only the essentials of the rebalance\n"
"for casual play and fighting the CPU.";
char text_earena[] =
"Additional tweaks tailored to \n"
"improve the PVP experience in 2P mode.\n";
char text_portraits[] =
"Corrects the proportions of all character\n"
"portraits when running the game at its\n"
"native aspect ratio (approx 3:2).\n"
"If your emulator or scaler stretches\n"
"the game's overall aspect ratio to 4:3,\n"
"or you're using a texture hack, you do\n"
"not need this fix.";
char text_fmvs[] =
"Changes the FMVs so that they use\n"
"the Japanese audio and subtitles\n"
"which are closer to the original\n"
"script.";
char text_graphics[] =
"Fixes graphical bugs with\n"
"portraits and the battle UI. If\n"
"you are using a texture hack, you\n"
"may NOT need this fix.\n";
char text_voices[] =
"Switches to the Japanese voices\n"
"used in battle and the Speed\n"
"minigame.";
char text_narena[] =
"Uses the Battle Arena balancing\n"
"from the base game.\n";
char text_story_mode[] =
"Turns off random encounters entirely\n"
"and ensures all enemies in the game\n"
"can be killed in one hit.\n";
char text_flashes[] =
"Turns off flashes during battle.\n"
"A side effect is that the victory\n"
"screen won't dim when experience\n"
"and gold are given. Certain scenes\n"
"will also have notable graphical\n"
"bugs.";
char text_roni[] =
"Swaps Roni's portrait for his\n"
"Perfect Works design.\n";

// Global methods
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void initialise(HWND hWnd);
void initialiseWindows(HWND hWnd);
void createPaths(HWND hWnd);
void createButtons(HWND hWnd);
void graphicWindows(HWND hWnd);
void gameplayWindows(HWND hWnd);
void arenaWindows(HWND hWnd);
void storyWindows(HWND hWnd);
void audioWindows(HWND hWnd);
void modeWindows(HWND hWnd);
void initialiseFont();
void initialisePatchLists();
void checkboxLock();
void patchBoxLock();
void relock();
void reinitialisePatches();
void clearText();
void tooltipTextMaker(HWND hWnd);
void drawText();
void monsterEdits(std::string file);
void exeEdits(std::string file);
void battleExeEdits(std::string file);
bool applyPatch(int discNum);
void restoreDefaults();
void clearPatchLists();
void openFile(HWND hWnd);
void windowSelect();
void patchProcess(HWND hWnd);
void paintProcess(HWND hWnd);
void minMaxInfo(LPARAM lParam);