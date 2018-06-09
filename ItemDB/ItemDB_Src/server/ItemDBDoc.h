// ItemDBDoc.h : interface of the CItemDBDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMDBDOC_H__11F8D784_0830_4948_91E5_6F0718C190D0__INCLUDED_)
#define AFX_ITEMDBDOC_H__11F8D784_0830_4948_91E5_6F0718C190D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CItemDBDoc : public CDocument
{
protected: // create from serialization only
	CItemDBDoc();
	DECLARE_DYNCREATE(CItemDBDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemDBDoc)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CItemDBDoc();
#ifdef _DEBUG
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CItemDBDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMDBDOC_H__11F8D784_0830_4948_91E5_6F0718C190D0__INCLUDED_)
