#include <windows.h>

enum DllNo {DLLNO_D2CLIENT, DLLNO_D2COMMON, DLLNO_D2GFX, DLLNO_D2WIN, DLLNO_D2LANG, DLLNO_D2CMP, DLLNO_D2MULTI, DLLNO_BNCLIENT, DLLNO_D2NET, DLLNO_STORM};

#define DLLBASE_D2CLIENT		0x6FAA0000
#define DLLBASE_D2COMMON		0x6FD40000
#define DLLBASE_D2GFX			0x6FA70000
#define DLLBASE_D2WIN			0x6F8A0000
#define DLLBASE_D2LANG			0x6FC10000
#define DLLBASE_D2CMP			0x6FDF0000
#define DLLBASE_D2MULTI			0x6F9A0000
#define DLLBASE_BNCLIENT		0x6FF00000
#define DLLBASE_D2NET			0x6FC00000
#define DLLBASE_STORM			0x6FFB0000
