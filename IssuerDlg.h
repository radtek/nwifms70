#pragma once

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif
// CIssuerDlg dialog

class CIssuerDlg : public CDialog
{
	DECLARE_DYNAMIC(CIssuerDlg)

public:
	CIssuerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIssuerDlg();

// Dialog Data
	enum { IDD = IDD_ISSUER_DIALOG };
	CDBSheet m_SS;
	COraLoader m_OraLoader;

	CCntlEdit m_Issuer;
	CCntlEdit m_IMIssuer;
	CNumEdit m_OptLimit;
	CNumEdit m_OptLimit2;
	COptComboBox *m_Country;
	COptComboBox *m_Currency;
	COptComboBox *m_FwdCurrency;
	COptComboBox *m_AssetClass;
	CCheckBox *m_SovFlag;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void EnableCtrls();
	BOOL IsOK();
	void LoadData(CDBRec &DBRec);
	void MapData(int nRow);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedIssuerAddButton();
	afx_msg void OnBnClickedIssuerUpdateButton();
	afx_msg void OnBnClickedIssuerDeleteButton();
	DECLARE_EVENTSINK_MAP()
	void OnDblClickIssuerList(long Col, long Row);
	void OnBlockSelectedIssuerList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnBnClickedIssuerClearButton();
	afx_msg void OnEnChangeIssuerIssuerEdit();
};
