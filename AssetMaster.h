#ifndef __ASSETMASTER_H__
#define __ASSETMASTER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AssetMaster.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAssetMaster form view

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

#ifndef __ASSETMASTERDATA_H__
#include "assetmasterdata.h"
#endif

class CAssetMaster : public CFormView
{
protected:
	CAssetMaster();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAssetMaster)

// Form Data
public:
	//{{AFX_DATA(CAssetMaster)
	enum { IDD = IDD_ASSET_FORM };
	CAssetMasterData m_Data;

	CDBSheet m_SS;
	CDBSheet m_IntSS;
	CDBSheet m_ClassSS;

	CCntlEdit m_Asset;
	CCntlEdit m_AssetDesc;
	CNumEdit m_Par;
	CCntlEdit m_Asset2;

	CCntlEdit m_Common;
	CCntlEdit m_Cusip;
	CCntlEdit m_Sedol;
	CCntlEdit m_Isin;
	CCntlEdit m_PID;

	CMaskDateEdit m_IssueDate;
	CMaskDateEdit m_Maturity;
	CMaskDateEdit m_RollDate;
	CCntlEdit m_CDSFeeType;
	CCntlEdit m_RefIndex;
	CCntlEdit m_Formula;
	CCntlEdit m_Source;
	
	COptComboBox *m_Country;
	COptComboBox *m_Currency;
	COptComboBox *m_Issuer;
	COptComboBox *m_Industry;
	COptComboBox *m_RateType;
	COptComboBox *m_RateBasis;
	COptComboBox *m_PmntFrq;
	
	CNumEdit m_AdjFactor;
	CCheckBox *m_Bond;
	CCheckBox *m_Accrue;
	CCheckBox *m_PrePaid;
	CCheckBox *m_Future;
	CCheckBox *m_1256;
	CCheckBox *m_BelowRating;
	CCheckBox *m_144A;
	CCheckBox *m_Fifo;
	CCheckBox *m_UnListed;
	CCheckBox *m_Inactive;

	CMaskDateEdit m_FromDate;
	CMaskDateEdit m_ToDate;
	CMaskDateEdit m_SetDate;
	CNumEdit m_Rate;
	CNumEdit m_Amort;
	CNumEdit m_Cap;
	CNumEdit m_Plus;
	CCheckBox *m_Inclusive;
	CMaskDateEdit m_IntFrom;
	CMaskDateEdit m_IntTo;

	CCntlEdit m_AssetCode;
	CCntlEdit m_AssetClassDesc;
	CCntlEdit m_AssetMaster;
	COptComboBox *m_Category;
	COptComboBox *m_Bucket;
	COptComboBox *m_Class;
	unsigned m_CurrID;
	//}}AFX_DATA

// Attributes
public:
	CNWIFMS70Doc	*GetDocument();
	void EnableSubCtrls();
	CDocData &GetData() { return GetDocument()->GetData(); }
	BOOL UpdateData(BOOL bSaveandValid = TRUE);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAssetMaster)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAssetMaster();
	BOOL IsAssetOK();
	BOOL IsInterestOK();
	BOOL IsClassOK();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CAssetMaster)
	afx_msg void OnBlockSelectedAssetList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickAssetList(long Col, long Row);
	afx_msg void OnBlockSelectedAssetInterestList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickAssetInterestList(long Col, long Row);
	afx_msg void OnBlockSelectedAssetClassList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickAssetClassList(long Col, long Row);
	afx_msg void OnAssetClearScreen();
	afx_msg void OnAssetFind();
	afx_msg void OnAssetAdd();
	afx_msg void OnAssetUpdate();
	afx_msg void OnAssetDelete();
	afx_msg void OnAssetAddRate();
	afx_msg void OnAssetUpdateRate();
	afx_msg void OnAssetDeleteRate();
	afx_msg void OnAssetAddClass();
	afx_msg void OnAssetUpdateClass();
	afx_msg void OnAssetDeleteClass();
	afx_msg void OnAssetFindItem();
	afx_msg void OnAssetOther();
	afx_msg void OnAssetSecFee();
	afx_msg void OnUpdateAssetAdd(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAssetDelete(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAssetFind(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAssetUpdate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAssetAddClass(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAssetDeleteClass(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAssetUpdateClass(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAssetAddRate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAssetDeleteRate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAssetUpdateRate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAssetFillRate(CCmdUI* pCmdUI);
	afx_msg void OnAssetFillRate();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in WFMSView.cpp
inline CNWIFMS70Doc* CAssetMaster::GetDocument()
   { return (CNWIFMS70Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
