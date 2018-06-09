// D2PacketDoc.cpp : implementation of the CD2PacketDoc class
//

#include "stdafx.h"
#include "D2Packet.h"

#include "D2PacketDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CD2PacketDoc

IMPLEMENT_DYNCREATE(CD2PacketDoc, CDocument)

BEGIN_MESSAGE_MAP(CD2PacketDoc, CDocument)
	//{{AFX_MSG_MAP(CD2PacketDoc)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CD2PacketDoc construction/destruction

CD2PacketDoc::CD2PacketDoc()
{
	// TODO: add one-time construction code here

}

CD2PacketDoc::~CD2PacketDoc()
{
}

BOOL CD2PacketDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CD2PacketDoc serialization

void CD2PacketDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		ar.Write(m_filter.GetFilter(), 256);		
	}
	else
	{
		// TODO: add loading code here
		BYTE aFilter[256];
		if (ar.Read(aFilter, 256) == 256)
			m_filter.SetFilter(aFilter, 256);		
	}
}

/////////////////////////////////////////////////////////////////////////////
// CD2PacketDoc diagnostics

#ifdef _DEBUG
void CD2PacketDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CD2PacketDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CD2PacketDoc commands

CD2PacketFilter* CD2PacketDoc::GetFilter()
{
	return &m_filter;
}

const CD2PacketFilter* CD2PacketDoc::GetFilter() const
{
	return &m_filter;
}

BOOL CD2PacketDoc::IsPacketOK(const BYTE *aPacket, DWORD aLen, BOOL bReceived) const
{
	return m_filter.IsAllowed(aPacket[0], bReceived);
}

void CD2PacketDoc::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(this->IsModified());
}
