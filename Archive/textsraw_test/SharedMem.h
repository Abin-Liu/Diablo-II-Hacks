//////////////////////////////////////////////////////////////////
//			          SharedMem.h
//
// "CSharedMem", an API wrapping class with build-in synchronization
// methods. The purpose of this class is to make sharing memory among
// different processes easy and safe, it frees users from the
// headache of memory maintenance and synchronization. This class does
// not require MFC.
//
// In order to properly establish memory block that is being shared with
// other processes, the following information need to be specified:
//
// 1, Name of the shared memory:  The memory is shared among multiple
//    processes, an unique name is required by operating system to
//    distinguish the memory block. Use of GUID is highly recommended.
//
// 2, Size of the shared memory: The total size(int bytes) of memory to
//    be allocated.
//
// 3, Timeout: If the memory is currently locked by other applications,
//    the time(in milliseconds) to wait before returning FALSE.
//
//
// Copyright, 2002	Bin Liu
//
// Update history:
//
// 2002-7-09	Initial release. (Bin Liu)
// 2002-7-11	Added convenient access for regular data types. (Bin Liu)
// 2003-2-08	The class has been made UNICODE compliant. (Bin Liu)
// 2004-8-26    Fill with zero's if the memory is newly created. (Bin Liu)
//
//////////////////////////////////////////////////////////////////

#ifndef __SHAREDMEM_H__
#define __SHAREDMEM_H__

#define MAX_MUTEX_NAME	512

#include <windows.h>

class CSharedMem  
{
public:	

	// Constructor/destroctor
	CSharedMem();
	virtual ~CSharedMem();

	// Create/close the memory file.
	virtual BOOL Create(LPCTSTR lpUniqueName, DWORD dwSize, DWORD dwTimeOut = 250);
	virtual void Close();

	// Synchronization
	BOOL Lock();
	BOOL Unlock();

	// General memory data access.
	virtual int SetData(LPCVOID pData, DWORD dwSize, DWORD dwStartPosition);
	virtual int GetData(LPVOID pBuffer, DWORD dwSize, DWORD dwStartPosition);
	int EraseAll(); // Fill memory block with zero's

	// Convenient data access(Get) for regular data types
	BYTE GetByte(DWORD dwPosition, int* pResult = NULL); // BYTE
	TCHAR GetChar(DWORD dwPosition, int* pResult = NULL); // char/TCHAR
	short GetShort(DWORD dwPosition, int* pResult = NULL); // short/WORD
	int GetInt(DWORD dwPosition, int* pResult = NULL); // int/UINT
	long GetLong(DWORD dwPosition, int* pResult = NULL); // long/DWORD
	BOOL GetBool(DWORD dwPosition, int* pResult = NULL); // BOOL
	int GetString(LPTSTR lpBuffer, DWORD dwMaxLen, DWORD dwPosition, int* pResult = NULL); // char string

	// Convenient data access(Set) for regular data types
	int SetByte(BYTE b, DWORD dwPosition); // BYTE
	int SetChar(TCHAR ch, DWORD dwPosition); // char/TCHAR
	int SetShort(short n, DWORD dwPosition); // short/WORD
	int SetInt(int n, DWORD dwPosition); // int/UINT
	int SetLong(long l, DWORD dwPosition); // long/DWORD
	int SetBool(BOOL b, DWORD dwPosition); // BOOL
	int SetString(LPCTSTR lpsz, DWORD dwPosition); // char string

	// Memory information
	DWORD GetAllocSize() const; // Retrieve the current allocated memory size
	BOOL IsCreated() const; // Is memory file created?
	DWORD GetTimeOut() const; // Get time out value.

	// Error messages, will be returned if errors occured.
	enum {
		ERR_INVALIDPARAM = 0, // Invalid parameter
		ERR_NOTCREATED = -1, // Shared memory not created yet
		ERR_LOCKFAILED = -2, // Lock failed 
		ERR_INVALIDADDRESS = -3 // Invalid memory address
		};

protected:

	// Protected member methods used for synchronizations
	BOOL InternalLock();
	BOOL InternalUnlock();
	
protected:

	// Member data
	HANDLE m_hMutex; // Mutex used for synchronization
	char* m_pData; // Points to the data area
	HANDLE m_hFileMapping; // File mapping handle
	DWORD m_dwAllocSize; // Memory block size
	BOOL m_bCreated; // Created/non-created flag
	DWORD m_dwTimeOut; // Time out value in milliseconds
	BOOL m_bExternalLocked;
};

#endif