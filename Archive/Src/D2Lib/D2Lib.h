///////////////////////////////////////////////////////////////////////////////
// D2Lib.h
//
// The D2LIB Library Beta
//
// Author:		Abin
// Homepage:	http://www.wxjindu.com/abin/
// Email:		abinn32@yahoo.com
//
// The D2Lib library is a Set of C++ Classes for D2H Module Development. It
// wraps a lot of important functionalities of the Diablo II game hacking,
// and aims for the following:
// 
// 1, To simplify D2H module development. 
// 2, To provide an Object-Oriented interface to the D2Hackit SDK.
// 3, To establish a Message-Driving system between the game and your application. 
//
// Please refer to the readme file(s) included in the library for more details.
//
// Acknowledgement
//
// The D2Lib library is an improvement that is made to the D2Hackit SDK, which
// was originally developed by Thohell.
//
// License Agreement
// 
// 1, Developing or using any third-party applications on the Battle.net closed
//    realms are considered hacking and may therefore receive punishments from
//    Blizzard Entertainment, including, but not limited to, character deletion,
//    account deletion, or CD-key removal. The author holds no responsibility
//    to any damage of data caused by use of this library. USE IT AT YOUR OWN RISK.
// 2, This library may be used in compiled form in any way you desire. This library
//    may be redistributed unmodified by any means providing it is not sold for
//    profit without the author¡¯s written consent, and providing that this notice
//    and the authors name is included. If the source code in this library is used
//    in any commercial application then acknowledgement must be made to the author.
// 3, This library is provided "as is" with no expressed or implied warranty.
//
// Installation
//
// 1, Add "D2Lib.lib" and "ClientCore.cpp" to your workspace.
// 2, Ignore "libc.lib". To do so, click "Project->Settings->Link", select "Input"
//    in the "Category" list, and type "libc.lib" in the "Ignore Libraries" box.
// 3, Include "D2Lib.h" where needed.
//
// Version History
//
// v1.00 beta - Initial beta release. (7/25/04, Abin)
// v1.01 beta - Fixed a problem on detecting player's base HP/mana.
// v1.02 beta - Added class "CD2TextDraw", some minor adjustments.
// v1.03 beta - Added PM_ACTCHANGE message.
// v1.04 beta - Fixed some problems on object interaction.
// v1.05 beta - Added PM_GAMEREADY message.
// v1.06 beta - Some performance improvements.
// v1.07 beta - Fixed a problem when parsing throwing item data
// v1.08 beta - Fixed a map data glitch.
// v1.09 beta - Improved the CD2TextDraw class.
///////////////////////////////////////////////////////////////////////////////

#ifndef __D2LIB_H__
#define __D2LIB_H__

// Common includes
#include "Classes\\D2Abstract.h"
#include "Classes\\D2Player.h"
#include "Classes\\D2Object.h"
#include "Classes\\D2StaticObject.h"
#include "Classes\\D2Unit.h"
#include "Classes\\D2Monster.h"
#include "Classes\\D2Merc.h"
#include "Classes\\Utils\\KeyBind.h"
#include "Classes\\Utils\\D2TextDraw.h"

#endif // __D2LIB_H__