//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ModuleWalker.h"
#include "ModuleLoader.h"
#include "RemoteExecute.h"

struct LoadModuleContext
{
	HMODULE (__stdcall *fnLoadLibrary)(LPCSTR lpFileName);
	BOOL (__stdcall *fnFreeLibrary)(HMODULE hModule);
	HMODULE hModule;
	char szModuleName[MAX_PATH];
};

DWORD __stdcall RemoteLoadModule(LoadModuleContext* pctx)
{
	DWORD retval;
	if(!pctx->hModule)
		retval = (DWORD)pctx->fnLoadLibrary(pctx->szModuleName);
	else
		retval = (DWORD)pctx->fnFreeLibrary(pctx->hModule);
	return retval;
}
DECLARE_FUNCTION_END(RemoteLoadModule)

HMODULE CModuleLoader::LoadModule( DWORD processId, HWND hwnd, LPCTSTR lpszModuleName, HMODULE hModule )
{
	if(!lpszModuleName && !hModule) return NULL;

	CRemoteExecute execContext;
	if(!execContext.OpenProcess(processId, hwnd,REXEC_NT_PREFER|REXEC_FREE_MEM_AFTER_CALL|REXEC_USE_WINDOW_HOOK))
		return FALSE;
	LoadModuleContext context;
	DWORD len = sizeof(LoadModuleContext)+FUNCTLEN(RemoteLoadModule);
	if(!execContext.AllocMemory(len))
		return FALSE;

	HMODULE hKernel32;

	LPCTSTR p = lpszModuleName ? lpszModuleName : _T("");
	useconv_tombstr(szModuleName, p);

	strncpy(context.szModuleName, szModuleName, sizeof(context.szModuleName)-1 );
	context.szModuleName[sizeof(context.szModuleName)-1] = '\0';

	context.hModule = hModule;
	
	hKernel32 = GetModuleHandle(_T("Kernel32.dll"));
	if (!hKernel32)
	{
		return NULL;
	}
	*(FARPROC*)&context.fnLoadLibrary = GetProcAddress(hKernel32, "LoadLibraryA");
	*(FARPROC*)&context.fnFreeLibrary = GetProcAddress(hKernel32, "FreeLibrary");


	DWORD retval;
	if(execContext.Execute(
				(CRemoteExecute::entry_t)CRemoteExecute::GetFuncAddress(&RemoteLoadModule),
				FUNCTLEN(RemoteLoadModule),
				&context, sizeof(context), retval))
				return (HMODULE)retval;
	return NULL;
}

