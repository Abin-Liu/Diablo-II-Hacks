//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ModuleWalker.h"
#include <tlhelp32.h>


static BOOL isNT()
{
	OSVERSIONINFO osversioninfo;
	osversioninfo.dwOSVersionInfoSize = sizeof(osversioninfo);

	// Get the current OS version
	if (!GetVersionEx(&osversioninfo))
		return FALSE;
	return osversioninfo.dwPlatformId == VER_PLATFORM_WIN32_NT;
}
BOOL g_isNTPlatform = isNT();

CModuleWalker::CModuleWalker(void)
{
}

CModuleWalker::~CModuleWalker(void)
{
}


BOOL CModuleWalker::Walk(DWORD pid)
{
	DelDllList delDlls(m_dlls);
	return Walk(pid, delDlls);
}

BOOL CModuleWalker::Walk(DWORD pid, DelDllList& delDlls)
{
	return (g_isNTPlatform && WalkNT(pid, delDlls)) || WalkToolhelp32(pid, delDlls);
}

BOOL CModuleWalker::WalkToolhelp32(DWORD pid, DelDllList& delDlls)
{
	// Typedefs for toolhelp32
	typedef BOOL (WINAPI *fnModule32First)(HANDLE hSnapshot, LPMODULEENTRY32 lpme);
	typedef BOOL (WINAPI *fnModule32Next)(HANDLE hSnapshot, LPMODULEENTRY32 lpme);
	typedef HANDLE (WINAPI *fnCreateToolhelp32Snapshot)(DWORD dwFlags, DWORD th32ProcessID);  

	static fnModule32First Module32First;
	static fnModule32Next Module32Next;
	static fnCreateToolhelp32Snapshot CreateToolhelp32Snapshot;
	if(!Module32First) {
		// Find out if the toolhelp API exists in kernel32
		HMODULE k32=GetModuleHandle(_T("kernel32.dll"));
		if (!k32)
		{
			return FALSE;
		}

#ifndef UNICODE
		Module32First = (fnModule32First)GetProcAddress(k32, "Module32First");
		Module32Next = (fnModule32Next)GetProcAddress(k32, "Module32Next");
#else
		Module32First = (fnModule32First)GetProcAddress(k32, "Module32FirstW");
		Module32Next = (fnModule32Next)GetProcAddress(k32, "Module32NextW");
#endif // UNICODE
		CreateToolhelp32Snapshot=(fnCreateToolhelp32Snapshot)GetProcAddress(k32,"CreateToolhelp32Snapshot");

		// Verify that the ToolHelp32 API is available
		if (!(Module32First) || !(Module32Next) || !(CreateToolhelp32Snapshot))
		{
			Module32First = NULL;

			return FALSE;
		}
	}


	// toolhelp code

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	if ((int)hSnapshot == -1) {
		return FALSE;
	}

	MODULEENTRY32 lpme; lpme.dwSize = sizeof(MODULEENTRY32);

	// Get first module, this is needed for win9x/ME
	if (!Module32First(hSnapshot, &lpme)) {
		CloseHandle(hSnapshot);
		return FALSE;
	}

	// Loop through all other modules
	while (TRUE)
	{
		DWORD key = (DWORD)lpme.modBaseAddr;
		DllInfo& di = m_dlls[key];
		if( di.imageBase != key && di.imageSize != lpme.modBaseSize )
		{
			InitializeDllInfo(di, key, lpme.modBaseSize, lpme.szExePath);
		}
		else delDlls.erase(key);
		if (!Module32Next(hSnapshot, &lpme))
		{
			CloseHandle(hSnapshot);
			break;
		}
	}

	return TRUE;
}


BOOL CModuleWalker::WalkNT(DWORD pid, DelDllList& delDlls)
{
	typedef struct _DEBUGMODULEINFO	{
		DWORD	ImageBase;
		DWORD	ImageSize;
		DWORD	Unknown01;			// possibly some kind of version info. nthandle doesnt use it
		USHORT	DllSequenceNum;  // if 0 then EXE
		USHORT	NumDlls;			// only know if seqnum is 0
		DWORD	GrantedAccess;
		CHAR	Name[MAX_PATH];
		DWORD	Unknown02;
	}DEBUGMODULEINFO, *PDEBUGMODULEINFO;

	typedef struct _QUERYDEBUGBUFFER_HEADER	{
		DWORD	Unkown12[12];
		DWORD*	ModArrayHeader;
		DWORD	Unkown11[11];
		DWORD	NumNames;         // total entries including the EXE
		DWORD	Reserved[2];
		DEBUGMODULEINFO ModInfo[1];
	}QUERYDEBUGBUFFER, *PQUERYDEBUGBUFFER;

	typedef DWORD* (WINAPI *PRtlCreateQueryDebugBuffer)( DWORD, DWORD );
	typedef DWORD (WINAPI *PRtlDestroyQueryDebugBuffer)( PDWORD );
	typedef DWORD (WINAPI *PRtlQueryProcessDebugInformation)(HANDLE, DWORD, PVOID );

	static PRtlCreateQueryDebugBuffer RtlCreateQueryDebugBuffer;
	static PRtlDestroyQueryDebugBuffer RtlDestroyQueryDebugBuffer;
	static PRtlQueryProcessDebugInformation RtlQueryProcessDebugInformation;
	if(!RtlCreateQueryDebugBuffer)
	{
		HMODULE hNtDll = GetModuleHandle(_T("ntdll.dll"));
		if(!hNtDll)
		{
			return FALSE;
		}

		RtlCreateQueryDebugBuffer = (PRtlCreateQueryDebugBuffer)
			GetProcAddress( hNtDll,
			"RtlCreateQueryDebugBuffer" );

		RtlDestroyQueryDebugBuffer = (PRtlDestroyQueryDebugBuffer)
			GetProcAddress( hNtDll,
			"RtlDestroyQueryDebugBuffer" );

		RtlQueryProcessDebugInformation = (PRtlQueryProcessDebugInformation)
			GetProcAddress( hNtDll,
			"RtlQueryProcessDebugInformation" );
		if(!RtlCreateQueryDebugBuffer || !RtlDestroyQueryDebugBuffer || !RtlQueryProcessDebugInformation)
		{
			RtlCreateQueryDebugBuffer = NULL;
			return FALSE;
		}
	}

	DWORD *pRtlBuffer = RtlCreateQueryDebugBuffer(NULL, NULL);

	if( pRtlBuffer == NULL )
		return FALSE;


	DWORD error = RtlQueryProcessDebugInformation((HANDLE)pid, 1, pRtlBuffer);
	if( error != 0 )
	{
		RtlDestroyQueryDebugBuffer( pRtlBuffer );
		return FALSE;
	}

	QUERYDEBUGBUFFER *pDebugInfo = (QUERYDEBUGBUFFER*)pRtlBuffer;

	for(DWORD i = 0; i < pDebugInfo->NumNames; i++)
	{
		DllInfo& di = m_dlls[pDebugInfo->ModInfo[i].ImageBase];
		if( di.imageBase != pDebugInfo->ModInfo[i].ImageBase || di.imageSize != pDebugInfo->ModInfo[i].ImageSize )
		{
#ifdef UNICODE
			WCHAR szPath[MAX_PATH];
			mymbstowcs2(szPath, pDebugInfo->ModInfo[i].Name);
#else
			LPCSTR szPath = pDebugInfo->ModInfo[i].Name;
#endif
			InitializeDllInfo(di, pDebugInfo->ModInfo[i].ImageBase, pDebugInfo->ModInfo[i].ImageSize, szPath);
		}
		else delDlls.erase(pDebugInfo->ModInfo[i].ImageBase);
	}
	RtlDestroyQueryDebugBuffer( pRtlBuffer );
	return TRUE;
}

inline
void CModuleWalker::InitializeDllInfo(DllInfo& di, DWORD ImageBase, DWORD ImageSize, LPCTSTR lpModulePath)
{
	di.imagePath = lpModulePath;
	di.imageBase = ImageBase;
	di.imageSize = ImageSize;
}


CModuleWalker::DllInfo* CModuleWalker::FindModule( LPCTSTR pszModuleName, BOOL fIgnorePath, BOOL fFullMatch, LPTSTR pszProcessName, DWORD cbSize )
{
	if( m_dlls.empty() ) return NULL;

	LPCTSTR ps;
	const int clen = 4;
	DllList::iterator cit = m_dlls.begin();
	DllList::iterator cend = m_dlls.end();
	DllInfo* pdi = NULL;
	while(cit != cend && (!pdi || !cbSize))
	{
		DllInfo& di = cit->second;
		ps = di.imagePath.c_str();
		if(cbSize && pszProcessName
		&& di.imagePath.length() > clen
		&& _tcsnicmp(ps+di.imagePath.length()-clen, _T(".exe"), clen) == 0)
		{
			_tcsncpy(pszProcessName, GetFileNameFromPath(ps), cbSize);
			pszProcessName[cbSize-1] = _T('\0');
			cbSize = 0;
		}
		if( fIgnorePath )
			ps = GetFileNameFromPath(ps);
		if(fFullMatch)
		{
			if( _tcsicmp( ps, pszModuleName ) == 0 )
				pdi = &di;
		}
		else
		{
			if( _tcsstr( ps, pszModuleName ) != NULL )
				pdi = &di;
		}

		++cit;
	}
	return pdi;
}

CModuleWalker::DllInfo* CModuleWalker::FindModuleByAddress(DWORD address)
{
	DllList::iterator cit = m_dlls.begin();
	DllList::iterator end = m_dlls.end();
	for(; cit != end; ++cit)
	{
		DllInfo& di = cit->second;
		if(di.TestAddress(address))
		{
			return &di;
		}
	}
	return NULL;

}

BOOL CModuleWalker::TestModuleInProcess(DWORD processId, LPCTSTR pszModuleName)
{
	CModuleWalker walker;
	return walker.Walk(processId) && walker.FindModule( pszModuleName ) != NULL;
}
