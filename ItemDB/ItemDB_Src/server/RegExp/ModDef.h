/////////////////////////////////////////////////////////////////////
// ModDef.h
//
// Abin (abinn32@yahoo.com)
/////////////////////////////////////////////////////////////////////

#ifndef __MODDEF_H__
#define __MODDEF_H__

#include "..\\external\\StringEx.h"
#include "..\\..\\common\\ArrayEx.h"
#include "BuiltinVar.h"

void RemoveComments(CStringEx& str); // global function

class CModDef;
class CModDictionary;

class CModDictionary
{
public:		
	BOOL Lookup(CStringEx& sVarName, LPCSERVERITEM lpItem, int& rValue) const;
	
	CModDictionary();

	int Load(LPCSTR lpszFilePath);
	int GetSize() const;
	const CModDef* Lookup(LPCSTR lpszAlias) const;
	const CModDef& operator[](int nIndex) const;


private:

	static int ExtractIndex(CStringEx& str, int& nIndex);
	static BOOL CALLBACK EnumModProc(LPCTSTR lpKeyName, LPCTSTR lpValue, LPVOID lpParam);
	CArrayEx<CModDef, const CModDef&> m_aDefs; // for fast binary search!
	CBuiltinVarDictionary m_builtinDictionary;
};

class CModDef
{
public:	
	static int GetModValue(LPCITEMMOD lpMod, DWORD dwModCount, DWORD dwModID, int nIndex);

	friend class CModDictionary;

	CModDef();
	CModDef(const CModDef& rSrc);

	LPCSTR GetAlias() const;
	DWORD GetModID() const;
	int GetValue(int nIndex) const;
	BYTE GetMatchFlag(int nIndex) const;	

	void Clear();
	BOOL Parse(LPCSTR lpszAlias, LPCSTR lpszDef);
	BOOL CompareSingleMod(const ITEMMOD& im) const;
	LPCITEMMOD CheckItemMods(LPCITEMMOD lpItemMods, int nCount) const;
	static LPCITEMMOD CheckItemMods(LPCITEMMOD lpItemMods, int nCount, DWORD dwModID);

	const CModDef& operator=(const CModDef& rhs);
	BOOL operator==(const CModDef& rhs) const;
	BOOL operator>(const CModDef& rhs) const;

private:
	
	CStringEx m_sAlias;
	DWORD m_dwModID;			// Mod ID
	int m_aValues[4];			// Values
	BYTE m_aMatchs[4];		// Must match?
};

#endif // __MODDEF_H__
