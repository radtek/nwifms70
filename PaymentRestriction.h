#ifndef __PAYMENTRESTRICTION_H__
#define __PAYMENTRESTRICTION_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PaymentRestriction.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPaymentRestriction dialog
#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __PAYRESTRICTIONDATA_H__
#include "payrestrictiondata.h"
#endif

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

class CPaymentRestriction : public CDialog
{
// Construction
public:
	CPaymentRestriction(CWnd* pParent = NULL);   // standard constructor
	~CPaymentRestriction();

// Dialog Data
	//{{AFX_DATA(CPaymentRestriction)
	enum { IDD = IDD_PAYMENT_RESTRICTION_DIALOG };
	CPayRestrictionData m_Data;
	CDBSheet m_SS;
	CDBSheet m_Ref_SS;

	COptComboBox *m_Portfolio;
	COptComboBox *m_Ref;
	CDocData *m_pDocData;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaymentRestriction)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	COraLoader &GetOraLoader() { return m_pDocData->GetOraLoader(); }
	CDocData *GetData() { return m_pDocData; }

	// Generated message map functions
	//{{AFX_MSG(CPaymentRestriction)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeArPortfolioCombo();
	afx_msg void OnSelchangeArPaymentCombo();
	afx_msg void OnDblClickArPayInstructionList(long Col, long Row);
	afx_msg void OnDblClickArList(long Col, long Row);
	afx_msg void OnBlockSelectedArList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnArAddButton();
	afx_msg void OnArUpdateButton();
	afx_msg void OnArDeleteButton();
	virtual void OnCancel();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
