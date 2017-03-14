#ifndef __IRSRECEIVABLEDLG_H__
#define __IRSRECEIVABLEDLG_H__

#pragma once

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

// CIRSReceivableDlg dialog

class CIRSReceivableDlg : public CDialog
{
	DECLARE_DYNAMIC(CIRSReceivableDlg)
public:
	CDocData *m_pData;
	CDBSheet m_SelSS;
	CDBSheet m_SS;
	CMaskIDEdit m_TransNum;
	CMaskDateEdit m_FromDate;
	CMaskDateEdit m_ToDate;
	COptComboBox *m_Portfolio;
	COptComboBox *m_Currency;
	CNumEdit m_Interest;
	
	
public:
	CIRSReceivableDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIRSReceivableDlg();

// Dialog Data
	enum { IDD = IDD_IRS_RECEIVABLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void OnBlockSelectedIrsSelList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	void OnDblClickIrsSelList(long Col, long Row);
	void OnDblClickIrsList(long Col, long Row);
	afx_msg void OnBnClickedIrsUpdateButton();
	afx_msg void OnBnClickedIrsDeleteButton();
	afx_msg void OnBnClickedIrsLoadButton();
};
#endif