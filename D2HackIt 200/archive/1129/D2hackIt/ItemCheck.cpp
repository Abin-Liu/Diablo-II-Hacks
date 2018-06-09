///////////////////////////////////////////////////////////
// ItemCheck.cpp
//
// Abin (abinn32@yahoo.com)
///////////////////////////////////////////////////////////

#include "ItemCheck.h"
#include "Constants.h"

///////////////////////////////////////////////
// Item size table
// Originally ackmed's Code(Shopbot)
///////////////////////////////////////////////
struct ITEM_SIZE
{
//	char *name;
	char *code;
	int x;
	int y;
};

static const ITEM_SIZE ITEM_SIZE_TB[]=
{
	{"hax",1,3},
	{"axe",2,3},
	{"2ax",2,3},
	{"mpi",2,3},
	{"wax",2,3},
	{"lax",2,3},
	{"bax",2,3},
	{"btx",2,3},
	{"gax",2,4},
	{"gix",2,3},
	{"wnd",1,2},
	{"ywn",1,2},
	{"bwn",1,2},
	{"gwn",1,2},
	{"clb",1,3},
	{"scp",1,3},
	{"gsc",1,3},
	{"wsp",2,3},
	{"spc",1,3},
	{"mac",1,3},
	{"mst",1,3},
	{"fla",2,3},
	{"whm",2,3},
	{"mau",2,4},
	{"gma",2,3},
	{"ssd",1,3},
	{"scm",1,3},
	{"sbr",1,3},
	{"flc",1,3},
	{"crs",2,3},
	{"bsd",2,3},
	{"lsd",2,3},
	{"wsd",1,3},
	{"2hs",1,4},
	{"clm",1,4},
	{"gis",1,4},
	{"bsw",1,4},
	{"flb",2,4},
	{"gsd",2,4},
	{"dgr",1,2},
	{"dir",1,2},
	{"kri",1,3},
	{"bld",1,3},
	{"tkf",1,2},
	{"tax",1,2},
	{"bkf",1,2},
	{"bal",2,3},
	{"jav",1,3},
	{"pil",1,3},
	{"ssp",1,3},
	{"glv",1,4},
	{"tsp",1,4},
	{"spr",2,4},
	{"tri",2,4},
	{"brn",2,4},
	{"spt",2,4},
	{"pik",2,4},
	{"bar",2,4},
	{"vou",2,4},
	{"scy",2,4},
	{"pax",2,4},
	{"hal",2,4},
	{"wsc",2,4},
	{"sst",1,3},
	{"lst",1,4},
	{"cst",1,4},
	{"bst",1,4},
	{"wst",2,4},
	{"sbw",2,3},
	{"hbw",2,3},
	{"lbw",2,4},
	{"cbw",2,3},
	{"sbb",2,3},
	{"lbb",2,4},
	{"swb",2,3},
	{"lwb",2,4},
	{"lxb",2,3},
	{"mxb",2,3},
	{"hxb",2,4},
	{"rxb",2,3},
	{"gps",1,1},
	{"ops",1,1},
	{"gpm",1,1},
	{"opm",1,1},
	{"gpl",1,1},
	{"opl",1,1},
	{"d33",1,2},
	{"g33",1,2},
	{"leg",1,3},
	{"hdm",1,2},
	{"hfh",2,3},
	{"hst",1,4},
	{"msf",1,3},
	{"9ha",1,3},
	{"9ax",2,3},
	{"92a",2,3},
	{"9mp",2,3},
	{"9wa",2,3},
	{"9la",2,3},
	{"9ba",2,3},
	{"9bt",2,3},
	{"9ga",2,4},
	{"9gi",2,3},
	{"9wn",1,2},
	{"9yw",1,2},
	{"9bw",1,2},
	{"9gw",1,2},
	{"9cl",1,3},
	{"9sc",1,3},
	{"9qs",1,3},
	{"9ws",2,3},
	{"9sp",1,3},
	{"9ma",1,3},
	{"9mt",1,3},
	{"9fl",2,3},
	{"9wh",2,3},
	{"9m9",2,4},
	{"9gm",2,3},
	{"9ss",1,3},
	{"9sm",1,3},
	{"9sb",1,3},
	{"9fc",1,3},
	{"9cr",2,3},
	{"9bs",2,3},
	{"9ls",2,3},
	{"9wd",1,3},
	{"92h",1,4},
	{"9cm",1,4},
	{"9gs",1,4},
	{"9b9",1,4},
	{"9fb",2,4},
	{"9gd",2,4},
	{"9dg",1,2},
	{"9di",1,2},
	{"9kr",1,3},
	{"9bl",1,3},
	{"9tk",1,2},
	{"9ta",1,2},
	{"9bk",1,2},
	{"9b8",2,3},
	{"9ja",1,3},
	{"9pi",1,3},
	{"9s9",1,3},
	{"9gl",1,4},
	{"9ts",1,4},
	{"9sr",2,4},
	{"9tr",2,4},
	{"9br",2,4},
	{"9st",2,4},
	{"9p9",2,4},
	{"9b7",2,4},
	{"9vo",2,4},
	{"9s8",2,4},
	{"9pa",2,4},
	{"9h9",2,4},
	{"9wc",2,4},
	{"8ss",1,3},
	{"8ls",1,4},
	{"8cs",1,4},
	{"8bs",1,4},
	{"8ws",2,4},
	{"8sb",2,3},
	{"8hb",2,3},
	{"8lb",2,4},
	{"8cb",2,3},
	{"8s8",2,3},
	{"8l8",2,4},
	{"8sw",2,3},
	{"8lw",2,4},
	{"8lx",2,3},
	{"8mx",2,3},
	{"8hx",2,4},
	{"8rx",2,3},
	{"qf1",2,3},
	{"qf2",2,3},
//	{"","",""},
	{"ktr",1,3},
	{"wrb",1,3},
	{"axf",1,3},
	{"ces",1,3},
	{"clw",1,3},
	{"btl",1,3},
	{"skr",1,3},
	{"9ar",1,3},
	{"9wb",1,3},
	{"9xf",1,3},
	{"9cs",1,3},
	{"9lw",1,3},
	{"9tw",1,3},
	{"9qr",1,3},
	{"7ar",1,3},
	{"7wb",1,3},
	{"7xf",1,3},
	{"7cs",1,3},
	{"7lw",1,3},
	{"7tw",1,3},
	{"7qr",1,3},
	{"7ha",1,3},
	{"7ax",2,3},
	{"72a",2,3},
	{"7mp",2,3},
	{"7wa",2,3},
	{"7la",2,3},
	{"7ba",2,3},
	{"7bt",2,3},
	{"7ga",2,4},
	{"7gi",2,3},
	{"7wn",1,2},
	{"7yw",1,2},
	{"7bw",1,2},
	{"7gw",1,2},
	{"7cl",1,3},
	{"7sc",1,3},
	{"7qs",1,3},
	{"7ws",2,3},
	{"7sp",1,3},
	{"7ma",1,3},
	{"7mt",1,3},
	{"7fl",2,3},
	{"7wh",2,3},
	{"7m7",2,4},
	{"7gm",2,3},
	{"7ss",1,3},
	{"7sm",1,3},
	{"7sb",1,3},
	{"7fc",1,3},
	{"7cr",2,3},
	{"7bs",2,3},
	{"7ls",2,3},
	{"7wd",1,3},
	{"72h",1,4},
	{"7cm",1,4},
	{"7gs",1,4},
	{"7b7",1,4},
	{"7fb",2,4},
	{"7gd",2,4},
	{"7dg",1,2},
	{"7di",1,2},
	{"7kr",1,3},
	{"7bl",1,3},
	{"7tk",1,2},
	{"7ta",1,2},
	{"7bk",1,2},
	{"7b8",2,3},
	{"7ja",1,3},
	{"7pi",1,3},
	{"7s7",1,3},
	{"7gl",1,4},
	{"7ts",1,4},
	{"7sr",2,4},
	{"7tr",2,4},
	{"7br",2,4},
	{"7st",2,4},
	{"7p7",2,4},
	{"7o7",2,4},
	{"7vo",2,4},
	{"7s8",2,4},
	{"7pa",2,4},
	{"7h7",2,4},
	{"7wc",2,4},
	{"6ss",1,3},
	{"6ls",1,4},
	{"6cs",1,4},
	{"6bs",1,4},
	{"6ws",2,4},
	{"6sb",2,3},
	{"6hb",2,3},
	{"6lb",2,4},
	{"6cb",2,3},
	{"6s7",2,3},
	{"6l7",2,4},
	{"6sw",2,3},
	{"6lw",2,4},
	{"6lx",2,3},
	{"6mx",2,3},
	{"6hx",2,4},
	{"6rx",2,3},
	{"ob1",1,2},
	{"ob2",1,2},
	{"ob3",1,2},
	{"ob4",1,2},
	{"ob5",1,3},
	{"am1",2,4},
	{"am2",2,4},
	{"am3",2,4},
	{"am4",2,4},
	{"am5",1,3},
	{"ob6",1,2},
	{"ob7",1,2},
	{"ob8",1,2},
	{"ob9",1,2},
	{"oba",1,3},
	{"am6",2,4},
	{"am7",2,4},
	{"am8",2,4},
	{"am9",2,4},
	{"ama",1,3},
	{"obb",1,2},
	{"obc",1,2},
	{"obd",1,2},
	{"obe",1,2},
	{"obf",1,3},
	{"amb",2,4},
	{"amc",2,4},
	{"amd",2,4},
	{"ame",2,4},
	{"amf",1,3},
	{"cap",2,2},
	{"skp",2,2},
	{"hlm",2,2},
	{"fhl",2,2},
	{"ghm",2,2},
	{"crn",2,2},
	{"msk",2,2},
	{"qui",2,3},
	{"lea",2,3},
	{"hla",2,3},
	{"stu",2,3},
	{"rng",2,3},
	{"scl",2,3},
	{"chn",2,3},
	{"brs",2,3},
	{"spl",2,3},
	{"plt",2,3},
	{"fld",2,3},
	{"gth",2,3},
	{"ful",2,3},
	{"aar",2,3},
	{"ltp",2,3},
	{"buc",2,2},
	{"sml",2,2},
	{"lrg",2,3},
	{"kit",2,3},
	{"tow",2,3},
	{"gts",2,4},
	{"lgl",2,2},
	{"vgl",2,2},
	{"mgl",2,2},
	{"tgl",2,2},
	{"hgl",2,2},
	{"lbt",2,2},
	{"vbt",2,2},
	{"mbt",2,2},
	{"tbt",2,2},
	{"hbt",2,2},
	{"lbl",2,1},
	{"vbl",2,1},
	{"mbl",2,1},
	{"tbl",2,1},
	{"hbl",2,1},
	{"bhm",2,2},
	{"bsh",2,3},
	{"spk",2,3},
	{"xap",2,2},
	{"xkp",2,2},
	{"xlm",2,2},
	{"xhl",2,2},
	{"xhm",2,2},
	{"xrn",2,2},
	{"xsk",2,2},
	{"xui",2,3},
	{"xea",2,3},
	{"xla",2,3},
	{"xtu",2,3},
	{"xng",2,3},
	{"xcl",2,3},
	{"xhn",2,3},
	{"xrs",2,3},
	{"xpl",2,3},
	{"xlt",2,3},
	{"xld",2,3},
	{"xth",2,3},
	{"xul",2,3},
	{"xar",2,3},
	{"xtp",2,3},
	{"xuc",2,2},
	{"xml",2,2},
	{"xrg",2,3},
	{"xit",2,3},
	{"xow",2,3},
	{"xts",2,4},
	{"xlg",2,2},
	{"xvg",2,2},
	{"xmg",2,2},
	{"xtg",2,2},
	{"xhg",2,2},
	{"xlb",2,2},
	{"xvb",2,2},
	{"xmb",2,2},
	{"xtb",2,2},
	{"xhb",2,2},
	{"zlb",2,1},
	{"zvb",2,1},
	{"zmb",2,1},
	{"ztb",2,1},
	{"zhb",2,1},
	{"xh9",2,2},
	{"xsh",2,3},
	{"xpk",2,3},
//	{"","",""},
	{"dr1",2,2},
	{"dr2",2,2},
	{"dr3",2,2},
	{"dr4",2,2},
	{"dr5",2,2},
	{"ba1",2,2},
	{"ba2",2,2},
	{"ba3",2,2},
	{"ba4",2,2},
	{"ba5",2,2},
	{"pa1",2,2},
	{"pa2",2,2},
	{"pa3",2,4},
	{"pa4",2,4},
	{"pa5",2,2},
	{"ne1",2,2},
	{"ne2",2,2},
	{"ne3",2,2},
	{"ne4",2,2},
	{"ne5",2,2},
	{"ci0",2,2},
	{"ci1",2,2},
	{"ci2",2,2},
	{"ci3",2,2},
	{"uap",2,2},
	{"ukp",2,2},
	{"ulm",2,2},
	{"uhl",2,2},
	{"uhm",2,2},
	{"urn",2,2},
	{"usk",2,2},
	{"uui",2,3},
	{"uea",2,3},
	{"ula",2,3},
	{"utu",2,3},
	{"ung",2,3},
	{"ucl",2,3},
	{"uhn",2,3},
	{"urs",2,3},
	{"upl",2,3},
	{"ult",2,3},
	{"uld",2,3},
	{"uth",2,3},
	{"uul",2,3},
	{"uar",2,3},
	{"utp",2,3},
	{"uuc",2,2},
	{"uml",2,2},
	{"urg",2,3},
	{"uit",2,3},
	{"uow",2,3},
	{"uts",2,4},
	{"ulg",2,2},
	{"uvg",2,2},
	{"umg",2,2},
	{"utg",2,2},
	{"uhg",2,2},
	{"ulb",2,2},
	{"uvb",2,2},
	{"umb",2,2},
	{"utb",2,2},
	{"uhb",2,2},
	{"ulc",2,1},
	{"uvc",2,1},
	{"umc",2,1},
	{"utc",2,1},
	{"uhc",2,1},
	{"uh9",2,2},
	{"ush",2,3},
	{"upk",2,3},
	{"dr6",2,2},
	{"dr7",2,2},
	{"dr8",2,2},
	{"dr9",2,2},
	{"dra",2,2},
	{"ba6",2,2},
	{"ba7",2,2},
	{"ba8",2,2},
	{"ba9",2,2},
	{"baa",2,2},
	{"pa6",2,2},
	{"pa7",2,2},
	{"pa8",2,4},
	{"pa9",2,4},
	{"paa",2,2},
	{"ne6",2,2},
	{"ne7",2,2},
	{"ne8",2,2},
	{"ne9",2,2},
	{"nea",2,2},
	{"drb",2,2},
	{"drc",2,2},
	{"drd",2,2},
	{"dre",2,2},
	{"drf",2,2},
	{"bab",2,2},
	{"bac",2,2},
	{"bad",2,2},
	{"bae",2,2},
	{"baf",2,2},
	{"pab",2,2},
	{"pac",2,2},
	{"pad",2,4},
	{"pae",2,4},
	{"paf",2,2},
	{"neb",2,2},
	{"neg",2,2},
	{"ned",2,2},
	{"nee",2,2},
	{"nef",2,2},

	{"elx",1,1},
	{"hpo",1,1},
	{"mpo",1,1},
	{"hpf",1,1},
	{"mpf",1,1},
	{"vps",1,1},
	{"yps",1,1},
	{"rvs",1,1},
	{"rvl",1,1},
	{"wms",1,1},
	{"tbk",1,2},
	{"ibk",1,2},
	{"amu",1,1},
	{"vip",1,1},
	{"rin",1,1},
//	{"gld",1,1},
	{"bks",2,2},
	{"bkd",2,2},
	{"aqv",1,3},
	{"tch",1,2},
	{"cqv",1,3},
	{"tsc",1,1},
	{"isc",1,1},

	/*
	{"hrt",1,1},
	{"brz",1,1},
	{"jaw",1,1},
	{"eyz",1,1},
	{"hrn",1,1},
	{"tal",1,1},
	{"flg",1,2},
	{"fng",1,1},
	{"qll",1,1},
	{"sol",1,1},
	{"scz",1,1},
	{"spe",1,1},
	*/
	{"key",1,1},
	{"luv",1,2},
	{"xyz",1,1},
	{"j34",1,2},
	{"g34",1,2},
	{"bbb",2,2},
	{"box",2,2},
	{"tr1",2,2},
	{"mss",1,1},
	{"ass",2,2},
	{"qey",1,1},
	{"qhr",1,1},
	{"qbr",1,1},
	{"ear",1,1},
	{"gcv",1,1},
	{"gfv",1,1},
	{"gsv",1,1},
	{"gzv",1,1},
	{"gpv",1,1},
	{"gcy",1,1},
	{"gfy",1,1},
	{"gsy",1,1},
	{"gly",1,1},
	{"gpy",1,1},
	{"gcb",1,1},
	{"gfb",1,1},
	{"gsb",1,1},
	{"glb",1,1},
	{"gpb",1,1},
	{"gcg",1,1},
	{"gfg",1,1},
	{"gsg",1,1},
	{"glg",1,1},
	{"gpg",1,1},
	{"gcr",1,1},
	{"gfr",1,1},
	{"gsr",1,1},
	{"glr",1,1},
	{"gpr",1,1},
	{"gcw",1,1},
	{"gfw",1,1},
	{"gsw",1,1},
	{"glw",1,1},
	{"gpw",1,1},
	{"hp1",1,1},
	{"hp2",1,1},
	{"hp3",1,1},
	{"hp4",1,1},
	{"hp5",1,1},
	{"mp1",1,1},
	{"mp2",1,1},
	{"mp3",1,1},
	{"mp4",1,1},
	{"mp5",1,1},
	{"skc",1,1},
	{"skf",1,1},
	{"sku",1,1},
	{"skl",1,1},
	{"skz",1,1},
//	{"","",""},
	{"hrb",1,1},
	{"cm1",1,1},
	{"cm2",1,2},
	{"cm3",1,3},
	{"rps",1,1},
	{"rpl",1,1},
	{"bps",1,1},
	{"bpl",1,1},
	{"r01",1,1},
	{"r02",1,1},
	{"r03",1,1},
	{"r04",1,1},
	{"r05",1,1},
	{"r06",1,1},
	{"r07",1,1},
	{"r08",1,1},
	{"r09",1,1},
	{"r10",1,1},
	{"r11",1,1},
	{"r12",1,1},
	{"r13",1,1},
	{"r14",1,1},
	{"r15",1,1},
	{"r16",1,1},
	{"r17",1,1},
	{"r18",1,1},
	{"r19",1,1},
	{"r20",1,1},
	{"r21",1,1},
	{"r22",1,1},
	{"r23",1,1},
	{"r24",1,1},
	{"r25",1,1},
	{"r26",1,1},
	{"r27",1,1},
	{"r28",1,1},
	{"r29",1,1},
	{"r30",1,1},
	{"r31",1,1},
	{"r32",1,1},
	{"r33",1,1},
	{"jew",1,1},
	{"ice",1,1},
	{"0sc",1,1},
	{"tr2",2,2},

	{NULL,-1,-1}
};

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

SIZE D2GetItemSize(LPCTSTR lpszItemCode)
{
	SIZE val = { 0 };
	if (lpszItemCode == NULL)
		return val;

	for (int i = 0; ITEM_SIZE_TB[i].code; i++)
	{
		if (stricmp(lpszItemCode, ITEM_SIZE_TB[i].code) == 0)
		{
			val.cx = ITEM_SIZE_TB[i].x;
			val.cy = ITEM_SIZE_TB[i].y;
			return val;
		}
	}

	return val;
}

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
