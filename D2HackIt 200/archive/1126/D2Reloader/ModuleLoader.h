//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#pragma once

class CModuleLoader
{
public:
	// Yes, we need HWND here to load module to other process under win9x/me
	// not necessary with NT family
	static HMODULE LoadModule( DWORD processId, HWND hwnd, LPCTSTR lpszModuleName, HMODULE hModule = NULL );
};
