///////////////////////////////////////////////////////////
// ItemCheck.cpp
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////

#include "ItemCheck.h"
#include "Constants.h"

///////////////////////////////////////////////////////////
// Quest Item Table (Abin)
///////////////////////////////////////////////////////////
static const LPCSTR QUESTITEMS[] = {
	
	// Act 1 Items
	"bks", // Scroll of Inifuss (pre-translated)
	"bkd", // Scroll of Inifuss (post-translated)
	"hdm", // Horadric Malus
	"leg", // Wert's Leg
	
	// Act 2 Items
	"ass", // Book of Skill
	"box", // Horadic cube
	"tr1", // Horadric Scroll
	"vip", // Viper Amulet
	"msf", // Shaft of Kings
	"hst", // Horadric Staff

	// Act 3 Items
	"j34", // A Jade Figurine
	"g34", // The Golden Bird
	"xyz", // Potion of Life
	"g33", // The Gidbinn
	"qey", // Khalim's Eye
	"qhr", // Khalim's Heart
	"qbr", // Khalim's Brain
	"qf1", // Khalim's Flail
	"qf2", // Khalim's Will
	"bbb", // Lam Esen's Tome
	"mss", // Mephisto's Soulstone

	// Act 4 Items
	"hfh", // Hell Forge Hammer

	// Act 5 Items
	"ice", // Malah's Potion
	"tr2", // Scroll of Resistance

	NULL  // End of story
};

DWORD D2ComposeItemInfo(WPARAM& wParam, LPARAM& lParam, DWORD dwItemID, BYTE iEvent, BYTE iType, BYTE iLevel, BYTE iQuality, WORD x, WORD y);

int D2IsPotion(LPCSTR lpszItemCode)
{
	if (lpszItemCode == NULL)
		return POTION_UNKNOWN;
	
	if (!stricmp(lpszItemCode, "rvl")
		|| !stricmp(lpszItemCode, "rvs"))
		return POTION_PURPLE;

	if (!stricmp(lpszItemCode, "hp1")
		|| !stricmp(lpszItemCode, "hp2")
		|| !stricmp(lpszItemCode, "hp3")
		|| !stricmp(lpszItemCode, "hp4")
		|| !stricmp(lpszItemCode, "hp5"))
		return POTION_RED;

	if (!stricmp(lpszItemCode, "mp1")
		|| !stricmp(lpszItemCode, "mp2")
		|| !stricmp(lpszItemCode, "mp3")
		|| !stricmp(lpszItemCode, "mp4")
		|| !stricmp(lpszItemCode, "mp5"))
		return POTION_BLUE;

	if (!stricmp(lpszItemCode, "yps")
		|| !stricmp(lpszItemCode, "vps")
		|| !stricmp(lpszItemCode, "wms"))
		return POTION_JUNK;

	return POTION_UNKNOWN;
}

BOOL D2IsThrowItem(LPCSTR szItemCode)
{
	if (szItemCode == NULL)
		return FALSE;
	
	return !stricmp(szItemCode, "jav")
		|| !stricmp(szItemCode, "9ja")
		|| !stricmp(szItemCode, "7ja")

		|| !stricmp(szItemCode, "tax")
		|| !stricmp(szItemCode, "9ta")
		|| !stricmp(szItemCode, "7ta")

		|| !stricmp(szItemCode, "tkf")
		|| !stricmp(szItemCode, "9tk")
		|| !stricmp(szItemCode, "7tk")

		|| !stricmp(szItemCode, "am5")
		|| !stricmp(szItemCode, "ama")
		|| !stricmp(szItemCode, "amf")

		|| !stricmp(szItemCode, "pil")
		|| !stricmp(szItemCode, "9pi")
		|| !stricmp(szItemCode, "7pi")

		|| !stricmp(szItemCode, "bkf")
		|| !stricmp(szItemCode, "9bk")
		|| !stricmp(szItemCode, "7bk")

		|| !stricmp(szItemCode, "bal")
		|| !stricmp(szItemCode, "9b8")
		|| !stricmp(szItemCode, "7b8")

		|| !stricmp(szItemCode, "glv")
		|| !stricmp(szItemCode, "9gl")
		|| !stricmp(szItemCode, "7gl")

		|| !stricmp(szItemCode, "tsp")
		|| !stricmp(szItemCode, "9ts")
		|| !stricmp(szItemCode, "7ts");
}

BOOL D2IsBow(LPCSTR szItemCode)
{
	if (szItemCode == NULL)
		return FALSE;

	return !stricmp(szItemCode, "sbw")
		|| !stricmp(szItemCode, "hbw")
		|| !stricmp(szItemCode, "lbw")
		|| !stricmp(szItemCode, "cbw")
		|| !stricmp(szItemCode, "sbb")
		|| !stricmp(szItemCode, "lbb")
		|| !stricmp(szItemCode, "swb")
		|| !stricmp(szItemCode, "lwb")

		|| !stricmp(szItemCode, "8sb")
		|| !stricmp(szItemCode, "8hb")
		|| !stricmp(szItemCode, "8lb")
		|| !stricmp(szItemCode, "8cb")
		|| !stricmp(szItemCode, "8s8")
		|| !stricmp(szItemCode, "8l8")
		|| !stricmp(szItemCode, "8sw")
		|| !stricmp(szItemCode, "8lw")

		|| !stricmp(szItemCode, "6sb")
		|| !stricmp(szItemCode, "6hb")
		|| !stricmp(szItemCode, "6lb")
		|| !stricmp(szItemCode, "6cb")
		|| !stricmp(szItemCode, "6s7")
		|| !stricmp(szItemCode, "6l7")
		|| !stricmp(szItemCode, "6sw")
		|| !stricmp(szItemCode, "6lw");
}

BOOL D2IsCrossBow(LPCSTR szItemCode)
{
	if (szItemCode == NULL)
		return FALSE;

	return !stricmp(szItemCode, "lxb")
		|| !stricmp(szItemCode, "mxb")
		|| !stricmp(szItemCode, "hxb")
		|| !stricmp(szItemCode, "rxb")

		|| !stricmp(szItemCode, "8lx")
		|| !stricmp(szItemCode, "8mx")
		|| !stricmp(szItemCode, "8hx")
		|| !stricmp(szItemCode, "8rx")

		|| !stricmp(szItemCode, "6lx")
		|| !stricmp(szItemCode, "6mx")
		|| !stricmp(szItemCode, "6hx")
		|| !stricmp(szItemCode, "6rx");
}

int D2IsBelt(LPCTSTR lpszItemCode)
{
	if (lpszItemCode == NULL)
		return 0;

	if (stricmp(lpszItemCode, "lbl") == 0 // sash
		|| stricmp(lpszItemCode, "vbl") == 0) // light belt
		return 2;
	else if (stricmp(lpszItemCode, "mbl") == 0 // belt
		|| stricmp(lpszItemCode, "tbl") == 0) // heavy belt
		return 3;
	else if (stricmp(lpszItemCode, "hbl") == 0 // plated belt
		|| stricmp(lpszItemCode, "zlb") == 0 // demonhide sash
		|| stricmp(lpszItemCode, "zvb") == 0 // sharkskin belt
		|| stricmp(lpszItemCode, "zmb") == 0 // mesh belt
		|| stricmp(lpszItemCode, "ztb") == 0 // battle belt
		|| stricmp(lpszItemCode, "zhb") == 0 // war belt
		|| stricmp(lpszItemCode, "ulc") == 0 // Spiderweb Sash
		|| stricmp(lpszItemCode, "uvc") == 0 // Vampirefang Belt
		|| stricmp(lpszItemCode, "umc") == 0 // Mithril Coil
		|| stricmp(lpszItemCode, "utc") == 0 // Troll Belt
		|| stricmp(lpszItemCode, "uhc") == 0) // Colossus Girdle
		return 4;
	else
		return 0;
}

BOOL D2IsCirclets(LPCSTR lpszItemCode)
{
	return !stricmp(lpszItemCode, "ci0")
		|| !stricmp(lpszItemCode, "ci1")
		|| !stricmp(lpszItemCode, "ci2")
		|| !stricmp(lpszItemCode, "ci3");
}

BOOL D2IsGloves(LPCSTR lpszItemCode)
{
	return !stricmp(lpszItemCode, "lgl")
		|| !stricmp(lpszItemCode, "vgl")
		|| !stricmp(lpszItemCode, "mgl")
		|| !stricmp(lpszItemCode, "tgl")
		|| !stricmp(lpszItemCode, "hgl")

		|| !stricmp(lpszItemCode, "xlg")
		|| !stricmp(lpszItemCode, "xvg")
		|| !stricmp(lpszItemCode, "xmg")
		|| !stricmp(lpszItemCode, "xtg")
		|| !stricmp(lpszItemCode, "xhg")
		
		|| !stricmp(lpszItemCode, "ulg")
		|| !stricmp(lpszItemCode, "uvg")
		|| !stricmp(lpszItemCode, "umg")
		|| !stricmp(lpszItemCode, "utg")
		|| !stricmp(lpszItemCode, "uhg");
}

BOOL D2IsBoots(LPCSTR lpszItemCode)
{
	return !stricmp(lpszItemCode, "lbt")
		|| !stricmp(lpszItemCode, "vbt")
		|| !stricmp(lpszItemCode, "mbt")
		|| !stricmp(lpszItemCode, "tbt")
		|| !stricmp(lpszItemCode, "hbt")

		|| !stricmp(lpszItemCode, "xlb")
		|| !stricmp(lpszItemCode, "xvb")
		|| !stricmp(lpszItemCode, "xmb")
		|| !stricmp(lpszItemCode, "xtb")
		|| !stricmp(lpszItemCode, "xhb")
		
		|| !stricmp(lpszItemCode, "ulb")
		|| !stricmp(lpszItemCode, "uvb")
		|| !stricmp(lpszItemCode, "umb")
		|| !stricmp(lpszItemCode, "utb")
		|| !stricmp(lpszItemCode, "uhb");
}

BOOL D2IsRune(LPCSTR lpszItemCode)
{
	if (lpszItemCode == NULL)
		return FALSE;

	return lpszItemCode[0] == 'r' && isdigit(lpszItemCode[1]) && isdigit(lpszItemCode[2]);
}


BOOL D2IsQuestItem(LPCSTR lpszItemCode)
{
	if (lpszItemCode == NULL)
		return FALSE;

	for (int i = 0; QUESTITEMS[i]; i++)
	{
		if (!::stricmp(lpszItemCode, QUESTITEMS[i]))
			return TRUE;
	}

	return FALSE;
}

DWORD GetBitField(const BYTE* data, DWORD pos, DWORD len, DWORD max);

// from mousepad with some small mods
struct BitFields {
	const BYTE *data;
	DWORD pos;
	DWORD max;
	BitFields(const BYTE *d, DWORD mx) {
		data = d; 
		pos = 0; 
		max = (mx * 8);  // convert to max bits
	}
	DWORD GetField(DWORD len) {
		return GetBitField(data, (pos+=len)-len, len, max);
	}
};

DWORD GetBitField(const BYTE* data, DWORD pos, DWORD len, DWORD max) 
{
	// trying to read beyond the end of the packet

	if(pos + len > max) {

		// if we are already at or beyond the end of packet, return 0;
		if(pos >= max) {
			return 0;
		}
		// otherwise adjust len to all remaining bits
		//	D2Print("Trying to read beyond eop. len:%d pos:%d max:%d",len,pos,max);
		len = max - pos;
	}

	return (DWORD)(*(unsigned __int64 *)(data+pos/8)<<(64-len-(pos&7))>>(64-len));
}

DWORD D2ParseItemIDFromPacket(const BYTE* aPacket, DWORD aLen)
{
	if ((aPacket[0] != 0x9c && aPacket[0] != 0x9d) || aLen < 10)
		return 0;

	DWORD dwItemID = 0;
	::memcpy(&dwItemID, aPacket + 4, 4);
	return dwItemID;
}

BOOL D2ParseItemCodeFromPacket(const BYTE* aPacket, DWORD aLen, LPSTR lpszBuffer)
{
	if ((aPacket[0] != 0x9c && aPacket[0] != 0x9d) || aLen < 10 || lpszBuffer == NULL)
		return FALSE;

	BitFields iPacket(aPacket,aLen);	
	iPacket.GetField((aPacket[0] == 0x9d) ? 164 : 124);

	lpszBuffer[0] = (char)iPacket.GetField(8);
	lpszBuffer[1] = (char)iPacket.GetField(8);
	lpszBuffer[2] = (char)iPacket.GetField(8);
	lpszBuffer[3] = '\0';
	return strlen(lpszBuffer) == 3;
}
