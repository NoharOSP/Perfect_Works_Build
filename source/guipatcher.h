#pragma once

#include "resource.h"
#include <shellapi.h>
#include <commdlg.h>
#include <iostream>
#include "romFinder.h"
#include <winuser.h>
#include <shobjidl.h> 
#include "PatchProcess.h"
#include <commctrl.h>
#include <vector>


#define MAX_LOADSTRING 100
#define BUF_SIZE 300
#define _CRTDBG_MAP_ALLOC //to get more details
#include <crtdbg.h>   //for malloc and free
#include <errno.h>

#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

