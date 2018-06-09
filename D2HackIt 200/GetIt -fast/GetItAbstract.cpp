// GetItAbstract.cpp: implementation of the CGetItAbstract class.
//
//////////////////////////////////////////////////////////////////////

#include "GetItAbstract.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGetItAbstract::CGetItAbstract() : m_pLib(NULL), m_dwLastAnnounce(0), m_dwLastAnnounceTime(0)
{

}

CGetItAbstract::CGetItAbstract(CGetItConfigLib* pLib) : m_pLib(pLib), m_dwLastAnnounce(0), m_dwLastAnnounceTime(0)
{
}

CGetItAbstract::~CGetItAbstract()
{

}

void CGetItAbstract::AnnounceItem(DWORD dwItemID)
{
	if (dwItemID && (m_dwLastAnnounce != dwItemID || ::GetTickCount() - m_dwLastAnnounceTime > 3000))
	{
		// avoid multiple announcements
		m_dwLastAnnounce = dwItemID;
		m_dwLastAnnounceTime = ::GetTickCount();

		// announce it
		char szAnnounce[128] = "";
		server->GetItemDescription(dwItemID, szAnnounce, 127, TRUE);
		server->GamePrintInfo(szAnnounce);

		// log it?
		if (m_pLib)
			m_pLib->WriteLog(szAnnounce + 3);
	}
}

void CGetItAbstract::SetConfigLib(CGetItConfigLib *pLib)
{
	m_pLib = pLib;
}

const CGetItConfigLib* CGetItAbstract::GetConfigLib() const
{
	return m_pLib;
}

CGetItConfigLib* CGetItAbstract::GetConfigLib()
{
	return m_pLib;
}

BOOL CGetItAbstract::ExamItem(DWORD dwItemID, BOOL bPickOrMove)
{
	return m_pLib ? m_pLib->Lookup(dwItemID, bPickOrMove) : FALSE;
}
