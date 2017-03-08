#if !defined(AFX_CHANGEFUNDDLG_H__7DC80761_AE1B_4348_9538_9461AB5699AE__INCLUDED_)
#define AFX_CHANGEFUNDDLG_H__7DC80761_AE1B_4348_9538_9461AB5699AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChangeFundDlg.h : header file
//
#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CChangeFundDlg dialog

class CChangeFundDlg : public CDialog
{
// Construction
public:
	CChangeFundDlg(CWnd* pParent = NULL);   // standard constructor
	~CChangeFundDlg();

// Dialog Data
	//{{AFX_DATA(CChangeFundDlg)
	enum { IDD = IDD_CHANGE_FUND_DIALOG };
	CDocData *m_pData;
	COraLoader m_OraLoader;
	CMaskIDEdit m_TransNum;
	COptComboBox *m_Portfolio;
	COptComboBox *m_NewPortfolio;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChangeFundDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChangeFundDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCfFind();
	afx_msg void OnCfUpdate();
	afx_msg void OnChangeCfTransnumEdit();
	afx_msg void OnSelchangeCfNewportfolioCombo();
	afx_msg void OnCfClear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANGEFUNDDLG_H__7DC80761_AE1B_4348_9538_9461AB5699AE__INCLUDED_)
