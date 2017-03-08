#ifndef __ASSETCLASSDLG_H__
#define __ASSETCLASSDLG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

#ifndef __CNTLEDIT_H__
#include "cntledit.h"
#endif

#ifndef __COMBOX_H__
#include "combox.h"
#endif

#ifndef __CHECKBOX_H__
#include "checkbox.h"
#endif

// CAssetClassDlg dialog

class CAssetClassDlg : public CDialog
{
public:
	CAssetClassDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAssetClassDlg();

// Dialog Data
	//{{AFX_DATA(CAccountDlg)
	enum { IDD = IDD_ASSETCLASS_DIALOG };
	COraLoader m_OraLoader;
	CDBSheet m_SS;
	CCntlEdit m_Class;
	CCntlEdit m_PGroup;
	CCntlEdit m_EGroup;
	COptComboBox *m_Liquidity;
	CCheckBox *m_PGroupCheck;
	int m_Row;
	BOOL m_bPowerUser;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAssetClassDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void LoadData();
	BOOL UpdateRecord(BOOL bAddNew = TRUE);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAssetClassDlg)
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAssetclassClearButton();
	afx_msg void OnBnClickedAssetclassAddButton();
	afx_msg void OnBnClickedAssetclassUpdateButton();
	afx_msg void OnBnClickedAssetclassDeleteButton();
	DECLARE_EVENTSINK_MAP()
	void OnDblClickAssetclassList(long Col, long Row);
	void OnBlockSelectedAssetclassList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	//}}AFX_MSG
	afx_msg void OnEnChangeAssetclassClassEdit();
};
#endif
