///////////////////////////////////////////////////////////
// NpcSession.h
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////

#ifndef __NPCSESSION_H__
#define __NPCSESSION_H__

#include <windows.h>

BOOL IsVendor(DWORD dwClassID);
BOOL IsGambler(DWORD dwClassID);
BOOL IsResurrector(DWORD dwClassID);
DWORD GetCurrentNpcID();
BOOL IsCurrentNpcResurrector();
BOOL IsNpcActive(int nInteractType = 0);
BOOL IsNpcBullShitting();

void CleanNpcSessionStates();
BOOL ProcessNpcSession(const volatile BYTE* lpStopFlag);

void AnalyzeVendorItem(DWORD dwItemID, LPCSTR lpszItemCode);
void OnVendorItemPacket(const BYTE* aPacket, DWORD aLen);
void OnNpcClosed(DWORD dwNpcID);

#endif // __NPCSESSION_H__