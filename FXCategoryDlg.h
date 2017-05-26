#ifndef __FXCATEGORYDLG_H__
#define __FXCATEGORYDLG_H__
#pragma once

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __COMBOX_H__
#include "combobox.h"
#endif

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

// CFXCategoryDlg dialog

class CFXCategoryDlg : public CDialog
{
	DECLARE_DYNAMIC(CFXCategoryDlg)

public:
	CFXCategoryDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFXCategoryDlg() {}

// Dialog Data
	enum { IDD = IDD_FXCAT_DIALOG };
	COraLoader m_OraLoader;
	CDBSheet m_SS;
	CDocData *m_pData;
	COptComboBox m_Trader;
	CMCComboBox m_Category;
	COptComboBox m_Currency;
	CMaskIDEdit	m_Ticket;
	CMaskDateEdit m_TradeDate;
	CNumEdit m_Fxrate;
	CNumEdit m_Nominal;
	CString m_RowID;
	CString m_sTicket;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

protected:
	void EnableCtrls();
	BOOL IsOK();
	void RowToControls();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnEnChangeFxcatTradedateEdit();
	afx_msg void OnCbnSelchangeFxcatCategoryCombo();
	afx_msg void OnBnClickedFxcatLoadButton();
	afx_msg void OnBnClickedFxcatUpdateButton();
	DECLARE_EVENTSINK_MAP()
	void DblClickFxcatList(long Col, long Row);
	void BlockSelectedFxcatList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
};
#endif