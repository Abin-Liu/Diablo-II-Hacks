//////////////////////////////////////////////////////////////////////
// TickThread.h: interface for the CTickThread class.
//--------------------------------------------------------------------
// A "Tick Thread" is a special thread that calls a particular procedure
// at specified intervals.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#ifndef __TICKTHREAD_H__
#define __TICKTHREAD_H__

#include <windows.h>

typedef const volatile BYTE* LPCVBYTE; // Make the type less messy...
typedef void (CALLBACK *TICKPROC)(LPVOID lParam, LPCVBYTE lpStopFlag); // Tick procedure type def

//////////////////////////////////////////////////////////////////////
// CTickThread Class Definition
//////////////////////////////////////////////////////////////////////
class CTickThread  
{
public:	

	//////////////////////////////////////////////////////////////////
	// Constructor & Destructor
	//////////////////////////////////////////////////////////////////
	CTickThread();
	virtual ~CTickThread();

	//////////////////////////////////////////////////////////////////
	// Thread Manipulation
	//////////////////////////////////////////////////////////////////
	BOOL Start(TICKPROC lpfnProc, LPVOID lpParam = NULL, DWORD dwInterval = 100, int nPriority = THREAD_PRIORITY_NORMAL);
	BOOL Stop(DWORD dwTimeout);
	DWORD Suspend() const;
	DWORD Resume() const;	
	BOOL IsAlive() const;

	//////////////////////////////////////////////////////////////////
	// Thread Attributes
	//////////////////////////////////////////////////////////////////
	operator HANDLE() const;
	HANDLE GetThreadHandle() const;
	DWORD GetThreadID() const;

	//////////////////////////////////////////////////////////////////
	// General Access
	//////////////////////////////////////////////////////////////////
	int GetPriority() const;
	BOOL SetPriority(int nPriority) const;
	DWORD GetInterval() const;
	void SetInterval(DWORD dwInterval);
	LPVOID GetParam() const;
	void SetParam(LPVOID lpParam);	

protected:

	//////////////////////////////////////////////////////////////////
	// Internal Procedure
	//////////////////////////////////////////////////////////////////
	static DWORD CALLBACK InternalProc(LPVOID lpParam);	

	//////////////////////////////////////////////////////////////////
	// Member Data
	//////////////////////////////////////////////////////////////////
	TICKPROC m_lpfnProc;			// Procedure to be called
	HANDLE m_hThreadHandle;			// Thread handle
	DWORD m_dwThreadID;				// Thread ID
	volatile LPVOID m_lpParam;		// Parameter to be passed to procedure	
	volatile DWORD m_dwInterval;	// Interval between two procedure calls, must be dividable by 100ms
	volatile BYTE m_iStopFlas;		// Whether to stop the thread
	volatile BYTE m_iIsAlive;		// Whether thread is alive
};

#endif // __TICKTHREAD_H__
