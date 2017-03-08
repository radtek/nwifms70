#if !defined(AFX_AGENTDLG_H__14013CF1_9156_45B0_ADD5_AF3007B7987E__INCLUDED_)
#define AFX_AGENTDLG_H__14013CF1_9156_45B0_ADD5_AF3007B7987E__INCLUDED_

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
// CAgentDlg dialog

class CAgentDlg : public CDialog
{
// Construction
public:
	CAgentDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAgentDlg)
	enum { IDD = IDD_AGENT_DIALOG };
	COraLoader m_OraLoader;
	CDBSheet m_SS;
	CCntlEdit m_IDEdit;
	CCntlEdit m_AgentEdit;
	CNumEdit m_RebateEdit;
	int m_Row;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAgentDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAgentDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeAgentIdEdit();
	afx_msg void OnAgentAddButton();
	afx_msg void OnAgentClearButton();
	afx_msg void OnAgentDeleteButton();
	afx_msg void OnAgentFindButton();
	afx_msg void OnBlockSelectedAgentList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickAgentList(long Col, long Row);
	afx_msg void OnAgentUpdateButton();
	virtual void OnCancel();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AGENTDLG_H__14013CF1_9156_45B0_ADD5_AF3007B7987E__INCLUDED_)
