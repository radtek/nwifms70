#ifndef __CASHFORM_H__
#define __CASHFORM_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CashForm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCashForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#ifndef __NWIFMS70DOC_H__
#include "nwifms70doc.h"
#endif

#ifndef __CASHDATA_H__
#include "cashdata.h"
#endif

#ifndef __MASKDATEEDIT_H__
#include "maskdateedit.h"
#endif

class CCashForm : public CFormView
{
protected:
	CCashForm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCashForm)

// Form Data
public:
	//{{AFX_DATA(CCashForm)
	enum { IDD = IDD_CASH_FORM };
	CCashData m_Data;
	CDBSheet m_SS;
	CDBSheet m_InvSS;
	CDBSheet m_DataSS;

	CCheckBox *m_CouponPaid;

	CMaskIDEdit m_CashNum;
	CMaskIDEdit m_PayID;
	CMaskIDEdit m_Link;
	CMaskIDEdit m_IRSID;

	CMaskDateEdit m_CloseDate;
	CMaskDateEdit m_PostDate;
	CMaskDateEdit m_ToDate;

	COptComboBox *m_Portfolio;
	COptComboBox *m_Account;
	COptComboBox *m_Currency;
	COptComboBox *m_PayType;
	COptComboBox *m_DiffReason;
	COptComboBox *m_AccountTo;
	COptComboBox *m_PortfolioTo;

	CCntlEdit m_CustRef;
	CCntlEdit m_Note;
	CCntlEdit m_Asset;

	CNumEdit m_Amount;
	CNumEdit m_Fxrate;
	CNumEdit m_Remaining;
	CString m_UserClass;

	CString m_Fund;
	CStringArray m_DefAccounts;
	CString m_CashAssetCode;
	CString m_InvNum;
	BOOL m_bFind;
	BOOL m_bInCurrMonth;
	BOOL m_bSameDate;
	//}}AFX_DATA

// Attributes
public:
	CNWIFMS70Doc* GetDocument();
	CDocData& GetData() { return GetDocument()->GetData(); }

// Operations
public:
	void InitControls();
	void EnableControls();
	void SetInCurrMonth();
	void SetSameDate();
	BOOL UpdateData(BOOL bSaveandValid = TRUE);
	void UpdateTotalCash(long Row);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCashForm)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCashForm();
	BOOL IsOK();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CCashForm)
	afx_msg void OnCashAdd();
	afx_msg void OnCashClear();
	afx_msg void OnCashDelete();
	afx_msg void OnCashFind();
	afx_msg void OnCashFindAsset();
	afx_msg void OnCashLoad();
	afx_msg void OnCashLoadCash();
	afx_msg void OnCashRefresh();
	afx_msg void OnCashUpdate();
	afx_msg void OnBlockSelectedCashList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickCashList(long Col, long Row);
	afx_msg void OnDblClickCashMapList(long Col, long Row);
	afx_msg void OnDblClickCashTradeList(long Col, long Row);
	afx_msg void OnUpdateCashAdd(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCashDelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCashUpdate(CCmdUI* pCmdUI);
	afx_msg void OnCashShortCash();
	afx_msg void OnUpdateCashFind(CCmdUI* pCmdUI);
	afx_msg void OnSelchangeCashPortfolioCombo();
	afx_msg void OnUpdateCashLoadCoupon(CCmdUI* pCmdUI);
	afx_msg void OnCashLoadCoupon();
	afx_msg void OnUpdateCashSwap(CCmdUI* pCmdUI);
	afx_msg void OnCashSwap();
	afx_msg void OnUpdateCashErrorlink(CCmdUI* pCmdUI);
	afx_msg void OnCashErrorlink();
	afx_msg void OnUpdateCashLoad(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCashLoadCash(CCmdUI* pCmdUI);
	afx_msg void OnSelchangeCashTypeCombo();
	afx_msg void OnChangeCashAmountEdit();
	afx_msg void OnCashLoadcdscoupon();
	afx_msg void OnUpdateCashLoadcdscoupon(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCashClear(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCashFindAsset(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCashRefresh(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCashShortCash(CCmdUI* pCmdUI);
	afx_msg void OnCashFindExact();
	afx_msg void OnUpdateCashFindExact(CCmdUI* pCmdUI);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCashLoadirscoupon();
	afx_msg void OnUpdateCashLoadirscoupon(CCmdUI *pCmdUI);
	afx_msg void OnCashUploadcashentry();
};

#ifndef _DEBUG  // debug version in WFMSView.cpp
inline CNWIFMS70Doc* CCashForm::GetDocument()
   { return (CNWIFMS70Doc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __CASHFORM_H__
