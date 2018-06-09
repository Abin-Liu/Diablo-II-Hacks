#ifndef __CODEDEF_H__
#define __CODEDEF_H__

#include <windows.h>

struct ITEMENTRY
{
	char szCode[4];
	char szName[64];
};

struct ITEMENTRY1
{
	const ITEMENTRY* pEntry;
};

struct ITEMENTRY2
{
	const ITEMENTRY* pEntry;
};

BOOL operator==(const ITEMENTRY1& lhs, const ITEMENTRY1& rhs);
BOOL operator>(const ITEMENTRY1& lhs, const ITEMENTRY1& rhs);
BOOL operator==(const ITEMENTRY2& lhs, const ITEMENTRY2& rhs);
BOOL operator>(const ITEMENTRY2& lhs, const ITEMENTRY2& rhs);

const ITEMENTRY* GetEntry(int nIndex);

#endif