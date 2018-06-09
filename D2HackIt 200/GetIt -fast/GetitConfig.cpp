//////////////////////////////////////////////////////////////////////
// GetitConfig.cpp
//
// Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////////////

#include "GetitConfig.h"
#include <string.h>
#include <time.h>

typedef CArrayEx<CStringEx, const CStringEx&> CStringArrayEx;
const static LPCSTR COMMENTS[] = { "//", ";", "'", NULL }; // comments(c/ini/vb styles)

CGetitConfig::CGetitConfig()
{
	m_bTeleport = FALSE;
	m_bValid = FALSE;
}

CGetitConfig::~CGetitConfig()
{

}

BOOL CGetitConfig::Load(const CIni *pIni, LPCSTR lpszSection)
{
	m_bValid = FALSE;
	if (pIni == NULL || lpszSection == NULL || lpszSection[0] == 0)
		return FALSE;

	m_sSection = lpszSection;

	char szTemp[1024];
	pIni->GetString(lpszSection, "Code", szTemp, 1023);
	m_sCodes = szTemp;
	if (m_sCodes.IsEmpty())
		return FALSE; // codes cannot be empty in ini

	if (m_sCodes == "*")
		m_sCodes.Empty();
	
	m_bTeleport = pIni->GetBool(lpszSection, "teleport", FALSE);

	// common
	pIni->GetString(lpszSection, "Common", szTemp, 1023);
	RemoveComments(szTemp);

	m_sPick.Empty();
	m_sKeep.Empty();

	if (szTemp[0])
	{
		m_sPick.Format("(%s)", szTemp);
		m_sKeep = m_sPick;
	}

	CStringArrayEx aExps;
	pIni->EnumKeys(lpszSection, EnumPickProc, (LPVOID)&aExps);
	if (!aExps.IsEmpty())
	{
		if (!m_sPick.IsEmpty())
			m_sPick += "&&";
		m_sPick += '(';
		for (int i = 0; i < aExps.GetSize(); i++)
		{
			if (i)
				m_sPick += "||";
			m_sPick += '(';
			m_sPick += aExps[i];
			m_sPick += ')';
		}
		m_sPick += ")";
	}

	aExps.RemoveAll();
	pIni->EnumKeys(lpszSection, EnumKeepProc, (LPVOID)&aExps);
	if (!aExps.IsEmpty())
	{
		if (!m_sKeep.IsEmpty())
			m_sKeep += "&&";
		m_sKeep += '(';
		for (int i = 0; i < aExps.GetSize(); i++)
		{
			if (i)
				m_sKeep += "||";
			m_sKeep += '(';
			m_sKeep += aExps[i];
			m_sKeep += ')';
		}
		m_sKeep += ")";
	}

	m_bValid = TRUE;
	return TRUE;
}

BOOL CGetitConfig::IsValid() const
{
	return m_bValid;
}

void CGetitConfig::RemoveComments(LPSTR lpsz)
{
	if (lpsz == NULL || lpsz[0] == 0)
		return;

	int i = 0;
	for (i = 0; COMMENTS[i]; i++)
	{
		LPSTR p = ::strstr(lpsz, COMMENTS[i]);
		if (p)
			*p = 0;
	}

	const int LEN = ::strlen(lpsz);
	for (i = LEN - 1; i >= 0; i--)
	{
		if (lpsz[i] == ' ' || lpsz[i] == '\t' || lpsz[i] == '\r' || lpsz[i] == '\n')
			lpsz[i] = 0;
		else
			break;
	}
}

BOOL CGetitConfig::EnumPickProc(LPCTSTR lpszKey, LPCTSTR lpszValue, LPVOID lpParam)
{
	CStringArrayEx* pArray = (CStringArrayEx*)lpParam;
	if (pArray == NULL || lpszKey == NULL || lpszValue == NULL || ::stricmp(lpszKey, "pick"))
		return TRUE;

	LPSTR p = ::strdup(lpszValue);
	RemoveComments(p);
	if (p && p[0])
		pArray->Add(p);	
	::free(p);
	return TRUE;
}

BOOL CGetitConfig::EnumKeepProc(LPCTSTR lpszKey, LPCTSTR lpszValue, LPVOID lpParam)
{
	CStringArrayEx* pArray = (CStringArrayEx*)lpParam;
	if (pArray == NULL || lpszKey == NULL || lpszValue == NULL || ::stricmp(lpszKey, "keep"))
		return TRUE;

	LPSTR p = ::strdup(lpszValue);
	RemoveComments(p);
	if (p && p[0])
		pArray->Add(p);	
	::free(p);

	return TRUE;
}

LPCSTR CGetitConfig::GetSectionName() const
{
	return m_sSection;
}

BOOL CGetitConfig::Exam(DWORD dwItemID, BOOL bPickorKeep, BOOL* pTeleport)
{
	if (!m_bValid)
		return FALSE;

	const CStringEx* pStr = bPickorKeep ? &m_sPick : &m_sKeep;
	if (pStr->IsEmpty())
		return FALSE;

	BOOL bSyntaxError = FALSE;
	BOOL bOK = server->ExamItemProperties(dwItemID, (LPCSTR)(*pStr), m_sCodes, &bSyntaxError);
	if (bSyntaxError)
	{
		bOK = FALSE;
		m_bValid = FALSE;
		server->GameErrorf("ÿc8[GetIt]: Syntax error in section \"ÿc9%sÿc9\", section ignored!", (LPCSTR)m_sSection);
	}

	if (bOK && pTeleport)
		*pTeleport = m_bTeleport;

	return bOK;
}

CGetItConfigLib::CGetItConfigLib()
{
	m_bTownPick = TRUE;
	m_dwRadius = 15;
	m_bClientMove = FALSE;
	m_dwDelay = 300;
	m_nLeftCol = 0;
	m_nRightCol = 5;
	m_bUseCube = TRUE;
}

CGetItConfigLib::~CGetItConfigLib()
{
}

void CGetItConfigLib::FreeCfgItem(CGetitConfig*& pData)
{
	if (pData)
	{
		delete pData;
		pData = NULL;
	}
}

BOOL CGetItConfigLib::Load(LPCSTR lpszIniFile)
{
	if (lpszIniFile == NULL || lpszIniFile[0] == 0)
	{
		char szPath[MAX_PATH + 1] = "";
		::sprintf(szPath, "%s\\GetIt.ini", server->GetModuleDirectory());
		m_ini.SetPathName(szPath);
	}
	else
	{
		m_ini.SetPathName(lpszIniFile);
	}
	
	m_aConfigs.RemoveAll();	

	m_bTownPick = m_ini.GetBool("Global", "Town Pick", TRUE);
	m_dwRadius = m_ini.GetUInt("Global", "Radius", 15);
	m_bClientMove = m_ini.GetBool("Global", "Client Move", FALSE);
	m_dwDelay = m_ini.GetUInt("Global", "Delay", 300);
	m_nLeftCol = m_ini.GetInt("Global", "Left Col", 0);
	m_nRightCol = m_ini.GetInt("Global", "Right Col", 5);
	m_bUseCube = m_ini.GetBool("Global", "Use cube", TRUE);

	char szTemp[1024];
	m_ini.GetString("Global", "Log", szTemp, 1023);
	m_sLogFile = szTemp;

	m_ini.GetString("Global", "Full cmds", szTemp, 1023);
	m_sFullCallback = szTemp;
	
	m_ini.GetString("Global", "Stash Cmds", szTemp, 1023);
	m_sIDCallback = szTemp;

	m_ini.EnumSections(EnumSectionProc, (LPVOID)this);
	return TRUE;
}

BOOL CGetItConfigLib::EnumSectionProc(LPCSTR lpszSection, LPVOID lpParam)
{	
	CGetItConfigLib* pLib = (CGetItConfigLib*)lpParam;
	if (pLib == NULL)
		return FALSE;

	if (lpszSection == NULL || lpszSection[0] == 0 || ::stricmp(lpszSection, "Global") == 0)
		return TRUE;

	CGetitConfig* p = new CGetitConfig;
	if (p->Load(&pLib->m_ini, lpszSection))
		pLib->m_aConfigs.InsertRear(p);
	else
		delete p;

	return TRUE;
}

BOOL CGetItConfigLib::Lookup(DWORD dwItemID, BOOL bPickOrKeep, BOOL* pTeleport)
{
	POSITION pos = m_aConfigs.GetHeadPosition();
	while (pos)
	{
		CGetitConfig* p = m_aConfigs.GetNext(pos);
		if (p == NULL || !p->IsValid())
			continue;

		if (p->Exam(dwItemID, bPickOrKeep, pTeleport))
			return TRUE;
	}

	return FALSE;
}

int CGetItConfigLib::InvokeFullCallback() const
{
	return InvokeCallback(m_sFullCallback);
}

int CGetItConfigLib::InvokeStashCallback() const
{
	return InvokeCallback(m_sIDCallback);
}

BOOL CGetItConfigLib::WriteLog(LPCSTR lpszString) const
{
	if (m_sLogFile.IsEmpty())
		return FALSE;
	
	FILE *stream = _tfopen(m_sLogFile, lpszString ? "a+" : "w");
	if (stream == NULL)
		return FALSE;

	if (lpszString == NULL)
	{
		::fclose(stream);
		return TRUE;
	}

	// get time stamp
	char szTimeString[33] = "";
	struct tm *when;
    time_t now;
    ::time(&now);
    when = ::localtime(&now);
	::strftime(szTimeString, 32, "%m/%d %H:%M:%S", when);
	if (szTimeString[0] == '0')
		szTimeString[0] = ' ';

	const int LEN = ::strlen(szTimeString) + ::strlen(lpszString) + 10;
	char* pszString = new char[LEN];
	::sprintf(pszString, "%s | %s\n", szTimeString, lpszString);

	// replace the special color char to '%' so the log file can be read by
	// MFC CStdioFile
	for (int i = 0; pszString[i] != '\0'; i++)
	{
		if (pszString[i] == 'ÿ')
			pszString[i] = '%';
	}

	// write to file
	::fprintf(stream, "%s", pszString);

	delete [] pszString;
	::fclose(stream);
	return TRUE;
}

int CGetItConfigLib::InvokeCallback(CStringEx str)
{
	BOOL bOK = str.FindToken(";");
	int nCount = 0;
	while (bOK)
	{
		CStringEx s;
		bOK = str.FindNextToken(s);
		server->GameCommandLine((LPSTR)(LPCSTR)s);
		nCount++;
	}

	return nCount;
}

BOOL CGetItConfigLib::GetClientMove() const
{
	return !!m_bClientMove;
}

void CGetItConfigLib::SetClientMove(BOOL bSet)
{
	m_bClientMove = !!bSet;
	m_ini.WriteBool("Global", "Client Move", m_bClientMove);
	DisplayClientMove();
}

DWORD CGetItConfigLib::GetRadius() const
{
	return m_dwRadius;	
}

void CGetItConfigLib::SetRadius(DWORD dwRadius)
{
	m_dwRadius = dwRadius;	
	m_ini.WriteUInt("Global", "Radius", m_dwRadius);
	DisplayRadius();
}

DWORD CGetItConfigLib::GetDelay() const
{
	return m_dwDelay;
}

void CGetItConfigLib::SetDelay(DWORD dwDelay)
{
	m_dwDelay = max(100, dwDelay);	
	m_ini.WriteUInt("Global", "Delay", m_dwDelay);
	DisplayDelay();
}

int CGetItConfigLib::GetLeftCol() const
{
	return m_nLeftCol;
}

int CGetItConfigLib::GetRightCol() const
{
	return m_nRightCol;
}

void CGetItConfigLib::SetMoveColumns(int nLeftCol, int nRightCol)
{
	m_nLeftCol = min(9, nLeftCol);
	m_nRightCol = min(9, nRightCol);
	m_ini.WriteInt("Global", "Left Col", m_nLeftCol);
	m_ini.WriteInt("Global", "Right Col", m_nRightCol);
	DisplayColumnsPick();
}

BOOL CGetItConfigLib::GetUseCube() const
{
	return !!m_bUseCube;
}

void CGetItConfigLib::SetUseCube(BOOL bSet)
{
	m_bUseCube = !!bSet;	
	m_ini.WriteBool("Global", "Use cube", m_bUseCube);
	DisplayUseCube();
}

LPCSTR CGetItConfigLib::GetFullCallback() const
{
	return m_sFullCallback;
}

void CGetItConfigLib::SetFullCallback(LPCSTR lpsz)
{
	m_sFullCallback = lpsz;	
	m_ini.WriteString("Global", "Full cmds", m_sFullCallback);
	DisplayFullCallback();
}

LPCSTR CGetItConfigLib::GetStashallback() const
{
	return m_sIDCallback;
}

void CGetItConfigLib::SetStashCallback(LPCSTR lpsz)
{
	m_sIDCallback = lpsz;	
	m_ini.WriteString("Global", "Stash Cmds", m_sIDCallback);
	DisplayStashCallback();
}

LPCSTR CGetItConfigLib::GetLogFile() const
{
	return m_sLogFile;
}

void CGetItConfigLib::SetLogFile(LPCSTR lpszPath)
{
	m_sLogFile = lpszPath;	
	m_ini.WriteString("Global", "Log", m_sLogFile);
	DisplayLog();
}

BOOL CGetItConfigLib::GetTownPick() const
{
	return !!m_bTownPick;
}

void CGetItConfigLib::SetTownPick(BOOL bSet)
{
	m_bTownPick = !!bSet;	
	m_ini.WriteBool("Global", "Town Pick", m_bTownPick);
	DisplayTownPick();
}

void CGetItConfigLib::DisplayTownPick() const
{
	server->GameInfof("ÿc4Town Pick:ÿc0    %d", m_bTownPick);
}

void CGetItConfigLib::DisplayRadius() const
{
	server->GameInfof("ÿc4Radius:ÿc0       %d", m_dwRadius);
}

void CGetItConfigLib::DisplayClientMove() const
{
	server->GameInfof("ÿc4Client Move:ÿc0  %d", m_bClientMove);
}

void CGetItConfigLib::DisplayDelay() const
{
	server->GameInfof("ÿc4Delay:ÿc0        %d", m_dwDelay);
}

void CGetItConfigLib::DisplayColumnsPick() const
{
	server->GameInfof("ÿc4Move Columns:ÿc0 %d - %d", m_nLeftCol, m_nRightCol);
}

void CGetItConfigLib::DisplayUseCube() const
{
	server->GameInfof("ÿc4Use Cube:ÿc0     %d", m_bUseCube);
}

void CGetItConfigLib::DisplayFullCallback() const
{
	server->GameInfof("ÿc4Full Callback:ÿc0 %s", m_sFullCallback.IsEmpty() ? "<None>" : m_sFullCallback);
}

void CGetItConfigLib::DisplayStashCallback() const
{
	server->GameInfof("ÿc4Stash Callback:ÿc0 %s", m_sIDCallback.IsEmpty() ? "<None>" : m_sIDCallback);
}

void CGetItConfigLib::DisplayLog() const
{
	server->GameInfof("ÿc4Log File:ÿc0      %s", m_sLogFile.IsEmpty() ? "<None>" : m_sLogFile);
}


void CGetItConfigLib::DisplaySummary() const
{
	server->GamePrintInfo("ÿc4-------------------------------------------------");
	server->GamePrintInfo("ÿc2GetIt Settings");
	server->GamePrintInfo(" ");
	DisplayRadius();
	DisplayDelay();
	DisplayTownPick();
	DisplayClientMove();
	DisplayColumnsPick();
	DisplayUseCube();
	DisplayFullCallback();
	DisplayStashCallback();
	DisplayLog();
	server->GamePrintInfo("ÿc4-------------------------------------------------");
}
