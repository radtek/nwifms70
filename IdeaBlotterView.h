#ifndef __IDEABLOTTERVIEW_H__
#define __IDEABLOTTERVIEW_H__

#pragma once

#ifndef __IDEADATA_H__
#include "ideadata.h"
#endif

#ifndef __NWIFMS70DOC_H__
#include "nwifms70doc.h"
#endif

// CIdeaBlotterView form view

class CIdeaBlotterView : public CFormView
{
	DECLARE_DYNCREATE(CIdeaBlotterView)

protected:
	CIdeaBlotterView();           // protected constructor used by dynamic creation
	virtual ~CIdeaBlotterView();

public:
	enum { IDD = IDD_TRADEIDEA_BLOTTER_FORM };
	CIdeaData m_Data;
	CDBSheet m_SS;

	CMaskIDEdit m_IdeaID;
	
	CMaskDateEdit m_Date;
	
	COptComboBox *m_Action;
	COptComboBox *m_Aim;
	COptComboBox *m_Dir;
	COptComboBox *m_Currency;
	COptComboBox *m_Counterparty;
	COptComboBox *m_Trader;
	COptComboBox *m_Risk;
	COptComboBox *m_Reward;
	COptComboBox *m_Opinion;
	COptComboBox *m_Decision;

	CNumEdit m_Capital;
	CNumEdit m_ApprovedCapital;

	CCntlEdit m_Idea;
	CCntlEdit m_Note;
	CCntlEdit m_RiskNote;

	BOOL m_bFind;

// Attributes
public:
	CNWIFMS70Doc *GetDocument();
	CDocData &GetData() { return GetDocument()->m_Data; }

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void InitControls();
	BOOL UpdateData(BOOL bSaveandValid);

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void OnInitialUpdate();
	void OnDblClickIdeabList(long Col, long Row);
	afx_msg void OnIdeablotterLoadideas();
	afx_msg void OnIdeablotterClearscreen();
	afx_msg void OnIdeablotterFindideas();
};


#ifndef _DEBUG  // debug version in CTicketEntry.cpp
inline CNWIFMS70Doc* CIdeaBlotterView::GetDocument()
   { return (CNWIFMS70Doc*)m_pDocument; }
#endif

#endif
