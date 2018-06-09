//////////////////////////////////////////////////////////////////////
// D2Abstract.h
//
// CD2Abstract, an abstract class which presents an event-driving interface
// of Diablo II game events, objects and accessors. This class will be the
// topmost base class of any Diablo II game manipulating classes.
// 
// The class encapsulates the following pure virtual methods which must
// be implemented in any derived classes:
//
// OnGameJoin
// OnGameLeave
// OnGamePacketBeforeReceived
//
// Written by Abin (abinn32@yahoo.com)
// http://www.wxjindu.com/abin/
//
// July 14th, 2004
//
//////////////////////////////////////////////////////////////////////

#ifndef __D2ABSTRACT_H__
#define __D2ABSTRACT_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#include <string.h>
#include "D2Hackit\\D2Hackit.h"
#include "D2Hackit\\D2Client.h"
#include "Definition\\D2MessageDef.h"
#include "Definition\\SpellDef.h"
#include "Definition\\MapDef.h"
#include "Definition\\AffectionDef.h"
#include "Definition\\KeyDef.h"
#include "Definition\\PlayerDef.h"
#include "Definition\\WaypointDef.h"
#include "Template\\ArrayEx.h"
#include "Template\\LinkedList.h"
#include "Utils\\BitFields.h"
#include "Utils\\Ini.h"
#include "Utils\\StringParse.h"
#include "Utils\\Ticker.h"
#include "Utils\\item.h"
#include "Utils\\D2Stdio.h"

///////////////////////////////////////////////////////////////////////
// CD2Abstract Class Definition
///////////////////////////////////////////////////////////////////////
class CD2Abstract 
{
public:	
		
	///////////////////////////////////////////////////////////////////////
	// Constructor(s) & destructor
	///////////////////////////////////////////////////////////////////////
	CD2Abstract();
	virtual ~CD2Abstract();

	///////////////////////////////////////////////////////////////////////
	// Client Entries
	///////////////////////////////////////////////////////////////////////
	virtual void OnGameJoin(const THISGAMESTRUCT* pGame) = 0; // To be called in "OnGameJoin"
	virtual void OnGameLeave() = 0; // To be called in "OnGameLeave"
	virtual void OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen) = 0; // To be called in "OnGamePacketBeforeReceived"
	virtual void OnGamePacketBeforeSent(const BYTE* aPacket, DWORD aLen); // To be called in "OnGamePacketBeforeSent"
	virtual void OnGameTimerTick(); // To be called in "OnGameTimerTick"

	///////////////////////////////////////////////////////////////////////
	// Public Attributes
	///////////////////////////////////////////////////////////////////////
	BOOL IsLoadedBeforeJoin() const; // Is module loaded before joining game?
	BOOL IsGameJoined() const; // Is game joined?
	BOOL IsGameReady() const; // Is the game ready(all data have been received)?
	BYTE GetCurrentAct() const; // Current game act
	BOOL IsDebug() const; // Is in debug mode?
	
	///////////////////////////////////////////////////////////////////////
	// Public Operations
	///////////////////////////////////////////////////////////////////////
	virtual void InitAttributes(); // Reset all attributes
	virtual void SetD2MessageProc(D2MESSAGEPROC lpfn, LPVOID lpData = NULL); // Set the message handler function
	virtual BOOL DisplayNotLoadedBeforeJoinWarning(LPCTSTR lpszModuleName) const; // Displays a warning message for not being loaded befor joining game
	void SetDebug(BOOL bSet); // Allow/disallow debug

	static DWORD FormatPacket(BOOL bReceived, const BYTE* aPacket, DWORD aLen, LPTSTR lpszBuffer, DWORD dwMaxChars, BOOL bUseColor = FALSE);
	BOOL PrintPacket(BOOL bReceived, const BYTE *aPacket, DWORD aLen) const;

protected:

	///////////////////////////////////////////////////////////////////////
	// Overrides
	///////////////////////////////////////////////////////////////////////
	
	///////////////////////////////////////////////////////////////////////
	// Protected Operations
	///////////////////////////////////////////////////////////////////////
	BOOL NotifyD2Message(UINT nMessage, WPARAM wParam, LPARAM lParam) const; // Send a D2 message
	BOOL DebugMessage(LPCTSTR lpszDebug) const; // Send text
	BOOL FormatAndDebugMessage(LPCTSTR lpszFormat, ...) const; // Format text and send
	BOOL SendPacketToServer(const BYTE* aPacket, DWORD aLen, int nTimes = 1) const; // Send a packet to server
	BOOL SendPacketToGame(const BYTE* aPacket, DWORD aLen, int nTimes = 1) const; // Send a packet to game

	///////////////////////////////////////////////////////////////////////
	// Protected Member Data
	///////////////////////////////////////////////////////////////////////
	BOOL m_bDebug; // Whether to display debug messages
	D2PLAYERDATA m_thisPlayer; // Data for current game player

private:

	///////////////////////////////////////////////////////////////////////
	// Private Member Data
	///////////////////////////////////////////////////////////////////////
	D2MESSAGEPROC m_lpfnMessageProc; // The message handler
	LPVOID m_lpMessageData; // 32-bit application data, sent along with object messages
	BOOL m_bLoadedBeforeJoin; // is module loaded before joining a game?
	BOOL m_bGameJoined; // Joined a game
};

#endif // __D2ABSTRACT_H__
