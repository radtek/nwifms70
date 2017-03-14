#ifndef __OPTEXPDLG_H__
#define __OPTEXPDLG_H__

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

#endif 
