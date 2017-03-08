#if !defined(AFX_ACCOUNTDLG_H__3BF7427A_81F0_420F_9CC3_E38870CA229D__INCLUDED_)
#define AFX_ACCOUNTDLG_H__3BF7427A_81F0_420F_9CC3_E38870CA229D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AccountDlg.h : header file
//

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __ACCOUNTDATA_H__
#include "accountdata.h"
#endif

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccountDlg dialog

class CAccountDlg : public CDialog
{
// Construction
public:
	CAccountDlg(CWnd* pParent = NULL);   // standard constructor
	~CAccountDlg();

// Dialog Data
	//{{AFX_DATA(CAccountDlg)
	enum { IDD = IDD_ACCOUNT_DIALOG };
	CAccountData m_Data;
	CDocData *m_pData;
	CDBSheet m_SS;
	
	COptComboBox *m_CP;
	CCntlEdit m_Account;	
	CCntlEdit m_Desc;
	CCntlEdit m_Group;
	CCheckBox *m_Collateral;
	CCheckBox *m_Clearing;
	CCheckBox *m_Future;
	CCheckBox *m_Active;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccountDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void EnableCtrls();

	// Generated message map functions
	//{{AFX_MSG(CAccountDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnAccountClearButton();
	afx_msg void OnAccountFindButton();
	afx_msg void OnAccountAddButton();
	afx_msg void OnAccountUpdateButton();
	afx_msg void OnBlockSelectedAccountList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickAccountList(long Col, long Row);
	afx_msg void OnSelchangeAccountCombo();
	afx_msg void OnAccountGroupButton();
	afx_msg void OnAccountDeleteButton();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCOUNTDLG_H__3BF7427A_81F0_420F_9CC3_E38870CA229D__INCLUDED_)
