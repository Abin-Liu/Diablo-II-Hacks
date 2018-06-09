#include "D2Stdio.h"
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include "..\\D2Hackit\\D2Hackit.h"
#include "..\\D2Hackit\\D2Client.h"

void D2Errorf(LPCTSTR lpszFormat, ...)
{
	if (lpszFormat == NULL || lpszFormat[0] == _T('\0'))
		return;

	TCHAR szString[256] = "";
	va_list args;
	va_start(args, lpszFormat);
	_vsntprintf(szString, 255, lpszFormat, args);
	va_end(args);
	server->GamePrintError(szString);
}

void D2Infof(LPCTSTR lpszFormat, ...)
{
	if (lpszFormat == NULL || lpszFormat[0] == _T('\0'))
		return;

	TCHAR szString[256] = "";
	va_list args;
	va_start(args, lpszFormat);
	_vsntprintf(szString, 255, lpszFormat, args);
	va_end(args);
	server->GamePrintInfo(szString);
}

void D2Stringf(LPCTSTR lpszFormat, ...)
{
	if (lpszFormat == NULL || lpszFormat[0] == _T('\0'))
		return;

	TCHAR szString[256] = "";
	va_list args;
	va_start(args, lpszFormat);
	_vsntprintf(szString, 255, lpszFormat, args);
	va_end(args);
	server->GamePrintString(szString);
}