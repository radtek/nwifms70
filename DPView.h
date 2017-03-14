#ifndef __DPVIEW_H__
#define __DPVIEW_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DPView.h : header file
//

#ifndef __NWIFMS70DOC_H__
#include "nwifms70doc.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CDPView view

class CDPView : public CEditView
{
protected:
	CDPView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDPView)

// Attributes
public:
	CNWIFMS70Doc *GetDocument();
	CDocData &GetData() { return GetDocument()->m_Data; }

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDPView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDPView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDPView)
	afx_msg void OnDpCustody();
	afx_msg void OnDpRunReport();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnDpBulkcopy();
	afx_msg void OnUpdateDpBulkcopy(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CDPView.cpp
inline CNWIFMS70Doc* CDPView::GetDocument()
   { return (CNWIFMS70Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
