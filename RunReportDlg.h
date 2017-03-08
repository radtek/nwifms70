#if !defined(AFX_RUNREPORTDLG_H__230A4D73_BCDB_11D5_A479_00062926CA4A__INCLUDED_)
#define AFX_RUNREPORTDLG_H__230A4D73_BCDB_11D5_A479_00062926CA4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RunReportDlg.h : header file
//
#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __ROWCTRL_H__
#include "rowctrl.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CRunReportDlg dialog

class CRunReportDlg : public CDialog
{
// Construction
public:
	CRunReportDlg(CWnd* pParent = NULL);   // standard constructor
	~CRunReportDlg();

// Dialog Data
	//{{AFX_DATA(CRunReportDlg)
	enum { IDD = IDD_RUNREPORT_DIALOG };
	COraLoader *m_pOraLoader;
	CDBSheet m_SS;
	CRowCtrl m_Row;
	CCntlEdit m_ReportEdit;
	CCheckBox *m_PrintFlag;
	CString m_Report;
	CString m_RunFile;
	CString m_PrintFile;
	CString m_PrintDef;
	CString m_Cmd;
	CString m_TheDate;
	CString m_FDate;
	CString m_LDate;
	CString m_FNavDate;
	CString m_AccrDate;
	CString m_FundDate;	
	CString m_PrevYearEnd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRunReportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void LoadReport();

	// Generated message map functions
	//{{AFX_MSG(CRunReportDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRunreportRunButton();
	afx_msg void OnDblClickRunreportList(long Col, long Row);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RUNREPORTDLG_H__230A4D73_BCDB_11D5_A479_00062926CA4A__INCLUDED_)
