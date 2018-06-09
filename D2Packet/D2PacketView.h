// D2PacketView.h : interface of the CD2PacketView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_D2PACKETVIEW_H__99CCF9C7_4340_4B3A_8111_567DAC1CFF85__INCLUDED_)
#define AFX_D2PACKETVIEW_H__99CCF9C7_4340_4B3A_8111_567DAC1CFF85__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "D2PacketRelayer.h"

class CD2PacketView : public CFormView, public CD2PacketRelayer
{
protected: // create from serialization only
	CD2PacketView();
	DECLARE_DYNCREATE(CD2PacketView)

public:
	//{{AFX_DATA(CD2PacketView)
	enum { IDD = IDD_D2PACKET_FORM };
	CListCtrl	m_wndList;
	//}}AFX_DATA

// Attributes
public:
	CD2PacketDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CD2PacketView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CD2PacketView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	BOOL m_bPaused;
	BOOL m_bInitialized;
	BOOL m_bDebug;
	CString m_sSearch;

// Generated message map functions
protected:
	void UpdateList();
	void PrintPacket(const BYTE *aPacket, DWORD aLen, BOOL bReceived);

	void OnClientStart() {}
	void OnClientStop() {}
	void OnGameJoin(LPCPLAYERINFO lpPlayer);
	void OnGameLeave();
	void OnGamePacketBeforeSent(const BYTE *aPacket, DWORD aLen);
	void OnGamePacketBeforeReceived(const BYTE *aPacket, DWORD aLen);
	//{{AFX_MSG(CD2PacketView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClear();
	afx_msg void OnFilter();
	afx_msg void OnPause();
	afx_msg void OnUpdatePause(CCmdUI* pCmdUI);
	afx_msg void OnPopupBlock();
	afx_msg void OnUpdatePopupBlock(CCmdUI* pCmdUI);
	afx_msg void OnPopupClipboard();
	afx_msg void OnUpdatePopupClipboard(CCmdUI* pCmdUI);
	afx_msg void OnRcvAllowall();
	afx_msg void OnRcvBlockall();
	afx_msg void OnResume();
	afx_msg void OnUpdateResume(CCmdUI* pCmdUI);
	afx_msg void OnSntAllowall();
	afx_msg void OnSntBlockall();
	afx_msg void OnRcvCustom();
	afx_msg void OnSntCustom();
	afx_msg void OnDebug();
	afx_msg void OnUpdateDebug(CCmdUI* pCmdUI);
	afx_msg void OnPlayerinfo();
	afx_msg void OnUpdatePlayerinfo(CCmdUI* pCmdUI);
	afx_msg void OnSearch();
	afx_msg void OnUpdateSearch(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in D2PacketView.cpp
inline CD2PacketDoc* CD2PacketView::GetDocument()
   { return (CD2PacketDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D2PACKETVIEW_H__99CCF9C7_4340_4B3A_8111_567DAC1CFF85__INCLUDED_)
