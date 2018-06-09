// Diablo2CharStatsSearch.h: interface for the CDiablo2CharStatsSearch class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __DIABLO2CHARSTATSSEARCH_H__
#define __DIABLO2CHARSTATSSEARCH_H__

#include "ArrayEx.h"
#include "SharedMem.h"

typedef struct tagMemPage
{
	DWORD dwBaseAddress;
	DWORD dwSize;
} MEMPAGE, *LPMEMPAGE;
typedef const tagMemPage* LPCMEMPAGE;
typedef CArrayEx<MEMPAGE, const MEMPAGE&> CPageArray;

typedef struct tagCharStats
{
	DWORD dwFoundAddress;
	DWORD dwPageBaseAddress;
	LPCVOID pMaxHPPtr;
	LPCVOID pCurHPPtr;
	LPCVOID pMaxManaPtr;
	LPCVOID pCurManaPtr;
} CHARSTATS, *LPCHARSTATS;
typedef const tagCharStats* LPCCHARSTATS;
typedef CArrayEx<CHARSTATS, const CHARSTATS&> CStatsArray;

enum { CS_SEARCH_NOTSTARTED = 0, CS_SEARCH_SEARCHING, CS_SEARCH_SUCCEEDED, CS_SEARCH_FAILED };

class CDiablo2CharStatsSearch  
{
public:		
	
	CDiablo2CharStatsSearch();
	virtual ~CDiablo2CharStatsSearch();

	BOOL SetD2Wnd(HWND hWnd);
	void Reset();

	BOOL Search();
	void EndSearch();
	BOOL IsSearching() const;
	int GetSearchState() const;
	DWORD GetResultAddress(int nIndex) const;
	DWORD GetResultPageAddress(int nIndex) const;
	int GetFoundCount() const;
	BOOL ValidateHPMana(WORD wHP = 0, WORD wMana = 0);
	DWORD GetTimeSinceLastSearch() const;

	WORD GetCurHP() const;
	WORD GetMaxHP() const;
	WORD GetCurMana() const;
	WORD GetMaxMana() const;
	BYTE GetHPPercent() const;
	BYTE GetManaPercent() const;

protected:

	static DWORD CALLBACK ThreadProc(LPVOID lpParam);
	static BOOL FindPattern(HANDLE hProcess, DWORD dwAddr, LPCHARSTATS lpBuffer);
	static int SearchInPages(HANDLE hProcess, DWORD dwOffset, DWORD dwStep, const volatile BYTE* pStop, const CPageArray& aPages, CStatsArray& aStats);
	static BOOL _D2ReadMemory(HANDLE hProcess, LPCVOID lpBaseAddress, LPVOID lpBuffer, DWORD dwBufferSize);
	void QueryProcessEntries();
	BOOL CheckStats(const CHARSTATS& stats, WORD wHP, WORD wMana) const;

	volatile BYTE m_iStop;
	BOOL m_bSearching;
	BOOL m_bSearchedSInceReset;
	HANDLE m_hProcess;
	DWORD m_dwProcessID;
	HANDLE m_hThread;
	int m_nCurSel; 
	CPageArray m_aPages;
	CSharedMem m_mem;
	CStatsArray m_aPtrs;
	DWORD m_dwLastSearchTime;
};

#endif // __DIABLO2CHARSTATSSEARCH_H__
