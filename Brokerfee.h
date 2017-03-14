#ifndef __BROKERFEE_H__
#define __BROKERFEE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Brokerfee.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBrokerFee dialog

class CBrokerFee : public CDialog
{
// Construction
public:
	CBrokerFee(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBrokerFee)
	enum { IDD = IDD_BROKERFEE_DIALOG };
	CString	m_Total;
	double m_Amount;
	CString m_sAmount;
	CEdit m_BrokerFee;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBrokerFee)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBrokerFee)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeBrokerfeeEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif