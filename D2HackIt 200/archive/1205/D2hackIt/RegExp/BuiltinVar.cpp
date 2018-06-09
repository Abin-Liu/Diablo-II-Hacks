//////////////////////////////////////////////////////////////////////////
// BuiltinVar.cpp
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////////

#include "BuiltinVar.h"
#include <string.h>
#include "D2HackIt.h"

static const DWORD ALLRES_IDS[4] = { 39, 41, 43, 45 }; 
static const DWORD ALLATTR_IDS[4] = { 0, 1, 2, 3 }; 

int GetBV_quality(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_quantity(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_sockets(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_ethereal(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_identified(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_value(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_level(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_defense(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_runeword(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_all_res(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_all_attr(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_sizex(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_sizey(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_req_str(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_req_dex(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);
int GetBV_req_level(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);

static const BUILTINVAR BVS[] = {
	{ "code", NULL, 0 },
	{ "type", NULL, 0 },
	{ "quality", GetBV_quality, 0 },
	{ "quantity", GetBV_quantity, 0 },
	{ "sockets", GetBV_sockets, 0 },
	{ "ethereal", GetBV_ethereal, 0 },
	{ "identified", GetBV_identified, 0 },
	{ "value", GetBV_value, 0 },
	{ "level", GetBV_level, 0 },
	{ "defense", GetBV_defense, 0 },
	{ "runeword", GetBV_runeword, 0 },
	{ "all_res", GetBV_all_res, 0 },
	{ "all_attr", GetBV_all_attr, 0 },
	{ "true", NULL, 1 },
	{ "false", NULL, 0 },
	{ "sizex", GetBV_sizex, 0 },
	{ "sizey", GetBV_sizey, 0 },
	{ "inferior", NULL, 1 },
	{ "normal", NULL, 2 },
	{ "superior", NULL, 3 },
	{ "magical", NULL, 4 },
	{ "set", NULL, 5 },
	{ "rare", NULL, 6 },
	{ "unique", NULL, 7 },
	{ "craft", NULL, 8},
	{ "req_str", GetBV_req_str, 0 },
	{ "req_dex", GetBV_req_dex, 0 },
	{ "req_level", GetBV_req_level, 0 },
	{ "", NULL, 0 }
};

BOOL operator==(const BUILTINVAR& lhs, const BUILTINVAR& rhs)
{
	return ::strcmp(lhs.szName, rhs.szName) == 0;
}

BOOL operator>(const BUILTINVAR& lhs, const BUILTINVAR& rhs)
{
	return ::strcmp(lhs.szName, rhs.szName) > 0;
}

CBuiltinVarDictionary::CBuiltinVarDictionary()
{
	m_aVars.RemoveAll();
	for (int i = 0; BVS[i].szName[0]; i++)
		m_aVars.Add(BVS[i]);
	m_aVars.Sort();
}

LPCBUILTINVAR CBuiltinVarDictionary::Lookup(LPCSTR lpszVarName) const
{
	if (lpszVarName == NULL || *lpszVarName == 0)
		return NULL;

	BUILTINVAR temp;
	::strcpy(temp.szName, lpszVarName);
	int nIndex = m_aVars.Find(temp);
	if (nIndex == -1)
		return NULL;

	const BUILTINVAR& bv = m_aVars.GetAt(nIndex);
	return &bv;
}

BOOL CBuiltinVarDictionary::Lookup(LPCSTR lpszVarName, DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount, int& rValue) const
{
	rValue = 0;
	LPCBUILTINVAR p = Lookup(lpszVarName);
	if (p == NULL)
		return FALSE;

	if (p->GetBuiltinValue == NULL)
		rValue = p->nConstant;
	else
		rValue = p->GetBuiltinValue(dwItemID, lpszItemCode, lpMod, dwModCount);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////
// Built-in Value Retrieval
//////////////////////////////////////////////////////////////////////////////////
int GetBV_quality(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	return GetItemQuality(dwItemID);
}

int GetBV_quantity(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	GAMEUNIT gu = { dwItemID, UNIT_TYPE_ITEM };
	return GetUnitStat(&gu, STAT_AMMOQUANTITY);
}

int GetBV_sockets(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	return GetItemSockets(dwItemID);
}

int GetBV_ethereal(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	return (GetItemAttributes(dwItemID) & ITEM_ETHEREAL) != 0;
}

int GetBV_identified(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	return (GetItemAttributes(dwItemID) & ITEM_IDENTIFIED) != 0;
}

int GetBV_value(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	GAMEUNIT gu = { dwItemID, UNIT_TYPE_ITEM };
	if (::strcmp(lpszItemCode, "gld"))
	{
		return GetItemPrice(dwItemID, 513, FALSE);
	}
	else
	{
		return GetUnitStat(&gu, STAT_GOLD);
	}		
}

int GetBV_level(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	return (int)GetItemLevel(dwItemID);
}

int GetBV_defense(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	GAMEUNIT gu = { dwItemID, UNIT_TYPE_ITEM };
	return (int)GetUnitStat(&gu, 31);
}

int GetBV_runeword(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	return (GetItemAttributes(dwItemID) & ITEM_RUNEWORD) != 0;
}

int GetBV_all_res(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	int nAllRes = 65535;
	for (int i = 0; i < 4; i++)
	{
		LPCITEMMOD p = CModDef::CheckItemMods(lpMod, dwModCount, ALLRES_IDS[i]);
		if (p == NULL)
			return 0;

		nAllRes = min(nAllRes, p->aValues[0]);
	}
		
	return nAllRes;
}

int GetBV_all_attr(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	int nAllSttr = 65535;
	for (int i = 0; i < 4; i++)
	{
		LPCITEMMOD p = CModDef::CheckItemMods(lpMod, dwModCount, ALLATTR_IDS[i]);
		if (p == NULL)
			return 0;

		nAllSttr = min(nAllSttr, p->aValues[0]);
	}
		
	return nAllSttr;
}

int GetBV_sizex(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	SIZE sz = GetItemSize(lpszItemCode);
	return sz.cx;
}

int GetBV_sizey(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	SIZE sz = GetItemSize(lpszItemCode);
	return sz.cy;
}

int GetBV_req_str(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	return GetItemStrRequirement(dwItemID);
}

int GetBV_req_dex(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	return GetItemDexRequirement(dwItemID);
}

int GetBV_req_level(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount)
{
	return GetItemLevelRequirement(dwItemID);
}

