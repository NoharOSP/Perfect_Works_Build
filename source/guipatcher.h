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
#include "writeFile.h"
#include <cstdlib>
#include <cstdio>
#include "patchChecker.h"

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
int winX = 640;
int winY = 380;
int id;
int tabNo;
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
bool p_stats = false;
bool p_arena = false;
bool patchPathValid = false;
std::string home = "";
std::string path1 = "";
std::string path2 = "";
std::string patchPath = "\patches";
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
std::string statName1 = "";
std::string statName2 = "";
std::string arenaName1 = "";
std::string arenaName2 = "";
std::vector<HWND> globalWindList;
std::vector<HWND> generalWindList;
std::vector<HWND> miscWindList;
std::vector<std::string> patchList1;
std::vector<std::string> patchList2;
HWND cd1path;
HWND cd2path;
HWND browsebutton1;
HWND browsebutton2;
HWND aboutbutton;
HWND patchbutton;
HWND encounters;
HWND fasttext;
HWND arena;
HWND expgold;
HWND itemspells;
HWND monsters;
HWND portraits;
HWND stats;
HWND script;
HWND all;
HWND easy;
HWND hard;
HWND tc;
HWND hwndPB;
RECT rcWindow;
RECT rc;
TOOLINFO toolInfo;
HDC hdc;
TCHAR cd1text[256];
TCHAR cd2text[256];
TCHAR qoltext[256];
TCHAR balancetext[256];
TCHAR storytext[256];
TCHAR misctext[256];
TCHAR modetext[256];
TCHAR dummy[256];
std::ofstream disc1_cue;
std::ofstream disc2_cue;

// Global methods

void initialiseGlobalButtonList();
void initialiseGeneralButtonList();
void initialiseMiscButtonList();
void initialisePatchLists();
void removeGeneralButtons();
void removeMiscButtons();
void checkboxLock();
void patchBoxLock();
void relock();
void reinitialisePatches();
void clearText();
void initialiseGeneralWindows(HWND hWnd);
void tooltipTextMaker(HWND hWnd);
void StartCommonControls(DWORD flags);
void DestroyTabs(const HWND hWnd);
void initialiseGlobalWindows(HWND hWnd);
void initialiseMiscWindows(HWND hWnd);
void initialiseGlobalFont();
void initialiseGeneralFont();
void initialiseMiscFont();
void generalButtonCustomiser(HWND hWnd);
void miscButtonCustomiser(HWND hWnd);
void drawGUIText();
void drawGlobalText();
HWND CreateToolTip(HWND hParent, HWND hText, HINSTANCE hInst, PTSTR pszText);
HWND toolGenerator(char* text, HWND hWnd, HWND hText);
HWND CreateTabController(HWND hParent, HINSTANCE hInst, DWORD dwStyle, const RECT& rc, const int id);
int InsertTab(HWND TabController, const ustring& txt, int item_index, int image_index, UINT mask);
BOOL ParseALargeFile(HWND hWnd, LPTSTR lpszFileName);
