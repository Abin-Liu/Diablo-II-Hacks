//////////////////////////////////////////////////////////////////////
// GetItAbstract.h
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __GETITABSTRACT_H__
#define __GETITABSTRACT_H__

#include "GetitConfig.h"
#include "..\\Includes\\CommonStructs.h"

class CGetItAbstract  
{
public:		

	CGetItAbstract();
	CGetItAbstract(CGetItConfigLib* pLib);
	virtual ~CGetItAbstract();

	void SetConfigLib(CGetItConfigLib* pLib);
	const CGetItConfigLib* GetConfigLib() const;
	CGetItConfigLib* GetConfigLib();

	virtual void OnGameJoin() = 0;
	virtual void OnGameLeave() = 0;
	virtual void OnGameTimerTick() = 0;
	virtual void OnThisPlayerMessage(UINT nMessage, WPARAM wParam, LPARAM lParam) = 0;
	virtual void OnUnitMessage(UINT nMessage, LPCGAMEUNIT lpUnit, WPARAM wParam, LPARAM lParam) = 0;

protected:

	void AnnounceItem(DWORD dwItemID);
	BOOL ExamItem(DWORD dwItemID, BOOL bPickOrMove);

	DWORD m_dwLastAnnounce; // last announced item ID
	DWORD m_dwLastAnnounceTime;
	CGetItConfigLib* m_pLib;
};

#endif // __GETITABSTRACT_H__
