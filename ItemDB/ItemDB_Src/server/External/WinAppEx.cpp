// WinAppEx.cpp: implementation of the CWinAppEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WinAppEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CWinAppEx* AfxGetAppEx()
{
	return (CWinAppEx*)AfxGetApp();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CWinAppEx::CWinAppEx()
{
	UpdateAppDir();
}

CWinAppEx::CWinAppEx(LPCTSTR lpszUUID)
{
	UpdateAppDir();
	CreateSingleton(lpszUUID);
}

CWinAppEx::~CWinAppEx()
{

}

CSingleton& CWinAppEx::GetSingleton()
{
	return m_st;
}

CString CWinAppEx::GetUUID() const
{
	return m_sUUID;
}

CString CWinAppEx::GetAppDir() const
{
	return m_sAppDir;
}

BOOL CWinAppEx::CreateSingleton(LPCTSTR lpszUUID)
{
	m_sUUID = CString(lpszUUID);
	return m_st.Create(lpszUUID);
}

void CWinAppEx::UpdateAppDir()
{
	CString sAppDir;
	::GetModuleFileName(NULL, sAppDir.GetBuffer(MAX_PATH), MAX_PATH);
	sAppDir.ReleaseBuffer();

	if (sAppDir.Find('\\') != -1)
	{
		for (int i = sAppDir.GetLength() - 1; i >= 0; i--)
		{
			TCHAR ch = sAppDir[i];
			sAppDir.Delete(i);
			if (ch == '\\')
				break;	
		}
	}

	m_sAppDir = sAppDir;
}

BOOL CWinAppEx::IsInstanceUnique() const
{
	return m_st.IsInstanceUnique();
}

void CWinAppEx::SetPrevWnd(HWND hWnd)
{
	m_st.SetPrevWnd(hWnd);
}

HWND CWinAppEx::GetPrevWnd() const
{
	return m_st.GetPrevWnd();
}

BOOL CWinAppEx::RestorePrevWnd() const
{
	if (!::IsWindow(m_st.GetPrevWnd()))
		return FALSE;

	m_st.RestorePrevWnd();
	return TRUE;
}
