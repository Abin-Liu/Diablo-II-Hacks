#ifndef __POPUPMENU_H__
#define __POPUPMENU_H__

void CmdRouteMenu(CWnd* pWnd, CMenu* pPopupMenu);
BOOL PopupMenu(UINT nMenuID, UINT nSubMenuID, CWnd* pParentWnd, LPPOINT lpLocation = NULL, UINT nFlags = TPM_LEFTALIGN);
BOOL PopupMenu(CMenu* pPopupMenu, CWnd* pParentWnd, LPPOINT lpLocation = NULL, UINT nFlags = TPM_LEFTALIGN);

#endif