#ifndef __CONNECT_H__
#define __CONNECT_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Connect.h : header file
//
#include "nwifms70.h"

/////////////////////////////////////////////////////////////////////////////
// CConnect dialog

class CConnect : public CDialog
{
// Construction
public:
	CConnect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConnect)
	enum { IDD = IDD_CONNECT };
	CNWIFMS70App *m_pTheApp;
	int	m_nTrials;
	//}}AFX_DATA

private:
	int     	m_nCount;

private:
	void EnableOk();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConnect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int PasswordExpiryWarning();

	// Generated message map functions
	//{{AFX_MSG(CConnect)
	afx_msg void OnChangeHost();
	afx_msg void OnChangePassword();
	afx_msg void OnChangeUser();
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif
