#pragma once

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

#ifndef __CHECKBOX_H__
#include "checkbox.h"
#endif

#ifndef __CNTLEDIT_H__
#include "cntledit.h"
#endif

// CTraderDlg dialog

class CTraderDlg : public CDialog
{
	DECLARE_DYNAMIC(CTraderDlg)

public:
	CTraderDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTraderDlg();

// Dialog Data
	enum { IDD = IDD_TRADER_DIALOG };
	CDBSheet m_SS;

	COraLoader m_OraLoader;
	CCntlEdit m_TraderIniEdit;
	CCntlEdit m_TraderNameEdit;
	CCntlEdit m_FMSNameEdit;
	CCheckBox m_Active;
	CCheckBox m_PowerUser;
	CCheckBox m_RiskManager;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void GetData(CDBRec &Rec);
	COraLoader &GetOraLoader() { return m_OraLoader; }
	BOOL IsOK(BOOL bNew);
	int LoadTraders();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void DblClickTraderList(long Col, long Row);
	void BlockSelectedTraderList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnBnClickedTraderLoadButton();
	afx_msg void OnBnClickedTraderAddButton();
	afx_msg void OnBnClickedTraderUpdateButton();
	afx_msg void OnBnClickedTraderDeleteButton();
	afx_msg void OnEnChangeTraderIniEdit();
};
