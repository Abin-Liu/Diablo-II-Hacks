//#include "stdafx.h"
#include "item.h"
#include "BitFields.h"
#include "properties.h"
#include "itemsize.h"

///////////////////////////////////////////////
// Item Mod Parsing, by Ackmed or Nuttzy
///////////////////////////////////////////////
BOOL __D2ParseMods(CBitFields &iPacket, LPITEM lpItem)
{
	if (!lpItem)
		return FALSE;

	int temp;
	lpItem->iModCount = 0;

	do
	{
		LPITEMMOD p= &lpItem->aMods[lpItem->iModCount];
		p->dwModID = iPacket.GetField(9);

		if(lpItem->iModCount > 0)
		{
			if(p->dwModID < lpItem->aMods[lpItem->iModCount - 1].dwModID) 
			{
				lpItem->iModCount = 0;
				return FALSE;
			}
		}

		if (p->dwModID == 511) 
			return TRUE;

		const D2_PROPERTIE* prop = GetPropertiesRow(p->dwModID);
		if(!prop)
			break;

		for (BYTE i = 0; i < prop->numFields; i++)
		{
			p->aValues[i] = iPacket.GetField(prop->bits[i]);
			p->aValues[i] -= prop->bias;
		}

		switch(p->dwModID)
		{
		case 48:
		case 50:
		case 52:
		case 54:
			p->iCount = 1;
			p->aValues[0] += p->aValues[1];
			break;

		case 57:
			p->iCount = 2;
			temp = (p->aValues[0] + p->aValues[1]) / 2;
			p->aValues[0] = (DWORD)(p->aValues[2] * 0.04);
			p->aValues[1] = temp * p->aValues[0] / 10;
			break;
			
		default:
			p->iCount = prop->numFields;
			break;
		}
		
		lpItem->iModCount++;
	}while(lpItem->iModCount < ITEM_MAX_MOD && iPacket.GetPos() + 9 < iPacket.GetMaxBits());

	return FALSE;
}

SIZE D2GetItemSize(LPCTSTR lpszItemCode)
{
	SIZE val = { 0 };
	if (lpszItemCode == NULL)
		return val;

	for (int i = 0; ITEM_SIZE_TB[i].code; i++)
	{
		if (stricmp(lpszItemCode, ITEM_SIZE_TB[i].code) == 0)
		{
			val.cx = ITEM_SIZE_TB[i].x;
			val.cy = ITEM_SIZE_TB[i].y;
			return val;
		}
	}

	return val;
}

BOOL IsPotion(LPCITEM lpItem)
{
	if (!lpItem)
		return FALSE;

	return !stricmp(lpItem->szItemCode, "rvl")
		|| !stricmp(lpItem->szItemCode, "rvs")

		|| !stricmp(lpItem->szItemCode, "hp1")
		|| !stricmp(lpItem->szItemCode, "hp2")
		|| !stricmp(lpItem->szItemCode, "hp3")
		|| !stricmp(lpItem->szItemCode, "hp4")
		|| !stricmp(lpItem->szItemCode, "hp5")

		|| !stricmp(lpItem->szItemCode, "mp1")
		|| !stricmp(lpItem->szItemCode, "mp2")
		|| !stricmp(lpItem->szItemCode, "mp3")
		|| !stricmp(lpItem->szItemCode, "mp4")
		|| !stricmp(lpItem->szItemCode, "mp5");
}

int D2IsPotion(LPCSTR lpszItemCode)
{
	if (lpszItemCode == NULL)
		return POTION_UNKNOWN;
	
	if (!stricmp(lpszItemCode, "rvl")
		|| !stricmp(lpszItemCode, "rvs"))
		return POTION_PURPLE;

	if (!stricmp(lpszItemCode, "hp1")
		|| !stricmp(lpszItemCode, "hp2")
		|| !stricmp(lpszItemCode, "hp3")
		|| !stricmp(lpszItemCode, "hp4")
		|| !stricmp(lpszItemCode, "hp5"))
		return POTION_RED;

	if (!stricmp(lpszItemCode, "mp1")
		|| !stricmp(lpszItemCode, "mp2")
		|| !stricmp(lpszItemCode, "mp3")
		|| !stricmp(lpszItemCode, "mp4")
		|| !stricmp(lpszItemCode, "mp5"))
		return POTION_BLUE;

	if (!stricmp(lpszItemCode, "yps")
		|| !stricmp(lpszItemCode, "vps")
		|| !stricmp(lpszItemCode, "wms"))
		return POTION_JUNK;

	return POTION_UNKNOWN;
}

BOOL D2IsThrowItem(LPCSTR szItemCode)
{
	if (szItemCode == NULL)
		return FALSE;
	
	return !stricmp(szItemCode, "jav")
		|| !stricmp(szItemCode, "9ja")
		|| !stricmp(szItemCode, "7ja")

		|| !stricmp(szItemCode, "tax")
		|| !stricmp(szItemCode, "9ta")
		|| !stricmp(szItemCode, "7ta")

		|| !stricmp(szItemCode, "tkf")
		|| !stricmp(szItemCode, "9tk")
		|| !stricmp(szItemCode, "7tk")

		|| !stricmp(szItemCode, "am5")
		|| !stricmp(szItemCode, "ama")
		|| !stricmp(szItemCode, "amf")

		|| !stricmp(szItemCode, "pil")
		|| !stricmp(szItemCode, "9pi")
		|| !stricmp(szItemCode, "7pi")

		|| !stricmp(szItemCode, "bkf")
		|| !stricmp(szItemCode, "9bk")
		|| !stricmp(szItemCode, "7bk")

		|| !stricmp(szItemCode, "bal")
		|| !stricmp(szItemCode, "9b8")
		|| !stricmp(szItemCode, "7b8")

		|| !stricmp(szItemCode, "glv")
		|| !stricmp(szItemCode, "9gl")
		|| !stricmp(szItemCode, "7gl")

		|| !stricmp(szItemCode, "tsp")
		|| !stricmp(szItemCode, "9ts")
		|| !stricmp(szItemCode, "7ts");
}

BOOL D2IsBow(LPCSTR szItemCode)
{
	if (szItemCode == NULL)
		return FALSE;

	return !stricmp(szItemCode, "sbw")
		|| !stricmp(szItemCode, "hbw")
		|| !stricmp(szItemCode, "lbw")
		|| !stricmp(szItemCode, "cbw")
		|| !stricmp(szItemCode, "sbb")
		|| !stricmp(szItemCode, "lbb")
		|| !stricmp(szItemCode, "swb")
		|| !stricmp(szItemCode, "lwb")

		|| !stricmp(szItemCode, "8sb")
		|| !stricmp(szItemCode, "8hb")
		|| !stricmp(szItemCode, "8lb")
		|| !stricmp(szItemCode, "8cb")
		|| !stricmp(szItemCode, "8s8")
		|| !stricmp(szItemCode, "8l8")
		|| !stricmp(szItemCode, "8sw")
		|| !stricmp(szItemCode, "8lw")

		|| !stricmp(szItemCode, "6sb")
		|| !stricmp(szItemCode, "6hb")
		|| !stricmp(szItemCode, "6lb")
		|| !stricmp(szItemCode, "6cb")
		|| !stricmp(szItemCode, "6s7")
		|| !stricmp(szItemCode, "6l7")
		|| !stricmp(szItemCode, "6sw")
		|| !stricmp(szItemCode, "6lw");
}

BOOL D2IsCrossBow(LPCSTR szItemCode)
{
	if (szItemCode == NULL)
		return FALSE;

	return !stricmp(szItemCode, "lxb")
		|| !stricmp(szItemCode, "mxb")
		|| !stricmp(szItemCode, "hxb")
		|| !stricmp(szItemCode, "rxb")

		|| !stricmp(szItemCode, "8lx")
		|| !stricmp(szItemCode, "8mx")
		|| !stricmp(szItemCode, "8hx")
		|| !stricmp(szItemCode, "8rx")

		|| !stricmp(szItemCode, "6lx")
		|| !stricmp(szItemCode, "6mx")
		|| !stricmp(szItemCode, "6hx")
		|| !stricmp(szItemCode, "6rx");
}

BOOL D2GetItemCode(const BYTE* aPacket, DWORD aLen, LPTSTR szBuffer)
{
	if (aPacket == NULL || aLen < 19 || szBuffer == NULL)
		return FALSE;

	::memset(szBuffer, 0, (ITEM_CODE_LEN + 1) * sizeof(char));
	if(aPacket[0] != 0x9c && aPacket[0] != 0x9d)
		return FALSE;

	CBitFields iPacket(aPacket,aLen);	
	iPacket.GetField((aPacket[0] == 0x9d) ? 164 : 124);

	szBuffer[0] = (char)iPacket.GetField(8);
	szBuffer[1] = (char)iPacket.GetField(8);
	szBuffer[2] = (char)iPacket.GetField(8);
	return strlen(szBuffer) == ITEM_CODE_LEN;
}

///////////////////////////////////////////////
// Item Data Parsing, by Ackmed (or Nuttzy?)
///////////////////////////////////////////////
BOOL D2ParseItem(const BYTE *aPacket, DWORD aLen, LPITEM lpItem)
{
	if (!lpItem)
		return FALSE;
	
	::memset(lpItem, 0 ,sizeof(ITEM));
	if (aPacket[0] != 0x9c && aPacket[0] != 0x9d)
		return FALSE;
	
	CBitFields iPacket(aPacket,aLen);	

	lpItem->iMessageID = (BYTE)iPacket.GetField(8);//message
	lpItem->iAction = (BYTE)iPacket.GetField(8);
	iPacket.GetField(8);//message size
	lpItem->iType = (BYTE)iPacket.GetField(8);
	lpItem->dwItemID = iPacket.GetField(32);

	if (aPacket[0] == 0x9d)
		iPacket.GetField(40); // for 0x9d

	if (aPacket[1] == ITEM_ACTION_TO_BELTSLOT
		|| aPacket[1] == ITEM_ACTION_FROM_BELTSLOT
		|| aPacket[1] == ITEM_ACTION_SWITCH_BELTSLOT)
	{
		// determine column & row
		lpItem->iAtBeltRow = (((aPacket[14] % 32) - (aPacket[14] % 8)) / 8) % 4;
		lpItem->iAtBeltColumn = ((aPacket[14] % 8) / 2) % 4;
	}
	else if (aPacket[1] == ITEM_ACTION_SHIFT_BELTSLOT)
	{
		// determine column & row
		lpItem->iAtBeltRow = (((aPacket[19] % 32) - (aPacket[19] % 8)) / 8) % 4;
		lpItem->iAtBeltColumn = ((aPacket[19] % 8) / 2) % 4;
	}

	lpItem->iIsSocketFull = (BYTE)iPacket.GetField(1);
	iPacket.GetField(3);
	lpItem->iIdentified = (BYTE)iPacket.GetField(1);
	iPacket.GetField(1);
	lpItem->iSwtichIn = (BYTE)iPacket.GetField(1);
	lpItem->iSwitchOut = (BYTE)iPacket.GetField(1);
	lpItem->iBroken = (BYTE)iPacket.GetField(1);
	iPacket.GetField(2);
	BOOL bSocketed = iPacket.GetField(1);
	iPacket.GetField(4);

	BOOL bIsEar = (BOOL)iPacket.GetField(1);
	iPacket.GetField(5);
	lpItem->iEthereal = (BYTE)iPacket.GetField(1);
	iPacket.GetField(1);

	lpItem->iPersonalized = (BYTE)iPacket.GetField(1);
	lpItem->iGamble = (BYTE)iPacket.GetField(1);
	lpItem->iRuneword = (BYTE)iPacket.GetField(1);
	iPacket.GetField(15);
	lpItem->iLocation = (BYTE)iPacket.GetField(3);

	if(aPacket[1] == ITEM_ACTION_DROP || aPacket[1] == ITEM_ACTION_NEW_GROUND || aPacket[1] == ITEM_ACTION_OLD_GROUND)
	{
		lpItem->wPositionX = (WORD)iPacket.GetField(16);
		lpItem->wPositionY = (WORD)iPacket.GetField(16);
	}
	else
	{
		iPacket.GetField(4);
		lpItem->wPositionX = (WORD)iPacket.GetField(4);
		lpItem->wPositionY = (WORD)iPacket.GetField(4);
		lpItem->iStorageID = (BYTE)iPacket.GetField(3);
	}

	if (bIsEar)
		return TRUE;

	lpItem->szItemCode[0] = (char)iPacket.GetField(8);
	lpItem->szItemCode[1] = (char)iPacket.GetField(8);
	lpItem->szItemCode[2] = (char)iPacket.GetField(8);
	lpItem->szItemCode[3] = '\0';
	iPacket.GetField(8);
	
	if (!stricmp(lpItem->szItemCode, "gld"))
	{
		if(iPacket.GetField(1))
			lpItem->dwGoldAmount = iPacket.GetField(32);
		else
			lpItem->dwGoldAmount = iPacket.GetField(12);
		return TRUE;
	}

	if (   !stricmp(lpItem->szItemCode, "ibk")
		|| !stricmp(lpItem->szItemCode, "tbk")
		|| !stricmp(lpItem->szItemCode, "key")
		|| !stricmp(lpItem->szItemCode, "box")
		|| !stricmp(lpItem->szItemCode, "cqv")
		|| !stricmp(lpItem->szItemCode, "aqv"))
	{
		return TRUE;
	}

	// packet not long enough to read ItemLevel
	if(iPacket.GetPos() + 14 >= iPacket.GetMaxBits())
		return TRUE;

	lpItem->iGemmedNumber = (BYTE)iPacket.GetField(3);
	lpItem->iLevel = (BYTE)iPacket.GetField(7);
	lpItem->iQuality = (BYTE)iPacket.GetField(4);

	BOOL flag1 = iPacket.GetField(1);
	BOOL flag2 = iPacket.GetField(1);

	if(flag1)//ring amu charm jew
	{
		iPacket.GetField(3);
	}
	else if(flag2)
	{
		iPacket.GetField(11);
	}

	int pos_begin = iPacket.GetPos();

///////////////////////////////////////////
	int retry_time=0;
RE_TRY:
	iPacket.SetCurPos(pos_begin);
///////////////////////////////////////////

	if(lpItem->iIdentified)
	{
		switch(lpItem->iQuality)
		{
		case ITEM_LEVEL_MAGIC:
			lpItem->wMagicPrefix = (BYTE)iPacket.GetField(11);
			lpItem->wMagicSuffix = (BYTE)iPacket.GetField(11);
			break;
			
		case ITEM_LEVEL_UNIQUE:
			lpItem->wUniqueID = (WORD)iPacket.GetField(12);
			break;

		case ITEM_LEVEL_SET:
			lpItem->wSetID = (WORD)iPacket.GetField(12);
			break;

		case ITEM_LEVEL_SUPERIOR:
			iPacket.GetField(3);
			break;

		case ITEM_LEVEL_RARE:
		case ITEM_LEVEL_CRAFT:
			lpItem->wRarePrefix=(WORD)iPacket.GetField(8) - 155;
			lpItem->wRareSuffix =(WORD)iPacket.GetField(8);
			if (!lpItem->iIdentified)
				return TRUE;

			/////////////////////////////////////////////
			// 这一段不知道是什么含义，长度也不固定
			
			iPacket.GetField(6);
			const int rcskip[]={3,4,5,2,6,7,1,8,9};
			for (int i = 0; i < rcskip[retry_time]; i++)
			{
				iPacket.GetField(11);
			}
			/////////////////////////////////////////////
			break;
			
		}
	}

	if (lpItem->iRuneword)
	{
		iPacket.GetField(16);
	}

	//基本防御
	if(lpItem->iType ==ITEM_ITEMTYPE_ARMOR
		||lpItem->iType==ITEM_ITEMTYPE_HELM
		||lpItem->iType==ITEM_ITEMTYPE_SHIELD
		||lpItem->iType==ITEM_ITEMTYPE_EXPANSION
		||lpItem->iType==ITEM_ITEMTYPE_OTHER && !flag1)
	{
		lpItem->wDefense = (WORD)(iPacket.GetField(11) - 10);
	}


	if(D2IsThrowItem(lpItem->szItemCode))
	{
		iPacket.GetField(9);
		iPacket.GetField(17);
	}
	else if(!stricmp(lpItem->szItemCode, "7cr")) //phase blade
	{
		iPacket.GetField(8);
	}
	else if(lpItem->iType ==ITEM_ITEMTYPE_BOW)
	{
		iPacket.GetField(17);
	}
	else if(!flag1)
	{
		lpItem->wDurability = (WORD)iPacket.GetField(8);
		lpItem->wMaxDurability = (WORD)iPacket.GetField(8);
		iPacket.GetField(1);
	}

	if(bSocketed)
		lpItem->iSocketNumber = (BYTE)iPacket.GetField(4);

	if (!lpItem->iIdentified)
		return TRUE;

	if (lpItem->iQuality == ITEM_LEVEL_SET)
	{
		iPacket.GetField(5);
	}

	BOOL bModOK=__D2ParseMods(iPacket, lpItem);

	///////////////////////////////////////////////////
	if(!bModOK)
	{
		if((lpItem->iQuality==ITEM_LEVEL_RARE
			||lpItem->iQuality==ITEM_LEVEL_CRAFT)
			&&retry_time<9)
		{
			lpItem->wDefense = 0;
			lpItem->wMaxDurability = 0;
			lpItem->wDurability = 0;
			lpItem->iSocketNumber = 0;
			retry_time++;
			goto RE_TRY;
		}
	}
	///////////////////////////////////////////////////

	return TRUE;
}

BOOL D2GetItemMod(LPCITEM lpItem, DWORD dwModID, LPITEMMOD pMod)
{
	if (pMod)
		::memset(pMod, 0, sizeof(ITEMMOD));
	if (dwModID >= 511)
		return FALSE;

	for (BYTE i = 0; i < lpItem->iModCount; i++)
	{
		if (lpItem->aMods[i].dwModID == dwModID && lpItem->aMods[i].iCount > 0)
		{
			if (pMod)
				::memcpy(pMod, &lpItem->aMods[i], sizeof(ITEMMOD));
			return TRUE;
		}
	}

	return FALSE;
}

BYTE D2GetBeltRows(LPCTSTR lpszItemCode)
{
	if (lpszItemCode == NULL)
		return 0;

	if (stricmp(lpszItemCode, "lbl") == 0 // sash
		|| stricmp(lpszItemCode, "vbl") == 0) // light belt
		return 2;
	else if (stricmp(lpszItemCode, "mbl") == 0 // belt
		|| stricmp(lpszItemCode, "tbl") == 0) // heavy belt
		return 3;
	else if (stricmp(lpszItemCode, "hbl") == 0 // plated belt
		|| stricmp(lpszItemCode, "zlb") == 0 // demonhide sash
		|| stricmp(lpszItemCode, "zvb") == 0 // sharkskin belt
		|| stricmp(lpszItemCode, "zmb") == 0 // mesh belt
		|| stricmp(lpszItemCode, "ztb") == 0 // battle belt
		|| stricmp(lpszItemCode, "zhb") == 0 // war belt
		|| stricmp(lpszItemCode, "ulc") == 0 // Spiderweb Sash
		|| stricmp(lpszItemCode, "uvc") == 0 // Vampirefang Belt
		|| stricmp(lpszItemCode, "umc") == 0 // Mithril Coil
		|| stricmp(lpszItemCode, "utc") == 0 // Troll Belt
		|| stricmp(lpszItemCode, "uhc") == 0) // Colossus Girdle
		return 4;
	else
		return 0;
}

DWORD D2ItemCode2Dword(LPCTSTR lpszItemCode) // convert a string into dword
{
	if (lpszItemCode == NULL)
		return 0;

	DWORD dw = 0;
	::memcpy(&dw, lpszItemCode, ITEM_CODE_LEN);
	return dw;
}

BOOL D2IsCirclets(LPCSTR lpszItemCode)
{
	return !stricmp(lpszItemCode, "ci0")
		|| !stricmp(lpszItemCode, "ci1")
		|| !stricmp(lpszItemCode, "ci2")
		|| !stricmp(lpszItemCode, "ci3");
}

BOOL D2IsGloves(LPCSTR lpszItemCode)
{
	return !stricmp(lpszItemCode, "lgl")
		|| !stricmp(lpszItemCode, "vgl")
		|| !stricmp(lpszItemCode, "mgl")
		|| !stricmp(lpszItemCode, "tgl")
		|| !stricmp(lpszItemCode, "hgl")

		|| !stricmp(lpszItemCode, "xlg")
		|| !stricmp(lpszItemCode, "xvg")
		|| !stricmp(lpszItemCode, "xmg")
		|| !stricmp(lpszItemCode, "xtg")
		|| !stricmp(lpszItemCode, "xhg")
		
		|| !stricmp(lpszItemCode, "ulg")
		|| !stricmp(lpszItemCode, "uvg")
		|| !stricmp(lpszItemCode, "umg")
		|| !stricmp(lpszItemCode, "utg")
		|| !stricmp(lpszItemCode, "uhg");
}

BOOL D2IsBoots(LPCSTR lpszItemCode)
{
	return !stricmp(lpszItemCode, "lbt")
		|| !stricmp(lpszItemCode, "vbt")
		|| !stricmp(lpszItemCode, "mbt")
		|| !stricmp(lpszItemCode, "tbt")
		|| !stricmp(lpszItemCode, "hbt")

		|| !stricmp(lpszItemCode, "xlb")
		|| !stricmp(lpszItemCode, "xvb")
		|| !stricmp(lpszItemCode, "xmb")
		|| !stricmp(lpszItemCode, "xtb")
		|| !stricmp(lpszItemCode, "xhb")
		
		|| !stricmp(lpszItemCode, "ulb")
		|| !stricmp(lpszItemCode, "uvb")
		|| !stricmp(lpszItemCode, "umb")
		|| !stricmp(lpszItemCode, "utb")
		|| !stricmp(lpszItemCode, "uhb");
}