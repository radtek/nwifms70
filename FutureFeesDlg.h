#ifndef __FUTURE_FEE_DLG_H__
#define __FUTURE_FEE_DLG_H__
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
// CFutureFeesDlg dialog

class CFutureFeesDlg : public CDialog
{
	DECLARE_DYNAMIC(CFutureFeesDlg)

public:
	CFutureFeesDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFutureFeesDlg();

// Dialog Data
	enum { IDD = IDD_FUTURE_FEES_DIALOG };
	COraLoader m_OraLoader;
	CDBSheet m_SS;
	CDocData *m_pData;
	COptComboBox *m_Custodian;
	COptComboBox *m_Account;
	COptComboBox *m_TransType;
	COptComboBox *m_FutureType;
	COptComboBox *m_Currency;
	CMaskDateEdit m_FromDate;
	CMaskDateEdit m_ToDate;
	CNumEdit m_Fees;
	CString m_RowID;

protected:
	void EnableCtrls();
	BOOL IsOK(int nAction);
	void RowToControls();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CCountry)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void DblClickFfeeList(long Col, long Row);
	void BlockSelectedFfeeList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnBnClickedFfeeLoadButton();
	afx_msg void OnBnClickedFfeeAddButton();
	afx_msg void OnBnClickedFfeeUpdateButton();
	afx_msg void OnBnClickedFfeeDeleteButton();
};
#endif
