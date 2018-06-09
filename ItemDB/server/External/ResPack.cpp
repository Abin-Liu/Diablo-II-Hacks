// ResPack.cpp: implementation of the CResPack class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ResPack.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define BLOCK_SIZE	1024

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CResPack::CResPack()
{
	m_nOpenFlag = PKG_OPEN_NONE;
}

CResPack::~CResPack()
{
	ClosePackage();
}

BOOL CResPack::Add(DWORD dwResID, LPCTSTR lpszSrcFile)
{
	if (m_nOpenFlag != PKG_OPEN_WRITE || lpszSrcFile == NULL)
		return FALSE;	

	CFile file;
	if (!file.Open(lpszSrcFile, CFile::modeRead))
		return FALSE;

	if (file.GetLength() == 0)
	{
		file.Close();
		return FALSE;
	}

	POINT rpi = { 0 };
	if (m_index.Lookup(dwResID, rpi))
		return FALSE; // ID already exists

	rpi.x = (long)m_package.GetLength();
	rpi.y = (long)file.GetLength();
	m_index.SetAt(dwResID, rpi);
	
	m_package.SeekToEnd();
	BYTE* pData = new BYTE[BLOCK_SIZE];
	ASSERT(pData != NULL);
	while (TRUE)
	{
		DWORD dwRead = file.Read(pData, BLOCK_SIZE);
		if (dwRead > 0)
			m_package.Write(pData, dwRead);
		
		if (dwRead < BLOCK_SIZE)
			break;		
	}
	delete [] pData;
	file.Close();
	return TRUE;	
}

BOOL CResPack::Add(DWORD dwResID, const BYTE *lpData, DWORD dwDataSize)
{
	if (m_nOpenFlag != PKG_OPEN_WRITE || lpData == NULL || dwDataSize == 0)
		return FALSE;

	POINT rpi = { 0 };
	if (m_index.Lookup(dwResID, rpi))
		return FALSE; // ID already exists

	rpi.x = (long)m_package.GetLength();
	rpi.y = (long)dwDataSize;
	m_index.SetAt(dwResID, rpi);

	m_package.SeekToEnd();
	m_package.Write(lpData, dwDataSize); // The contents
	return TRUE;
}

BOOL CResPack::OpenPackage(LPCTSTR lpszPackagePath, BOOL bForWrite, BOOL bTruncateExist)
{
	if (lpszPackagePath == NULL)
		return FALSE;

	ClosePackage();

	if (bForWrite) // for write
	{
		// check for file existence
		UINT nOpenFlag = CFile::modeReadWrite | CFile::shareDenyWrite;
		if (bTruncateExist)
		{
			nOpenFlag |= CFile::modeCreate;
		}
		else
		{
			CFileFind ff;
			if (!ff.FindFile(lpszPackagePath))
				nOpenFlag |= CFile::modeCreate;
		}		

		if (!m_package.Open(lpszPackagePath, nOpenFlag))
			return FALSE;

		DWORD dwIndexTableStartPos = 0;
		LoadIndexTable(&dwIndexTableStartPos);
		dwIndexTableStartPos = max(sizeof(DWORD) * 2, dwIndexTableStartPos);
		m_package.SetLength(dwIndexTableStartPos); // truncate the index table, we will rewrite it later
		m_nOpenFlag = PKG_OPEN_WRITE;
		return TRUE;
	}
	else // for read
	{
		// load the index table
		if (!m_package.Open(lpszPackagePath, CFile::modeRead | CFile::shareDenyWrite))
			return FALSE;

		if (LoadIndexTable(NULL))
		{
			m_nOpenFlag = PKG_OPEN_READ;
			return TRUE;
		}		
		else
		{
			m_package.Close();
			return FALSE;
		}
	}
}

void CResPack::ClosePackage()
{
	if (m_nOpenFlag == PKG_OPEN_NONE)
		return;

	if (m_nOpenFlag == PKG_OPEN_WRITE)
	{		
		const DWORD IDX_POS = m_package.GetLength();
		const DWORD TDX_COUNT = m_index.GetCount();
	
		// write the index table
		m_package.SeekToEnd();
		DWORD dwResID;
		POINT rpi;
		POSITION pos = m_index.GetStartPosition();
		while (pos != NULL)
		{
			m_index.GetNextAssoc(pos, dwResID, rpi);
			m_package.Write(&dwResID, sizeof(DWORD));
			m_package.Write(&rpi, sizeof(POINT));
		}

		m_package.SeekToBegin(); // return to the beginning of file, where the start position of index table is stored
		m_package.Write(&IDX_POS, sizeof(DWORD)); // write the index table start position
		m_package.Write(&TDX_COUNT, sizeof(DWORD)); // write the index count
	}	
	
	m_package.Close();
	m_nOpenFlag = PKG_OPEN_NONE;
}

DWORD CResPack::Extract(DWORD dwResID, BYTE *pBuffer, DWORD dwMaxBufSize, DWORD dwStartPosition)
{
	if (m_nOpenFlag == PKG_OPEN_NONE || pBuffer == NULL || dwMaxBufSize == 0)
		return 0;

	POINT rpi = { 0 };
	if (!m_index.Lookup(dwResID, rpi))
		return 0;

	if (dwStartPosition >= (DWORD)rpi.y)
		return 0;

	DWORD dwRead = min((DWORD)rpi.y - dwStartPosition, dwMaxBufSize);
	if (dwRead == 0)
		return 0;

	try
	{
		m_package.Seek(rpi.x + dwStartPosition, CFile::begin);
		dwRead = m_package.Read(pBuffer, dwRead);
	} catch (...)
	{
		dwRead = 0;
	}

	return dwRead;
}

DWORD CResPack::GetResSize(DWORD dwResID) const
{
	POINT rpi = { 0 };
	m_index.Lookup(dwResID, rpi);
	return (DWORD)rpi.y;
}

BOOL CResPack::SaveResToFile(DWORD dwResID, LPCTSTR lpszFileName, BOOL bCreateFile)
{
	if (m_nOpenFlag == PKG_OPEN_NONE || lpszFileName == NULL)
		return 0;

	DWORD dwResSize = GetResSize(dwResID);
	if (dwResSize == 0)
		return FALSE;

	UINT nOpenFlag = CFile::modeWrite | CFile::shareDenyWrite;
	if (bCreateFile)
		nOpenFlag |= CFile::modeCreate;
	CFile file;
	if (!file.Open(lpszFileName, nOpenFlag))
		return FALSE;

	BYTE* pData = new BYTE[BLOCK_SIZE];
	ASSERT(pData != NULL);
	DWORD dwStartPosition = 0;

	while (TRUE)
	{
		DWORD dwRead = Extract(dwResID, pData, BLOCK_SIZE, dwStartPosition);
		if (dwRead > 0)
			file.Write(pData, dwRead);

		if (dwRead < BLOCK_SIZE)
			break;
		
		dwStartPosition += dwRead;
	}

	delete [] pData;
	file.Close();	
	return TRUE;
}

DWORD CResPack::GetResCount() const
{
	return m_index.GetCount();
}

BOOL CResPack::LoadIndexTable(DWORD* pdwIdxStartPos)
{
	// load the index table

	if (pdwIdxStartPos != NULL)
		*pdwIdxStartPos = 0;

	m_index.RemoveAll();

	DWORD dwIndexStartPos = 0;
	DWORD dwIndexCount = 0;

	try
	{
		m_package.SeekToBegin();
		if (m_package.Read(&dwIndexStartPos, sizeof(DWORD)) != sizeof(DWORD)
			|| m_package.Read(&dwIndexCount, sizeof(DWORD)) != sizeof(DWORD))
			return FALSE;
	} catch (...)
	{
		return FALSE;
	}

	if (dwIndexStartPos < sizeof(DWORD) * 2
		|| dwIndexStartPos >= m_package.GetLength()
		|| dwIndexStartPos + sizeof(DWORD) * 3 * dwIndexCount > m_package.GetLength())
		return FALSE;

	try
	{
		m_package.Seek((LONG)dwIndexStartPos, CFile::begin);
	} catch (...)
	{
		return FALSE;
	}
		
	DWORD arr[3]; // contains res-ID, start-pos, and res-size
	BOOL bOK = TRUE;
	for (DWORD i = 0; i < dwIndexCount; i++)
	{
		try
		{
			if (m_package.Read(arr, sizeof(DWORD) * 3) < sizeof(DWORD) * 3)
			{
				bOK = FALSE;
				break;
			}
		} catch (...)
		{
			bOK = FALSE;
			break;
		}

		POINT rpi;
		rpi.x = (long)arr[1];
		rpi.y = (long)arr[2];

		if (!VerifyIndex(dwIndexStartPos, arr[0], rpi))
		{
			bOK = FALSE;
			break;
		}

		m_index.SetAt(arr[0], rpi);
	}

	if (!bOK)
	{
		dwIndexStartPos = 0;
		m_index.RemoveAll();
	}

	if (pdwIdxStartPos != NULL)
		*pdwIdxStartPos = dwIndexStartPos;

	return bOK;
}

BOOL CResPack::RemoveAll()
{
	if (m_nOpenFlag != PKG_OPEN_WRITE)
		return FALSE;

	m_index.RemoveAll();
	m_package.SetLength(sizeof(DWORD));
	return TRUE;
}

BOOL CResPack::Remove(DWORD dwResID)
{
	if (m_nOpenFlag != PKG_OPEN_WRITE)
		return FALSE;
	
	POINT rpi = { 0 };
	if (!m_index.Lookup(dwResID, rpi))
		return FALSE;

	if (m_package.GetLength() < (DWORD)rpi.y + sizeof(DWORD))
		return FALSE;

	// now move all conents after that resource to left by rpi.y bytes
	// 1024 bytes a time
	BYTE aSegment[BLOCK_SIZE];
	long lCopyPos = rpi.x + rpi.y;
	long lWritePos = rpi.x;
	DWORD dwRead = 0;

	try
	{
		do
		{
			m_package.Seek(lCopyPos, CFile::begin);
			dwRead = m_package.Read(aSegment, BLOCK_SIZE);
			m_package.Seek(lWritePos, CFile::begin);
			m_package.Write(aSegment, dwRead);
			lCopyPos += (long)dwRead;
			lWritePos += (long)dwRead;
		} while(dwRead == BLOCK_SIZE);		
	} catch (...)
	{
	}

	// remove the entry from index table also
	m_index.RemoveKey(dwResID);

	// set the new file size
	m_package.SetLength(m_package.GetLength() - (DWORD)rpi.y);

	// update resource start positions in the index table
	const DWORD START = rpi.x;
	const long COUNT = rpi.y;

	DWORD dwID = 0;
	POSITION pos = m_index.GetStartPosition();
	while (pos != NULL)
	{
		m_index.GetNextAssoc(pos, dwID, rpi);
		if ((DWORD)rpi.x >= START)
		{
			rpi.x -= COUNT;
			m_index.SetAt(dwID, rpi);
		}		
	}
	return TRUE;
}

CString CResPack::GetPackagePathName() const
{
	return m_package.GetFilePath();
}

POSITION CResPack::GetFirstResPosition() const
{
	return m_index.GetStartPosition();
}

void CResPack::GetNextRes(POSITION &nextPosition, DWORD &dwResID, DWORD &dwResSize) const
{
	POINT rpi = { 0 };
	m_index.GetNextAssoc(nextPosition, dwResID, rpi);
	dwResSize = (DWORD)rpi.y;
}

BOOL CResPack::VerifyPackage(LPCTSTR lpszFileName)
{
	CResPack rp;
	BOOL bOK = rp.OpenPackage(lpszFileName, FALSE);
	rp.ClosePackage();
	return bOK;	
}

BOOL CResPack::VerifyIndex(DWORD dwIndexStartPos, DWORD dwResID, POINT rpi) const
{
	POINT temp = { 0 };
	return (DWORD)rpi.x >= sizeof(DWORD) * 2
		&& (DWORD)rpi.x + (DWORD)rpi.y <= dwIndexStartPos
		&& !m_index.Lookup(dwResID, temp);
}

int CResPack::GetOpenStatus() const
{
	return m_nOpenFlag;
}

BOOL CResPack::ModifyRes(DWORD dwOldID, DWORD dwNewID)
{
	if (m_nOpenFlag != PKG_OPEN_WRITE)
		return FALSE;

	POINT rpi;
	if (m_index.Lookup(dwNewID, rpi) || !m_index.Lookup(dwOldID, rpi))
		return FALSE;

	m_index.RemoveKey(dwOldID);
	m_index.SetAt(dwNewID, rpi);
	return TRUE;
}

BOOL CResPack::ModifyRes(DWORD dwResID, const BYTE *lpNewData, DWORD dwNewDataSize)
{
	if (m_nOpenFlag != PKG_OPEN_WRITE || lpNewData == NULL || dwNewDataSize == 0)
		return FALSE;

	POINT rpi = { 0 };
	if (!m_index.Lookup(dwResID, rpi))
		return FALSE;

	if (dwNewDataSize == (DWORD)rpi.y)
	{
		// old and new res have same size
		try
		{
			m_package.Seek(rpi.x, CFile::begin);
		}
		catch (...)
		{
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		// old and new res have different size
		return Remove(dwResID) && Add(dwResID, lpNewData, dwNewDataSize);
	}	
}

BOOL CResPack::ModifyRes(DWORD dwResID, LPCTSTR lpszNewSrcFile)
{
	if (m_nOpenFlag != PKG_OPEN_WRITE || lpszNewSrcFile == NULL)
		return FALSE;

	POINT rpi = { 0 };
	if (!m_index.Lookup(dwResID, rpi))
		return FALSE; // ID not exist

	CFile file;
	if (!file.Open(lpszNewSrcFile, CFile::modeRead))
		return FALSE;

	if (file.GetLength() == 0)
	{
		file.Close();
		return FALSE;
	}

	if (file.GetLength() == (DWORD)rpi.y)
	{
		// old and new res have same size
		try
		{
			m_package.Seek(rpi.x, CFile::begin);
		}
		catch (...)
		{
			file.Close();
			return FALSE;
		}

		BYTE* pData = new BYTE[BLOCK_SIZE];
		ASSERT(pData != NULL);

		while (TRUE)
		{
			DWORD dwRead = file.Read(pData, BLOCK_SIZE);
			if (dwRead > 0)
				m_package.Write(pData, dwRead);
		
			if (dwRead < BLOCK_SIZE)
				break;		
		}

		delete [] pData;

		file.Close();
		return TRUE;
	}
	else
	{
		// old and new res have different size
		file.Close();
		return Remove(dwResID) && Add(dwResID, lpszNewSrcFile);
	}	
}
