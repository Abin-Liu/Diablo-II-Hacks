//////////////////////////////////////////////////////////////////////
// D2PlayerStats.h: interface for the CD2PlayerStats class.
//
// This class is used for querying player's stats.
//
// Written by Abin (abinn32@yahoo.com)
// 8/06/2004
//////////////////////////////////////////////////////////////////////

#ifndef __D2PLAYERSTATS_H__
#define __D2PLAYERSTATS_H__

#include <windows.h>

//////////////////////////////////////////////////////////////////////
// The D2PLAYERSTATS Struct Definition
//////////////////////////////////////////////////////////////////////
typedef struct tagD2PlayerStats
{ 
      DWORD dwStatsID;	// Stats ID
      DWORD dwValue;	// Stats Value
} D2PLAYERSTATS, *LPD2PLAYERSTATS;
typedef const tagD2PlayerStats* LPCD2PLAYERSTATS;


//////////////////////////////////////////////////////////////////////
// Stats ID Definition (Partial)
//////////////////////////////////////////////////////////////////////
#define D2PS_STRENGTH				0
#define	D2PS_ENERGY					1
#define	D2PS_DESTERITY				2
#define	D2PS_VITALITY				3
#define	D2PS_HP						6
#define	D2PS_MAXHP					7
#define	D2PS_MANA					8
#define	D2PS_MAXMANA				9
#define	D2PS_STAMINA				10
#define	D2PS_MAXSTAMINA				11
#define	D2PS_LEVEL					12
#define	D2PS_GOLD					14
#define	D2PS_GOLDBANK				15
#define	D2PS_TOBLOCK				20
#define	D2PS_DAMAGEREDUCTION		34	
#define	D2PS_MAGICDAMAGEREDUCTION	35
#define	D2PS_DAMAGERESIST			36
#define	D2PS_MAGICRESIST			37
#define	D2PS_MAXMAGICRESIST			38
#define	D2PS_FIRERESIST				39
#define	D2PS_MAXFIRERESIST			40
#define	D2PS_LIGHTNINGRESIST		41
#define	D2PS_MAXLIGHTNINGRESIST		42
#define	D2PS_COLDRESIST				43
#define	D2PS_MAXCOLDRESIST			44
#define	D2PS_POISONRESIST			45
#define	D2PS_MAXPOISONRESIST		46
#define	D2PS_VELOCITYPERCENT		67
#define D2PS_AMMOQUANTITY			70
#define D2PS_DURABILITY				72
#define D2PS_MAXDURABILITY			73
#define	D2PS_MAGICFIND				80
#define	D2PS_IAS					93
#define	D2PS_FASTERRUNWALK			96
#define	D2PS_FASTERHITRECOVERY		99
#define	D2PS_FASTERBLOCK			102
#define	D2PS_FASTERCAST				105
#define	D2PS_CRUSHINGBLOW			136
#define	D2PS_DEADLYSTRIKE			141
#define	D2PS_FIREABSORBPERCENT		142
#define	D2PS_FIREABSORB				143
#define	D2PS_LIGHTNINGABSORBPERCENT	144
#define	D2PS_LIGHTNINGABSORB		145
#define	D2PS_COLDABSORBPERCENT		148
#define	D2PS_COLDABSORB				149


//////////////////////////////////////////////////////////////////////
// The CD2PlayerStats Class Definition
//////////////////////////////////////////////////////////////////////
class CD2PlayerStats
{
public:	
		
	CD2PlayerStats();
	virtual ~CD2PlayerStats();

	BOOL SetD2Wnd(HWND hWnd);	// Set D2 HWND
	BOOL HasStats(DWORD dwStatsID);
	DWORD GetStats(DWORD dwStatsID);	// Stats retrieval

protected:

	BOOL ReadMemStats();		// Read memory
	BOOL UpdateStats();			// Update stats
	void ReCreateStats(int nCount);	// Reallocate stats array

	LPD2PLAYERSTATS m_pStats;	// Stats array
	int m_nCount;				// Stats count
	HWND m_hWnd;				// D2 HWND
	HANDLE m_hProcess;			// D2 Process handle
	DWORD m_dwLastUpdate;		// Last update time
	BOOL m_bLastResult;			// Last memory reading result
};

#endif // __D2PLAYERSTATS_H__
