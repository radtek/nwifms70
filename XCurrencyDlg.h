#if !defined(AFX_XCURRENCYDLG_H__B84A170F_9BEB_48DB_9B39_537E8D74F047__INCLUDED_)
#define AFX_XCURRENCYDLG_H__B84A170F_9BEB_48DB_9B39_537E8D74F047__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XCurrencyDlg.h : header file
//
#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

#ifndef __COMBOX_H__
#include "combox.h"
#endif

#ifndef __NUMEDIT_H__
#include "numedit.h"
#endif

#ifndef __ASSETFINDDATA_H__
#include "assetfinddata.h"
#endif

#ifndef __ASSETREC_H__
#include "assetrec.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CXCurrencyDlg dialog

class CXCurrencyDlg : public CDialog
{
// Construction
public:
	CXCurrencyDlg(CWnd* pParent = NULL);   // standard constructor
	~CXCurrencyDlg();

// Dialog Data
	//{{AFX_DATA(CXCurrencyDlg)
	enum { IDD = IDD_XCURRENCY_DIALOG };
	CDocData *m_pData;
	CDBSheet m_SS;
	COraLoader m_OraLoader;
	COptComboBox *m_CategoryCombo;
	COptComboBox *m_CounterpartyCombo;
	COptComboBox *m_CurrencyCombo;
	COptComboBox *m_ValueDateCombo;
	COptComboBox *m_PortfolioCombo;
	CNumEdit m_NominalEdit;
	CNumEdit m_Nominal2Edit;
	CNumEdit m_UnWindNominalEdit;
	CNumEdit m_UnWindNominal2Edit;
	CNumEdit m_PriceEdit;
	CNumEdit m_NetEdit;
	CNumEdit m_Net2Edit;
	int m_UserClass;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXCurrencyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ComputeData();
	void ComputeNominal(int Row, int Row2);
	void EnableButton();
	CDocData &GetData() { return *m_pData; }

	// Generated message map functions
	//{{AFX_MSG(CXCurrencyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeXcurrencyCpCombo();
	afx_msg void OnSelchangeXcurrencyCurrencyCombo();
	afx_msg void OnChangeXcurrencyVdateCombo();
	afx_msg void OnChangeXcurrencyPriceEdit();
	afx_msg void OnChangeXcurrencyUnwindAmountEdit();
	afx_msg void OnXcurrencyLoadButton();
	afx_msg void OnBlockSelectedXcurrencyList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnSelChangeXcurrencyList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2, long CurCol, long CurRow);
	afx_msg void OnClickXcurrencyList(long Col, long Row);
	afx_msg void OnSelchangeXcurrencyCategoryCombo();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XCURRENCYDLG_H__B84A170F_9BEB_48DB_9B39_537E8D74F047__INCLUDED_)
