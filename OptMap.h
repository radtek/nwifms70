#ifndef __OPTMAP__H__
#define __OPTMAP__H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TwoField.h : header file
//

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __CNTLEDIT_H__
#include "cntledit.h"
#endif
/////////////////////////////////////////////////////////////////////////////
// COptMap dialog

class COptMap : public CDialog
{
// Construction
public:
	COptMap(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptMap)
	enum { IDD = IDD_OPTMAP_DIALOG };
	CDBSheet m_SS;
	COraLoader m_OraLoader;
	CCntlEdit m_OptTicker;
	CCntlEdit m_ImagineCode;
	CCntlEdit m_ImagineBBTick;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptMap)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptMap)
	virtual BOOL OnInitDialog();
	afx_msg void OnOptMapUpdateButton();
	afx_msg void OnDblClickOptMapList(long Col, long Row);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // 
