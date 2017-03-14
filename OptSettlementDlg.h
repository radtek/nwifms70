#ifndef __OPTSETTLEMENTDLG_H__
#define __OPTSETTLEMENTDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AgentDlg.h : header file
//
#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

#ifndef __NUMEDIT_H__
#include "numedit.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// COptSettlementDlg dialog

class COptSettlementDlg : public CDialog
{
// Construction
public:
	COptSettlementDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptSettlementDlg)
	enum { IDD = IDD_OPTSET_DIALOG };
	COraLoader m_OraLoader;
	CDBSheet m_SS;
	CCntlEdit m_OptSetEdit;
	CCntlEdit m_ZoneEdit;
	CCntlEdit m_TimeEdit;
	CString m_SetCode;
	CString m_Title;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptSettlementDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptSettlementDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblClickOptSetList(long Col, long Row);
	virtual void OnOK();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
