//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////


#if !defined(AFX_REMOTEEXECUTE_H__6D53618E_72BB_4D86_89E7_9C5CCABD577D__INCLUDED_)
#define AFX_REMOTEEXECUTE_H__6D53618E_72BB_4D86_89E7_9C5CCABD577D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AutoHandle.h"

#define REXEC_FREE_MEM_AFTER_CALL 1
#define REXEC_NT_PREFER 2
#define REXEC_USE_WINDOW_HOOK 4

#define FUNCTLEN(func) ((DWORD)CRemoteExecute::GetFuncAddress(&func##_END)-(DWORD)CRemoteExecute::GetFuncAddress(&func))
#define DECLARE_FUNCTION_END(func) void _declspec(naked) func##_END(void){__asm call func}

class CRemoteExecute  
{
public:
	//----------------------------------------------------------------------
	// processId: target process id, required.
	// hwnd: target window handle. must not be NULL if specifying
	//       REXEC_USE_WINDOW_HOOK flag or not running on NT platform.
	// flags: flags for remote execute
	//          -- REXEC_NT_PREFER: use NT implementation if program is running
	//             on NT family OS (including NT4.0/2000/XP/2003 )
	//
	//          -- REXEC_FREE_MEM_AFTER_CALL: let injected code free memory
	//             after call, automatically enable if running on win9x/me
	//
	//          -- REXEC_USE_WINDOW_HOOK: use to execute under the hwnd's
	//             owner thread context, spcify this flag in case you know
	//             race condition exists.
	//----------------------------------------------------------------------
	BOOL OpenProcess(DWORD processId, HWND hwnd = NULL, DWORD flags = REXEC_NT_PREFER|REXEC_FREE_MEM_AFTER_CALL);
public:
	typedef DWORD (__stdcall *entry_t)(LPVOID);

	CRemoteExecute();
	~CRemoteExecute();
	static PVOID GetFuncAddress(PVOID addr);
	BOOL AllocMemory(DWORD size);
	LPVOID WriteProcessMemory(LPVOID buffer, DWORD size);
	BOOL ReadProcessMemory(LPCVOID lpBaseAddress, LPVOID lpBuffer, DWORD nSize);
	DWORD GetCurrentAvailableBase() const { return m_currAddr; }
	void WriteProcessBYTES(DWORD lpAddress, void* buf, int len);
	BOOL Execute(entry_t entry, DWORD nEntrySize, LPVOID param, DWORD nParamSize, DWORD& result);
private:
	BOOL ExecuteNT(entry_t entry, DWORD nEntrySize, LPVOID param, DWORD nParamSize, DWORD& result);
	LPVOID VirtualAllocEx9x(DWORD nSize);
	void AssignMemoryBlock(LPBYTE lpBaseAddress, DWORD nSize);
	DWORD GetCallContextSize();
	static DWORD GetBaseAddress(DWORD pid, LPCTSTR lpszModuleName);
private:
	AutoHandle<HANDLE> m_hProcess;
	DWORD m_processId;
	HWND m_hwnd;
	LPBYTE m_pAllocBase;
	DWORD m_currAddr;
	DWORD m_totalLen;
	DWORD m_flags;
};

#endif // !defined(AFX_REMOTEEXECUTE_H__6D53618E_72BB_4D86_89E7_9C5CCABD577D__INCLUDED_)
