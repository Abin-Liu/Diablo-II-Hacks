//#include "stdafx.h"
#include "item.h"
#include "BitFields.h"
#include "properties.h"
#include "itemsize.h"

BOOL __D2ParseMods(CBitFields &iPacket, ITEM& item)
{
	int temp;
	item.iModCount = 0;

	do
	{
		LPITEMMOD p= &item.aMods[item.iModCount];
		p->dwModID = iPacket.GetField(9);

		if(item.iModCount > 0)
		{
			if(p->dwModID < item.aMods[item.iModCount - 1].dwModID) 
			{
				item.iModCount = 0;
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
		
		item.iModCount++;
	}while(item.iModCount < ITEM_MAX_MOD && iPacket.GetPos() + 9 < iPacket.GetMaxBits());

	return FALSE;
}

SIZE D2GetItemSize(LPCTSTR lpszItemCode)
{
	SIZE val = { 0 };
	if (lpszItemCode == NULL)
		return val;

	for (int i = 0; ITEM_SIZE_TB[i].code; i++)
	{
		if (_tcsicmp(lpszItemCode, ITEM_SIZE_TB[i].code) == 0)
		{
			val.cx = ITEM_SIZE_TB[i].x;
			val.cy = ITEM_SIZE_TB[i].y;
			return val;
		}
	}

	return val;
}

BOOL IsPotion(const ITEM& item)
{
	return !_tcsicmp(item.szItemCode, _T("rvl"))
		|| !_tcsicmp(item.szItemCode, _T("rvs"))

		|| !_tcsicmp(item.szItemCode, _T("hp1"))
		|| !_tcsicmp(item.szItemCode, _T("hp2"))
		|| !_tcsicmp(item.szItemCode, _T("hp3"))
		|| !_tcsicmp(item.szItemCode, _T("hp4"))
		|| !_tcsicmp(item.szItemCode, _T("hp5"))

		|| !_tcsicmp(item.szItemCode, _T("mp1"))
		|| !_tcsicmp(item.szItemCode, _T("mp2"))
		|| !_tcsicmp(item.szItemCode, _T("mp3"))
		|| !_tcsicmp(item.szItemCode, _T("mp4"))
		|| !_tcsicmp(item.szItemCode, _T("mp5"));
}

BOOL IsThrowItem(const ITEM& item)
{
	return !_tcsicmp(item.szItemCode, _T("jav"))
		|| !_tcsicmp(item.szItemCode, _T("9ja"))
		|| !_tcsicmp(item.szItemCode, _T("7ja"))

		|| !_tcsicmp(item.szItemCode, _T("tax"))
		|| !_tcsicmp(item.szItemCode, _T("9ta"))
		|| !_tcsicmp(item.szItemCode, _T("7ta"))

		|| !_tcsicmp(item.szItemCode, _T("tkf"))
		|| !_tcsicmp(item.szItemCode, _T("9tk"))
		|| !_tcsicmp(item.szItemCode, _T("7tk"))

		|| !_tcsicmp(item.szItemCode, _T("am5"))
		|| !_tcsicmp(item.szItemCode, _T("ama"))
		|| !_tcsicmp(item.szItemCode, _T("amf"))

		|| !_tcsicmp(item.szItemCode, _T("pil"))
		|| !_tcsicmp(item.szItemCode, _T("9pi"))
		|| !_tcsicmp(item.szItemCode, _T("7pi"))

		|| !_tcsicmp(item.szItemCode, _T("bkf"))
		|| !_tcsicmp(item.szItemCode, _T("9bk"))
		|| !_tcsicmp(item.szItemCode, _T("7bk"))

		|| !_tcsicmp(item.szItemCode, _T("bal"))
		|| !_tcsicmp(item.szItemCode, _T("9b8"))
		|| !_tcsicmp(item.szItemCode, _T("7b8"))

		|| !_tcsicmp(item.szItemCode, _T("glv"))
		|| !_tcsicmp(item.szItemCode, _T("9gl"))
		|| !_tcsicmp(item.szItemCode, _T("7gl"))

		|| !_tcsicmp(item.szItemCode, _T("tsp"))
		|| !_tcsicmp(item.szItemCode, _T("9ts"))
		|| !_tcsicmp(item.szItemCode, _T("7ts"));
}

BOOL D2GetItemCode(const BYTE* aPacket, DWORD aLen, LPTSTR szBuffer)
{
	if (aPacket == NULL || aLen < 19 || szBuffer == NULL)
		return FALSE;

	::memset(szBuffer, 0, (ITEM_CODE_LEN + 1) * sizeof(TCHAR));
	if(aPacket[0] != 0x9c && aPacket[0] != 0x9d)
		return FALSE;

	CBitFields iPacket(aPacket,aLen);	
	iPacket.GetField((aPacket[0] == 0x9d) ? 164 : 124);

	szBuffer[0] = (TCHAR)iPacket.GetField(8);
	szBuffer[1] = (TCHAR)iPacket.GetField(8);
	szBuffer[2] = (TCHAR)iPacket.GetField(8);
	return _tcslen(szBuffer) == ITEM_CODE_LEN;
}

BOOL D2ParseItem(const BYTE *aPacket, DWORD aLen, ITEM& item)
{
	::memset(&item, 0 ,sizeof(ITEM));
	if (aPacket[0] != 0x9c && aPacket[0] != 0x9d)
		return FALSE;
	
	CBitFields iPacket(aPacket,aLen);	

	item.iMessageID = (BYTE)iPacket.GetField(8);//message
	item.iAction = (BYTE)iPacket.GetField(8);
	iPacket.GetField(8);//message size
	item.iType = (BYTE)iPacket.GetField(8);
	item.dwItemID = iPacket.GetField(32);

	if (aPacket[0] == 0x9d)
		iPacket.GetField(40); // for 0x9d

	if (aPacket[1] == ITEM_ACTION_TO_BELTSLOT
		|| aPacket[1] == ITEM_ACTION_FROM_BELTSLOT
		|| aPacket[1] == ITEM_ACTION_SWITCH_BELTSLOT)
	{
		// determine column & row
		item.iAtBeltRow = (((aPacket[14] % 32) - (aPacket[14] % 8)) / 8) % 4;
		item.iAtBeltColumn = ((aPacket[14] % 8) / 2) % 4;
	}
	else if (aPacket[1] == ITEM_ACTION_SHIFT_BELTSLOT)
	{
		// determine column & row
		item.iAtBeltRow = (((aPacket[19] % 32) - (aPacket[19] % 8)) / 8) % 4;
		item.iAtBeltColumn = ((aPacket[19] % 8) / 2) % 4;
	}

	item.iIsSocketFull = (BYTE)iPacket.GetField(1);
	iPacket.GetField(3);
	item.iIdentified = (BYTE)iPacket.GetField(1);
	iPacket.GetField(1);
	item.iSwtichIn = (BYTE)iPacket.GetField(1);
	item.iSwitchOut = (BYTE)iPacket.GetField(1);
	item.iBroken = (BYTE)iPacket.GetField(1);
	iPacket.GetField(2);
	BOOL bSocketed = iPacket.GetField(1);
	iPacket.GetField(4);

	BOOL bIsEar = (BOOL)iPacket.GetField(1);
	iPacket.GetField(5);
	item.iEthereal = (BYTE)iPacket.GetField(1);
	iPacket.GetField(1);

	item.iPersonalized = (BYTE)iPacket.GetField(1);
	item.iGamble = (BYTE)iPacket.GetField(1);
	item.iRuneword = (BYTE)iPacket.GetField(1);
	iPacket.GetField(15);
	item.iLocation = (BYTE)iPacket.GetField(3);

	if(aPacket[1] == ITEM_ACTION_DROP || aPacket[1] == ITEM_ACTION_NEW_GROUND || aPacket[1] == ITEM_ACTION_OLD_GROUND)
	{
		item.wPositionX = (WORD)iPacket.GetField(16);
		item.wPositionY = (WORD)iPacket.GetField(16);
	}
	else
	{
		iPacket.GetField(4);
		item.wPositionX = (WORD)iPacket.GetField(4);
		item.wPositionX = (WORD)iPacket.GetField(4);
		item.iStorageID = (BYTE)iPacket.GetField(3);
	}

	if (bIsEar)
		return TRUE;

	item.szItemCode[0] = (TCHAR)iPacket.GetField(8);
	item.szItemCode[1] = (TCHAR)iPacket.GetField(8);
	item.szItemCode[2] = (TCHAR)iPacket.GetField(8);
	item.szItemCode[3] = _T('\0');
	iPacket.GetField(8);
	
	if (!_tcsicmp(item.szItemCode, _T("gld")))
	{
		if(iPacket.GetField(1))
			item.dwGoldAmount = iPacket.GetField(32);
		else
			item.dwGoldAmount = iPacket.GetField(12);
		return TRUE;
	}

	if (   !_tcsicmp(item.szItemCode, _T("ibk"))
		|| !_tcsicmp(item.szItemCode, _T("tbk"))
		|| !_tcsicmp(item.szItemCode, _T("key"))
		|| !_tcsicmp(item.szItemCode, _T("box"))
		|| !_tcsicmp(item.szItemCode, _T("cqv"))
		|| !_tcsicmp(item.szItemCode, _T("aqv")))
	{
		return TRUE;
	}

	// packet not long enough to read ItemLevel
	if(iPacket.GetPos() + 14 >= iPacket.GetMaxBits())
		return TRUE;

	iPacket.GetField(3);
	item.iLevel = (BYTE)iPacket.GetField(7);
	item.iQuality = (BYTE)iPacket.GetField(4);

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

	if(item.iIdentified)
	{
		switch(item.iQuality)
		{
		case ITEM_LEVEL_MAGIC:
			/*item.wMagicPrefix = (BYTE)*/iPacket.GetField(11);
			/*item.wMagicSuffix = (BYTE)*/iPacket.GetField(11);
			break;
			
		case ITEM_LEVEL_UNIQUE:
			/*item.wUniqueID = (WORD)*/iPacket.GetField(12);
			break;

		case ITEM_LEVEL_SET:
			/*item.wSetID = (WORD)*/iPacket.GetField(12);
			break;

		case ITEM_LEVEL_SUPERIOR:
			iPacket.GetField(3);
			break;

		case ITEM_LEVEL_RARE:
		case ITEM_LEVEL_CRAFT:
			/*item.wMagicPrefix=(WORD)*/iPacket.GetField(8)/* - 155*/;
			/*item.wMagicSuffix =(WORD)*/iPacket.GetField(8);
			if (!item.iIdentified)
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

	if (item.iRuneword)
	{
		iPacket.GetField(16);
	}

	//基本防御
	if(item.iType ==ITEM_ITEMTYPE_ARMOR
		||item.iType==ITEM_ITEMTYPE_HELM
		||item.iType==ITEM_ITEMTYPE_SHIELD
		||item.iType==ITEM_ITEMTYPE_EXPANSION
		||item.iType==ITEM_ITEMTYPE_OTHER && !flag1)
	{
		item.wDefense = (WORD)(iPacket.GetField(11) - 10);
	}


	if(IsThrowItem(item))
	{
		iPacket.GetField(9);
		iPacket.GetField(17);
	}
	else if(!_tcsicmp(item.szItemCode, _T("7cr"))) //phase blade
	{
		iPacket.GetField(8);
	}
	else if(item.iType ==ITEM_ITEMTYPE_BOW)
	{
		iPacket.GetField(17);
	}
	else if(!flag1)
	{
		item.wDurability = (WORD)iPacket.GetField(8);
		item.wMaxDurability = (WORD)iPacket.GetField(8);
		iPacket.GetField(1);
	}

	if(bSocketed)
		item.iSocketNumber = (BYTE)iPacket.GetField(4);

	if (!item.iIdentified)
		return TRUE;

	if (item.iQuality == ITEM_LEVEL_SET)
	{
		iPacket.GetField(5);
	}

	BOOL bModOK=__D2ParseMods(iPacket, item);

	///////////////////////////////////////////////////
	if(!bModOK)
	{
		if((item.iQuality==ITEM_LEVEL_RARE
			||item.iQuality==ITEM_LEVEL_CRAFT)
			&&retry_time<9)
		{
			item.wDefense = 0;
			item.wMaxDurability = 0;
			item.wDurability = 0;
			item.iSocketNumber = 0;
			retry_time++;
			goto RE_TRY;
		}
	}
	///////////////////////////////////////////////////

	return TRUE;
}

BOOL D2GetItemMod(const ITEM& item, DWORD dwModID, LPITEMMOD pMod)
{
	if (pMod)
		::memset(pMod, 0, sizeof(ITEMMOD));
	if (dwModID >= 511)
		return FALSE;

	for (BYTE i = 0; i < item.iModCount; i++)
	{
		if (item.aMods[i].dwModID == dwModID && item.aMods[i].iCount > 0)
		{
			if (pMod)
				::memcpy(pMod, &item.aMods[i], sizeof(ITEMMOD));
			return TRUE;
		}
	}

	return FALSE;
}

BYTE D2GetBeltRows(LPCTSTR lpszItemCode)
{
	if (lpszItemCode == NULL)
		return 0;

	if (_tcsicmp(lpszItemCode, _T("lbl")) == 0 // sash
		|| _tcsicmp(lpszItemCode, _T("vbl")) == 0) // light belt
		return 2;
	else if (_tcsicmp(lpszItemCode, _T("mbl")) == 0 // belt
		|| _tcsicmp(lpszItemCode, _T("tbl")) == 0) // heavy belt
		return 3;
	else if (_tcsicmp(lpszItemCode, _T("hbl")) == 0 // plated belt
		|| _tcsicmp(lpszItemCode, _T("zlb")) == 0 // demonhide sash
		|| _tcsicmp(lpszItemCode, _T("zvb")) == 0 // sharkskin belt
		|| _tcsicmp(lpszItemCode, _T("zmb")) == 0 // mesh belt
		|| _tcsicmp(lpszItemCode, _T("ztb")) == 0 // battle belt
		|| _tcsicmp(lpszItemCode, _T("zhb")) == 0 // war belt
		|| _tcsicmp(lpszItemCode, _T("ulc")) == 0 // Spiderweb Sash
		|| _tcsicmp(lpszItemCode, _T("uvc")) == 0 // Vampirefang Belt
		|| _tcsicmp(lpszItemCode, _T("umc")) == 0 // Mithril Coil
		|| _tcsicmp(lpszItemCode, _T("utc")) == 0 // Troll Belt
		|| _tcsicmp(lpszItemCode, _T("uhc")) == 0) // Colossus Girdle
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