//////////////////////////////////////////////////////////////////////
// D2PacketRelayer.h
//
// CD2PacketRelayer, a generic packet relayer class which relays incoming
// and outgoing packets occur in the Diablo II game and let the developers
// read those packets outside of the game. Relayed packets, however, are
// all read-only, you cannot modify any packets.
//
// This class is useful for those who want to debug their packet parsing
// programs in VC++, because it is extremely difficult to do so in a D2H
// module.
//
// Written by Abin (abinn32@yahoo.com)
// http://www.wxjindu.com/abin
// July 8th, 2004
//////////////////////////////////////////////////////////////////////

#ifndef __D2PACKETRELAYER_H__
#define __D2PACKETRELAYER_H__

#include <windows.h>

// Player Info Struct Definition
typedef struct tagPlayerInfo
{
	DWORD dwPlayerID;		// Player ID
	BYTE iCharClass;		// Player character class
	char szPlayerName[32];	// Player name
	BYTE iAct;				// Act
	DWORD dwLocation;		// Location, 0x05=in town, 0x01=out of town
	WORD wPositionX;		// PositionX
	WORD wPositionY;		// PositionY
} PLAYERINFO, *LPPLAYERINFO;
typedef const tagPlayerInfo* LPCPLAYERINFO;


// Player Character Class Definition
enum
{
	CHARCLASS_AMAZON = 0,	// Amazon
	CHARCLASS_SORCERESS,	// Sorceress
	CHARCLASS_NECROMANCER,	// Necromancer
	CHARCLASS_PALADIN,		// Paladin
	CHARCLASS_BARBARIAN,	// Barbarian
	CHARCLASS_DRUID,		// Druid
	CHARCLASS_ASSASSIN		// Assassin
};


// The CPacketRelayer ADT Class Definition
class CD2PacketRelayer
{
public:		
	
	////////////////////////////////////////////////////////
	// Constructor & Destructor
	////////////////////////////////////////////////////////
	CD2PacketRelayer();
	virtual ~CD2PacketRelayer();

	////////////////////////////////////////////////////////
	// Attributes
	////////////////////////////////////////////////////////
	BOOL IsRelayerValid() const;		// Is this relayer valid?
	DWORD GetGamePacketCount() const;	// Number of packets that were sent/received in current game
	LPCPLAYERINFO GetPlayer() const;	// Get current player's info
	DWORD GetMercID() const;			// Get merc's ID

protected:

	////////////////////////////////////////////////////////
	// Must be Implemented in Derived Classes
	////////////////////////////////////////////////////////
	virtual void OnClientStart() = 0; // Called when the in-game module starts
	virtual void OnClientStop() = 0; // Called when the in-game module stops
	virtual void OnGameJoin(LPCPLAYERINFO lpPlayerInfo) = 0; // Called when the player joins a game
	virtual void OnGameLeave() = 0; // Called when a player leaves a game
	virtual void OnGamePacketBeforeReceived(const BYTE* aPacket, DWORD aLen) = 0; // Called when a packet is received	
	virtual void OnGamePacketBeforeSent(const BYTE *aPacket, DWORD aLen) = 0; // Called when a packet is sent	

private:

	////////////////////////////////////////////////////////
	// Even Derived Classes Shall Not Access These Members
	////////////////////////////////////////////////////////
	static LRESULT CALLBACK RelayPacket(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	DWORD m_dwPacketNo;		// No of packets that have been received/sent in current game
	HWND m_hRelayerWnd;		// A hidden window which processes packet notifications sent from the game
	DWORD m_dwCurLen;		// Current packet length
	DWORD m_dwCurCopied;	// Current copied length	
	BOOL m_bCurType;		// Current packet type (received/sent)
	BYTE* m_pCurPacket;		// Current copied packet data
	PLAYERINFO m_player;	// Player info data
	DWORD m_dwMercID;		// Merc's ID
};

#endif // __D2PACKETRELAYER_H__
