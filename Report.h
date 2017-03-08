#if !defined(AFX_REPORT_H__509B6693_CA3B_11D4_A452_00062926CA4A__INCLUDED_)
#define AFX_REPORT_H__509B6693_CA3B_11D4_A452_00062926CA4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Report.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CReport form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#ifndef __NWIFMS70DOC_H__
#include "nwifms70doc.h"
#endif

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __REPORTDATA_H__
#include "reportdata.h"
#endif


class CReport : public CFormView
{
protected:
	CReport();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CReport)

// Form Data
public:
	//{{AFX_DATA(CReport)
	enum { IDD = IDD_REPORT_FORM };
	CReportData m_Data;
	COptComboBox *m_Date;
	COptComboBox *m_Report;
	COptComboBox *m_ReportDef;
	CDBSheet m_SS;
	//}}AFX_DATA

// Attributes
public:
	CNWIFMS70Doc *GetDocument();	

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReport)
public:
	virtual void OnInitialUpdate();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CReport();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CReport)
	afx_msg void OnReportSetup();
	afx_msg void OnSelchangeReportDateCombo();
	afx_msg void OnSelchangeReportReportCombo();
	afx_msg void OnSelchangeReportDefCombo();
	afx_msg void OnReportSelectBlock();
	afx_msg void OnUpdateReportSelectBlock(CCmdUI* pCmdUI);
	afx_msg void OnReportCopy();
	afx_msg void OnUpdateReportCopy(CCmdUI* pCmdUI);
	afx_msg void OnReportRetrieve();
	afx_msg void OnUpdateReportRetrieve(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CReport.cpp
inline CNWIFMS70Doc* CReport::GetDocument()
   { return (CNWIFMS70Doc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REPORT_H__509B6693_CA3B_11D4_A452_00062926CA4A__INCLUDED_)
