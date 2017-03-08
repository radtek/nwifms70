
// NWIFMS70View.h : interface of the CNWIFMS70View class
//


#pragma once


class CNWIFMS70View : public CView
{
protected: // create from serialization only
	CNWIFMS70View();
	DECLARE_DYNCREATE(CNWIFMS70View)

// Attributes
public:
	CNWIFMS70Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CNWIFMS70View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in NWIFMS70View.cpp
inline CNWIFMS70Doc* CNWIFMS70View::GetDocument() const
   { return reinterpret_cast<CNWIFMS70Doc*>(m_pDocument); }
#endif

