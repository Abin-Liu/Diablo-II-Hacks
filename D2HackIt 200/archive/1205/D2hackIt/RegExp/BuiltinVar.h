//////////////////////////////////////////////////////////////////////////
// BuiltinVar.h
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////////

#ifndef __BUILTINVAR_H__
#define __BUILTINVAR_H__

#include <windows.h>
#include "ModDef.h"
#include "..\\..\\Includes\\Utilities\\ArrayEx.h"

typedef int (*fnGetBuiltinValue)(DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount);

typedef struct tagBuiltinVar
{
	char szName[64]; // built-in varaible name
	fnGetBuiltinValue GetBuiltinValue;	// function pointer
	int nConstant; // constant
} BUILTINVAR, *LPBUILTINVAR;
typedef const tagBuiltinVar* LPCBUILTINVAR;

BOOL operator==(const BUILTINVAR& lhs, const BUILTINVAR& rhs);
BOOL operator>(const BUILTINVAR& lhs, const BUILTINVAR& rhs);

class CBuiltinVarDictionary
{
public:

	CBuiltinVarDictionary();
	LPCBUILTINVAR Lookup(LPCSTR lpszVarName) const;
	BOOL Lookup(LPCSTR lpszVarName, DWORD dwItemID, LPCSTR lpszItemCode, LPCITEMMOD lpMod, DWORD dwModCount, int& rValue) const;

private:

	CArrayEx<BUILTINVAR, const BUILTINVAR&> m_aVars;
	
};

#endif // __BUILTINVAR_H__