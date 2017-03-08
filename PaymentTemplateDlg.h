#if !defined(AFX_PAYMENTTEMPLATEDLG_H__819BEBBA_55A4_48F0_A8D5_D44E797A3A10__INCLUDED_)
#define AFX_PAYMENTTEMPLATEDLG_H__819BEBBA_55A4_48F0_A8D5_D44E797A3A10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PaymentTemplateDlg.h : header file
//
#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __PAYMENTDATA_H__
#include "paymentdata.h"
#endif

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif
/////////////////////////////////////////////////////////////////////////////
// CPaymentTemplateDlg dialog

class CPaymentTemplateDlg : public CDialog
{
// Construction
public:
	CPaymentTemplateDlg(CWnd* pParent = NULL);   // standard constructor
	~CPaymentTemplateDlg();

// Dialog Data
	//{{AFX_DATA(CPaymentTemplateDlg)
	enum { IDD = IDD_PAY_TEMPLATE_DIALOG };
	CDocData *m_pData;
	CPayTemplate m_Data;
	CDBSheet m_SS;
	CDBSheet m_FundSS;
	CCntlEdit m_ID;
	COptComboBox *m_CP;
	COptComboBox *m_Currency;
	COptComboBox *m_Bank;
	COptComboBox *m_Portfolio;
	CCntlEdit m_Account;
	CCntlEdit m_Beneficiary;
	CCntlEdit m_BeneficiaryID;
	CCntlEdit m_BeneficiaryBIC;
	CCntlEdit m_BeneficiaryAccount;
	CCntlEdit m_FFC;
	CCntlEdit m_FFCID;
	CCntlEdit m_FFCBIC;
	CCntlEdit m_FFCAccount;
	CCntlEdit m_CPRef;
	CCheckBox *m_Active;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPaymentTemplateDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL AddFundList();
	void LoadFundSheet();

	// Generated message map functions
	//{{AFX_MSG(CPaymentTemplateDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBlockSelectedPayTemplateInstructionList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickPayTemplateInstructionList(long Col, long Row);
	afx_msg void OnPayTemplateAddButton();
	afx_msg void OnPayTemplateClearButton();
	afx_msg void OnPayTemplateDeleteButton();
	afx_msg void OnPayTemplateFindButton();
	afx_msg void OnPayTemplateUpdateButton();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPayTemplateAddfundButton();
	afx_msg void OnBnClickedPayTemplateDeletefundButton();
	void OnClickPayTemplateInstructionFundList(long Col, long Row);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAYMENTTEMPLATEDLG_H__819BEBBA_55A4_48F0_A8D5_D44E797A3A10__INCLUDED_)
