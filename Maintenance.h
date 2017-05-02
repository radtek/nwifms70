#ifndef __MAINTENANCE_H__
#define __MAINTENANCE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Maintenance.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMaintenance form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#ifndef __NWIFMS70DOC_H__
#include "nwifms70doc.h"
#endif

#ifndef __CHECKBOX_H__
#include "checkbox.h"
#endif

#ifndef __NUMEDIT_H__
#include "numedit.h"
#endif

#ifndef __MASKDATEEDIT_H__
#include "maskdateedit.h"
#endif

#ifndef __ASSETDATA_H__
#include "assetdata.h"
#endif

class CMaintenance : public CFormView
{
protected:
	CMaintenance();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CMaintenance)

// Form Data
public:
	//{{AFX_DATA(CMaintenance)
	enum { IDD = IDD_MAINTENANCE_FORM };
	CAssetData m_Data;
	CDBSheet m_SS;
	CDBSheet m_IntSS;
	CDBSheet m_HoldingSS;

	CCheckBox *m_Bond;
	CCheckBox *m_Accrue;
	CCheckBox *m_PrePaid;
	CCheckBox *m_UnListed;
	CCheckBox *m_OTCClear;
	CCheckBox *m_Forward;
	CCheckBox *m_Active;
	CCheckBox *m_1256;
	CCheckBox *m_BelowRating;
	CCheckBox *m_144A;
	CCheckBox *m_Margin;
	CCheckBox *m_FRA;
	CCntlEdit m_Asset;
	CCntlEdit m_AssetDesc;
	CCntlEdit m_Common;
	CCntlEdit m_Cusip;
	CCntlEdit m_Isin;
	CCntlEdit m_Sedol;
	CCntlEdit m_RedCode;
	CCntlEdit m_ImagineCode;
	CCntlEdit m_PID;
	CMaskIDEdit m_AssetID;
	CMaskDateEdit m_IssueDate;
	CMaskDateEdit m_CouponStartDate;
	CMaskDateEdit m_RollDate;
	CMaskDateEdit m_Maturity;
	CMaskDateEdit m_FxDate;
	CCntlEdit m_Formula;
	CCntlEdit m_Source;
	CNumEdit m_Par;
	CNumEdit m_AdjF;
	CNumEdit m_SwapFactor;
	CNumEdit m_Amount;
	CCntlEdit m_Underline;
	CCntlEdit m_UnderlineID;
	CCntlEdit m_AdminID;
	COptComboBox* m_Industry;
	COptComboBox* m_Country;
	COptComboBox* m_ProfileCountry;
	COptComboBox* m_Currency;
	COptComboBox* m_Currency2;
	COptComboBox* m_Class;
	COptComboBox* m_ProfileClass;
	COptComboBox* m_RateBasis;
	COptComboBox* m_RateType;
	COptComboBox* m_Category;
	COptComboBox* m_Issuer;
	COptComboBox* m_Method;
	COptComboBox* m_Liquidity;
	COptComboBox* m_PFClass;
	COptComboBox* m_PFSubClass;
	COptComboBox* m_CitiClass;
	COptComboBox* m_BlmKey;
	COptComboBox* m_Account;
	COptComboBox* m_FutureType;
	
	CMCComboBox m_Index;
	CMCComboBox m_Bucket;
	CMCComboBox m_Bucket2;
	CMCComboBox m_Bucket3;
	CMCComboBox m_Class2;
	CMCComboBox m_PmntFreq;
	CMCComboBox m_CmpdFreq;
	CMCComboBox m_Level;
	CMCComboBox m_CDSFeeType;
	CMCComboBox m_BDate;
	CMCComboBox m_CDSRestructuring;
	CMCComboBox m_BDC;
	CMCComboBox m_YieldCurve;
	CMCComboBox m_IRSTemplate;
	CMCComboBox m_Exchange;

	CMaskDateEdit m_FromDate;
	CMaskDateEdit m_ToDate;
	CMaskDateEdit m_SetDate;
	CMaskDateEdit m_EndDate;
	CNumEdit m_Rate;
	CNumEdit m_Amort;
	CNumEdit m_Cap;
	CNumEdit m_Plus;
	COptComboBox* m_Action;

	BOOL m_bLoaded;
	unsigned m_CurrID;
	CString m_OldInvType;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CNWIFMS70Doc* GetDocument();
	CDocData &GetData() { return GetDocument()->m_Data; }
	BOOL UpdateData(BOOL bSaveandValid = TRUE);

// Operations
public:
	void InitControls();
	void EnableInvCtrls();
	void EnableCtrls(CString &Class);
	void FindFieldItem(CCntlEdit &Edit, LPCTSTR Sql);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaintenance)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMaintenance();
	BOOL IsUnique(CWnd &Ctrl, LPCTSTR FieldName, LPCTSTR DescFieldName, BOOL bAdd);
	BOOL IsAssetOK(BOOL nAdd = TRUE);
	BOOL IsInterestOK();
	void SetRateControls();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CMaintenance)
	afx_msg void OnBlockSelectedMaintAssetList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickMaintAssetList(long Col, long Row);
	afx_msg void OnMaintAdd();
	afx_msg void OnUpdateMaintAdd(CCmdUI* pCmdUI);
	afx_msg void OnMaintClearScreen();
	afx_msg void OnUpdateMaintDelete(CCmdUI* pCmdUI);
	afx_msg void OnMaintDelete();
	afx_msg void OnMaintFind();
	afx_msg void OnMaintUpdate();
	afx_msg void OnUpdateMaintUpdate(CCmdUI* pCmdUI);
	afx_msg void OnMaintBondCheck();
	afx_msg void OnBlockSelectedMaintInterestList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickMaintInterestList(long Col, long Row);
	afx_msg void OnChangeMaintAsset();
	afx_msg void OnMaintAddRate();
	afx_msg void OnUpdateMaintAddRate(CCmdUI* pCmdUI);
	afx_msg void OnMaintDeleteRate();
	afx_msg void OnUpdateMaintDeleteRate(CCmdUI* pCmdUI);
	afx_msg void OnMaintUpdateRate();
	afx_msg void OnUpdateMaintUpdateRate(CCmdUI* pCmdUI);
	afx_msg void OnMaintFindItem();
	afx_msg void OnUpdateMaintFindItem(CCmdUI* pCmdUI);
	afx_msg void OnMaintOther();
	afx_msg void OnMaintSecFee();
	afx_msg void OnSetfocusMaintDfFeeTypeEdit();
	afx_msg void OnSelchangeMaintAssetTypeCombo();
	afx_msg void OnSetfocusMaintUnderlineEdit();
	afx_msg void OnUpdateMaintOther(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMaintFillRate(CCmdUI* pCmdUI);
	afx_msg void OnMaintFillRate();
	afx_msg void OnUpdateMaintSecFee(CCmdUI* pCmdUI);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeMaintRateTypeCombo();
	afx_msg void OnEnSetfocusMaintCalendarEdit();
	afx_msg void OnEnSetfocusMaintRestructuringEdit();
	afx_msg void OnMaintOrFee();
	afx_msg void OnUpdateMaintOrFee(CCmdUI *pCmdUI);
	afx_msg void OnMaintImport();
	afx_msg void OnEnKillfocusMaintIssueDateEdit();
};

#ifndef _DEBUG  // debug version in CMaintenance.cpp
inline CNWIFMS70Doc* CMaintenance::GetDocument()
   { return (CNWIFMS70Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
