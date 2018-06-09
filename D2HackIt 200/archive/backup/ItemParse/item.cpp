#include "item.h"
#include "BitFields.h"
#include "properties.h"

BOOL IsThrowingItem(LPCSTR szItemCode);

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


	if(IsThrowingItem(lpItem->szItemCode))
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

BOOL IsThrowingItem(LPCSTR szItemCode)
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
