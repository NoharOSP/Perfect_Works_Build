#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#if !defined(AFX_STDAFX_H__57026808_9147_11D7_BD86_00E098852DE0__INCLUDED_)
#define AFX_STDAFX_H__57026808_9147_11D7_BD86_00E098852DE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

#define _AFXDLL
#include <afxwin.h>
#include <afxext.h> 
#include <afxdtctl.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>	
#endif

#endif

#define WINVER 0x0501
#define _WIN32_WINNT = 0x0501
#define ISOLATION_AWARE_ENABLED 1

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>