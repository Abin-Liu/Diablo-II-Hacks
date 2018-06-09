////////////////////////////////////////////////////////////////////////////
// BindEntry.cpp
//
// Abin (abinn32@yahoo.com)
////////////////////////////////////////////////////////////////////////////

#include "BindEntry.h"
#include <string.h>
#include "..\\Includes\\D2Hackit.h"
#include "..\\Includes\\D2Client.h"

CBindEntry::CBindEntry()
{
	m_iKeyCode = 0;
	m_iReturnCode = 0;
	m_bRotate = FALSE;
	m_pszCur = NULL;
	m_pszCommands = NULL;
	m_pszDelimited = NULL;
}

CBindEntry::CBindEntry(BYTE iKeyCode, BYTE iReturnCode, LPCSTR lpszCommands)
{
	m_bRotate = FALSE;
	m_pszCur = NULL;
	m_pszCommands = NULL;
	m_pszDelimited = NULL;
	Set(iKeyCode, iReturnCode, lpszCommands);
}

CBindEntry::CBindEntry(const CBindEntry& rhs)
{
	m_bRotate = rhs.m_bRotate;
	m_pszCur = NULL;
	m_pszCommands = NULL;
	m_pszDelimited = NULL;
	Set(rhs.m_iKeyCode, rhs.m_iReturnCode, rhs.m_pszCommands);
}

CBindEntry::~CBindEntry()
{
	FreeBuffer();
}

const CBindEntry& CBindEntry::operator=(const CBindEntry& rhs)
{
	if (this == &rhs)
		return *this;

	FreeBuffer();
	Set(rhs.m_iKeyCode, rhs.m_iReturnCode, rhs.m_pszCommands);
	return *this;
}

void CBindEntry::Set(BYTE iKeyCode, BYTE iReturnCode, LPCSTR lpszCommands)
{
	m_iKeyCode = iKeyCode;
	m_iReturnCode = iReturnCode;
	FreeBuffer();	
	
	if (lpszCommands && *lpszCommands)
		m_pszCommands = ::strdup(lpszCommands);

	if (m_pszCommands)
	{
		const int LEN = ::strlen(m_pszCommands);
		m_pszDelimited = new char[LEN + 2];
		::strcpy(m_pszDelimited, m_pszCommands);
		m_pszDelimited[LEN + 1] = 0;

		for (int i = 0; i < LEN; i++)
		{
			if (m_pszDelimited[i] == ';')
			{
				if (i == 0 || (i < LEN - 1 && m_pszCommands[i + 1] == ';'))
					m_pszDelimited[i] = ' ';
				else
					m_pszDelimited[i] = 0;
			}
		}
	}
}

BYTE CBindEntry::GetKeyCode() const
{
	return m_iKeyCode;
}

BYTE CBindEntry::GetReturnCode() const
{
	return m_iReturnCode;
}

LPCSTR CBindEntry::GetCommands() const
{
	return m_pszCommands;
}

void CBindEntry::FreeBuffer()
{
	m_pszCur = NULL;
	
	if (m_pszCommands)
	{
		::free(m_pszCommands);
		m_pszCommands = NULL;
	}

	if (m_pszDelimited)
	{
		::free(m_pszDelimited);
		m_pszDelimited = NULL;
	}
}

void CBindEntry::InvokeCommands()
{
	if (m_pszCur == NULL)
		m_pszCur = m_pszDelimited;

	if (m_bRotate && m_pszCur)
	{
		server->GameCommandLine((LPSTR)m_pszCur);
		m_pszCur += ::strlen(m_pszCur) + 1;
		if (*m_pszCur == 0)
			m_pszCur = NULL;
	}
	else
	{
		for (LPCSTR p = m_pszDelimited; p && *p; p += ::strlen(p) + 1)
			server->GameCommandLine((LPSTR)p);
	}	
}

void CBindEntry::Clear()
{	
	m_bRotate = FALSE;
	m_iReturnCode = m_iKeyCode;
	FreeBuffer();
}

void CBindEntry::AddCommands(LPCSTR lpszCommands)
{
	if (lpszCommands == NULL || *lpszCommands == 0)
		return;

	int nAddLen = lpszCommands ?  ::strlen(lpszCommands) : 0;
	int nOldLen = m_pszCommands ? ::strlen(m_pszCommands) : 0;
	if (nAddLen == 0)
		return;

	LPSTR pszNew = new char[nAddLen + nOldLen + 10];
	pszNew[0] = 0;
	if (m_pszCommands)
	{
		::strcat(pszNew, m_pszCommands);
		::strcat(pszNew, ";");
	}
	
	::strcat(pszNew, lpszCommands);
	Set(m_iKeyCode, m_iReturnCode, pszNew);
	delete [] pszNew;
}

void CBindEntry::SetRotate(BOOL bSet)
{
	m_bRotate = bSet;
}

BOOL CBindEntry::IsRotate() const
{
	return m_bRotate ? 1 : 0;
}
