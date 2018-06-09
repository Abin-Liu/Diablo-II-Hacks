//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#pragma once

#pragma warning( disable : 4786 )
#include <map>
#include "tstring.h"
#include "AutoList.h"

class CModuleWalker
{
public:
	struct DllInfo {
		DWORD imageBase;
		DWORD imageSize;
		tstring imagePath;
		BOOL TestAddress(DWORD addr) const { return imageBase < addr && imageBase+imageSize > addr; }
	};
	typedef std::map<DWORD,DllInfo> DllList;
	typedef AutoList<DllList> DelDllList;
	DllList m_dlls;
public:
	CModuleWalker(void);
	~CModuleWalker(void);
	BOOL Walk(DWORD pid);
	BOOL Walk(DWORD pid, DelDllList& delDlls);
	
	// helpers
	DllInfo* FindModule( LPCTSTR pszModuleName, BOOL fIgnorePath = TRUE, BOOL fFullMatch = TRUE, LPTSTR pszProcessName = NULL, DWORD cbSize=0 );
	DllInfo* FindModuleByAddress(DWORD address);
	static BOOL TestModuleInProcess(DWORD processId, LPCTSTR pszModuleName);

	// details
private:
	// toolhelp32 version
	BOOL WalkToolhelp32(DWORD pid, DelDllList& delDlls);
	// NT version
	BOOL WalkNT(DWORD pid, DelDllList& delDlls);
	static void InitializeDllInfo(DllInfo& di, DWORD ImageBase, DWORD ImageSize, LPCTSTR lpModulePath);
};
