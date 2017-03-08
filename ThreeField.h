#if !defined(AFX_THREEFIELD_H__801F95D3_494D_11D5_A45D_00062926CA4A__INCLUDED_)
#define AFX_THREEFIELD_H__801F95D3_494D_11D5_A45D_00062926CA4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ThreeField.h : header file
//

#ifndef __SMALLRECDATA_H__
#include "smallrecdata.h"
#endif

#ifndef __CNTLEDIT_H__
#include "cntledit.h"
#endif
/////////////////////////////////////////////////////////////////////////////
// CThreeField dialog

class CThreeField : public CDialog
{
// Construction
public:
	CThreeField(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CThreeField)
	enum { IDD = IDD_THREE_FIELD_DIALOG };
	CSmallRecData m_Data;
	CDBSheet m_SS;
	CCntlEdit m_KeyEdit;
	CCntlEdit m_DataEdit;
	CCntlEdit m_Data2Edit;
	BOOL m_bModified;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CThreeField)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CThreeField)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeThreeKeyEdit();
	afx_msg void OnChangeThreeDataEdit();
	afx_msg void OnChangeThreeData2Edit();
	afx_msg void OnThreeFindButton();
	afx_msg void OnThreeClearButton();
	afx_msg void OnThreeAddButton();
	afx_msg void OnThreeDeleteButton();
	afx_msg void OnThreeUpdateButton();
	afx_msg void OnDblClickThreeDataList(long Col, long Row);
	afx_msg void OnBlockSelectedThreeDataList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_THREEFIELD_H__801F95D3_494D_11D5_A45D_00062926CA4A__INCLUDED_)
