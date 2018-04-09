#ifndef __CONFIRMVIEW_H__
#define __CONFIRMVIEW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfirmView.h : header file
//
#ifndef __NWIFMS70DOC_H__
#include "nwifms70doc.h"
#endif

#ifndef __CONFIRMDATA_H__
#include "confirmdata.h"
#endif

#ifndef __MASKDATEEDIT_H__
#include "maskdateedit.h"
#endif

#ifndef __MASKIDEDIT_H__
#include "maskidedit.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfirmView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CConfirmView : public CFormView
{
protected:
	CConfirmView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CConfirmView)

// Form Data
public:
	//{{AFX_DATA(CConfirmView)
	enum { IDD = IDD_CONFIRM_FORM };
	CConfirmData m_Data;
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
	CCheckBox m_ShortSale;
	CCheckBox m_CSPBShort;
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

	CMCComboBox m_Dir;
	CMCComboBox m_Trader;
	CMCComboBox m_Index;
	CMCComboBox m_OptSet;
	CMCComboBox m_OptSet2;
	CMCComboBox m_ETrade;

	CCntlEdit m_Note;
	CCntlEdit m_Note2;
	CCntlEdit m_Asset;
	CCntlEdit m_AssetDesc;
	CCntlEdit m_AssetCurr;
	CCntlEdit m_CT;
	CCntlEdit m_OptTicker;
	CCntlEdit m_SetConvention;
	CCntlEdit m_OptID;
	CCntlEdit m_CPTradeID;
	CCntlEdit m_SwapTicket;
	CCntlEdit m_Binary;

	CMaskIDEdit m_Ticket;
	CMaskIDEdit m_TransNum;
	CMaskIDEdit m_Link;
	CMaskIDEdit m_Seq;
	CMaskIDEdit m_CancelTicket;
	CMaskIDEdit m_CorrectTicket;

	CMaskDateEdit m_TradeDate;
	CMaskDateEdit m_ValueDate;
	CMaskDateEdit m_Maturity;
	CMaskDateEdit m_OriBooking;
	CMaskDateEdit m_OriMaturity;
	CMaskDateEdit m_ExerciseDate;
	CMaskDateEdit m_OptExp;
	CMaskDateEdit m_DeliveryDate;
	CMaskDateEdit m_FXDate;

	CNumEdit m_Amount;
	CNumEdit m_Fxrate;
	CNumEdit m_BrFee;
	CNumEdit m_SoftDollar;
	CNumEdit m_OtherFee;
	CNumEdit m_VAR;
	CNumEdit m_Rate;
	CNumEdit m_Price;
	CNumEdit m_Strike;
	CNumEdit m_Margin;
	CNumEdit m_MarginAmount;
	CNumEdit m_DV01;
	CNumEdit m_IMFxrate;

	CCntlEdit m_InvAsset;
	CCntlEdit m_InvAssignCT;
	CCntlEdit m_RecCurrency;

	CMaskDateEdit m_InvValueDate;
	CMaskDateEdit m_InvCloseDate;
	CMaskDateEdit m_InvPostDate;

	CMaskIDEdit m_InvForexRef;
	CNumEdit m_InvCrossRate;	
	CNumEdit m_InvAmount;
	CNumEdit m_InvCash;
	CNumEdit m_InvInterest;
	CNumEdit m_RecInterest;
	CNumEdit m_RecAccretion;
	CNumEdit m_RecCash;
	CNumEdit m_RecTotal;

	COptComboBox m_InvTrDesc;
	COptComboBox m_InvAccount;
	COptComboBox m_InvCurrency;
	COptComboBox m_InvAssignCP;

	UINT m_nCurrID;
	int m_Mode;
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

public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfirmView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CConfirmView() {}
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CConfirmView)
	afx_msg void OnBlockSelectedConfirmInvList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickConfirmInvList(long Col, long Row);
	afx_msg void OnBlockSelectedConfirmTicketList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickConfirmTicketList(long Col, long Row);
	afx_msg void OnSelchangeConfirmCpCombo();
	afx_msg void OnSelchangeConfirmTypeCombo();
	afx_msg void OnSelchangeConfirmDirCombo();
	afx_msg void OnSetfocusConfirmCtEdit();
	afx_msg void OnConfirmListedCheck();
	afx_msg void OnUpdateConfirmClearscreen(CCmdUI* pCmdUI);
	afx_msg void OnConfirmClearscreen();
	afx_msg void OnConfirmDelete();
	afx_msg void OnUpdateConfirmDelete(CCmdUI* pCmdUI);
	afx_msg void OnConfirmFindasset();
	afx_msg void OnUpdateConfirmFindasset(CCmdUI* pCmdUI);
	afx_msg void OnConfirmFindticket();
	afx_msg void OnUpdateConfirmFindticket(CCmdUI* pCmdUI);
	afx_msg void OnConfirmRefresh();
	afx_msg void OnUpdateConfirmRefresh(CCmdUI* pCmdUI);
	afx_msg void OnConfirmUpdate();
	afx_msg void OnUpdateConfirmUpdate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateConfirmInventory(CCmdUI* pCmdUI);
	afx_msg void OnConfirmFindItem();
	afx_msg void OnUpdateConfirmFindItem(CCmdUI* pCmdUI);
	afx_msg void OnSetfocusConfirmDealEdit();
	afx_msg void OnChangeConfirmTransNum();
	afx_msg void OnUpdateConfirmInvertPrice(CCmdUI* pCmdUI);
	afx_msg void OnConfirmInvertPrice();
	afx_msg void OnConfirmRepoPrice();
	afx_msg void OnUpdateConfirmRepoPrice(CCmdUI* pCmdUI);
	afx_msg void OnConfirmDailyRepoCheck();
	afx_msg void OnSetfocusConfirmInvAssetEdit();
	afx_msg void OnSetfocusConfirmAssetEdit();
	afx_msg void OnUpdateConfirmOptionExp(CCmdUI* pCmdUI);
	afx_msg void OnConfirmOptionExp();
	afx_msg void OnConfirmTrsCheck();
	afx_msg void OnSetfocusConfirmInvAssignctEdit();
	afx_msg void OnConfirmFindExactTicket();
	afx_msg void OnUpdateConfirmFindExactTicket(CCmdUI* pCmdUI);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnSetfocusConfirmInvForexRefEdit();
	afx_msg void OnEnSetfocusConfirmSetconventionEdit();
	afx_msg void OnEnKillfocusConfirmExercisedateEdit();
	afx_msg void OnEnKillfocusConfirmMaturityEdit();
	afx_msg void OnUpdateConfirmOtherfee(CCmdUI *pCmdUI);
	afx_msg void OnConfirmOtherfee();
	afx_msg void OnEnSetfocusConfirmBinaryEdit();
	afx_msg void OnEnChangeConfirmPriceEdit();
};

#ifndef _DEBUG  // debug version in ConfirmView.cpp
inline CNWIFMS70Doc* CConfirmView::GetDocument()
   { return (CNWIFMS70Doc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif