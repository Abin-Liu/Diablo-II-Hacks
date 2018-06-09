// D2PacketRelayer.cpp: implementation of the CD2PacketRelayer class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "D2PacketRelayer.h"
#include "SharedMem.h"
#include "MessageDef.h"
#include <tchar.h>

#define D2PACKETWND_CLASS _T("{ECFBA126-824C-4FC5-BB2F-5090404797CD}")
CSharedMem g_mem;
CD2PacketRelayer* g_pRelayer = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CD2PacketRelayer::CD2PacketRelayer()
{	
	::memset(&m_player, 0, sizeof(PLAYERINFO));
	m_dwCurLen = 0;
	m_dwCurCopied = 0;
	m_bCurType = TRUE;
	m_pCurPacket = NULL;
	m_dwPacketNo = 0;
	m_hRelayerWnd = NULL;
	m_dwMercID = 0;
	g_pRelayer = this; // The global pointer to this class

	// Register our window class
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = RelayPacket;
	wc.lpszClassName = D2PACKETWND_CLASS;
	::RegisterClass(&wc);

	// Create our hidden window. This window will be processing all packet
	// notification sent by "pr.dll" from the Diablo II game.
	m_hRelayerWnd = ::CreateWindow(D2PACKETWND_CLASS, D2PACKETWND_CLASS, WS_OVERLAPPED, 0, 0, 100, 100, ::GetDesktopWindow(), NULL, NULL, 0);
	if (!::IsWindow(m_hRelayerWnd))
		return;

	// Other initialization
	g_mem.Create(MEM_ID, MEM_SIZE);
	if (!g_mem.IsCreated())
		return;

	// find the first location to register our window
	BOOL bRegistered = FALSE;
	for (DWORD i = 0; i < MAX_HWND; i++)
	{
		HWND hWnd = (HWND)g_mem.GetLong(MEM_POS_HWND + i * 4);
		if (!::IsWindow(hWnd))
		{
			g_mem.SetLong((long)m_hRelayerWnd, MEM_POS_HWND + i * 4);
			bRegistered = TRUE;
			break;
		}
	}

	if (!bRegistered)
	{
		g_mem.Close();
		::DestroyWindow(m_hRelayerWnd);	
		m_hRelayerWnd = NULL;
	}

	// check if client is already started
	if (g_mem.GetByte(MEM_POS_CLIENT))
		::PostMessage(m_hRelayerWnd, WM_CLIENT_START, 0, 0);

	// check if game is already joined
	if (g_mem.GetByte(MEM_POS_GAME))
	{
		DWORD dwPlayerID = (DWORD)g_mem.GetLong(MEM_POS_PLAYERID);
		BYTE iCharClass = g_mem.GetByte(MEM_POS_CHARCLASS);
		::PostMessage(m_hRelayerWnd, WM_GAMEJOIN, (WPARAM)iCharClass, (LPARAM)dwPlayerID);
	}

	// check if merc is already spawned
	m_dwMercID = (DWORD)g_mem.GetLong(MEM_POS_MERCID);
	if (m_dwMercID)
		::PostMessage(m_hRelayerWnd, WM_GAMEJOIN, (WPARAM)g_mem.GetByte(MEM_POS_MERCALIVE), (LPARAM)m_dwMercID);
}

CD2PacketRelayer::~CD2PacketRelayer()
{	
	// Release all resources
	g_mem.Close();
	if (::IsWindow(m_hRelayerWnd))
		::DestroyWindow(m_hRelayerWnd);

	m_hRelayerWnd = NULL;
	if (m_pCurPacket)
	{
		delete [] m_pCurPacket;
		m_pCurPacket = NULL;
	}
}

LRESULT CD2PacketRelayer::RelayPacket(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (!g_pRelayer)
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		
	switch (uMsg)
	{
	case WM_CLIENT_START:
		::memset(&g_pRelayer->m_player, 0, sizeof(PLAYERINFO));
		g_pRelayer->OnClientStart();
		break;

	case WM_CLIENT_STOP:
		::memset(&g_pRelayer->m_player, 0, sizeof(PLAYERINFO));
		g_pRelayer->OnClientStop();
		break;

	case WM_GAMEJOIN:
		g_pRelayer->m_dwMercID = 0;
		::memset(&g_pRelayer->m_player, 0, sizeof(PLAYERINFO));
		g_pRelayer->m_player.dwPlayerID = (DWORD)lParam;
		g_pRelayer->m_player.iCharClass = (BYTE)wParam;
		g_mem.GetString(g_pRelayer->m_player.szPlayerName, 31, MEM_POS_PLAYERNAME);
		g_pRelayer->m_player.dwLocation = 0x05;
		g_pRelayer->OnGameJoin(&g_pRelayer->m_player);
		break;

	case WM_GAMELEAVE:
		g_pRelayer->m_dwMercID = 0;
		::memset(&g_pRelayer->m_player, 0, sizeof(PLAYERINFO));
		g_pRelayer->OnGameLeave();
		break;

	case WM_PLAYERLOCATION:
		g_pRelayer->m_player.iAct = (BYTE)LOWORD(wParam);
		g_pRelayer->m_player.dwLocation = HIWORD(wParam);
		g_pRelayer->m_player.wPositionX = LOWORD(lParam);
		g_pRelayer->m_player.wPositionY = HIWORD(lParam);
		break;

	case WM_MERC:
		g_pRelayer->m_dwMercID = (DWORD)lParam;
		break;

	case WM_PACKET_START: // Packet notofication head	
		
		if (g_pRelayer->m_pCurPacket)
		{
			delete [] g_pRelayer->m_pCurPacket;
			g_pRelayer->m_pCurPacket = NULL;
		}

		g_pRelayer->m_dwCurLen = HIWORD(lParam);
		if (g_pRelayer->m_dwCurLen)
		{
			g_pRelayer->m_dwPacketNo = (DWORD)wParam;
			g_pRelayer->m_bCurType = LOWORD(lParam);
			g_pRelayer->m_dwCurCopied = 0;
			g_pRelayer->m_pCurPacket = new BYTE[g_pRelayer->m_dwCurLen];
		}
		break;	

	case WM_PACKET_BODY: // Packet notofication body

		if (g_pRelayer->m_pCurPacket && g_pRelayer->m_dwCurCopied < g_pRelayer->m_dwCurLen)
		{
			DWORD dwLimit = min(g_pRelayer->m_dwCurLen - g_pRelayer->m_dwCurCopied, 4);
			::memcpy(g_pRelayer->m_pCurPacket + g_pRelayer->m_dwCurCopied, &wParam, dwLimit);
			g_pRelayer->m_dwCurCopied += dwLimit;

			dwLimit = min(g_pRelayer->m_dwCurLen - g_pRelayer->m_dwCurCopied, 4);
			::memcpy(g_pRelayer->m_pCurPacket + g_pRelayer->m_dwCurCopied, &lParam, dwLimit);
			g_pRelayer->m_dwCurCopied += dwLimit;
		}
		break;

	case WM_PACKET_END: // Packet notofication end

		if ((DWORD)wParam == g_pRelayer->m_dwPacketNo
			&& g_pRelayer->m_dwCurLen == g_pRelayer->m_dwCurCopied 
			&& g_pRelayer->m_dwCurLen > 0
			&& g_pRelayer->m_pCurPacket)
		{
			if (g_pRelayer->m_bCurType)
				g_pRelayer->OnGamePacketBeforeReceived(g_pRelayer->m_pCurPacket, g_pRelayer->m_dwCurLen);
			else
				g_pRelayer->OnGamePacketBeforeSent(g_pRelayer->m_pCurPacket, g_pRelayer->m_dwCurLen);
		}
		else
		{
			if (g_pRelayer->m_pCurPacket)
			{
				delete [] g_pRelayer->m_pCurPacket;
				g_pRelayer->m_pCurPacket = NULL;
			}

			g_pRelayer->m_dwCurLen = 0;
			g_pRelayer->m_bCurType = TRUE;
			g_pRelayer->m_dwCurCopied = 0;
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		break;
	}
	
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

DWORD CD2PacketRelayer::GetGamePacketCount() const
{
	return m_dwPacketNo;
}

BOOL CD2PacketRelayer::IsRelayerValid() const
{
	return g_mem.IsCreated() && ::IsWindow(m_hRelayerWnd);
}

LPCPLAYERINFO CD2PacketRelayer::GetPlayer() const
{
	return &m_player;
}

DWORD CD2PacketRelayer::GetMercID() const
{
	return m_dwMercID;
}
