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

#define MAX_LOADSTRING 100
#define BUF_SIZE 300
#define WM_UPDATEPROGRESS WM_USER + 1

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

typedef std::basic_string<TCHAR> ustring;

enum {
	IDC_TAB = 200,
};

// Global variables

int discNum;
int winX = 800;
int winY = 600;
int id;
int workingDisc;
int InsertTab(HWND TabController, const ustring& txt, int item_index, int image_index, UINT mask);
int tabNo;
float graphicsx = 0.03;
float gameplayx = 0.23;
float arenax = 0.43;
float storyx = 0.63;
float audiox = 0.83;
float smx = 0.03;
bool pathFound1 = false;
bool pathFound2 = false;
bool p_encounters = false;
bool p_exp_gold = false;
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
bool p_no_encounters = false;
bool p_zero_HP = false;
bool patchPathValid = false;
bool filePathValid = false;
bool changed;
bool space = false;
std::string home = "";
std::string path1 = "";
std::string path2 = "";
std::string newPath1 = "";
std::string newPath2 = "";
std::string patchPath = "\patches";
std::string filePath = "\gamefiles";
std::string spacePath = "\\xenotemp";
std::string tempcd1 = "Xenogears1.bin";
std::string tempcd2 = "Xenogears2.bin";
std::string tempPath;
std::string encountersName1 = "";
std::string encountersName2 = "";
std::string expgoldName1 = "";
std::string expgoldName2 = "";
std::string fastName1 = "";
std::string fastName2 = "";
std::string fmvName1 = "";
std::string fmvName2 = "";
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
std::string noEncountersName1 = "";
std::string noEncountersName2 = "";
std::string zeroHPName1 = "";
std::string zeroHPName2 = "";
std::vector<HWND> globalWindList;
std::vector<HWND> generalWindList;
std::vector<HWND> miscWindList;
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
HWND expgold;
HWND itemspells;
HWND monsters;
HWND portraits;
HWND script;
HWND all;
HWND fmvs;
HWND graphics;
HWND voice;
HWND storyMode;
HWND hwndPB;
HWND normalarena;
HWND CreateTabController(HWND hParent, HINSTANCE hInst, DWORD dwStyle, const RECT& rc, const int id);
HWND tc;
RECT rcWindow;
RECT rc;
TOOLINFO toolInfo;
HDC hdc;
TCHAR cd1text[256];
TCHAR cd2text[256];
TCHAR graphicstext[256];
TCHAR gameplaytext[256];
TCHAR storytext[256];
TCHAR arenatext[256];
TCHAR audiotext[256];
TCHAR dummy[256];
std::ofstream cue_stream;
void DestroyTabs(const HWND hWnd);
void removeGeneralButtons();
void removeMiscButtons();
void generalButtonCustomiser(HWND hWnd);
void miscButtonCustomiser(HWND hWnd);

// Global methods

void initialiseGlobalButtonList();
void initialisePatchLists();
void checkboxLock();
void patchBoxLock();
void relock();
void reinitialisePatches();
void clearText();
void tooltipTextMaker(HWND hWnd);
void StartCommonControls(DWORD flags);
void initialiseGlobalWindows(HWND hWnd);
void initialiseGlobalFont();
void drawGUIText();
void drawGlobalText();
void applyPatch(int discNum);
HWND CreateToolTip(HWND hParent, HWND hText, HINSTANCE hInst, PTSTR pszText);
HWND toolGenerator(char* text, HWND hWnd, HWND hText);