//////////////////////////////////////////////////////////////////////
// ItemDatabase.h
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __ITEMDATABASE_H__
#define __ITEMDATABASE_H__

#include "..\\external\\ado.h"
#include "..\\external\\ReportCtrl.h"
#include "..\\regexp\\ServerItem.h"
#include "D2ItemDesc.h"

enum { RECORD_FAIL = 0, RECORD_EXIST, RECORD_ADDED, RECORD_CHANGED };

typedef BOOL (CALLBACK *LISTPROC)(const CD2ItemDesc* pItem, WPARAM wParam, LPARAM lParam);
typedef BOOL (CALLBACK *QueryProc)(CADORecordset* pRS, LPARAM lParam);

class CItemDatabase : public CSyncObj
{
public:		

	CItemDatabase();
	virtual ~CItemDatabase();

	BOOL OpenDatabase(LPCSTR lpszPath);
	void CloseDatabase();
	BOOL Execute(LPCSTR lpszCommand);
	BOOL Query(CADORecordset* pRS, LPCSTR lpszFmt, ...);
	BOOL Query(QueryProc lpfn, LPARAM lParam, LPCSTR lpszFmt, ...);

	BOOL KeywordSearch(LPCSTR lpsz, CReportCtrl* pwndList);
	BOOL ExpSearch(LPCSTR lpsz, CReportCtrl* pwndList, BOOL& rSyntaxError);
	BOOL LoadItemInfo(CADORecordset* pRS, LPSERVERITEM lpBuffer);
	DWORD GetItemCount();

	int GetRealmID(LPCSTR lpszRealmName, DWORD& rResult);
	int GetAccountID(LPCSTR lpszAccountName, DWORD dwRealmID, DWORD& rResult);
	DWORD CreatePlayer(LPCCHARINFO lpChar, DWORD dwAccountID);
	DWORD GetTitleID(LPCITEMINFO p);
	BYTE GetTypeID(BYTE iTypeID);
	void CheckItemBase(LPCITEMINFO p);
	const CModLib* GetModLib() const;	
	DWORD AddItem(DWORD dwPlayerID, LPITEMINFO p);
	operator CADODatabase*();

	BOOL GetAccountPlayerFromItem(DWORD dwItemID, DWORD& rRealmID, DWORD& rAccountID, DWORD& rPlayerID);
	BOOL DeletePlayer(DWORD dwID);
	BOOL DeleteAccount(DWORD dwID);
	BOOL DeleteRealm(DWORD dwID);

private:
	static void ListItem(CADORecordset* pRS, CReportCtrl* pwndList);

	int LoadModLib();
	int UpdateGetID1(LPCSTR lpszName, LPCSTR lpszTableName, LPCSTR lpszIDField, LPCSTR lpszNameField, DWORD& rResult);
	int UpdateGetID2(LPCSTR lpszName, LPCSTR lpszTableName, LPCSTR lpszIDField, LPCSTR lpszNameField, LPCSTR lpszRelateIDField, DWORD dwRelateID, DWORD& rResult);
	static CString GetItemQueryString(LPCSTR lpsz);

	CADODatabase m_db;
	CModLib m_aModLib;
};

#endif // __ITEMDATABASE_H__
