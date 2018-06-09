//-----------------------------------------------------------------------------
// Picture (Prototypes) Version 1.00
//
// Routins 4 Showing Picture Files... (.BMP .DIB .EMF .GIF .ICO .JPG .WMF)
//
// Author: Dr. Yovav Gad, EMail: Sources@SuperMain.com ,Web: www.SuperMain.com
//
// Update History:
//
// v1.01 - Added "Draw(CDC* pDC, POINT LeftTop)" for convenience, when the user
//        does not want the images to be Stretched and does not know the original
//        image dimensions. Also renamed "Show" functions to "Draw".
//
//=============================================================================

#if !defined(AFX_PICTURE_H__COPYFREE_BY_YOVAV_GAD__SOURCES_AT_SUPERMAIN_DOT_COM__INCLUDED_)
#define AFX_PICTURE_H__COPYFREE_BY_YOVAV_GAD__SOURCES_AT_SUPERMAIN_DOT_COM__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPicture
{
public:
	
	CPicture();
	virtual ~CPicture();

	BOOL Load(LPBYTE lpData, DWORD dwDataSize);
	BOOL Draw(CDC* pDC, POINT LeftTop);
	BOOL Draw(CDC* pDC, CPoint LeftTop, CPoint WidthHeight, int MagnifyX, int MagnifyY);
	BOOL Draw(CDC* pDC, CRect DrawRect, BOOL bStretch = FALSE);

	void FreePictureData();
	BOOL Load(CString sFilePathName);
	BOOL Load(UINT ResourceName, LPCSTR ResourceType);
	BOOL LoadPictureData(BYTE* pBuffer, int nSize);
	BOOL SaveAsBitmap(CString sFilePathName);	
	BOOL ShowBitmapResource(CDC* pDC, const int BMPResource, CPoint LeftTop);
	BOOL UpdateSizeOnDC(CDC* pDC);
	SIZE GetPictureSize() const;
	BOOL IsValid() const;

private:

	IPicture* m_IPicture; // Same As LPPICTURE (typedef IPicture __RPC_FAR *LPPICTURE)
	LONG      m_Height; // Height (In Pixels Ignor What Current Device Context Uses)
	LONG      m_Weight; // Size Of The Image Object In Bytes (File OR Resource)
	LONG      m_Width;  // Width (In Pixels Ignor What Current Device Context Uses)
};

#endif // !defined(AFX_PICTURE_H__COPYFREE_BY_YOVAV_GAD__SOURCES_AT_SUPERMAIN_DOT_COM__INCLUDED_)
