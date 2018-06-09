// SharedMem.cpp: implementation of the CSharedMem class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h" // only include when used in MFC applications
#include "SharedMem.h"
#include <tchar.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSharedMem::CSharedMem()
{
	m_pData = NULL;
	m_hFileMapping = NULL;
	m_dwAllocSize = 0;
	m_bCreated = FALSE;
	m_dwTimeOut = 50;
	m_hMutex = NULL;
}

CSharedMem::~CSharedMem()
{
	Close();
}

// Deallocate the memory and initialize all member data.
void CSharedMem::Close()
{
	// release mutex and close mutex handle
	if (m_hMutex != NULL)
	{
		Unlock();
		::CloseHandle(m_hMutex);
		m_hMutex = NULL;
	}

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

	m_dwAllocSize = 0;
	m_dwTimeOut = 50;
	m_bCreated = FALSE; // Memory no more accessible
}

DWORD CSharedMem::GetAllocSize() const
{
	return m_dwAllocSize;
}

DWORD CSharedMem::GetTimeOut() const
{
	return m_dwTimeOut;
}

BOOL CSharedMem::IsCreated() const
{
	return m_bCreated;
}

BOOL CSharedMem::Create(LPCTSTR lpUniqueName, DWORD dwSize, DWORD dwTimeOut)
{
	if (lpUniqueName == NULL)
		return FALSE;

	const DWORD LEN = ::_tcslen(lpUniqueName); 

	if (m_bCreated || dwSize == 0 || LEN == 0)
		return FALSE; // Bad parameters
	
	m_dwAllocSize = dwSize;
	m_dwTimeOut = dwTimeOut;

	// Create mutex
	// Reuse the GUID as mutex name but add one more character, reason for doing so:
	// 1: User only needs to provider one GUID instead of two.
	// 2: The additional character ensuring that mutex name will not be the same as memory file name.

	TCHAR szMutexName[MAX_MUTEX_NAME + 2] = _T("A"); // should be enough
	::_tcsncpy(szMutexName + 1, lpUniqueName, MAX_MUTEX_NAME);

	m_hMutex = ::CreateMutex(NULL, FALSE, szMutexName);

	if (m_hMutex == NULL) // Mutex creation failed
		return FALSE;

	// Create the mapping file
	m_hFileMapping = ::CreateFileMapping((HANDLE)0xffffffff, // use system page file
							NULL,
							PAGE_READWRITE,
							0,
							m_dwAllocSize,
							lpUniqueName);
	if (m_hFileMapping == NULL) // Allocation failed
		return FALSE;

	m_pData = (char*)::MapViewOfFile(m_hFileMapping,
							FILE_MAP_ALL_ACCESS,
							0,
							0,
							m_dwAllocSize);	
	if (m_pData == NULL) // Allocation failed
	{
		::CloseHandle(m_hFileMapping);
		m_hFileMapping = NULL;
		return FALSE;
	}

	// All good.
	m_bCreated = TRUE;
	return TRUE;
}

BOOL CSharedMem::Lock()
{
	// Lock the memory so other apps are not able to access
	return (::WaitForSingleObject(m_hMutex, m_dwTimeOut) == WAIT_OBJECT_0);
}

BOOL CSharedMem::Unlock()
{
	// Unlock the memory
	return ::ReleaseMutex(m_hMutex); 
}

// Retrieve data from the memory
int CSharedMem::GetData(char* pBuffer, DWORD dwSize, DWORD dwStartPosition)
{
	if (!m_bCreated)
		return ERR_NOTCREATED;
	
	int nBlock = m_dwAllocSize - dwStartPosition;
	if (nBlock < 0) // start position is out of allocated range
		return ERR_INVALIDADDRESS;

	nBlock = min((DWORD)nBlock, dwSize); // Use the safer(smaller) size

	if (!Lock())
		return ERR_LOCKFAILED; // failed to lock memory

	memcpy(pBuffer, m_pData + dwStartPosition, nBlock); // copy data
	Unlock(); // Release mutex

	return nBlock; // Return the actual size(in bytes) copied
}

// Modify the memory contents
int CSharedMem::SetData(const char* pData, DWORD dwSize, DWORD dwStartPosition)
{
	if (!m_bCreated)
		return ERR_NOTCREATED;
	
	int nBlock = m_dwAllocSize - dwStartPosition;
	if (nBlock < 0) // start position is out of allocated range
		return ERR_INVALIDADDRESS;

	nBlock = min((DWORD)nBlock, dwSize);

	if (!Lock())
		return ERR_LOCKFAILED; // failed to lock memory

	memcpy(m_pData + dwStartPosition, pData, nBlock); // copy data

	Unlock();
	return nBlock;
}

BOOL CSharedMem::EraseAll()
{
	if (!m_bCreated)
		return FALSE;

	if (!Lock())
		return FALSE;

	memset(m_pData, 0, m_dwAllocSize); // set all to zero
	Unlock();
	return TRUE;
}

BYTE CSharedMem::GetByte(DWORD dwPosition, int* pResult)
{
	BYTE b;
	int nErrorCode = GetData((char*)&b, sizeof(BYTE), dwPosition);
	if (pResult != NULL)
		*pResult = nErrorCode; // Record occurred errors, if any.
	return b; // Return data read
}

TCHAR CSharedMem::GetChar(DWORD dwPosition, int* pResult)
{
	TCHAR ch;
	int nErrorCode = GetData((char*)&ch, sizeof(TCHAR), dwPosition);
	if (pResult != NULL)
		*pResult = nErrorCode;
	return ch;
}

short CSharedMem::GetShort(DWORD dwPosition, int* pResult)
{
	short n;
	int nErrorCode = GetData((char*)&n, sizeof(short), dwPosition);
	if (pResult != NULL)
		*pResult = nErrorCode;
	return n;
}

int CSharedMem::GetInt(DWORD dwPosition, int* pResult)
{
	int n;
	int nErrorCode = GetData((char*)&n, sizeof(int), dwPosition);
	if (pResult != NULL)
		*pResult = nErrorCode;
	return n;
}

long CSharedMem::GetLong(DWORD dwPosition, int* pResult)
{
	long l;
	int nErrorCode = GetData((char*)&l, sizeof(long), dwPosition);
	if (pResult != NULL)
		*pResult = nErrorCode;
	return l;
}

BOOL CSharedMem::GetBool(DWORD dwPosition, int* pResult)
{
	int n;
	int nErrorCode = GetData((char*)&n, sizeof(BOOL), dwPosition);
	if (pResult != NULL)
		*pResult = nErrorCode;
	return (n != 0);
}

int CSharedMem::SetByte(BYTE b, DWORD dwPosition)
{
	return SetData((const char*)&b, sizeof(BYTE), dwPosition);
}

int CSharedMem::SetChar(TCHAR ch, DWORD dwPosition)
{
	return SetData((const char*)&ch, sizeof(TCHAR), dwPosition);
}

int CSharedMem::SetShort(short n, DWORD dwPosition)
{
	return SetData((const char*)&n, sizeof(short), dwPosition);
}

int CSharedMem::SetInt(int n, DWORD dwPosition)
{
	return SetData((const char*)&n, sizeof(int), dwPosition);
}

int CSharedMem::SetLong(long l, DWORD dwPosition)
{
	return SetData((const char*)&l, sizeof(long), dwPosition);
}

int CSharedMem::SetBool(BOOL b, DWORD dwPosition)
{
	return SetData((const char*)&b, sizeof(BOOL), dwPosition);
}

int CSharedMem::SetString(LPCTSTR lpsz, DWORD dwPosition)
{
	return SetData(lpsz, (_tcslen(lpsz) + 1) * sizeof(TCHAR), dwPosition);
}

int CSharedMem::GetString(LPTSTR lpBuffer, DWORD dwMaxLen, DWORD dwPosition, int *pResult)
{
	memset(lpBuffer, NULL, (dwMaxLen + 1) * sizeof(TCHAR));
	int nBlock = GetData(lpBuffer, dwMaxLen * sizeof(TCHAR), dwPosition);
	if (nBlock <= 0)
	{
		// error occurred
		if (pResult != NULL)
			*pResult = nBlock;
		nBlock = 0;
	}

	return nBlock / sizeof(TCHAR);
}