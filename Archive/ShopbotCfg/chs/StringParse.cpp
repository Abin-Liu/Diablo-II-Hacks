#include "stdafx.h"
#include "StringParse.h"
#include <tchar.h>
#include <string.h>

CStringParse::CStringParse()
{
	m_lpszString = NULL;
	m_lpszDelimitor = NULL;
}

CStringParse::CStringParse(LPCTSTR lpszString, LPCTSTR lpszDelimitor)
{
	m_lpszString = NULL;
	m_lpszDelimitor = NULL;
	SetStringDelimitor(lpszString, lpszDelimitor);
}

CStringParse::CStringParse(const CStringParse& rhs)
{
	m_lpszString = NULL;
	m_lpszDelimitor = NULL;
	SetStringDelimitor(rhs.m_lpszString, rhs.m_lpszDelimitor);
}

CStringParse::~CStringParse()
{
	FreeData();
}

void CStringParse::FreeData()
{
	if (m_lpszString != NULL)
	{
		delete [] m_lpszString;
		m_lpszString = NULL;
	}

	if (m_lpszDelimitor != NULL)
	{
		delete [] m_lpszDelimitor;
		m_lpszDelimitor = NULL;
	}
}

void CStringParse::SetStringDelimitor(LPCTSTR lpszString, LPCTSTR lpszDelimitor)
{
	FreeData();
	if (lpszString != NULL && lpszString[0] != _T('\0'))
		m_lpszString = _tcsdup(lpszString);

	if (lpszDelimitor != NULL && lpszDelimitor[0] != _T('\0'))
		m_lpszDelimitor = _tcsdup(lpszDelimitor);
}

POSITION CStringParse::GetStartPosition() const
{
	return (POSITION)AdvancePosition(m_lpszString);
}

void CStringParse::GetNextString(POSITION &rPosition, LPTSTR lpszBuffer, DWORD dwMaxChars) const
{
	if (lpszBuffer != NULL)
		::memset(lpszBuffer, 0, dwMaxChars);

	if (!IsValidPosition(rPosition))
	{
		rPosition = NULL;		
		return;
	}

	if (m_lpszDelimitor == NULL)
	{
		_tcsncpy(lpszBuffer, (LPCTSTR)rPosition, dwMaxChars);
		rPosition = NULL;
		return;
	}

	LPCTSTR p = _tcsstr((LPCTSTR)rPosition, m_lpszDelimitor);
	if (p == NULL)
	{
		_tcsncpy(lpszBuffer, (LPCTSTR)rPosition, dwMaxChars);
		rPosition = NULL;
	}
	else
	{
		_tcsncpy(lpszBuffer, (LPCTSTR)rPosition, min(dwMaxChars, ((DWORD)p - (DWORD)rPosition) / sizeof(TCHAR)));
		p = AdvancePosition(p);
		rPosition = (p == NULL || p[0] == _T('\0')) ? NULL : (POSITION)p;
	}
}

BOOL CStringParse::IsValidPosition(POSITION position) const
{
	return m_lpszString != NULL
		&& position != NULL
		&& (DWORD)position >= (DWORD)m_lpszString
		&& (DWORD)position - (DWORD)m_lpszString <= _tcslen(m_lpszString);
}

LPCTSTR CStringParse::AdvancePosition(LPCTSTR lpszCur) const
{
	if (lpszCur == NULL || m_lpszDelimitor == NULL)
		return lpszCur;

	LPCTSTR p = lpszCur;	
	while (p == _tcsstr(p, m_lpszDelimitor))
		p = &p[_tcslen(m_lpszDelimitor)]; 

	return IsValidPosition((POSITION)p) ? p : NULL;
}
