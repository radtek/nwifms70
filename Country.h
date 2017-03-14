#ifndef __COUNTRY_H__
#define __COUNTRY_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Country.h : header file
//
#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __COUNTRYDATA_H__
#include "countrydata.h"
#endif

#ifndef __MASKDATEEDIT_H__
#include "maskdateedit.h"
#endif

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif
/////////////////////////////////////////////////////////////////////////////
// CCountry dialog

class CCountry : public CDialog
{
// Construction
public:
	CCountry(CWnd* pParent = NULL);   // standard constructor
	~CCountry();

// Dialog Data
	//{{AFX_DATA(CCountry)
	enum { IDD = IDD_COUNTRY_DIALOG };
	CCountryData m_Data;
	CDocData *m_pData;
	CDBSheet m_SS;
	
	CCntlEdit m_Country;
	CCntlEdit m_Code;
	CMCComboBox m_Code2;
	COptComboBox *m_OldCountry;
	COptComboBox *m_GeoArea;
	COptComboBox *m_Pru;
	COptComboBox *m_PFRegion;
	COptComboBox *m_ProfileRegion;
	CCntlEdit m_Currency;
	COptComboBox *m_NewCurrency;
	CNumEdit m_Conversion;
	CMaskDateEdit m_ConvDate;
	CCheckBox *m_SovFlag;
	CCheckBox *m_SecFlag;
	CCheckBox *m_DMFlag;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCountry)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void EnableCtrls();

	// Generated message map functions
	//{{AFX_MSG(CCountry)
	virtual BOOL OnInitDialog();
	afx_msg void OnCountryAddButton();
	afx_msg void OnCountryClearButton();
	afx_msg void OnCountryDeleteButton();
	afx_msg void OnCountryFindButton();
	afx_msg void OnCountryUpdateButton();
	afx_msg void OnBlockSelectedCountryList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickCountryList(long Col, long Row);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
