///////////////////////////////////////////////////////////////////
//                 IniFile.h (Non-MFC)
//
// "CIniFile" is a simple API wrap class used for ini file access.
// The purpose of this class is to make ini file access more
// convenient than direct API calls.
//
// Also, a few methods are added to allow reading double values from
// ini file, and allow int/double values to be written into ini file,
// those functionalities were not available in direct API calls.
//
// 7/08/2002    Bin Liu
// All rights reserved.
//
// Update history:
//
// 7/08/2002 -- Initial release. (Bin Liu)
// 7/14/2002 -- Added "IncreaseProfileInt" and "AppendProfileString"
// 9/02/2002 -- Added "removeProfileSection" and "RemoveProfileEntry"
//
///////////////////////////////////////////////////////////////////

#ifndef __CINIFILE_H__
#define __CINIFILE_H__

#include <windows.h>

class CIniFile  
{
public:

	//-----------------------------------------------------------
	//    Constructors/destructor
	//-----------------------------------------------------------
	CIniFile(); // Default constructor
	CIniFile(LPCTSTR lpPathName); // Construct with a given file name
	virtual ~CIniFile();

	//-----------------------------------------------------------
	//    Ini file path and name specification
	//-----------------------------------------------------------
	BOOL SetPathName(LPCTSTR lpPathName); // Specify a new file name
	int GetPathName(LPSTR lpBuffer, DWORD nLength) const; // Retrieve current file name

	//------------------------------------------------------------
	//    Retrieve data from ini file
	//------------------------------------------------------------
	DWORD GetProfileString(LPCTSTR lpSection, LPCTSTR lpEntry, LPCTSTR lpDefault, LPSTR lpBuffer, DWORD nLength) const;
	double GetProfileDbl(LPCTSTR lpSection, LPCTSTR lpEntry, double fDefault) const;
	int GetProfileInt(LPCTSTR lpSection, LPCTSTR lpEntry, int nDefault) const;
	BOOL GetProfileBool(LPCTSTR lpSection, LPCTSTR lpEntry, BOOL bDefault) const;

	//------------------------------------------------------------
	//    Write data into ini file
	//------------------------------------------------------------
	BOOL WriteProfileString(LPCTSTR lpSection, LPCTSTR lpEntry, LPCTSTR lpValue) const;
	BOOL WriteProfileInt(LPCTSTR lpSection, LPCTSTR lpEntry, int nValue) const;
	BOOL WriteProfileBool(LPCTSTR lpSection, LPCTSTR lpEntry, BOOL bValue) const;
	BOOL WriteProfileDbl(LPCTSTR lpSection, LPCTSTR lpEntry, double fValue) const;
	BOOL AppendProfileString(LPCTSTR Section, LPCTSTR lpEntry, LPCTSTR lpString) const;
	BOOL IncreaseProfileInt(LPCTSTR lpSection, LPCTSTR lpEntry, int nIncrease = 1) const;

	//------------------------------------------------------------
	//    List all sections/entries of the ini file
	//------------------------------------------------------------
	DWORD GetProfileSectionNames(LPTSTR lpBuffer, DWORD nLength, char cDelimiter = '\0') const;
	DWORD GetProfileSection(LPCTSTR lpSection, LPTSTR lpBuffer, DWORD nLength, char cDelimiter ='\0') const;

	//------------------------------------------------------------
	//    Delete a section/entry from the ini file
	//------------------------------------------------------------
	BOOL RemoveProfileSection(LPCTSTR lpSection) const;
	BOOL RemoveProfileEntry(LPCTSTR lpSection, LPCTSTR lpEntry) const;
	
protected:

	// Helper functions, used for searching a specified section/entry in ini file
	static BOOL FindEntry(LPCTSTR lpEntry, LPCTSTR lpString);
	static BOOL IsEntry(LPCTSTR lpString, LPTSTR lpParsedResult);
	static BOOL FindSection(LPCTSTR lpSection, LPCTSTR lpString);
	static BOOL IsSection(LPCTSTR lpString, LPTSTR lpSection);

	// the only member data of this class
	TCHAR* m_pszPathName; // The ini file path and name
};

#endif