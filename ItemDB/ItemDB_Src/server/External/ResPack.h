//////////////////////////////////////////////////////////////////////////////////////////////
//		ResPack.h: interface for the CResPack class.
//
// This class is used for packing multiple file/data resources into one file, it will generate
// a package file which stores resource contents and an "index table". The "index table"
// stores resource indices, each of which contains resource-ID, resource-start position and
// resource-size(in bytes). The "index table" is manipulated by a MFC "CMap" object for
// fast lookup. 
//
// This class is particularly useful where there are many resource files(images, multimedia files,
// text files, etc.) need to be included in one project.
//
// Contents in the package file are stored in this format:
//
// (BOF)INDEX_TABLE_START_POSITION(4 bytes) + INDEX_COUNT(4 bytes) + RESOURCE_DATA(vary) + INDEX_TABLE(vary)(EOF)
//
// Written by Abin (abinn32@yahoo.com)
//
// History:
//
// Mar 29, 2004 - Initial release.
//
//////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __RESPACK_H__
#define __RESPACK_H__

#include <afxtempl.h>
enum { PKG_OPEN_NONE = 0, PKG_OPEN_READ, PKG_OPEN_WRITE }; // Package open status

class CResPack
{
public:		
	
	// Constructors & Destructor
	CResPack();
	virtual ~CResPack();	

	// Open/close the package for read/write
	BOOL OpenPackage(LPCTSTR lpszPackagePath, BOOL bForWrite, BOOL TruncateExist = FALSE); // Must be called before read/write the package contents
	void ClosePackage(); // Should be called after read/write operations are done
	int GetOpenStatus() const; // Returns PKG_OPEN_NONE, PKG_OPEN_READ, or PKG_OPEN_WRITE

	// Package attributes
	CString GetPackagePathName() const; // Path of the package file
	DWORD GetResCount() const; // Number of resources contained in the package file
	DWORD GetResSize(DWORD dwResID) const; // Get contents size of a resource, identified by a resource ID

	// Pack new data into the package file, fail if the resource ID already exists
	BOOL Add(DWORD dwResID, LPCTSTR lpszSrcFile); // Add whole contents of a file
	BOOL Add(DWORD dwResID, const BYTE* lpData, DWORD dwDataSize); // Add a block of data

	// Modify/Remove resources from the package file
	BOOL ModifyRes(DWORD dwOldID, DWORD dwNewID);
	BOOL ModifyRes(DWORD dwResID, LPCTSTR lpszNewSrcFile);
	BOOL ModifyRes(DWORD dwResID, const BYTE* lpNewData, DWORD dwNewDataSize);
	BOOL Remove(DWORD dwResID);
	BOOL RemoveAll();

	// Extract contents from the package file
	DWORD Extract(DWORD dwResID, BYTE* pBuffer, DWORD dwMaxBufSize, DWORD dwStartPosition = 0); // Extract a resource from the package
	BOOL SaveResToFile(DWORD dwResID, LPCTSTR lpszFileName, BOOL bCreateFile = TRUE); // Extract a resource from the package file, and save its contents to a new file

	// Iterate all resource ID's and sizes in the package file
	POSITION GetFirstResPosition() const;
	void GetNextRes(POSITION& nextPosition, DWORD& dwResID, DWORD& dwResSize) const;

	// Verify a package file, return TRUE if it is in a valid package format, FALSE otherwise
	static BOOL VerifyPackage(LPCTSTR lpszFileName);

protected:
	BOOL VerifyIndex(DWORD dwIndexStartPos, DWORD dwResID, POINT rpi) const; // Verify an index entry
	BOOL LoadIndexTable(DWORD* pdwIdxStartPos); // Load the index table from the package file
	int m_nOpenFlag; // File opened flag
	CFile m_package; // The package file
	CMap<DWORD, DWORD, POINT, POINT> m_index; // The index table, store indices for fast lookup
};

#endif // __RESPACK_H__