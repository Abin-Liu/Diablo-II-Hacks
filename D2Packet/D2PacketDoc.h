// D2PacketDoc.h : interface of the CD2PacketDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_D2PACKETDOC_H__91A4E735_863D_4534_8AAC_181D3A66AA2A__INCLUDED_)
#define AFX_D2PACKETDOC_H__91A4E735_863D_4534_8AAC_181D3A66AA2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "D2PacketFilter.h"

class CD2PacketDoc : public CDocument
{
protected: // create from serialization only
	CD2PacketDoc();
	DECLARE_DYNCREATE(CD2PacketDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CD2PacketDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL IsPacketOK(const BYTE *aPacket, DWORD aLen, BOOL bReceived) const;
	CD2PacketFilter* GetFilter();
	const CD2PacketFilter* GetFilter() const;
	virtual ~CD2PacketDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CD2PacketFilter m_filter;

// Generated message map functions
protected:
	//{{AFX_MSG(CD2PacketDoc)
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D2PACKETDOC_H__91A4E735_863D_4534_8AAC_181D3A66AA2A__INCLUDED_)
