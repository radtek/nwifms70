#if !defined(AFX_BLMID_H__BF18E259_C09D_467C_81D2_3F708B8DBA10__INCLUDED_)
#define AFX_BLMID_H__BF18E259_C09D_467C_81D2_3F708B8DBA10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BlmID.h : header file
//

#ifndef __DBSHEET_H__
#include "dbsheet.h"
#endif

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif
/////////////////////////////////////////////////////////////////////////////
// CBlmID dialog

class CBlmID : public CDialog
{
// Construction
public:
	CBlmID(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBlmID)
	enum { IDD = IDD_BLOOMBERGID_DIALOG };
	CDBSheet m_SS;
	COraLoader m_OraLoader;
	CDBRecArray m_DataArray;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlmID)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void GetKeyInfo(CString Text, CString &TransType, CString &Asset, CString &Ticket,
					CString &TransNum, CString &Strike, CString &OptExp);
	void SetSheetSize();
	void UpdateRecord(CString &Text, LPCTSTR ID, int Status);

	// Generated message map functions
	//{{AFX_MSG(CBlmID)
	virtual BOOL OnInitDialog();
	afx_msg void OnBlmCopyButton();
	afx_msg void OnBlmPasteButton();
	afx_msg void OnBlmReloadButton();
	afx_msg void OnBlmSaveButton();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLMID_H__BF18E259_C09D_467C_81D2_3F708B8DBA10__INCLUDED_)
