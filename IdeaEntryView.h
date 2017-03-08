#ifndef __IDEAENTRYVIEW_H__
#define __IDEAENTRYVIEW_H__

#pragma once

#ifndef __IDEADATA_H__
#include "ideadata.h"
#endif

#ifndef __NWIFMS70DOC_H__
#include "nwifms70doc.h"
#endif

// CIdeaEntryView form view

class CIdeaEntryView : public CFormView
{
	DECLARE_DYNCREATE(CIdeaEntryView)

protected:
	CIdeaEntryView();           // protected constructor used by dynamic creation
	virtual ~CIdeaEntryView();

public:
	enum { IDD = IDD_TRADEIDEA_ENTRY_FORM };
	CIdeaData m_Data;
	CDBSheet m_SS;

	CMaskIDEdit m_IdeaID;
	
	CMaskDateEdit m_Date;
	
	COptComboBox *m_Action;
	COptComboBox *m_Aim;
	COptComboBox *m_Dir;
	COptComboBox *m_Currency;
	COptComboBox *m_Trader;
	COptComboBox *m_Counterparty;
	COptComboBox *m_Risk;
	COptComboBox *m_Reward;

	CNumEdit m_Capital;

	CCntlEdit m_Idea;

	BOOL m_bLoaded;

// Attributes
public:
	CNWIFMS70Doc *GetDocument();
	CDocData &GetData() { return GetDocument()->m_Data; }

// Operations
public:
	void InitControls();
	BOOL IsOK();
	BOOL UpdateData(BOOL bSaveandValid = TRUE);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIdeaEntryView)
	public:
	virtual void OnInitialUpdate();
	//}}AFX_VIRTUAL

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void OnDblClickIdeaeList(long Col, long Row);
	void OnBlockSelectedIdeaeList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnIdeasAdd();
	afx_msg void OnIdeasUpdate();
	afx_msg void OnIdeasDelete();
	afx_msg void OnUpdateIdeasAdd(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIdeasUpdate(CCmdUI *pCmdUI);
	afx_msg void OnUpdateIdeasDelete(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in CIdeaEntryView.cpp
inline CNWIFMS70Doc* CIdeaEntryView::GetDocument()
   { return (CNWIFMS70Doc*)m_pDocument; }
#endif

#endif