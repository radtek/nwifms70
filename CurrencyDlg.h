#ifndef __CURRENCYDLG_H__
#define __CURRENCYDLG_H__

// CCurrencyDlg dialog

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Country.h : header file
//

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __CURRENCYDATA_H__
#include "currencydata.h"
#endif

#ifndef __MASKDATEEDIT_H__
#include "maskdateedit.h"
#endif

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

class CCurrencyDlg : public CDialog
{
	DECLARE_DYNAMIC(CCurrencyDlg)

public:
	CCurrencyDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCurrencyDlg();

// Dialog Data
	enum { IDD = IDD_CURRENCY_DIALOG };
	CCurrencyData m_Data;
	CDocData *m_pData;
	CDBSheet m_SS;

	CCntlEdit m_Currency;
	CCntlEdit m_NewCurrency;

	CNumEdit m_Conversion;
	CMaskDateEdit m_ConvDate;

	COptComboBox* m_DBPBTier;
	COptComboBox* m_HSBCTier;
	
	CCheckBox* m_Inverted;
	CCheckBox* m_NDF_Fix;
	CCheckBox* m_1256;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

// Implementation
protected:
	void EnableCtrls();

	// Generated message map functions
	//{{AFX_MSG(CCountry)
	virtual BOOL OnInitDialog();
public:
	DECLARE_EVENTSINK_MAP()
	void OnDblClickCurrencyList(long Col, long Row);
	void OnBlockSelectedCurrencyList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnBnClickedCurrencyFindButton();
	afx_msg void OnBnClickedCurrencyClearButton();
	afx_msg void OnBnClickedCurrencyAddButton();
	afx_msg void OnBnClickedCurrencyUpdateButton();
	afx_msg void OnBnClickedCurrencyDeleteButton();
};
#endif