
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "FileView.h"
#include "ClassView.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	UINT m_nCurrent;

// Operations
public:
	void LoadAppMenu(UINT MenuID, LPCTSTR PopupMenuTitle);
	void RemoveAppMenu();

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	CFileView         m_wndFileView;
	CClassView        m_wndClassView;
	COutputWnd        m_wndOutput;
	CPropertiesWnd    m_wndProperties;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	afx_msg void OnUpdateinfo();
	afx_msg void OnPassword();
	afx_msg void OnChangePortfolio();
	afx_msg void OnUpdateChangePortfolio(CCmdUI *pCmdUI);
	afx_msg void OnNWIFMSApplication(UINT nCmdID);
	afx_msg void OnUpdateTicketBlotter(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTicketEntry(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTicketProcess(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTicketSign(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTicketApproval(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTransaction(CCmdUI *pCmdUI);
	afx_msg void OnUpdateConfirm(CCmdUI *pCmdUI);
	afx_msg void OnUpdateCash(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePayment(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMaintenance(CCmdUI *pCmdUI);
	afx_msg void OnUpdateAsset(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDaily(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDataProcessing(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTicketMaturity(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTicketProcessMaturity(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIdeaEntry(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIdeaRisk(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIdeaApproval(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTicketFxentry(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFundSizing(CCmdUI *pCmdUI);
	afx_msg void OnUpdateTradeOrder(CCmdUI *pCmdUI);
};


