////////////////////////////////////////////////////////////
// BitFields.h 
//
// Defines the CBitFields class. Originally ThoHell's code
// with some modification.
//
////////////////////////////////////////////////////////////

#ifndef __BITFIELDS_H__
#define __BITFIELDS_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class CBitFields
{	
public:	
	
	CBitFields() { m_pData = NULL; m_dwCurPos = 0; m_dwMaxBits = 0; }
	CBitFields(const BYTE* pData, DWORD dwLength) { SetData(pData, dwLength); }
	void SetData(const BYTE* pData, DWORD dwLength) { m_pData = pData; m_dwCurPos = 0; m_dwMaxBits = (dwLength * 8); }

	static DWORD GetBitField(const BYTE* pData, DWORD dwPos, DWORD dwLength, DWORD dwMaxBits)
	{
		if (dwPos + dwLength > dwMaxBits)
		{
			// if we are already at or beyond the end of packet, return 0;
			if (dwPos >= dwMaxBits)
				return 0;

			// otherwise adjust len to all remaining bits
			dwLength = dwMaxBits - dwPos;
		}

		return (DWORD)(*(unsigned __int64 *)(pData + dwPos / 8) << (64 - dwLength - (dwPos & 7)) >> (64 - dwLength));
	}

	DWORD GetPos() const { return m_dwCurPos; }
	DWORD GetMaxBits() const { return m_dwMaxBits; }
	DWORD SetCurPos(DWORD dwPos) { m_dwCurPos = min(m_dwMaxBits, dwPos); return m_dwCurPos; }
	const BYTE* GetData() const { return m_pData; }
	DWORD GetField(DWORD dwLength) { return GetBitField(m_pData, (m_dwCurPos += dwLength) - dwLength, dwLength, m_dwMaxBits); }

protected:

	const BYTE* m_pData;
	DWORD m_dwCurPos;
	DWORD m_dwMaxBits;
};

#endif // __BITFIELDS_H__