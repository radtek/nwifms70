#if !defined(AFX_REPOPRICE_H__89D9C5C3_2216_11D5_A459_00062926CA4A__INCLUDED_)
#define AFX_REPOPRICE_H__89D9C5C3_2216_11D5_A459_00062926CA4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RepoPrice.h : header file
//

#ifndef __NUMEDIT_H__
#include "numedit.h"
#endif

#ifndef __ASSETVAL_H__
#include "assetval.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CRepoPrice dialog

class CRepoPrice : public CDialog
{
// Construction
public:
	CRepoPrice(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRepoPrice)
	enum { IDD = IDD_REPOPRICE_DIALOG };
	CString	m_Asset;
	CString	m_NetPrice;
	CString m_NWINetPrice;
	CString m_ValueDate;
	CAssetVal m_Val;
	CNumEdit m_CleanPriceEdit;
	CNumEdit m_DirtyPriceEdit;
	CNumEdit m_HairCutEdit;
	CNumEdit m_NetPriceEdit;
	CNumEdit m_NWINetPriceEdit;
	CString	m_HairCut;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRepoPrice)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void DisplayPrice(int Index);

	// Generated message map functions
	//{{AFX_MSG(CRepoPrice)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeRepoPriceCleanpriceEdit();
	afx_msg void OnChangeRepoPriceDirtypriceEdit();
	afx_msg void OnChangeRepoPriceHairCutEdit();
	afx_msg void OnChangeRepopriceNetpriceEdit();
	afx_msg void OnChangeRepopriceNwinetpriceEdit();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPOPRICE_H__89D9C5C3_2216_11D5_A459_00062926CA4A__INCLUDED_)
