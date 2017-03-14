#ifndef __PORTFOLIO_H__
#define __PORTFOLIO_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Portfolio.h : header file
//

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __PORTFOLIODATA_H__
#include "portfoliodata.h"
#endif

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif
/////////////////////////////////////////////////////////////////////////////
// CPortfolio dialog

class CPortfolio : public CDialog
{
// Construction
public:
	CPortfolio(CWnd* pParent = NULL);   // standard constructor
	~CPortfolio();

// Dialog Data
	//{{AFX_DATA(CPortfolio)
	enum { IDD = IDD_PORTFOLIO_DIALOG };
	CPortfolioData m_Data;
	CDocData *m_pData;
	CDBSheet m_SS;
	CCntlEdit m_Portfolio;
	CCntlEdit m_Name;
	CCntlEdit m_Accountant;
	CNumEdit m_Amount;
	CNumEdit m_CtrlFactor;
	CNumEdit m_WtFactor;
	COptComboBox *m_TableSpace;
	COptComboBox *m_DefCustodian;
	COptComboBox *m_DefAccount;
	COptComboBox *m_Currency;
	CCheckBox *m_Status;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPortfolio)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void EnableCtrls();

	// Generated message map functions
	//{{AFX_MSG(CPortfolio)
	afx_msg void OnPortfolioAddButton();
	afx_msg void OnPortfolioClearButton();
	afx_msg void OnPortfolioDeleteButton();
	afx_msg void OnPortfolioFindButton();
	afx_msg void OnPortfolioUpdateButton();
	afx_msg void OnDblClickPortfolioList(long Col, long Row);
	afx_msg void OnBlockSelectedPortfolioList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	virtual BOOL OnInitDialog();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
