#ifndef __FMSTICKET_H__
#define __FMSTICKET_H__

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

class CFmsTicket : public CFormView
{
protected:
	CFmsTicket();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CFmsTicket)

// Form Data
public:
	//{{AFX_DATA(CFmsTicket)
	enum { IDD = IDD_TICKET_FORM };
	CFmsTicketData m_Data;
	CDBSheet m_SS;
	CDBSheet m_InvSS;

	CCheckBox m_TRS;
	CCheckBox m_IPO;
	CCheckBox m_EuroOpt;
	CCheckBox m_Listed;
	CCheckBox m_DocRq;
	CCheckBox m_DocRecvd;
	CCheckBox m_WI;
	CCheckBox m_Confirm;
	CCheckBox m_OptAuto;
	CCheckBox m_Funded;
	CCheckBox m_CSPBShort;
	CCheckBox m_ShortSale;
	CCheckBox m_SecFee;
	CCheckBox m_OrFee;
	
    COptComboBox m_Portfolio;
    COptComboBox m_TransType;
	COptComboBox m_CP;
    COptComboBox m_PFU;
	COptComboBox m_Currency;
    COptComboBox m_RateBasis;
	COptComboBox m_Formula;
	COptComboBox m_Custodian;
    COptComboBox m_USDAcct;
    COptComboBox m_FxAcct;
	COptComboBox m_MarginCurrency;
	COptComboBox m_BestExecution;

	CMaskIDEdit m_Ticket;
	CMaskDateEdit m_TradeDate;
	CMaskDateEdit m_ValueDate;
	CNumEdit m_Amount;
	CNumEdit m_Fxrate;
	CNumEdit m_BrFee;
	CNumEdit m_SoftDollar;
	CNumEdit m_OtherFee;
	CNumEdit m_VAR;
	CNumEdit m_IMFxrate;
	CCntlEdit m_Note;
	CCntlEdit m_Note2;
	CCntlEdit m_Binary;
	CNumEdit m_Rate;
	CMaskDateEdit m_Maturity;
	CNumEdit m_Price;
	CCntlEdit m_Asset;
	CCntlEdit m_AssetDesc;
	CCntlEdit m_AssetCurr;
	CCntlEdit m_CT;
	CMaskDateEdit m_OriBooking;
	CMaskDateEdit m_OriMaturity;
	CMaskDateEdit m_ExerciseDate;
	CMaskDateEdit m_OptExp;
	CMaskDateEdit m_FXDate;
	CMaskDateEdit m_DeliveryDate;
	CNumEdit m_Strike;
	CCntlEdit m_OptTicker;
	CCntlEdit m_SetConvention;
	CCntlEdit m_OptID;

	CMCComboBox m_Dir;
	CMCComboBox m_Trader;
	CMCComboBox m_Index;
	CMCComboBox m_OptSet;
	CMCComboBox m_OptSet2;
	CMCComboBox m_ETrade;

	CNumEdit m_Margin;
	CNumEdit m_MarginAmount;
	CNumEdit m_DV01;
	CCntlEdit m_SwapTicket;
	CCntlEdit m_CPTradeID;
	
	CMaskIDEdit m_TransNum;
	CMaskIDEdit m_CancelTicket;
	CMaskIDEdit m_CorrectTicket;
	CMaskIDEdit m_Link;
	CMaskIDEdit m_Seq;

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
	UINT m_nCurrID;
	int m_Mode;
	CString m_Port;
	CString m_Rev;
	BOOL m_bInCurrMonth;
	BOOL m_bSameDate;
	//}}AFX_DATA

// Attributes
public:
	CNWIFMS70Doc* GetDocument();
	CDocData& GetData() { return GetDocument()->GetData(); }

// Operations
public:
	void ChangeShortLabel();
	void EnableCtrls();
	void EnableInvCtrls();
	void InitControls();
	BOOL IsOK();
	void SetInCurrMonth();
	void SetMode(int Mode);
	void SetSameDate();
	void UpdateCPCT(COptComboBox &CP, CCntlEdit &CT);
	BOOL UpdateData(BOOL bSaveandValid = TRUE);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFmsTicket)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFmsTicket() {}
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CFmsTicket)
	afx_msg void OnSelchangeTicketPortfolioCombo();
	afx_msg void OnSelchangeTicketTypeCombo();
	afx_msg void OnSelchangeTicketDirCombo();
	afx_msg void OnSetfocusTicketCtEdit();
	afx_msg void OnTicketListedCheck();
	afx_msg void OnBlockSelectedTicketTicketList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickTicketTicketList(long Col, long Row);
	afx_msg void OnTransAdd();
	afx_msg void OnUpdateTransAdd(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTransClearscreen(CCmdUI* pCmdUI);
	afx_msg void OnTransClearscreen();
	afx_msg void OnTransCopy();
	afx_msg void OnUpdateTransCopy(CCmdUI* pCmdUI);
	afx_msg void OnTransDelete();
	afx_msg void OnUpdateTransDelete(CCmdUI* pCmdUI);
	afx_msg void OnTransFindasset();
	afx_msg void OnUpdateTransFindasset(CCmdUI* pCmdUI);
	afx_msg void OnTransFindticket();
	afx_msg void OnUpdateTransFindticket(CCmdUI* pCmdUI);
	afx_msg void OnTransRefresh();
	afx_msg void OnUpdateTransRefresh(CCmdUI* pCmdUI);
	afx_msg void OnTransUpdate();
	afx_msg void OnUpdateTransUpdate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTransInventory(CCmdUI* pCmdUI);
	afx_msg void OnBlockSelectedTicketInvList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickTicketInvList(long Col, long Row);
	afx_msg void OnTransFindItem();
	afx_msg void OnUpdateTransFindItem(CCmdUI* pCmdUI);
	afx_msg void OnSetfocusTicketDealEdit();
	afx_msg void OnChangeTicketTransNum();
	afx_msg void OnUpdateTransInvertPrice(CCmdUI* pCmdUI);
	afx_msg void OnTransInvertPrice();
	afx_msg void OnTransRepoPrice();
	afx_msg void OnUpdateTransRepoPrice(CCmdUI* pCmdUI);
	afx_msg void OnTicketDailyRepoCheck();
	afx_msg void OnSetfocusTicketInvAssetEdit();
	afx_msg void OnSetfocusTicketAssetEdit();
	afx_msg void OnUpdateTransOptionExp(CCmdUI* pCmdUI);
	afx_msg void OnTransOptionExp();
	afx_msg void OnTicketTrsCheck();
	afx_msg void OnSetfocusTicketInvAssignctEdit();
	afx_msg void OnTransXcurrency();
	afx_msg void OnTransFindExactTicket();
	afx_msg void OnUpdateTransFindExactTicket(CCmdUI* pCmdUI);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnSetfocusTicketInvForexRefEdit();
	afx_msg void OnEnSetfocusTicketSetconventionEdit();
	afx_msg void OnUpdateTransOtherfee(CCmdUI *pCmdUI);
	afx_msg void OnTransOtherfee();
	afx_msg void OnEnSetfocusTicketBinaryEdit();
	afx_msg void OnTransCc();
	afx_msg void OnUpdateTransCc(CCmdUI *pCmdUI);
	afx_msg void OnEnChangeTicketPriceEdit();
};

#ifndef _DEBUG  // debug version in WFMSView.cpp
inline CNWIFMS70Doc* CFmsTicket::GetDocument()
   { return (CNWIFMS70Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __FMSTICKET_H__
