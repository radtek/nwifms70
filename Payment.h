#ifndef __PAYMENT_H__
#define __PAYMENT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Payment.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPayment form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#ifndef __NWIFMS70DOC_H__
#include "nwifms70doc.h"
#endif

#ifndef __PAYMENTDATA_H__
#include "paymentdata.h"
#endif

class CPayment : public CFormView
{
protected:
	CPayment();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPayment)

// Form Data
public:
	//{{AFX_DATA(CPayment)
	enum { IDD = IDD_PAYMENT };
	CPaymentData m_Data;
	CDBSheet m_SS;
	CDBSheet m_InstSS;
	CDBSheet m_OT_SS;
	CDBSheet m_Sel_SS;

	CMaskIDEdit m_PayID;
	CMaskDateEdit m_Date;
	CMaskDateEdit m_ValueDate;
	CCntlEdit m_RefID;
	COptComboBox *m_Portfolio;
	COptComboBox *m_PMAccount;
	COptComboBox *m_CP;
	COptComboBox *m_Dir;
	COptComboBox *m_Currency;
	COptComboBox *m_CashType;
	COptComboBox *m_ToAccount;
	CNumEdit m_Amount;
	CNumEdit m_Adjust;
	CNumEdit m_Fxrate;
	CCntlEdit m_Note;
	CCntlEdit m_AssetCode;
	CString m_StrDate;

	CNumEdit m_Sum;
	CCntlEdit m_Bank;
	CCntlEdit m_BankID;
	CCntlEdit m_BankBIC;
	CCntlEdit m_Account;
	CCntlEdit m_Beneficiary;
	CCntlEdit m_BeneficiaryID;
	CCntlEdit m_BeneficiaryBIC;
	CCntlEdit m_BeneficiaryAccount;
	CCntlEdit m_FFC;
	CCntlEdit m_FFCID;
	CCntlEdit m_FFCBIC;
	CCntlEdit m_FFCAccount;
	CCntlEdit m_CPRef;

	CCheckBox *m_Paid;
	int m_nCurrID;
	CString m_Deletable;
	//}}AFX_DATA

// Attributes
public:
	CNWIFMS70Doc *GetDocument();
	CDocData &GetData() { return GetDocument()->m_Data; }
	BOOL UpdateData(BOOL bSaveAndValidate = TRUE);
	void UpdateSum(BOOL bAdjustment = FALSE);
	BOOL FindInvNum(LPCTSTR InvNum);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPayment)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPayment();
	void Init();
	void LoadCoupon(int nType);
	BOOL IsOK();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPayment)
	afx_msg void OnDblClickPayOpentradeList(long Col, long Row);
	afx_msg void OnDblClickPaySelTradeList(long Col, long Row);
	afx_msg void OnDblClickPayInstructionList(long Col, long Row);
	afx_msg void OnChangePayAmountEdit();
	afx_msg void OnPayClearscreen();
	afx_msg void OnPayFind();
	afx_msg void OnPayLoadOpentrade();
	afx_msg void OnPayAdd();
	afx_msg void OnPayUpdate();
	afx_msg void OnUpdatePayUpdate(CCmdUI* pCmdUI);
	afx_msg void OnPayDelete();
	afx_msg void OnUpdatePayDelete(CCmdUI* pCmdUI);
	afx_msg void OnSelchangePayTypeCombo();
	afx_msg void OnUpdatePayLoadOpentrade(CCmdUI* pCmdUI);
	afx_msg void OnBlockSelectedPayList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickPayList(long Col, long Row);
	afx_msg void OnPayReport();
	afx_msg void OnUpdatePayReport(CCmdUI* pCmdUI);
	afx_msg void OnPayClearall();
	afx_msg void OnPayFindAsset();
	afx_msg void OnUpdatePayFindAsset(CCmdUI* pCmdUI);
	afx_msg void OnSetfocusPayAssetEdit();
	afx_msg void OnKillfocusPayAssetEdit();
	afx_msg void OnSelchangePayPortfolioCombo();
	afx_msg void OnUpdatePayLoadCoupon(CCmdUI* pCmdUI);
	afx_msg void OnPayLoadCoupon();
	afx_msg void OnPayLoadCdsCoupon();
	afx_msg void OnUpdatePayLoadCdsCoupon(CCmdUI* pCmdUI);
	afx_msg void OnSelchangePayCashtypeCombo();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPayLoadIrsCoupon();
	afx_msg void OnUpdatePayLoadIrsCoupon(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in CTicketProcess.cpp
inline CNWIFMS70Doc* CPayment::GetDocument()
   { return (CNWIFMS70Doc*) m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __PAYMENT_H__
