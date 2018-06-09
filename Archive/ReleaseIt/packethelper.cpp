// packethelper.cpp ackmed@gotwalls.com
#include <windows.h>
#include "packethelper.h"
#include "External\\D2Hackit.h"
#include "External\\D2Client.h"

BOOL TakePortal(DWORD wpEntityID)
{
	if (wpEntityID == 0x00)
		return FALSE;

	InteractWithEntity(2,wpEntityID);
	InteractWithEntity(2,wpEntityID);
	return TRUE;
}

BOOL TakeWayPoint(DWORD wpEntityID, DWORD Dest) {

	// first interact with entity
	InteractWithEntity(2,wpEntityID);
	BYTE packet[9];
	packet[0] = 0x49;
	memcpy(&packet[1],&wpEntityID,4);
	memcpy(&packet[5],&Dest,4);
	server->GameSendPacketToServer(packet,9);
	return true;
}

BOOL InteractWithEntity(DWORD type,DWORD EntityID)
{
	if (EntityID == 0x00)
		return FALSE;

	BYTE packet[9];
	packet[0] = 0x13;
	memcpy(&packet[1],&type,4);
	memcpy(&packet[5],&EntityID,4);
	server->GameSendPacketToServer(packet,9);
	return true;

}

BOOL StopInteractWithEntity(DWORD type,DWORD EntityID)
{
	if (EntityID == 0x00)
		return FALSE;

	BYTE packet[9];
	packet[0] = 0x30;
	memcpy(&packet[1],&type,4);
	memcpy(&packet[5],&EntityID,4);
	server->GameSendPacketToServer(packet,9);
	return true;
}

BOOL TakeStep(WORD x, WORD y) {
	
	BYTE packet[5];
	packet[0] = 0x03;
	memcpy(&packet[1],&x,2);
	memcpy(&packet[3],&y,2);
	server->GameSendPacketToServer(packet,5);
	return true;
}

BOOL OpenStore( DWORD npcID)
{
	BYTE packet[17];
	/*
	packet[0] = 0x59;
	packet[1] = 0x01;
	packet[2] = packet[3] = packet[4] = 0;
	memcpy(&packet[5],&npcID,4);
	memcpy(&packet[9],&x,2);
	packet[11] = packet[12] = 0x00;
	memcpy(&packet[13],&y,2);
	packet[15] = packet[16] = 0x00;
	server->GameSendPacketToServer(packet,17);
	*/

	InteractWithEntity(1,npcID);
	
	packet[0] = 0x2f;
	packet[1] = 0x01;
	packet[2] = packet[3] = packet[4] = 0;
	memcpy(&packet[5],&npcID,4);
	server->GameSendPacketToServer(packet,9);

	packet[0] = 0x38;
	packet[1] = 0x01;
	packet[2] = packet[3] = packet[4] = 0;
	memcpy(&packet[5],&npcID,4);
	packet[9] = packet[10] = packet[11] = packet[12] = 0;
	server->GameSendPacketToServer(packet,13);

	return true;
}

// from berto
BOOL BuyItem(DWORD itemID, DWORD npcID) {
	BYTE packet[17];
	packet[0] = 0x32;
	memcpy(&packet[1],&npcID,4);
	memcpy(&packet[5],&itemID,4);
	packet[9] = 0x00;
	packet[10] = 0x00;
	packet[11] = 0x00;
	packet[12] = 0x00;
	packet[13] = 0x00;
	packet[14] = 0x00;
	packet[15] = 0x00;
	packet[16] = 0x00;
	server->GameSendPacketToServer(packet,17);
	return true;
}

BOOL CloseStore(DWORD npcID)
{
	if (npcID == 0x00)
		return FALSE;
	
	StopInteractWithEntity(1,npcID);
	return(StopInteractWithEntity(1,npcID)); // YES do it twice.
}


// forces the client view to goto x, y
// type
//		0 = hard cut to the location
//		1 = fadded cut to location (like when taking a waypoint)


BOOL ForceClientViewLocation(WORD x, WORD y, BYTE type) {
	BYTE packet[11];

	packet[0] = 0x15;
	packet[1] = 0x00;
	memcpy (packet + 2, &thisgame->player->PlayerID, 4);
	packet[5]  = 0x00;
	memcpy(&packet[6],&x,2);
	memcpy(&packet[8],&y,2);
	packet[10] = type;

	server->GameSendPacketToGame(packet,11);
	return true;
}

// sends the sound message that are trigged by the number pad
BOOL SendSoundToServer(WORD sndID) {
	BYTE packet[3];

	packet[0] = 0x3f;
	memcpy(&packet[1],&sndID,2);
	server->GameSendPacketToServer(packet,3);

	return true;
}

// fakes a sound message from another player or self
BOOL SendSoundToClient(DWORD pID, WORD sndID) {
	BYTE packet[8];

	packet[0] = 0x2c;
	packet[1] = 0x00;
	memcpy(&packet[2],&pID,4);
	memcpy(&packet[6],&sndID,2);
	server->GameSendPacketToGame(packet,8);
	return true;

}

// by Abin: Step towards Entity
void PrintPacket(BYTE* aPacket, int aLen, int SR)
{
	char sz[1024] = "";
	
	if (SR == 1) // sent
		strcpy(sz, "ÿc7Sÿc2");
	else if (SR == 2) // received
		strcpy(sz, "ÿc7Rÿc2");
	else // both
	{
	}

	sprintf(sz, "%s%d:ÿc0 ", sz, aLen);

	for (int i = 0; i < aLen; i++)
		sprintf(sz, "%s%02x ", sz, aPacket[i]);
	server->GamePrintInfo(sz);

}

BOOL StepToEntity(DWORD type, DWORD EntityID)
{
	if (EntityID == 0x00)
		return FALSE;

	BYTE packet[9];
	packet[0] = 0x04;
	memcpy(packet + 1, &type, 4);
	memcpy(packet + 5,&EntityID, 4);
	server->GameSendPacketToServer(packet,9); // step to entity
	return TRUE;
}

BOOL SelectSkill(WORD SkillID, BOOL bRight)
{
	BYTE aPacket[9];
	aPacket[0] = 0x3c;
	memcpy(aPacket + 1, &SkillID, 2);
	aPacket[3] = 0x00;
	aPacket[4] = bRight ? 0x00 : 0x80;
	aPacket[5] = aPacket[6] =aPacket[7] = aPacket[8] = 0xff;
	server->GameSendPacketToServer(aPacket, 9);
	server->GameSendPacketToServer(aPacket, 9);
	return TRUE;
}

BOOL CastOnMap(WORD x, WORD y, WORD SkillID, BOOL bRight)
{
	SelectSkill(SkillID, bRight);
	BYTE aPacket[5];
	aPacket[0] = bRight ? 0x0c : 0x05;
	memcpy(aPacket + 1, &x, 2);
	memcpy(aPacket + 3, &y, 2);	
	//PrintPacket(aPacket, 5);
	server->GameSendPacketToServer(aPacket, 5);
	//ForceClientCastOnMap(SkillID, x, y);
	return TRUE;
}

BOOL CastOnEntity(DWORD EntityID, WORD SkillID, BOOL bRight)
{
	SelectSkill(SkillID, bRight);
	BYTE aPacket[9];
	aPacket[0] = bRight ? 0x0d : 0x06;
	memcpy(aPacket + 1, &thisgame->player->PlayerID, 4);
	memcpy(aPacket + 5, &EntityID, 4);
	//PrintPacket(aPacket, 9);
	server->GameSendPacketToServer(aPacket, 9);
	//ForceClientCastOnEntity(SkillID, EntityID);
	return TRUE;
}

BOOL RepairAll(DWORD NpcID)
{
	if (NpcID == 0x00)
		return FALSE;
	
	BYTE aPacket[17];
	aPacket[0] = 0x35;
	memcpy(aPacket + 1, &NpcID, 4);
	memset(aPacket + 5, 0, 11);
	aPacket[16] = 0x80;
	//PrintPacket(aPacket, 9);

	// doing it twice won't hurt
	server->GameSendPacketToServer(aPacket, 17);
	server->GameSendPacketToServer(aPacket, 17);
	return TRUE;
}

BOOL OverHeadMsg(LPCTSTR lpMsg)
{
	int nSize = strlen(lpMsg);
	if (nSize == 0)
		return TRUE;

	int aLen = nSize + 6;
	BYTE* aPacket = new BYTE[aLen];
	aPacket[0] = 0x14;
	memset(aPacket + 1, 0, 2);
	memset(aPacket + 3 + nSize, 0, 3);
	for (int i = 0; i < nSize; i++)
	{
		memcpy(aPacket + 3 + i, lpMsg + i, 1);
	}
	server->GameSendPacketToServer(aPacket, aLen);
	delete [] aPacket;
	return TRUE;
}

// displays information above character's head, but does not send any packet to server
BOOL OverHeadMsgFake(LPCSTR lpMsg)
{
	int nSize = strlen(lpMsg);
	if (nSize == 0)
		return TRUE;

	int aLen = nSize + 12;
	BYTE* aPacket = new BYTE[aLen];
	aPacket[0] = 0x26;
	aPacket[1] = 0x05;
	aPacket[2] = 0x00;
	aPacket[3] = 0x00;
	::memcpy(aPacket + 4, &thisgame->player->PlayerID, 4);
	WORD wRand = (rand() % 65535) + 1;
	::memcpy(aPacket + 8, &wRand, 2);
	aPacket[10] = 0x00;
	::memcpy(aPacket + 11, lpMsg, nSize + 1); // copy whole string including the tail-null
	server->GameSendPacketToGame(aPacket, aLen);
	delete [] aPacket;
	return TRUE;
}

BOOL AntiIdle(int& nAntiIdleMode)
{
	nAntiIdleMode += 250;

	if (nAntiIdleMode <= 250)
	{
		OverHeadMsg("Anti-idle.");
		nAntiIdleMode += 250;
	}
	else if (nAntiIdleMode == 10000)
	{
		SendSoundToServer(26);  // this is for you;)
		nAntiIdleMode += 250;
	}
	else if (nAntiIdleMode >= 20000)
	{
		SendSoundToServer(26);  // this is for you;)
		nAntiIdleMode = 0;
	}

	return TRUE;
}

BOOL PickCorpose()
{
	BYTE aPacket[9];
	aPacket[0] = 0x13;
	memset(aPacket + 1, 0x00, 4);
	memset(aPacket + 5, 0x02, 1);
	memset(aPacket + 6, 0x00, 3);
	server->GameSendPacketToServer(aPacket, 9);
	return TRUE;
}

BOOL ResurrectMerc(DWORD npcID)
{
	if (npcID == 0x00)
		return FALSE;

	const aLen = 5;
	BYTE aPacket[aLen];
	aPacket[0] = 0x62;
	memcpy(aPacket + 1, &npcID, 4);
	server->GameSendPacketToServer(aPacket, aLen);
	return TRUE;
}

// force client view to "cast" the skill on map
BOOL ForceClientCastOnMap(WORD wSkillID, WORD x, WORD y)
{
	BYTE aPacket[15];
	aPacket[0] = 0x4d;
	aPacket[1] = 0x00;
	::memcpy(aPacket + 2, &thisgame->player->PlayerID, 4);
	::memcpy(aPacket + 6, &wSkillID, 2);
	aPacket[8] = 0x01;
	::memcpy(aPacket + 9, &x, 2);
	::memcpy(aPacket + 11, &y, 2);
	aPacket[13] = aPacket[14] = 0x00;
	server->GameSendPacketToGame(aPacket, 15);
	return TRUE;
}

// force client view to "cast" the skill on entity
BOOL ForceClientCastOnEntity(WORD wSkillID, DWORD dwEntityID)
{
	BYTE aPacket[16];
	aPacket[0] = 0x4c;
	aPacket[1] = 0x00;
	::memcpy(aPacket + 2, &thisgame->player->PlayerID, 4);
	::memcpy(aPacket + 6, &wSkillID, 2);
	aPacket[8] = aPacket[9] = 0x01;
	::memcpy(aPacket + 10, &dwEntityID, 4);
	aPacket[14] = aPacket[15] = 0x00;
	server->GameSendPacketToGame(aPacket, 16);
	return TRUE;
}

// from Syadasti's "Deposit" module
void DepositGold(DWORD dwAmount)
{
	if (dwAmount == 0)
		return;

	BYTE packet[7];
	WORD g1 = 0, g2 = 0;
	memset(&packet, 0, 7);
	// build deposit packet
	packet[0] = 0x4f;
	packet[1] = 0x14;
	g2 = dwAmount / 65536;
	g1 = dwAmount % 65536;
	memcpy(&packet[3], &g2, 2);
	memcpy(&packet[5], &g1, 2);
	server->GameSendPacketToServer(packet, 7);
}

// write a packet to a file
BOOL WritePacket(const BYTE* aPacket, DWORD aLen, int SR, LPCTSTR lpFile)
{
	// open the log file
	FILE *stream; 
	if (lpFile != NULL)
	{
		stream = fopen(lpFile, "a+");
	}
	else
	{
		stream = fopen("c:\\Packets.log", "a+");
	}

	// convert packet contents to string
	char szPacket[1025] = "";

	if (SR == 1) // sent
		strcpy(szPacket, "S: ");
	else if (SR == 2) // received
		strcpy(szPacket, "R: ");
	else // both
	{
	}

	for (int i = 0; i < aLen; i++)
		sprintf(szPacket, "%s%02X ", szPacket, aPacket[i]);
	fprintf(stream, "%s\n", szPacket); 
	fclose(stream); 
	return TRUE;
}
