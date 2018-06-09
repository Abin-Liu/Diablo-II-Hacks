//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "RemoteExecute.h"
#include "ModuleWalker.h"

#define INST_CALL 0xe8
#define INST_JMP 0xe9

#define PatchSize			5
struct CallResult
{
	DWORD lasterr;
	DWORD retval;
};
struct CallContext
{
	BYTE oldcode[PatchSize];
	BYTE padding[3];
	DWORD (__stdcall *entry)(LPVOID);
	LPVOID param;
	LPVOID pAllocBase;
	HANDLE hProcess;
	CallResult* retval;
	BOOL (__stdcall *fnVirtualProtect)(LPVOID lpAddress,SIZE_T dwSize,DWORD flNewProtect,PDWORD lpflOldProtect);
	BOOL (__stdcall *fnWriteProcessMemory)(HANDLE hProcess,LPVOID lpBaseAddress,LPVOID lpBuffer,DWORD nSize,LPDWORD lpNumberOfBytesWritten);
	BOOL (__stdcall *fnCloseHandle)(HANDLE hObject);
	BOOL (__stdcall *fnVirtualFree)(LPVOID lpAddress,SIZE_T dwSize,DWORD dwFreeType);
	DWORD (__stdcall *fnGetLastError)(void);
	DWORD cbSize;
};

struct CallContextNT : public CallContext
{
	HMODULE (WINAPI *fnGetModuleHandle)(LPCSTR lpModuleName);
	FARPROC (WINAPI *fnGetProcAddress)(HMODULE hModule,LPCSTR lpProcName);
	LRESULT (WINAPI *fnSendMessage)(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam);
	LONG (WINAPI *fnSetWindowLong)(HWND hWnd,int nIndex,LONG dwNewLong);
	union {
		CallResult cr;
		struct {
			HWND hwnd;
			WNDPROC WndProcEntry;
		};
	};
	LRESULT (WINAPI *fnCallWindowProc)(WNDPROC lpPrevWndFunc,HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam);
	WNDPROC lpPrevWndFunc;
};

struct StackVars
{
	struct
	{
		DWORD edi;
		DWORD esi;
		DWORD ebp;
		DWORD esp;
		DWORD ebx;
		DWORD edx;
		DWORD ecx;
		DWORD eax;
	};
	DWORD retaddr;
	WNDPROC lpPrevWndFunc;
	HWND hwnd;
	UINT uMsg;
	WPARAM wParam;
	LPARAM lParam;
};

void _declspec(naked) CallEntry_ASM(void)
{
	__asm {
		pushad;
		call geteip;					// Get our EIP
geteip:
		pop ebp;
		sub ebp, 6;					// We now have base address in ebp
		mov eax, [ebp-4];					// size of parameter
		sub ebp, eax;					// offset to data
		
		// modify return address
		mov ebx, [esp]StackVars.retaddr;
		sub ebx, 5;
		mov [esp]StackVars.retaddr, ebx;
		
		// mark as writable
		push eax; // space for original protect
		push esp; // address
		push PAGE_READWRITE;
		push PatchSize;
		push ebx;
		call [ebp]CallContext.fnVirtualProtect;
		
		// restore oldcode
		lea esi, [ebp]CallContext.oldcode;
		mov edi, [esp+4]StackVars.retaddr;
		mov ecx, PatchSize;
		rep movsb;
		
		// give back old protect
		mov ebx, [esp+4]StackVars.retaddr;
		push esp;		// address for return protect flag
		push [esp+4];  // original protect
		push PatchSize;
		push ebx;
		call [ebp]CallContext.fnVirtualProtect;
		pop eax;

		mov eax, [ebp]CallContext.param
		push eax
		call [ebp]CallContext.entry

		push eax;
		call [ebp]CallContext.fnGetLastError;
		push eax;
		push NULL;
		push 8;
		lea eax, [esp+8];
		push eax;
		push [ebp]CallContext.retval;
		push [ebp]CallContext.hProcess;
		call [ebp]CallContext.fnWriteProcessMemory;
		pop eax;
		pop eax;

		push [ebp]CallContext.hProcess;
		call [ebp]CallContext.fnCloseHandle;

		cmp [ebp]CallContext.fnVirtualFree, 0;
		jne setmefree;
		// return normally
		popad;
		ret;
setmefree:
		// free myself
		// prepare code for after VirtualFree return
		push 0xccc36158; // = pop eax; popad; ret; int 3
		// parameters of VirtualFree
		push MEM_RELEASE;
		push 0;
		push [ebp]CallContext.pAllocBase;
		lea eax, [esp+0x0c]; // load return address of VirtualFree
		push eax; // return address of VirtualFree
		push [ebp]CallContext.fnVirtualFree;
		ret; // call VirtualFree
	}
}
DECLARE_FUNCTION_END(CallEntry_ASM)

struct VirtualAllocCallContext
{
	LPVOID (__stdcall *fnVirtualAlloc)(LPVOID lpAddress,SIZE_T dwSize,DWORD flAllocationType,DWORD flProtect);
	DWORD nSize;
};

void _declspec(naked) __stdcall RemoteVirtualAlloc_ASM(VirtualAllocCallContext* pctx)
{
	__asm
	{
		mov ecx, [esp+4];
		push PAGE_EXECUTE_READWRITE;
		push MEM_COMMIT;
		push [ecx]VirtualAllocCallContext.nSize;
		push NULL;
		call [ecx]VirtualAllocCallContext.fnVirtualAlloc;
		ret 4;
	}
}

DECLARE_FUNCTION_END(RemoteVirtualAlloc_ASM)


DWORD _declspec(naked) __stdcall RemoteThreadProc_ASM( CallContextNT* pctx )
{
	DWORD user32;
	HWND hwnd;
	WNDPROC lpWndProc;
	DWORD freestub[2];
	union {
		char szUser32Name[12];
		DWORD dwUser32Name[3];
	};
	__asm
	{
		push ebp;
		mov ebp, esp;
		sub esp, __LOCAL_SIZE;
	}

	if(!pctx->hwnd)
	{
		pctx->cr.retval = pctx->entry(pctx->param);
		pctx->cr.lasterr = pctx->fnGetLastError();
	}
	else
	{
		// construct string: "USER32.DLL"
		dwUser32Name[0] = 0x52455355;
		dwUser32Name[1] = 0x442e3233;
		dwUser32Name[2] = 0x00004c4c;
		user32 = (DWORD)pctx->fnGetModuleHandle(szUser32Name);
		if(user32)
		{
			*(DWORD*)&pctx->fnSendMessage = user32 + (DWORD)pctx->fnSendMessage;
			*(DWORD*)&pctx->fnSetWindowLong = user32 + (DWORD)pctx->fnSetWindowLong;
			*(DWORD*)&pctx->fnCallWindowProc = user32 + (DWORD)pctx->fnCallWindowProc;

			hwnd = pctx->hwnd;
			lpWndProc = pctx->WndProcEntry;
			pctx->cr.retval = 0;
			pctx->cr.lasterr = -1;

			pctx->lpPrevWndFunc = (WNDPROC)pctx->fnSetWindowLong(hwnd, GWL_WNDPROC, (LONG)lpWndProc);
			pctx->fnSendMessage(hwnd, WM_APP, 0, 0);
		}
		else
		{
			pctx->cr.lasterr = pctx->fnGetLastError();
			pctx->cr.retval = 0;
		}
	}
	pctx->fnWriteProcessMemory(pctx->hProcess, pctx->retval, &pctx->cr, sizeof(CallResult), NULL);
	pctx->fnCloseHandle(pctx->hProcess);
	
	if(pctx->fnVirtualFree)
	{
		// construct code: xor eax, eax, mov esp, ebp, pop ebp, ret 4
		freestub[0] = 0xE58BC033;
		freestub[1] = 0x0004C25D;
		__asm
		{
			push MEM_RELEASE;
			push 0;
			mov ecx, pctx;
			push [ecx]CallContext.pAllocBase;
			lea eax, freestub; // load return address of VirtualFree
			push eax; // return address of VirtualFree
			push [ecx]CallContext.fnVirtualFree;
			ret; // call VirtualFree
		}
	}

	__asm      /* epilog */
	{
		xor eax, eax;
		mov esp, ebp;
		pop ebp;
		ret 4;
	}
}
DECLARE_FUNCTION_END(RemoteThreadProc_ASM)

LRESULT _declspec(naked) CALLBACK RemoteWndProcEntry_ASM(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CallContextNT* pctx;
	__asm {
		jmp realentry;
		nop;
		nop;
		nop;
		nop;
	}
realentry:
	__asm
	{
		call geteip;
geteip:
		pop eax;
		sub eax, 5+4;
		mov eax, [eax];

		push ebp;
		mov ebp, esp;
		sub esp, __LOCAL_SIZE;
	}
	__asm mov pctx, eax;

	if(pctx->fnSetWindowLong)
	{
		pctx->fnSetWindowLong(hwnd, GWL_WNDPROC, (LONG)pctx->lpPrevWndFunc);
		pctx->fnSetWindowLong = NULL;
		pctx->cr.retval = pctx->entry(pctx->param);
		pctx->cr.lasterr = pctx->fnGetLastError();
	}
	pctx->fnCallWindowProc(pctx->lpPrevWndFunc, hwnd, uMsg, wParam, lParam);

	__asm      /* epilog */
	{
		mov esp, ebp;
		pop ebp;
		ret 0x10;
	}
}
DECLARE_FUNCTION_END(RemoteWndProcEntry_ASM)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static LPVOID	(WINAPI *NTAPI_VirtualAllocEx)(HANDLE hProcess,LPVOID lpAddress,SIZE_T dwSize,DWORD flAllocationType,DWORD flProtect);
static BOOL		(WINAPI *NTAPI_VirtualFreeEx)(HANDLE hProcess,LPVOID lpAddress,SIZE_T dwSize,DWORD dwFreeType);
static HANDLE	(WINAPI *NTAPI_CreateRemoteThread)(HANDLE hProcess,LPSECURITY_ATTRIBUTES lpThreadAttributes,SIZE_T dwStackSize,LPTHREAD_START_ROUTINE lpStartAddress,LPVOID lpParameter,DWORD dwCreationFlags,LPDWORD lpThreadId);


CRemoteExecute::CRemoteExecute()
			:m_pAllocBase(NULL)
{
	if(g_isNTPlatform && !NTAPI_VirtualAllocEx)
	{
		HMODULE h = GetModuleHandle(_T("KERNEL32.DLL"));
		*(FARPROC*)&NTAPI_VirtualAllocEx = GetProcAddress(h, "VirtualAllocEx");
		*(FARPROC*)&NTAPI_VirtualFreeEx = GetProcAddress(h, "VirtualFreeEx");
		*(FARPROC*)&NTAPI_CreateRemoteThread = GetProcAddress(h, "CreateRemoteThread");
	}
}

CRemoteExecute::~CRemoteExecute()
{
	if(m_pAllocBase && NTAPI_VirtualFreeEx)
		NTAPI_VirtualFreeEx(m_hProcess, m_pAllocBase, 0, MEM_RELEASE);
}

PVOID CRemoteExecute::GetFuncAddress(PVOID addr)
{
#ifdef _DEBUG
	//check if instruction is relative jump (E9)
	if (INST_JMP != *((UCHAR*)addr))
		return addr;
	// calculate base of relative jump
	ULONG base = (ULONG)((UCHAR*)addr + 5);
	// calculate offset 
	ULONG *offset = (ULONG*)((UCHAR*)addr + 1);
	return (PVOID)(base + *offset);
#else
	return addr;
#endif // _DEBUG
}

DWORD CRemoteExecute::GetBaseAddress(DWORD pid, LPCTSTR lpszModuleName)
{
	if (!pid || !lpszModuleName) return 0;
	CModuleWalker walker;
	if( !walker.Walk(pid) ) return 0;
	
	CModuleWalker::DllList::const_iterator cit = walker.m_dlls.begin();
	size_t len = _tcslen(lpszModuleName);
	while( cit != walker.m_dlls.end() )
	{
		const CModuleWalker::DllInfo& di = (*cit).second;
		if (len <= di.imagePath.length() &&
			_tcsnicmp(di.imagePath.c_str()+di.imagePath.length()-len, lpszModuleName, len) == 0)
		{
			return di.imageBase;
		}
		++cit;
	}
	return 0;
}

LPVOID CRemoteExecute::VirtualAllocEx9x(DWORD nSize)
{
	DWORD PatchAddress = GetBaseAddress(m_processId, _T(".exe"));
	if (!PatchAddress) { 
		return NULL;
	}
	PatchAddress += 0x400;
	const int codelen = FUNCTLEN(RemoteVirtualAlloc_ASM);
	DWORD patchlen = codelen+sizeof(VirtualAllocCallContext)+GetCallContextSize();

	LPVOID pAllocBase = NULL;
	VirtualAllocCallContext context;
	context.nSize = nSize;
	*(FARPROC*)&context.fnVirtualAlloc = GetProcAddress(GetModuleHandle(_T("KERNEL32.DLL")), "VirtualAlloc");
	if(!context.fnVirtualAlloc)
		return NULL;


	DWORD oldprot;
	if(!VirtualProtectEx(m_hProcess, (LPVOID)PatchAddress, patchlen, PAGE_EXECUTE_READWRITE, &oldprot))
		return NULL;

	LPBYTE oldpatch = new BYTE [patchlen];
	if(!ReadProcessMemory((LPVOID)PatchAddress, oldpatch, sizeof(patchlen)))
		return NULL;

	DWORD retval = 0;
	__try {

		AssignMemoryBlock((LPBYTE)PatchAddress, patchlen);
		if(!Execute((entry_t)GetFuncAddress(RemoteVirtualAlloc_ASM),
			FUNCTLEN(RemoteVirtualAlloc_ASM),
			&context, sizeof(context),
			retval
			))
			retval = 0;


	}__finally{
		WriteProcessBYTES(PatchAddress, oldpatch, sizeof(patchlen));
		delete [] oldpatch;
		AssignMemoryBlock(0, 0);
	}
	VirtualProtect((LPVOID)PatchAddress, patchlen, oldprot, NULL);

	return (LPVOID)retval;
}

BOOL CRemoteExecute::OpenProcess(DWORD processId, HWND hwnd, DWORD execFlags)
{
	if( hwnd == NULL && ((execFlags&REXEC_USE_WINDOW_HOOK) ||!g_isNTPlatform) )
		return FALSE;
	m_hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	if(m_hProcess != NULL)
	{
		m_processId = processId;
		m_hwnd = hwnd;
		m_flags = execFlags;
		if(!g_isNTPlatform)
			m_flags |= REXEC_FREE_MEM_AFTER_CALL;
	}
	return m_hProcess != NULL;
}

void CRemoteExecute::AssignMemoryBlock(LPBYTE lpBaseAddress, DWORD nSize)
{
	m_pAllocBase = lpBaseAddress;
	m_totalLen = nSize;
	m_currAddr = (DWORD)(LPBYTE)m_pAllocBase;
}

DWORD CRemoteExecute::GetCallContextSize()
{
	DWORD size;
	if((m_flags&REXEC_NT_PREFER) && g_isNTPlatform)
	{
		size = sizeof(CallContextNT)+FUNCTLEN(RemoteThreadProc_ASM);
		if(m_flags&REXEC_USE_WINDOW_HOOK)
			size += FUNCTLEN(RemoteWndProcEntry_ASM);
	}
	else
		size = sizeof(CallContext)+FUNCTLEN(CallEntry_ASM);
	return size;
}

BOOL CRemoteExecute::AllocMemory(DWORD size)
{
	if(m_hProcess == NULL) return FALSE;
	size += GetCallContextSize();
	
	LPBYTE pBaseAddress = NTAPI_VirtualAllocEx ? (LPBYTE)NTAPI_VirtualAllocEx(m_hProcess, NULL, size, MEM_COMMIT, PAGE_EXECUTE_READWRITE) : (LPBYTE)VirtualAllocEx9x(size);
	if(pBaseAddress == NULL)
		return FALSE;
	AssignMemoryBlock(pBaseAddress, size);
	return TRUE;
}

LPVOID CRemoteExecute::WriteProcessMemory(LPVOID buffer, DWORD size)
{
	if(m_hProcess == NULL || m_pAllocBase == NULL) return NULL;
	if(m_currAddr+size > ((DWORD)(LPBYTE)m_pAllocBase)+m_totalLen) return NULL;
	DWORD cbSize = 0;
	LPVOID addr = (LPVOID)m_currAddr;
	if(!::WriteProcessMemory(m_hProcess, addr, buffer, size, &cbSize))
		return NULL;
	m_currAddr += cbSize;
	return addr;
}

void CRemoteExecute::WriteProcessBYTES(DWORD lpAddress, void* buf, int len)
{
	DWORD oldprot,dummy = 0;
	VirtualProtectEx(m_hProcess, (void*) lpAddress, len, PAGE_READWRITE, &oldprot);
	::WriteProcessMemory(m_hProcess, (void*) lpAddress, buf, len, 0);
	VirtualProtectEx(m_hProcess, (void*) lpAddress, len, oldprot, &dummy);
}

BOOL CRemoteExecute::ReadProcessMemory(LPCVOID lpBaseAddress, LPVOID lpBuffer, DWORD nSize)
{
	if(m_hProcess == NULL) return FALSE;
	DWORD cbSize;
	return ::ReadProcessMemory(m_hProcess, lpBaseAddress, lpBuffer, nSize, &cbSize);
}

BOOL CRemoteExecute::Execute(entry_t entry, DWORD nEntrySize, LPVOID param, DWORD nParamSize, DWORD& result)
{
	if((m_flags&REXEC_NT_PREFER) && g_isNTPlatform)
		return ExecuteNT(entry, nEntrySize, param, nParamSize, result);

	// Get location of the game's WinProc
	// must call GetClassLongA here, GetClassLongW doesn't work
	DWORD WinProcAddress = GetClassLongA(m_hwnd,GCL_WNDPROC);
	if (!WinProcAddress)
	{
		return FALSE;
	}

	LPVOID EntryAddress = WriteProcessMemory(entry, nEntrySize);
	if(!EntryAddress) return FALSE;
	LPVOID ParamAddress = WriteProcessMemory(param, nParamSize);
	if(!ParamAddress) return FALSE;
	
	CallResult retval = {0, 0};
	CallContext context;
	context.cbSize = sizeof(context);
	context.entry = (entry_t)EntryAddress;
	context.param = ParamAddress;
	context.retval = &retval;

	HMODULE hKernel32 = GetModuleHandle(_T("KERNEL32.DLL"));
	*(FARPROC*)&context.fnVirtualProtect = GetProcAddress(hKernel32, "VirtualProtect");
	*(FARPROC*)&context.fnWriteProcessMemory = GetProcAddress(hKernel32, "WriteProcessMemory");
	*(FARPROC*)&context.fnCloseHandle = GetProcAddress(hKernel32, "CloseHandle");
	*(FARPROC*)&context.fnGetLastError = GetProcAddress(hKernel32, "GetLastError");
	if(!context.fnVirtualProtect || !context.fnWriteProcessMemory || !context.fnCloseHandle
		|| !context.fnGetLastError) {
			return FALSE;
		}

	if(m_flags&REXEC_FREE_MEM_AFTER_CALL)
	{
		// let remote routine free himself
		*(FARPROC*)&context.fnVirtualFree = GetProcAddress(hKernel32, "VirtualFree");
		if(!context.fnVirtualFree)
			return FALSE;
		context.pAllocBase = m_pAllocBase;
	}
	else
	{
		context.pAllocBase = NULL;
		context.fnVirtualFree = NULL;
	}

	ReadProcessMemory((LPVOID)WinProcAddress, context.oldcode, sizeof(context.oldcode));

	if(!DuplicateHandle(GetCurrentProcess(), GetCurrentProcess(), m_hProcess, &context.hProcess, 0, FALSE, DUPLICATE_SAME_ACCESS ))
		return FALSE;

	ParamAddress = WriteProcessMemory(&context, sizeof(context));
	if(!ParamAddress) return FALSE;
	
	DWORD PatchAddress
		= (DWORD)WriteProcessMemory(
		GetFuncAddress(&CallEntry_ASM),
		FUNCTLEN(CallEntry_ASM));
	if(!PatchAddress) return FALSE;
	
	// Now hi-jack WinProc of the game to our code
	BYTE buffer[5];
	buffer[0] = INST_CALL;
	*(DWORD*) (buffer + 1) = PatchAddress - (WinProcAddress + 5);
	WriteProcessBYTES(WinProcAddress, buffer, PatchSize);

	SendMessage(m_hwnd, WM_APP, 0, 0);

	SetLastError(retval.lasterr);

	result = retval.retval;
	
	if(m_flags&REXEC_FREE_MEM_AFTER_CALL)
		AssignMemoryBlock(0, 0);
	return TRUE;
}

BOOL CRemoteExecute::ExecuteNT(entry_t entry, DWORD nEntrySize, LPVOID param, DWORD nParamSize, DWORD& result)
{
	if(!NTAPI_CreateRemoteThread) return FALSE;

	LPVOID EntryAddress = WriteProcessMemory(entry, nEntrySize);
	if(!EntryAddress) return FALSE;
	LPVOID ParamAddress = WriteProcessMemory(param, nParamSize);
	if(!ParamAddress) return FALSE;

	CallResult retval = {0, 0};
	CallContextNT context;
	context.cbSize = sizeof(context);
	context.entry = (entry_t)EntryAddress;
	context.param = ParamAddress;
	context.retval = &retval;

	HMODULE hKernel32 = GetModuleHandle(_T("KERNEL32.DLL"));
	*(FARPROC*)&context.fnVirtualProtect = GetProcAddress(hKernel32, "VirtualProtect");
	*(FARPROC*)&context.fnWriteProcessMemory = GetProcAddress(hKernel32, "WriteProcessMemory");
	*(FARPROC*)&context.fnCloseHandle = GetProcAddress(hKernel32, "CloseHandle");
	*(FARPROC*)&context.fnGetLastError = GetProcAddress(hKernel32, "GetLastError");
	if(!context.fnVirtualProtect || !context.fnWriteProcessMemory || !context.fnCloseHandle
		|| !context.fnGetLastError) {
			return FALSE;
	}

	if(m_flags&REXEC_FREE_MEM_AFTER_CALL)
	{
		// let remote routine free himself
		*(FARPROC*)&context.fnVirtualFree = GetProcAddress(hKernel32, "VirtualFree");
		if(!context.fnVirtualFree)
			return FALSE;
		context.pAllocBase = m_pAllocBase;
	}
	else
	{
		context.pAllocBase = NULL;
		context.fnVirtualFree = NULL;
	}
	if(m_flags&REXEC_USE_WINDOW_HOOK)
	{
		if(!m_hwnd) return FALSE;
		context.hwnd = m_hwnd;
		*(FARPROC*)&context.fnGetModuleHandle = GetProcAddress(hKernel32, "GetModuleHandleA");
		*(FARPROC*)&context.fnGetProcAddress = GetProcAddress(hKernel32, "GetProcAddress");
		HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
		if(!hUser32) return FALSE;
		*(FARPROC*)&context.fnSendMessage = GetProcAddress(hUser32, "SendMessageA");
		*(FARPROC*)&context.fnSetWindowLong = GetProcAddress(hUser32, "SetWindowLongA");
		*(FARPROC*)&context.fnCallWindowProc = GetProcAddress(hUser32, "CallWindowProcA");
		if(!context.fnSendMessage || !context.fnSetWindowLong || !context.fnCallWindowProc)
			return FALSE;
		*(DWORD*)&context.fnSendMessage = (DWORD)context.fnSendMessage - (DWORD)hUser32;
		*(DWORD*)&context.fnSetWindowLong = (DWORD)context.fnSetWindowLong - (DWORD)hUser32;
		*(DWORD*)&context.fnCallWindowProc = (DWORD)context.fnCallWindowProc - (DWORD)hUser32;

		context.WndProcEntry = (WNDPROC)WriteProcessMemory(GetFuncAddress(&RemoteWndProcEntry_ASM), FUNCTLEN(RemoteWndProcEntry_ASM));
		if(!context.WndProcEntry) return FALSE;
	}
	else
		context.hwnd = NULL;


	if(!DuplicateHandle(GetCurrentProcess(), GetCurrentProcess(), m_hProcess, &context.hProcess, 0, FALSE, DUPLICATE_SAME_ACCESS ))
		return FALSE;

	ParamAddress = WriteProcessMemory(&context, sizeof(context));
	if(!ParamAddress) return FALSE;

	if(m_hwnd)
	{
		WriteProcessBYTES((DWORD)context.WndProcEntry+2, &ParamAddress, sizeof(ParamAddress));
	}

	DWORD PatchEntry
		= (DWORD)WriteProcessMemory(
		GetFuncAddress(&RemoteThreadProc_ASM),
		FUNCTLEN(RemoteThreadProc_ASM));
	if(!PatchEntry) return FALSE;

	DWORD tid;
	AutoHandle<HANDLE> ht = NTAPI_CreateRemoteThread( m_hProcess, 0, 0, (DWORD (WINAPI *)( PVOID))PatchEntry, ParamAddress, 0, &tid );
	if ( ht == NULL )
		return FALSE;

	::WaitForSingleObject( ht, INFINITE );
	SetLastError(retval.lasterr);
	result = retval.retval;

	if(m_flags&REXEC_FREE_MEM_AFTER_CALL)
		AssignMemoryBlock(0, 0);
	return TRUE;
}
