//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////


#ifndef __INTEGRITY_WALKER_H__
#define __INTEGRITY_WALKER_H__

#include "ModuleWalker.h"
#include <vector>

class CIntegrityWalker  
{
	typedef CModuleWalker::DllInfo DllInfo;
	typedef CModuleWalker::DllList DllList;
	typedef std::map<DWORD, DllInfo*> DllRefList;
public:
	// Modification struct
	struct ModInfo
	{
		DWORD address;
		DllInfo* pdi; // in which dll
		DWORD offset; // where changed
		DWORD len; // length of modification
		std::vector<BYTE> abOriginal;
		std::vector<BYTE> abModified;
		tstring strOriginal;
		tstring strModified;
	};
	typedef std::map<DWORD, ModInfo> ModList;
	typedef AutoList<ModList> DelModList;
	public:
	CIntegrityWalker();
	~CIntegrityWalker();
	BOOL Walk(DWORD processId, BOOL fGameDllsOnly);
	BOOL Walk(DWORD processId, BOOL fGameDllsOnly, DelModList& delMods);
	BOOL IntegrityCheck(DWORD processId, BOOL fGameDllsOnly, BOOL& fClean);
	BOOL IntegrityCheck(DWORD processId, LPCTSTR dllName, BOOL& fClean);
private:
	void WalkDll(DllInfo& currCheckingDll, LPBYTE pLocalImageBase, LPBYTE pPreparedImageBase, DelModList& delMods);
	BOOL IntegrityCheck(DllInfo& currCheckingDll, LPBYTE pLocalImageBase, LPBYTE pPreparedImageBase, BOOL& fClean);
	void PrepareSpecificDllReferences(DllRefList& dlls, TCHAR *dllNames[], int num);
	void PrepareAllDllReferences(DllRefList& dlls);
	static DWORD FindMaxImageSize(const DllRefList& dlls);
	void CompareImage(DllInfo& currCheckingDll, LPBYTE pLocalImage, LPBYTE pPreparedImage, DelModList& delMods);
	BOOL LoadLocalCleanImage(LPCTSTR lpFileName, LPBYTE pLocalImage, LPBYTE pPreparedImage, DWORD remoteImageBase);
	BOOL PrepareDLLImage(void *pMemoryImage, void* pFileImage, void* pPreparedImage);
	BOOL FixupDLLImage(void *pMemoryImage, void* pPreparedImage, DWORD remoteImageBase);
	static BOOL IsImageClean(LPBYTE pLocalImage, LPBYTE pPreparedImage);
	static int GetDiffLength(LPBYTE pleft, LPBYTE pright, int maxlen);
	DWORD __fastcall ReportDiff(DllInfo& currCheckingDll, LPBYTE pleft, LPBYTE pright, int len, DWORD offset, DWORD& lastoffset, DelModList& delMods);
	BOOL __fastcall FormatDispString(DWORD realCallerAddress, DWORD lastRealCallerAddress, LPBYTE caller, tstring& strDisp, int& instlen);
	BOOL __fastcall FormatDispStringInRange(DWORD realCallerAddress, DWORD lastRealCallerAddress, LPBYTE caller, tstring& strDisp, int& instlen, int& diff);
	static void __fastcall FormatDefaultDispString(LPBYTE ps, int len, tstring& strDisp);
	// walker for target process
	CModuleWalker m_remoteDllWalker;
	// walker for local process, for performance
	CModuleWalker m_localDllWalker;
public:
	ModList m_mods;
public:
	static void __fastcall FormatDispString_CALL(CModuleWalker& dllWalker, DWORD realCallerAddress, LPBYTE caller, tstring& strDisp);
	static void __fastcall FormatDispString_JMP(CModuleWalker& dllWalker, DWORD realCallerAddress, LPBYTE caller, tstring& strDisp);
	static void __fastcall FormatDispString_JNZ(CModuleWalker& dllWalker, DWORD realCallerAddress, LPBYTE caller, tstring& strDisp);
	static void __fastcall FormatDispString_JMPR(CModuleWalker& dllWalker, DWORD realCallerAddress, LPBYTE caller, tstring& strDisp);
};

#endif // __INTEGRITY_WALKER_H__
