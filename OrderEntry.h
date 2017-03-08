#ifndef __ORDERENTRY_H__
#define __ORDERENTRY_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TicketEntry.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTicketEntry form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#ifndef __NWIFMS70DOC_H__
#include "nwifms70doc.h"
#endif

#ifndef __ORDERDATA_H__
#include "orderdata.h"
#endif

#ifndef __RISKREC_H__
#include "RiskRec.h"
#endif

#ifndef __MASKDATEEDIT_H__
#include "maskdateedit.h"
#endif

#ifndef __MASKIDEDIT_H__
#include "maskidedit.h"
#endif

#ifndef __MASKMINTIMEEDIT_H__
#include "maskmintimeedit.h"
#endif

class COrderEntry : public CFormView
{
protected:
	COrderEntry();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(COrderEntry)

// Form Data
public:
	//{{AFX_DATA(COrderEntry)
	enum { IDD = IDD_TRADEORDER_FORM };
	COrderData m_Data;
	CRiskControl m_Risk;
	CDBSheet m_SS;

	CCheckBox *m_TRS;
	CCheckBox *m_IPO;
	CCheckBox *m_MarketOrder;
	CCheckBox *m_SameDay;
	CCheckBox *m_PartialFill;
	CCheckBox *m_Discretionary;
	CCheckBox *m_Expired;
	CCheckBox *m_Cancelled;

	CMaskIDEdit m_OrderID;

	CCntlEdit m_Asset;
	CCntlEdit m_AssetDesc;
	CCntlEdit m_AssetCurrency;
	CCntlEdit m_Note;

	CMaskDateEdit m_OrderDate;
	CMaskDateEdit m_Maturity;

	CNumEdit m_Fxrate;
	CNumEdit m_Amount;
	CNumEdit m_Strike;
	CNumEdit m_Rate;
	CNumEdit m_Price;

    COptComboBox *m_TransType;
    COptComboBox *m_CP;
    COptComboBox *m_Currency;
	COptComboBox *m_BestExecution;
//	COptComboBox *m_Reason;

	CMCComboBox m_Trader;
	CMCComboBox m_AAMethod;
	CMCComboBox m_Dir;

	BOOL m_bLoaded;
	int m_nRiskLevel;
	BOOL m_bWarned;
	//}}AFX_DATA

// Attributes
public:
	CNWIFMS70Doc *GetDocument();
	CDocData &GetData() { return GetDocument()->m_Data; }

// Operations
public:
	void EnableCtrls();
	void InitControls();
	BOOL IsOK();
	void ProcessVerifyRisk();
	BOOL UpdateData(BOOL bSaveandValid = TRUE);
	int VerifyRisk(CString &Text);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrderEntry)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~COrderEntry();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(COrderEntry)
	afx_msg void OnSelchangeTranstypeCombo();
	afx_msg void OnBlockSelectedOrderList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickOrderList(long Col, long Row);
	afx_msg void OnOrderAdd();
	afx_msg void OnOrderUpdate();
	afx_msg void OnOrderRefresh();
	afx_msg void OnSelchangeDirCombo();
	afx_msg void OnSelchangeCurrencyCombo();
	afx_msg void OnOrderFindAsset();
	afx_msg void OnIPOCheck();
	afx_msg void OnSetfocusAssetEdit();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnEnSetfocusEntryBinaryEdit();
	afx_msg void OnEntryFinditem();
	afx_msg void OnCbnSelchangeOrderCurrencyCombo();
	afx_msg void OnBnClickedOrderTrsCheck();
	afx_msg void OnOrderContract();
	afx_msg void OnCbnSelchangeOrderAaCombo();
	afx_msg void OnCbnKillfocusOrderAaCombo();
	afx_msg void OnEnChangeOrderAmountEdit();
	afx_msg void OnEnKillfocusOrderAmountEdit();
	afx_msg void OnEnChangeOrderPriceEdit();
	afx_msg void OnEnKillfocusOrderPriceEdit();
	afx_msg void OnEnChangeOrderAssetEdit();
	afx_msg void OnEnKillfocusOrderAssetEdit();
};

#ifndef _DEBUG  // debug version in COrderEntry.cpp
inline CNWIFMS70Doc* COrderEntry::GetDocument()
   { return (CNWIFMS70Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __ORDERENTRY_H__
