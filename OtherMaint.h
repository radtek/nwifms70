#if !defined(AFX_OTHERMAINT_H__801F95D4_494D_11D5_A45D_00062926CA4A__INCLUDED_)
#define AFX_OTHERMAINT_H__801F95D4_494D_11D5_A45D_00062926CA4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OtherMaint.h : header file
//

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif
/////////////////////////////////////////////////////////////////////////////
// COtherMaint dialog

class COtherMaint : public CDialog
{
// Construction
public:
	COtherMaint(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COtherMaint)
	enum { IDD = IDD_MAINT_OTHER_DIALOG };
	CString	m_Table;
	CString	m_TableSpace;
	CString m_Key;
	CString m_Key2;
	CString m_Fields;
	CString m_PreLoad;
	CDBSheet m_SS;
	COraLoader m_OraLoader;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COtherMaint)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString &GetFields();
	void GetKeys(LPCTSTR Key);

	// Generated message map functions
	//{{AFX_MSG(COtherMaint)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblClickMaintOtherTableList(long Col, long Row);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OTHERMAINT_H__801F95D4_494D_11D5_A45D_00062926CA4A__INCLUDED_)
