#ifndef __D2STDIO_H__
#define __D2STDIO_H__

#include <windows.h>

void D2Errorf(LPCTSTR lpszFormat, ...); // server->GamePrintError
void D2Infof(LPCTSTR lpszFormat, ...); // server->GamePrintInfo
void D2Stringf(LPCTSTR lpszFormat, ...); // server->GamePrintString

#endif