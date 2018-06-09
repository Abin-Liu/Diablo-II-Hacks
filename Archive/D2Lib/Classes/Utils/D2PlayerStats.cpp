// D2PlayerStats.cpp: implementation of the CD2PlayerStats class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "D2PlayerStats.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CD2PlayerStats::CD2PlayerStats()
{
	m_hProcess = NULL;
	m_pStats = NULL;
	m_dwLastUpdate = 0;
	m_bLastResult = TRUE;
	m_hWnd = NULL;
	m_nCount = 0;
}

CD2PlayerStats::~CD2PlayerStats()
{
	ReCreateStats(0);
	if (m_hProcess != NULL)
	{
		::CloseHandle(m_hProcess);
		m_hProcess = NULL;
	}
}

void CD2PlayerStats::ReCreateStats(int nCount)
{
	if (m_pStats != NULL)
	{
		delete [] m_pStats;
		m_pStats = NULL;
	}
	m_nCount = nCount;

	if (m_nCount > 0)
	{
		m_pStats = new D2PLAYERSTATS[m_nCount];
		::memset(m_pStats, 0, sizeof(D2PLAYERSTATS) * m_nCount);
	}
}

BOOL CD2PlayerStats::SetD2Wnd(HWND hWnd)
{
	if (hWnd != m_hWnd)
	{
		ReCreateStats(0);
		if (m_hProcess != NULL)
		{
			::CloseHandle(m_hProcess);
			m_hProcess = NULL;
		}

		DWORD dwProcID = 0;
		::GetWindowThreadProcessId(hWnd, &dwProcID);
		m_hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, NULL, dwProcID);
	}
	
	if (m_hProcess == NULL)
		return FALSE;

	m_dwLastUpdate = 0;
	return UpdateStats();
}

BOOL CD2PlayerStats::UpdateStats()
{
	DWORD dwNow = ::GetTickCount();
	if (dwNow == m_dwLastUpdate)
		return m_bLastResult;

	m_dwLastUpdate = dwNow;
	m_bLastResult = ReadMemStats();
	return m_bLastResult;	
}

DWORD CD2PlayerStats::GetStats(DWORD dwStatsID)
{
	UpdateStats();
	for (int i = 0; i < m_nCount; i++)
	{
		if (m_pStats[i].dwStatsID == dwStatsID)
			return m_pStats[i].dwValue;
	}
	return 0;
}

BOOL CD2PlayerStats::ReadMemStats()
{
	// Offsets values are from Mousepad
	ReCreateStats(0);
	DWORD dwRead = 0;
	DWORD dwUnit = 0;
	if (!::ReadProcessMemory(m_hProcess, (LPCVOID)0x6FBBC200, &dwUnit, 4, &dwRead) || dwRead != 4)
		return FALSE;

	DWORD dwStats = 0;
	if (!::ReadProcessMemory(m_hProcess, (LPCVOID)(dwUnit + 0x5c), &dwStats, 4, &dwRead) || dwRead != 4)
		return FALSE;
	
	DWORD dwTemp = 0;
	if (!::ReadProcessMemory(m_hProcess, (LPCVOID)(dwStats + 0x10), &dwTemp, 4, &dwRead) || dwRead != 4)
		return FALSE;

	DWORD dwStatsOff = (dwTemp & 0x80000000) ? 0x48 : 0x24; 

	DWORD dwStats2 = 0;
	if (!::ReadProcessMemory(m_hProcess, (LPCVOID)(dwStats + dwStatsOff + 0x00), &dwStats2, 4, &dwRead) || dwRead != 4)
		return FALSE;

	WORD wCount = 0;
	if (!::ReadProcessMemory(m_hProcess, (LPCVOID)(dwStats + dwStatsOff + 0x04), &wCount, 2, &dwRead) || dwRead != 2)
		return FALSE;

	ReCreateStats((int)wCount);
	if (wCount == 0)
		return FALSE;

	if (!::ReadProcessMemory(m_hProcess, (LPCVOID)dwStats2, m_pStats, wCount * sizeof(D2PLAYERSTATS), &dwRead))
	{
		ReCreateStats(0);
		return FALSE;
	}

	for (int i = 0; i < m_nCount; i++)
	{
		m_pStats[i].dwStatsID = HIWORD(m_pStats[i].dwStatsID);
		if (m_pStats[i].dwStatsID == D2PS_HP 
			|| m_pStats[i].dwStatsID == D2PS_MAXHP
			|| m_pStats[i].dwStatsID == D2PS_MANA
			|| m_pStats[i].dwStatsID == D2PS_MAXMANA
			|| m_pStats[i].dwStatsID == D2PS_STAMINA
			|| m_pStats[i].dwStatsID == D2PS_MAXSTAMINA)
		{
			m_pStats[i].dwValue /= 256;
		}
	}

	return TRUE;
}

BOOL CD2PlayerStats::HasStats(DWORD dwStatsID)
{
	UpdateStats();
	for (int i = 0; i < m_nCount; i++)
	{
		if (m_pStats[i].dwStatsID == dwStatsID)
			return TRUE;
	}
	return FALSE;
}
