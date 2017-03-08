// inprice.h : header file
//
//{{AFX_INCLUDES()
#include "spreadsheet.h"
//}}AFX_INCLUDES
#ifndef __FLOATREPO_H__
#define __FLOATREPO_H__

#ifndef _MSROWCTRL_H
#include "msrowctrl.h"
#endif

#ifndef __FLOATREPODATA_H__
#include "floatrepodata.h"
#endif

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

#ifndef __MASKDATEEDIT_H__
#include "maskdateedit.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CFloatRepo dialog
class CFloatRepo : public CDialog
{
// Construction
public:
	CFloatRepo(CWnd* pParent = NULL);	// standard constructor
	~CFloatRepo();

// Dialog Data
	//{{AFX_DATA(CFloatRepo)
	enum { IDD = IDD_INPUT_REPORATE_DIALOG };
	CMaskDateEdit m_FromDate;
	CMaskDateEdit m_ToDate;
	CNumEdit m_Rate;
	CNumEdit m_Days;
	CNumEdit m_Interest;

	CMaskIDEdit m_TransNum;
	CCntlEdit m_Portfolio;
	CCntlEdit m_AssetCode;
	CCntlEdit m_Asset;
	CCntlEdit m_Currency;
	CCntlEdit m_RateBasis;
	CMaskDateEdit m_ValueDate;
	CMaskDateEdit m_Maturity;
	CNumEdit m_NomAmount;
	CNumEdit m_Price;	
	CNumEdit m_Amort;
	CNumEdit m_IntRate;
	CNumEdit m_CapRate;
	CNumEdit m_Plus;
	CNumEdit m_Amount;
	
	CNumEdit m_TotalInterest;
	CNumEdit m_Total;

	COptComboBox *m_Date;
	COptComboBox *m_CP;
	CDocData *m_pData;
	CFloatRepoData m_Data;
	CDBSheet m_SS;
	CDBSheet m_DetailSS;

	CString m_Dates;
	//}}AFX_DATA

// Attributes
public:	

private:
	void ComputeInterest();
	void UpdateDates();
	void SetTotal();
	BOOL IsOK(BOOL bShow = TRUE);
	void AddDates();
	void SetDates();
	CDocData *GetData() { return m_pData; }

// Overrides
public:

protected:
	virtual void DoDataExchange(CDataExchange *pDX);	// DDX/DDV support
	BOOL OnInitDialog();
	

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFloatRepo)	
	afx_msg void OnAddButton();
	afx_msg void OnUpdateButton();
	afx_msg void OnDeleteButton();
	afx_msg void OnSelchangeDateCombo();
	afx_msg void OnSelchangeCounterpartyCombo();
	afx_msg void OnDblClickRepoList(long Col, long Row);
	afx_msg void OnDblClickRepoDetailList(long Col, long Row);
	afx_msg void OnBlockSelectedRepoDetailList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnBlockSelectedRepoList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnRepoDef();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // __INREPO_H__
