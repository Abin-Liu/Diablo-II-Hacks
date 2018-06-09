////////////////////////////////////////////////////////////////////////////
// ExamItemProperties.cpp
//
// Abin (abinn32@yahoo.com)
////////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include "D2Hackit.h"
#include "syntax.H"
#include "..\\..\\Includes\\Utilities\\StringEx.h"
#include "..\\D2Stdio.h"
#include "ModDef.h"
#include "BuiltinVar.h"

#define ASSIGN_PTR(p,v)		((p) ? *(p)=v : NULL)
const static char OPERS[] = "()+-*/><=!&|"; // operators
enum { CHAR_END = 0, CHAR_BLANK, CHAR_VAR, CHAR_CONST, CHAR_NUMVAR_SYMBOL, CHAR_OPER };

CModDictionary g_modDictionary; // Item mod alias dictionary
//CTypeDictionary g_typeDictionary; // Item type dictionary

BOOL TranslateItemMod(LPCITEMPROPERTY lpProperty, LPCITEMPROPERTY aList, DWORD dwCount, LPITEMMOD lpBuffer); // translateitemmod.cpp
LPCSTR SkipBlanks(LPCSTR lpszString);
int GetCharType(char ch);
BOOL VerifyExpression(LPCSTR lpszExpression);
BOOL CodeMatch(LPCSTR lpszItemCode, LPCSTR lpszCodeOrType);
LPCSTR GetFirstSegment(LPCSTR lpszExp, CStringEx& sSegment);

/////////////////////////////////////////////////////////////////////
// ExamItemProperties
//-------------------------------------------------------------------
// Exam an item with a regular expression string.
// Abin (abinn32@yahoo.com)
/////////////////////////////////////////////////////////////////////
BOOL EXPORT ExamItemProperties(DWORD dwItemID, LPCSTR lpszExpression, LPCSTR lpszCodes, BOOL* pSyntaxError)
{
	ASSIGN_PTR(pSyntaxError, FALSE);

	CStringEx sExp(lpszExpression);	
	RemoveComments(sExp);
	sExp.Trim();

	if (sExp.IsEmpty())
	{
		GamePrintError("Syntax error: Empty Expression!");
		ASSIGN_PTR(pSyntaxError, TRUE);
		return FALSE;
	}

	if (!VerifyExpression(sExp))
	{
		GameErrorf("Syntax error: ÿc1%s", (LPCSTR)sExp);
		ASSIGN_PTR(pSyntaxError, TRUE);
		return FALSE;
	}	

	sExp.MakeLower();

	// veryfy item
	char szItemCode[4] = "";
	if (!GetItemCode(dwItemID, szItemCode, 3))
	{
		ItemError(dwItemID, "ExamItemProperties");		
		return FALSE;
	}

	::strlwr(szItemCode);
	if (!CodeMatch(szItemCode, lpszCodes))
		return FALSE;
	
	ITEMPROPERTY aProperties[32];
	const DWORD COUNT = GetItemProperties(dwItemID, aProperties, 32);
	ITEMMOD aMods[32];
	for (DWORD n = 0; n < COUNT; n++)
		TranslateItemMod(&aProperties[n], aProperties, COUNT, &aMods[n]);

	CStringEx sSegment;
	CStringEx sBuf;
	CStringEx sValue;

	BOOL bPrevOper = TRUE;
	int nPrevType = CHAR_END;
	int nPars = 0;
	int i = 0;	

	for (LPCSTR p = GetFirstSegment(sExp, sSegment); p && sSegment.GetLength(); p = GetFirstSegment(p, sSegment))
	{
		int nValue = 0;
		int nType = GetCharType(sSegment[0]);
		switch (nType)
		{
		case CHAR_NUMVAR_SYMBOL:
		case CHAR_VAR:
		case CHAR_CONST:
			if (nPrevType == CHAR_NUMVAR_SYMBOL || nPrevType == CHAR_VAR || nPrevType == CHAR_CONST)
			{				
				GameErrorf("Syntax error: ÿc1%s", (LPCSTR)sSegment);
				ASSIGN_PTR(pSyntaxError, TRUE);
				return FALSE;
			}
			
			if (nType != CHAR_CONST)
			{
				if (!g_modDictionary.Lookup(sSegment, dwItemID, szItemCode, aMods, COUNT, nValue))
				{				
					ASSIGN_PTR(pSyntaxError, TRUE);
					return FALSE;
				}

				sValue.Format("%d", nValue);
			}
			else
			{
				sValue = sSegment;
			}
			
			sBuf += sValue;
			sBuf.Append(')', nPars);
			bPrevOper = FALSE;
			nPars = 0;
			break;

		case CHAR_OPER:
			if (sSegment == '=' || sSegment == '|' || sSegment == '&')
				sSegment += sSegment;

			if (bPrevOper)
			{
				// signs "+/-"
				if (sSegment == '+' || sSegment == '-')
				{
					int nNextType = GetCharType(*p);
					if (nNextType == CHAR_VAR || nNextType == CHAR_CONST)
					{
						if (sSegment[0] == '-')
						{
							nPars++;
							sBuf += "(0-";
						}

						break;
					}
				}

				// not (!), stacking allowed (!!!...!)
				if (sSegment.FindNoneOf("!") == -1)
				{
					nPars += sSegment.GetLength();
					for (int i = 0; i < sSegment.GetLength(); i++)
						sBuf += "(0==";
					break;
				}					
			}

			sBuf += sSegment;
			bPrevOper = TRUE;
			break;

		default:
			// shall never happen but just in case...
			GameErrorf("Syntax error: ÿc1%s", (LPCSTR)sSegment);
			ASSIGN_PTR(pSyntaxError, TRUE);
			return FALSE;
			break;
		}

		if (*p == 0)
			break; // we've reached the end
		
		nPrevType = nType;
	}

	//GameInfof("buffer: %s", (LPCTSTR)sBuf);
	if (sBuf.IsEmpty())
	{
		ASSIGN_PTR(pSyntaxError, TRUE);
		return FALSE;
	}

	CSyntax syn;
	int nResult = 0;
	if (!syn.Parse(sBuf.GetBuffer(sBuf.GetLength()), nResult))
	{
		GameErrorf("Syntax error: ÿc1%s", lpszExpression);
		ASSIGN_PTR(pSyntaxError, TRUE);
		return FALSE;
	}

	ASSIGN_PTR(pSyntaxError, FALSE);
	return nResult;
}

// skip white spaces
LPCSTR SkipBlanks(LPCSTR lpszString)
{
	if (lpszString == NULL)
		return NULL;

	LPCSTR p = lpszString;
	while (p && CStringEx::IsBlank(*p))
		p += sizeof(char);

	return p;
}

// determines char types
int GetCharType(char ch)
{
	if (ch == '$')
		return CHAR_NUMVAR_SYMBOL;

	if (CStringEx::IsBlank(ch))
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
LPCSTR GetFirstSegment(LPCSTR lpszExp, CStringEx& sSegment)
{
	sSegment.Empty();

	if (lpszExp == NULL || *lpszExp == 0)
		return NULL;

	LPCSTR p = SkipBlanks(lpszExp);
	if (p == NULL || *p == 0)
		return NULL;

	const int TYPE_1ST = GetCharType(*p);
	if (TYPE_1ST == CHAR_END)
		return NULL;

	sSegment += *p;
	p++;

	while (TRUE)
	{	
		int nType = GetCharType(*p);
		if (nType == CHAR_END)
			break;
		
		if (TYPE_1ST == CHAR_NUMVAR_SYMBOL)
		{
			 if (*p == '[' || *p == ']' || isdigit(*p))
			 {
				sSegment += *p;
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
				sSegment += ch;
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

	return p;
}

void LoadTypeModDefinition(LPCSTR lpszIniFile)
{
	g_modDictionary.Load(lpszIniFile); // load alias dictionary
	//g_typeDictionary.Load(lpszIniFile); // load type dictionary
}

BOOL CodeMatch(LPCSTR lpszItemCode, LPCSTR lpszCodes)
{		
	if (lpszItemCode == NULL)
		return FALSE;

	if (lpszCodes == NULL)
		return TRUE;

	CStringEx sCodes = lpszCodes;
	RemoveComments(sCodes);
	sCodes.Trim();
	sCodes.MakeLower();
	if (sCodes.IsEmpty() || sCodes == "*")
		return TRUE;

	return sCodes.Find(lpszItemCode) != -1;

	/*
	// find in codes first
	CTypeDef td;
	if (!td.SetType("temp", lpszCodes))
		return TRUE;

	if (td.CodeMatch(lpszItemCode))
		return TRUE;

	// try lookup in type dictionary
	for (int i = 0; i < td.GetCodeCount(); i++)
	{
		if (g_typeDictionary.Lookup(td[i], lpszItemCode))
			return TRUE;
	}

	return FALSE;
	*/
}