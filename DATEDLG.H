#ifndef __DATEDLG_H__
#define __DATEDLG_H__

#if !defined(AFX_DATEDLG_H__25F4CCA3_BE12_11D3_A43A_00062926CA4A__INCLUDED_)
#define AFX_DATEDLG_H__25F4CCA3_BE12_11D3_A43A_00062926CA4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DateDlg.h : header file
//

#ifndef __MASKDATEEDIT_H__
#include "maskdateedit.h"
#endif

#ifndef __MASKIDEDIT_H__
#include "MaskIDEdit.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CDateDlg dialog

class CDateDlg : public CDialog
{
// Construction
public:
	CDateDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDateDlg)
	enum { IDD = IDD_INPUT_DIALOG };
	CString	m_Data;
	CMaskDateEdit m_DateEdit;
	CMaskIDEdit m_IDEdit;
	int m_nInputType;
	CString m_DlgHeader;
	CString m_Prompt;
	//}}AFX_DATA

protected:
    void EnableOk();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeDate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATEDLG_H__25F4CCA3_BE12_11D3_A43A_00062926CA4A__INCLUDED_)
#endif //
