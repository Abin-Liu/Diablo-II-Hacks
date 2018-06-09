#if !defined(AFX_D2VIEW_H__8C286B7B_2F82_4C1F_94B7_E57FC9072813__INCLUDED_)
#define AFX_D2VIEW_H__8C286B7B_2F82_4C1F_94B7_E57FC9072813__INCLUDED_

#include "D2Storage.h"
#include "..\\External\\ResPack.h"
#include "HoverWnd.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// D2View.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CD2View window

class CD2View : public CStatic
{
// Construction
public:
	
	CD2View();
	virtual ~CD2View();

	BOOL Initialize(LPCSTR lpszResourcePath);
	void GetMinRect(CRect& rect) const;
	void SetPlayer(CD2PlayerData* pPlayer);
	static CString GetLocationName(int nStorageType);
	int FindItemByID(DWORD dwItemID, CRect& rect) const;
	CResPack* GetResPack();
	void CheckMouseMove(POINT point);
	void OnEscape();
	CWnd* GetHoverWnd();
	void OpenMerc();
	void OpenCube();
	BOOL IsCurrentPlayer(DWORD dwPlayerID) const;
	const CD2PlayerData* GetPlayer() const;
	DWORD GetPlayerID() const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CD2View)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CD2View)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	
	void NotifyStatus(LPCSTR lpsz);
	BOOL IsCubeOpened() const;	
	void SetPosition(int x, int y);
	BOOL IsMercOpened() const;	
	void CloseMerc();
	void CloseCube();
	const CD2Storage* GetStorageConst(int nSTorageType) const;
	CD2Storage* GetStorage(int nStorageType);
	const CD2Storage* GetStorage(int nStorageType) const;	
	const CD2ItemData* GetItemAtPos(POINT pt, CRect& rect) const;

	CResPack m_res;
	CD2Storage m_equip;
	CD2Storage m_inv;
	CD2Storage m_cube;
	CD2Storage m_stash;
	CD2Storage m_merc;
	CD2Storage m_cursor;
	POINT m_ptMerc;
	CPicture m_mercPortrait;
	CPicture m_hand;
	CRect m_rcInvGold;
	CRect m_rcStashGold;
	CRect m_rcGoldLimit;	
	CD2PlayerData* m_pPlayer;
	CFont m_font;
	CRect m_rcMercName;
	CHoverWnd m_wndHover;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_D2VIEW_H__8C286B7B_2F82_4C1F_94B7_E57FC9072813__INCLUDED_)
