#ifndef __TICKETBLOCK_H__
#define __TICKETBLOCK_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TicketBlock.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTicketBlock form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#ifndef __NWIFMS70DOC_H__
#include "nwifms70doc.h"
#endif

#ifndef __BLOCKDATA_H__
#include "blockdata.h"
#endif

#ifndef __MASKDATEEDIT_H__
#include "maskdateedit.h"
#endif

#ifndef __MASKIDEDIT_H__
#include "maskidedit.h"
#endif

class CTicketBlock : public CFormView
{
protected:
	CTicketBlock();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTicketBlock)

// Form Data
public:
	//{{AFX_DATA(CTicketBlock)
	enum { IDD = IDD_TICKETBLOCK_FORM };
	CBlockData m_Data;
	CDBSheet m_SS;

	CCheckBox *m_Listed;
	CCheckBox *m_EuroOpt;
	CCheckBox *m_Trs;
	CCheckBox *m_IPO;
	CCheckBox *m_WI;
	CCheckBox *m_OptAuto;
	CCheckBox *m_Funded;
	CCheckBox *m_ShortSale;

	CMaskIDEdit m_Ticket;
	CMaskIDEdit m_SwapTicket;
	CMaskIDEdit m_UnWindTicket;
	CMaskIDEdit m_CancelTicket;
	CMaskIDEdit m_CorrectTicket;

	CCntlEdit m_OptTicker;
	CCntlEdit m_OptID;
	CCntlEdit m_Note;
	CCntlEdit m_Note2;
	CCntlEdit m_Asset;
	CCntlEdit m_AssetDesc;
	CCntlEdit m_AssetCurrency;
	CCntlEdit m_OptSet;
	CCntlEdit m_OptSet2;
	CCntlEdit m_VARStatus;
	CCntlEdit m_Ratio;
	CCntlEdit m_Binary;

	CMaskDateEdit m_TradeDate;
	CMaskDateEdit m_ValueDate;
	CMaskDateEdit m_OptExp;
	CMaskDateEdit m_Maturity;
	CMaskDateEdit m_SWBooking;
	CMaskDateEdit m_SWMaturity;
	CMaskDateEdit m_FloatMaturity;
	CMaskDateEdit m_DeliveryDate;
	CNumEdit m_Amount;
	CNumEdit m_Strike;
	CNumEdit m_FxRate;
	CNumEdit m_Rate;
	CNumEdit m_FloatRate;
	CNumEdit m_Price;
	CNumEdit m_DownPymnt;
	CNumEdit m_BrokerFee;
	CNumEdit m_SoftDollar;
	CNumEdit m_OtherFee;
	CNumEdit m_VAR;
	CNumEdit m_Margin;
	CNumEdit m_MarginAmount;
	CNumEdit m_DV01;

    COptComboBox *m_TransType;
    COptComboBox *m_CP;
    COptComboBox *m_Currency;
	COptComboBox *m_RepoCP;
	COptComboBox *m_RateBasis;
	COptComboBox *m_FloatRateBasis;
	COptComboBox *m_Portfolio;
	COptComboBox *m_AssignCP;
	COptComboBox *m_MarginCurrency;
	COptComboBox *m_BestExecution;

	CMCComboBox m_Dir;
	CMCComboBox m_Trader;
	double m_dNomAmount;
	unsigned int m_nNomID;
	int m_Try;
	UINT m_OptSetID;
	BOOL m_bLoaded;
	CString m_Future;
	//}}AFX_DATA

// Attributes
public:
	CNWIFMS70Doc *GetDocument();
	CDocData &GetData() { return GetDocument()->m_Data; }

// Operations
public:
	void EnableCtrls();
	void EnableRepoCtrls(BOOL bRepoCpSel);
	void InitControls();
	BOOL IsOK();
	BOOL UpdateData(BOOL bSaveandValid = TRUE);
	void UpdateNumber(CWnd* Ctrl, double Number, BOOL WholeNumber = FALSE);
	void UpdateRatio();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTicketBlock)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTicketBlock();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CTicketBlock)
	afx_msg void OnSelchangeTranstypeCombo();
	afx_msg void OnBlockSelectedTicketList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickTicketList(long Col, long Row);
	afx_msg void OnBlockAdd();
	afx_msg void OnBlockDelete();
	afx_msg void OnBlockUpdate();
	afx_msg void OnBlockRefresh();
	afx_msg void OnUpdateBlockUpdate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBlockDelete(CCmdUI* pCmdUI);
	afx_msg void OnSelchangeTransdirCombo();
	afx_msg void OnSelchangeCurrencyCombo();
	afx_msg void OnBlockFindAsset();
	afx_msg void OnUpdateBlockFindAsset(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBlockAdd(CCmdUI* pCmdUI);
	afx_msg void OnListedCheck();
	afx_msg void OnSetfocusAssetEdit();
	afx_msg void OnChangeAssetEdit();
	afx_msg void OnSelchangeLevsourceCombo();
	afx_msg void OnBlockCopyTicket();
	afx_msg void OnUpdateBlockCopyTicket(CCmdUI* pCmdUI);
	afx_msg void OnBlockBrokerfee();
	afx_msg void OnUpdateBlockRepoprice(CCmdUI* pCmdUI);
	afx_msg void OnBlockRepoprice();
	afx_msg void OnUpdateBlockBrokerfee(CCmdUI* pCmdUI);
	afx_msg void OnSetfocusAmountEdit();
	afx_msg void OnKillfocusDownpymntEdit();
	afx_msg void OnBlockLoadBlotter();
	afx_msg void OnBlockLoadTrade();
	afx_msg void OnBlockOptionExp();
	afx_msg void OnUpdateBlockOptionExp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBlockContractConvertor(CCmdUI* pCmdUI);
	afx_msg void OnBlockContractConvertor();
	afx_msg void OnBlockDealCheck();
	afx_msg void OnKillfocusAmountEdit();
	afx_msg void OnBlockXcurrency();
	afx_msg void OnSetfocusOptSetEdit();
	afx_msg void OnSetfocusOptSet2Edit();
	afx_msg void OnBlockSettlementtime();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CTicketBlock.cpp
inline CNWIFMS70Doc* CTicketBlock::GetDocument()
   { return (CNWIFMS70Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __TICKETBLOCK_H__
