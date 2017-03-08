#if !defined(AFX_PORTFOLIOINFO_H__6E419D33_86D7_11D7_A4C1_00062926CA4A__INCLUDED_)
#define AFX_PORTFOLIOINFO_H__6E419D33_86D7_11D7_A4C1_00062926CA4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PortfolioInfo.h : header file
//

#ifndef __PORTFOLIOINFODATA_H__
#include "portfolioinfodata.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CPortfolioInfo dialog

class CPortfolioInfo : public CDialog
{
// Construction
public:
	CPortfolioInfo(CWnd* pParent = NULL);   // standard constructor
	~CPortfolioInfo();

// Dialog Data
	//{{AFX_DATA(CPortfolioInfo)
	enum { IDD = IDD_PORTFOLIOINFO_DIALOG };
	CPortfolioInfoData m_Data;
	COraLoader m_OraLoader;
	CDBSheet m_SS;
	COptComboBox *m_Portfolio;
	CCntlEdit m_Custodian;
	CCntlEdit m_CustodianAccount;
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
	//{{AFX_VIRTUAL(CPortfolioInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void EnableCtrls();

	// Generated message map functions
	//{{AFX_MSG(CPortfolioInfo)
	virtual BOOL OnInitDialog();
	afx_msg void OnPortfolioinfoAddButton();
	afx_msg void OnPortfolioinfoDeleteButton();
	afx_msg void OnPortfolioinfoUpdateButton();
	afx_msg void OnPortfolioinfoFindButton();
	afx_msg void OnPortfolioinfoClearButton();
	afx_msg void OnDblClickPortfolioInfoList(long Col, long Row);
	afx_msg void OnBlockSelectedPortfolioInfoList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangePortfolioinfoAccount2Edit();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PORTFOLIINFO_H__6E419D33_86D7_11D7_A4C1_00062926CA4A__INCLUDED_)
