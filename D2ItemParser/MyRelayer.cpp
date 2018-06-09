// MyRelayer.cpp: implementation of the CMyRelayer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyRelayer.h"
#include "item.h"
#include "D2ModParser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyRelayer::CMyRelayer()
{
	m_wHPAdd = 0;
	m_wManaAdd = 0;
	m_wHPMult = 0;
	m_wManaMult = 0;

	SBLoadConfig("D:\\Projects\\D2ItemParser\\Shopbot.ini", "Test Item", &m_config);
}

CMyRelayer::~CMyRelayer()
{

}

void CMyRelayer::OnGamePacketBeforeReceived(const BYTE *aPacket, DWORD aLen)
{
	// TODO: Add your implementation here

	if (aPacket[0] == 0x9c && aPacket[1] == 0x04)
	{
		//if (aPacket[1] == ITEM_ACTION_SHIFT_BELTSLOT)
		//{
			ITEM item = {0};
		D2ParseItem(aPacket, aLen, item);
			//SIZE sz = D2GetItemSize(item.szItemCode);
		BOOL b = SBVerifyItemMods(&item, &m_config);
		CString sMod;
		for (BYTE i = 0; i < item.iModCount; i++)
		{
			ITEMMOD mod = item.aMods[i];
			GetItemModDesc(&mod, sMod);
			int g = 0;
		}
		//}
	}
/*
	if (aPacket[0] == 0xa8
		&& (aPacket[7] == 32 || aPacket[7] == 149 || aPacket[7] == 139 || aPacket[7] == 140))
	{
		BYTE iLevel = CalcPuffLevel(aPacket, aLen);
		int h = 0;
	}
	*/
}

void CMyRelayer::OnGamePacketBeforeSent(const BYTE *aPacket, DWORD aLen)
{
	// TODO: Add your implementation here
}

BYTE CMyRelayer::CalcPuffLevel(const BYTE* aPacket, DWORD aLen)
{
	if (aPacket == NULL || aPacket[0] != 0xa8)
		return 0;

	BYTE iLevel = 0;
	WORD wVal;
	switch (aPacket[7])
	{
	case 32: //bo
		::memcpy(&wVal, aPacket + 9, 2);
		iLevel = (wVal - 0x004c) / 0x06 + 2;
		break;

	case 149: // oak
		::memcpy(&wVal, aPacket + 9, 2);
		iLevel = (wVal - 0x003c) / 0x0a + 1;
		break;

	case 139:
		iLevel = CalcWolf(aPacket[14]);
		break;

	case 140:
		iLevel = CalcBear(aPacket[12]);
		break;

	default:
		return 0;
	}

	return iLevel;
}

BYTE CMyRelayer::CalcWolf(BYTE iFlag)
{
	if (iFlag < 0x16)
		return 0;

	if (iFlag == 0x16)
		return 1;

	BYTE iS = 0x16;
	for (BYTE i = 2; i < 100; i++)
	{
		if (i % 2)
			iS += 2;
		else
			iS += 3;

		if (iFlag == iS)
			return i;
	}

	return 0;
}

BYTE CMyRelayer::CalcBear(BYTE iFlag)
{
	if (iFlag < 0x11)
		return 0;

	if (iFlag == 0x11)
		return 1;

	BYTE iS = 0x11;
	for (BYTE i = 2; i < 100; i++)
	{
		if ((i - 3) % 4)
			iS += 1;
		else
			iS += 2;

		if (iFlag == iS)
			return i;
	}

	return 0;
}
