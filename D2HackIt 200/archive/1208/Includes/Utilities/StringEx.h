//////////////////////////////////////////////////////////////////////
// StringEx.h
//
// String manipulation without MFC or std::string. Unicode compliant.
//
// Functions/features that MFC CString is lack of:
//
// 1, Trim -          Trims the string from both sides.
// 2, Occurence -     Calculates occurence of a (set of) specified character(s)
//                    in the string.
// 3, FindToken &     
//    FindNextToken - Extract all tokens from the string using given delimitors.
// 4, FindNoneOf -    Finds first character that is not included in a given string.
// 5, ToBinary -      Formats an integer into its binary representation.
// 6, Partially copies/appends from another string.
//                              
// Abin (abinn32@yahoo.com)
//
// Histyory
//
// 2004-12-01 - Initial release.
//////////////////////////////////////////////////////////////////////

#ifndef __STRINGEX_H__E698CF45_6886_4720_A935_5D77EF94F68F__
#define __STRINGEX_H__E698CF45_6886_4720_A935_5D77EF94F68F__

#include <windows.h>

//////////////////////////////////////////////////////////////////
// CStringEx Class Definition
//////////////////////////////////////////////////////////////////
class CStringEx  
{
public:		

	//////////////////////////////////////////////////////////////
	// Constructors/Destructor
	//////////////////////////////////////////////////////////////
	CStringEx();
	CStringEx(LPCTSTR lpszSrc, int nLength = -1);
	CStringEx(TCHAR chSrc, int nCount = 1);
	CStringEx(const CStringEx& rSrc, int nLength = -1);
	virtual ~CStringEx();
	
	//////////////////////////////////////////////////////////////
	// The String as an Array 
	//////////////////////////////////////////////////////////////
	int GetLength() const;
	BOOL IsEmpty() const;
	void Empty();
	TCHAR GetAt(int nIndex) const;
	TCHAR operator[](int nIndex) const;
	void SetAt(int nIndex, TCHAR ch);
	operator LPCTSTR() const;

	//////////////////////////////////////////////////////////////
	// Assignment/Concatenation  
	//////////////////////////////////////////////////////////////
	const CStringEx& operator=(LPCTSTR rhs);
	const CStringEx& operator=(TCHAR rhs);
	const CStringEx& operator=(const CStringEx& rhs);
	const CStringEx& operator+=(LPCTSTR rhs);
	const CStringEx& operator+=(TCHAR rhs);
	const CStringEx& operator+=(const CStringEx& rhs);	
	void Copy(TCHAR ch, int nCount = 1);
	void Copy(LPCTSTR lpsz, int nLength = -1);
	void Copy(const CStringEx& rSrc, int nLength = -1);
	void Append(TCHAR ch, int nCount = 1);
	void Append(LPCTSTR lpsz, int nLength = -1);
	void Append(const CStringEx& rSrc, int nLength = -1);
	
	//////////////////////////////////////////////////////////////
	// Comparison 
	//////////////////////////////////////////////////////////////	
	int Compare(LPCTSTR lpsz) const;
	int CompareNoCase(LPCTSTR lpsz) const;
	int Collate(LPCTSTR lpsz) const;
	int CollateNoCase(LPCTSTR lpsz) const;

	//////////////////////////////////////////////////////////////
	// Extraction
	//////////////////////////////////////////////////////////////
	CStringEx Mid(int nFirst, int nCount = -1) const;
	CStringEx Left(int nCount) const;
	CStringEx Right(int nCount) const;

	//////////////////////////////////////////////////////////////
	// Other Conversions
	//////////////////////////////////////////////////////////////
	void MakeUpper();
	void MakeLower();
	void MakeReverse();
	int Replace(TCHAR chOld, TCHAR chNew);
	int Replace(LPCTSTR lpszOld, LPCTSTR lpszNew);
	int Remove(TCHAR ch);
	int Insert(int nIndex, TCHAR ch);
	int Insert(int nIndex, LPCTSTR lpsz);
	int Delete(int nIndex, int nCount = 1);
	void DeleteInclusive(LPCTSTR lpszCharSet);
	void DeleteExclusive(LPCTSTR lpszCharSet);
	void Format(LPCTSTR lpszFormat, ...);
	void FormatV(LPCTSTR lpszFormat, va_list argList);
	void ToBinary(DWORD dwValue, int nMinWidth = 0);
	void Trim();
	void Trim(TCHAR chTarget);
	void Trim(LPCTSTR lpszTargets);
	void TrimLeft();
	void TrimLeft(TCHAR chTarget);
	void TrimLeft(LPCTSTR lpszTargets);
	void TrimRight();
	void TrimRight(TCHAR chTarget);
	void TrimRight(LPCTSTR lpszTargets);	

	//////////////////////////////////////////////////////////////
	// Searching
	//////////////////////////////////////////////////////////////
	int Find(TCHAR ch, int nStart = 0) const;
	int Find(LPCTSTR lpsz, int nStart = 0) const;
	int ReverseFind(TCHAR ch, int nStartBefore = -1) const;
	int ReverseFind(LPCTSTR lpsz, int nStartBefore = -1) const;
	int FindOneOf(LPCTSTR lpszCharSet) const;
	int FindNoneOf(LPCTSTR lpszCharSet) const;
	BOOL FindToken(LPCTSTR lpszDelimitors, BOOL bWholeMatch = FALSE);
	BOOL FindNextToken(CStringEx& rStr);
	int Occurence(TCHAR ch) const;
	int Occurence(LPCTSTR lpsz) const;

	//////////////////////////////////////////////////////////////
	// Other Attributes
	//////////////////////////////////////////////////////////////
	static BOOL IsAlpha(TCHAR ch);
	BOOL IsAlpha() const;
	static BOOL IsDigit(TCHAR ch);
	BOOL IsDigit() const;
	static BOOL IsBlank(TCHAR ch);
	BOOL IsBlank() const;
	static BOOL IsHex(TCHAR ch);
	BOOL IsHex() const;
	BOOL IsNumeric(BOOL bInteger) const;
	
	//////////////////////////////////////////////////////////////
	// Buffer Access
	//////////////////////////////////////////////////////////////
	LPTSTR GetBuffer(int nMinBufLength = -1);
	LPTSTR GetBufferSetLength(int nNewLength);
	void ReleaseBuffer(int nNewLength = -1);	
	void FreeExtra();

private:		
	
	//////////////////////////////////////////////////////////////
	// Internal Methods
	//////////////////////////////////////////////////////////////
	void Initialize();
	int ReallocBuffer(int nLen, BOOL bKeepOriginalData);
	void FreeBuffer();
	void LeftShift(int nIndex, int nDistance);
	void RightShift(int nIndex, int nDistance);
	BOOL AdvanceToken();
	BOOL IsValidPtr(LPCTSTR ptr) const;
	BOOL MatchDelimitor(LPCTSTR lpsz) const;
	int Ptr2Idx(LPCTSTR lpsz) const;

	//////////////////////////////////////////////////////////////
	// Member Data
	//////////////////////////////////////////////////////////////
	LPTSTR m_pszData; // The string buffer
	int m_nBufferLen; // String buffer allocated length
	int m_nStrLen; // String length
	LPTSTR m_pszTemp; // Temp buffer for "GetBuffer"
	int m_nTempLen; // Temp buffer allocated length
	LPTSTR m_pszDelimitors; // For token functions
	LPCTSTR m_lpszToken; // For token functions
	BOOL m_bWholeMatch; // For token functions
	int m_nDelimitorLen; // Delimitor length
};
//////////////////////////////////////////////////////////////////
// End of CStringEx Class Definition
//////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
// Global Operators Definition
//////////////////////////////////////////////////////////////////
CStringEx operator+(const CStringEx& lhs, TCHAR rhs);
CStringEx operator+(TCHAR lhs, const CStringEx& rhs);
CStringEx operator+(const CStringEx& lhs, LPCTSTR rhs);
CStringEx operator+(LPCTSTR lhs, const CStringEx& rhs);
CStringEx operator+(const CStringEx& lhs, const CStringEx& rhs);
BOOL operator==(const CStringEx& lhs, LPCTSTR rhs);
BOOL operator!=(const CStringEx& lhs, LPCTSTR rhs);
BOOL operator<(const CStringEx& lhs, LPCTSTR rhs);
BOOL operator<=(const CStringEx& lhs, LPCTSTR rhs);
BOOL operator>(const CStringEx& lhs, LPCTSTR rhs);
BOOL operator>=(const CStringEx& lhs, LPCTSTR rhs);
BOOL operator==(LPCTSTR lhs, const CStringEx& rhs);
BOOL operator!=(LPCTSTR lhs, const CStringEx& rhs);
BOOL operator<(LPCTSTR lhs, const CStringEx& rhs);
BOOL operator<=(LPCTSTR lhs, const CStringEx& rhs);
BOOL operator>(LPCTSTR lhs, const CStringEx& rhs);
BOOL operator>=(LPCTSTR lhs, const CStringEx& rhs);
BOOL operator==(const CStringEx& lhs, const CStringEx& rhs);
BOOL operator!=(const CStringEx& lhs, const CStringEx& rhs);
BOOL operator<(const CStringEx& lhs, const CStringEx& rhs);
BOOL operator<=(const CStringEx& lhs, const CStringEx& rhs);
BOOL operator>(const CStringEx& lhs, const CStringEx& rhs);
BOOL operator>=(const CStringEx& lhs, const CStringEx& rhs);
//////////////////////////////////////////////////////////////////
// End of Global Operators Definition
//////////////////////////////////////////////////////////////////

#endif // __STRINGEX_H__E698CF45_6886_4720_A935_5D77EF94F68F__