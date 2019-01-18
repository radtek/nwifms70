#pragma once

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

// CCategoryDlg dialog

class CCategoryDlg : public CDialog
{
	DECLARE_DYNAMIC(CCategoryDlg)

public:
	CCategoryDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCategoryDlg();

// Dialog Data
	enum { IDD = IDD_CATEGORY_DIALOG };
	COraLoader m_OraLoader;
	CDocData *m_pData;
	CDBSheet m_SS;
	CCntlEdit m_Strategy;
	CNumEdit m_Index;
	CMCComboBox m_Trader;
	CCntlEdit m_CatCode;
	BOOL m_bModified;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void EnableButtons();
	BOOL IsOK();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void DblClickCatList(long Col, long Row);
	void BlockSelectedCatList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnBnClickedCatClearButton();
	afx_msg void OnBnClickedCatAddButton();
	afx_msg void OnBnClickedCatUpdateButton();
	afx_msg void OnBnClickedCatDeleteButton();
};
