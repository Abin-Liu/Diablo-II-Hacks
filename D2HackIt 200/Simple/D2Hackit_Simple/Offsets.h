#ifndef __OFFSETS_H__
#define __OFFSETS_H__

#include <windows.h>

DWORD GetDllOffset(char *dll, int offset);
DWORD GetDllOffset(int num);
void DefineOffsets();

#endif // __OFFSETS_H__