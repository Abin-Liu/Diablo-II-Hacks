//////////////////////////////////////////////////////////////////////
//	KeyBind.h
//
// CKeyBind, a helper class used for bind.d2h. It captures in-game
// key strokes and maintains command lists those are binded to captured
// key strokes.
//
// Written by Abin (abinn32@yahoo.com)
// June 26, 2004
//
//////////////////////////////////////////////////////////////////////

#ifndef __KEYBIND_H__
#define __KEYBIND_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define BIND_ENTRY_COUNT	256

// Whenever an appropriate key-stroke is captured, this function will be called.
// iVKCode: The virtual key code of the captured key stroke
// lpszCommand: The command binded to the key code, if NULL, means the player wants to display the key code
typedef BOOL (CALLBACK *KEYBINDPROC)(BYTE iVkCode, LPCTSTR lpszCommand);

// A hotkey entry
class CKeyBindEntry
{
public:	
	
	CKeyBindEntry();
	CKeyBindEntry(BYTE iVKCode, BYTE iRetCode, LPCTSTR lpszCommands);
	CKeyBindEntry(const CKeyBindEntry& rhs);
	virtual ~CKeyBindEntry();

	const CKeyBindEntry& operator=(const CKeyBindEntry& rhs);
	BOOL operator==(const CKeyBindEntry& rhs) const;
	BOOL operator>(const CKeyBindEntry& rhs) const; // for search and sort

	void SetVKCode(BYTE iVKCode);
	BYTE GetVKCode() const;

	BYTE GetRetCode() const;
	void SetRetCode(BYTE iRetCode);

	void SetCommands(LPCTSTR lpszCommands);
	DWORD GetCommandStartPosition() const; // Get the first command
	void GetNextCommand(DWORD& dwPosition, LPTSTR lpszBuffer, DWORD dwChars) const; // Get the next command after lpszPosition

	BOOL GetDisplayStr(LPTSTR lpszBuffer) const;
	static BOOL VerifyVKCode(LPCTSTR lpsz, DWORD dwLen, BYTE* pVKCode);
	BOOL ParseLineStr(LPCTSTR lpszLineStr);
	BOOL GetLineStr(LPTSTR lpszBuffer) const;
	BOOL GetEntryNameStr(LPTSTR lpszBuffer) const;
	static BOOL GetLineStr(BYTE iRetCode, LPCTSTR lpszCommands, LPTSTR lpszBuffer);
	static BOOL GetEntryNameStr(BYTE iVKCode, LPTSTR lpszBuffer);
	BOOL IsMapped() const;

protected:
	
	void __FreeData();

protected:	
	
	BYTE m_iVKCode; // The virtual key code to capture
	BYTE m_iRetCode; // The return key code
	LPTSTR m_lpszCommands; // Command(s) binded to this key
};

// The CKeyBind class.
class CKeyBind  
{
public:		

	static BOOL InstallKeyBind(); // Must be called before use, "OnClientStart" is a good place for this
	static BOOL UninstallKeyBind(); // Should be called when no longer needed, "OnClientStop" is a good place for this
	static void SetKeyBindProc(KEYBINDPROC lpfnProc); // Set the callback function pointer

	static void Start();
	static void Stop();
	static BOOL IsStarted();
	static void OnGameJoin();
	static void OnGameLeave();

	static void SetShowCode(BOOL bSet = TRUE); // The player wants to display the virtual key code of the next key-stroke
	static CKeyBindEntry* CKeyBind::GetEntry(BYTE iIndex); // Get info of a particular entry
	static void ClearAll(); // Remove all entries from the list
	static BOOL SetEntry(BYTE iVKCode, BYTE iRetCode, LPCTSTR lpszCommand);	// Add or set an entry
	static BOOL IsTyping(); // whether the player is typing some chat message

	static void SetTargetWnd(HWND hWnd);
	static BOOL IsAllCleared();
	static BOOL Clear(BYTE iIndex);

protected:

	static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam); // The build in process function
};

#endif // __KEYBIND_H__
