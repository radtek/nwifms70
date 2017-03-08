#if !defined(AFX_CASHIRSIDDLG_H__C00975F6_3838_4998_9DBB_AA613E515743__INCLUDED_)
#define AFX_CASHIRSIDDLG_H__C00975F6_3838_4998_9DBB_AA613E515743__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CashIRSIDDlg.h : header file
//

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CCashIRSIDDlg dialog

class CCashIRSIDDlg : public CDialog
{
// Construction
public:
	CCashIRSIDDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCashIRSIDDlg)
	enum { IDD = IDD_CASH_IRS_DIALOG };
	COraLoader m_OraLoader;
	CDBSheet m_SS;

	CString m_Portfolio;
	CString m_PayType;
	CString m_Currency;
	CString m_TransNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCashIRSIDDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCashIRSIDDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblClickCashirsList(long Col, long Row);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CASHIRSIDDLG_H__C00975F6_3838_4998_9DBB_AA613E515743__INCLUDED_)
