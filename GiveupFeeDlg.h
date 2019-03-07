#ifndef __GIVEUPFEEDLG_H__
#define __GIVEUPFEEDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Country.h : header file
//
#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __MASKDATEEDIT_H__
#include "maskdateedit.h"
#endif

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif
/////////////////////////////////////////////////////////////////////////////
// CGiveupFeeDlg dialog

class CGiveupFeeDlg : public CDialog
{
// Construction
public:
	CGiveupFeeDlg(CWnd* pParent = NULL);   // standard constructor
	~CGiveupFeeDlg();

// Dialog Data
	//{{AFX_DATA(CCountry)
	enum { IDD = IDD_GIVEUPFEE_DIALOG };
	CDocData *m_pData;
	CDBSheet m_SS;
	
	COptComboBox m_PB;
	COptComboBox m_ExoticOptType;
	CMaskDateEdit m_FromDate;
	CMaskDateEdit m_ToDate;
	CNumEdit m_Cost;
	CNumEdit m_Cost2;
	CCntlEdit m_CP;
	BOOL m_bAddNew;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGiveupFeeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL IsOK();
	BOOL UpdateData(BOOL bSaveAndValid);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void EnableCtrls();

	// Generated message map functions
	//{{AFX_MSG(CGiveupFeeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnGiveupAddButton();
	afx_msg void OnGiveupClearButton();
	afx_msg void OnGiveupDeleteButton();
	afx_msg void OnGiveupFindButton();
	afx_msg void OnGiveupUpdateButton();
	afx_msg void OnBlockSelectedGiveupList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickGiveupList(long Col, long Row);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
