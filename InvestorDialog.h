#ifndef __INVESTORDIALOG_H__
#define __INVESTORDIALOG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPDialog.h : header file
//

#ifndef __ORALOADER_H__
#include "oraLoader.h"
#endif

#ifndef __INVESTORDATA_H__
#include "investordata.h"
#endif

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif
/////////////////////////////////////////////////////////////////////////////
// CInvestorDialog dialog

class CInvestorDialog : public CDialog
{
// Construction
public:
	CInvestorDialog(CWnd* pParent = NULL);   // standard constructor
	~CInvestorDialog();

// Dialog Data
	//{{AFX_DATA(CInvestorDialog)
	enum { IDD = IDD_INVESTOR_DIALOG };
	CInvestorData m_Data;
	CDocData *m_pData;
	CDBSheet m_SS;
	CDBSheet m_AllocSS;

	CCntlEdit m_InvCode;
	CCntlEdit m_Investor;
	CCntlEdit m_Address;
	CCntlEdit m_City;
	COptComboBox *m_Country;
	COptComboBox *m_Agent;
	COptComboBox *m_InvClass;
	CNumEdit m_MgmtFee;
	CNumEdit m_Performance;
	CCheckBox *m_Status;

	COptComboBox *m_Asset;
	CCntlEdit m_TaxID;
	CCheckBox *m_OnShore;
	CCheckBox *m_Erisa;
	CCheckBox *m_Bhca;
	CCheckBox *m_IPO;
	CCheckBox *m_Side;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInvestorDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void EnableCtrls();

	// Generated message map functions
	//{{AFX_MSG(CCPDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnInvestorAddButton();
	afx_msg void OnInvestorDeleteButton();
	afx_msg void OnInvestorFindButton();
	afx_msg void OnInvestorUpdateButton();
	afx_msg void OnDblClickAllocList(long Col, long Row);
	afx_msg void OnBlockSelectedAllocList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickInvestorList(long Col, long Row);
	afx_msg void OnBlockSelectedInvestorList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnInvestorClearButton();
	afx_msg void OnKillfocusIDEdit();
	afx_msg void OnKillfocusAssetEdit();
	afx_msg void OnInvestorAddAssetButton();
	afx_msg void OnInvestorDeleteAssetButton();
	afx_msg void OnInvestorUpdateAssetButton();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
