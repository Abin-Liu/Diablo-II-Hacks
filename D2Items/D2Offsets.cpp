#include "stdafx.h"
#include "D2Offsets.h"

BOOL VerifyD2Modules(LPCD2MODULES lpDM);
typedef ItemBinData* (__stdcall *fnItemGetBinFromClass)(int itemclsid); // -0x2968
typedef void (__fastcall *fnGetItemNameString)(UnitItem * i, wchar_t * name, int length); // 0x3D360
typedef UnitItem* (__stdcall *fnGetFirstInventoryItem)(Inventory * ptInventory); // -0x2825				// #10277
typedef UnitItem* (__stdcall *fnGetNextInventoryItem)(UnitItem * ptItem); // -0x2840 					// #10304

BOOL LoadD2Modules(LPCSTR lpszDirectory, LPD2MODULES lpDM)
{
	if (lpszDirectory == NULL || lpDM == NULL)
		return FALSE;

	::memset(lpDM, 0, sizeof(D2MODULES));

	char szOld[MAX_PATH + 1] = "";
	::GetCurrentDirectory(MAX_PATH, szOld);
	::SetCurrentDirectory(lpszDirectory);

	lpDM->hD2Client = ::LoadLibrary("D2Client.dll");
	lpDM->hD2Common = ::LoadLibrary("D2Common.dll");
	if (lpDM->hD2Client == NULL || lpDM->hD2Common == NULL)
	{
		FreeD2Modules(lpDM);
		return FALSE;
	}

	// load dll functions
	lpDM->fpGetFirstInventoryItem = ::GetProcAddress(lpDM->hD2Common, (LPCSTR)0x2825);
	lpDM->fpGetNextInventoryItem = ::GetProcAddress(lpDM->hD2Common, (LPCSTR)0x2840);
	lpDM->fpGetItemTxt = ::GetProcAddress(lpDM->hD2Common, (LPCSTR)0x2968);
	lpDM->fpGetItemNameString = (FARPROC)((DWORD)lpDM->hD2Common + 0x3D360);

	if (!VerifyD2Modules(lpDM))
	{
		FreeD2Modules(lpDM);
		return FALSE;
	}

	return TRUE;
}

void FreeD2Modules(LPD2MODULES lpDM)
{
	if (lpDM)
	{
		if (lpDM->hD2Client)
			::FreeLibrary(lpDM->hD2Client);

		if (lpDM->hD2Common)
			::FreeLibrary(lpDM->hD2Common);

		::memset(lpDM, 0, sizeof(D2MODULES));
	}
}

BOOL VerifyD2Modules(LPCD2MODULES lpDM)
{
	return lpDM != NULL
		&& lpDM->hD2Client != NULL
		&& lpDM->hD2Common != NULL
		&& lpDM->fpGetFirstInventoryItem != NULL
		&& lpDM->fpGetItemNameString != NULL
		&& lpDM->fpGetItemTxt != NULL
		&& lpDM->fpGetNextInventoryItem != NULL;
}


BOOL GetBNetData(HANDLE hProcess, BnetData* pBuffer)
{
	if (hProcess == NULL || pBuffer == NULL)
		return FALSE;

	DWORD dwAddr = 0;
	DWORD dwRead = 0;
	if (!::ReadProcessMemory(hProcess, (LPCVOID)OF_BNDATA, &dwAddr, 4, &dwRead))
		return FALSE;

	if (!::ReadProcessMemory(hProcess, (LPCVOID)dwAddr, pBuffer, sizeof(BnetData), &dwRead))
		return FALSE;
	
	return TRUE;
}

BOOL GetPlayerUnit(HANDLE hProcess, UnitAny* pBuffer)
{
	if (hProcess == NULL || pBuffer == NULL)
		return FALSE;

	DWORD dwAddr = 0;
	DWORD dwRead = 0;
	if (!::ReadProcessMemory(hProcess, (LPCVOID)OF_PLAYERDATA, &dwAddr, 4, &dwRead))
		return FALSE;

	if (!::ReadProcessMemory(hProcess, (LPCVOID)dwAddr, pBuffer, sizeof(UnitAny), &dwRead))
		return FALSE;

	return TRUE;
}

BOOL EnumInventoryItems(HANDLE hProcess, LPCD2MODULES lpDM)
{
	if (hProcess == NULL || !VerifyD2Modules(lpDM))
		return FALSE;
	
	UnitAny ua = { 0 };
	if (!GetPlayerUnit(hProcess, &ua))
		return FALSE;

	if (ua.ptInventory == NULL)
		return FALSE;

	fnGetFirstInventoryItem GetFirstInventoryItem;
	fnGetNextInventoryItem GetNextInventoryItem;
	::memcpy(&GetFirstInventoryItem, &lpDM->fpGetFirstInventoryItem, sizeof(FARPROC));
	::memcpy(&GetNextInventoryItem, &lpDM->fpGetNextInventoryItem, sizeof(FARPROC));

	UnitItem* p = GetFirstInventoryItem(ua.ptInventory);

	//Inventory inv = {0};
	//if (!::ReadProcessMemory(hProcess, (LPCVOID)ua.ptInventory, &inv, sizeof(Inventory), &dwRead))
	//	return FALSE;


	//fnGetFirstInventoryItem GetFirstInventoryItem = (fnGetFirstInventoryItem)OF_GETFIRSTINVITEM;
	//if (!::ReadProcessMemory(hProcess, (LPCVOID)OF_GETFIRSTINVITEM, &GetFirstInventoryItem, 4, &dwRead))
	//	return FALSE;

	//if (GetFirstInventoryItem == NULL)
	//	return FALSE;

	//UnitItem* p = GetFirstInventoryItem(ua.ptInventory);

	return 1;
	
}



