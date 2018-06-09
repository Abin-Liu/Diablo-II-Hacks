// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
// TODO: reference additional headers your program requires here
#define GetFileNameFromPath(path) (_tcsrchr(path, _T('\\'))+1)
#ifndef COUNTOF
#define COUNTOF(x)	sizeof((x)) / sizeof(x[0])
#endif



#define mymbstowcs(wcstr, cb_wcstr, mbstr, cb_mbstr)	\
	MultiByteToWideChar(CP_ACP,					\
	0,										\
	mbstr,									\
	cb_mbstr,								\
	wcstr,									\
	cb_wcstr)

#define mymbstowcs2(wcstr, mbstr) mymbstowcs(wcstr, COUNTOF(wcstr), mbstr, -1)

#define mywcstombs(mbstr, cb_mbstr, wcstr, cb_wcstr)	\
	WideCharToMultiByte(CP_ACP,					\
	0,										\
	wcstr,									\
	cb_wcstr,								\
	mbstr,									\
	cb_mbstr,								\
	NULL,									\
	NULL)

#define mywcstombs2(mbstr, wcstr) mywcstombs(mbstr, COUNTOF(mbstr), wcstr, -1)

#ifdef UNICODE
#define useconv_towcstr(wcstr, tstr) \
	LPCWSTR wcstr = tstr;
#define useconv_tombstr(mbstr, tstr) \
	char mbstr[MAX_PATH]; \
	mywcstombs2(mbstr, tstr);
#else // multibytes
#define useconv_towcstr(wcstr, tstr) \
	WCHAR wcstr[MAX_PATH]; \
	mymbstowcs2(wcstr, tstr);
#define useconv_tombstr(mbstr, tstr) \
	LPCSTR mbstr = tstr;
#endif // UNICODE

extern BOOL g_isNTPlatform;