#include "ShopbotPathDef.h"

int SBGetPathCount(LPCD2POSITION lpPos)
{
	if (lpPos == NULL)
		return 0;

	int nCount = 0;
	for (int i = 0; i < 64 && lpPos[i].x != 0; i++)
		nCount++;
	return nCount;
}
