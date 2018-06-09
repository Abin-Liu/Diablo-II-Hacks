////////////////////////////////////////////////////////////////////////////
// ExamItemProperties.cpp
//
// Abin (abinn32@yahoo.com)
////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "TranslateItemMod.h"
#include "D2Hackit.h"
#include "syntax.H"
#include "..\\..\\Includes\\Utilities\\Ini.h"
#include "..\\..\\Includes\\Utilities\\ArrayEx.h"
#include "..\\D2Stdio.h"

#define EXP_MAX	512
#define ASSIGN_PTR(p,v)		((p) ? *(p)=v : NULL)

struct MOD
{
	DWORD dwModID;
	int aValues[4];
	BYTE aMatchs[4];
};

struct MOD_DEF
{
	char szModDef[256];
	MOD mod;
};

BOOL operator==(const MOD_DEF& lhs, const MOD_DEF& rhs) { return stricmp(lhs.szModDef, rhs.szModDef) == 0; }
BOOL operator>(const MOD_DEF& lhs, const MOD_DEF& rhs) { return stricmp(lhs.szModDef, rhs.szModDef) > 0; }

// for binary fast search, we will NOT search the ini file for
// each single definition or expression, otherwise it would be
// slow like hell
CArrayEx<MOD_DEF, const MOD_DEF&> g_aModDefs;

#define INT_MAX_LEN	10 // width of the largest int (INT_MAX = 4294967296)
const static char OPERS[] = "()+-*/><=!&|"; // operators
const static char BLANKS[] = " \t\n\r"; // blanks
const static LPCSTR COMMENTS[] = { "//", ";", "'", NULL }; // comments(c/ini/vb styles)
enum { CHAR_END = 0, CHAR_BLANK, CHAR_VAR, CHAR_CONST, CHAR_NUMVAR_SYMBOL, CHAR_OPER };
static const LPCSTR SPECIAL_VARS[] = {
	"code", "type", "quality", "quantity", "sockets", "ethereal", "identified",
	"value", "all_res", "level", NULL };

BOOL CALLBACK EnumModProc(LPCTSTR lpKeyName, LPCTSTR lpValue, LPVOID lpParam);
BOOL IsSpecialVar(LPCSTR lpszVar);
int GetSpecialValue(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount, LPCSTR lpszSpecial); 
BOOL GetModValue(LPCITEMMOD lpMod, DWORD dwModCount, LPSTR lpszSegment, int& rVal);
int GetModValue(LPCITEMMOD lpMod, DWORD dwModCount, const MOD* pMod, int nIndex);
int FindSpecialValue(LPCITEMMOD lpMod, DWORD dwModCount, DWORD dwModID, int nMatch);
BOOL GetNumVarValue(LPCITEMMOD lpMod, DWORD dwModCount, LPSTR lpszSegment, int& rVal);

LPCSTR TrimLeft(LPCSTR lpszString);
LPSTR TrimLeft(LPSTR lpszString);
void TrimRight(LPSTR lpszString);
LPSTR Trim(LPSTR lpszString);

BOOL IsCode(char ch);
BOOL FindCode(LPCSTR lpszString, LPCSTR lpszCode);

int GetCharType(char ch);
BOOL VerifyExpression(LPCSTR lpszExpression);
int IsAllNot(LPCSTR lpszString);
BOOL ParseModDef(LPCSTR lpszModDef, MOD& mod);
void StrnCpyTrim(LPSTR lpszBuffer, LPCSTR lpszStart, LPCSTR lpszEnd, int nLimit);
BOOL IsAllDigit(LPCSTR lpsz);
const MOD* FindModDef(LPCSTR lpszMod);
BOOL ModMatch(const ITEMMOD& im, const MOD* pMod);
BOOL CodeMatch(LPCSTR lpszItemCode, LPCSTR lpszCodeOrType);
void DebugModDef();
void SyntaxError(LPCSTR lpszError);
LPSTR GetCommentPtr(LPCSTR lpszString);
int GetIndex(LPSTR lpszString);
LPCSTR GetFirstSegment(LPCSTR lpszExp, LPSTR lpszBuffer);

/////////////////////////////////////////////////////////////////////
// ExamItemProperties
//-------------------------------------------------------------------
// Exam an item with a regular expression string.
// Abin (abinn32@yahoo.com)
/////////////////////////////////////////////////////////////////////
BOOL EXPORT ExamItemProperties(DWORD dwItemID, LPCSTR lpszExpression, LPCSTR lpszCodes, BOOL* pSyntaxError)
{
	ASSIGN_PTR(pSyntaxError, FALSE);

	if (lpszExpression == NULL || *lpszExpression == 0)
	{
		SyntaxError("Empty Expression!");
		ASSIGN_PTR(pSyntaxError, TRUE);
		return FALSE;
	}

	// veryfy item
	char szItemCode[4] = "";
	if (!GetItemCode(dwItemID, szItemCode, 3))
	{
		ItemError(dwItemID, "ExamItemProperties");		
		return FALSE;
	}

	if (!CodeMatch(szItemCode, lpszCodes))
	{
		//GameInfof("Code unmatch");
		return FALSE;
	}

	char szExp[EXP_MAX + 1] = "";
	strncpy(szExp, TrimLeft(lpszExpression), EXP_MAX);
	LPSTR pEnd = GetCommentPtr(szExp);
	if (pEnd)
		*pEnd = 0;
	TrimRight(szExp);
	
	if (*szExp == 0)
	{
		SyntaxError("Empty Expression!");
		ASSIGN_PTR(pSyntaxError, TRUE);
		return FALSE;
	}

	if (!VerifyExpression(szExp))
	{
		SyntaxError(lpszExpression);
		ASSIGN_PTR(pSyntaxError, TRUE);
		return FALSE;
	}	

	ITEMPROPERTY aProperties[32];
	const DWORD COUNT = GetItemProperties(dwItemID, aProperties, 32);
	ITEMMOD aMods[32];
	for (DWORD n = 0; n < COUNT; n++)
		TranslateItemMod(&aProperties[n], aProperties, COUNT, &aMods[n]);

	char szSegment[EXP_MAX + 1] = "";
	char Buf[EXP_MAX * INT_MAX_LEN + 1] = "";	
	char szValue[32] = "";
	BOOL bPrevOper = TRUE;
	int nPrevType = CHAR_END;
	int nPars = 0;
	int i = 0;	
	LPCSTR p = szExp;

	while (p = GetFirstSegment(p, szSegment))
	{
		int nValue = 0;
		int nType = GetCharType(*szSegment);
		switch (nType)
		{
		case CHAR_NUMVAR_SYMBOL:
			if (nPrevType == CHAR_NUMVAR_SYMBOL || nPrevType == CHAR_VAR || nPrevType == CHAR_CONST)
			{				
				SyntaxError(szSegment);
				ASSIGN_PTR(pSyntaxError, TRUE);
				return FALSE;
			}
			
			if (!GetNumVarValue(aMods, COUNT, szSegment, nValue))
			{				
				//SyntaxError(szSegment);
				ASSIGN_PTR(pSyntaxError, TRUE);
				return FALSE;
			}

			sprintf(szValue, "%d", nValue);								
			strcat(Buf, szValue);
			for (i = 0; i < nPars; i++)
				strcat(Buf, ")");

			bPrevOper = FALSE;
			nPars = 0;
			break;
			
		case CHAR_VAR:
			if (nPrevType == CHAR_NUMVAR_SYMBOL || nPrevType == CHAR_VAR || nPrevType == CHAR_CONST)
			{
				SyntaxError(szSegment);
				ASSIGN_PTR(pSyntaxError, TRUE);
				return FALSE;
			}

			if (IsSpecialVar(szSegment))
			{
				nValue = GetSpecialValue(dwItemID, szItemCode, aMods, COUNT, szSegment);
			}
			else
			{
				if (!GetModValue(aMods, COUNT, szSegment, nValue))
				{
					//SyntaxError(szSegment);
					ASSIGN_PTR(pSyntaxError, TRUE);
					return FALSE;
				}
			}

			sprintf(szValue, "%d", nValue);								
			strcat(Buf, szValue);
			for (i = 0; i < nPars; i++)
				strcat(Buf, ")");

			bPrevOper = FALSE;
			nPars = 0;
			break;

		case CHAR_OPER:
			if (!strcmp(szSegment, "=")
				|| !strcmp(szSegment, "|")
				|| !strcmp(szSegment, "&"))
			{
				strcat(Buf, szSegment);
			}

			if (bPrevOper)
			{
				// signs "+/-"
				if (!strcmp(szSegment, "+") || !strcmp(szSegment, "-"))
				{
					int nNextType = GetCharType(*p);
					if (nNextType == CHAR_VAR || nNextType == CHAR_CONST)
					{
						if (szSegment[0] == '-')
						{
							nPars++;
							strcat(Buf, "(0-");
						}

						break;
					}
				}

				// not (!), stacking allowed (!!!...!)
				int nNot = IsAllNot(szSegment);
				if (nNot > 0)
				{
					nPars += nNot;
					for (int i = 0; i < nNot; i++)
						strcat(Buf, "(0==");
					break;
				}					
			}

			strcat(Buf, szSegment);
			bPrevOper = TRUE;
			break;

		case CHAR_CONST:
			if (nPrevType == CHAR_NUMVAR_SYMBOL || nPrevType == CHAR_VAR || nPrevType == CHAR_CONST)
			{
				SyntaxError(szSegment);
				ASSIGN_PTR(pSyntaxError, TRUE);
				return FALSE;
			}
			
			strcat(Buf, szSegment);
			for (i = 0; i < nPars; i++)
				strcat(Buf, ")");
			bPrevOper = FALSE;
			nPars = 0;
			break;

		default:
			// shall never happen but just in case...
			SyntaxError(szSegment);
			ASSIGN_PTR(pSyntaxError, TRUE);
			return FALSE;
			break;
		}

		if (*p == 0)
			break; // we've reached the end
		
		nPrevType = nType;
	}

	CSyntax syn;
	int nResult = syn.Parse(Buf);

	if (nResult == PARSING_FAIL)
	{
		ASSIGN_PTR(pSyntaxError, TRUE);
		return FALSE;
	}

	ASSIGN_PTR(pSyntaxError, FALSE);
	return nResult;
}

int IsAllNot(LPCSTR lpszString)
{
	if (lpszString == NULL)
		return 0;

	int nLen = 0;
	for (int i = 0; lpszString[i]; i++)
	{
		if (lpszString[i] != '!')
			return 0;
		nLen++;
	}

	return nLen;
}

// remove white spaces
LPCSTR TrimLeft(LPCSTR lpszString)
{
	if (lpszString == NULL)
		return NULL;

	LPCSTR p = lpszString;
	while (p && strchr(BLANKS, *p))
		p += sizeof(char);

	return p;
}

LPSTR TrimLeft(LPSTR lpszString)
{
	if (lpszString == NULL)
		return NULL;

	LPSTR p = lpszString;
	while (p && strchr(BLANKS, *p))
		p += sizeof(char);

	return p;
}

void TrimRight(LPSTR lpszString)
{
	if (lpszString == NULL)
		return;

	for (int i = strlen(lpszString) - 1; i >= 0; i--)
	{
		if (strchr(BLANKS, lpszString[i]) == NULL)
			break;

		lpszString[i] = 0;
	}
}

LPSTR Trim(LPSTR lpszString)
{
	TrimRight(lpszString);
	return TrimLeft(lpszString);
}

// determines char types
int GetCharType(char ch)
{
	if (ch == '$')
		return CHAR_NUMVAR_SYMBOL;

	if (strchr(BLANKS, ch))
		return CHAR_BLANK;

	if (isdigit(ch))
		return CHAR_CONST;

	if (strchr(OPERS, ch))
		return CHAR_OPER;

	if (isalpha(ch) || ch=='_'  || ch=='[' || ch == ']')
		return CHAR_VAR;

	return CHAR_END;
}

// check exp
BOOL VerifyExpression(LPCSTR lpszExpression)
{
	if (lpszExpression == NULL || *lpszExpression == '\0')
		return FALSE;

	int nPars = 0;
	int nSqs = 0;
	for (int i = 0; lpszExpression[i]; i++)
	{
		if (GetCharType(lpszExpression[i]) == CHAR_END)
			return FALSE;

		if (lpszExpression[i] == '(')
			nPars++;
		else if (lpszExpression[i] == ')')
			nPars--;
		else if (lpszExpression[i] == '[')
			nSqs++;
		else if (lpszExpression[i] == ']')
			nSqs--;
	}

	return nPars == 0 && nSqs == 0;
}

// split exp into segments, including vars, constants, operators, parenthesizes
LPCSTR GetFirstSegment(LPCSTR lpszExp, LPSTR lpszBuffer)
{
	if (lpszExp == NULL || *lpszExp == 0)
		return NULL;

	*lpszBuffer = 0;	
	LPCSTR p = TrimLeft(lpszExp);
	if (p == NULL || *p == 0)
		return NULL;

	const int TYPE_1ST = GetCharType(*p);
	if (TYPE_1ST == CHAR_END)
		return NULL;

	lpszBuffer[0] = *p;
	p++;

	int nCopy = 1;	

	while (TRUE)
	{	
		int nType = GetCharType(*p);
		if (nType == CHAR_END)
			break;
		
		if (TYPE_1ST == CHAR_NUMVAR_SYMBOL)
		{
			 if (*p == '[' || *p == ']' || isdigit(*p))
			 {
				lpszBuffer[nCopy++] = *p;
				p++;
			 }
			 else
			 {
				 break;
			 }
		}
		else
		{
			if (nType == TYPE_1ST || (nType == CHAR_CONST && TYPE_1ST == CHAR_VAR))
			{
				char ch = *p;
				lpszBuffer[nCopy++] = ch;
				p++;
				if (ch != '!' && *p == '!')
					break;
			}	
			else
			{
				break;
			}
		}		
	}

	lpszBuffer[nCopy] = '\0';
	return p;
}

BOOL IsSpecialVar(LPCSTR lpszVar)
{
	if (lpszVar == NULL)
		return FALSE;

	for (int i = 0; SPECIAL_VARS[i]; i++)
	{
		if (!::stricmp(SPECIAL_VARS[i], lpszVar))
			return TRUE;
	}

	return FALSE;
}

int FindSpecialValue(LPCITEMMOD lpMod, DWORD dwModCount, DWORD dwModID, int nMatch)
{
	if (lpMod == NULL)
		return 0;

	for (DWORD i = 0; i < dwModCount; i++)
	{
		if (lpMod[i].dwModID == dwModID && lpMod[i].aValues[1] == nMatch)
			return lpMod[i].aValues[0];
	}
	return 0;
}

int GetSpecialValue(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount, LPCSTR lpszSpecial)
{
	if (lpszSpecial == NULL)
		return 0;

	GAMEUNIT gu = { dwItemID, UNIT_TYPE_ITEM };

	if (!::stricmp(lpszSpecial, "quality"))
	{
		return GetItemQuality(dwItemID);
	}
	if (!::stricmp(lpszSpecial, "quantity"))
	{		
		return GetUnitStat(&gu, STAT_AMMOQUANTITY);
	}
	else if (!::stricmp(lpszSpecial, "sockets"))
	{
		return GetItemSockets(dwItemID);
	}
	else if (!::stricmp(lpszSpecial, "ethereal"))
	{
		return (GetItemAttributes(dwItemID) & ITEM_ETHEREAL) != 0;
	}
	else if (!::stricmp(lpszSpecial, "identified"))
	{
		return (GetItemAttributes(dwItemID) & ITEM_IDENTIFIED) != 0;
	}
	else if (!::stricmp(lpszSpecial, "value"))
	{
		if (::stricmp(lpszItemCode, "gld"))
		{
			return GetItemPrice(dwItemID, 513, FALSE);
		}
		else
		{
			return GetUnitStat(&gu, STAT_GOLD);
		}			
	}
	else if (!::stricmp(lpszSpecial, "all_res"))
	{
		MOD md = { 0 };		
		int nMinRes = 65535;
		int nRes = 0;

		md.dwModID = 39;
		nRes = GetModValue(lpMod, dwModCount, &md, 0);
		nMinRes = min(nMinRes, nRes);

		md.dwModID = 41;
		nRes = GetModValue(lpMod, dwModCount, &md, 0);
		nMinRes = min(nMinRes, nRes);

		md.dwModID = 43;
		nRes = GetModValue(lpMod, dwModCount, &md, 0);
		nMinRes = min(nMinRes, nRes);

		md.dwModID = 45;
		nRes = GetModValue(lpMod, dwModCount, &md, 0);
		nMinRes = min(nMinRes, nRes);
		
		return nMinRes;
	}
	else if (!::stricmp(lpszSpecial, "level"))
	{
		return (int)GetItemLevel(dwItemID);
	}
	
	return 0;
}

BOOL GetNumVarValue(LPCITEMMOD lpMod, DWORD dwModCount, LPSTR lpszSegment, int& rVal)
{
	rVal = 0;
	if (lpMod == NULL || dwModCount == 0 || lpszSegment == NULL)
		return TRUE;

	int nIndex = GetIndex(lpszSegment + 1); // ignore the leading '$'
	if (nIndex == -1)
		return FALSE;

	MOD mod = {0};
	mod.dwModID = strtoul(lpszSegment + 1, NULL, 10);

	rVal = GetModValue(lpMod, dwModCount, &mod, nIndex);
	//GameInfof("lpszSegment %s, mod.dwModID %d, rVal %d", lpszSegment, mod.dwModID, rVal);
	return TRUE;
}

BOOL GetModValue(LPCITEMMOD lpMod, DWORD dwModCount, LPSTR lpszSegment, int& rVal)
{
	rVal = 0;
	if (lpMod == NULL || dwModCount == 0 || lpszSegment == NULL)
		return TRUE;

	int nIndex = GetIndex(lpszSegment);
	if (nIndex == -1)
		return FALSE;

	const MOD* p = FindModDef(lpszSegment);
	if (p == NULL)
	{
		//SyntaxError(lpszSegment);
		return FALSE;
	}

	rVal = GetModValue(lpMod, dwModCount, p, nIndex);
	//GameInfof("lpszSegment %s, dwModID %d, %d, %d, %d, %d", lpszSegment, p->dwModID, p->aValues[0], p->aValues[1], p->aValues[2], p->aValues[3]);
	return TRUE;
}

void StrnCpyTrim(LPSTR lpszBuffer, LPCSTR lpszStart, LPCSTR lpszEnd, int nLimit)
{
	if (lpszBuffer == NULL)
		return;

	*lpszBuffer = 0;
	if (nLimit < 1 || lpszStart == NULL)
		return;
	
	lpszStart = TrimLeft(lpszStart);
	if (lpszEnd && lpszEnd <= lpszStart)
		return;	

	int nCopy = lpszEnd ? lpszEnd - lpszStart : strlen(lpszStart);
	nCopy = min(nCopy, nLimit);
	if (nCopy < 1)
		return;

	strncpy(lpszBuffer, lpszStart, nCopy);
	lpszBuffer[nCopy] = 0;
	
	for (int i = nCopy - 1; i >= 0; i--)
	{
		if (strchr(BLANKS, lpszBuffer[i]) == NULL)
			break;

		lpszBuffer[i] = 0;
	}
}

BOOL IsAllDigit(LPCSTR lpsz)
{
	if (lpsz == NULL)
		return FALSE;

	for (int i = 0; lpsz[i]; i++)
	{
		if (!isdigit(lpsz[i]))
			return FALSE;
	}

	return TRUE;
}

BOOL ParseModDef(LPCSTR lpszModDef, MOD& mod)
{
	memset(&mod, 0, sizeof(MOD));
	if (lpszModDef == NULL || *lpszModDef == '\0')
		return FALSE;

	// moddef: nnn,?,?,?,?
	LPCSTR pStart = lpszModDef;
	LPCSTR pEnd = strchr(lpszModDef, ',');
	
	if (pEnd == NULL)
	{
		if (!IsAllDigit(lpszModDef))
			return FALSE;
		
		mod.dwModID = strtoul(lpszModDef, NULL, 10);
		return TRUE;
	}

	char szSeg[128] = "";	

	StrnCpyTrim(szSeg, pStart, pEnd, 127);
	if (!IsAllDigit(szSeg))
		return FALSE;
	
	mod.dwModID = strtoul(szSeg, NULL, 10);	

	for (int i = 0; i < 3; i++)
	{
		pStart = pEnd + sizeof(char);
		pEnd = strchr(pStart, ',');
		StrnCpyTrim(szSeg, pStart, pEnd, 127);
		if (IsAllDigit(szSeg))
		{
			mod.aValues[i] = atoi(szSeg);
			mod.aMatchs[i] = 1;
		}

		if (pEnd == NULL)
			break;
	}

	return TRUE;
}

BOOL CALLBACK EnumModProc(LPCTSTR lpKeyName, LPCTSTR lpValue, LPVOID lpParam)
{
	if (lpKeyName && lpValue && *lpKeyName && *lpValue)
	{
		LPCSTR p = NULL;
		char* psz = new char[strlen(lpValue) + 1];
		p = GetCommentPtr(lpValue);
		StrnCpyTrim(psz, lpValue, p, 255);
		
		MOD_DEF md = { 0 };
		if (ParseModDef(psz, md.mod))
		{
			p = GetCommentPtr(lpKeyName);
			StrnCpyTrim(md.szModDef, lpKeyName, p, 255);
			g_aModDefs.Add(md);
		}

		delete [] psz;
	}

	return TRUE;
}

void LoadTypeModDefinition(LPCSTR lpszIniFile)
{
	g_aModDefs.RemoveAll();
	CIni ini(lpszIniFile);
	ini.EnumKeys("mod", EnumModProc, 0);
	
	// Sort to enable binary fast search!
	g_aModDefs.Sort();
}

void DebugModDef()
{	
	char sz[4][64];
	for (int i = 0; i < g_aModDefs.GetSize(); i++)
	{
		const MOD_DEF& md = g_aModDefs.GetAt(i);
		for (int j = 0; j < 4; j++)
		{
			if (!md.mod.aMatchs[j])
				strcpy(sz[j], "?");
			else
				sprintf(sz[j], "%d", md.mod.aValues[j]);
		}

		GameInfof("%s: %d - %s,%s,%s,%s", md.szModDef, md.mod.dwModID, sz[0], sz[1], sz[2], sz[3]);
	}
}

const MOD* FindModDef(LPCSTR lpszMod)
{
	if (lpszMod == NULL || *lpszMod == 0)
		return NULL;

	MOD_DEF temp = { 0 };
	strncpy(temp.szModDef, lpszMod, 255);
	int nIdx = g_aModDefs.Find(temp);
	if (nIdx == -1)
	{
		char szErr[256] = "";
		sprintf(szErr, "ÿc4Undefined mod alias - 'ÿc0 %s ÿc4'", lpszMod);
		SyntaxError(szErr);
		return NULL;
	}

	const MOD_DEF& md = g_aModDefs.GetAt(nIdx);
	return &md.mod;
}

int GetModValue(LPCITEMMOD lpMod, DWORD dwModCount, const MOD* pMod, int nIndex)
{
	if (lpMod == NULL || dwModCount == 0 || pMod == NULL)
		return 0;

	if (nIndex < 0 || nIndex > 3)
		nIndex = 0;

	for (DWORD i = 0; i < dwModCount; i++)
	{
		if (ModMatch(lpMod[i], pMod))
			return lpMod[i].aValues[nIndex];
	}

	return 0;
}

BOOL ModMatch(const ITEMMOD& im, const MOD* pMod)
{	
	if (pMod == NULL)
		return FALSE;

	if (im.dwModID != pMod->dwModID && im.dwOriginalID != pMod->dwModID)
		return FALSE;

	for (int i = 0; i < 4; i++)
	{
		if (pMod->aMatchs[i] && im.aValues[i] != pMod->aValues[i])
			return FALSE;
	}
	
	return TRUE;
}

BOOL CodeMatch(LPCSTR lpszItemCode, LPCSTR lpszCodes)
{
	if (lpszItemCode == NULL || *lpszItemCode == 0 || strlen(lpszItemCode) != 3)
		return FALSE;

	if (lpszCodes == NULL || *lpszCodes == 0)
		return TRUE; // code not care

	LPSTR pszCodes = strdup(lpszCodes);
	LPSTR p = (LPSTR)GetCommentPtr(pszCodes);
	if (p)
		*p = 0;

	TrimRight(pszCodes);
	
	BOOL bMatch = !::strcmp(pszCodes, "*") || ::strstr(pszCodes, lpszItemCode);
	free(pszCodes);
	return bMatch;
}

void SyntaxError(LPCSTR lpszError)
{
	if (lpszError)
		GameErrorf("ÿc4Syntax Errorÿc1::ÿc0 %s", lpszError);
}

LPSTR GetCommentPtr(LPCSTR lpszString)
{
	if (lpszString == NULL)
		return NULL;

	for (int i = 0; COMMENTS[i]; i++)
	{
		LPSTR p = strstr(lpszString, COMMENTS[i]);
		if (p)
			return p;
	}

	return NULL;
}

int GetIndex(LPSTR lpszString)
{
	// verify 
	if (lpszString == NULL)
		return -1;

	int i = 0;
	int nSq1 = -1;
	int nSq2 = -1;
	for (i = 0; lpszString[i]; i++)
	{
		if (lpszString[i] == '[')
		{
			if (nSq1 != -1)
			{
				SyntaxError(lpszString);
				return -1;
			}
			nSq1 = i;
		}
		else if (lpszString[i] == ']')
		{
			if (nSq1 == -1 || nSq2 != -1)
			{
				SyntaxError(lpszString);
				return -1;
			}
			nSq2 = i;
		}
	}

	if (nSq1 == -1 && nSq2 == -1)
		return 0;

	if (nSq1 <= 0 || nSq2 < nSq1 + 2)
	{
		SyntaxError(lpszString);
		return -1;
	}

	LPSTR p = TrimLeft(lpszString + nSq1 + 1);
	char ch = 0;
	for (i = 0; p[i] && p[i] != ']'; i++)
	{
		if (ch || (!isdigit(p[i]) && !strchr(BLANKS, p[i])))
		{
			SyntaxError(lpszString);
			return -1;
		}
		ch = p[i];
	}

	int nIndex = (int)ch - (int)'0';

	if (nIndex < 0 || nIndex > 3)
	{
		SyntaxError(lpszString);
		return -1;
	}

	lpszString[nSq1] = 0;
	TrimRight(lpszString);
	return nIndex;
}