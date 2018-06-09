/////////////////////////////////////////////////////////////////////
// ModDef.cpp
//
// Abin (abinn32@yahoo.com)
/////////////////////////////////////////////////////////////////////

#include "ModDef.h"
#include "..\\..\\Includes\\Utilities\\Ini.h"
#include "D2HackIt.h"
#include "BuiltinVar.h"

enum { INDEX_NONE = 0, INDEX_VALID, INDEX_INVALID };

const static LPCSTR COMMENTS[] = { "//", ";", "'", NULL }; // comments(c/ini/vb styles)

void RemoveComments(CStringEx& str)
{
	for (int i = 0; COMMENTS[i]; i++)
	{
		int nIdx = str.Find(COMMENTS[i]);
		if (nIdx != -1)
		{
			str.SetAt(nIdx, '\0');
			return;
		}
	}
}

CModDef::CModDef()
{
	Clear();
}

CModDef::CModDef(const CModDef& rSrc)
{
	m_sAlias = rSrc.m_sAlias;
	m_dwModID = rSrc.m_dwModID;
	::memcpy(m_aMatchs, rSrc.m_aMatchs, sizeof(m_aMatchs));
	::memcpy(m_aValues, rSrc.m_aValues, sizeof(m_aValues));
}

void CModDef::Clear()
{
	m_sAlias.Empty();
	m_dwModID = -1;
	::memset(m_aMatchs, 0, sizeof(m_aMatchs));
	::memset(m_aValues, 0, sizeof(m_aValues));
}

const CModDef& CModDef::operator=(const CModDef& rhs)
{
	if (this != &rhs)
	{
		m_sAlias = rhs.m_sAlias;
		m_dwModID = rhs.m_dwModID;
		::memcpy(m_aMatchs, rhs.m_aMatchs, sizeof(m_aMatchs));
		::memcpy(m_aValues, rhs.m_aValues, sizeof(m_aValues));
	}
	return *this;
}

BOOL CModDef::operator==(const CModDef& rhs) const
{
	return m_sAlias == rhs.m_sAlias;
}

BOOL CModDef::operator>(const CModDef& rhs) const
{
	return m_sAlias > rhs.m_sAlias;
}

BOOL CModDef::Parse(LPCSTR lpszAlias, LPCSTR lpszDef)
{
	Clear();

	if (lpszAlias == NULL || lpszDef == NULL)
		return FALSE;

	m_sAlias = lpszAlias;
	RemoveComments(m_sAlias);
	m_sAlias.Trim();
	m_sAlias.MakeLower();

	if (m_sAlias.IsEmpty() || (m_sAlias[0] != '_' && !::isalpha(m_sAlias[0])))
		return FALSE;

	// parse mod ID and values
	CStringEx sDef(lpszDef);
	RemoveComments(sDef);
	sDef.Trim();

	if (sDef.IsEmpty())
		return FALSE;

	// moddef: nnn,?,?,?,?
	BOOL bContinue = sDef.FindToken(",");
	CStringEx sValue;
	int nValCount = 0;
	for (int i = 0; i < 5 && nValCount < 4 && bContinue; i++)
	{
		bContinue = sDef.FindNextToken(sValue);
		sValue.Trim();

		const BOOL ALLDIGITS = sValue.IsDigit();
		if (m_dwModID == -1)
		{
			if (!ALLDIGITS)
				return FALSE;
			
			m_dwModID = ::strtoul(sValue, NULL, 10);
		}
		else
		{
			if (ALLDIGITS)
			{
				m_aValues[nValCount] = atoi(sValue);
				m_aMatchs[nValCount] = 1;				
			}
			
			nValCount++;
		}		
	}

	return m_dwModID != -1;
}

const CModDef& CModDictionary::operator[](int nIndex) const
{
	return m_aDefs.GetAt(nIndex);
}

DWORD CModDef::GetModID() const
{
	return m_dwModID;
}

LPCSTR CModDef::GetAlias() const
{
	return m_sAlias;
}

int CModDef::GetValue(int nIndex) const
{
	return nIndex >= 0 && nIndex < 4 ? m_aValues[nIndex] : 0;
}

BYTE CModDef::GetMatchFlag(int nIndex) const
{
	return nIndex >= 0 && nIndex < 4 ? m_aMatchs[nIndex] : 0x00;
}

BOOL CModDef::CompareSingleMod(const ITEMMOD& im) const
{
	if (m_sAlias.IsEmpty() || m_dwModID == -1)
		return FALSE;
	
	if (m_dwModID != im.dwModID && m_dwModID != im.dwOriginalID)
		return FALSE;

	for (int i = 0; i < 4; i++)
	{
		//GameInfof("i=%d, match %d, v1 %d  v2 %d", i, m_aMatchs[i], m_aValues[i], im.aValues[i]);
		if (m_aMatchs[i] && m_aValues[i] != im.aValues[i])
			return FALSE;
	}
	
	return TRUE;
}

LPCITEMMOD CModDef::CheckItemMods(LPCITEMMOD lpItemMods, int nCount) const
{
	if (lpItemMods == NULL)
		return NULL;

	for (int i = 0; i < nCount; i++)
	{
		if (CompareSingleMod(lpItemMods[i]))
			return &lpItemMods[i];
	}
	return NULL;
}

CModDictionary::CModDictionary()
{
}

const CModDef* CModDictionary::Lookup(LPCSTR lpszAlias) const
{
	if (lpszAlias == NULL)
		return NULL;

	CModDef md; // temporary object
	md.m_sAlias = lpszAlias;
	md.m_sAlias.Trim();
	md.m_sAlias.MakeLower();

	int nIdx = m_aDefs.Find(md);
	if (nIdx == -1)
		return NULL;

	const CModDef& cmd = m_aDefs.GetAt(nIdx);
	return &cmd;
}

int CModDictionary::Load(LPCSTR lpszFilePath)
{
	m_aDefs.RemoveAll();
	if (lpszFilePath == NULL)
		return 0;

	CIni ini(lpszFilePath);
	ini.EnumKeys("mod", EnumModProc, (LPVOID)this);
	
	// Sort to enable binary fast search!
	m_aDefs.Sort();
	return m_aDefs.GetSize();
}

BOOL CALLBACK CModDictionary::EnumModProc(LPCTSTR lpKeyName, LPCTSTR lpValue, LPVOID lpParam)
{
	CModDictionary* pDic = (CModDictionary*)lpParam;
	if (pDic == NULL)
		return FALSE;

	CModDef md;
	if (!md.Parse(lpKeyName, lpValue))
	{
		GameErrorf("Invalid mod definition: ÿc1%s=%s", lpKeyName, lpValue);
		return TRUE;
	}

	if (pDic->m_builtinDictionary.Lookup(md.GetAlias()))
	{
		GameErrorf("Invalid alias: ÿc1%sÿc0. Conflicts with a built-in variable.", lpKeyName);
		return TRUE;
	}

	int nIndex = pDic->m_aDefs.Find(md);
	if (nIndex != -1)
	{
		GameErrorf("Alias redefinition: ÿc1%s", lpKeyName);
	}
	else
	{
		pDic->m_aDefs.Add(md);
	}

	return TRUE;
}


int CModDictionary::GetSize() const
{
	return m_aDefs.GetSize();
}

LPCITEMMOD CModDef::CheckItemMods(LPCITEMMOD lpItemMods, int nCount, DWORD dwModID)
{
	if (lpItemMods == NULL)
		return NULL;

	for (int i = 0; i < nCount; i++)
	{
		if (lpItemMods[i].dwModID == dwModID || lpItemMods[i].dwOriginalID == dwModID)
			return &lpItemMods[i];
	}

	return NULL;

}

CTypeDef::CTypeDef()
{
}

CTypeDef::CTypeDef(const CTypeDef& rSrc)
{
	m_sType = rSrc.m_sType;
	m_aCodes.Copy(rSrc.m_aCodes);
}

const CTypeDef& CTypeDef::operator=(const CTypeDef& rhs)
{
	if (this != &rhs)
	{
		m_sType = rhs.m_sType;
		m_aCodes.Copy(rhs.m_aCodes);
	}

	return *this;	
}

BOOL CTypeDef::operator==(const CTypeDef& rhs) const
{
	return m_sType == rhs.m_sType;
}

BOOL CTypeDef::operator>(const CTypeDef& rhs) const
{
	return m_sType > rhs.m_sType;
}

BOOL CTypeDef::CodeMatch(LPCSTR lpszCode) const
{
	if (lpszCode == NULL)
		return FALSE;

	if (m_aCodes.Find(lpszCode) != -1)
		return TRUE;

	return m_aCodes.Find("*") != -1;
}

BOOL CTypeDef::SetType(LPCSTR lpszType, LPCSTR lpszCodes)
{
	m_sType = lpszType;
	m_aCodes.RemoveAll();

	RemoveComments(m_sType);
	m_sType.Trim();
	if (m_sType.IsEmpty())
		return FALSE;
	m_sType.MakeLower();

	CStringEx sCodes = lpszCodes;
	RemoveComments(sCodes);
	sCodes.Trim();
	sCodes.MakeLower();

	CStringEx sTemp;
	BOOL bOK = sCodes.FindToken(",");
	while (bOK)
	{
		bOK = sCodes.FindNextToken(sTemp);
		sTemp.Trim();
		if (!sTemp.IsEmpty() && m_aCodes.Find(sTemp) == -1)
			m_aCodes.Add(sTemp);
	}

	m_aCodes.Sort();
	return m_aCodes.GetSize();
}

CTypeDictionary::CTypeDictionary()
{
}

int CTypeDictionary::Load(LPCSTR lpszFilePath)
{
	m_aDefs.RemoveAll();
	if (lpszFilePath == NULL)
		return 0;

	CIni ini(lpszFilePath);
	ini.EnumKeys("type", EnumTypeProc, (LPVOID)&m_aDefs);
	
	// Sort to enable binary fast search!
	m_aDefs.Sort();
	return m_aDefs.GetSize();
}

BOOL CTypeDictionary::Lookup(LPCSTR lpszType, LPCSTR lpszItemCode) const
{
	CTypeDef td;
	if (!td.SetType(lpszType, lpszItemCode))
		return FALSE;

	//GameInfof("type %s", td.GetType());

	int nIndex = m_aDefs.Find(td);
	//GameInfof("idx %d", nIndex);
	if (nIndex == -1)
		return FALSE;

	return m_aDefs.GetAt(nIndex).CodeMatch(td.m_aCodes[0]);
}

BOOL CALLBACK CTypeDictionary::EnumTypeProc(LPCTSTR lpKeyName, LPCTSTR lpValue, LPVOID lpParam)
{
	CArrayEx<CTypeDef, const CTypeDef&>* pArray = (CArrayEx<CTypeDef, const CTypeDef&>*)lpParam;
	if (pArray == NULL)
		return FALSE;

	CTypeDef td;
	if (td.SetType(lpKeyName, lpValue))
		pArray->Add(td);
	else
		GameErrorf("Invalid type definition: ÿc1%s=%s", lpKeyName, lpValue);
	return TRUE;
}

int CTypeDef::GetCodeCount() const
{
	return m_aCodes.GetSize();
}

LPCSTR CTypeDef::operator[](int nIndex) const
{
	if (!m_aCodes.IsIndexValid(nIndex))
		return NULL;
	return m_aCodes.GetAt(nIndex);
}

LPCSTR CTypeDef::GetType() const
{
	return m_sType;
}

LPCSTR CTypeDef::GetCodesString() const
{
	CStringEx str;
	for (int i = 0; i < m_aCodes.GetSize(); i++)
	{
		str += m_aCodes[i];
		str += ',';
	}
	str.TrimRight(',');
	return str;
}

LPCSTR CTypeDef::GetCode(int nIndex) const
{
	if (!m_aCodes.IsIndexValid(nIndex))
		return NULL;
	return m_aCodes.GetAt(nIndex);
}

BOOL CModDictionary::Lookup(CStringEx &sVarName, DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount, int &rValue) const
{
	rValue = 0;	
	sVarName.Trim();
	if (sVarName.IsEmpty())
		return FALSE;

	// has indexing operators?
	int nIndex = 0;
	int nIdxStats = ExtractIndex(sVarName, nIndex);
	if (nIdxStats == INDEX_INVALID)
	{
		GameErrorf("Invalid indexing: ÿc1%s", (LPCSTR)sVarName);
		return FALSE;
	}	

	// is constant?
	if (sVarName[0] == '$')
	{
		sVarName.Delete(0);
		if (!sVarName.IsDigit())
		{
			GameErrorf("Invalid constant: ÿc1$%s", (LPCSTR)sVarName);
			return FALSE;
		}

		rValue = CModDef::GetModValue(lpMod, dwModCount, ::atoi(sVarName), nIndex);
		return TRUE;
	}

	// is built-in variable?
	LPCBUILTINVAR pbv = m_builtinDictionary.Lookup(sVarName);
	if (pbv)
	{
		if (nIdxStats != INDEX_NONE)
		{
			GameErrorf("Indexing not allowed on built-in variable: ÿc1%s", (LPCSTR)sVarName);
			return FALSE;
		}	

		rValue = GetBuiltinVarValue(pbv, dwItemID, lpszItemCode, lpMod, dwModCount);
		return TRUE;
	}
	
	// user difined variable
	const CModDef* pmd = Lookup(sVarName);
	if (pmd == NULL)
	{
		GameErrorf("Undefined mod alias: ÿc1%s", (LPCSTR)sVarName);
		return FALSE;
	}

	LPCITEMMOD p = pmd->CheckItemMods(lpMod, dwModCount);
	if (p)
		rValue = p->aValues[nIndex];
	
	return TRUE;
}

int CModDictionary::ExtractIndex(CStringEx &str, int& nIndex)
{
	nIndex = 0;

	// verify
	const int START = str.Find('[');
	if (START == -1)
		return INDEX_NONE;
	
	if (str[str.GetLength() - 1] != ']' || str.Occurence('[') != 1 || str.Occurence(']') != 1)
		return INDEX_INVALID;

	CStringEx sIndex = str.Mid(START + 1);
	sIndex.TrimRight(']');
	sIndex.Trim();
	
	if (sIndex.GetLength() != 1 || sIndex[0] < '0' || sIndex[0] > '3')
		return INDEX_INVALID;

	str.SetAt(START, '\0');
	str.Trim();
	nIndex = sIndex[0] - '0';
	return INDEX_VALID;
}

int CModDef::GetModValue(LPCITEMMOD lpMod, DWORD dwModCount, DWORD dwModID, int nIndex)
{
	if (nIndex < 0 || nIndex > 3)
		nIndex = 0;

	LPCITEMMOD p = CheckItemMods(lpMod, (int)dwModCount, dwModID);
	if (p == NULL)
		return 0;

	return p->aValues[nIndex];
}
