#ifndef __CPDIALOG_H__
#define __CPDIALOG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CPDialog.h : header file
//

#ifndef __ORALOADER_H__
#include "oraLoader.h"
#endif

#ifndef __CPDATA_H__
#include "cpdata.h"
#endif

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif
/////////////////////////////////////////////////////////////////////////////
// CCPDialog dialog

class CCPDialog : public CDialog
{
// Construction
public:
	CCPDialog(CWnd* pParent = NULL);   // standard constructor
	~CCPDialog();

// Dialog Data
	//{{AFX_DATA(CCPDialog)
	enum { IDD = IDD_CP_DIALOG };
	CCPData m_Data;
	CDocData *m_pData;
	CDBSheet m_SS;
	CDBSheet m_CTSS;

	CCntlEdit m_CPCode;
	CCntlEdit m_CP;
	CCntlEdit m_ImgCP;
	CNumEdit m_Limit;
	CNumEdit m_SetLimit;
	CCheckBox *m_Pru;
	CCheckBox *m_Active;

	CCntlEdit m_CTCode;
	CCntlEdit m_Name;
	CCntlEdit m_CT;
	CCntlEdit m_Address;
	CCntlEdit m_Address2;
	CCntlEdit m_Phone;
	CCntlEdit m_Fax;
	CCntlEdit m_Cedel;
	CCntlEdit m_EuroClear;
	CCntlEdit m_MSTC;
	CCntlEdit m_FedID;
	CNumEdit m_CTLimit;
	CCntlEdit m_Isin;
	CCntlEdit m_Bic;
	CCntlEdit m_DTC;
	CCntlEdit m_Group;
	CCntlEdit m_SecID;
	CCntlEdit m_FSAID;
	CCheckBox *m_CTIntl;
	CCheckBox *m_CTFx;
	CCheckBox *m_CTActive;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void EnableCtrls();

	// Generated message map functions
	//{{AFX_MSG(CCPDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnCpAddButton();
	afx_msg void OnCpDeleteButton();
	afx_msg void OnCpFindButton();
	afx_msg void OnCpUpdateButton();
	afx_msg void OnDblClickCpCtList(long Col, long Row);
	afx_msg void OnBlockSelectedCpCtList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickCpCpList(long Col, long Row);
	afx_msg void OnBlockSelectedCpCpList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnCpClearButton();
	afx_msg void OnKillfocusCPCPCodeEdit();
	afx_msg void OnKillfocusCPEdit();
	afx_msg void OnCpAddCtButton();
	afx_msg void OnCpDeleteCtButton();
	afx_msg void OnCpUpdateCtButton();
	afx_msg void OnKillfocusCpCtcodeEdit();
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCpCtfxCheck();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
