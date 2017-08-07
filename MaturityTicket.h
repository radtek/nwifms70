#ifndef __MATURITYTICKET_H__
#define __MATURITYTICKET_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FmsTicket.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFmsTicket form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#ifndef __NWIFMS70DOC_H__
#include "nwifms70doc.h"
#endif

#ifndef __FMSDATA_H__
#include "fmsdata.h"
#endif

#ifndef __MASKDATEEDIT_H__
#include "maskdateedit.h"
#endif

#ifndef __MASKIDDATE_H__
#include "maskidedit.h"
#endif

class CMaturityTicket : public CFormView
{
protected:
	CMaturityTicket();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMaturityTicket)

// Form Data
public:
	//{{AFX_DATA(CMaturityTicket)
	enum { IDD = IDD_TICKETMATURITY_FORM };
	CDBSheet m_SS;
	CDBSheet m_InvSS;

	CCheckBox m_TRS;
	CCheckBox m_DocRq;
	CCheckBox m_DocRecvd;
	CCheckBox m_Confirm;
		
    COptComboBox m_Portfolio;
	COptComboBox m_TransType;
	COptComboBox m_Currency;
    COptComboBox m_CP;
    COptComboBox m_RateBasis;
	COptComboBox m_Formula;
	COptComboBox m_Custodian;

	CMCComboBox m_Dir;
	CMCComboBox m_Trader;

	CMaskIDEdit m_Ticket;
	CMaskIDEdit m_TransNum;
	
	CMaskDateEdit m_TradeDate;
	CMaskDateEdit m_ValueDate;
	CMaskDateEdit m_Maturity;
	CMaskDateEdit m_OriBooking;
	CMaskDateEdit m_OriMaturity;

	CNumEdit m_Amount;
	CNumEdit m_Fxrate;
	CNumEdit m_BrFee;
	CNumEdit m_SoftDAmount;
	CNumEdit m_OtherFee;
	CNumEdit m_Price;
	CNumEdit m_Rate;

	CCntlEdit m_Note;
	CCntlEdit m_Note2;
	CCntlEdit m_Asset;
	CCntlEdit m_AssetDesc;
	CCntlEdit m_AssetCurr;
	CCntlEdit m_CT;
	CCntlEdit m_CPTradeID;

	CCntlEdit m_InvAsset;
	CMaskDateEdit m_InvValueDate;
	CMaskDateEdit m_InvCloseDate;
	CMaskDateEdit m_InvPostDate;
	CMaskIDEdit m_InvForexRef;
	CNumEdit m_InvCrossRate;	
	CNumEdit m_InvAmount;
	CNumEdit m_InvCash;
	CNumEdit m_InvInterest;
	COptComboBox m_InvTrDesc;
	COptComboBox m_InvAccount;
	COptComboBox m_InvCurrency;
	COptComboBox m_InvAssignCP;
	CCntlEdit m_InvAssignCT;

	CCntlEdit m_RecCurrency;
	CNumEdit m_RecInterest;
	CNumEdit m_RecAccretion;
	CNumEdit m_RecCash;
	CNumEdit m_RecTotal;

	COptComboBox m_SetCurrency;
	CNumEdit m_SetAmount;
	CNumEdit m_SetFx;
	CMaskDateEdit m_SetMaturity;

	CString m_InvNum;
	CString m_Processed;

	CAssetVal m_Val;
	//}}AFX_DATA

// Attributes
public:
	CNWIFMS70Doc* GetDocument();
	CDocData& GetData() { return GetDocument()->GetData(); }

// Operations
public:
	void ComputeValue();
	void InitControls();
	BOOL IsOK();
	void SetupAssetInfo();
	BOOL UpdateData(BOOL bSaveandValid = TRUE);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaturityTicket)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMaturityTicket() {}
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CMaturityTicket)
	afx_msg void OnDblClickMaturityTicketList(long Col, long Row);
	afx_msg void OnDblClickMaturityInvList(long Col, long Row);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedMaturityLoadButton();
	afx_msg void OnBnClickedMaturityProcessButton();
	afx_msg void OnCbnSelchangeMaturitySetCurrencyCombo();
	afx_msg void OnEnChangeMaturitySetcashEdit();
	afx_msg void OnMaturityLoadTrade();
	afx_msg void OnMaturityProcessTrade();
	afx_msg void OnUpdateMaturityProcessTrade(CCmdUI *pCmdUI);
	afx_msg void OnEnChangeMaturitySetfxEdit();
	afx_msg void OnEnChangeMaturitySetmaturityEdit();
};

#ifndef _DEBUG  // debug version in WFMSView.cpp
inline CNWIFMS70Doc* CMaturityTicket::GetDocument()
   { return (CNWIFMS70Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __MATURITYTICKET_H__
