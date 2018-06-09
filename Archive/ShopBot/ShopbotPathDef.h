#ifndef __SHOPBOTPATHDEF_H__
#define __SHOPBOTPATHDEF_H__

#include "..\\D2Lib\\D2Lib.h"

// Act 1 Path(Relative to Stash Position, act1 map changes every game, fuck that)
const D2POSITION A1_START_TO_WP_TOP[] = { {7, -1}, {3, -15}, {-6, -27}, {0, 0} };
const D2POSITION A1_START_TO_WP_BOTTOM[] = { {7, -1}, {19, -1}, {33, -10}, {34, -17}, {0, 0} };
const D2POSITION A1_WP_TOP_TO_CHARIS[] = { {-14, -18}, {-24, -21}, {0, 0} };
const D2POSITION A1_WP_BOTTOM_TO_CHARIS[] = { {21, -4}, {11, -5}, {-3, -16}, {-20, -13}, {0, 0} };
const D2POSITION A1_WP_TOP_TO_AKARA[] = { {6, -8}, {16, -3}, {34, -4}, {48, -11}, {63, -16}, {0, 0} };
const D2POSITION A1_WP_BOTTOM_TO_AKARA[] = { {46, -14}, {63, -16}, {0, 0} };

// Act 2 Path(Absolute)
const D2POSITION A2_START_TO_WP[] = { {0x1421, 0x1453}, {0x1419, 0x1446}, {0x1405, 0x143a}, {0x1401, 0x1425}, {0x13eb, 0x141c}, {0x13e6, 0x1401}, {0x13de, 0x13e9}, {0x13ce, 0x13de}, {0, 0} };
//const D2POSITION A2_WP_TO_FARA[] = { {0x13ce, 0x13c6}, {0x13e5, 0x13c5}, {0x1401, 0x13c6}, {0x1405, 0x13d8}, {0, 0} };
const D2POSITION A2_WP_TO_FARA[] = { {0x13dc, 0x13e6}, {0x13f9, 0x13e6}, {0x13fa, 0x13db}, {0, 0} };
const D2POSITION A2_WP_TO_GROGNAN[] = {{0x13ce, 0x13c6}, {0x13e6, 0x13c2}, {0x13e6, 0x13b0}, {0, 0} };

// Act 3 Path(Absolute)
const D2POSITION A3_START_TO_WP[] = { {0x13fe, 0x1430}, {0x140b, 0x142c}, {0x140c, 0x1414}, {0x140c, 0x13fc}, {0x140e, 0x13e6}, {0x141b, 0x13e2}, {0x141f, 0x13cc}, {0x1425, 0x13bd}, {0, 0} };
const D2POSITION A3_WP_TO_ORMUS[] = { {0x1426, 0x13d3}, {0x141c, 0x13e2}, {0x140f, 0x13e4}, {0, 0}};
 
// Act 4 Path(Absolute)
const D2POSITION A4_START_TO_WP[] = { {0x13b8, 0x13b3}, {0x13b5, 0x139e}, {0, 0} };
const D2POSITION A4_WP_TO_JAMELLA[] = { {0x13b8, 0x13ae}, {0x13cb, 0x13b1}, {0x13df, 0x13b4}, {0x13e1, 0x13bd}, {0, 0} };
const D2POSITION A4_WP_TO_HALBU[] = { {0x13b8, 0x13ae}, {0x13cb, 0x13b1}, {0x13df, 0x13b4}, {0x13e1, 0x13a4}, {0, 0} };

// ACt 5 Path(Absolute)
const D2POSITION A5_START_TO_WP[] = { {0x13ea, 0x139f}, {0x13f0, 0x13b4}, {0x13f6, 0x13c7}, {0x13f9, 0x13c8}, {0, 0} };
const D2POSITION A5_WP_TO_MALAH[] = { {0x13f1, 0x13b8}, {0x13ec, 0x139f}, {0x13dd, 0x13a7}, {0x13d5, 0x13a5}, {0, 0} };
const D2POSITION A5_WP_TO_LARZUK[] = { {0x1407, 0x13c3}, {0x1418, 0x13be}, {0x1417, 0x13b5}, {0, 0} };
const D2POSITION A5_WP_TO_ANYA[] = { {0x1401, 0x13e3}, {0x140c, 0x13f4}, {0x1401, 0x1402}, {0x13f5, 0x13fe}, {0, 0} };

int SBGetPathCount(LPCD2POSITION lpPos);

#endif