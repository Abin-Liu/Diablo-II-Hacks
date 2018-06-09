//////////////////////////////////////////////////////////////////////
// EnumPresetUnit.cpp
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include "CommonStructs.h"
#include "Utilities\\ArrayEx.h"
#include "definitions.h"
#include "d2functions.h"
#include "D2HackIt.h"

BOOL RevealPresetUnits(CArrayEx<PRESETUNIT, const PRESETUNIT&>& aUnits);
void FindPresetUnitsFromRoom2(RoomOther *ro, CArrayEx<DWORD, DWORD>& aChecked, CArrayEx<PRESETUNIT, const PRESETUNIT&>& aUnits);
BOOL operator==(const PRESETUNIT& lhs, const PRESETUNIT& rhs);
BOOL operator>(const PRESETUNIT& lhs, const PRESETUNIT& rhs);

BOOL operator==(const PRESETUNIT& lhs, const PRESETUNIT& rhs)
{
	return ::memcmp(&lhs, &rhs, sizeof(PRESETUNIT)) == 0;
}

BOOL operator>(const PRESETUNIT& lhs, const PRESETUNIT& rhs)
{
	return ::memcmp(&lhs, &rhs, sizeof(PRESETUNIT)) > 0;
}

BOOL RevealPresetUnits(CArrayEx<PRESETUNIT, const PRESETUNIT&>& aUnits)
{
	CArrayEx<DWORD, DWORD> aChecked;
	aUnits.RemoveAll();

	UnitAny *player=D2CLIENT_GetPlayerUnit();
	if(!player)
	{
		//GamePrintError("Invalid player");
		return FALSE;
	}

	Room *curRoom=D2COMMON_GetRoomFromUnit(player);
	if(!curRoom)
	{
		//GamePrintError("Invalid Room");
		return FALSE;
	}
	
	RoomOther *curRoomOther = curRoom->ptRoomOther;
	if(!curRoomOther)
	{
		//GamePrintError("Invalid Room2");
		return FALSE;
	}

	FindPresetUnitsFromRoom2(curRoomOther, aChecked, aUnits);
	return aUnits.GetSize();
}

void FindPresetUnitsFromRoom2(RoomOther *ro, CArrayEx<DWORD, DWORD>& aChecked, CArrayEx<PRESETUNIT, const PRESETUNIT&>& aUnits)
{
	if(ro->ptDrlgLevel->LevelNo!=GetCurrentMapID())
		return;

	if (aChecked.Find((DWORD)ro) != -1)
		return;

	BOOL add_room=FALSE;
	if(!ro->pRoom)
	{
		add_room=TRUE;
		D2COMMON_AddRoomData(D2CLIENT_GetPlayerUnit()->ptAct, GetCurrentMapID(), ro->xPos, ro->yPos, D2CLIENT_GetPlayerUnit());
	}

	aChecked.Add((DWORD)ro);
	aChecked.Sort();
	
	PresetUnit *unit = ro->pPresetUnits;
	while (unit) 
	{	
		PRESETUNIT pu;
		pu.dwID = unit->nTxtFileNo;
		pu.dwType =unit->nUnitType;
		pu.x = unit->xPos+ro->xPos*5;
		pu.y = unit->yPos+ro->yPos*5;

		if (aUnits.Find(pu) == -1)
			aUnits.Add(pu);

		unit = unit->pNext;
	}

	RoomOther **n = ro->ptList;
	for(int i=0;i<ro->nRoomList;i++)
	{
		FindPresetUnitsFromRoom2(n[i], aChecked, aUnits);
	}
	if (add_room)
	{
		D2COMMON_RemoveRoomData(D2CLIENT_GetPlayerUnit()->ptAct, GetCurrentMapID(), ro->xPos, ro->yPos, D2CLIENT_GetPlayerUnit());
	}
}

BOOL EXPORT EnumPresetUnits(fnEnumPresetUnitProc lpfnEnumPresetUnitProc, LPARAM lParam)
{
	if (lpfnEnumPresetUnitProc == NULL)
		return FALSE;

	CArrayEx<PRESETUNIT, const PRESETUNIT&> aUnits;
	RevealPresetUnits(aUnits);
	
	for (int i = 0; i < aUnits.GetSize(); i++)
	{
		if (!lpfnEnumPresetUnitProc(&aUnits[i], lParam))
			return FALSE;
	}

	return TRUE;
}