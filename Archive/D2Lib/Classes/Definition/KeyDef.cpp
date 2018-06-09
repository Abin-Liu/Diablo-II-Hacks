#include "KeyDef.h"
#include <tchar.h>
#include <string.h>
#include <stdio.h>

DWORD D2GetKeyName(BYTE iVKCode, LPTSTR lpszBuffer, DWORD dwMaxChars)
{
	if (lpszBuffer == NULL)
		return 0;

	lpszBuffer[0] = _T('\0');
	if (dwMaxChars == 0 || iVKCode > 0x80)
		return 0;

	::memset(lpszBuffer, 0, dwMaxChars * sizeof(TCHAR));

	switch (iVKCode)
	{
	case 0x08:

		_tcsncpy(lpszBuffer, _T("BACK SPACE"), dwMaxChars);
		break;
		
	case 0x09:
	
		_tcsncpy(lpszBuffer, _T("TAB"), dwMaxChars);
		break;

	case 0x0c:
	
		_tcsncpy(lpszBuffer, _T("NUMPAD 5"), dwMaxChars);
		break;
		
	case 0x0d:
	
		_tcsncpy(lpszBuffer, _T("RETURN"), dwMaxChars);
		break;

	case 0x10:
	
		_tcsncpy(lpszBuffer, _T("SHIFT"), dwMaxChars);
		break;

	case 0x11:
	
		_tcsncpy(lpszBuffer, _T("CTRL"), dwMaxChars);
		break;

	case 0x12:
	
		_tcsncpy(lpszBuffer, _T("ALT"), dwMaxChars);
		break;

	case 0x13:
	
		_tcsncpy(lpszBuffer, _T("PAUSE"), dwMaxChars);
		break;

	case 0x14:
	
		_tcsncpy(lpszBuffer, _T("CAPS LOCK"), dwMaxChars);
		break;

	case 0x1b:
	
		_tcsncpy(lpszBuffer, _T("ESC"), dwMaxChars);
		break;

	case 0x20:
	
		_tcsncpy(lpszBuffer, _T("SPACE"), dwMaxChars);
		break;

	case 0x21:
	
		_tcsncpy(lpszBuffer, _T("PAGE UP"), dwMaxChars);
		break;
		
	case 0x22:
	
		_tcsncpy(lpszBuffer, _T("PAGE DOWN"), dwMaxChars);
		break;

	case 0x23:
	
		_tcsncpy(lpszBuffer, _T("END"), dwMaxChars);
		break;
	case 0x24:
	
		_tcsncpy(lpszBuffer, _T("HOME"), dwMaxChars);
		break;

	case 0x25:
	
		_tcsncpy(lpszBuffer, _T("LEFT"), dwMaxChars);
		break;

	case 0x26:
	
		_tcsncpy(lpszBuffer, _T("UP"), dwMaxChars);
		break;

	case 0x27:
	
		_tcsncpy(lpszBuffer, _T("RIGHT"), dwMaxChars);
		break;

	case 0x28:
	
		_tcsncpy(lpszBuffer, _T("DOWN"), dwMaxChars);
		break;

	case 0x2d:
	
		_tcsncpy(lpszBuffer, _T("INSERT"), dwMaxChars);
		break;
		
	case 0x2e:
	
		_tcsncpy(lpszBuffer, _T("DELETE"), dwMaxChars);
		break;
		
	case 0x30:
	case 0x31:
	case 0x32:
	case 0x33:
	case 0x34:
	case 0x35:
	case 0x36:
	case 0x37:
	case 0x38:
	case 0x39:
	
		_stprintf(lpszBuffer, _T("%d"), iVKCode - 0x30);
		break;

	case 0x41: // a
	case 0x42: // b
	case 0x43: // c
	case 0x44: // d
	case 0x45: // e
	case 0x46: // f
	case 0x47: // g
	case 0x48: // h
	case 0x49: // i
	case 0x4a: // j
	case 0x4b: // k
	case 0x4c: // l
	case 0x4d: // m
	case 0x4e: // n
	case 0x4f: // o
	case 0x50: // p
	case 0x51: // q
	case 0x52: // r
	case 0x53: // s
	case 0x54: // t
	case 0x55: // u
	case 0x56: // v
	case 0x57: // w
	case 0x58: // x
	case 0x59: // y
	case 0x5a: // z
	
		_stprintf(lpszBuffer, _T("%C"), iVKCode - 0x41 + _T('A'));
		break;

	case 0x60:
	case 0x61:
	case 0x62:
	case 0x63:
	case 0x64:
	case 0x65:
	case 0x66:
	case 0x67:
	case 0x68:
	case 0x69:
	
		_stprintf(lpszBuffer, _T("NUMPAD %d"), iVKCode - 0x60);
		break;

	case 0x6a:
	
		_tcsncpy(lpszBuffer, _T("NUMPAD *"), dwMaxChars);
		break;

	case 0x6b:
	
		_tcsncpy(lpszBuffer, _T("NUMPAD +"), dwMaxChars);
		break;
	case 0x6d:
	
		_tcsncpy(lpszBuffer, _T("NUMPAD -"), dwMaxChars);
		break;

	case 0x6e:
	
		_tcsncpy(lpszBuffer, _T("NUMPAD ."), dwMaxChars);
		break;

	case 0x6f:
	
		_tcsncpy(lpszBuffer, _T("NUMPAD /"), dwMaxChars);
		break;

	case VK_F1:
	case VK_F2:
	case VK_F3:
	case VK_F4:
	case VK_F5:
	case VK_F6:
	case VK_F7:
	case VK_F8:
	case VK_F9:
	case VK_F10:
	case VK_F11:
	case VK_F12:
	
		_stprintf(lpszBuffer, _T("F%d"), iVKCode - VK_F1 + 1);
		break;

	case 0x90:
	
		_tcsncpy(lpszBuffer, _T("NUM LOCK"), dwMaxChars);
		break;
	case 0x91:
	
		_tcsncpy(lpszBuffer, _T("SCROLL LOCK"), dwMaxChars);
		break;

	case 0xba:
	
		_tcsncpy(lpszBuffer, _T(";"), dwMaxChars);
		break;

	case 0xbb:
	
		_tcsncpy(lpszBuffer, _T("="), dwMaxChars);
		break;

	case 0xbc:
	
		_tcsncpy(lpszBuffer, _T(","), dwMaxChars);
		break;

	case 0xbd:
	
		_tcsncpy(lpszBuffer, _T("-"), dwMaxChars);
		break;
	case 0xbe:
	
		_tcsncpy(lpszBuffer, _T("."), dwMaxChars);
		break;

	case 0xbf:
	
		_tcsncpy(lpszBuffer, _T("/"), dwMaxChars);
		break;
	case 0xc0:
	
		_tcsncpy(lpszBuffer, _T("`"), dwMaxChars);
		break;

	case 0xdb:
	
		_tcsncpy(lpszBuffer, _T("["), dwMaxChars);
		break;
	case 0xdc:
	
		_tcsncpy(lpszBuffer, _T("\\"), dwMaxChars);
		break;

	case 0xdd:
	
		_tcsncpy(lpszBuffer, _T("]"), dwMaxChars);
		break;
	case 0xde:
	
		_tcsncpy(lpszBuffer, _T("'"), dwMaxChars);
		break;

	default:
		return 0;
		break;	
	}

	return _tcslen(lpszBuffer);
}