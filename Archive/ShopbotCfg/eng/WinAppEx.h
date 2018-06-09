// WinAppEx.h: interface for the CWinAppEx class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __WINAPPEX_H__
#define __WINAPPEX_H__

#include "Singleton.h"

class CWinAppEx : public CWinApp  
{
public:
	CWinAppEx();	
	CWinAppEx(LPCTSTR lpszUUID);
	virtual ~CWinAppEx();

	BOOL CreateSingleton(LPCTSTR lpszUUID);
	CString GetAppDir() const; // app directory
	CString GetUUID() const; // app uuid, if any
	CSingleton& GetSingleton(); // the single-instance checker

protected:
	void UpdateAppDir();
	CSingleton m_st;
	CString m_sUUID;
	CString m_sAppDir;
};

CWinAppEx* AfxGetAppEx();

#endif // __WINAPPEX_H__
