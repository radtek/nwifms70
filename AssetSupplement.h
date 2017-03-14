#ifndef __ASSETSUPPLEMENT_H__
#define __ASSETSUPPLEMENT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AssetSupplement.h : header file
//

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __ASSETSUPPLEMENTDATA_H__
#include "assetsupplementdata.h"
#endif

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif
/////////////////////////////////////////////////////////////////////////////
// CAssetSupplement dialog

class CAssetSupplement : public CDialog
{
// Construction
public:
	CAssetSupplement(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAssetSupplement)
	enum { IDD = IDD_ASSET_SUPPLEMENT_DIALOG };
	CAssetSupplementData m_Data;
	CDocData *m_pData;
	CDBSheet m_SS;
	
	CCntlEdit m_Asset;
	CCntlEdit m_Code;
	CCntlEdit m_Desc;
	CCntlEdit m_Common;
	CCntlEdit m_Isin;
	CNumEdit m_AmountOf;
	CNumEdit m_PerAmount;
	CNumEdit m_RoundingFactor;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAssetSupplement)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void EnableCtrls();

	// Generated message map functions
	//{{AFX_MSG(CAssetSupplement)
	virtual BOOL OnInitDialog();
	afx_msg void OnAssetsuppAddButton();
	afx_msg void OnAssetsuppClearButton();
	afx_msg void OnAssetsuppFindButton();
	afx_msg void OnBlockSelectedAssetsuppList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickAssetsuppList(long Col, long Row);
	afx_msg void OnAssetsuppDeleteButton();
	afx_msg void OnAssetsuppFindassetButton();
	afx_msg void OnAssetsuppUpdateButton();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
