#ifndef __SECFEE_H__
#define __SECFEE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SECFee.h : header file
//

#ifndef __ORALOADER__
#include "oraloader.h"
#endif
/////////////////////////////////////////////////////////////////////////////
// CSECFee dialog

class CSECFee : public CDialog
{
// Construction
public:
	CSECFee(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSECFee)
	enum { IDD = IDD_SEC_FEE_DIALOG };
	COraLoader m_OraLoader;
	CDBSheet m_SS;
	CMaskDateEdit m_FromDate;
	CMaskDateEdit m_ToDate;
	CNumEdit m_Fee;
	BOOL m_bSecFee;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSECFee)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	void LoadData();
	void UpdateData(BOOL bAdd);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSECFee)
	virtual BOOL OnInitDialog();
	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeSecfeeFromEdit();
	afx_msg void OnEnChangeSecfeeToEdit();
	afx_msg void OnChangeSecFeeEdit();
	DECLARE_EVENTSINK_MAP()
	void OnDblClickSecfeeList(long Col, long Row);
	void OnBlockSelectedSecfeeList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnBnClickedSecfeeAddButton();
	afx_msg void OnBnClickedSecfeeUpdateButton();
	afx_msg void OnBnClickedSecfeeDeleteButton();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif