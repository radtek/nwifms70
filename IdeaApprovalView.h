#ifndef __IDEAAPPROVALVIEW_H__
#define __IDEAAPPROVALVIEW_H__

#pragma once

#ifndef __IDEADATA_H__
#include "ideadata.h"
#endif

#ifndef __NWIFMS70DOC_H__
#include "nwifms70doc.h"
#endif

// CIdeaApprovalView form view

class CIdeaApprovalView : public CFormView
{
	DECLARE_DYNCREATE(CIdeaApprovalView)

protected:
	CIdeaApprovalView();           // protected constructor used by dynamic creation
	virtual ~CIdeaApprovalView();

public:
	enum { IDD = IDD_TRADEIDEA_APPROVAL_FORM };
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
	COptComboBox *m_Decision;

	CNumEdit m_Capital;
	CNumEdit m_ApprovedCapital;

	CCntlEdit m_Idea;
	CCntlEdit m_Note;
	CCntlEdit m_RiskNote;
	BOOL m_bRiskManager;

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
	void OnInitialUpdate();
	afx_msg void OnIdeasapprovalLoadideas();
	afx_msg void OnIdeasapprovalLoadnewideas();
	afx_msg void OnCbnSelchangeIdeaaDecisionCombo();
	DECLARE_EVENTSINK_MAP()
	void OnDblClickIdeaaList(long Col, long Row);
	void OnBlockSelectedIdeaaList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnBnClickedIdeaaUpdateButton();
	afx_msg void OnEnChangeIdeaaRisknoteEdit();
};

#ifndef _DEBUG  // debug version in CIdeaApproval.cpp
inline CNWIFMS70Doc* CIdeaApprovalView::GetDocument()
   { return (CNWIFMS70Doc*)m_pDocument; }
#endif

#endif
