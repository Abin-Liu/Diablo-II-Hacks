// Singleton.cpp: implementation of the CSingleton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h" // only include in MFC applications
#include "Singleton.h"
#include <assert.h>
#include <memory.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef NULL
	#define NULL	0
#endif

#define THIS_FLAG	0x12C76D9 // a special value

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSingleton::CSingleton()
{
	m_pData = NULL;
	m_hFileMapping = NULL;
	m_bFlagged = FALSE;
}

CSingleton::CSingleton(LPCTSTR lpUniqueString)
{
	BOOL bSucceed = Create(lpUniqueString);
	assert(bSucceed);
}

CSingleton::~CSingleton()
{
	// un-map memory file
	if (m_pData != NULL)
	{
		::UnmapViewOfFile(m_pData);
		m_pData = NULL;
	}

	// close memory file handle
	if (m_hFileMapping != NULL)
	{
		::CloseHandle(m_hFileMapping);
		m_hFileMapping = NULL;
	}
}

BOOL CSingleton::Create(LPCTSTR lpUniqueString)
{
	if (lpUniqueString == NULL)
		return FALSE;

	if (m_hFileMapping != NULL) // Already allocated
		return TRUE;

	// Create the mapping file
	m_hFileMapping = ::CreateFileMapping((HANDLE)0xffffffff, // use system page file
							NULL,
							PAGE_READWRITE,
							0,
							sizeof(DWORD) + sizeof(HINSTANCE) + sizeof(HWND),
							lpUniqueString);

	if (m_hFileMapping == NULL)
		return FALSE;

	m_pData = (char*)::MapViewOfFile(m_hFileMapping,
							FILE_MAP_ALL_ACCESS,
							0,
							0,
							sizeof(DWORD) + sizeof(HINSTANCE) + sizeof(HWND));
	if (m_pData == NULL) // Allocation failed
	{
		::CloseHandle(m_hFileMapping);
		m_hFileMapping = NULL;
		return FALSE;
	}

	// All good.
	DWORD dwFlag;
	memcpy(&dwFlag, m_pData, sizeof(DWORD));

	m_bFlagged = (dwFlag == THIS_FLAG);
	if (!m_bFlagged)
	{
		dwFlag = THIS_FLAG;
		memcpy(m_pData, &dwFlag, sizeof(DWORD));
		memset(m_pData + sizeof(DWORD), NULL, sizeof(HINSTANCE) + sizeof(HWND));
	}

	return TRUE;
}

BOOL CSingleton::IsInstanceUnique() const
{
	return !m_bFlagged;
}

HINSTANCE CSingleton::GetPrevInstance() const
{
	HINSTANCE hInstance;
	memcpy(&hInstance, m_pData + sizeof(DWORD), sizeof(HINSTANCE));
	return hInstance;
}

HWND CSingleton::GetPrevWnd() const
{
	HWND hWnd;
	memcpy(&hWnd, m_pData + sizeof(DWORD) + sizeof(HINSTANCE), sizeof(HWND));
	return hWnd;
}

void CSingleton::SetPrevInstance(HINSTANCE hInstance)
{
	memcpy(m_pData + sizeof(DWORD), &hInstance, sizeof(HINSTANCE));
}

void CSingleton::SetPrevWnd(HWND hWnd)
{
	memcpy(m_pData + sizeof(DWORD) + sizeof(HINSTANCE), &hWnd, sizeof(HWND));
}

void CSingleton::RestorePrevWnd() const
{
	assert(m_pData != NULL);
	
	HWND hPrevWnd = GetPrevWnd();
	if (hPrevWnd != NULL)
	{
		HWND hChildWnd = ::GetLastActivePopup(hPrevWnd);

		if (::IsIconic(hPrevWnd))
			::ShowWindow(hPrevWnd, SW_RESTORE);

		if (hChildWnd != NULL)
		{
			if (::IsIconic(hChildWnd))
				::ShowWindow(hChildWnd, SW_RESTORE);
		}

		::SetForegroundWindow(hChildWnd);
	}
}
