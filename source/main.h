#pragma once

#include "resource.h"
#include <shellapi.h>
#include <commdlg.h>
#include <iostream>
#include <winuser.h>
#include <shobjidl.h> 
#include <commctrl.h>
#include <vector>
#include <cstdlib>
#include <cstdio>
#include <direct.h>
#include "toolGenerator.h"
#include "Window.h"
#include "framework.h"

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

Window* pWin;
int winX = 800;
int winY = 600;
int id;
int workingDisc;
bool changed;
bool space = false;
bool guidrawn = false;
bool globdrawn = false;
bool safeDrive = false;
std::string newPath1 = "";
std::string newPath2 = "";
HWND window;
HWND hwndPB;
HWND tc;
RECT rc;
HDC hdc_new;
HBRUSH hBrush;

// Global methods
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void minMaxInfo(LPARAM lParam);