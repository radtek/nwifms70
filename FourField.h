#ifndef __FOURFIELD_H__
#define __FOURFIELD_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FourField.h : header file
//

#ifndef __SMALLRECDATA_H__
#include "smallrecdata.h"
#endif

#ifndef __CNTLEDIT_H__
#include "cntledit.h"
#endif
/////////////////////////////////////////////////////////////////////////////
// CFourField dialog

class CFourField : public CDialog
{
// Construction
public:
	CFourField(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFourField)
	enum { IDD = IDD_FOUR_FIELD_DIALOG };
	CSmallRecData m_Data;
	CDBSheet m_SS;
	CCntlEdit m_KeyEdit;
	CCntlEdit m_DataEdit;
	CCntlEdit m_Data2Edit;
	CCntlEdit m_Data3Edit;
	BOOL m_bModified;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFourField)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFourField)
	afx_msg void OnFourAddButton();
	afx_msg void OnFourClearButton();
	afx_msg void OnFourDeleteButton();
	afx_msg void OnFourFindButton();
	afx_msg void OnFourUpdateButton();
	afx_msg void OnBlockSelectedFourDataList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickFourDataList(long Col, long Row);
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeFourDataEdit();
	afx_msg void OnChangeFourData2Edit();
	afx_msg void OnChangeFourData3Edit();
	afx_msg void OnChangeFourKeyEdit();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
