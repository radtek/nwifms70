#if !defined(AFX_BANKDLG_H__D58E4FB0_5FCE_4945_B679_EFDFDFBC8FDD__INCLUDED_)
#define AFX_BANKDLG_H__D58E4FB0_5FCE_4945_B679_EFDFDFBC8FDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BankDlg.h : header file
//

#ifndef __SMALLRECDATA_H__
#include "smallrecdata.h"
#endif

#ifndef __CNTLEDIT_H__
#include "cntledit.h"
#endif
/////////////////////////////////////////////////////////////////////////////
// CBankDlg dialog

class CBankDlg : public CDialog
{
// Construction
public:
	CBankDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBankDlg)
	enum { IDD = IDD_BANK_DIALOG };
	CSmallRecData m_Data;
	CDBSheet m_SS;
	CCntlEdit m_BankEdit;
	CCntlEdit m_IDEdit;
	CCntlEdit m_ABAEdit;
	CCntlEdit m_BICEdit;
	BOOL m_bModified;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBankDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBankDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBlockSelectedBankList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickBankList(long Col, long Row);
	afx_msg void OnBankClearButton();
	afx_msg void OnBankDeleteButton();
	afx_msg void OnBankFindButton();
	afx_msg void OnBankUpdateButton();
	afx_msg void OnBankAddButton();
	afx_msg void OnChangeBankAbaEdit();
	afx_msg void OnChangeBankBicEdit();
	afx_msg void OnChangeBankIdEdit();
	afx_msg void OnChangeBankNameEdit();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BANKDLG_H__D58E4FB0_5FCE_4945_B679_EFDFDFBC8FDD__INCLUDED_)
