#ifndef __WAYPOINTDEF_H__
#define __WAYPOINTDEF_H__

#include "MapDef.h"

///////////////////////////////////////////////////////////////////////
// WAYPOINT Struct Definition
///////////////////////////////////////////////////////////////////////

#define D2X_WAYPOINT_COUNT	39 // There are total 39 waypoints in D2X
typedef struct tagWayPoint
{
	BYTE iMapID;
	WORD wFingerprinterData;
} WAYPOINT, *LPWAYPOINT;
typedef const tagWayPoint* LPCWAYPOINT;

static const WAYPOINT D2X_WAYPOINTS[D2X_WAYPOINT_COUNT] = {

	// Act1 WP
	{ MAP_A1_ROGUE_ENCAMPMENT,			0x0077 },
	{ MAP_A1_COLD_PLAINS,				0x0077 },
	{ MAP_A1_STONY_FIELD,				0x0077 },
	{ MAP_A1_DARK_WOOD,					0x0077 },
	{ MAP_A1_BLACK_MARSH,				0x0077 },
	{ MAP_A1_OUTER_CLOISTER,			0x0077 },
	{ MAP_A1_JAIL_LEVEL_1,				0x009d },
	{ MAP_A1_INNER_CLOISTER,			0x0077 },
	{ MAP_A1_CATACOMBS_LEVEL_2,			0x009d },

	// Act2 WP
	{ MAP_A2_LUT_GHOLEIN,				0x009c },
	{ MAP_A2_SEWERS_LEVEL_2,			0x0143 },
	{ MAP_A2_DRY_HILLS,					0x009c },
	{ MAP_A2_HALLS_OF_THE_DEAD_LEVEL_2,	0x009c },
	{ MAP_A2_FAR_OASIS,					0x009c },
	{ MAP_A2_LOST_CITY,					0x009c },
	{ MAP_A2_PALACE_CELLAR_LEVEL_1,		0x0120 },
	{ MAP_A2_ARCANE_SANCTUARY,			0x0192 },
	{ MAP_A2_CAYON_OF_THE_MAGI,			0x0192 },

	// Act3 WP
	{ MAP_A3_KURAST_DOCKS,				0x00ed },
	{ MAP_A3_SPIDER_FOREST,				0x00ed },
	{ MAP_A3_GREAT_MARSH,				0x00ed },
	{ MAP_A3_FLAYER_JUNGLE,				0x00ed },
	{ MAP_A3_LOWER_KURAST,				0x00ed },
	{ MAP_A3_KURAST_BAZAAR,				0x00ed },
	{ MAP_A3_UPPER_KURAST,				0x00ed },
	{ MAP_A3_TRAVINCAL,					0x00ed },
	{ MAP_A3_DURANCE_OF_HATE_LEVEL_2,	0x0144 },

	// Act4 WP
	{ MAP_A4_THE_PANDEMONIUM_FORTRESS,	0x018e },
	{ MAP_A4_CITY_OF_THE_DAMNED,		0x00ee },
	{ MAP_A4_RIVER_OF_FLAME,			0x00ee },

	// Act5 WP
	{ MAP_A5_HARROGATH,					0x01ad },
	{ MAP_A5_FRIGID_HIGHLANDS,			0x01f0 },
	{ MAP_A5_ARREAT_PLATEAU,			0x01ff },
	{ MAP_A5_CRYSTALLINE_PASSAGE,		0x01ff },
	{ MAP_A5_GLACIAL_TRAIL,				0x01ff },
	{ MAP_A5_HALLS_OF_PAIN,				0x01f0 },
	{ MAP_A5_FROZEN_TUNDRA,				0x01f0 },
	{ MAP_A5_THE_ANCIENTS_WAY,			0x01ff },
	{ MAP_A5_WORLDSTONE_KEEP_LEVEL_2,	0x01ee },
};

WORD GetWaypointFingerprinterData(BYTE iMapID);

#endif // __WAYPOINTDEF_H__