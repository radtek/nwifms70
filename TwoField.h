#ifndef __TWOFIELD_H__
#define __TWOFIELD_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TwoField.h : header file
//

#ifndef __SMALLRECDATA_H__
#include "smallrecdata.h"
#endif

#ifndef __CNTLEDIT_H__
#include "cntledit.h"
#endif
/////////////////////////////////////////////////////////////////////////////
// CTwoField dialog

class CTwoField : public CDialog
{
// Construction
public:
	CTwoField(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTwoField)
	enum { IDD = IDD_TWO_FIELD_DIALOG };
	CSmallRecData m_Data;
	CDBSheet m_SS;
	CCntlEdit m_KeyEdit;
	CCntlEdit m_DataEdit;
	BOOL m_bModified;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTwoField)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTwoField)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeTwoKeyEdit();
	afx_msg void OnChangeTwoDataEdit();
	afx_msg void OnTwoFindButton();
	afx_msg void OnTwoAddButton();
	afx_msg void OnTwoDeleteButton();
	afx_msg void OnTwoUpdateButton();
	afx_msg void OnDblClickTwoDataList(long Col, long Row);
	afx_msg void OnBlockSelectedTwoDataList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnTwoClearButton();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif