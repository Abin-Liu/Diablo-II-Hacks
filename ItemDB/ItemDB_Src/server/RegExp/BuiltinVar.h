//////////////////////////////////////////////////////////////////////////
// BuiltinVar.h
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////////

#ifndef __BUILTINVAR_H__
#define __BUILTINVAR_H__

#include <windows.h>
#include "..\\..\\common\\ArrayEx.h"
#include "ServerItem.h"

typedef int (*fnGetBuiltinValue)(LPCSERVERITEM lpItem);

typedef struct tagBuiltinVar
{
	char szName[64]; // built-in varaible name
	fnGetBuiltinValue GetBuiltinValue;	// function pointer
	int nConstant; // constant
} BUILTINVAR, *LPBUILTINVAR;
typedef const tagBuiltinVar* LPCBUILTINVAR;

BOOL operator==(const BUILTINVAR& lhs, const BUILTINVAR& rhs);
BOOL operator>(const BUILTINVAR& lhs, const BUILTINVAR& rhs);
int GetBuiltinVarValue(LPCBUILTINVAR pbv, LPCSERVERITEM lpItem);

class CBuiltinVarDictionary
{
public:

	CBuiltinVarDictionary();
	LPCBUILTINVAR Lookup(LPCSTR lpszVarName) const;
	BOOL Lookup(LPCSTR lpszVarName, LPCSERVERITEM lpItem, int& rValue) const;

private:

	CArrayEx<BUILTINVAR, const BUILTINVAR&> m_aVars;
	
};

#endif // __BUILTINVAR_H__