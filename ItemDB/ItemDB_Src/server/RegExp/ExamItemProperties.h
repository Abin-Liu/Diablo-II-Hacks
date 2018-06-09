#ifndef __EXAMITEMPROPERTIES_H__
#define __EXAMITEMPROPERTIES_H__

#include <windows.h>
#include "ServerItem.h"

void LoadTypeModDefinition(LPCSTR lpszIniFile);
BOOL ExamItemProperties(LPCSERVERITEM lpItem, LPCSTR lpszExpression, BOOL* pSyntaxError = NULL);

#endif // __EXAMITEMPROPERTIES_H__