#ifndef __BULKCOPY_H__
#define __BULKCOPY_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BulkCopy.h : header file
//

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CBulkCopy dialog

class CBulkCopy : public CDialog
{
// Construction
public:
	CBulkCopy(CWnd* pParent = NULL);   // standard constructor
	~CBulkCopy();

// Dialog Data
	//{{AFX_DATA(CBulkCopy)
	enum { IDD = IDD_BULK_COPY };
	CString m_Source;
	COraLoader* m_pOraLoader;
	COptComboBox* m_Owner;
	COptComboBox* m_Table;
	CHLBox* m_AvailCol;
	CHLBox* m_SelCol;	
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBulkCopy)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void GetSqlSelColumns(CString &Sql, CHLBox *SelCols);
	void ProcessData(CString& DelSql, CString& Sql);
	void AddSelItem(CHLBox *Avail, CHLBox *Sel);
	void DelSelItem(CHLBox *Sel);
	void EnableButtons();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBulkCopy)
	virtual BOOL OnInitDialog();
	afx_msg void OnBcAddAllButton();
	afx_msg void OnBcAddButton();
	afx_msg void OnBcDelButton();
	afx_msg void OnBcDelallButton();
	afx_msg void OnSelchangeBcAvailList();
	afx_msg void OnDblclkBcAvailList();
	afx_msg void OnSelchangeBcOwnerCombo();
	afx_msg void OnSelchangeBcSelList();
	afx_msg void OnDblclkBcSelList();
	afx_msg void OnBcRunButton();
	afx_msg void OnSelchangeBcTableCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
