#if !defined(AFX_CHARDLG_H__6250770A_B695_430E_8B6A_2BC5640CCAFC__INCLUDED_)
#define AFX_CHARDLG_H__6250770A_B695_430E_8B6A_2BC5640CCAFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CharDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCharDlg dialog

// Character class ID values
#define CCD_AMAZON				0
#define CCD_ASSASSIN			1
#define CCD_BARBARIAN			2
#define CCD_DRUID				3
#define CCD_NECROMANCER			4
#define CCD_PALADIN				5
#define CCD_SORCERESS			6
#define CCD_UNKNOWN				7

// Character class string representation
const TCHAR* const CHARCLASSNAME[] =
 { _T("Amazon"), _T("Assassin"), _T("Barbarian"), _T("Druid"), _T("Necromancer"), _T("Paladin"), _T("Sorceress") }; 

class CCharDlg : public CDialog
{
// Construction
public:
	BYTE GetCharClass() const;
	CString GetCharName() const;
	CCharDlg(LPCTSTR lpszCharName = NULL, int nCharClass = CCD_SORCERESS, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCharDlg)
	enum { IDD = IDD_NEWCHARDLG };
	CComboBox	m_wndCharClass;
	CEdit	m_wndCharName;
	CString	m_sCharName;
	int		m_nCharClass;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCharDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCharDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARDLG_H__6250770A_B695_430E_8B6A_2BC5640CCAFC__INCLUDED_)
