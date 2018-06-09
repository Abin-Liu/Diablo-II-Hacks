// KeyBind.cpp: implementation of the CKeyBind class.
//
//////////////////////////////////////////////////////////////////////
//#include "StdAfx.h"
#include "KeyBind.h"
#include <string.h>
#include <tchar.h>
#include <stdio.h>
#include <stdlib.h>

// by Abin: Diablo II game.exe memory offsets for chat input and menu
#define MEM_OFFSET_INPUT	0x6FBBA6BC // player is typing some chat message
//#define MEM_OFFSET_MENU		0x6FBBA6CC // player has activated the diablo ii menu

HHOOK g_hHook = NULL; // The hook handle
CKeyBindEntry g_aEntries[BIND_ENTRY_COUNT]; // 256 hotkey entries
KEYBINDPROC g_lpfnProc = NULL; // callback function pointer
BOOL g_bShowCode = FALSE; // showing key code?
HWND g_hTargetWnd = NULL; // HANDLE to the Diablo II game window
BOOL g_bStarted = FALSE;

CKeyBindEntry::CKeyBindEntry()
: m_iVKCode(0x00), m_iRetCode(0x00), m_lpszCommands(NULL)
{
}

CKeyBindEntry::CKeyBindEntry(BYTE iVKCode, BYTE iRetCode, LPCTSTR lpszCommands)
: m_iVKCode(iVKCode), m_iRetCode(iRetCode), m_lpszCommands(NULL)
{
	SetCommands(lpszCommands);
}

CKeyBindEntry::CKeyBindEntry(const CKeyBindEntry& rhs)
: m_iVKCode(rhs.m_iVKCode), m_iRetCode(rhs.m_iRetCode), m_lpszCommands(NULL)
{
	SetCommands(rhs.m_lpszCommands);	
}

CKeyBindEntry::~CKeyBindEntry()
{
	__FreeData();
}

void CKeyBindEntry::__FreeData()
{
	if (m_lpszCommands != NULL)
	{
		delete [] m_lpszCommands;
		m_lpszCommands = NULL;
	}
}

const CKeyBindEntry& CKeyBindEntry::operator=(const CKeyBindEntry& rhs)
{
	m_iVKCode = rhs.m_iVKCode;
	m_iRetCode = rhs.m_iRetCode;
	SetCommands(rhs.m_lpszCommands);
	return *this;
}

BOOL CKeyBindEntry::operator==(const CKeyBindEntry& rhs) const
{
	return m_iVKCode == rhs.m_iVKCode;
}

BOOL CKeyBindEntry::operator>(const CKeyBindEntry& rhs) const
{
	return m_iVKCode > rhs.m_iVKCode;
}

void CKeyBindEntry::SetVKCode(BYTE iVKCode)
{
	m_iVKCode = iVKCode;
}

BYTE CKeyBindEntry::GetVKCode() const
{
	return m_iVKCode;
}

void CKeyBindEntry::SetCommands(LPCTSTR lpszCommands)
{
	__FreeData();

	if (lpszCommands != NULL)
	{
		// trim leading irrelevant chars
		while (*lpszCommands == _T(';') || *lpszCommands == _T(' ') || *lpszCommands == _T('\n') || *lpszCommands == _T('\t') || *lpszCommands == _T('\r'))
			lpszCommands += sizeof(TCHAR);

		if (*lpszCommands != _T('\0'))
			m_lpszCommands = _tcsdup(lpszCommands);	
	}
}

DWORD CKeyBindEntry::GetCommandStartPosition() const
{
	return (DWORD)m_lpszCommands;
}

void CKeyBindEntry::GetNextCommand(DWORD& dwPosition, LPTSTR lpszBuffer, DWORD dwChars) const
{
	if (lpszBuffer != NULL)
		*lpszBuffer = _T('\0');

	if (dwPosition == 0)
		return;

	DWORD dwLen = 0;
	LPCTSTR p = (LPCTSTR)dwPosition;

	while (*p != _T(';') && *p != _T('\0'))
	{
		p += sizeof(TCHAR);
		dwLen++;
	}

	DWORD dwCopy = min(dwLen, dwChars);
	if (dwCopy > 0)
		_tcsncpy(lpszBuffer, (LPCTSTR)dwPosition, dwCopy);

	if (*p == _T(';'))
		p += sizeof(TCHAR);

	dwPosition = *p == _T('\0') ? 0 : (DWORD)p;
}

BOOL CKeyBindEntry::IsMapped() const
{
	return m_iVKCode > 0 && (m_iRetCode != m_iVKCode || m_lpszCommands != NULL);
}

void CKeyBindEntry::SetRetCode(BYTE iRetCode)
{
	m_iRetCode = iRetCode;
}

BYTE CKeyBindEntry::GetRetCode() const
{
	return m_iRetCode;
}

//////////////////////////////////////////////////////////////////////
// CKeyBind Implementation
//////////////////////////////////////////////////////////////////////
BOOL CKeyBind::InstallKeyBind()
{
	ClearAll();
	UninstallKeyBind();
	g_hHook = ::SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, 0, ::GetCurrentThreadId());
	return g_hHook != NULL;
}

BOOL CKeyBind::UninstallKeyBind()
{		
	BOOL bOK = ::UnhookWindowsHookEx(g_hHook);
	g_hHook = NULL;
	return bOK;
}

LRESULT CKeyBind::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (!g_bStarted
		|| nCode != HC_ACTION  // not our message
		|| g_lpfnProc == NULL // callback function not specified
		|| (lParam & 0x80000000) // key up
		|| (lParam & 0x40000000) // key repeat
		|| g_aEntries[(BYTE)wParam].GetVKCode() != (BYTE)wParam // code mismatch
		|| IsTyping()) // player is chatting
	{
		return ::CallNextHookEx(g_hHook, nCode, wParam, lParam);
	}

	if (g_bShowCode)
	{
		g_lpfnProc((BYTE)wParam, NULL);
		return 1;
	}

	BYTE iIndex = (BYTE)wParam;
	DWORD dwPos = g_aEntries[iIndex].GetCommandStartPosition();
	while (dwPos)
	{
		TCHAR szCommand[4097] = _T("");
		g_aEntries[iIndex].GetNextCommand(dwPos, szCommand, 4096);
		if (!g_lpfnProc(iIndex, szCommand))
			break; // abort if callback function returns FALSE
	}

	// steal the message
	if (g_aEntries[iIndex].GetRetCode() != (BYTE)wParam)
	{
		if (g_aEntries[iIndex].GetRetCode() != 0 && g_hTargetWnd != NULL)
			::PostMessage(g_hTargetWnd, WM_KEYDOWN, (WPARAM)g_aEntries[iIndex].GetRetCode(), lParam);

		return 1;
	}
	
	return ::CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

BOOL CKeyBind::SetEntry(BYTE iVKCode, BYTE iRetCode, LPCTSTR lpszCommand)
{
	g_aEntries[iVKCode].SetVKCode(iVKCode);
	g_aEntries[iVKCode].SetRetCode(iRetCode);
	g_aEntries[iVKCode].SetCommands(lpszCommand);
	return TRUE;
}

void CKeyBind::SetKeyBindProc(KEYBINDPROC lpfnProc)
{
	g_lpfnProc = lpfnProc;
}

void CKeyBind::ClearAll()
{
	for (int i = 0; i < BIND_ENTRY_COUNT; i++)
	{
		g_aEntries[i].SetVKCode((BYTE)i);
		g_aEntries[i].SetRetCode((BYTE)i);
		g_aEntries[i].SetCommands(NULL);
	}
}

CKeyBindEntry* CKeyBind::GetEntry(BYTE iIndex)
{
	return &g_aEntries[iIndex];
}

void CKeyBind::SetShowCode(BOOL bSet)
{
	g_bShowCode = bSet;
}

BOOL CKeyBind::IsTyping()
{
	DWORD dwProcID = ::GetCurrentProcessId();
	HANDLE hProcess = ::OpenProcess(PROCESS_VM_READ, NULL, dwProcID);
	if (hProcess == NULL)
		return FALSE;

	BOOL bRes = FALSE;
	DWORD dwVal = 0, dwSize = 0;
	if (::ReadProcessMemory(hProcess, (LPCVOID)MEM_OFFSET_INPUT, &dwVal, 4, &dwSize) && dwSize == 4)
		bRes = dwVal != 0;

	::CloseHandle(hProcess);
	return bRes;
}

BOOL CKeyBind::Clear(BYTE iIndex)
{
	SetEntry(iIndex, iIndex, NULL);
	return TRUE;
}

BOOL CKeyBindEntry::GetEntryNameStr(BYTE iVKCode, LPTSTR lpszBuffer)
{
	if (lpszBuffer == NULL)
		return FALSE;

	_stprintf(lpszBuffer, _T("Keycode%02X"), iVKCode);
	return TRUE;
}

BOOL CKeyBindEntry::GetLineStr(BYTE iRetCode, LPCTSTR lpszCommands, LPTSTR lpszBuffer)
{
	if (lpszBuffer == NULL)
		return FALSE;	

	if (lpszCommands != NULL)
		_stprintf(lpszBuffer, _T("%02X0;%s"), iRetCode, lpszCommands);
	else
		_stprintf(lpszBuffer, _T("%02X0"), iRetCode);	

	return TRUE;
}

BOOL CKeyBindEntry::GetEntryNameStr(LPTSTR lpszBuffer) const
{
	return GetEntryNameStr(m_iVKCode, lpszBuffer);
}

BOOL CKeyBindEntry::GetLineStr(LPTSTR lpszBuffer) const
{
	return GetLineStr(m_iRetCode, m_lpszCommands, lpszBuffer);
}

BOOL CKeyBindEntry::ParseLineStr(LPCTSTR lpszLineStr)
{
	if (lpszLineStr == NULL)
		return FALSE;

	TCHAR szRetCode[4] = _T("");
	const int LEN = _tcslen(lpszLineStr);
	_tcsncpy(szRetCode, lpszLineStr, 2);

	BYTE iRetCode = 0;
	if (!VerifyVKCode(szRetCode, 2, &iRetCode))
		return FALSE;

	m_iRetCode = iRetCode;
	SetCommands(LEN < 5 ? NULL : &lpszLineStr[4]);
	return TRUE;
}

BOOL CKeyBindEntry::VerifyVKCode(LPCTSTR lpsz, DWORD dwLen, BYTE *pVKCode)
{
	if (pVKCode)
		*pVKCode = 0x00;

	if (lpsz == NULL)
		return FALSE;

	if (dwLen > 0 && _tcslen(lpsz) != dwLen)
		return FALSE;

	char* x = "";
	DWORD dwVKCode = _tcstoul(lpsz, &x, 16);
	if (_tcslen(x))
		return FALSE;

	if (pVKCode)
		*pVKCode = (BYTE)dwVKCode;
	return TRUE;
}

BOOL CKeyBind::IsAllCleared()
{
	for (int i = 0; i < BIND_ENTRY_COUNT; i++)
	{
		if (g_aEntries[i].IsMapped())
			return FALSE;
	}
	return TRUE;
}

BOOL CKeyBindEntry::GetDisplayStr(LPTSTR lpBuffer) const
{
	if (lpBuffer == NULL)
		return FALSE;
	_stprintf(lpBuffer, _T("Keycode ÿc2%02X %02X ÿc7%s"), m_iVKCode, m_iRetCode, m_lpszCommands);
	return TRUE;
}

void CKeyBind::SetTargetWnd(HWND hWnd)
{
	g_hTargetWnd = hWnd;
}

void CKeyBind::Start()
{
	g_bStarted = TRUE;
}

void CKeyBind::Stop()
{
	g_bStarted = FALSE;
}

BOOL CKeyBind::IsStarted()
{
	return g_bStarted;
}

void CKeyBind::OnGameJoin()
{
	Start();
}

void CKeyBind::OnGameLeave()
{
	Stop();
}
