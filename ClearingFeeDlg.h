#ifndef __CLEARINGFEEDLG_H__
#define __CLEARINGFEEDLG_H__
#pragma once


// CClearingFeeDlg dialog

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __CLEARINGFEEDATA_H__
#include "clearingfeedata.h"
#endif

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

class CClearingFeeDlg : public CDialog
{
	DECLARE_DYNAMIC(CClearingFeeDlg)

public:
	CClearingFeeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CClearingFeeDlg();

// Dialog Data
	enum { IDD = IDD_CLEARINGFEE_DIALOG };
	CClearingFeeData m_Data;
	CDBSheet m_SS;
	
	COptComboBox *m_Account;
	CMaskDateEdit m_FromDate;	
	CMaskDateEdit m_ToDate;
	CNumEdit m_Fee;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void EnableCtrls();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeClAccountCombo();
	DECLARE_EVENTSINK_MAP()
	void OnBlockSelectedClList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	void OnDblClickClList(long Col, long Row);
	afx_msg void OnBnClickedClFindButton();
	afx_msg void OnBnClickedClClearButton();
	afx_msg void OnBnClickedClAddButton();
	afx_msg void OnBnClickedClUpdateButton();
	afx_msg void OnBnClickedClDeleteButton();
};
#endif