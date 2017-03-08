#ifndef __CPCOUNTERPARTYDLG_H__
#define __CPCOUNTERPARTYDLG_H__

#pragma once

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

// CCPCounterpartyDlg dialog

class CCPCounterpartyDlg : public CDialog
{
	DECLARE_DYNAMIC(CCPCounterpartyDlg)

public:
	CCPCounterpartyDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCPCounterpartyDlg();
	CDBSheet m_SS;

	CCntlEdit m_CPEdit;
	COptComboBox* m_CPList;

// Dialog Data
	enum { IDD = IDD_CPCP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void EnableCtrls();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void OnBlockSelectedCpcpList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	void OnDblClickCpcpList(long Col, long Row);

	afx_msg void OnBnClickedCpcpClearButton();
	afx_msg void OnBnClickedCpcpAddButton();
	afx_msg void OnBnClickedCpcpUpdateButton();
	afx_msg void OnBnClickedCpcpDeleteButton();
	afx_msg void OnBnClickedCpcpLoadButton();
	afx_msg void OnBnClickedCpcpLoadallButton();
};
#endif