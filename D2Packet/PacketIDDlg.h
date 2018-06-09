#if !defined(AFX_PACKETIDDLG_H__85E4EFE4_A9B0_4E6B_A95B_ACF78BACCEB7__INCLUDED_)
#define AFX_PACKETIDDLG_H__85E4EFE4_A9B0_4E6B_A95B_ACF78BACCEB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PacketIDDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPacketIDDlg dialog

class CPacketIDDlg : public CDialog
{
// Construction
public:
	BYTE GetPacketID() const;
	CPacketIDDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPacketIDDlg)
	enum { IDD = IDD_PACKETIDDLG };
	CEdit	m_wndEdit;
	CString	m_sPacketID;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPacketIDDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPacketIDDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PACKETIDDLG_H__85E4EFE4_A9B0_4E6B_A95B_ACF78BACCEB7__INCLUDED_)
