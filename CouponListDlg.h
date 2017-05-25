#ifndef __COUPONLISTDLG_H__
#define __COUPONLISTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CouponDlg.h : header file
//
#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __MASKDATEEDIT_H__
#include "maskdateedit.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CCouponListDlg dialog

class CCouponListDlg : public CDialog
{
// Construction
public:
	CCouponListDlg(CWnd* pParent = NULL);   // standard constructor
	~CCouponListDlg() {}

// Dialog Data
	//{{AFX_DATA(CCouponListDlg)
	enum { IDD = IDD_COUPONLIST_DIALOG };
	COraLoader m_OraLoader;
	CDBSheet m_SS;
	CDBSheet m_sSS;
	CDBSheet m_SelSS;

	CString m_Portfolio;
	CString m_Counterparty;
	CString m_Asset;
	CString m_Date;
	CCntlEdit m_AssetEdit;
	CNumEdit m_TotalEdit;
	CNumEdit m_TotalRemainingEdit;
	CNumEdit m_FxrateEdit;
	CNumEdit m_AmountEdit;
	CNumEdit m_RemainingEdit;
	CMaskDateEdit m_DateEdit;
	CCheckBox m_CouponPaid;
	CString m_Amount;
	CString m_Currency;
	CString m_Curr;
	CString m_LAmount;
	CString m_Remaining;
	int m_CouponType;
	BOOL m_bLoaded;
	BOOL m_bCash;
	CDBRecArray m_CouponArray;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCouponListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateFields();
	void UpdateNumber(BOOL bFxChange);

	// Generated message map functions
	//{{AFX_MSG(CCouponListDlg)
	afx_msg void OnDblClickCouponList(long Col, long Row);
	afx_msg void OnDblClickCouponSelList(long Col, long Row);
	afx_msg void OnDblClickCouponInfoList(long Col, long Row);
	afx_msg void OnKillfocusCouponListAmountEdit();
	afx_msg void OnKillfocusCouponListFxrateEdit(); 
	afx_msg void OnCouponListUpdateButton();
	afx_msg void OnCouponListRemoveButton();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
