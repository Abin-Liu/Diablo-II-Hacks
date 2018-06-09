//////////////////////////////////////////////////////////////////////////
// BuiltinVar.cpp
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "BuiltinVar.h"
#include <string.h>
#include "ModDef.h"

static const DWORD ALLRES_IDS[4] = { 39, 41, 43, 45 }; 
static const DWORD ALLATTR_IDS[4] = { 0, 1, 2, 3 }; 

int GetBV_type(LPCSERVERITEM lpItem);
int GetBV_quality(LPCSERVERITEM lpItem);
int GetBV_quantity(LPCSERVERITEM lpItem);
int GetBV_sockets(LPCSERVERITEM lpItem);
int GetBV_ethereal(LPCSERVERITEM lpItem);
int GetBV_identified(LPCSERVERITEM lpItem);
int GetBV_value(LPCSERVERITEM lpItem);
int GetBV_level(LPCSERVERITEM lpItem);
int GetBV_defense(LPCSERVERITEM lpItem);
int GetBV_runeword(LPCSERVERITEM lpItem);
int GetBV_allres(LPCSERVERITEM lpItem);
int GetBV_allattr(LPCSERVERITEM lpItem);
int GetBV_sizex(LPCSERVERITEM lpItem);
int GetBV_sizey(LPCSERVERITEM lpItem);
int GetBV_strreq(LPCSERVERITEM lpItem);
int GetBV_dexreq(LPCSERVERITEM lpItem);
int GetBV_levelreq(LPCSERVERITEM lpItem);
int GetBV_twohand(LPCSERVERITEM lpItem);
int GetBV_mindamage(LPCSERVERITEM lpItem);
int GetBV_maxdamage(LPCSERVERITEM lpItem);

static const BUILTINVAR BVS[] = {
	{ "code", NULL, 0 },
	{ "type", GetBV_type, 0 },
	{ "quality", GetBV_quality, 0 },
	{ "quantity", GetBV_quantity, 0 },
	{ "sockets", GetBV_sockets, 0 },
	{ "ethereal", GetBV_ethereal, 0 },
	{ "identified", GetBV_identified, 0 },
	{ "value", GetBV_value, 0 },
	{ "level", GetBV_level, 0 },
	{ "defense", GetBV_defense, 0 },
	{ "runeword", GetBV_runeword, 0 },
	{ "allres", GetBV_allres, 0 },
	{ "allattr", GetBV_allattr, 0 },
	{ "sizex", GetBV_sizex, 0 },
	{ "sizey", GetBV_sizey, 0 },	
	{ "strreq", GetBV_strreq, 0 },
	{ "dexreq", GetBV_dexreq, 0 },
	{ "levelreq", GetBV_levelreq, 0 },
	{ "twohand", GetBV_twohand, 0 },
	{ "mindamage", GetBV_mindamage, 0 },
	{ "maxdamage", GetBV_maxdamage, 0 },
	{ "damage", GetBV_maxdamage, 0 },

	// item quality constants
	{ "inferior", NULL, 1 },
	{ "normal", NULL, 2 },
	{ "superior", NULL, 3 },
	{ "magical", NULL, 4 },
	{ "set", NULL, 5 },
	{ "rare", NULL, 6 },
	{ "unique", NULL, 7 },
	{ "craft", NULL, 8},

	// item type constants
	{ "shield", NULL, 2 },
	{ "armor", NULL, 3 },
	{ "boots", NULL, 15 },
	{ "gloves", NULL, 16 },
	{ "belt", NULL, 19 },
	{ "scepter", NULL, 24 },
	{ "wand", NULL, 25},
	{ "staff", NULL, 26 },
	{ "bow", NULL, 27 },
	{ "axe", NULL, 28 },
	{ "club", NULL, 29 },
	{ "sword", NULL, 30 },
	{ "hammer", NULL, 31 },
	{ "dagger", NULL, 32 },
	{ "spear", NULL, 33 },
	{ "polearm", NULL, 34 },
	{ "crossbow", NULL, 35 },
	{ "mace", NULL, 36 },
	{ "helm", NULL, 37 },
	{ "throwknife", NULL, 42 },
	{ "throwaxe", NULL, 43 },
	{ "javelin", NULL, 44},
	{ "orb", NULL, 68 },
	{ "palshield", NULL, 70 },
	{ "barhelm", NULL, 71 },
	{ "rune", NULL, 74 },
	{ "circlet", NULL, 75 },
	{ "claw", NULL, 88},
	{ "amajavelin", NULL, 87 },
	{ "amabow", NULL, 85 },
	{ "amaspear", NULL, 86 },
	{ "druhelm", NULL, 72 },
	{ "necshield", NULL, 69 },
	
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

BOOL CBuiltinVarDictionary::Lookup(LPCSTR lpszVarName, LPCSERVERITEM lpItem, int& rValue) const
{
	rValue = 0;
	LPCBUILTINVAR p = Lookup(lpszVarName);
	if (p == NULL)
		return FALSE;

	rValue = GetBuiltinVarValue(p, lpItem);	
	return TRUE;
}

int GetBuiltinVarValue(LPCBUILTINVAR pbv, LPCSERVERITEM lpItem)
{
	if (pbv == NULL)
		return 0;

	if (pbv->GetBuiltinValue == NULL)
		return pbv->nConstant;
	else
		return pbv->GetBuiltinValue(lpItem);
}

//////////////////////////////////////////////////////////////////////////////////
// Built-in Value Retrieval
//////////////////////////////////////////////////////////////////////////////////
int GetBV_type(LPCSERVERITEM lpItem)
{
	return lpItem->iType;
}

int GetBV_quality(LPCSERVERITEM lpItem)
{
	return lpItem->iQuality;
}

int GetBV_quantity(LPCSERVERITEM lpItem)
{
	return lpItem->wQuantity;
}

int GetBV_sockets(LPCSERVERITEM lpItem)
{
	return lpItem->iSockets;
}

int GetBV_ethereal(LPCSERVERITEM lpItem)
{
	return (lpItem->dwFlags & 0x00400000) != 0;
}

int GetBV_identified(LPCSERVERITEM lpItem)
{
	return (lpItem->dwFlags & 0x00000010) != 0;
}

int GetBV_value(LPCSERVERITEM lpItem)
{
	return lpItem->dwValue;
}

int GetBV_level(LPCSERVERITEM lpItem)
{
	return lpItem->iLevel;
}

int GetBV_defense(LPCSERVERITEM lpItem)
{
	return lpItem->wdefense;
}

int GetBV_runeword(LPCSERVERITEM lpItem)
{
	return (lpItem->dwFlags & 0x04000000) != 0;
}

int GetBV_allres(LPCSERVERITEM lpItem)
{
	int nAllRes = 65535;
	for (int i = 0; i < 4; i++)
	{
		LPCITEMMOD p = CModDef::CheckItemMods(lpItem->aMods, lpItem->iModCount, ALLRES_IDS[i]);
		if (p == NULL)
			return 0;

		nAllRes = min(nAllRes, p->aValues[0]);
	}
		
	return nAllRes;
}

int GetBV_allattr(LPCSERVERITEM lpItem)
{
	int nAllSttr = 65535;
	for (int i = 0; i < 4; i++)
	{
		LPCITEMMOD p = CModDef::CheckItemMods(lpItem->aMods, lpItem->iModCount, ALLATTR_IDS[i]);
		if (p == NULL)
			return 0;

		nAllSttr = min(nAllSttr, p->aValues[0]);
	}
		
	return nAllSttr;
}

int GetBV_sizex(LPCSERVERITEM lpItem)
{
	return lpItem->cx;
}

int GetBV_sizey(LPCSERVERITEM lpItem)
{
	return lpItem->cy;
}

int GetBV_strreq(LPCSERVERITEM lpItem)
{
	return lpItem->wStrReq;
}

int GetBV_dexreq(LPCSERVERITEM lpItem)
{
	return lpItem->wDexReq;
}

int GetBV_levelreq(LPCSERVERITEM lpItem)
{
	return lpItem->iLevelreq;
}

int GetBV_twohand(LPCSERVERITEM lpItem)
{
	return lpItem->i2Hand;
}

int GetBV_mindamage(LPCSERVERITEM lpItem)
{	
	return lpItem->wMinDamage;
}

int GetBV_maxdamage(LPCSERVERITEM lpItem)
{
	return lpItem->wMaxDamage;
}
