#if !defined(AFX_OPTEXPDLG_H__4228EA83_E994_11D5_A47F_00062926CA4A__INCLUDED_)
#define AFX_OPTEXPDLG_H__4228EA83_E994_11D5_A47F_00062926CA4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptExpDlg.h : header file
//
#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif
/////////////////////////////////////////////////////////////////////////////
// COptExpDlg dialog

class COptExpDlg : public CDialog
{
// Construction
public:
	COptExpDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptExpDlg)
	enum { IDD = IDD_OPTEXP_DIALOG };
	CString m_Date;
	COraLoader m_OraLoader;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptExpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	COraLoader &GetOraLoader() { return m_OraLoader; }

	// Generated message map functions
	//{{AFX_MSG(COptExpDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusOptexpSrcdateEdit();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTEXPDLG_H__4228EA83_E994_11D5_A47F_00062926CA4A__INCLUDED_)
