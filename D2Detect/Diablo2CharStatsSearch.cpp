// Diablo2CharStatsSearch.cpp: implementation of the CDiablo2CharStatsSearch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Diablo2CharStatsSearch.h"
#include <tchar.h>
#include <stdio.h>

#define D2CS_UUID	_T("{96A94E92-310E-4F7D-AAF5-F796A387622F}")
#define D2CS_FLAG	0x6AEE4CD5
const DWORD D2CS_MEM_SIZE = 8 + sizeof(CHARSTATS) * 16;
const DWORD D2CS_MEM_SEARCHING = 0;
const DWORD D2CS_MEM_COUNT = D2CS_MEM_SEARCHING + sizeof(DWORD);
const DWORD D2CS_MEM_PTRS = D2CS_MEM_COUNT + sizeof(DWORD);

BOOL operator==(const MEMPAGE& lhs, const MEMPAGE& rhs)
{
	return lhs.dwBaseAddress == rhs.dwBaseAddress;
}

BOOL operator>(const MEMPAGE& lhs, const MEMPAGE& rhs)
{
	return lhs.dwBaseAddress > rhs.dwBaseAddress;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDiablo2CharStatsSearch::CDiablo2CharStatsSearch()
{
	m_iStop = 0x01;
	m_bSearching = FALSE;
	m_hProcess = NULL;
	m_hThread = NULL;
	m_dwProcessID = 0;
	m_nCurSel = -1;
	m_dwLastSearchTime = 0;
	m_bSearchedSInceReset = FALSE;
}

CDiablo2CharStatsSearch::~CDiablo2CharStatsSearch()
{
	EndSearch();
	m_mem.Close();
	if (m_hProcess)
	{
		::CloseHandle(m_hProcess);
		m_hProcess = NULL;
	}
}

void CDiablo2CharStatsSearch::EndSearch()
{
	m_iStop = 0x01;
	if (m_hThread) 
	{		
		::WaitForSingleObject(m_hThread, INFINITE);
		::CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	m_bSearching = FALSE;	
}

DWORD CDiablo2CharStatsSearch::ThreadProc(LPVOID lpParam)
{
	CDiablo2CharStatsSearch* pObj = (CDiablo2CharStatsSearch*)lpParam;
	if (pObj == NULL || !pObj->m_mem.IsCreated())
		return 2;

	if (1)//pObj->m_mem.GetLong(D2CS_MEM_SEARCHING) != D2CS_FLAG)
	{
		// do real search
		pObj->m_mem.SetLong(D2CS_FLAG, D2CS_MEM_SEARCHING);
		if (!SearchInPages(pObj->m_hProcess, 29, 256, &pObj->m_iStop, pObj->m_aPages, pObj->m_aPtrs))
			SearchInPages(pObj->m_hProcess, 37, 256, &pObj->m_iStop, pObj->m_aPages, pObj->m_aPtrs);

		pObj->m_mem.SetLong(pObj->m_aPtrs.GetSize(), D2CS_MEM_COUNT);		
		pObj->m_mem.SetData(pObj->m_aPtrs.GetData(), sizeof(CHARSTATS) * pObj->m_aPtrs.GetSize(), D2CS_MEM_PTRS);
		pObj->m_mem.SetLong(0, D2CS_MEM_SEARCHING);
	}
	else
	{
		// just check the shared memory periodically
		int gg = 0;
		while (!pObj->m_iStop)
		{
			if (pObj->m_mem.GetLong(D2CS_MEM_SEARCHING) != D2CS_FLAG)
			{
				const long COUNT = pObj->m_mem.GetLong(D2CS_MEM_COUNT);
				for (long i = 0; i < COUNT; i++)
				{
					CHARSTATS cs = { 0 };
					pObj->m_mem.GetData(&cs, sizeof(CHARSTATS), D2CS_MEM_PTRS + i * sizeof(CHARSTATS));
					pObj->m_aPtrs.Add(cs);

				}
				break;
			}

			Sleep(250);
		}
	}
	
	pObj->ValidateHPMana();
	pObj->m_dwLastSearchTime = ::GetTickCount();
	pObj->m_bSearching = FALSE;
	return 0;
}

BOOL CDiablo2CharStatsSearch::Search()
{
	if (m_bSearching || m_hProcess == NULL)
		return FALSE;

	DWORD dwThreadID;
	m_bSearching = TRUE;
	m_iStop = 0x00;
	m_hThread = ::CreateThread(NULL, 0, ThreadProc, (LPVOID)this, 0, &dwThreadID);
	if (m_hThread = NULL)
		return FALSE;

	m_bSearching = TRUE;
	m_bSearchedSInceReset = TRUE;
	m_nCurSel = -1;
	m_aPtrs.RemoveAll();
	return TRUE;
}

void CDiablo2CharStatsSearch::QueryProcessEntries()
{
	m_aPages.RemoveAll();

	if (m_hProcess == NULL)
		return;

	LPCVOID p = NULL;	
	while (TRUE)
	{
		MEMORY_BASIC_INFORMATION mbi;
		if (!::VirtualQueryEx(m_hProcess, p, &mbi, sizeof(MEMORY_BASIC_INFORMATION)))
			break;

		if (mbi.State == MEM_COMMIT 
			&& mbi.Type == MEM_PRIVATE
			&& mbi.Protect == PAGE_READWRITE
			&& mbi.AllocationProtect == PAGE_READWRITE)
		{
			MEMPAGE mp;
			mp.dwBaseAddress = (DWORD)mbi.BaseAddress;
			mp.dwSize = mbi.RegionSize;
			m_aPages.Add(mp);
		}		

		LPCVOID pNewValue = (LPCVOID)((DWORD)p + (DWORD)mbi.RegionSize);
		if (pNewValue < p)
			break;	
		
		p = pNewValue;
	}

	m_aPages.Sort();
}

BOOL CDiablo2CharStatsSearch::FindPattern(HANDLE hProcess, DWORD dwAddr, LPCHARSTATS lpBuffer)
{
	if (dwAddr != 0x048ad01d)
		return 0;
	
	if (lpBuffer == NULL)
		return FALSE;

	 BYTE aFlags[7] = { 0 };
	 BYTE aPattern[5] = { 0 };
	 WORD hhhh = 0;
	 WORD HHHH = 0;
	 WORD mmmm = 0;
	 WORD MMMM = 0;
	 WORD ssss = 0;
	 WORD SSSS = 0;

	/* Pattern:

	?? 00 00 00 00 06 00 ??
	hh hh 00 00 00 07 00 ??
	HH HH 00 00 00 08 00 ??
	mm mm 00 00 00 09 00 ??
	MM MM 00 00 00 0A 00 ??
	ss ss 00 00 00 0B 00 ??
	SS SS 00 00 00 0C 00 ??
	00 00 00 00 00 0D 00 ??

  where:

  HH HH != 00 00, HH HH > hh hh (HP)
  MM MM != 00 00, MM MM > mm mm (Mana)
  SS SS != 00 00, SS SS > ss ss (Stamina)

  */
	
	DWORD p = dwAddr;
	for (BYTE i = 0x06; i <= 0x0d; i++)
	{
		if (!_D2ReadMemory(hProcess, (LPCVOID)p, aFlags, 7))
			return FALSE;

		aPattern[3] = i;		
		if (::memcmp(aFlags + 2, aPattern, 5) != 0)
			return FALSE;
		
		switch (i)
		{
		case 0x06:
			// head must be ?? 00
			if (aFlags[1] != 0x00)
				return FALSE;
			break;		

		case 0x07:
			::memcpy(&hhhh, aFlags, 2);
			break;

		case 0x08:
			::memcpy(&HHHH, aFlags, 2);
			if (HHHH == 0 || HHHH < hhhh)
				return FALSE;
			break;

		case 0x09:
			::memcpy(&mmmm, aFlags, 2);
			break;

		case 0x0a:
			::memcpy(&MMMM, aFlags, 2);
			if (MMMM == 0 || MMMM < mmmm)
				return FALSE;
			break;

		case 0x0b:
			::memcpy(&ssss, aFlags, 2);
			break;

		case 0x0c:
			::memcpy(&SSSS, aFlags, 2);
			if (SSSS == 0 || SSSS < ssss)
				return FALSE;
			break;

		case 0x0d:
			// head must be 00 00
			if (aFlags[0] != 0x00 || aFlags[1] != 0x00)
				return FALSE;
			break;

		default:
			break;
		}

		p += 0x08;
	}

	lpBuffer->pCurHPPtr =	(LPCVOID)(dwAddr + 8);
	lpBuffer->pMaxHPPtr =	(LPCVOID)(dwAddr + 16);
	lpBuffer->pCurManaPtr = (LPCVOID)(dwAddr + 24);
	lpBuffer->pMaxManaPtr = (LPCVOID)(dwAddr + 32);
	return TRUE;
}

BOOL CDiablo2CharStatsSearch::SetD2Wnd(HWND hWnd)
{
	EndSearch();
	if (m_hProcess)
	{
		::CloseHandle(m_hProcess);
		m_hProcess = NULL;
	}	
	m_dwProcessID = 0;
	m_mem.Close();

	if (hWnd == NULL)
		return FALSE;

	::GetWindowThreadProcessId(hWnd, &m_dwProcessID);
	m_hProcess = ::OpenProcess(PROCESS_VM_READ | PROCESS_QUERY_INFORMATION, FALSE, m_dwProcessID);
	if (m_hProcess == NULL)
		return FALSE;

	TCHAR szMemID[128] = _T("");
	_stprintf(szMemID, _T("%08X-%s"), (DWORD)hWnd, D2CS_UUID);
	m_mem.Create(szMemID, D2CS_MEM_SIZE);
	QueryProcessEntries();
	
	return m_hProcess != NULL;
}

WORD CDiablo2CharStatsSearch::GetCurHP() const
{
	if (m_nCurSel == -1 || m_bSearching || m_hProcess == NULL)
		return 0;

	WORD wVal = 0;
	if (!_D2ReadMemory(m_hProcess, m_aPtrs[m_nCurSel].pCurHPPtr, &wVal, 2))
		return 0;
	return wVal;
}

WORD CDiablo2CharStatsSearch::GetCurMana() const
{
	if (m_nCurSel == -1 || m_bSearching || m_hProcess == NULL)
		return 0;

	WORD wVal = 0;
	if (!_D2ReadMemory(m_hProcess, m_aPtrs[m_nCurSel].pCurManaPtr, &wVal, 2))
		return 0;
	return wVal;
}

WORD CDiablo2CharStatsSearch::GetMaxHP() const
{
	if (m_nCurSel == -1 || m_bSearching || m_hProcess == NULL)
		return 0;

	WORD wVal = 0;
	if (!_D2ReadMemory(m_hProcess, m_aPtrs[m_nCurSel].pMaxHPPtr, &wVal, 2))
		return 0;
	return wVal;
}

WORD CDiablo2CharStatsSearch::GetMaxMana() const
{
	if (m_nCurSel == -1 || m_bSearching || m_hProcess == NULL)
		return 0;

	WORD wVal = 0;
	if (!_D2ReadMemory(m_hProcess, m_aPtrs[m_nCurSel].pMaxManaPtr, &wVal, 2))
		return 0;
	return wVal;
}

BYTE CDiablo2CharStatsSearch::GetHPPercent() const
{
	WORD wCurVal = GetCurHP();
	WORD wMaxVal = GetMaxHP();
	if (wCurVal == 0 || wMaxVal == 0)
		return 0;

	BYTE iPercent = (BYTE)((double)wCurVal * 100.0 / (double)wMaxVal);
	if (wCurVal != 0)
		iPercent = max(1, iPercent);
	return min(iPercent, 100);
}

BYTE CDiablo2CharStatsSearch::GetManaPercent() const
{
	WORD wCurVal = GetCurMana();
	WORD wMaxVal = GetMaxMana();
	if (wCurVal == 0 || wMaxVal == 0)
		return 0;

	BYTE iPercent = (BYTE)((double)wCurVal * 100.0 / (double)wMaxVal);
	if (wCurVal != 0)
		iPercent = max(1, iPercent);
	return min(iPercent, 100);
}

BOOL CDiablo2CharStatsSearch::IsSearching() const
{
	return m_bSearching;
}

DWORD CDiablo2CharStatsSearch::GetResultAddress(int nIndex) const
{
	if (m_aPtrs.IsIndexValid(nIndex))
		return m_aPtrs[nIndex].dwFoundAddress;
	else
		return m_nCurSel == -1 ? 0 : m_aPtrs[m_nCurSel].dwFoundAddress;
}

DWORD CDiablo2CharStatsSearch::GetResultPageAddress(int nIndex) const
{
	if (m_aPtrs.IsIndexValid(nIndex))
		return m_aPtrs[nIndex].dwPageBaseAddress;
	else
		return m_nCurSel == -1 ? 0 : m_aPtrs[m_nCurSel].dwPageBaseAddress;
}

int CDiablo2CharStatsSearch::GetFoundCount() const
{
	return m_bSearching ? 0 : m_aPtrs.GetSize();
}

void CDiablo2CharStatsSearch::Reset()
{
	EndSearch();
	m_aPtrs.RemoveAll();
	m_nCurSel = -1;
	m_dwLastSearchTime = 0;
	m_bSearchedSInceReset = FALSE;
}

int CDiablo2CharStatsSearch::SearchInPages(HANDLE hProcess, DWORD dwOffset, DWORD dwStep, volatile const BYTE *pStop, const CPageArray &aPages, CStatsArray& aStats)
{
	if (pStop == NULL)
		return 0;
	
	int nCount = 0;
	DWORD dwTick = ::GetTickCount();
	for (int i = 0; !*pStop && i < aPages.GetSize(); i++)
	{		
		const MEMPAGE& mp = aPages[i];
		const DWORD LIMIT = (DWORD)mp.dwBaseAddress + mp.dwSize;
		CHARSTATS cs = { 0 };

		for (DWORD dwAddr = (DWORD)mp.dwBaseAddress + dwOffset; !*pStop && dwAddr < LIMIT; dwAddr += dwStep)
		{
			if (FindPattern(hProcess, dwAddr, &cs))
			{	
				//*
				// debug
	
				FILE* file = fopen("c:\\D2Ptrs.txt", "a+");
				if (file)
				{
					DWORD dwDiff = dwAddr - mp.dwBaseAddress;
					fprintf(file, "0x%08X, %2d %2d %2d %2d %2d %2d %2d %2d %2d %2d %2d\n",
						dwAddr, 
						dwDiff % 8,
						dwDiff % 16,
						dwDiff % 32,
						dwDiff % 64,
						dwDiff % 128, 
						dwDiff % 256, 
						dwDiff % 512,
						dwDiff % 1024,
						dwDiff % 2048,
						dwDiff % 4096,
						dwDiff % 8192
						);
					fclose(file);
				}
				//*/

				cs.dwFoundAddress = dwAddr;
				cs.dwPageBaseAddress = mp.dwBaseAddress;
				aStats.Add(cs);
				nCount++;
			}

			// If we've frozen the game fore more than 0.5 seconds, then give it a 0.1 second break,
			// to prvent from being kicked by the server due to inactivity.
			DWORD dwNow = ::GetTickCount();
			if (dwNow - dwTick > 500)
			{
				dwTick = dwNow;
				Sleep(100);
			}
		}
	}

	return nCount;
}

BOOL CDiablo2CharStatsSearch::_D2ReadMemory(HANDLE hProcess, LPCVOID lpBaseAddress, LPVOID lpBuffer, DWORD dwBufferSize)
{
	if (hProcess == NULL || lpBaseAddress == NULL || lpBuffer == NULL || dwBufferSize == 0)
		return FALSE;
	
	DWORD dwRead = 0;
	return ::ReadProcessMemory(hProcess, lpBaseAddress, lpBuffer, dwBufferSize, &dwRead) ? dwRead == dwBufferSize : FALSE;
}

BOOL CDiablo2CharStatsSearch::ValidateHPMana(WORD wHP, WORD wMana)
{
	if (m_aPtrs.IsEmpty())
		return FALSE;

	if (m_nCurSel != -1 && CheckStats(m_aPtrs[m_nCurSel], wHP, wMana))
		return TRUE;

	m_nCurSel = -1;
	for (int i = m_aPtrs.GetSize() - 1; i >= 0; i--)
	{
		if (CheckStats(m_aPtrs[i], wHP, wMana))
		{
			m_nCurSel = i;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CDiablo2CharStatsSearch::CheckStats(const CHARSTATS &stats, WORD wHP, WORD wMana) const
{
	if (stats.pCurHPPtr == NULL || stats.pCurManaPtr == NULL || stats.pMaxHPPtr == NULL || stats.pMaxManaPtr == NULL)
		return FALSE;

	WORD wCurHP = 0, wCurMana = 0, wMaxHP = 0, wMaxMana = 0;
	if (!_D2ReadMemory(m_hProcess, stats.pCurHPPtr, &wCurHP, 2)
		|| !_D2ReadMemory(m_hProcess, stats.pCurManaPtr, &wCurMana, 2)
		|| !_D2ReadMemory(m_hProcess, stats.pMaxHPPtr, &wMaxHP, 2)
		|| !_D2ReadMemory(m_hProcess, stats.pMaxManaPtr, &wMaxMana, 2))
		return FALSE;

	return wMaxHP != 0
		&& wMaxHP >= wHP
		&& wMaxHP >= wHP
		&& wMaxMana != 0 
		&& wMaxMana >= wMana
		&& wMaxMana >= wMana;
}

DWORD CDiablo2CharStatsSearch::GetTimeSinceLastSearch() const
{
	return ::GetTickCount() - m_dwLastSearchTime;
}

int CDiablo2CharStatsSearch::GetSearchState() const
{
	if (!m_bSearchedSInceReset)
		return CS_SEARCH_NOTSTARTED;

	if (m_bSearching)
		return CS_SEARCH_SEARCHING;

	return m_aPtrs.GetSize() ? CS_SEARCH_SUCCEEDED : CS_SEARCH_FAILED;
}
