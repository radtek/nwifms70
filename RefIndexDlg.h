#ifndef __REFINDEX__H__
#define __REFINDEX__H__
#pragma once

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

// CRefIndexDlg dialog

class CRefIndexDlg : public CDialog
{
	DECLARE_DYNAMIC(CRefIndexDlg)

public:
	CRefIndexDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRefIndexDlg();

// Dialog Data
	enum { IDD = IDD_REFIDX_DIALOG };
	CDBSheet m_SS;
	COraLoader m_OraLoader;
	CCntlEdit m_RefIndex;
	CCntlEdit m_IndexDesc;
	CCntlEdit m_IMIndex;
	CMCComboBox m_CitcoCode;
	CMCComboBox m_ImagineYC;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL UpdateRecord();
	BOOL DeleteRecord();

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	virtual BOOL OnInitDialog();
	void OnDblClickRefidxList(long Col, long Row);
	void OnBlockSelectedRefidxList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnBnClickedRefidxAddButton();
	afx_msg void OnBnClickedRefidxUpdateButton();
	afx_msg void OnBnClickedRefidxDeleteButton();
	afx_msg void OnEnChangeRefidxIdxEdit();
};
#endif