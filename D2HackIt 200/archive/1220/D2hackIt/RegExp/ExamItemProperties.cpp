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
enum { CHAR_INVALID = 0, CHAR_VAR, CHAR_OPER, CHAR_PAR };

CModDictionary g_modDictionary; // Item mod alias dictionary

BOOL TranslateItemMod(LPCITEMPROPERTY lpProperty, LPCITEMPROPERTY aList, DWORD dwCount, LPITEMMOD lpBuffer); // translateitemmod.cpp
int GetCharType(char ch);
BOOL VerifyExpression(LPCSTR lpszExpression);
BOOL CodeMatch(LPCSTR lpszItemCode, LPCSTR lpszCodeOrType);
BOOL GetFirstSegment(const CStringEx& sExp, int& rStart, CStringEx& sSegment, BOOL& rError);
int PeekNextType(const CStringEx& sExp, int nStart);
BOOL VerifyNot(char ch1, char ch2);

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
	sExp.DeleteInclusive(" \t\r\n"); // wipe out all blank spaces
	sExp.MakeLower();

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
	
	ITEMMOD aProperties[32];
	const DWORD COUNT = GetItemProperties2(dwItemID, aProperties, 32);

	CStringEx sSegment;
	CStringEx sBuf;
	CStringEx sValue;

	int nPrevType = CHAR_INVALID;
	int nPars = 0;
	BOOL bError = FALSE;
	int nStart = 0;

	while (GetFirstSegment(sExp, nStart, sSegment, bError) && !bError)
	{
		//GamePrintInfo(sSegment);
		switch(GetCharType(sSegment[0]))
		{
		case CHAR_VAR:

			if (nPrevType == CHAR_VAR)
			{
				bError = TRUE;
				break;
			}

			if (!sSegment.IsDigit())
			{
				int nValue = 0;
				if (!g_modDictionary.Lookup(sSegment, dwItemID, szItemCode, aProperties, COUNT, nValue))
				{				
					bError = TRUE;
					break;
				}

				sSegment.Format("%d", nValue);
			}

			nPrevType = CHAR_VAR;
			break;

		case CHAR_OPER:

			if (nPrevType != CHAR_VAR && PeekNextType(sExp, nStart) == CHAR_VAR)
			{
				// Unary operators +/-/!

				if (sSegment == '+')
					sSegment.Empty(); // unary +

				if (sSegment == '-')
				{
					nPars++;
					sBuf += "(0-";
					sSegment.Empty();
				}

				// unary not (!), stacking allowed (!!!...!)
				if (sSegment.FindNoneOf("!") == -1)
				{
					for (int i = 0; i < sSegment.GetLength(); i++)
					{
						nPars++;
						sBuf += "(0==";
					}
					sSegment.Empty();
				}
			}

			nPrevType = CHAR_OPER;
			break;

		default:
			break;
		}

		if (bError)
			break;

		sBuf += sSegment;
	}

	if (bError)
	{
		GameErrorf("Syntax error: ÿc1%s", lpszExpression);
		ASSIGN_PTR(pSyntaxError, TRUE);
		return FALSE;
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

// determines char types
int GetCharType(char ch)
{
	if (ch == '(' || ch == ')')
		return CHAR_PAR;

	if (ch == '$' || isdigit(ch) || isalpha(ch) || ch=='_'  || ch=='[' || ch == ']')
		return CHAR_VAR;

	if (strchr("+-*/><=!&|", ch))
		return CHAR_OPER;

	return CHAR_INVALID;
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
		if (GetCharType(lpszExpression[i]) == CHAR_INVALID)
			return FALSE;

		if (lpszExpression[i] == '(')
		{
			nPars++;
		}
		else if (lpszExpression[i] == ')')
		{
			if (nPars)
				nPars--;
			else
				return FALSE;
		}
		else if (lpszExpression[i] == '[')
		{
			nSqs++;
		}
		else if (lpszExpression[i] == ']')
		{
			if (nSqs)
				nSqs--;
			else
				return FALSE;
		}
	}

	return nPars == 0 && nSqs == 0;
}

// split exp into segments, including vars, constants, operators, parenthesizes
BOOL GetFirstSegment(const CStringEx& sExp, int& rStart, CStringEx& sSegment, BOOL& rError)
{
	sSegment.Empty();
	rError = FALSE;
	if (rStart < 0 || rStart >= sExp.GetLength())
		return 0;

	const int TYPE_1ST = GetCharType(sExp[rStart]);
	if (TYPE_1ST == CHAR_INVALID)
	{
		rStart = -1;
		rError = TRUE;
		return 0;
	}

	TCHAR chPrev = sExp[rStart++];
	sSegment += chPrev;

	while (rStart < sExp.GetLength()
		&& GetCharType(sExp[rStart]) == TYPE_1ST
		&& VerifyNot(chPrev, sExp[rStart]))
	{
		chPrev = sExp[rStart++];
		sSegment += chPrev;
	}

	return sSegment.GetLength();
}

void LoadTypeModDefinition(LPCSTR lpszIniFile)
{
	g_modDictionary.Load(lpszIniFile); // load alias dictionary
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
}

int PeekNextType(const CStringEx& sExp, int nStart)
{
	for (int i = nStart; i < sExp.GetLength(); i++)
	{
		int nType = GetCharType(sExp[i]);
		if (nType == CHAR_VAR || nType == CHAR_OPER)
			return nType;
	}

	return CHAR_INVALID;
}

BOOL VerifyNot(char ch1, char ch2)
{
	if (ch1 == '!')
		return ch2 == '!' || ch2 == '=';

	if (ch2 == '!')
		return ch1 == '!';

	return TRUE;
}