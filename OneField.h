#if !defined(AFX_ONEFIELD_H__801F95D3_494D_11D5_A45D_00062926CA4A__INCLUDED_)
#define AFX_ONEFIELD_H__801F95D3_494D_11D5_A45D_00062926CA4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OneField.h : header file
//

#ifndef __SMALLRECDATA_H__
#include "smallrecdata.h"
#endif

#ifndef __CNTLEDIT_H__
#include "cntledit.h"
#endif

#ifndef __DBRECARRAY_H__
#include "dbrecarray.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// COneField dialog

class COneField : public CDialog
{
// Construction
public:
	COneField(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COneField)
	enum { IDD = IDD_ONE_FIELD_DIALOG };
	CSmallRecData m_Data;
	CDBSheet m_SS;
	CCntlEdit m_KeyEdit;
	BOOL m_bModified;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COneField)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COneField)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeOneKeyEdit();
	afx_msg void OnOneFindButton();
	afx_msg void OnOneClearButton();
	afx_msg void OnOneAddButton();
	afx_msg void OnOneDeleteButton();
	afx_msg void OnOneUpdateButton();
	afx_msg void OnDblClickOneDataList(long Col, long Row);
	afx_msg void OnBlockSelectedOneDataList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ONEFIELD_H__801F95D3_494D_11D5_A45D_00062926CA4A__INCLUDED_)
