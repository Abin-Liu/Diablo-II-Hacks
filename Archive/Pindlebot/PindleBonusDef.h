#ifndef __PINDLEBONUSDEF_H__
#define __PINDLEBONUSDEF_H__

#include <windows.h>

///////////////////////////////////////////////////////////////////////
// Pindleskin Bonus Definition
// Auras are OR'ed by 0x8000 to Avoid Conflicts with Other Bonuses
///////////////////////////////////////////////////////////////////////
#define PINDLE_MAX_BONUS					2
#define PINDLE_BONUS_NONE					0x0000 // none
#define PINDLE_BONUS_AURA_ENCHANTED			0x00f0 // aura enchanted
#define PINDLE_BONUS_COLD_ENCHANTED			0x0090 // cold enchanted
#define PINDLE_BONUS_CURSED					0x0038 // cursed
#define PINDLE_BONUS_EXTRA_FAST				0x0030 // extra fast
#define PINDLE_BONUS_EXTRA_STRONG			0x0028 // extra strong
#define PINDLE_BONUS_LIGHTNING_ENCHANTED	0x0088 // lightning enchanted
#define PINDLE_BONUS_FIRE_ENCHANTED			0x00b0 // fire enchanted
#define PINDLE_BONUS_MAGIC_RESISTANT		0x0040 // magic resistant
#define PINDLE_BONUS_MANA_BURN				0x00c8 // mana burn
#define PINDLE_BONUS_MULTISHOT				0x00e8 // multishot
#define PINDLE_BONUS_SPECTRAL_HIT			0x00d8 // spectral hit
#define PINDLE_BONUS_STONE_SKIN				0x00e0 // stone skin
#define PINDLE_BONUS_TELEPORT				0x00d0 // teleport

// Auras
#define PINDLE_AURA_HOLYFREEZE				0x802b // holy freeze
#define PINDLE_AURA_BLESSEDAIM				0x8028 // blessed aim
#define PINDLE_AURA_MIGHT					0x8021 // might
#define PINDLE_AURA_HOLYFIRE				0x8023 // holy fire
#define PINDLE_AURA_HOLYSHOCK				0x802e // holy shock
#define PINDLE_AURA_CONVICTION				0x801c // conviction
#define PINDLE_AURA_FANATICISM				0x8031 // fanaticism

DWORD PBotGetBonusName(WORD wBonus, LPTSTR lpszBuffer, DWORD dwMaxChars, BOOL bChinese);

#endif // __PINDLEBONUSDEF_H__