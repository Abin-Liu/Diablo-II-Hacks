//////////////////////////////////////////////////////////////////////
// D2Monitor.h
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __D2MONITOR_H__
#define __D2MONITOR_H__ 

#include <windows.h>
#include "TickThread.h"
#include "..\\..\\Common\\ArrayEx.h"

#define MAX_D2WNDS	32

class CD2Monitor  
{
public:

	CD2Monitor();
	virtual ~CD2Monitor();

	BOOL Start(HWND hTargetWnd, UINT nMsg, DWORD dwInterval);
	void Stop();

private:	

	void OnTimerTick();
	static void CALLBACK TickProc(LPVOID lParam, LPCVBYTE lpStopFlag);
	static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);

	CTickThread m_thread;
	HWND m_hTargetWnd;
	UINT m_nMsg;
	CArrayEx<HWND, HWND> m_aD2Wnds;
};

#endif // __D2MONITOR_H__
