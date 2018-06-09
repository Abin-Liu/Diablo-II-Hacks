/////////////////////////////////////////////////////////////////
// StorageBuffer.h
//
// Abin (abinn32@yahoo.com)
/////////////////////////////////////////////////////////////////

#ifndef __STORAGEBUFFER_H__
#define __STORAGEBUFFER_H__

#include <windows.h>

class CStorageBuffer
{
public:	
		
	CStorageBuffer(BYTE iType = -1);
	~CStorageBuffer();

	BOOL IsValid() const;
	BOOL IsValidIndex(int x, int y) const;
	void SetType(BYTE iType);
	BOOL Dump(LPCSTR lpszFilePath) const;
	DWORD GetItem(int x, int y) const;
	static DWORD GetCubeID();
	BOOL GetBestFit(int cx, int cy, LPPOINT lpBuffer) const;

private:
	
	BOOL IsValidLocationSize(int x, int y, int cx, int cy) const;
	BOOL GetAdjacent(int x, int y, int cx, int cy, int& rAdj, int& rIntens) const;
	BOOL AddToBuffer(DWORD dwItemID, int x, int y, int cx, int cy);
	DWORD m_aBuffer[10][8];
	int m_cx;
	int m_cy;
};

#endif // __STORAGEBUFFER_H__