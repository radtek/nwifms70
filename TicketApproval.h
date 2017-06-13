#ifndef __TICKETAPPROVAL_H__
#define __TICKETAPPROVAL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TicketApproval.h : header file
//

#ifndef __NWIFMS70DOC_H__
#include "nwifms70doc.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CTicketApproval form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#ifndef __TICKETAPPROVALDATA_H__
#include "ticketapprovaldata.h"
#endif

class CTicketApproval : public CFormView
{
protected:
	CTicketApproval();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTicketApproval)

// Form Data
public:
	//{{AFX_DATA(CTicketApproval)
	enum { IDD = IDD_TICKETAPPROVAL_FORM };
	CTicketApprovalData m_Data;
	CDBSheet m_SS;
	CDBSheet m_AllocSS;
	CDBSheet m_PositionSS;
	CCheckBox m_Listed;
	CCheckBox m_EuroOpt;
    CCheckBox m_TRS;
    CCheckBox m_IPO;
	CCheckBox m_WI;
	CCheckBox m_Funded;
	CCheckBox m_OptAuto;
	CCheckBox m_ShortSale;

	CCntlEdit m_Ticket;
	CCntlEdit m_Sign;
	CCntlEdit m_Trader;
	CCntlEdit m_TradeDate;
	CCntlEdit m_ExecTime;
	CCntlEdit m_DateStamp;
	CCntlEdit m_ValueDate;
	CCntlEdit m_Dir;
	CCntlEdit m_TransType;
	CCntlEdit m_Amount;
	CCntlEdit m_Strike;
	CCntlEdit m_OptExp;
	CCntlEdit m_OptTicker;
	CCntlEdit m_OptSet;
	CCntlEdit m_OptSet2;
	CCntlEdit m_OptID;
	CCntlEdit m_CP;
	CCntlEdit m_Currency;
	CCntlEdit m_Fxrate;
	CCntlEdit m_RepoCP;
	CCntlEdit m_Rate;
	CCntlEdit m_RateBasis;
	CCntlEdit m_Maturity;
	CCntlEdit m_SWBooking;
	CCntlEdit m_SWMaturity;
	CCntlEdit m_FloatRate;
	CCntlEdit m_FloatBasis;
	CCntlEdit m_FloatMaturity;
	CCntlEdit m_Price;
	CCntlEdit m_GiveupCP;
	CCntlEdit m_Note;
	CCntlEdit m_Note2;
	CCntlEdit m_SNote;
	CCntlEdit m_AAMethod;
	CCntlEdit m_AAFReason;
	CCntlEdit m_Booker;
	CCntlEdit m_Asset;
	CCntlEdit m_AssetDesc;
	CCntlEdit m_AssetCurrency;
	CCntlEdit m_Country;
	CCntlEdit m_Industry;
	CCntlEdit m_Common;
	CCntlEdit m_Cusip;
	CCntlEdit m_Isin;
	CCntlEdit m_Sedol;
	CCntlEdit m_Class;
	CCntlEdit m_Category;
	CCntlEdit m_Bucket;
	CCntlEdit m_Coupon;
	CCntlEdit m_AssetMaturity;
	CCntlEdit m_VARStatus;
	CCntlEdit m_BestExecution;
	CCntlEdit m_Ratio;
	CCntlEdit m_Binary;
	CCntlEdit m_ETrade;

	CNumEdit m_PrevVAR;
	CNumEdit m_CurrVAR;
	CNumEdit m_VARLimit;
	CString m_CurrAsset;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	CNWIFMS70Doc* GetDocument();
	CDocData &GetData() { return GetDocument()->GetData(); }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTicketApproval)
public:
	virtual void OnInitialUpdate();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTicketApproval() {}
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	void InitControls();
	void LoadSNote();
	BOOL UpdateData(BOOL bSaveAndValid = TRUE);

protected:
	// Generated message map functions
	//{{AFX_MSG(CTicketApproval)
	afx_msg void OnApprovalSignButton();
	afx_msg void OnDblClickApprovalTicketList(long Col, long Row);
	afx_msg void OnApprovalLoad();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void OnDblClickApprovalAllocationList(long Col, long Row);
};

#ifndef _DEBUG  // debug version in TicketApproval.cpp
inline CNWIFMS70Doc* CTicketApproval::GetDocument()
   { return (CNWIFMS70Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __TICKETAPPROVAL_H__
