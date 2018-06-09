////////////////////////////////////////////////////////////////////
// AuraDef.h
//
// Defined aura types.
//
// Written by Abin (abinn32@yahoo.com)
////////////////////////////////////////////////////////////////////

#ifndef __AURADEF_H__
#define __AURADEF_H__

#include <windows.h>

////////////////////////////////////////////////////////////////////
// Retrieves Aura Name
////////////////////////////////////////////////////////////////////
DWORD D2GetAuraName(BYTE iAuraID, LPTSTR lpszBuffer, DWORD dwMaxChars);


////////////////////////////////////////////////////////////////////
// Aura Type Definition
////////////////////////////////////////////////////////////////////
#define AURA_NONE				0x00 // none
#define AURA_HOLYFREEZE			0x2b // holy freeze
#define AURA_BLESSEDAIM			0x28 // blessed aim
#define AURA_MIGHT				0x21 // might
#define AURA_HOLYFIRE			0x23 // holy fire
#define AURA_HOLYSHOCK			0x2e // holy shock
#define AURA_CONVICTION			0x1c // conviction
#define AURA_FANATICISM			0x31 // fanaticism
#define AURA_SALVATION			0x08 // Salvation
#define AURA_REDEMPTION			0x32 // Redemption
#define AURA_MEDITATION			0x30 // Meditation
#define AURA_VIGOR				0x29 // Vigor
#define AURA_RESISTLIGHTNING	0x05 // Resist lightning
#define AURA_CLEANSING			0x2d // Cleansing
#define AURA_RESISTCOLD			0x04 // Resist cold
#define AURA_DEFIANCE			0x25 // Defiance
#define AURA_RESISTFIRE			0x03 // Resist fire
#define AURA_PRAYER				0x22 // Prayer
#define AURA_SANCTUARY			0x2f // Sanctuary
#define AURA_CONCENTRATION		0x2a // Concentration
#define AURA_THORNS				0x24 // Thorns

#endif // __AURADEF_H__