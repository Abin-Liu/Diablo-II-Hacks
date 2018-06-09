// D2ItemDesc.h: interface for the CD2ItemDesc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_D2ITEMDESC_H__7630E132_6012_44CC_9B5E_C4E1E358A998__INCLUDED_)
#define AFX_D2ITEMDESC_H__7630E132_6012_44CC_9B5E_C4E1E358A998__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\\..\\Common\\ArrayEx.h"
#include "..\\external\\ado.h"
#include "..\\external\\Picture.h"
#include "..\\..\\Common\\ItemEventDef.h"
#include "..\\..\\Common\\LinkedList.h"
#include "..\\external\\ResPack.h"

#define LINE_GAP	2

struct MODDETAIL
{
	DWORD dwModID;
	int aValues[4];
	int nValueCount;
	BOOL bFloat;
	BOOL bBoolean;
	char szFmt[256];
};

typedef CArrayEx<MODDETAIL, const MODDETAIL&> CModLib;

BOOL operator==(const MODDETAIL& lhs, const MODDETAIL& rhs);
BOOL operator>(const MODDETAIL& lhs, const MODDETAIL& rhs);

class CD2ItemData
{
public:
	static COLORREF GetDescColor(DWORD dwQuality, BOOL bEthereal, int nSockets, BOOL bRuneword, BOOL bQuestItem);
	static BOOL IsIndexedItem(LPCSTR lpszCode);
	static BOOL IsQuestItem(LPCSTR lpszCode);
	void LoadUnknownResource(CResPack* pPack);
	BOOL LoadImageResource(CResPack* pPack);
	BOOL FindMod(DWORD dwModID) const;
	BOOL IsSpecialCode(LPSTR lpsz) const;
	static UINT FindUnknownResource(BYTE cx, BYTE cy);
	static BOOL IsBlund(DWORD dwTypeID);
	static BOOL IsValSame(int n1, int n2, int n3, int n4);
	void InsertStr(const CString& str);
	COLORREF GetDescColor() const;

	BOOL FromDatabase(CADODatabase* pDatabase, CADORecordset* pRS, const CModLib* aModLib, CResPack* pPack);
	void Format();
	BOOL GetModDesc(const MODDETAIL& mod, CString& sResult, int& nVal);
	static void UpdateCxCy(const CString& str, long& cx, long& cy, CDC* pDC);
	void CalcHoverSize(CWnd* pDlg);
	void DrawItem(CDC* pDC, const CRect& rect);

	DWORD dwItemID;	
	DWORD dwPlayerID;
	DWORD dwTypeID;
	DWORD dwFlags;
	DWORD dwStrReq;
	DWORD dwDexReq;
	DWORD dwLvlReq;
	DWORD dwiLevel;
	DWORD dwMinDmg;
	DWORD dwMaxDmg;
	DWORD dwDef;
	DWORD dwQty;
	DWORD dwMaxDura;
	DWORD dwMinDura;
	DWORD dwStorageID;
	int nArtwork;
	int x;
	int y;
	int cx;
	int cy;
	int nSockets;
	DWORD dwQualityID;
	BOOL bTwoHand; 
	BOOL bEthereal;
	BOOL bBlund;
	BOOL bRuneword;
	BOOL bIdentified;
	BOOL bQuestItem;

	CString sCode;
	CString sQualityName;
	CString sTypeName;
	CString sName;
	CString sLocation;

	MODDETAIL aMods[16];
	int nModCount;
	CArrayEx<CString, const CString&> m_aTitle;
	CArrayEx<CString, const CString&> m_aHead;
	CArrayEx<CString, const CString&> m_aMods;
	CArrayEx<CString, const CString&> m_aFoot;
	CPicture image;
	SIZE czImage;
	SIZE czHover;
};

void CALLBACK FreeD2ItemData(CD2ItemData*& pData);

class CD2ItemDesc
{
public:
	void CopyData(const CD2ItemData* pData);
	BOOL FromDatabase(CADORecordset* pRS);
	CD2ItemDesc();
	CD2ItemDesc(const CD2ItemDesc& rhs);
	const CD2ItemDesc& operator=(CD2ItemDesc& rhs);
	BOOL operator==(CD2ItemDesc& rhs) const;
	BOOL operator>(CD2ItemDesc& rhs) const;

	DWORD dwID;
	DWORD dwLevel;
	int nSockets;
	BOOL bRuneword;
	BOOL bEthereal;
	BOOL bQuest;
	CString sName;
	CString sCode;
	CString sQlyName;
	CString sType;
	CString sLocation;
	CString sPlayer;
	CString sAccount;
	CString sRealm;
	COLORREF descColor;
};

class CD2PlayerData
{
public:	

	BOOL FromDatabase(CADODatabase *pDatabase, CADORecordset *pRS, const CModLib* aModLib, CResPack* pPack);
	BOOL RemoveItem(DWORD dwItemID);
	BOOL RemoveItem(int nStorageType, int x, int y);
	BOOL AddItem(CD2ItemData* pItem);
	CD2ItemData* FindItem(int nStorageID, int x, int y);

	CLinkedList<CD2ItemData*, CD2ItemData*, FreeD2ItemData> m_aItemList;
	DWORD dwPlayerID;
	CString sName;
	BOOL bHardcore;
	DWORD dwClassID;
	CString sClassName;
	DWORD dwAccountID;
	DWORD dwInvGold;
	DWORD dwStashGold;
	DWORD dwGoldLimit;
	DWORD dwMercClass;
	CString sMercName;
	int nActiveSwitch;
};

void CALLBACK FreeD2PlayerData(CD2PlayerData*& pData);


#endif // !defined(AFX_D2ITEMDESC_H__7630E132_6012_44CC_9B5E_C4E1E358A998__INCLUDED_)
