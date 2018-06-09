//////////////////////////////////////////////////////////////////////
// StringEx.cpp
//
// String manipulation without MFC or std::string. Unicode compliant.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h" // Uncomment this line if getting "fatal error C1010"!
#include "StringEx.h"
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <tchar.h>

#define DEFAULT_BUFFER		64
#define VALIDSTR(p) ((p) ? (p) : _T(""))

//////////////////////////////////////////////////////////////////
// CStringEx Class Implementation
//////////////////////////////////////////////////////////////////
CStringEx::CStringEx()
{
	Initialize();
}

CStringEx::CStringEx(LPCTSTR lpszSrc, int nLength)
{
	Initialize();
	Copy(lpszSrc, nLength);
}

CStringEx::CStringEx(TCHAR chSrc, int nCount)
{
	Initialize();
	Copy(chSrc, nCount);
}

CStringEx::CStringEx(const CStringEx& rSrc, int nLength)
{
	Initialize();
	Copy(rSrc, nLength);
}

CStringEx::~CStringEx()
{
	// Free all allocated buffers
	FreeBuffer();	
	if (m_pszTemp)
		delete [] m_pszTemp;

	if (m_pszDelimitors)
		::free(m_pszDelimitors);
}

CStringEx::operator LPCTSTR() const
{
	return VALIDSTR(m_pszData);
}

const CStringEx& CStringEx::operator=(LPCTSTR lpszSrc)
{
	Copy(lpszSrc);
	return *this;
}

const CStringEx& CStringEx::operator=(TCHAR ch)
{
	Copy(ch, 1);
	return *this;
}

void CStringEx::FreeBuffer()
{
	if (m_pszData)
	{
		delete [] m_pszData;
		m_pszData = NULL;
	}

	m_nBufferLen = 0;
}

void CStringEx::FreeExtra()
{
	if (m_pszData == NULL || m_nBufferLen - 1 <= m_nStrLen)
		return; // no extra space

	m_nBufferLen = m_nStrLen + 1;
	LPTSTR pszNew = new TCHAR[m_nBufferLen];
	assert(pszNew);
	::memcpy(pszNew, m_pszData, (m_nBufferLen) * sizeof(TCHAR));
	FreeBuffer();
	m_pszData = pszNew;
}

int CStringEx::ReallocBuffer(int nLen, BOOL bKeepOriginalData)
{
	if (nLen <= 0)
		return 0;

	// we have enough buffer already
	if (m_nBufferLen - 1 >= nLen)
		return nLen;

	// need to reallocate, enlarge the buffer by doubling its size
	int nNewLen = m_nBufferLen - 1;
	if (nNewLen <= 0)
		nNewLen = DEFAULT_BUFFER;

	while (nNewLen < nLen)
		nNewLen *= 2;

	LPTSTR pszNew = new TCHAR[nNewLen + 1];
	assert(pszNew);
	pszNew[0] = _T('\0');

	if (bKeepOriginalData && m_pszData)
		::memcpy(pszNew, m_pszData, (m_nStrLen + 1) * sizeof(TCHAR));

	FreeBuffer();	
	m_pszData = pszNew;
	m_nBufferLen = nNewLen + 1;
	return nLen;
}

void CStringEx::Initialize()
{
	m_pszTemp = NULL;
	m_nTempLen = 0;
	m_nBufferLen = DEFAULT_BUFFER + 1;
	m_nStrLen = 0;
	m_pszData = new TCHAR[m_nBufferLen]; // the string buffer will never be null
	assert(m_pszData);
	m_pszData[0] = _T('\0');
	m_pszDelimitors = NULL;
	m_lpszToken = NULL;
	m_bWholeMatch = FALSE;
	m_nDelimitorLen = 0;
}

int CStringEx::GetLength() const
{
	return m_nStrLen;
}

BOOL CStringEx::IsEmpty() const
{
	return m_nStrLen <= 0;
}

void CStringEx::Empty()
{
	m_pszData[0] = _T('\0');
	m_nStrLen = 0;
}

TCHAR CStringEx::GetAt(int nIndex) const
{
	if (nIndex >= 0 && nIndex < m_nStrLen)
	{
		return m_pszData[nIndex];
	}
	else
	{
		assert(FALSE);
		return _T('\0');
	}	
}

TCHAR CStringEx::operator[](int nIndex) const
{
	return GetAt(nIndex);
}

void CStringEx::SetAt(int nIndex, TCHAR ch)
{
	if (nIndex >= 0 && nIndex < m_nStrLen)
	{
		m_pszData[nIndex] = ch;
		if (ch == _T('\0'))
			m_nStrLen = nIndex;
	}
	else
	{
		assert(FALSE);
	}	
}

int CStringEx::Compare(LPCTSTR lpsz) const
{
	return _tcscmp(m_pszData, VALIDSTR(lpsz));
}

int CStringEx::CompareNoCase(LPCTSTR lpsz) const
{
	return _tcsicmp(m_pszData, VALIDSTR(lpsz));
}

CStringEx CStringEx::Mid(int nFirst, int nCount) const
{
	if (IsEmpty() || nCount == 0)
		return _T("");

	if (nFirst < 0)
		nFirst = 0;
	if (nFirst >= m_nStrLen)
		nFirst = m_nStrLen - 1;

	if (nCount < 0)
		nCount = m_nStrLen - nFirst;
	else
		nCount = min(nCount, m_nStrLen - nFirst);

	CStringEx temp(&m_pszData[nFirst]);
	temp.m_pszData[nCount] = _T('\0');
	temp.m_nStrLen = nCount;
	return temp;
}

CStringEx CStringEx::Left(int nCount) const
{
	if (IsEmpty() || nCount <= 0)
		return _T("");

	nCount = min(nCount, m_nStrLen);
	CStringEx temp(m_pszData);
	temp.m_pszData[nCount] = _T('\0');
	temp.m_nStrLen = nCount;
	return temp;
}

CStringEx CStringEx::Right(int nCount) const
{
	if (IsEmpty() || nCount <= 0)
		return _T("");

	nCount = min(nCount, m_nStrLen);
	const int FIRST = m_nStrLen - nCount;
	CStringEx temp(&m_pszData[FIRST]);
	return temp;
}

void CStringEx::MakeUpper()
{	
	_tcsupr(m_pszData);	
}

void CStringEx::MakeLower()
{
	_tcslwr(m_pszData);
}

void CStringEx::MakeReverse()
{
	_tcsrev(m_pszData);
}

int CStringEx::Replace(TCHAR chOld, TCHAR chNew)
{
	if (chOld == _T('\0'))
		return 0;
	
	int nCount = 0;
	for (int i = 0; i < m_nStrLen; i++)
	{
		if (m_pszData[i] == chOld)
		{
			m_pszData[i] = chNew;
			nCount++;
			if (chNew == _T('\0'))
			{
				m_nStrLen = i;
				break;
			}
		}
	}

	return nCount;
}

int CStringEx::Replace(LPCTSTR lpszOld, LPCTSTR lpszNew)
{
	if (lpszOld == NULL || lpszNew == NULL)
		return 0;

	const int LENOLD = _tcslen(lpszOld);
	if (LENOLD == 0)
		return 0;

	const int LENNEW = _tcslen(lpszNew);

	const int DIFF = LENNEW - LENOLD;
	int nCount = 0;
	LPTSTR p = m_pszData;

	while ((p = _tcsstr(p, lpszOld)) != NULL) // make it clear
	{
		nCount++;
		const int IDX = Ptr2Idx(p);

		if (DIFF == 0)
		{
			// no need to change our buffer
		}
		else if (DIFF < 0)
		{
			// shrink(not really, should say "left shift") our buffer
			LeftShift(IDX - DIFF, -DIFF);
		}
		else
		{
			// inflate our buffer
			RightShift(IDX, DIFF);
			
		}

		if (LENNEW)
			::memcpy(p, lpszNew, LENNEW * sizeof(TCHAR));

		p = &p[LENNEW];
	}

	return nCount;
}

void CStringEx::LeftShift(int nIndex, int nDistance)
{
	if (nDistance == 0)
		return;

	const int LEN = m_nStrLen - nIndex + 1;
	::memmove(&m_pszData[nIndex - nDistance], &m_pszData[nIndex], LEN * sizeof(TCHAR));
	m_nStrLen -= nDistance;
}

void CStringEx::RightShift(int nIndex, int nDistance)
{
	if (nDistance == 0)
		return;

	ReallocBuffer(m_nStrLen + nDistance, TRUE);
	const int LEN = m_nStrLen - nIndex + 1;
	::memmove(&m_pszData[nIndex + nDistance], &m_pszData[nIndex], LEN * sizeof(TCHAR));
	m_nStrLen += nDistance;
}

int CStringEx::Remove(TCHAR ch)
{
	if (ch == _T('\0'))
		return 0;
	
	int nCount = 0;
	for (int i = 0; i < m_nStrLen; i++)
	{
		if (m_pszData[i] == ch)
		{			
			nCount++;
			LeftShift(i + 1, 1);
		}
	}
	
	return nCount;
}

int CStringEx::Insert(int nIndex, TCHAR ch)
{
	if (nIndex >= m_nStrLen)
		nIndex = m_nStrLen - 1;

	if (nIndex < 0)
		nIndex = 0;

	RightShift(nIndex, 1);
	m_pszData[nIndex] = ch;
	if (ch == _T('\0'))
		m_nStrLen = nIndex;

	return nIndex + 1;		
}

int CStringEx::Insert(int nIndex, LPCTSTR lpsz)
{
	if (lpsz == NULL || lpsz[0] == _T('\0'))
		return nIndex;
	
	if (nIndex >= m_nStrLen)
		nIndex = m_nStrLen - 1;

	if (nIndex < 0)
		nIndex = 0;

	const int LEN = _tcslen(lpsz);
	RightShift(nIndex, LEN);
	::memcpy(&m_pszData[nIndex], lpsz, LEN * sizeof(TCHAR));
	return nIndex + LEN;
}

int CStringEx::Delete(int nIndex, int nCount)
{
	if (IsEmpty() || nIndex < 0 || nIndex >= m_nStrLen || nCount <= 0)
		return 0;

	if (nCount > m_nStrLen - nIndex)
		nCount = m_nStrLen - nIndex;

	LeftShift(nIndex + nCount, nCount);
	return nCount;
}

void CStringEx::Format(LPCTSTR lpszFormat, ...)
{
	if (lpszFormat == NULL)
		return;

	va_list args;
	va_start(args, lpszFormat);
	FormatV(lpszFormat, args);
	va_end(args);
}

void CStringEx::FormatV(LPCTSTR lpszFormat, va_list argList)
{
	Empty();
	if (lpszFormat == NULL)
		return;

	_vsntprintf(m_pszData, m_nBufferLen - 1, lpszFormat, argList);
	int nLen = _tcslen(m_pszData);
	while (nLen >= m_nBufferLen - 1) 
	{
		ReallocBuffer((m_nBufferLen - 1) * 2, FALSE);
		_vsntprintf(m_pszData, m_nBufferLen - 1, lpszFormat, argList);
		nLen = _tcslen(m_pszData);
	}	

	m_nStrLen = nLen;
}

void CStringEx::TrimLeft()
{
	int nIndex = 0;
	while (IsBlank(m_pszData[nIndex]))
		nIndex++;
	LeftShift(nIndex, nIndex);
}

void CStringEx::TrimLeft(TCHAR chTarget)
{
	if (chTarget == _T('\0'))
		return;

	int nCount = 0;
	while (m_pszData[nCount] == chTarget)
		nCount++;

	LeftShift(nCount, nCount);	
}

void CStringEx::TrimLeft(LPCTSTR lpszTargets)
{
	if (lpszTargets == NULL || lpszTargets[0] == _T('\0'))
		return;

	const int LEN = _tcslen(lpszTargets);
	int nCount = 0;	
	LPCTSTR p = m_pszData;
	while ((p = _tcsstr(p, lpszTargets)) != NULL)
	{
		nCount++;
		p = &p[LEN];
	}

	LeftShift(nCount * LEN, nCount * 3);
}

void CStringEx::TrimRight()
{
	for (int i = m_nStrLen - 1; i >= 0 && IsBlank(m_pszData[i]); i--)
	{
		m_pszData[i] = _T('\0');
		m_nStrLen--;
	}
}

void CStringEx::TrimRight(TCHAR chTarget)
{
	if (chTarget == _T('\0'))
		return;
	
	for (int i = m_nStrLen - 1; i >= 0 && m_pszData[i] == chTarget; i--)
	{
		m_pszData[i] = _T('\0');
		m_nStrLen--;
	}
}

void CStringEx::TrimRight(LPCTSTR lpszTargets)
{
	if (lpszTargets == NULL || lpszTargets[0] == _T('\0'))
		return;

	const int LEN = _tcslen(lpszTargets);
	for (int i = m_nStrLen - LEN; i >= 0 && ::memcmp(&m_pszData[i], lpszTargets, LEN * sizeof(TCHAR)) == 0; i -= LEN)
	{
		m_pszData[i] = _T('\0');
		m_nStrLen -= LEN;
	}
}

void CStringEx::Trim()
{
	TrimRight();
	TrimLeft();
}

void CStringEx::Trim(TCHAR chTarget)
{
	TrimRight(chTarget);
	TrimLeft(chTarget);
}

void CStringEx::Trim(LPCTSTR lpszTargets)
{
	TrimRight(lpszTargets);
	TrimLeft(lpszTargets);
}

int CStringEx::Find(TCHAR ch, int nStart) const
{
	if (ch == _T('\0') || nStart < 0 || nStart >= m_nStrLen - 1)
		return -1;

	if (nStart)
		nStart++;

	LPCTSTR p = _tcschr(&m_pszData[nStart], ch);
	if (p == NULL)
		return -1;

	return Ptr2Idx(p);
}

int CStringEx::Find(LPCTSTR lpsz, int nStart) const
{
	if (lpsz == NULL || lpsz[0] == _T('\0') || nStart < 0 || nStart >= m_nStrLen - 1)
		return -1;

	if (nStart)
		nStart++;

	LPCTSTR p = _tcsstr(&m_pszData[nStart], lpsz);
	if (p == NULL)
		return -1;

	return Ptr2Idx(p);
}

int CStringEx::ReverseFind(TCHAR ch, int nStartBefore) const
{
	if (ch == _T('\0'))
		return -1;

	if (nStartBefore < 0 || nStartBefore > m_nStrLen)
		nStartBefore = m_nStrLen;

	nStartBefore--;

	for (int i = nStartBefore; i >= 0; i--)
	{
		if (m_pszData[i] == ch)
			return i;
	}

	return -1;
}

int CStringEx::ReverseFind(LPCTSTR lpsz, int nStartBefore) const
{
	if (lpsz == NULL || lpsz[0] == _T('\0'))
		return -1;

	if (nStartBefore < 0 || nStartBefore > m_nStrLen)
		nStartBefore = m_nStrLen;

	nStartBefore--;

	const int LEN = _tcslen(lpsz);	

	for (int i = nStartBefore; i >= 0; i--)
	{
		if (::memcmp(&m_pszData[i], lpsz, LEN * sizeof(TCHAR)) == 0)
			return i;
	}

	return -1;
}

int CStringEx::FindOneOf(LPCTSTR lpszCharSet) const
{
	if (lpszCharSet == NULL || lpszCharSet[0] == _T('\0'))
		return -1;

	LPCTSTR p = _tcspbrk(m_pszData, lpszCharSet);
	if (p == NULL)
		return -1;

	return Ptr2Idx(p);
}

int CStringEx::FindNoneOf(LPCTSTR lpszCharSet) const
{
	if (lpszCharSet == NULL || lpszCharSet[0] == _T('\0'))
		return -1;

	for (int i = 0; i < m_nStrLen; i++)
	{
		if (_tcschr(lpszCharSet, m_pszData[i]) == NULL)
			return i;
	}

	return -1;
}

LPTSTR CStringEx::GetBuffer(int nMinBufLength)
{
	if (m_pszTemp)
	{
		assert(FALSE);
		return NULL; // a previous buffer has not been released yet!
	}
	
	if (nMinBufLength < m_nStrLen)
		nMinBufLength = m_nStrLen;

	m_nTempLen = nMinBufLength;
	m_pszTemp = new TCHAR[nMinBufLength + 1];
	assert(m_pszTemp);
	::memcpy(m_pszTemp, m_pszData, (m_nStrLen + 1) * sizeof(TCHAR));
	return m_pszTemp;
}

LPTSTR CStringEx::GetBufferSetLength(int nNewLength)
{
	if (nNewLength < 0)
		nNewLength = 0;

	ReallocBuffer(nNewLength, TRUE);
	::memset(&m_pszData[nNewLength], 0x00, (m_nBufferLen - nNewLength) * sizeof(TCHAR));
	m_nStrLen = nNewLength;
	return m_pszData;
}

void CStringEx::ReleaseBuffer(int nNewLength)
{
	if (m_pszTemp == NULL)
		return;

	const int LEN = _tcslen(m_pszTemp);

	if (nNewLength < 0)
		nNewLength = LEN;

	if (nNewLength > m_nTempLen)
		nNewLength = m_nTempLen; // so user won't be able to get a 10 length buffer and release as 20

	if (nNewLength > LEN)
		nNewLength = LEN;

	m_pszTemp[nNewLength] = _T('\0');
	ReallocBuffer(nNewLength, FALSE);
	::memcpy(m_pszData, m_pszTemp, (nNewLength + 1) * sizeof(TCHAR));
	m_nStrLen = nNewLength;

	// release the temp buffer
	m_nTempLen = 0;
	if (m_pszTemp)
	{
		delete [] m_pszTemp;
		m_pszTemp = NULL;
	}
}

int CStringEx::Collate(LPCTSTR lpsz) const
{
	return _tcscoll(m_pszData, VALIDSTR(lpsz));
}

int CStringEx::CollateNoCase(LPCTSTR lpsz) const
{
	return _tcsicoll(m_pszData, VALIDSTR(lpsz));
}

const CStringEx& CStringEx::operator=(const CStringEx& rhs)
{
	Copy(rhs);
	return *this;
}

const CStringEx& CStringEx::operator+=(const CStringEx& rhs)
{
	Append(rhs);	
	return *this;
}

void CStringEx::Append(const CStringEx &rSrc, int nLength)
{
	const int SRCLEN = rSrc.GetLength();	
	
	if (nLength < 0 || nLength > SRCLEN)
		nLength = SRCLEN;

	if (nLength == 0)
		return;

	ReallocBuffer(m_nStrLen + nLength, TRUE);
	::memcpy(&m_pszData[m_nStrLen], rSrc.m_pszData, nLength * sizeof(TCHAR));
	m_nStrLen += nLength;
	m_pszData[m_nStrLen] = _T('\0');
}

void CStringEx::Append(LPCTSTR lpsz, int nLength)
{
	if (lpsz == NULL || nLength == 0)
		return;

	const int SRCLEN = _tcslen(lpsz);	
	
	if (nLength < 0 || nLength > SRCLEN)
		nLength = SRCLEN;

	if (nLength == 0)
		return;

	ReallocBuffer(m_nStrLen + nLength, TRUE);
	::memcpy(&m_pszData[m_nStrLen], lpsz, nLength * sizeof(TCHAR));
	m_nStrLen += nLength;
	m_pszData[m_nStrLen] = _T('\0');
}

void CStringEx::Append(TCHAR ch, int nCount)
{
	if (ch == _T('\0') || nCount <= 0)
		return;

	ReallocBuffer(m_nStrLen + nCount, TRUE);
	for (int i = 0; i < nCount; i++)
		m_pszData[m_nStrLen + i] = ch;
	m_nStrLen += nCount;
	m_pszData[m_nStrLen] = _T('\0');
}

void CStringEx::Copy(TCHAR ch, int nCount)
{
	if (nCount <= 0)
		return;

	ReallocBuffer(nCount, FALSE);
	m_nStrLen = ch == _T('\0') ? 0 : nCount;
	for (int i = 0; i < m_nStrLen; i++)
		m_pszData[i] = ch;
	m_pszData[m_nStrLen] = _T('\0');
}

void CStringEx::Copy(LPCTSTR lpsz, int nLength)
{
	if (lpsz == m_pszData)
		return;

	if (nLength == 0)
	{
		Empty();
		return;
	}

	const int SRCLEN = _tcslen(VALIDSTR(lpsz));
	if (nLength < 0 || nLength > SRCLEN)
		nLength = SRCLEN;

	if (nLength == 0)
	{
		Empty();
		return;
	}

	ReallocBuffer(nLength, FALSE);
	::memcpy(m_pszData, lpsz, nLength * sizeof(TCHAR));
	m_nStrLen = nLength;
	m_pszData[m_nStrLen] = _T('\0');
}

void CStringEx::Copy(const CStringEx& rSrc, int nLength)
{
	if (this == &rSrc)
		return;

	if (nLength == 0)
	{
		Empty();
		return;
	}

	const int SRCLEN = rSrc.GetLength();
	if (nLength < 0 || nLength > SRCLEN)
		nLength = SRCLEN;

	if (nLength == 0)
	{
		Empty();
		return;
	}

	ReallocBuffer(nLength, FALSE);
	::memcpy(m_pszData, rSrc.m_pszData, nLength * sizeof(TCHAR));
	m_nStrLen = nLength;
	m_pszData[m_nStrLen] = _T('\0');
}

void CStringEx::ToBinary(DWORD dwValue, int nMinWidth)
{
	TCHAR szTemp[33] = _T("");
	int nIndex = 0;	
	do
	{
		szTemp[nIndex++] = (dwValue % 2) ? _T('1') : _T('0');
		dwValue /= 2;
	} while (dwValue);

	Copy(szTemp);

	if (nMinWidth < m_nStrLen)
		nMinWidth = m_nStrLen;

	Append(_T('0'), nMinWidth - m_nStrLen);
	MakeReverse();
}

BOOL CStringEx::FindToken(LPCTSTR lpszDelimitors, BOOL bWholeMatch)
{
	m_bWholeMatch = bWholeMatch;
	m_lpszToken = NULL;
	if (m_pszDelimitors)
	{
		::free(m_pszDelimitors);
		m_pszDelimitors = NULL;
	}

	m_pszDelimitors = _tcsdup(VALIDSTR(lpszDelimitors));
	assert(m_pszDelimitors);
	m_nDelimitorLen = _tcslen(m_pszDelimitors);

	if (IsEmpty())
		return FALSE;

	m_lpszToken = m_pszData;

	if (m_nDelimitorLen == 0)
		return TRUE;

	return AdvanceToken();
}

BOOL CStringEx::FindNextToken(CStringEx &rStr)
{
	rStr.Empty();
	if (!IsValidPtr(m_lpszToken))
		return FALSE;
	
	if (m_nDelimitorLen == 0)
	{
		rStr.Copy(m_pszData);
		m_lpszToken = NULL;
		return FALSE;
	}

	// skip all leading delimitors	
	if (!AdvanceToken())
		return FALSE;

	// Copy
	while (m_lpszToken[0] != _T('\0') && !MatchDelimitor(m_lpszToken))
	{
		rStr.Append(m_lpszToken[0]);
		m_lpszToken = &m_lpszToken[1];
	}

	// skip all delimitors
	return AdvanceToken();
}

BOOL CStringEx::IsValidPtr(LPCTSTR ptr) const
{
	const int IDX = Ptr2Idx(ptr);
	return IDX >= 0 && IDX < m_nStrLen;
}

BOOL CStringEx::AdvanceToken()
{
	if (m_lpszToken == NULL || m_pszDelimitors == NULL || m_pszDelimitors[0] == _T('\0'))
		return FALSE;

	while (MatchDelimitor(m_lpszToken))
		m_lpszToken = &m_lpszToken[1];

	if (!IsValidPtr(m_lpszToken))
		m_lpszToken = NULL;

	return m_lpszToken != NULL;	
}

int CStringEx::Occurence(TCHAR ch) const
{
	if (ch == '\0')
		return 0;

	int nCount = 0;
	for (int i = 0; i < m_nStrLen; i++)
	{
		if (m_pszData[i] == ch)
			nCount++;
	}

	return nCount;
}

int CStringEx::Occurence(LPCTSTR lpsz) const
{
	if (lpsz == NULL || lpsz[0] == _T('\0'))
		return 0;

	int nCount = 0;

	for (int i = 0; i < m_nStrLen; i++)
	{
		if (_tcsstr(&m_pszData[i], lpsz))
			nCount++;
	}

	return nCount;
}

BOOL CStringEx::MatchDelimitor(LPCTSTR lpsz) const
{
	if (lpsz == NULL || lpsz[0] == _T('\0') || m_pszDelimitors == NULL || m_nDelimitorLen <= 0)
		return FALSE;

	if (!m_bWholeMatch)
		return _tcschr(m_pszDelimitors, lpsz[0]) != NULL;

	return ::memcmp(lpsz, m_pszDelimitors, m_nDelimitorLen * sizeof(TCHAR)) == 0;
}

int CStringEx::Ptr2Idx(LPCTSTR lpsz) const
{
	if ((long)m_pszData == 0)
		return -1;

	return (int)(((long)lpsz - (long)m_pszData) / (long)sizeof(TCHAR));
}

BOOL CStringEx::IsDigit() const
{
	for (int i = 0; i < m_nStrLen; i++)
	{
		if (!IsDigit(m_pszData[i]))
			return FALSE;
	}
	
	return TRUE;
}

BOOL CStringEx::IsAlpha() const
{
	for (int i = 0; i < m_nStrLen; i++)
	{
		if (!IsAlpha(m_pszData[i]))
			return FALSE;
	}
	
	return TRUE;
}

BOOL CStringEx::IsBlank(TCHAR ch)
{
	return ch == _T(' ') || ch == _T('\t') || ch == _T('\r') || ch == _T('\n');
}

BOOL CStringEx::IsAlpha(TCHAR ch)
{
	return _istalpha(ch);
}

BOOL CStringEx::IsDigit(TCHAR ch)
{
	return _istdigit(ch);
}

BOOL CStringEx::IsBlank() const
{
	for (int i = 0; i < m_nStrLen; i++)
	{
		if (!IsBlank(m_pszData[i]))
			return FALSE;
	}

	return TRUE;
}

BOOL CStringEx::IsNumeric(BOOL bInteger) const
{
	if (IsEmpty())
		return FALSE;
	
	BOOL bDotFount = FALSE;
	BOOL bDigitFount = FALSE;
	for (int i = 0; i < m_nStrLen; i++)
	{
		if (IsDigit(m_pszData[i]))
		{
			bDigitFount = TRUE;
			continue;
		}

		if (m_pszData[i] == _T('+') || m_pszData[i] == _T('-'))
		{
			if (i == 0)
				continue;
		}

		if (m_pszData[i] == _T('.') && !bInteger && !bDotFount && bDigitFount)
		{
			bDotFount = TRUE;
			continue;
		}

		return FALSE;
	}

	return bDigitFount && m_pszData[m_nStrLen - 1] != _T('.');
}

BOOL CStringEx::IsHex(TCHAR ch)
{
	return (ch >= _T('0') && ch <= _T('9')) || (ch >= _T('a') && ch <= _T('f')) || (ch >= _T('A') && ch <= _T('F'));
}

BOOL CStringEx::IsHex() const
{
	for (int i = 0; i < m_nStrLen; i++)
	{
		if (!IsHex(m_pszData[i]))
			return FALSE;
	}

	return TRUE;
}

const CStringEx& CStringEx::operator+=(LPCTSTR lpszSrc)
{
	Append(lpszSrc);
	return *this;
}

const CStringEx& CStringEx::operator+=(TCHAR ch)
{
	Append(ch, 1);
	return *this;
}

void CStringEx::DeleteInclusive(LPCTSTR lpszCharSet)
{
	if (IsEmpty() || lpszCharSet == NULL || *lpszCharSet == _T('\0'))
		return;

	LPTSTR psz = new TCHAR[m_nStrLen + 1];
	int nCount = 0;
	for (int i = 0; i < m_nStrLen; i++)
	{
		if (_tcschr(lpszCharSet, m_pszData[i]) == NULL)
			psz[nCount++] = m_pszData[i];
	}
	
	if (nCount < m_nStrLen)
	{
		::memcpy(m_pszData, psz, nCount * sizeof(TCHAR));
		m_pszData[nCount] = _T('\0');
		m_nStrLen = nCount;
	}

	delete [] psz;
}

void CStringEx::DeleteExclusive(LPCTSTR lpszCharSet)
{
	if (IsEmpty() || lpszCharSet == NULL)
		return;

	if (*lpszCharSet == _T('\0'))
	{
		Empty();
		return;
	}

	LPTSTR psz = new TCHAR[m_nStrLen + 1];
	int nCount = 0;
	for (int i = 0; i < m_nStrLen; i++)
	{
		if (_tcschr(lpszCharSet, m_pszData[i]))
			psz[nCount++] = m_pszData[i];
	}
	
	if (nCount < m_nStrLen)
	{
		::memcpy(m_pszData, psz, nCount * sizeof(TCHAR));
		m_pszData[nCount] = _T('\0');
		m_nStrLen = nCount;
	}

	delete [] psz;
}
//////////////////////////////////////////////////////////////////
// End of CStringEx Class Implementation
//////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////
// Global Operators Implementation
//////////////////////////////////////////////////////////////////
CStringEx operator+(const CStringEx& lhs, TCHAR rhs)
{
	CStringEx temp(lhs);
	temp.Append(rhs, 1);
	return temp;
}

CStringEx operator+(TCHAR lhs, const CStringEx& rhs)
{
	CStringEx temp(lhs);
	temp.Append(rhs);
	return temp;
}

CStringEx operator+(const CStringEx& lhs, LPCTSTR rhs)
{
	CStringEx temp(lhs);
	temp.Append(rhs);
	return temp;
}

CStringEx operator+(LPCTSTR lhs, const CStringEx& rhs)
{
	CStringEx temp(lhs);
	temp.Append(rhs);
	return temp;
}

BOOL operator==(const CStringEx& lhs, LPCTSTR rhs)
{
	return _tcscmp((LPCTSTR)lhs, VALIDSTR(rhs)) == 0;
}

BOOL operator!=(const CStringEx& lhs, LPCTSTR rhs)
{
	return _tcscmp((LPCTSTR)lhs, VALIDSTR(rhs)) != 0;
}

BOOL operator<(const CStringEx& lhs, LPCTSTR rhs)
{
	return _tcscmp((LPCTSTR)lhs, VALIDSTR(rhs)) < 0;
}

BOOL operator<=(const CStringEx& lhs, LPCTSTR rhs)
{
	return _tcscmp((LPCTSTR)lhs, VALIDSTR(rhs)) <= 0;
}

BOOL operator>(const CStringEx& lhs, LPCTSTR rhs)
{
	return _tcscmp((LPCTSTR)lhs, VALIDSTR(rhs)) > 0;
}

BOOL operator>=(const CStringEx& lhs, LPCTSTR rhs)
{
	return _tcscmp((LPCTSTR)lhs, VALIDSTR(rhs)) >= 0;
}

BOOL operator==(LPCTSTR lhs, const CStringEx& rhs)
{
	return _tcscmp(VALIDSTR(lhs), (LPCTSTR)rhs) == 0;
}

BOOL operator!=(LPCTSTR lhs, const CStringEx& rhs)
{
	return _tcscmp(VALIDSTR(lhs), (LPCTSTR)rhs) != 0;
}

BOOL operator<(LPCTSTR lhs, const CStringEx& rhs)
{
	return _tcscmp(VALIDSTR(lhs), (LPCTSTR)rhs) < 0;
}

BOOL operator<=(LPCTSTR lhs, const CStringEx& rhs)
{
	return _tcscmp(VALIDSTR(lhs), (LPCTSTR)rhs) <= 0;
}

BOOL operator>(LPCTSTR lhs, const CStringEx& rhs)
{
	return _tcscmp(VALIDSTR(lhs), (LPCTSTR)rhs) > 0;
}

BOOL operator>=(LPCTSTR lhs, const CStringEx& rhs)
{
	return _tcscmp(VALIDSTR(lhs), (LPCTSTR)rhs) >= 0;
}

CStringEx operator+(const CStringEx& lhs, const CStringEx& rhs)
{
	CStringEx temp(lhs);
	temp.Append(rhs);
	return temp;
}

BOOL operator==(const CStringEx& lhs, const CStringEx& rhs)
{
	return _tcscmp((LPCTSTR)lhs, (LPCTSTR)rhs) == 0;
}

BOOL operator!=(const CStringEx& lhs, const CStringEx& rhs)
{
	return _tcscmp((LPCTSTR)lhs, (LPCTSTR)rhs) != 0;
}

BOOL operator<(const CStringEx& lhs, const CStringEx& rhs)
{
	return _tcscmp((LPCTSTR)lhs, (LPCTSTR)rhs) < 0;
}

BOOL operator<=(const CStringEx& lhs, const CStringEx& rhs)
{
	return _tcscmp((LPCTSTR)lhs, (LPCTSTR)rhs) <= 0;
}

BOOL operator>(const CStringEx& lhs, const CStringEx& rhs)
{
	return _tcscmp((LPCTSTR)lhs, (LPCTSTR)rhs) >= 0;
}

BOOL operator>=(const CStringEx& lhs, const CStringEx& rhs)
{
	return _tcscmp((LPCTSTR)lhs, (LPCTSTR)rhs) >= 0;
}
//////////////////////////////////////////////////////////////////
// End of Global Operators Implementation
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
// Note
//
// If you are getting compiler error:
//
// "fatal error C1010: unexpected end of file..."
//
// Please scroll all the way up to the begining of this file and
// uncomment the line:
//
// #include "stdafx.h"
//
//////////////////////////////////////////////////////////////////