#ifndef __COUPONDLG_H__
#define __COUPONDLG_H__

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
// CCouponDlg dialog

class CCouponDlg : public CDialog
{
// Construction
public:
	CCouponDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCouponDlg)
	enum { IDD = IDD_COUPON_DIALOG };
	COraLoader m_OraLoader;
	CDBSheet m_SS;
	CDBSheet m_sSS;

	CString m_Portfolio;
	CString m_Asset;
	CString m_Currency;
	CString m_Date;
	CString m_TransNum;
	CString m_LAmount;
	CString m_Amount;
	CString m_Fxrate;
	CString m_Remaining;
	CNumEdit m_FxrateEdit;
	CNumEdit m_AmountEdit;
	CNumEdit m_RemainingEdit;
	CMaskDateEdit m_DateEdit;
	int m_CouponType;
	BOOL m_bLoaded;
	BOOL m_bCash;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCouponDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateNumber(BOOL bFxChange);

	// Generated message map functions
	//{{AFX_MSG(CCouponDlg)
	afx_msg void OnDblClickCouponList(long Col, long Row);
	afx_msg void OnDblClickCouponInfoList(long Col, long Row);
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusCouponAmountEdit();
	afx_msg void OnKillfocusCouponFxrateEdit();
	virtual void OnOK();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
