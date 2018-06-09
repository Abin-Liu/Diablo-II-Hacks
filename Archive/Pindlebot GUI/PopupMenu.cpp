#include "stdafx.h"
#include "PopupMenu.h"

void CmdRouteMenu(CWnd* pWnd,CMenu* pPopupMenu)
{
	CCmdUI state;
	state.m_pMenu = pPopupMenu;
	state.m_pParentMenu = pPopupMenu;
	state.m_nIndexMax = pPopupMenu->GetMenuItemCount();

	for (state.m_nIndex = 0; 
	     state.m_nIndex < state.m_nIndexMax; 
	     state.m_nIndex++) 
	{
		state.m_nID = pPopupMenu->GetMenuItemID(state.m_nIndex);

		// menu separator or invalid cmd - ignore it
		if (state.m_nID == 0) continue; 

		if (state.m_nID == (UINT)-1)
		{
			// possibly a popup menu, route to child menu if so
			CMenu* pSub=pPopupMenu->GetSubMenu(state.m_nIndex);
			if(pSub) CmdRouteMenu(pWnd,pSub);
		}
		else 
		{
			// normal menu item, Auto disable if command is 
			// _not_ a system command.
			state.m_pSubMenu = NULL;
			state.DoUpdate(pWnd, FALSE);
		}
	}
}

BOOL PopupMenu(UINT nMenuID, UINT nSubMenuID, CWnd* pParentWnd, LPPOINT lpLocation, UINT nFlags)
{
	CMenu menu;
	if (!menu.LoadMenu(nMenuID))
		return FALSE;

	CMenu* pMenu = menu.GetSubMenu(nSubMenuID);
	if (pMenu == NULL)
		return FALSE;

	return PopupMenu(pMenu, pParentWnd, lpLocation, nFlags);
}

BOOL PopupMenu(CMenu* pPopupMenu, CWnd* pParentWnd, LPPOINT lpLocation, UINT nFlags)
{
	if (pParentWnd == NULL)
		pParentWnd = AfxGetMainWnd();

	HWND hWnd = NULL;
	if (pParentWnd != NULL)
		hWnd = pParentWnd->GetSafeHwnd();

	CmdRouteMenu(pParentWnd, pPopupMenu);
	POINT pt;
	if (lpLocation == NULL)
		::GetCursorPos(&pt);
	else
		pt = *lpLocation;	

	::SetForegroundWindow(hWnd);	
	BOOL bResult = pPopupMenu->TrackPopupMenu(nFlags, pt.x, pt.y, pParentWnd, NULL);
	::PostMessage(hWnd, WM_NULL, 0, 0);

	return bResult;
}