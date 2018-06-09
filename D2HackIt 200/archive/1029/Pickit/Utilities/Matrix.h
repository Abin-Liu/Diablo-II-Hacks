////////////////////////////////////////////////////////////////////////////
// CMatrix
//
// Two dimensional array template class.
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////////// 

#ifndef __MATRIX_H__
#define __MATRIX_H__

#define USE_MULTI_THREAD // uncomment this line if not used in multi-thread application

#ifdef USE_MULTI_THREAD
#include "SyncObj.h"
#endif // USE_MULTI_THREAD

//////////////////////////////////////////////////////////////////////
// Necessary Definitions for Non-MFC applications
//////////////////////////////////////////////////////////////////////
#ifndef __AFXWIN_H__ // If non-MFC ...
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <search.h>
#include <assert.h>
#ifndef ASSERT
#define ASSERT(x) assert(x)
#endif
#endif // __AFXWIN_H__

template <class TYPE>
class CMatrix
#ifdef USE_MULTI_THREAD
: public CSyncObj
#endif // USE_MULTI_THREAD
{
public:

	CMatrix();
	virtual ~CMatrix();

	BOOL Create(int cx, int cy);
	BOOL Create(int cx, int cy, const TYPE& initValue);
	void Destroy();

	BOOL IsCreated() const;
	BOOL IsValidIndex(int x, int y) const;
	const TYPE& GetAt(int x, int y) const;
	TYPE& ElementAt(int x, int y);
	BOOL SetAt(int x, int y, const TYPE& data);

	int GetCX() const;
	int GetCY() const;

	TYPE** GetData();
	const TYPE** GetData() const;
	SIZE ExportData(TYPE** ppBuffer, int cx, int cy) const;
	BOOL ExportData(CMatrix<TYPE>& rMatrix) const;
	
	TYPE* operator[] (int nIndex);
	const TYPE* operator[] (int nIndex) const;

protected:
	
	TYPE** m_ppData;
	int m_cx;
	int m_cy;		
};

template <class TYPE>
CMatrix<TYPE>::CMatrix()
{
	m_ppData = NULL;
	m_cx = 0;
	m_cy = 0;
}

template <class TYPE>
CMatrix<TYPE>::~CMatrix()
{
	Destroy();
}

template <class TYPE>
BOOL CMatrix<TYPE>::Create(int cx, int cy)
{
	Destroy();

	if (cx <= 0 || cy <= 0)
		return FALSE;

	m_cx = cx;
	m_cy = cy;
	
	m_ppData = new TYPE*[m_cx];
	if (m_ppData == NULL)
		return FALSE;

	::memset(m_ppData, 0, m_cx * sizeof(TYPE*));
	for (int i = 0; i < m_cx; i++)
	{
		m_ppData[i] = new TYPE[m_cy];
		if (m_ppData[i] == NULL)
		{
			Destroy();
			return FALSE;
		}
	}

	return TRUE;
}

template <class TYPE>
BOOL CMatrix<TYPE>::Create(int cx, int cy, const TYPE& initValue)
{
	if (!Create(cx, cy))
		return FALSE;

	for (int i = 0; i < m_cx; i++)
	{
		for (int j = 0; j < m_cy; j++)
			m_ppData[i][j] = initValue;
	}

	return TRUE;
}

template <class TYPE>
void CMatrix<TYPE>::Destroy()
{
	if (m_ppData)
	{
		for (int i = 0; i < m_cx; i++)
		{
			if (m_ppData[i])
			{
				delete [] m_ppData[i];
				m_ppData[i] = NULL;
			}
		}

		delete [] m_ppData;
		m_ppData = NULL;
	}	

	m_cx = 0;
	m_cy = 0;
}

template <class TYPE>
BOOL CMatrix<TYPE>::IsValidIndex(int x, int y) const
{
	return m_ppData != NULL && x >= 0 && x < m_cx && y >= 0 && y < m_cy;
}

template <class TYPE>
const TYPE& CMatrix<TYPE>::GetAt(int x, int y) const
{
	ASSERT(IsValidIndex(x, y));
	return m_ppData[x][y];
}

template <class TYPE>
TYPE& CMatrix<TYPE>::ElementAt(int x, int y)
{
	ASSERT(IsValidIndex(x, y));
	return m_ppData[x][y];
}

template <class TYPE>
BOOL CMatrix<TYPE>::SetAt(int x, int y, const TYPE& data)
{
	if (!IsValidIndex(x, y))
		return FALSE;

	m_ppData[x][y] = data;
	return TRUE;
}

template <class TYPE>
TYPE** CMatrix<TYPE>::GetData()
{
	return m_ppData;
}

template <class TYPE>
const TYPE** CMatrix<TYPE>::GetData() const
{
	return (const TYPE**)m_ppData;
}

template <class TYPE>
TYPE* CMatrix<TYPE>::operator[] (int nIndex)
{
	if (nIndex < 0 || nIndex >= m_cx)
		return NULL;

	return m_ppData[nIndex];
}

template <class TYPE>
const TYPE* CMatrix<TYPE>::operator[] (int nIndex) const
{
	if (nIndex < 0 || nIndex >= m_cx)
		return NULL;

	return (const TYPE*)m_ppData[nIndex];
}

template <class TYPE>
int CMatrix<TYPE>::GetCX() const
{
	return m_cx;
}

template <class TYPE>
int CMatrix<TYPE>::GetCY() const
{
	return m_cy;
}

template <class TYPE>
BOOL CMatrix<TYPE>::IsCreated() const
{
	return m_ppData != NULL;
}

template <class TYPE>
SIZE CMatrix<TYPE>::ExportData(TYPE** ppBuffer, int cx, int cy) const
{
	SIZE cz = { 0 };
	
	if (ppBuffer == NULL || cx <= 0 || cy <= 0 || !IsCreated())
		return cz;

	cz.cx = min(cx, m_cx);
	cz.cy = min(cy, m_cy);

	for (int i = 0; i < cz.cx; i++)
	{
		for (int j = 0; j < cz.cy; j++)
			ppBuffer[i][j] = m_ppData[i][j];
	}

	return cz;
}

template <class TYPE>
BOOL CMatrix<TYPE>::ExportData(CMatrix<TYPE>& rMatrix) const
{
	if (!IsCreated())
		return FALSE;

	if (rMatrix.GetCX() != m_cx || rMatrix.GetCY() != m_cy)
	{
		if (!rMatrix.Create(m_cx, m_cy))
			return FALSE;
	}

	for (int i = 0; i < m_cx; i++)
	{
		for (int j = 0; j < m_cy; j++)
			rMatrix.m_ppData[i][j] = m_ppData[i][j];
	}

	return TRUE;
}

#endif // __MATRIX_H__