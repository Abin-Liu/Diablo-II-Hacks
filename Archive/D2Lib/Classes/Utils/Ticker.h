// Ticker.h: interface for the CTicker class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __TICKER_H__
#define __TICKER_H__

#include <windows.h>

class CTicker  
{
public:	
	
	CTicker();
	virtual ~CTicker();

	DWORD GetElapsed() const;
	DWORD GetRemainer() const;
	BOOL IsTickEnded() const;

	void TickIt(DWORD dwCount);
	void ResetTicker(DWORD dwNewCount = 0);

private:
	DWORD m_dwStartWith;
	DWORD m_dwCount;
};

#endif
