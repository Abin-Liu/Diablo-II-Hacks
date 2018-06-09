/////////////////////////////////////////////////////////////////////////////
// StringParse.h
//
// An utility class for parsing a delimited string.
//
// Written by Abin (abinn32@yahoo.com)
/////////////////////////////////////////////////////////////////////////////

#ifndef __STRINGPARSE_H__
#define __STRINGPARSE_H__

#ifndef __AFXWIN_H__ // If non-MFC ...
#include <windows.h>
typedef LPCVOID POSITION; // The generic iterator
#endif // __AFXWIN_H__

/////////////////////////////////////////////////////////////////////////////
// CStringParse Class Definition
/////////////////////////////////////////////////////////////////////////////
class CStringParse
{
public:
	
	/////////////////////////////////////////////////////////////////////////
	// Constructors & Destructor
	/////////////////////////////////////////////////////////////////////////
	CStringParse();
	CStringParse(LPCTSTR lpszString, LPCTSTR lpszDelimitor);
	CStringParse(const CStringParse& rhs);
	virtual ~CStringParse();

	/////////////////////////////////////////////////////////////////////////
	// Operations
	/////////////////////////////////////////////////////////////////////////
	void SetStringDelimitor(LPCTSTR lpszString, LPCTSTR lpszDelimitor);
	POSITION GetStartPosition() const;
	void GetNextString(POSITION& rPosition, LPTSTR lpszBuffer, DWORD dwMaxChars) const;

protected:
	void FreeData(); // Free string memory
	LPCTSTR AdvancePosition(LPCTSTR lpszCur) const; // Advance a string position
	BOOL IsValidPosition(POSITION position) const; // Verify a position

	// Member Data
	LPTSTR m_lpszString; // The raw string
	LPTSTR m_lpszDelimitor; // The delimitor
};

#endif // __STRINGPARSE_H__