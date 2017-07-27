#ifndef __TICKETPROCESS_H__
#define __TICKETPROCESS_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TicketProcess.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTicketProcess form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#ifndef __NWIFMS70DOC_H__
#include "nwifms70doc.h"
#endif

#ifndef __MASKDATEEDIT_H__
#include "maskdateedit.h"
#endif

#ifndef __PROCESSDATA_H__
#include "processdata.h"
#endif

#ifndef __RISKREC_H__
#include "RiskRec.h"
#endif

class CTicketProcess : public CFormView
{
protected:
	CTicketProcess();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTicketProcess)

// Form Data
public:
	//{{AFX_DATA(CTicketProcess)
	enum { IDD = IDD_TICKETPROCESS_FORM };
	CProcessData m_Data;
	CRiskControl m_Risk;
	CDBSheet m_SS;
	CDBSheet m_AllocSS;

	CCheckBox m_IPO;
	CCheckBox m_TRS;
	CCheckBox m_Listed;
	CCheckBox m_EuroOpt;
	CCheckBox m_OptAuto;
	CCheckBox m_ShortSale;
	CCheckBox m_CSPBShort;
	CCheckBox m_WI;
	CCheckBox m_Funded;
	CCheckBox m_SecFee;
	CCheckBox m_OrFee;
	CCheckBox m_Confirm;
	CCheckBox m_DocReq;
	CCheckBox m_DocRecvd;
	
	CCntlEdit m_Asset;
	CCntlEdit m_AssetDesc;
	CCntlEdit m_AssetCurr;
	CCntlEdit m_OptTicker;
	CCntlEdit m_OptID;
	CCntlEdit m_Note;
	CCntlEdit m_Note2;
	
	CCntlEdit m_Booker;
	CCntlEdit m_SetCurrency;
	CCntlEdit m_Binary;
	CCntlEdit m_SetConvention;

	CCntlEdit m_ExecTime;
//	CMaskMinTimeEdit m_ExecTime;

	CMaskDateEdit m_TradeDate;
	CMaskDateEdit m_ValueDate;
	CMaskDateEdit m_Maturity;
	CMaskDateEdit m_ExerciseDate;
	CMaskDateEdit m_SWBooking;
	CMaskDateEdit m_SWMaturity;
	CMaskDateEdit m_DeliveryDate;
	CMaskDateEdit m_OptExpDate;
	CMaskDateEdit m_FXDate;

	CMaskIDEdit m_Ticket;
	CMaskIDEdit m_SwapTicket;
	CMaskIDEdit m_OptTicket;
	CMaskIDEdit m_CancelTicket;
	CMaskIDEdit m_CorrectTicket;

	CNumEdit m_Price;
	CNumEdit m_BrokerFee;
	CNumEdit m_SoftDollar;
	CNumEdit m_DownPymnt;
	CNumEdit m_VAR;
	CNumEdit m_DV01;
	CNumEdit m_Amount;
	CNumEdit m_FxRate;
	CNumEdit m_Rate;
	CNumEdit m_Strike;
	CNumEdit m_CT;
	CNumEdit m_RepoCT;
	CNumEdit m_AssignCT;
	CNumEdit m_Margin;
	CNumEdit m_MarginAmount;

	CNumEdit m_Cash;
	CNumEdit m_PrePaid;
	CNumEdit m_Accretion;
	CNumEdit m_Total;

	COptComboBox m_TransType;
	COptComboBox m_CP;
	COptComboBox m_RepoCP;
	COptComboBox m_AssignCP;
    COptComboBox m_Currency;
	COptComboBox m_RepoType;
    COptComboBox m_RateBasis;
	COptComboBox m_Formula;
    COptComboBox m_PFU;
    COptComboBox m_USDAcc;
    COptComboBox m_FxAcc;
	COptComboBox m_MarginCurrency;
	COptComboBox m_BestExecution;

	CMCComboBox m_Dir;
	CMCComboBox m_Trader;
	CMCComboBox m_Index;
	CMCComboBox m_OptSet;
	CMCComboBox m_OptSet2;
	CMCComboBox m_AAMethod;
	CMCComboBox m_ETrade;

	BOOL m_bShow;
	BOOL m_bWarned;
	int m_nRiskLevel;
	unsigned m_nCurrID;
	CString m_Future;
	CString m_Par;
	CString m_Custodian;
	CString m_sBooker;
	CString m_sImgID;
	//}}AFX_DATA

// Attributes
public:
	CNWIFMS70Doc *GetDocument();
	CDocData &GetData() { return GetDocument()->m_Data; }

// Operations
public:
	BOOL AutoProcessTrade(const CString Ticket);
	void ClearCtrls();
	void ChangeShortLabel();
	void EnableCtrls();
	void GetStatus(CDBRec &Rec, BOOL Updated = FALSE);
	void InitControls();
	BOOL IsOK();
	void UpdateCPCT(COptComboBox &CP, CCntlEdit &m_CT);
	BOOL UpdateData(BOOL bSaveandValid = TRUE);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTicketProcess)
public:
	virtual void OnInitialUpdate();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
protected:
	virtual ~CTicketProcess() {}
	void ProcessVerifyRisk();
	void UpdateAsset();
	void UpdateCash();
	void UpdateTradeDates(BOOL bEuroBond = FALSE);
	int VerifyRisk(CString &Text);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CTicketProcess)
	afx_msg void OnBlockSelectedProcessTicketList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickProcessTicketList(long Col, long Row);
	afx_msg void OnProcessLoad();
	afx_msg void OnProcessAdd();
	afx_msg void OnUpdateProcessAdd(CCmdUI* pCmdUI);
	afx_msg void OnProcessAllocation();
	afx_msg void OnProcessDelete();
	afx_msg void OnUpdateProcessDelete(CCmdUI* pCmdUI);
	afx_msg void OnProcessFindAsset();
	afx_msg void OnProcessRefresh();
	afx_msg void OnProcessUpdate();
	afx_msg void OnUpdateProcessUpdate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateProcessFindAsset(CCmdUI* pCmdUI);
	afx_msg void OnSetfocusProcessAssetEdit();
	afx_msg void OnSelchangeProcessTranstypeCombo();
	afx_msg void OnSelchangeProcessTransdirCombo();
	afx_msg void OnUpdateProcessAllocation(CCmdUI* pCmdUI);
	afx_msg void OnSelchangeProcessRepocpCombo();
	afx_msg void OnProcessFindItem();
	afx_msg void OnUpdateProcessFindItem(CCmdUI* pCmdUI);
	afx_msg void OnProcessListedCheck();
	afx_msg void OnUpdateProcessOptionExp(CCmdUI* pCmdUI);
	afx_msg void OnProcessOptionExp();
	afx_msg void OnUpdateProcessContractConvertor(CCmdUI* pCmdUI);
	afx_msg void OnProcessContractConvertor();
	afx_msg void OnSelchangeProcessCurrencyCombo();
	afx_msg void OnProcessXcurrency();
	afx_msg void OnSetfocusAmountEdit();
	afx_msg void OnKillfocusProcessExercisedateEdit();
	afx_msg void OnProcessFxreflink();
	afx_msg void OnBnClickedSecCheck();
	afx_msg void OnEnChangeProcessAssetEdit();
	afx_msg void OnEnKillfocusProcessPriceEdit();
	afx_msg void OnEnKillfocusProcessFxrateEdit();
	afx_msg void OnEnKillfocusProcessBrokerFeeEdit();
	afx_msg void OnEnKillfocusProcessOtherFeeEdit();
//	afx_msg void OnBnClickedProcessSecfeeCheck();
//	afx_msg void OnBnClickedProcessOrfeeCheck();
	afx_msg void OnEnChangeProcessUnwindTicketEdit();
	afx_msg void OnEnSetfocusProcessBinaryEdit();
	afx_msg void OnEnSetfocusProcessSetconventionEdit();
	afx_msg void OnEnSetfocusProcessCtEdit();
	afx_msg void OnEnSetfocusProcessRepoctEdit();
	afx_msg void OnEnSetfocusProcessAssignctEdit();
	afx_msg void OnProcessForbiddenList();
	afx_msg void OnProcessAutoProcessTrade();
	afx_msg void OnCbnSelchangeProcessAaCombo();
	afx_msg void OnEnChangeProcessAmountEdit();
	afx_msg void OnEnKillfocusProcessAmountEdit();
	afx_msg void OnEnChangeProcessPriceEdit();
	afx_msg void OnEnKillfocusProcessAssetEdit();
	afx_msg void OnCbnKillfocusProcessAaCombo();
	afx_msg void OnProcessFxcategory();
//	afx_msg void OnCbnSelchangeProcessAssigncpCombo();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CTicketProcess.cpp
inline CNWIFMS70Doc* CTicketProcess::GetDocument()
   { return (CNWIFMS70Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // __TICKETPROCESS_H__
