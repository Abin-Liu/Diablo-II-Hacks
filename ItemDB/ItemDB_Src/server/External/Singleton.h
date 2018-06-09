// Singleton.h: interface for the CSingleton class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _SINGLETON_H__
#define _SINGLETON_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>

class CSingleton  
{
public:
	CSingleton();
	CSingleton(LPCTSTR lpUniqueString);
	virtual ~CSingleton();

	BOOL Create(LPCTSTR lpUniqueString);

	BOOL IsInstanceUnique() const;
	void RestorePrevWnd() const;	
	
	HWND GetPrevWnd() const;
	void SetPrevWnd(HWND hWnd);
	
	HINSTANCE GetPrevInstance() const;
	void SetPrevInstance(HINSTANCE hInstance);

private:
	char* m_pData; // Points to the data area
	HANDLE m_hFileMapping; // File mapping handle
	BOOL m_bFlagged;
};

#endif // _SINGLETON_H__
