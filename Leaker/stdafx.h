#ifndef LEAKER_STDAFX_H_
#define LEAKER_STDAFX_H_

#define WIN32_LEAN_AND_MEAN	
#define _CRT_SECURE_NO_DEPRECATE

// Required for VS 2008 (fails on XP and Win2000 without this fix)
#ifndef _WIN32_WINNT
  #define _WIN32_WINNT 0x0500
#endif

#include <windows.h>
#include <objbase.h>
#include <pcap.h>
#include <UIlib.h>
#include "ControlEvent.h"
using namespace DuiLib;

#ifdef _DEBUG
#   ifdef _UNICODE
#	 pragma comment(lib, "..\\bin\\DuiLib_ud.lib")
#   else
#       pragma comment(lib, "..\\bin\\DuiLib_d.lib")
#		pragma comment(lib,"libboost_thread-vc90-mt-gd-1_55.lib")
#   endif
#else
#	pragma comment(lib,"libboost_thread-vc90-mt-1_55.lib")
#   ifdef _UNICODE
#       pragma comment(lib, "..\\bin\\DuiLib_u.lib")
#   else
#       pragma comment(lib, "..\\bin\\DuiLib.lib")
#		pragma comment(lib,"libboost_thread-vc90-mt-1_55.lib")
#   endif
#endif

#endif

