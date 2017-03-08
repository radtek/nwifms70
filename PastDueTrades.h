#if !defined(AFX_PASTDUETRADES_H__D2C6345E_5CC1_46DE_B2A1_592849255340__INCLUDED_)
#define AFX_PASTDUETRADES_H__D2C6345E_5CC1_46DE_B2A1_592849255340__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PastDueTrades.h : header file
//

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CPastDueTrades dialog

class CPastDueTrades : public CDialog
{
// Construction
public:
	CPastDueTrades(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPastDueTrades)
	enum { IDD = IDD_PASTDUE_TRADE_DIALOG };
	CDocData *m_pData;
	CDBSheet m_SS;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPastDueTrades)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPastDueTrades)
	virtual BOOL OnInitDialog();
	afx_msg void OnPastdueCopyButton();
	afx_msg void OnBlockSelectedPastdueTradeList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PASTDUETRADES_H__D2C6345E_5CC1_46DE_B2A1_592849255340__INCLUDED_)
