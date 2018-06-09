// D2Abstract.cpp: implementation of the CD2Abstract class.
//
//////////////////////////////////////////////////////////////////////

#include "D2Abstract.h"
#include <stdio.h>

#define MAX_DEBUG_BUF	1024

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CD2Abstract::CD2Abstract()
{
	m_bLoadedBeforeJoin = FALSE;
	m_bDebug = FALSE;
	m_lpfnMessageProc = NULL;
	m_lpMessageData = NULL;
	::memset(&m_thisPlayer, 0, sizeof(D2PLAYERDATA));
	m_bGameJoined = FALSE;
}

CD2Abstract::~CD2Abstract()
{
}

void CD2Abstract::SetD2MessageProc(D2MESSAGEPROC lpfn, LPVOID lpData)
{
	m_lpfnMessageProc = lpfn;
	m_lpMessageData = lpData;
}

BOOL CD2Abstract::DebugMessage(LPCTSTR lpszDebug) const
{
	if (!IsGameReady() || !m_bDebug || lpszDebug == NULL || lpszDebug[0] == _T('\0'))
		return FALSE;

	return server->GamePrintInfo(lpszDebug);
}

BOOL CD2Abstract::FormatAndDebugMessage(LPCTSTR lpszFormat, ...) const
{
	if (!IsGameReady() || !m_bDebug || lpszFormat == NULL || lpszFormat[0] == _T('\0'))
		return FALSE;

	TCHAR szMessage[MAX_DEBUG_BUF + 1] = _T("");
	va_list args;
	va_start(args, lpszFormat);
	_vsntprintf(szMessage, MAX_DEBUG_BUF, lpszFormat, args);
	va_end(args);
	return DebugMessage(szMessage);	
}

void CD2Abstract::OnGameJoin(const THISGAMESTRUCT* pGame)
{
	// If loaded before join, player should already have an ID before "OnGameJoin" is called
	m_bLoadedBeforeJoin = m_thisPlayer.dwPlayerID != 0;
	
	if (pGame != NULL)
	{
		m_thisPlayer.dwPlayerID = pGame->player->PlayerID;
		m_thisPlayer.iCharClass = (BYTE)pGame->player->CharacterClass;
		_tcsncpy(m_thisPlayer.szPlayerName, pGame->player->PlayerName, 16);
		m_thisPlayer.pPlayerXAddr = &pGame->player->PlayerPositionX;
		m_thisPlayer.pPlayerYAddr = &pGame->player->PlayerPositionY;
		m_thisPlayer.pPlayerLocationAddr = &pGame->player->PlayerLocation;
		m_thisPlayer.pPlayerActAddr = &pGame->player->Act;
	}

	m_bGameJoined = TRUE;
}

void CD2Abstract::OnGameLeave()
{
	m_bGameJoined = FALSE;
	m_bLoadedBeforeJoin = FALSE;
	::memset(&m_thisPlayer, 0, sizeof(D2PLAYERDATA));
}

void CD2Abstract::OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen)
{
	if (aPacket == NULL || aLen == 0)
		return;	

	// This will let derived classes have some player data to use before "OnGameJoin" is called.
	if (m_thisPlayer.dwPlayerID == 0
		&& aLen == 26
		&& aPacket[0] == 0x59)
	{
		::memcpy(&m_thisPlayer.dwPlayerID, aPacket + 1, 4);
		m_thisPlayer.iCharClass = aPacket[5];
		::memcpy(m_thisPlayer.szPlayerName, aPacket + 6, 17);
		return;
	}
}

void CD2Abstract::OnGamePacketBeforeSent(const BYTE *aPacket, DWORD aLen)
{
	aPacket = aPacket;
	aLen = aLen;
}

void CD2Abstract::OnGameTimerTick()
{
}

void CD2Abstract::InitAttributes()
{
}

BOOL CD2Abstract::SendPacketToServer(const BYTE *aPacket, DWORD aLen, int nTimes) const
{
	if (aPacket == NULL || aLen == 0 || server == NULL)
		return FALSE;

	for (int i = 0; i < nTimes; i++)
	{
		if (!server->GameSendPacketToServer((BYTE*)aPacket, aLen))
			return FALSE;
	}

	return TRUE;
}

BOOL CD2Abstract::SendPacketToGame(const BYTE *aPacket, DWORD aLen, int nTimes) const
{
	if (aPacket == NULL || aLen == 0 || server == NULL)
		return FALSE;

	for (int i = 0; i < nTimes; i++)
	{
		if (!server->GameSendPacketToGame((BYTE*)aPacket, aLen))
			return FALSE;
	}

	return TRUE;
}

BOOL CD2Abstract::NotifyD2Message(UINT nMessage, WPARAM wParam, LPARAM lParam) const
{
	if (m_lpfnMessageProc == NULL)
	{
		//server->GamePrintError(_T("CD2Abstract Error: m_lpfnMessageProc is NULL!"));
		return FALSE;
	}
	
	(*m_lpfnMessageProc)(nMessage, wParam, lParam, m_lpMessageData);
	return TRUE;
}

void CD2Abstract::SetDebug(BOOL bSet)
{
	m_bDebug = bSet;
}

BOOL CD2Abstract::IsGameReady() const
{
	return m_bGameJoined
		&& m_thisPlayer.dwPlayerID
		&& m_thisPlayer.pPlayerXAddr
		&& *m_thisPlayer.pPlayerXAddr
		&& m_thisPlayer.pPlayerYAddr
		&& *m_thisPlayer.pPlayerYAddr
		&& m_thisPlayer.szPlayerName
		&& m_thisPlayer.szPlayerName[0] != _T('\0');
}

BOOL CD2Abstract::IsLoadedBeforeJoin() const
{
	return m_bLoadedBeforeJoin;
}

BOOL CD2Abstract::DisplayNotLoadedBeforeJoinWarning(LPCTSTR lpszModuleName) const
{
	if (!m_bLoadedBeforeJoin)
	{
		TCHAR szModule[256] = _T("ÿc4D2Lib was not loaded before joining game!");
		if (lpszModuleName != NULL && lpszModuleName[0] != _T('\0'))
			_sntprintf(szModule, 255, _T("Module ÿc4%sÿc0 was not loaded before joining game!"), lpszModuleName);

		server->GamePrintError(_T("ÿc4-------------------------------------------------------------"));
		server->GamePrintError(_T("ÿc4         D2Lib Warning"));
		server->GamePrintError(_T(" "));
		server->GamePrintError(szModule);
		server->GamePrintError(_T("As a result, some features may be unavailable"));
		server->GamePrintError(_T("until you enter a new game."));
		server->GamePrintError(_T("ÿc4-------------------------------------------------------------"));
	}

	return !m_bLoadedBeforeJoin;
}

BOOL CD2Abstract::IsDebug() const
{
	return m_bDebug;
}

BYTE CD2Abstract::GetCurrentAct() const
{
	return m_thisPlayer.pPlayerActAddr ? *m_thisPlayer.pPlayerActAddr : 0;
}

DWORD CD2Abstract::FormatPacket(BOOL bReceived, const BYTE *aPacket, DWORD aLen, LPTSTR lpszBuffer, DWORD dwMaxChars, BOOL bUseColor)
{
	if (lpszBuffer == NULL)
		return 0;

	lpszBuffer[0] = _T('\0');
	if (aPacket == NULL || aLen == 0)
		return 0;

	TCHAR sz[64] = _T("");

	if (bUseColor)
	{
		if (bReceived)
			_stprintf(sz, _T("ÿc4R ÿc2%3dÿc4:ÿc0"), aLen);
		else
			_stprintf(sz, _T("ÿc4S ÿc2%3dÿc4:ÿc0"), aLen);
	}
	else
	{
		if (bReceived)
			_stprintf(sz, _T("R %3d:"), aLen);
		else
			_stprintf(sz, _T("S %3d:"), aLen);
	}
	

	DWORD dwCopied = _tcslen(sz);
	if (dwCopied > dwMaxChars)
		return 0;

	_tcscat(lpszBuffer, sz);

	for (DWORD i = 0; i < aLen && dwCopied + 3 <= dwMaxChars; i++)
	{
		_stprintf(sz, _T("  %02X"), aPacket[i]);
		_tcscat(lpszBuffer, sz);
		dwCopied += 3;
	}

	return dwCopied;
}

BOOL CD2Abstract::PrintPacket(BOOL bReceived, const BYTE *aPacket, DWORD aLen) const
{
	TCHAR szBuffer[1024] = _T("");
	if (!FormatPacket(bReceived, aPacket, aLen, szBuffer, 1023, TRUE))
		return FALSE;

	return server->GamePrintInfo(szBuffer);
}

BOOL CD2Abstract::IsGameJoined() const
{
	return m_bGameJoined;
}
