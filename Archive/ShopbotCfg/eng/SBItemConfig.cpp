#include "stdafx.h"
#include "SBItemConfig.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "StringParse.h"
#include "Ini.h"

BOOL ModMatch(LPCITEMMOD lpMod1, LPCITEMMOD lpMod2)
{
	if (lpMod1 == NULL || lpMod2 == NULL || lpMod1->dwModID != lpMod2->dwModID)
		return FALSE;

	// value[0]
	if (lpMod1->dwModID == 188 || lpMod1->dwModID == 83 || lpMod1->dwModID == 107)
	{
		// skill
		if (lpMod1->aValues[0] != lpMod2->aValues[0])
			return FALSE;
	}
	else if (lpMod1->dwModID == 91) // -requirements
	{
		// it's a negative number
		if (0 - lpMod1->aValues[0] < 0 - lpMod2->aValues[0])
			return FALSE;
	}
	else
	{
		if (lpMod1->aValues[0] < lpMod2->aValues[0])
			return FALSE;
	}

	// value[1]
	if (lpMod1->dwModID == 195 || lpMod1->dwModID == 198 || lpMod1->dwModID == 201 || lpMod1->dwModID == 204)
	{
		// skill
		if (lpMod1->aValues[1] != lpMod2->aValues[1])
			return FALSE;
	}
	else
	{
		if (lpMod1->aValues[1] < lpMod2->aValues[1])
			return FALSE;
	}

	// value[2], value[3]
	return lpMod1->aValues[2] >= lpMod2->aValues[2] && lpMod1->aValues[3] >= lpMod2->aValues[3];
}

BOOL CheckModInItem(LPCITEM lpItem, LPCITEMMOD lpMod)
{
	if (lpItem == NULL || lpMod == NULL)
		return FALSE;

	// check lpItem's each mod
	for (BYTE i = 0; i < lpItem->iModCount; i++)
	{
		if (ModMatch(&lpItem->aMods[i], lpMod))
			return TRUE;
	}

	return FALSE;
}

BOOL SBVerifyItemMods(LPCITEM lpItem, LPCSBITEMCONFIG lpConfig)
{
	if (lpItem == NULL || lpConfig == NULL)
		return FALSE;

	if (lpConfig->szItemCode[0] && !strstr(lpConfig->szItemCode, lpItem->szItemCode))
		return FALSE;

	if (lpItem->iSocketNumber < lpConfig->iSockets)
		return FALSE;

	if (lpConfig->bNonMagic && lpItem->iQuality == ITEM_LEVEL_MAGIC)
		return FALSE;

	for (BYTE i = 0; i < lpConfig->iModNum; i++)
	{
		if (!CheckModInItem(lpItem, &lpConfig->aMods[i]))
			return FALSE;
	}

	return TRUE;
}

BOOL SBLoadGlobal(LPCSTR lpszFile, LPSBGLOBAL lpBuffer)
{
	if (lpszFile == NULL || lpBuffer == NULL)
		return FALSE;

	CIni ini(lpszFile);
	lpBuffer->iWP = (BYTE)ini.GetUInt("Global", "WP", 0x81);
	lpBuffer->wNPC = (WORD)ini.GetUInt("Global", "NPC", 0x01ff);
	lpBuffer->iAct = min(4, (BYTE)ini.GetUInt("Global", "Act", 4));
	lpBuffer->iStopHP = min(100, (BYTE)ini.GetUInt("Global", "Stop HP", 90));
	lpBuffer->iHotkey = (BYTE)ini.GetUInt("Global", "Hotkey", VK_F12);
	if (lpBuffer->iHotkey < VK_F1 || lpBuffer->iHotkey > VK_F12)
		lpBuffer->iHotkey = VK_F12;	
	return TRUE;
}

BOOL SBSaveGlobal(LPCSTR lpszFile, LPCSBGLOBAL lpGlobal)
{
	if (lpszFile == NULL || lpGlobal == NULL)
		return FALSE;

	CIni ini(lpszFile);
	ini.WriteUInt("Global", "WP", lpGlobal->iWP);
	ini.WriteUInt("Global", "NPC", lpGlobal->wNPC);
	ini.WriteUInt("Global", "Act", lpGlobal->iAct);
	ini.WriteUInt("Global", "Stop HP", lpGlobal->iStopHP);
	ini.WriteUInt("Global", "Hotkey", lpGlobal->iHotkey);
	return TRUE;
}

BOOL SBLoadConfig(LPCSTR lpszFile, LPCSTR lpszSection, LPSBITEMCONFIG lpBuffer)
{
	if (lpszFile == NULL || lpszSection == NULL || lpBuffer == NULL || !stricmp(lpszSection, "Global"))
		return FALSE;

	::memset(lpBuffer, 0, sizeof(SBITEMCONFIG));

	CIni ini(lpszFile);
	strncpy(lpBuffer->szDescription, lpszSection, SB_MAX_DESCRIPTION);
	ini.GetString(lpszSection, "Code", lpBuffer->szItemCode, SB_MAX_DESCRIPTION);
	lpBuffer->iSockets = (BYTE)ini.GetUInt(lpszSection, "Sockets", 0);
	lpBuffer->bNonMagic = ini.GetBool(lpszSection, "NonMagic", FALSE);

	char* pszKeys = new char[1026];
	::memset(pszKeys, 0, 1026);
	ini.GetKeyNames(lpszSection, pszKeys, 1024);

	for (char* pKey = pszKeys; pKey[0] != '\0'; pKey = &pKey[strlen(pKey) + 1])
	{	
		if (!stricmp(pKey, "Code") || !stricmp(pKey, "Sockets"))
			continue;

		lpBuffer->aMods[lpBuffer->iModNum].dwModID  = strtoul(pKey, NULL, 10);
		char szVals[256] = "";
		if (!ini.GetString(lpszSection, pKey, szVals, 255))
			continue;

		CStringParse sp(szVals, ",");
		POSITION pos = sp.GetStartPosition();
		
		if (lpBuffer->aMods[lpBuffer->iModNum].dwModID == 107)
		{
			// single skill, multiple "107" keys may exist
			while (pos && lpBuffer->iModNum < SB_MAX_MOD)
			{
				char sz1[16] = "";
				char sz2[16] = "";
				sp.GetNextString(pos, sz1, 15);
				sp.GetNextString(pos, sz2, 15);
				lpBuffer->aMods[lpBuffer->iModNum].dwModID = 107;
				lpBuffer->aMods[lpBuffer->iModNum].iCount = 2;
				lpBuffer->aMods[lpBuffer->iModNum].aValues[0] = strtoul(sz1, NULL, 10);
				lpBuffer->aMods[lpBuffer->iModNum].aValues[1] = strtoul(sz2, NULL, 10);
				lpBuffer->iModNum++;				
			}
		}
		else
		{
			while (pos && lpBuffer->iModNum < 4)
			{
				char szMod[32] = "";
				sp.GetNextString(pos, szMod, 31);							
				lpBuffer->aMods[lpBuffer->iModNum].aValues[lpBuffer->aMods[lpBuffer->iModNum].iCount] = strtoul(szMod, NULL, 10);
				lpBuffer->aMods[lpBuffer->iModNum].iCount++;
			}
			lpBuffer->iModNum++;
		}
	}

	delete [] pszKeys;
	return TRUE;
}

BOOL SBSaveConfig(LPCSTR lpszFile, LPCSBITEMCONFIG lpConfig)
{
	if (lpszFile == NULL || lpConfig == NULL)
		return FALSE;

	CIni ini(lpszFile);
	ini.WriteString(lpConfig->szDescription, "Code", lpConfig->szItemCode);
	ini.WriteUInt(lpConfig->szDescription, "Sockets", lpConfig->iSockets);
	ini.WriteBool(lpConfig->szDescription, "NonMagic", lpConfig->bNonMagic);

	for (BYTE i = 0; i < lpConfig->iModNum; i++)
	{
		char szModID[32] = "";
		sprintf(szModID, "%u", lpConfig->aMods[i].dwModID);
		char szVals[128] = "";
		
		for (BYTE v = 0; v < lpConfig->aMods[i].iCount; v++)
			sprintf(szVals, "%s%u,", szVals, lpConfig->aMods[i].aValues[v]);

		if (lpConfig->aMods[i].dwModID == 107)
		{
			ini.AppendString(lpConfig->szDescription, szModID, szVals);
		}
		else
		{
			const int LEN = strlen(szVals);
			if (LEN > 1 && szVals[LEN - 1] == ',')
				szVals[LEN - 1] = 0;			
			ini.WriteString(lpConfig->szDescription, szModID, szVals);
		}		
	}

	return TRUE;
}
