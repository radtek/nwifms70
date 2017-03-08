#if !defined(AFX_CONTRACTCONVERTOR_H__0E3D4108_C35C_4810_980F_4D95EA6F3E0E__INCLUDED_)
#define AFX_CONTRACTCONVERTOR_H__0E3D4108_C35C_4810_980F_4D95EA6F3E0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ContractConvertor.h : header file
//

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif
/////////////////////////////////////////////////////////////////////////////
// CContractConvertor dialog

class CContractConvertor : public CDialog
{
// Construction
public:
	CContractConvertor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CContractConvertor)
	enum { IDD = IDD_CONTRACT_CONVERTOR_DIALOG };
	COraLoader m_OraLoader;
	CString m_AssetStr;
	CString m_NominalStr;
	double m_dFactor;
	double m_dNominal;
	CCntlEdit m_Asset;
	CCntlEdit m_Desc;
	CNumEdit m_Factor;
	CNumEdit m_Contract;
	CNumEdit m_Nominal;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CContractConvertor)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CContractConvertor)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnKillfocusContractContractEdit();
	afx_msg void OnKillfocusContractFactorEdit();
	afx_msg void OnKillfocusContractNominalEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTRACTCONVERTOR_H__0E3D4108_C35C_4810_980F_4D95EA6F3E0E__INCLUDED_)
