#ifndef __REPORTDEF_H__
#define __REPORTDEF_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportDef.h : header file
//
#ifndef __REPORTDATA_H__
#include "reportdata.h"
#endif

#ifndef __COMBOX_H__
#include "combox.h"
#endif

#ifndef __HLBOX_H__ 
#include "hlbox.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportDef dialog

class CReportDef : public CDialog
{
// Construction
public:
	CReportDef(CWnd* pParent = NULL);   // standard constructor
	~CReportDef();

// Dialog Data
	//{{AFX_DATA(CReportDef)
	enum { IDD = IDD_REPORT_DEF_DIALOG };
	COptComboBox *m_Report;
	COptComboBox *m_ReportDef;
	CEdit m_DefName;
	CHLBox *m_AvailField;
	CHLBox *m_SelField;
	CHLBox *m_AvailValue;
	CHLBox *m_SelValue;
	CReportData *m_ReportData;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportDef)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void AddSelItem(CHLBox *Avail, CHLBox *Sel);
	void DelSelItem(CHLBox *Sel);
	CReportData *GetReportData() { return m_ReportData; }
	void EnableButtons();
	void ProcessData();

	// Generated message map functions
	//{{AFX_MSG(CReportDef)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeReportdefReportCombo();
	afx_msg void OnSelchangeReportdefReportdefCombo();
	afx_msg void OnKillfocusReportdefNameEdit();
	afx_msg void OnReportdefAddfieldButton();
	afx_msg void OnReportdefDelfieldButton();
	afx_msg void OnDblclkReportdefAvailfieldList();
	afx_msg void OnDblclkReportdefSelfieldList();
	afx_msg void OnSelchangeReportdefSelfieldList();
	afx_msg void OnReportdefAddvalueButton();
	afx_msg void OnDblclkReportdefAvailvalueList();
	afx_msg void OnReportdefDelvalueButton();
	afx_msg void OnDblclkReportdefSelvalueList();
	afx_msg void OnReportdefAddButton();
	afx_msg void OnReportdefUpdateButton();
	afx_msg void OnReportdefCopyButton();
	afx_msg void OnReportdefDeleteButton();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
