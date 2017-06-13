#ifndef __TICKETFXENTRY_H__
#define __TICKETFXENTRY_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TicketFXEntry.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTicketEntry form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#ifndef __NWIFMS70DOC_H__
#include "nwifms70doc.h"
#endif

#ifndef __ENTRYDATA_H__
#include "entrydata.h"
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

class CTicketFXEntry : public CFormView
{
protected:
	CTicketFXEntry();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTicketFXEntry)

// Form Data
public:
	//{{AFX_DATA(CTicketFXEntry)
	enum { IDD = IDD_TICKETFXENTRY_FORM };
	CEntryData m_Data;
	CDBSheet m_SS;
	CDBSheet m_AllocSS;

	CCheckBox m_Listed;
	CCheckBox m_EuroOpt;
	CCheckBox m_OptAuto;
	CCheckBox m_ShortSale;
	CCheckBox m_CSPBShort;

	CMaskIDEdit m_Ticket;
	CMaskIDEdit m_SwapTicket;
	CMaskIDEdit m_UnWindTicket;

	CCntlEdit m_OptTicker;
	CCntlEdit m_OptID;
	CCntlEdit m_Note;
	CCntlEdit m_Note2;

	CCntlEdit m_AssetDesc;
	CCntlEdit m_AssetCurrency;
	CCntlEdit m_VARStatus;
	CCntlEdit m_Ratio;
	CCntlEdit m_Binary;

	CMaskMinTimeEdit m_ExecTime;
	CMaskDateEdit m_TradeDate;
	CMaskDateEdit m_ValueDate;
	CMaskDateEdit m_OptExp;
	CMaskDateEdit m_Maturity;
	CMaskDateEdit m_SWBooking;
	CMaskDateEdit m_DeliveryDate;

	CNumEdit m_Amount;
	CNumEdit m_Strike;
	CNumEdit m_FxRate;
	CNumEdit m_Price;
	CNumEdit m_VAR;
	CNumEdit m_AllocAmount;
	CNumEdit m_Remain;
	CNumEdit m_Margin;
	CNumEdit m_MarginAmount;
	CNumEdit m_DV01;
	CNumEdit m_PrevVAR;
	CNumEdit m_CurrVAR;
	CNumEdit m_VARLimit;

    COptComboBox m_TransType;
    COptComboBox m_CP;
    COptComboBox m_Currency;
	COptComboBox m_Portfolio;
	COptComboBox m_AssignCP;
	COptComboBox m_MarginCurrency;
	COptComboBox m_AAFReason;
	COptComboBox m_BestExecution;

    CMCComboBox m_Dir;
	CMCComboBox m_Trader;
	CMCComboBox m_Asset;
	CMCComboBox m_OptSet;
	CMCComboBox m_OptSet2;
	CMCComboBox m_AAMethod;
	CMCComboBox m_ETrade;

	double m_dNomAmount;
	int m_dParValue;
	unsigned int m_nNomID;
	int m_Try;
	UINT m_nCurrID;
	BOOL m_bAllocValid;
	BOOL m_bLoaded;
	CString m_Future;
	CString m_CPKey;
	CDBRecArray m_AllocArray;
	//}}AFX_DATA

// Attributes
public:
	CNWIFMS70Doc *GetDocument();
	CDocData &GetData() { return GetDocument()->m_Data; }

// Operations
public:
	BOOL AllocIsOK();
	void EnableAlloc();
	void EnableAllocCtrls();
	void EnableCtrls();
	void InitControls();
	BOOL IsOK();
	void LoadAssetData(BOOL bFind = FALSE);
	void UpdateETrade();
	BOOL UpdateData(BOOL bSaveandValid = TRUE);
	void UpdateNumber(CWnd* Ctrl, double Number, BOOL WholeNumber = FALSE);
	void UpdateAlloc(int Action = 0);
	void UpdateRatio();
	void UpdateLink();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTicketFXEntry)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CTicketFXEntry() {}
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CTicketEntry)
	afx_msg void OnSelchangeTranstypeCombo();
	afx_msg void OnBlockSelectedTicketList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickTicketList(long Col, long Row);
	afx_msg void OnEntryAdd();
	afx_msg void OnEntryDelete();
	afx_msg void OnEntryUpdate();
	afx_msg void OnEntryRefresh();
	afx_msg void OnUpdateEntryUpdate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEntryDelete(CCmdUI* pCmdUI);
	afx_msg void OnSelchangeTransdirCombo();
	afx_msg void OnSelchangeCurrencyCombo();
	afx_msg void OnEntryFindAsset();
	afx_msg void OnUpdateEntryFindAsset(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEntryAdd(CCmdUI* pCmdUI);
	afx_msg void OnListedCheck();
	afx_msg void OnSelChangeAssetCombo();
	afx_msg void OnEntryCopyTicket();
	afx_msg void OnUpdateEntryCopyTicket(CCmdUI* pCmdUI);
	afx_msg void OnEntryAddAllocation();
	afx_msg void OnEntryDeleteAllocation();
	afx_msg void OnEntryUpdateAllocation();
	afx_msg void OnUpdateEntryDeleteAllocation(CCmdUI* pCmdUI);
	afx_msg void OnDblClickEntryAllocationList(long Col, long Row);
	afx_msg void OnBlockSelectedEntryAllocationList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnEnChangeEntryAmountEdit();
	afx_msg void OnSetfocusAllocNomamountEdit();
	afx_msg void OnKillfocusAllocNomamountEdit();
	afx_msg void OnUpdateEntryUpdateAllocation(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEntryAddAllocation(CCmdUI* pCmdUI);
	afx_msg void OnSelchangeEntryPortfolioCombo();
	afx_msg void OnEntryLoadBlotter();
	afx_msg void OnEntryLoadTrade();
	afx_msg void OnEntryOptionExp();
	afx_msg void OnUpdateEntryOptionExp(CCmdUI* pCmdUI);
	afx_msg void OnKillfocusAmountEdit();
	afx_msg void OnEntryXcurrency();
	afx_msg void OnCbnSelchangeEntryAaCombo();
	afx_msg void OnEntryForbiddenList();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnEnSetfocusEntryBinaryEdit();
	afx_msg void OnEntryFinditem();
	afx_msg void OnCbnSelchangeFxentryAssetcodeCombo();
};

#ifndef _DEBUG  // debug version in CTicketEntry.cpp
inline CNWIFMS70Doc* CTicketFXEntry::GetDocument()
   { return (CNWIFMS70Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __TICKETFXENTRY_H__
