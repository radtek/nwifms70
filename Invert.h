#ifndef __INVERT_H__
#define __INVERT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Invert.h : header file
//

#ifndef __NUMEDIT_H__
#include "numedit.h"
#endif
/////////////////////////////////////////////////////////////////////////////
// CInvert dialog

class CInvert : public CDialog
{
// Construction
public:
	CInvert(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInvert)
	enum { IDD = IDD_INVERT_DIALOG };
	CString	m_New;
	CString	m_Origin;
	CNumEdit m_OriginEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInvert)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInvert)
	afx_msg void OnChangeInvertOriginEdit();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
