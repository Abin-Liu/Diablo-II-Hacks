//////////////////////////////////////////////////////////////////////
// GetitConfig.h
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __GETITCONFIG_H__
#define __GETITCONFIG_H__

#include "..\\includes\\D2Hackit.h"
#include "..\\includes\\D2Client.h"
#include "..\\includes\\utilities\\ini.h"
#include "..\\includes\\utilities\\StringEx.h"
#include "..\\includes\\utilities\\ArrayEx.h"
#include "..\\includes\\utilities\\LinkedList.h"

class CGetitConfig  
{
public:	

	CGetitConfig();
	~CGetitConfig();
	
	BOOL Load(const CIni* pIni, LPCSTR lpszSection);
	BOOL Exam(DWORD dwItemID, BOOL bPickorKeep, BOOL* pTeleport = NULL);
	BOOL IsValid() const;
	LPCSTR GetSectionName() const;

private:

	static void RemoveComments(LPSTR lpsz);
	static BOOL CALLBACK EnumPickProc(LPCTSTR lpszKey, LPCTSTR lpszValue, LPVOID lpParam);
	static BOOL CALLBACK EnumKeepProc(LPCTSTR lpszKey, LPCTSTR lpszValue, LPVOID lpParam);

	CStringEx m_sSection; // ini section name
	CStringEx m_sCodes; // code
	CStringEx m_sPick; // common + pick + ... + pick
	CStringEx m_sKeep; // common + keep + ... + keep
	BOOL m_bTeleport; // teleport
	BOOL m_bValid; // valid expression
};

class CGetItConfigLib
{
public:		

	CGetItConfigLib();
	~CGetItConfigLib();

	BOOL Load(LPCSTR lpszIniFile);
	BOOL Lookup(DWORD dwItemID, BOOL bPickOrKeep, BOOL* pTeleport = NULL);
	BOOL WriteLog(LPCSTR lpszString) const;
	int InvokeStashCallback() const;
	int InvokeFullCallback() const;

	BOOL GetTownPick() const;
	void SetTownPick(BOOL bSet);
	BOOL GetClientMove() const;
	void SetClientMove(BOOL bSet);
	DWORD GetRadius() const;
	void SetRadius(DWORD dwRadius);
	DWORD GetDelay() const;
	void SetDelay(DWORD dwDelay);
	int GetLeftCol() const;
	int GetRightCol() const;
	void SetMoveColumns(int nLeftCol, int nRightCol);
	BOOL GetUseCube() const;
	void SetUseCube(BOOL bSet);
	LPCSTR GetFullCallback() const;
	void SetFullCallback(LPCSTR lpsz);
	LPCSTR GetStashallback() const;
	void SetStashCallback(LPCSTR lpsz);
	LPCSTR GetLogFile() const;
	void SetLogFile(LPCSTR lpszPath);

	void DisplayRadius() const;
	void DisplayDelay() const;
	void DisplayTownPick() const;
	void DisplayClientMove() const;
	void DisplayColumnsPick() const;
	void DisplayUseCube() const;
	void DisplayFullCallback() const;
	void DisplayStashCallback() const;
	void DisplayLog() const;
	void DisplaySummary() const;

private:
	
	static void CALLBACK FreeCfgItem(CGetitConfig*& pData);
	static BOOL CALLBACK EnumSectionProc(LPCSTR lpszSection, LPVOID lpParam);
	static int InvokeCallback(CStringEx str);	

	BOOL m_bTownPick;
	DWORD m_dwRadius;
	BOOL m_bClientMove;
	DWORD m_dwDelay;
	int m_nLeftCol;
	int m_nRightCol;
	BOOL m_bUseCube;
	CStringEx m_sFullCallback;
	CStringEx m_sIDCallback;
	CStringEx m_sLogFile;
	CIni m_ini;
	CLinkedList<CGetitConfig*, CGetitConfig*, FreeCfgItem> m_aConfigs;
};

#endif // __GETITCONFIG_H__
