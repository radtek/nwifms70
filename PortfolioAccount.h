#ifndef __PORTFOLIOACCOUNT_H__
#define __PORTFOLIOACCOUNT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PortfolioInfo.h : header file
//

#ifndef __PORTFOLIOACCOUNTDATA_H__
#include "portfolioaccountdata.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CPortfolioAccount dialog

class CPortfolioAccount : public CDialog
{
// Construction
public:
	CPortfolioAccount(CWnd* pParent = NULL);   // standard constructor
	~CPortfolioAccount();

// Dialog Data
	//{{AFX_DATA(CPortfolioAccount)
	enum { IDD = IDD_PORTFOLIOACCT_DIALOG };
	CPortfolioAccountData m_Data;
	COraLoader m_OraLoader;
	CDBSheet m_SS;
	COptComboBox *m_Portfolio;
	COptComboBox *m_CustodianCode;
	CCntlEdit m_Custodian;
	CCntlEdit m_CustodianAccount;
	CCntlEdit m_AccountNum;
	CCntlEdit m_Account;
	CCntlEdit m_Account2;
	CCntlEdit m_Bank;
	CNumEdit m_EuroClear;
	CCntlEdit m_Contact;
	CCntlEdit m_Phone;
	CCntlEdit m_Fax;
	CCheckBox *m_Margin;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPortfolioAccount)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void EnableCtrls();

	// Generated message map functions
	//{{AFX_MSG(CPortfolioAccount)
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangePortfolioAccountAccount2Edit();
	afx_msg void OnPortfolioAccountAddButton();
	afx_msg void OnPortfolioAccountDeleteButton();
	afx_msg void OnPortfolioAccountUpdateButton();
	afx_msg void OnPortfolioAccountFindButton();
	afx_msg void OnPortfolioAccountClearButton();
	afx_msg void OnDblClickPortfolioAccountList(long Col, long Row);
	afx_msg void OnBlockSelectedPortfolioAccountList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(__PORTFOLIOACCOUNT_H__)
