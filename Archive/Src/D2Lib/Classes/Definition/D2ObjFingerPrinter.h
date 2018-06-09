//////////////////////////////////////////////////////////////////////
// D2ObjFingerprinter.h
//
// Definition of the D2OBJFINGERPRINTER struct, which represents an object's
// fingerprinter data.
//
// An object's fingerprinter is an array of bytes which distinguish the
// object from others, an object without a fingerprinter is meaningless.
// The fingerprinter will be verified against object spawning packets
// (0x51 and 0xac), an object can only become valid if its fingerprinter
// data exactly match the spawning packets.
//
// Some common D2 object's fingerprinter data have been defined in this
// file already, if, however, fingerprinter of the objects that you are
// interested of have not been defined in this file, it is your responsibility
// to find it out by yourself.
//
// Written by Abin (abinn32@yahoo.com)
// http://www.wxjindu.com/abin/
//
// July 14th, 2004
//
//////////////////////////////////////////////////////////////////////

#ifndef __D2OBJFINGERPRINTER_H__
#define __D2OBJFINGERPRINTER_H__

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

///////////////////////////////////////////////////////////////////////
// D2OBJFINGERPRINTER Struct Definition
///////////////////////////////////////////////////////////////////////
#define D2OBJ_MAX_FINGERPRINTER_LEN	32 // Maximum fingerprinter data length
typedef struct tagD2ObjFingerprinter
{
	BYTE aData[D2OBJ_MAX_FINGERPRINTER_LEN]; // The fingerprinter data bytes
	DWORD dwLength; // Data length
	DWORD dwPosStart; // Starting search position
	DWORD dwPosEnd; // Ending search position
} D2OBJFINGERPRINTER, *LPD2OBJFINGERPRINTER;
typedef const tagD2ObjFingerprinter* LPCD2OBJFINGERPRINTER;


///////////////////////////////////////////////////////////////////////
// Common Object Fingerprinter Definition
///////////////////////////////////////////////////////////////////////

// Static Objects
static const D2OBJFINGERPRINTER FP_OBJ_ACT1WP = {{0x77, 0x00}, 2, 6, 7}; // Act 1-5 Way-Points
static const D2OBJFINGERPRINTER FP_OBJ_ACT2WP = {{0x9c, 0x00}, 2, 6, 7};
static const D2OBJFINGERPRINTER FP_OBJ_ACT3WP = {{0xed, 0x00}, 2, 6, 7};
static const D2OBJFINGERPRINTER FP_OBJ_ACT4WP = {{0x8e, 0x01}, 2, 6, 7};
static const D2OBJFINGERPRINTER FP_OBJ_ACT5WP = {{0xad, 0x01}, 2, 6, 7};
static const D2OBJFINGERPRINTER FP_OBJ_TOWNPORTAL = {{0x3b, 0x00}, 2, 6, 7}; // Town Portals(Blue Protals)
static const D2OBJFINGERPRINTER FP_OBJ_ACT5REDPORTAL = {{0x3c, 0x00}, 2, 6, 7}; // Act5 Red Portals
static const D2OBJFINGERPRINTER FP_OBJ_STASH = {{0x0b, 0x01}, 2, 6, 7}; // Stash

// Act1 NPC
static const D2OBJFINGERPRINTER FP_NPC_ACT1CAIN = {{0x09, 0x01}, 2, 5, 6}; // Act1 Cain
static const D2OBJFINGERPRINTER FP_NPC_ACT1WARRIV = {{0x9b, 0x00}, 2, 5, 6}; // Act1 Warriv
static const D2OBJFINGERPRINTER FP_NPC_CHARSI = {{0x9a, 0x00}, 2, 5, 6}; // Charsi
static const D2OBJFINGERPRINTER FP_NPC_GHEED = {{0x93, 0x00}, 2, 5, 6}; // Gheed
static const D2OBJFINGERPRINTER FP_NPC_KASHYA = {{0x96, 0x00}, 2, 5, 6}; // Kashya (God she's sexy!)
static const D2OBJFINGERPRINTER FP_NPC_AKARA = {{0x94, 0x00}, 2, 5, 6}; // Akara

// Act2 NPC
static const D2OBJFINGERPRINTER FP_NPC_ACT2CAIN = {{0xf4, 0x00}, 2, 5, 6}; // Act2 Cain
static const D2OBJFINGERPRINTER FP_NPC_ACT2WARRIV = {{0xaf, 0x00}, 2, 5, 6}; // Act2 Warriv
static const D2OBJFINGERPRINTER FP_NPC_GREIZ = {{0xc6, 0x00}, 2, 5, 6}; // Greiz
static const D2OBJFINGERPRINTER FP_NPC_ELZIX = {{0xc7, 0x00}, 2, 5, 6}; // Elzix
static const D2OBJFINGERPRINTER FP_NPC_JERHYN = {{0xc9, 0x00}, 2, 5, 6}; // Jerhyn
static const D2OBJFINGERPRINTER FP_NPC_LYSANDER = {{0xca, 0x00}, 2, 5, 6}; // Lysander
static const D2OBJFINGERPRINTER FP_NPC_FARA = {{0xb2, 0x00}, 2, 5, 6}; // Fara
static const D2OBJFINGERPRINTER FP_NPC_GEGLASH = {{0xc8, 0x00}, 2, 5, 6}; // Geglash
static const D2OBJFINGERPRINTER FP_NPC_ATMA = {{0xb0, 0x00}, 2, 5, 6}; // Atma
static const D2OBJFINGERPRINTER FP_NPC_GROGNAN = {{0xb1, 0x00}, 2, 5, 6}; // Grognan
static const D2OBJFINGERPRINTER FP_NPC_ACT2MESHIF = {{0xd2, 0x00}, 2, 5, 6}; // Act2 Meshif

// Act3 NPC
static const D2OBJFINGERPRINTER FP_NPC_ACT3CAIN = {{0xf5, 0x00}, 2, 5, 6}; // Act3 Cain
static const D2OBJFINGERPRINTER FP_NPC_ORMUS = {{0xff, 0x00}, 2, 5, 6}; // Ormus
static const D2OBJFINGERPRINTER FP_NPC_ALKOR = {{0xfe, 0x00}, 2, 5, 6}; // Alkor
static const D2OBJFINGERPRINTER FP_NPC_ASHEARA = {{0xfc, 0x00}, 2, 5, 6}; // Asheara
static const D2OBJFINGERPRINTER FP_NPC_ACT3MESHIF = {{0x08, 0x01}, 2, 5, 6}; // Act3 Meshif
static const D2OBJFINGERPRINTER FP_NPC_HRATLI = {{0xfd, 0x00}, 2, 5, 6}; // Hratli

// Act4 NPC
static const D2OBJFINGERPRINTER FP_NPC_ACT4CAIN = {{0xf6, 0x00}, 2, 5, 6}; // Act4 Cain
static const D2OBJFINGERPRINTER FP_NPC_ACT4TYRAEL = {{0x6f, 0x01}, 2, 5, 6}; // Act4 Tyrael
static const D2OBJFINGERPRINTER FP_NPC_JAMELLA = {{0x95, 0x01}, 2, 5, 6}; // Jamella
static const D2OBJFINGERPRINTER FP_NPC_HALBU = {{0x01, 0x01}, 2, 5, 6}; // Halbu

// Act5 NPC
static const D2OBJFINGERPRINTER FP_NPC_ACT5CAIN = {{0x08, 0x02}, 2, 5, 6}; // Act5 Cain
static const D2OBJFINGERPRINTER FP_NPC_LARZUK = {{0xff, 0x01}, 2, 5, 6}; // Larzuk
static const D2OBJFINGERPRINTER FP_NPC_MALAH = {{0x01, 0x02}, 2, 5, 6}; // Malah
static const D2OBJFINGERPRINTER FP_NPC_QUAL = {{0x03, 0x02}, 2, 5, 6}; // Qual-Kehk
static const D2OBJFINGERPRINTER FP_NPC_TOWNANYA = {{0x00, 0x02}, 2, 5, 6}; // Anya(in town)

#endif