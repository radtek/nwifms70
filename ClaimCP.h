#ifndef __CLAIMCP_H__
#define __CLAIMCP_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ClaimCP.h : header file
//

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __CLAIMDATA_H__
#include "claimdata.h"
#endif

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CClaimCP dialog

class CClaimCP : public CDialog
{
// Construction
public:
	CClaimCP(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CClaimCP)
	enum { IDD = IDD_CLAIM_CP_DIALOG };
	CClaimData m_Data;
	CDocData *m_pData;
	CDBSheet m_SS;
	
	COptComboBox *m_CP;
	CCntlEdit m_ClaimCode;
	CCntlEdit m_Name;
	CCntlEdit m_Phone;
	CCntlEdit m_Fax;
	CCntlEdit m_EMail;	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClaimCP)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void EnableCtrls();

	// Generated message map functions
	//{{AFX_MSG(CClaimCP)
	virtual BOOL OnInitDialog();
	afx_msg void OnClaimAddButton();
	afx_msg void OnClaimClearButton();
	afx_msg void OnClaimDeleteButton();
	afx_msg void OnClaimFindButton();
	afx_msg void OnBlockSelectedClaimList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickClaimList(long Col, long Row);
	afx_msg void OnClaimUpdateButton();
	virtual void OnCancel();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
