//////////////////////////////////////////////////////////////////////
// IntegrityWalker.cpp: implementation of the CIntegrityWalker class.
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <algorithm>
#include "ModuleWalker.h"
#include "IntegrityWalker.h"
#include "AutoHandle.h"

#define INST_CALL 0xe8
#define INST_JMP 0xe9
#define INST_JMPR 0xeb
#define INST_JNZ 0x85

struct InstInterpret
{
	BYTE inst;
	int instlen;
	void (__fastcall *fnFormatDispString)(CModuleWalker& dllWalker, DWORD realCallerAddress, LPBYTE caller, tstring& strDisp);
} aInstInterpreters[] = {
	{INST_CALL, 5, CIntegrityWalker::FormatDispString_CALL},
	{INST_JMP, 5, CIntegrityWalker::FormatDispString_JMP},
	{INST_JMPR, 2, CIntegrityWalker::FormatDispString_JMPR},
//	{INST_JNZ, 5, CIntegrityWalker::FormatDispString_JNZ},
};


#define NTSIGNATURE(ptr) ((LPVOID)((BYTE *)(ptr) + ((PIMAGE_DOS_HEADER)(ptr))->e_lfanew))
#define SIZE_OF_NT_SIGNATURE (sizeof(DWORD))
#define PEFHDROFFSET(ptr) ((LPVOID)((BYTE *)(ptr)+((PIMAGE_DOS_HEADER)(ptr))->e_lfanew+SIZE_OF_NT_SIGNATURE))
#define OPTHDROFFSET(ptr) ((LPVOID)((BYTE *)(ptr)+((PIMAGE_DOS_HEADER)(ptr))->e_lfanew+SIZE_OF_NT_SIGNATURE+sizeof(IMAGE_FILE_HEADER)))
#define SECHDROFFSET(ptr) ((LPVOID)((BYTE *)(ptr)+((PIMAGE_DOS_HEADER)(ptr))->e_lfanew+SIZE_OF_NT_SIGNATURE+sizeof(IMAGE_FILE_HEADER)+sizeof(IMAGE_OPTIONAL_HEADER)))
#define RVATOVA(base,offset) ((LPVOID)((DWORD)(base)+(DWORD)(offset)))
#define VATORVA(base,offset) ((LPVOID)((DWORD)(offset)-(DWORD)(base)))

#pragma pack(push,1)

typedef struct {
	DWORD	dwPageRVA;
	DWORD	dwBlockSize;
} IMAGE_FIXUP_BLOCK, *PIMAGE_FIXUP_BLOCK;

typedef struct {
	WORD	offset:12;
	WORD	type:4;
} IMAGE_FIXUP_ENTRY, *PIMAGE_FIXUP_ENTRY;

#pragma pack(pop)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIntegrityWalker::CIntegrityWalker()
{

}

CIntegrityWalker::~CIntegrityWalker()
{

}

static TCHAR *checkingDllsName[] = {
	_T("D2CLIENT.DLL"),
	_T("D2COMMON.DLL"),
	_T("D2GFX.DLL"),
	_T("D2WIN.DLL"),
	_T("D2LANG.DLL"),
	_T("D2CMP.DLL"),
	_T("STORM.DLL"),
	_T("D2GAME.DLL"),
	_T("D2NET.DLL"),
	_T("D2MCPCLIENT.DLL"),
	_T("BNCLIENT.DLL"),
	_T("D2LAUNCH.DLL"),
	_T("D2GDI.DLL"),
	_T("D2SOUND.DLL"),
	_T("D2MULTI.DLL"),
	_T("FOG.DLL"),
};
void CIntegrityWalker::PrepareSpecificDllReferences(DllRefList& dlls, TCHAR *dllNames[], int num)
{
	DllList::iterator it = m_remoteDllWalker.m_dlls.begin();
	DllList::iterator end = m_remoteDllWalker.m_dlls.end();
	for(; it != end; ++it)
	{
		DllInfo& di = (*it).second;
		for(int i = 0; i < num; ++i)
		{
			size_t len = _tcslen(dllNames[i]);
			if(len <= di.imagePath.length() && _tcsnicmp(di.imagePath.c_str()+di.imagePath.length()-len, dllNames[i], len) == 0)
			{
				dlls[di.imageBase] = &di;
			}
		}
	}
}

void CIntegrityWalker::PrepareAllDllReferences(DllRefList& dlls)
{
	DllList::iterator it = m_remoteDllWalker.m_dlls.begin();
	DllList::iterator end = m_remoteDllWalker.m_dlls.end();
	for(; it != end; ++it)
	{
		DllInfo& di = (*it).second;
		dlls[di.imageBase] = &di;
	}
}

DWORD CIntegrityWalker::FindMaxImageSize(const DllRefList& dlls)
{
	DWORD size = 0;
	DllRefList::const_iterator cit = dlls.begin();
	DllRefList::const_iterator end = dlls.end();
	for(; cit != end; ++cit)
	{
		if((*cit).second->imageSize > size )
			size = (*cit).second->imageSize;
	}
	return size;
}

BOOL CIntegrityWalker::Walk(DWORD processId, BOOL fGameDllsOnly)
{
	DelModList delMods(m_mods);
	return Walk(processId, fGameDllsOnly, delMods);
}

BOOL CIntegrityWalker::Walk(DWORD processId, BOOL fGameDllsOnly, DelModList& delMods)
{
	AutoHandle<HANDLE> hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	if(hProcess == NULL ) return FALSE;
	if(!m_remoteDllWalker.Walk(processId)) return FALSE;
	
	
	DllRefList checkingDlls;
	if(fGameDllsOnly)
		PrepareSpecificDllReferences(checkingDlls, checkingDllsName, COUNTOF(checkingDllsName));
	else
		PrepareAllDllReferences(checkingDlls);
	
	DWORD maxImageSize = FindMaxImageSize(checkingDlls);
	AutoHandle<LPBYTE> pImageBase = (LPBYTE)VirtualAlloc(NULL, maxImageSize, MEM_COMMIT, PAGE_READWRITE);
	if(pImageBase == NULL) return FALSE;
	AutoHandle<LPBYTE> pLocalImageBase = (LPBYTE)VirtualAlloc(NULL, maxImageSize, MEM_COMMIT, PAGE_READWRITE);
	if(pLocalImageBase == NULL) return FALSE;
	
	DWORD cbSize;
	DllRefList::iterator cit = checkingDlls.begin();
	DllRefList::iterator end = checkingDlls.end();
	for(; cit != end; ++cit)
	{
		DllInfo& di = *(*cit).second;
		if(ReadProcessMemory(hProcess, (LPBYTE)di.imageBase, pImageBase, di.imageSize, &cbSize))
		{
			WalkDll(di, pLocalImageBase, pImageBase, delMods);
		}
	}
	
	return TRUE;
}

void CIntegrityWalker::WalkDll(DllInfo& currCheckingDll, LPBYTE pLocalImageBase, LPBYTE pPreparedImageBase, DelModList& delMods)
{
	if(LoadLocalCleanImage(currCheckingDll.imagePath.c_str(), pLocalImageBase, pPreparedImageBase, currCheckingDll.imageBase))
		CompareImage(currCheckingDll, pLocalImageBase, pPreparedImageBase, delMods);
}

HMODULE MyLoadLibraryEx(LPCTSTR lpFileName)
{
	if(g_isNTPlatform)
		return LoadLibraryEx(lpFileName, NULL, DONT_RESOLVE_DLL_REFERENCES );

	TCHAR szCurrentDirectory[MAX_PATH];
	GetCurrentDirectory(COUNTOF(szCurrentDirectory), szCurrentDirectory);

	LPTSTR p = _tcsrchr(lpFileName, _T('\\'));
	if(p)
	{
		*p = _T('\0');
		SetCurrentDirectory(lpFileName);
		*p = _T('\\');
	}

	HMODULE hDll = LoadLibrary(lpFileName);
	SetCurrentDirectory(szCurrentDirectory);

	return hDll;
}

BOOL CIntegrityWalker::PrepareDLLImage(void *pMemoryImage, void* pFileImage, void* pPreparedImage)
{
	//-----------------------------
	// Get Number of Sections
	//-----------------------------
	PIMAGE_FILE_HEADER pfh;

	pfh=(PIMAGE_FILE_HEADER) PEFHDROFFSET(pFileImage);
	int nSectionCount=pfh->NumberOfSections;

	//-------------------------------------------------
	// Get PE Header Length + Section Header Length
	//-------------------------------------------------
	PIMAGE_OPTIONAL_HEADER poh;

	poh=(PIMAGE_OPTIONAL_HEADER) OPTHDROFFSET(pFileImage);
	memcpy(pMemoryImage, pPreparedImage, poh->SizeOfImage);
	memcpy(pMemoryImage, pFileImage, poh->SizeOfHeaders);


	PIMAGE_SECTION_HEADER psh;

	psh=(PIMAGE_SECTION_HEADER) SECHDROFFSET(pFileImage);

	for( int i = 0; i < nSectionCount; i++ )
	{
		void *secMemAddr, *secFileAddr;

		secFileAddr = (char *)pFileImage + psh->PointerToRawData;
		secMemAddr  = (char *)RVATOVA(pMemoryImage, psh->VirtualAddress);

		// only recovery code section here
		// can check more section if neccessary
		if((psh->Characteristics & IMAGE_SCN_MEM_WRITE)==0)//IMAGE_SCN_MEM_EXECUTE)
		{
			memcpy(secMemAddr,secFileAddr,psh->SizeOfRawData);
		}
		psh++;
	}

	return TRUE;
}

BOOL CIntegrityWalker::FixupDLLImage(void *pMemoryImage, void* pPreparedImage, DWORD remoteImageBase)
{
	//-----------------------------------------------
	// Get headers
	//-----------------------------------------------
	PIMAGE_OPTIONAL_HEADER   poh;
	PIMAGE_SECTION_HEADER    psh;
	poh = (PIMAGE_OPTIONAL_HEADER)OPTHDROFFSET (pMemoryImage);
	psh = (PIMAGE_SECTION_HEADER)SECHDROFFSET (pMemoryImage);


	//-----------------------------------------------
	// Get number of image directories in list
	//-----------------------------------------------
	int nDirCount;
	nDirCount=poh->NumberOfRvaAndSizes;
	if(nDirCount<16) return FALSE;

	DllInfo *pdi = NULL;
	DllInfo *lfpdi = NULL; // local forward pdi
	DllInfo *rfpdi = NULL; // remote forward pdi
	HMODULE hDll = NULL;

	//-----------------------------------------------
	// Process import table
	//-----------------------------------------------
	if(poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size!=0)
	{
		PIMAGE_IMPORT_DESCRIPTOR pid;
		pid=(IMAGE_IMPORT_DESCRIPTOR *)RVATOVA(pMemoryImage,poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

		PIMAGE_IMPORT_DESCRIPTOR pid2;
		pid2=(IMAGE_IMPORT_DESCRIPTOR *)RVATOVA(pPreparedImage,poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

		//---------------------------------------------------
		// For all imported DLLs
		// OriginalFirstThunk is always zero for image generated by borland's linker
		//---------------------------------------------------
		while( 1 )
		{
			if ( (pid->TimeDateStamp==0 ) && (pid->Name==0) )
				break;


			//-------------------------------------------------------------
			// Store DLL infoz
			//-------------------------------------------------------------
			pid->ForwarderChain=pid2->ForwarderChain;

			pid->TimeDateStamp=pid2->TimeDateStamp;

			//-------------------------------------------------------------
			// Fill in Import Address Table
			//-------------------------------------------------------------
			PIMAGE_THUNK_DATA ptd_in,ptd_out;
			ptd_in=(PIMAGE_THUNK_DATA) RVATOVA(pMemoryImage, pid->OriginalFirstThunk);
			ptd_out=(PIMAGE_THUNK_DATA) RVATOVA(pMemoryImage, pid->FirstThunk);
			PIMAGE_THUNK_DATA ptd_in2,ptd_out2;
			ptd_in2=(PIMAGE_THUNK_DATA) RVATOVA(pPreparedImage, pid2->OriginalFirstThunk);
			ptd_out2=(PIMAGE_THUNK_DATA) RVATOVA(pPreparedImage, pid2->FirstThunk);
			if( pid->OriginalFirstThunk == 0 ) // No OriginalFirstThunk field?
			{
				if( pid->FirstThunk == 0 ) // No FirstThunk field?  Ooops!!!
					break;

				//-------------------------------------------------------------
				// Yes! Gotta have a non-zero FirstThunk field then.
				//-------------------------------------------------------------
				ptd_in = ptd_out;
				ptd_in2 = ptd_out2;
			}

			while(ptd_in->u1.Function!=NULL)
			{
				if(ptd_out->u1.Function != ptd_out2->u1.Function)
				{
					if(!hDll)
					{
						char *svDllName;
						svDllName=(char *) RVATOVA(pMemoryImage,pid->Name);

#ifdef UNICODE
						WCHAR szDllName[MAX_PATH];
						mymbstowcs2(szDllName, svDllName);
#else
						LPSTR szDllName = svDllName;
#endif

						//-------------------------------------------------------------
						// Map library into address space (could also use LoadDLL())
						//-------------------------------------------------------------
						pdi = m_remoteDllWalker.FindModule(szDllName);
						hDll=GetModuleHandleA(svDllName);
						if(!hDll)
							hDll = MyLoadLibraryEx(pdi->imagePath.c_str()); // never free, for performance
					}
					if(hDll && pdi)
					{
						DWORD func;

						//-------------------------------------------------------------
						// Determine if ordinal or name pointer
						//-------------------------------------------------------------
						if(ptd_in->u1.Ordinal & 0x80000000) {
							// Ordinal
							func=(DWORD)GetProcAddress(hDll,MAKEINTRESOURCEA(ptd_in->u1.Ordinal));
						} else {
							// Function name
							PIMAGE_IMPORT_BY_NAME pibn;
							pibn=(PIMAGE_IMPORT_BY_NAME) RVATOVA(pMemoryImage,ptd_in->u1.AddressOfData);
							func=(DWORD)GetProcAddress(hDll,(char *)pibn->Name);
						}

						if(func)
						{
							// make sure func is NOT a forward function
							if(pdi->TestAddress(func))
								*(DWORD*)&ptd_out->u1.Function = pdi->imageBase + (func - (DWORD)hDll);
							else
							{
								// this is a really fucked up forward function
								if(rfpdi && lfpdi && lfpdi->TestAddress(func))
								{
									// well we have known remote and local pdi
									*(DWORD*)&ptd_out->u1.Function = rfpdi->imageBase + (func - lfpdi->imageBase);
								}
								else
								{
									// find local and remote pdi
									lfpdi = m_localDllWalker.FindModuleByAddress(func);
									if(!lfpdi) m_localDllWalker.Walk(GetCurrentProcessId()); // in case new dll loaded after localWalker walking
									lfpdi = m_localDllWalker.FindModuleByAddress(func); // find again
									if(lfpdi)
									{
										rfpdi = m_remoteDllWalker.FindModule(lfpdi->imagePath.c_str(), FALSE);
										if(!rfpdi)
											rfpdi = m_remoteDllWalker.FindModule(_tcsrchr(lfpdi->imagePath.c_str(), _T('\\'))+1);
										if(rfpdi)
											*(DWORD*)&ptd_out->u1.Function = rfpdi->imageBase + (func - lfpdi->imageBase);
									}
								}
								
							}
						}
					}
				}

				ptd_in++;
				ptd_out++;

				ptd_in2++;
				ptd_out2++;
			}

			pdi = NULL;
			lfpdi = NULL;
			rfpdi = NULL;
			hDll = NULL;
			pid++;
			pid2++;
		}
	}


	//-------------------------------------------------------------
	// Process relocation tables if necessary
	//-------------------------------------------------------------


	//-------------------------------------------------------------
	// Calculate fixup delta
	//-------------------------------------------------------------
	DWORD delta;
	delta=remoteImageBase - (DWORD)poh->ImageBase;

	if((delta!=0) && (poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size!=0))
	{
		PIMAGE_FIXUP_BLOCK pfb;
		pfb=(PIMAGE_FIXUP_BLOCK)RVATOVA(pMemoryImage,poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);

		PIMAGE_FIXUP_BLOCK pfb2;
		pfb2=(PIMAGE_FIXUP_BLOCK)RVATOVA(pPreparedImage,poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);

		//-------------------------------------------------------------
		// For each fixup block
		//-------------------------------------------------------------
		while(pfb->dwPageRVA!=0)
		{
			PIMAGE_FIXUP_ENTRY pfe;
			int i,count;

			count=(pfb->dwBlockSize-sizeof(IMAGE_FIXUP_BLOCK))/sizeof(IMAGE_FIXUP_ENTRY);
			pfe=(PIMAGE_FIXUP_ENTRY)((char *)pfb + sizeof(IMAGE_FIXUP_BLOCK));

			PIMAGE_FIXUP_ENTRY pfe2;
			pfe2=(PIMAGE_FIXUP_ENTRY)((char *)pfb2 + sizeof(IMAGE_FIXUP_BLOCK));

			//-------------------------------------------------------------
			// For each fixup entry, fill with checking image data
			//-------------------------------------------------------------
			for(i=0;i<count;i++)
			{
				void *fixaddr;

				fixaddr=RVATOVA(pMemoryImage,pfb->dwPageRVA + pfe->offset);
				void *fixaddr2;
				fixaddr2=RVATOVA(pPreparedImage,pfb2->dwPageRVA + pfe2->offset);
				switch(pfe->type)
				{
				case IMAGE_REL_BASED_ABSOLUTE:
					break;
				case IMAGE_REL_BASED_HIGH:
				case IMAGE_REL_BASED_LOW:
					*((WORD *)fixaddr) = *((WORD *)fixaddr2);
					break;
				case IMAGE_REL_BASED_HIGHLOW:
					// bullshit, d2common doesnt fix all rebase addresses, i dont know why
					if( *((DWORD *)fixaddr) != *((DWORD *)fixaddr2))
						*((DWORD *)fixaddr) += delta;
					break;
				case IMAGE_REL_BASED_HIGHADJ: // This one's really fucked up.
					*((WORD *)fixaddr) = *((WORD *)fixaddr2);
					pfe++;
					pfe2++;
					break;
				default:
					return FALSE;
				}

				pfe++;
				pfe2++;
			}

			pfb=(PIMAGE_FIXUP_BLOCK)((char *)pfb + pfb->dwBlockSize);
			pfb2=(PIMAGE_FIXUP_BLOCK)((char *)pfb2 + pfb2->dwBlockSize);
		}
	}
	else
	{
		//------------------------------------------------------------------------------
		// if image base has changed but we cann't find the relocation table,
		// we failed to fixup this image,
		// which usually happens when this image is a release-version executable file.
		//------------------------------------------------------------------------------
		if( delta !=0 && poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size == 0 )
			return FALSE;
	}

	return TRUE;
}

BOOL CIntegrityWalker::LoadLocalCleanImage(LPCTSTR lpFileName, LPBYTE pLocalImageBase, LPBYTE pPreparedImage, DWORD remoteImageBase)
{
	//-------------------------------------------------
	// Open File
	//-------------------------------------------------
	HANDLE hfile=CreateFile(lpFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if(hfile==INVALID_HANDLE_VALUE) return NULL;

	//-------------------------------------------------
	// Create a file mapping
	//-------------------------------------------------
	AutoHandle<HANDLE> hmapping=CreateFileMapping(hfile, NULL, PAGE_READONLY, 0, 0, NULL);

	//-------------------------------------------------
	// Close file handle since we don't need it anymore
	//-------------------------------------------------
	CloseHandle(hfile);


	//-------------------------------------------------
	// Map file mapping object to memory image
	//-------------------------------------------------
	void *baseaddr=MapViewOfFile(hmapping,FILE_MAP_READ,0,0,0);
	if(baseaddr==NULL) {
		return FALSE;
	}

	
	// PrepareDLLImage recovery all data in code section with clean code
	// FixupDLLImage fixes imported address table and rebase data with checking image data
	if(!PrepareDLLImage(pLocalImageBase, baseaddr, pPreparedImage) || !FixupDLLImage(pLocalImageBase, pPreparedImage, remoteImageBase))
	{
		return FALSE;
	}

	UnmapViewOfFile(baseaddr);
	return TRUE;
}

// as fast as possible
DWORD _declspec(naked) __fastcall mymemcmpd(DWORD nSize, void* pleft, void* pright)
{
	__asm
	{
		push esi;
		push edi;
		shr ecx, 2;
		mov eax, edx;
		mov esi, edx; // pleft
		mov edi, [esp+0x0c]; // pright
		rep cmpsd;
		sub eax, esi;
		neg eax;
		pop edi;
		pop esi;
		ret 4;
	}
}

DWORD _declspec(naked) __fastcall mymemcmpb(DWORD nSize, LPBYTE pleft, LPBYTE pright)
{
	__asm
	{
		push esi;
		push edi;
		mov eax, edx;
		mov esi, edx;
		mov edi, [esp+0x0c];
		rep cmpsb;
		test ecx, ecx;
		jz notfound;
		sub eax, esi;
		not eax;
		pop edi;
		pop esi;
		ret 4;
notfound:
		xor eax, eax;
		pop edi;
		pop esi;
		ret 4;
	}
}

inline
int CIntegrityWalker::GetDiffLength(LPBYTE pleft, LPBYTE pright, int maxlen)
{
	int len = 0;
	while(maxlen-- && *pleft++ != *pright++) ++len;
	return len;
}

BOOL CIntegrityWalker::IsImageClean(LPBYTE pLocalImage, LPBYTE pPreparedImage)
{
	PIMAGE_OPTIONAL_HEADER poh;
	poh=(PIMAGE_OPTIONAL_HEADER) OPTHDROFFSET(pLocalImage);
	return mymemcmpd(poh->SizeOfImage, pPreparedImage, pLocalImage) >= poh->SizeOfImage;
}

BOOL CIntegrityWalker::IntegrityCheck(DWORD processId, BOOL fGameDllsOnly, BOOL& fClean)
{
	fClean = TRUE;
	AutoHandle<HANDLE> hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	if(hProcess == NULL ) return FALSE;
	if(!m_remoteDllWalker.Walk(processId)) return FALSE;
	
	
	DllRefList checkingDlls;
	if(fGameDllsOnly)
		PrepareSpecificDllReferences(checkingDlls, checkingDllsName, COUNTOF(checkingDllsName));
	else
		PrepareAllDllReferences(checkingDlls);
	
	DWORD maxImageSize = FindMaxImageSize(checkingDlls);
	AutoHandle<LPBYTE> pImageBase = (LPBYTE)VirtualAlloc(NULL, maxImageSize, MEM_COMMIT, PAGE_READWRITE);
	if(pImageBase == NULL) return FALSE;
	AutoHandle<LPBYTE> pLocalImageBase = (LPBYTE)VirtualAlloc(NULL, maxImageSize, MEM_COMMIT, PAGE_READWRITE);
	if(pLocalImageBase == NULL) return FALSE;
	
	DWORD cbSize;
	DllRefList::iterator cit = checkingDlls.begin();
	DllRefList::iterator end = checkingDlls.end();
	for(; cit != end; ++cit)
	{
		DllInfo& di = *(*cit).second;
		if(ReadProcessMemory(hProcess, (LPBYTE)di.imageBase, pImageBase, di.imageSize, &cbSize))
		{
			IntegrityCheck(di, pLocalImageBase, pImageBase, fClean);
			if(!fClean)	return TRUE;
		}
	}
	
	return TRUE;
}

BOOL CIntegrityWalker::IntegrityCheck(DllInfo& currCheckingDll, LPBYTE pLocalImageBase, LPBYTE pPreparedImageBase, BOOL& fClean)
{
	if(!LoadLocalCleanImage(currCheckingDll.imagePath.c_str(), pLocalImageBase, pPreparedImageBase, currCheckingDll.imageBase))
		return FALSE;
	
	fClean = IsImageClean(pLocalImageBase, pPreparedImageBase);
	return TRUE;
}

BOOL CIntegrityWalker::IntegrityCheck(DWORD processId, LPCTSTR dllName, BOOL& fClean)
{
	AutoHandle<HANDLE> hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	if(hProcess == NULL ) return FALSE;
	CModuleWalker dllWalker;
	if(!dllWalker.Walk(processId)) return FALSE;
	CModuleWalker::DllInfo* pdi = dllWalker.FindModule(dllName);
	if(!pdi) return FALSE;

	AutoHandle<LPBYTE> pPreparedImageBase = (LPBYTE)VirtualAlloc(NULL, pdi->imageSize, MEM_COMMIT, PAGE_READWRITE);
	if(pPreparedImageBase == NULL) return FALSE;
	ReadProcessMemory(hProcess, (LPVOID)pdi->imageBase, (LPBYTE)pPreparedImageBase, pdi->imageSize, NULL);
	AutoHandle<LPBYTE> pLocalImageBase = (LPBYTE)VirtualAlloc(NULL, pdi->imageSize, MEM_COMMIT, PAGE_READWRITE);
	if(pLocalImageBase == NULL) return FALSE;

	if(!LoadLocalCleanImage(pdi->imagePath.c_str(), pLocalImageBase, pPreparedImageBase, pdi->imageBase))
		return FALSE;

	fClean = IsImageClean(pLocalImageBase, pPreparedImageBase);
	return TRUE;
}

void CIntegrityWalker::FormatDispString_CALL(CModuleWalker& dllWalker, DWORD realCallerAddress, LPBYTE caller, tstring& strDisp)
{
	TCHAR szText[512];
	DWORD callee = realCallerAddress + *(int*)(caller+1) + 5;
	CModuleWalker::DllInfo* pdi = dllWalker.FindModuleByAddress(callee);
	_stprintf(szText, _T("call %08X (in %s)"), callee, pdi ? GetFileNameFromPath(pdi->imagePath.c_str()) : _T("Unknown Module") );
	strDisp = szText;
}

void CIntegrityWalker::FormatDispString_JMP(CModuleWalker& dllWalker, DWORD realCallerAddress, LPBYTE caller, tstring& strDisp)
{
	TCHAR szText[512];
	DWORD callee = realCallerAddress + *(int*)(caller+1) + 5;
	CModuleWalker::DllInfo* pdi = dllWalker.FindModuleByAddress(callee);
	_stprintf(szText, _T("jmp %08X (in %s)"), callee, pdi ? GetFileNameFromPath(pdi->imagePath.c_str()) : _T("Unknown Module") );
	strDisp = szText;
}

void CIntegrityWalker::FormatDispString_JNZ(CModuleWalker& dllWalker, DWORD realCallerAddress, LPBYTE caller, tstring& strDisp)
{
	TCHAR szText[512];
	DWORD callee = realCallerAddress + *(int*)(caller+1) + 5;
	CModuleWalker::DllInfo* pdi = dllWalker.FindModuleByAddress(callee);
	_stprintf(szText, _T("jnz %08X (in %s)"), callee, pdi ? GetFileNameFromPath(pdi->imagePath.c_str()) : _T("Unknown Module") );
	strDisp = szText;
}

void CIntegrityWalker::FormatDispString_JMPR(CModuleWalker& dllWalker, DWORD realCallerAddress, LPBYTE caller, tstring& strDisp)
{
	TCHAR szText[512];
	char offset = *(char*)(caller+1);
	DWORD callee = realCallerAddress + offset + 2;
	_stprintf(szText, _T("jmp %c0x%02x (%08X)"), offset > 0 ? _T('+') : _T('-'), offset > 0 ? offset : -offset, callee );
	strDisp = szText;
}

void CIntegrityWalker::FormatDefaultDispString(LPBYTE ps, int len, tstring& strDisp)
{
	TCHAR szText[512];
	LPTSTR p = szText;
	int c = len > 8 ? 8 : len;
	for(int i = 0; i < c; i++)
	{
		p = p + _stprintf(p, _T("%02x "), ps[i]);
	}
	if(p > szText) *(p-1) = _T('\0');
	strDisp = szText;
	if(len > 8)
		strDisp += _T(" ...");
}

BOOL CIntegrityWalker::FormatDispString(DWORD realCallerAddress, DWORD lastRealCallerAddress, LPBYTE caller, tstring& strDisp, int& instlen)
{
	for(int i = 0; i < COUNTOF(aInstInterpreters); i++)
	{
		if(*caller == aInstInterpreters[i].inst && realCallerAddress != lastRealCallerAddress)
		{
			instlen = aInstInterpreters[i].instlen;
			aInstInterpreters[i].fnFormatDispString(m_remoteDllWalker, realCallerAddress, caller, strDisp);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CIntegrityWalker::FormatDispStringInRange(DWORD realCallerAddress, DWORD lastRealCallerAddress, LPBYTE caller, tstring& strDisp, int& instlen, int& diff)
{
	static int offs[] = {-1, -2, 0, 1};
	for(int n = 0; n < COUNTOF(offs); n++)
	{
		if(FormatDispString(realCallerAddress+offs[n], lastRealCallerAddress, caller+offs[n], strDisp, instlen))
		{
			diff = offs[n];
			return TRUE;
		}
	}
	return FALSE;
}

DWORD CIntegrityWalker::ReportDiff(DllInfo& currCheckingDll, LPBYTE pleft, LPBYTE pright, int len, DWORD offset, DWORD& lastoffset, DelModList& delMods)
{
	int diff, instlen;
	tstring str;
	DWORD outoffset = offset+1;
	if(!FormatDispStringInRange(currCheckingDll.imageBase+offset, currCheckingDll.imageBase+lastoffset, pright, str, instlen, diff))
	{
		diff = 0;
		instlen = len;
		FormatDefaultDispString(pright, len, str);
	}
	lastoffset = offset;

	if(diff > 0)
	{
		len = diff;
		FormatDefaultDispString(pright, len, str);
	}
	else
	{
		offset += diff;
		len = instlen;
	}

	if(offset+len > outoffset ) outoffset = offset+len;

	ModInfo& mi = m_mods[currCheckingDll.imageBase+offset];
	if(mi.address == currCheckingDll.imageBase+offset
		&& len == mi.len
		&& memcmp(&mi.abModified[0], pright, len) == 0)
	{
		delMods.erase(currCheckingDll.imageBase+offset);
		return outoffset;
	}

	mi.address = currCheckingDll.imageBase+offset;
	mi.len = len;
	mi.pdi = &currCheckingDll;
	mi.offset = offset;
	mi.abOriginal.assign(pleft, pleft+len);
	mi.abModified.assign(pright, pright+len);
	mi.strModified = str;
	if(!FormatDispString(currCheckingDll.imageBase+offset, currCheckingDll.imageBase+lastoffset, pleft, mi.strOriginal, instlen))
	{
		FormatDefaultDispString(pleft, len, mi.strOriginal);
	}
	return outoffset;
}

void CIntegrityWalker::CompareImage(DllInfo& currCheckingDll, LPBYTE pLocalImage, LPBYTE pPreparedImage, DelModList& delMods)
{
	PIMAGE_OPTIONAL_HEADER poh;
	poh=(PIMAGE_OPTIONAL_HEADER) OPTHDROFFSET(pLocalImage);
	DWORD offset = 0;
	DWORD lastoffset = -1;
	while( (offset+=mymemcmpd(poh->SizeOfImage-offset, pPreparedImage+offset, pLocalImage+offset)) < poh->SizeOfImage )
	{
		// now offset is DWORD aligned
		offset -= 4;

		DWORD n = 4;
		do {
			offset += mymemcmpb(n, pLocalImage+offset, pPreparedImage+offset);
			// now offset is BYTE aligned

			if(pLocalImage[offset] == pPreparedImage[offset])
			{
				// no difference, break do...while
				offset += n;
				break;
			}

			// for each instruction
			int len;
			while( (len = GetDiffLength(pLocalImage+offset, pPreparedImage+offset, poh->SizeOfImage/4-offset)) != 0)
			{
				offset = ReportDiff(currCheckingDll, pLocalImage+offset, pPreparedImage+offset, len, offset, lastoffset, delMods);
			}
			// is offset DWORD aligned?
			n = 4-((++offset)&3);
		}while(n != 4); // offset is DWORD aligned out of do...while loop

		// now offset is DWORD aligned
	}
}
