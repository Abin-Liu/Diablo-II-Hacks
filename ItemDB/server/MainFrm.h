// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__0F40C21C_E631_4133_A830_917D066B0E36__INCLUDED_)
#define AFX_MAINFRM_H__0F40C21C_E631_4133_A830_917D066B0E36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "External\\SystemTray.h"
#include "Diablo2\\ItemDatabase.h"
#include "External\\StringEx.h"
#include "ItemDBToolbar.h"
#include "External\\D2Monitor.h"
#include "External\\MsgWnd.h"
#include "Diablo2\\D2View.h"
#include "External\\ReportCtrl.h"
#include "CharList.h"

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	
	virtual ~CMainFrame();

// Generated message map functions
protected:
	
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPopupRestore();
	afx_msg void OnUpdatePopupRestore(CCmdUI* pCmdUI);
	afx_msg void OnD2view();
	afx_msg void OnUpdateD2view(CCmdUI* pCmdUI);
	afx_msg void OnItemview();
	afx_msg void OnUpdateItemview(CCmdUI* pCmdUI);
	afx_msg void OnSqlview();
	afx_msg void OnUpdateSqlview(CCmdUI* pCmdUI);
	afx_msg void OnToolbarDropDown(NMTOOLBAR* pnmh, LRESULT* plRes);
	afx_msg void OnStartSearch();
	afx_msg void OnUpdateStartSearch(CCmdUI* pCmdUI);
	afx_msg void OnBykeywords();
	afx_msg void OnUpdateBykeywords(CCmdUI* pCmdUI);
	afx_msg void OnByexp();
	afx_msg void OnUpdateByexp(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdatePane(CCmdUI *pCmdUI);
	afx_msg void OnDelete();
	afx_msg void OnUpdateDelete(CCmdUI* pCmdUI);
	afx_msg void OnExport();
	afx_msg void OnUpdateExport(CCmdUI* pCmdUI);
	afx_msg void OnLangEng();
	afx_msg void OnUpdateLangEng(CCmdUI* pCmdUI);
	afx_msg void OnLangChs();
	afx_msg void OnUpdateLangChs(CCmdUI* pCmdUI);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnHelphelp();
	//}}AFX_MSG

	LRESULT OnSetItemView(WPARAM wParam, LPARAM lParam);
	LRESULT OnSetCharView(WPARAM wParam, LPARAM lParam);
	LRESULT OnSetRealmView(WPARAM wParam, LPARAM lParam);
	LRESULT OnSetListView(WPARAM wParam, LPARAM lParam);
	LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnStatusText(WPARAM wParam, LPARAM lParam);
	LRESULT OnListChars(WPARAM wParam, LPARAM lParam);	
	LRESULT OnListItems(WPARAM wParam, LPARAM lParam);
	LRESULT OnNavigateTo(WPARAM wParam, LPARAM lParam);
	LRESULT OnSearchIt(WPARAM wParam, LPARAM lParam);
	LRESULT OnGameWnd(WPARAM wParam, LPARAM lParam);
	LRESULT OnClientUpdate(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

protected:
	HTREEITEM FindTreeItem(HTREEITEM hParent, DWORD dwItemID) const;
	void ListItems(DWORD dwPlayerID);
	void SetType(int nType);
	BOOL VerifyViews() const;
	CD2PlayerData* CachePlayerData(DWORD dwPlayerID);
	BOOL RemoveCachedPlayer(DWORD dwPlayerID);
	BOOL OnUpdatePlayer(LPCHARINFO lpPlayer, int nItemCount, HANDLE hFile);
	static BOOL CALLBACK CleanupProc(HWND hWnd, LPARAM lParam);
	BOOL Export(CReportCtrl* pwndList, LPCSTR lpszTitle);
	static void WriteItemLine(CStdioFile* pFile, const CString& str, BOOL bHtml, COLORREF color = RGB(0, 0, 0));
	static BOOL CALLBACK ExportProc(CStdioFile* pFile, CReportCtrl* pwndList, int nIndex, BOOL bHTM);
	static void NormalizeSearchStr(CString& str);
	void ListRealms();
	void ListAccounts(HTREEITEM hRealm, DWORD dwRealmID);
	CD2PlayerData* FindCachedPlayer(DWORD dwPlayerID);
	void RemoveFirstCache(DWORD dwCurPlayerID);
	static void RemoveSingleQuote(LPSTR lpsz);
	static void RemoveSocketSymbol(LPITEMINFO lpItem);
	DWORD GetCharID(LPCHARINFO lpChar);

	CMenu m_menu;
	CStatusBar  m_wndStatusBar;
	CItemDBToolbar m_wndToolBar;
	CSplitterWnd m_wndSplitterH;
	CSplitterWnd m_wndSplitterV;
	CSystemTray m_tray;
	CMsgWnd m_msgWnd;

	CTreeCtrl* m_pwndRealms;
	CCharList* m_pwndChars;
	CD2View* m_pwndD2;
	CReportCtrl* m_pwndItems;
	CReportCtrl* m_pwndSQL;
	
	CD2Monitor m_monitor;

	CItemDatabase m_db;
	CImageList m_ilChars;
	CLinkedList<CD2PlayerData*, CD2PlayerData*, FreeD2PlayerData> m_aCache;

	BOOL m_bUseExp;
	BOOL m_bIsSearching;
	BOOL m_bMaximized;
	int m_nType;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__0F40C21C_E631_4133_A830_917D066B0E36__INCLUDED_)
