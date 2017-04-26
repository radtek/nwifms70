#ifndef __CPFUTURECOMMDLG_H__
#define __CPFUTURECOMMDLG_H__

#pragma once

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __COUNTRYDATA_H__
#include "countrydata.h"
#endif

#ifndef __COMBOX_H__
#include "combobox.h"
#endif

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

// CCPFutureCommDlg dialog

class CCPFutureCommDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCPFutureCommDlg)

public:
	CCPFutureCommDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCPFutureCommDlg();

// Dialog Data
	enum { IDD = IDD_FUTURE_COMM_SCHEDULE_DIALOG };
	COraLoader m_OraLoader;
	CDBSheet m_SS;
	CDocData *m_pData;
	COptComboBox *m_CP;
	COptComboBox *m_TransType;
	COptComboBox *m_Currency;
	COptComboBox *m_FutureType;
	CMaskDateEdit m_FromDate;
	CMaskDateEdit m_ToDate;
	CNumEdit m_Commission;
	CCheckBox *m_ETrade;
	CString m_RowID;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	void EnableCtrls();
	BOOL IsOK(int nAction);
	void RowToControls();

	// Generated message map functions
	//{{AFX_MSG(CCountry)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void BlockSelectedFcList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	void DblClickFcList(long Col, long Row);
	afx_msg void OnBnClickedFcLoadButton();
	afx_msg void OnBnClickedFcAddButton();
	afx_msg void OnBnClickedFcUpdateButton();
	afx_msg void OnBnClickedFcDeleteButton();
};
#endif