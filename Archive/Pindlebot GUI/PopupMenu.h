#ifndef __POPUPMENU_H__
#define __POPUPMENU_H__

BOOL PopupMenu(UINT nMenuID, UINT nSubMenuID, CWnd* pParentWnd, LPPOINT lpLocation = NULL, UINT nFlags = TPM_LEFTALIGN);
BOOL PopupMenu(CMenu* pPopupMenu, CWnd* pParentWnd, LPPOINT lpLocation = NULL, UINT nFlags = TPM_LEFTALIGN);

#endif