#ifndef __D2LIBDLL_H__
#define __D2LIBDLL_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "Classes\\D2Abstract.h"
#include "Classes\\D2Player.h"
#include "Classes\\D2Object.h"
#include "Classes\\D2StaticObject.h"
#include "Classes\\D2Unit.h"
#include "Classes\\D2Monster.h"
#include "Classes\\D2Merc.h"

void __declspec(dllexport) D2OnGameJoin(CD2Abstract* obj, const THISGAMESTRUCT* pGame);
void __declspec(dllexport) D2OnGameJoin(CD2Object* obj, const THISGAMESTRUCT* pGame);
void __declspec(dllexport) D2OnGameJoin(CD2StaticObject* obj, const THISGAMESTRUCT* pGame);
void __declspec(dllexport) D2OnGameJoin(CD2Unit* obj, const THISGAMESTRUCT* pGame);
void __declspec(dllexport) D2OnGameJoin(CD2Monster* obj, const THISGAMESTRUCT* pGame);
void __declspec(dllexport) D2OnGameJoin(CD2Merc* obj, const THISGAMESTRUCT* pGame);
void __declspec(dllexport) D2OnGameJoin(CD2Player* obj, const THISGAMESTRUCT* pGame);

void __declspec(dllexport) D2OnGameLeave();
void __declspec(dllexport) D2OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen);
void __declspec(dllexport) D2OnGamePacketBeforeSent(const BYTE* aPacket, DWORD aLen);
void __declspec(dllexport) D2OnGameTimerTick();

#endif