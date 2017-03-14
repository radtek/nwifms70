#ifndef __SHORTCASH_H__
#define __SHORTCASH_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShortCash.h : header file
//

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

#ifndef __MASKDATEEDIT_H__
#include "maskdateedit.h"
#endif

#ifndef __SHORTCASHDATA_H__
#include "shortcashdata.h"
#endif
/////////////////////////////////////////////////////////////////////////////
// CShortCash dialog

class CShortCash : public CDialog
{
// Construction
public:
	CShortCash(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CShortCash)
	enum { IDD = IDD_CASH_SHORT_DIALOG };
	CDocData *m_pData;
	CShortCashData m_Data;
	CDBSheet m_SS;
	CMaskDateEdit m_DateEdit;
	CNumEdit m_AmountEdit;
	COptComboBox* m_PortfolioCombo;
	COptComboBox* m_AccountCombo;
	COptComboBox* m_CurrencyCombo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShortCash)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void EnableCtrls();

	// Generated message map functions
	//{{AFX_MSG(CShortCash)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCashShortAccountCombo();
	afx_msg void OnCashShortAdd();
	afx_msg void OnKillfocusCashShortAmountEdit();
	afx_msg void OnSelchangeCashShortCurrencyCombo();
	afx_msg void OnKillfocusCashShortDateEdit();
	afx_msg void OnCashShortDelete();
	afx_msg void OnCashShortFind();
	afx_msg void OnBlockSelectedCashShortList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickCashShortList(long Col, long Row);
	afx_msg void OnSelchangeCashShortPortfolioCombo();
	afx_msg void OnCashShortUpdate();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif