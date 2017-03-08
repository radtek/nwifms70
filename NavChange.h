#if !defined(AFX_NAVCHANGE_H__11456903_644D_11D7_A4BE_00062926CA4A__INCLUDED_)
#define AFX_NAVCHANGE_H__11456903_644D_11D7_A4BE_00062926CA4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NavChange.h : header file
//

#ifndef __ORALOADER__
#include "oraloader.h"
#endif
/////////////////////////////////////////////////////////////////////////////
// CNavChange dialog

class CNavChange : public CDialog
{
// Construction
public:
	CNavChange(CWnd* pParent = NULL);   // standard constructor
	~CNavChange();

// Dialog Data
	//{{AFX_DATA(CNavChange)
	enum { IDD = IDD_NAV_DIALOG };
	COraLoader m_OraLoader;
	CMaskDateEdit m_Date;
	COptComboBox *m_Portfolio;
	CDBSheet m_SS;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNavChange)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void UpdateSum();

	// Generated message map functions
	//{{AFX_MSG(CNavChange)
	afx_msg void OnNavSaveButton();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeNavDateEdit();
	afx_msg void OnSelchangeNavPortfolioCombo();
	afx_msg void OnNavRunButton();
	afx_msg void OnChangeNavSheet(long Col, long Row);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAVCHANGE_H__11456903_644D_11D7_A4BE_00062926CA4A__INCLUDED_)
