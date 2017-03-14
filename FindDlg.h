#ifndef __FINDDLG_H__
#define __FINDDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FindDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFindDlg dialog
#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

class CFindDlg : public CDialog
{
// Construction
public:
	CFindDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFindDlg)
	enum { IDD = IDD_FIND_DIALOG };
	CDBSheet m_SS;
	CEdit m_KeyEdit;
	CEdit m_DataEdit;
	CString m_Key;
	COraLoader m_OraLoader;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL Find();

	// Generated message map functions
	//{{AFX_MSG(CFindDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblClickFindList(long Col, long Row);
	afx_msg void OnChangeFindDataEdit();
	afx_msg void OnChangeFindKeyEdit();
	virtual void OnOK();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
