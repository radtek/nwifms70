#if !defined(AFX_ERRORENTRYLINKDLG_H__71A765D1_672F_45E2_8197_7A1B5FB63D58__INCLUDED_)
#define AFX_ERRORENTRYLINKDLG_H__71A765D1_672F_45E2_8197_7A1B5FB63D58__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ErrorEntryLinkDlg.h : header file
//

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CErrorEntryLinkDlg dialog

class CErrorEntryLinkDlg : public CDialog
{
// Construction
public:
	CErrorEntryLinkDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CErrorEntryLinkDlg)
	enum { IDD = IDD_ERROR_DIALOG };
	COraLoader m_OraLoader;
	CDBSheet m_SS;

	CString m_Portfolio;
	CString m_Account;
	CString m_Currency;
	CString m_CashNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CErrorEntryLinkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CErrorEntryLinkDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblClickErrorList(long Col, long Row);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ERRORENTRYLINKDLG_H__71A765D1_672F_45E2_8197_7A1B5FB63D58__INCLUDED_)
