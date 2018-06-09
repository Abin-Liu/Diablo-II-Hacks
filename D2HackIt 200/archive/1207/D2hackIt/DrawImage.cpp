//////////////////////////////////////////////////////////////
// DrawImage.cpp
//
// Game image drawing functions. Functions are from Mousepad,
// Provided by netterhaufen.
//
// Added by Abin (abinn32@yahoo.com)
//////////////////////////////////////////////////////////////

#include "DrawImage.h"
#include "definitions.h"
#include "d2functions.h"
#include "D2HackIt.h"

void *memcpy2(void *dest, const void *src, size_t count);
HANDLE OpenFileRead(char *filename);
DWORD ReadFile(HANDLE hFile, void *buf, DWORD len);
BYTE *AllocReadFile(char *filename);
CellFile *LoadBmpCellFile(BYTE *buf1, int width, int height);
CellFile *LoadBmpCellFile(char *filename);
CellFile *InitCellFile(CellFile *cf);
void DeleteCellFile(CellFile *cf);
void DrawAutomapCell2(CellFile *cellfile, int xpos, int ypos, BYTE col);

void *memcpy2(void *dest, const void *src, size_t count)
{
	return (char *)memcpy(dest, src, count)+count;
}

HANDLE OpenFileRead(char *filename)
{
	return CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
}

DWORD ReadFile(HANDLE hFile, void *buf, DWORD len)
//NOTE :- validates len bytes of buf
{
	DWORD numdone = 0;
	ReadFile(hFile, buf, len, &numdone, NULL);
	return numdone;
}

BYTE *AllocReadFile(char *filename)
{
	HANDLE hFile = OpenFileRead(filename);
	int filesize = GetFileSize(hFile, 0);
	if (filesize <= 0) return 0;
	BYTE *buf = new BYTE[filesize];
	ReadFile(hFile, buf, filesize);
	CloseHandle(hFile);
	return buf;
}

CellFile *LoadBmpCellFile(BYTE *buf1, int width, int height) 
{ 
     BYTE *buf2 = new BYTE[(width*height*2)+height], *dest = buf2; 
 
     for (int i = 0; i < height; i++) { 
          BYTE *src = buf1+(i*((width+3)&-4)), *limit = src+width; 
          while (src < limit) { 
               BYTE *start = src, *limit2 = min(limit, src+0x7f), trans = !*src; 
               do src++; while ((trans == (BYTE)!*src) && (src < limit2)); 
               if (!trans || (src < limit)) *dest++ = (trans?0x80:0)+(src-start); 
               if (!trans) while (start < src) *dest++ = *start++; 
          } 
          *dest++ = 0x80; 
     } 
 
     static DWORD dc6head[] = {6, 1, 0, 0xeeeeeeee, 1, 1, 0x1c,  0, -1, -1, 0, 0, 0, -1, -1}; 
     dc6head[8] = width; 
     dc6head[9] = height; 
     BYTE *ret = new BYTE[dc6head[13] = sizeof(dc6head)+(dc6head[14] = dest-buf2)+3]; 
     memset(memcpy2(memcpy2(ret, dc6head, sizeof(dc6head)), buf2, dc6head[14]), 0xee, 3); 
     delete buf2; 
 
     return (CellFile *)ret; 
} 
 
CellFile *LoadBmpCellFile(char *filename) 
{ 
     BYTE *ret = 0; 
           
     BYTE *buf1 = AllocReadFile(filename); 
     BITMAPFILEHEADER *bmphead1 = (BITMAPFILEHEADER *)buf1; 
     BITMAPINFOHEADER *bmphead2 = (BITMAPINFOHEADER *)(buf1+sizeof(BITMAPFILEHEADER)); 
     if (buf1 && (bmphead1->bfType == 'MB') && (bmphead2->biBitCount == 8) && (bmphead2->biCompression == BI_RGB)) { 
          ret = (BYTE *)LoadBmpCellFile(buf1+bmphead1->bfOffBits, bmphead2->biWidth, bmphead2->biHeight); 
     } 
     delete buf1; 
 
     return (CellFile *)ret; 
} 
 
CellFile *InitCellFile(CellFile *cf) 
{ 
     if (cf) D2CMP_InitCellFile(cf, &cf, "?", 0, -1, "?"); 
     return cf; 
} 
 
void DeleteCellFile(CellFile *cf) 
{ 
     if (cf) { 
          D2CMP_DeleteCellFile(cf); 
          delete cf; 
     } 
} 

void DrawAutomapCell2(CellFile *cellfile, int xpos, int ypos, BYTE col)
{
	CellContext ct;
	memset(&ct, 0, sizeof(ct));
	if (!(ct.pCellFile = cellfile)) return;

	//xpos -= (cellfile->cells[0]->width/2);
	ypos += (cellfile->cells[0]->height);

	//int xpos2 = xpos - cellfile->cells[0]->xoffs, ypos2 = ypos - cellfile->cells[0]->yoffs;
	//if ((xpos2 >= D2GetScreenSizeX()) || ((xpos2 + (int)cellfile->cells[0]->width) <= 0) || (ypos2 >= D2GetScreenSizeY()) || ((ypos2 + (int)cellfile->cells[0]->height) <= 0)) return;

	static BYTE coltab[2][256];//, tabno = 0, lastcol = 0;
	if (!coltab[0][1]) for (int k = 0; k < 255; k++) coltab[0][k] = coltab[1][k] = k;
	cellfile->mylastcol = coltab[cellfile->mytabno ^= (col != cellfile->mylastcol)][255] = col;

	D2GFX_DrawAutomapCell2(&ct, xpos, ypos, -1, 5, coltab[cellfile->mytabno]);
}

HD2BITMAP EXPORT LoadD2Bitmap(LPCSTR lpszFilePath)
{
	CellFile* p = LoadBmpCellFile((char*)lpszFilePath);
	if (p == NULL)
		return NULL;

	return (HD2BITMAP)InitCellFile(p);
}

BOOL EXPORT DestroyD2Bitmap(HD2BITMAP hBitmap)
{
	if (hBitmap == NULL)
		return FALSE;

	DeleteCellFile((CellFile*)hBitmap);
	return TRUE;
}

BOOL DrawBmpToScreen(LPVOID hBitmap, long x, long y, BYTE iTransColor)
{
	CellFile* p = (CellFile*)hBitmap;
	if (p == NULL)
		return FALSE;

	DrawAutomapCell2(p, x, y, iTransColor);
	return TRUE;
}