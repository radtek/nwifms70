#ifndef __ALLOCATION_H__
#define __ALLOCATION_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Allocation.h : header file
//
#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

#ifndef __ASSETVAL_H__
#include "assetval.h"
#endif

#ifndef __ALLOCDATA_H__
#include "allocdata.h"
#endif

#ifndef __NUMEDIT_H__
#include "numedit.h"
#endif

#ifndef __RISKREC_H__
#include "RiskRec.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CAllocation dialog
class CAllocation : public CDialog
{
// Construction
public:
	CAllocation(CWnd* pParent = NULL);   // standard constructor
	~CAllocation() {}

// Dialog Data
	//{{AFX_DATA(CAllocation)
	enum { IDD = IDD_ALLOCATION };
	CAllocData m_Data;
	CDocData *m_pData;
	CRiskControl m_Risk;
	CDBSheet m_SS;
	CDBSheet m_RepoInfoSS;

	CCheckBox m_CSPBShort;

	COptComboBox m_Portfolio;
	COptComboBox m_Custodian;
	COptComboBox m_Account;
	CString m_TransNum;

	CNumEdit m_Amount;
	CNumEdit m_Price;
	CNumEdit m_OtherFee;

	CString m_Category;
	CString m_AssetClass;
	CString m_TransType;
	CString m_Formula;
	CString m_Margin;
	CString m_PFU;
	CString m_DownPay;
	CString m_sCustodian;
	int m_nRiskLevel;
	BOOL m_bWarned;
	CKeyDBRecArray m_RecArray;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAllocation)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void EnableCtrls();
	BOOL IsOK();
	CDocData &GetData() { return *m_pData; }

	void SetDefInfo(LPCTSTR Portfolio);
	void LoadRepoInfo();
	void ProcessVerifyRisk();
	void Refresh();
	void UpdateButtons();
	BOOL UpdateData(BOOL bAdd = TRUE);
	void Value();
	int VerifyRisk(CString &Text, const CString Portfolio);

	// Generated message map functions
	//{{AFX_MSG(CAllocation)
	afx_msg void OnAddButton();
	afx_msg void OnDblClickAllocationList(long Col, long Row);
	afx_msg void OnBlockSelectedAllocationList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDeleteButton();
	afx_msg void OnRepoButton();
	afx_msg void OnSelchangePortfolioCombo();
	afx_msg void OnSelchangeAccountCombo();
	afx_msg void OnUpdateButton();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusNomamountEdit();
	afx_msg void OnKillfocusPriceEdit();
	afx_msg void OnAllocFractionButton();
	afx_msg void OnKillfocusAllocDownpayEdit();
	afx_msg void OnAllocFinditemButton();
	afx_msg void OnAllocInvertPriceButton();
	afx_msg void OnAllocFinditem2Button();
	afx_msg void OnAllocContractConvertorButton();
	afx_msg void OnSetfocusAllocNomamountEdit();
	afx_msg void OnAllocRepopriceButton();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnKillfocusAllocPortfolioCombo();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
