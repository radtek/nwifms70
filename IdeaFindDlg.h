#ifndef __IDEAFINDDLG_H__
#define __IDEAFINDDLG_H__

#pragma once

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

#ifndef __COMBOX_H__
#include "combox.h"
#endif

#ifndef __MASKDATEEDIT_H__
#include "maskdateedit.h"
#endif

#ifndef __NUMEDIT_H__
#include "numedit.h"
#endif

#ifndef __IDEADATA_H__
#include "ideadata.h"
#endif

// CIdeaFindDlg dialog

class CIdeaFindDlg : public CDialog
{
	DECLARE_DYNAMIC(CIdeaFindDlg)

public:
	CIdeaFindDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIdeaFindDlg();

// Dialog Data
	enum { IDD = IDD_TRADEIDEA_FIND_DLG };

	CDocData *m_pData;
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

	CString m_strIdeaID;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void EnableFind(CEdit& Edit, BOOL bAssetDesc = FALSE);
	void EnableFind(COptComboBox* Combo);
	CDocData &GetData() { return *m_pData; }

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	void OnDblClickIdeafList(long Col, long Row);
	afx_msg void OnBnClickedIdeafClearButton();
	afx_msg void OnBnClickedIdeafFindButton();
	afx_msg void OnEnChangeIdeafDateEdit();
	afx_msg void OnEnChangeIdeafIdeaEdit();
	afx_msg void OnEnChangeIdeafRisknoteEdit();
	afx_msg void OnEnChangeIdeafNoteEdit();
	afx_msg void OnCbnSelchangeIdeafTraderCombo();
	afx_msg void OnCbnSelchangeIdeafActionCombo();
	afx_msg void OnCbnSelchangeIdeafDirCombo();
	afx_msg void OnCbnSelchangeIdeafCurrencyCombo();
	afx_msg void OnCbnSelchangeIdeafCpCombo();
	afx_msg void OnCbnSelchangeIdeafDecisionCombo();
	afx_msg void OnCbnSelchangeIdeafAimCombo();
	afx_msg void OnCbnSelchangeIdeafRiskCombo();
	afx_msg void OnCbnSelchangeIdeafRewardCombo();
};
#endif