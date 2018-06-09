#if !defined(AFX_D2STORAGE_H__15E353E7_BDF2_463D_BF6E_A7EC6FF3A419__INCLUDED_)
#define AFX_D2STORAGE_H__15E353E7_BDF2_463D_BF6E_A7EC6FF3A419__INCLUDED_

#include "..\\external\\Picture.h"
#include "D2ItemDesc.h"
#include "..\\..\\common\\ArrayEx.h"

typedef struct tagItemPos
{
	RECT rect;
	CD2ItemData* pItem;
	BOOL bVisible;
} ITEMPOS, *LPITEMPOS;
typedef const tagItemPos* LPCITEMPOS;

class CD2Storage
{
// Construction
public:
	
	CD2Storage();
	virtual ~CD2Storage();

	BOOL LoadFromPlayer(CD2PlayerData* pPlayer);
	BOOL IsVisible() const;
	void SetVisible(BOOL bVisible);
	void SetPosition(int x, int y);
	void DrawStorage(CDC* pDC);
	BOOL IsPtInStorage(POINT pt) const;
	const CD2ItemData* GetItemAtPos(POINT pt, CRect& rect) const;	
	void SetType(UINT nType, CResPack* pRes);	
	operator LPCRECT () const;
	const CRect& GetBoundary() const;
	int GetType() const;
	BOOL IsCloseButton(POINT pt) const;
	BOOL FindItemByID(DWORD dwItemID, CRect& rect) const;
	int IsPtOnWeaponSwitch(POINT pt) const;
	int GetItemCount() const;
	void WeaponSwitch();

private:

	BOOL CalcItemRect(const CD2ItemData* pItem, RECT& rect) const;

	UINT m_nType;
	POINT m_ptOrigin;
	CRect m_rcClose;
	POINT m_ptCloseOrigin;
	CPicture m_bkgnd;
	CPicture m_ws2;
	int m_cx;
	int m_cy;
	CRect m_rcBoundary;
	BOOL m_bVisible;
	BOOL m_bWs2;
	CRect m_rcWs1;
	CRect m_rcWs2;
	CArrayEx<ITEMPOS, const ITEMPOS&> m_items;
};

#endif // !defined(AFX_D2STORAGE_H__15E353E7_BDF2_463D_BF6E_A7EC6FF3A419__INCLUDED_)
