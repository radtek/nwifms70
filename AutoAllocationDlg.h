#pragma once

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

#ifndef __AUTOALLOCDATA_H__
#include "autoallocdata.h"
#endif

// CAutoAllocationDlg dialog

class CAutoAllocationDlg : public CDialog
{
	DECLARE_DYNAMIC(CAutoAllocationDlg)

public:
//	CAssetSupplementData m_Data;
	CDocData *m_pData;
	CAutoAllocData m_Data;
	CDBSheet m_SS;
	
	COptComboBox *m_AAMethod;
	CMaskDateEdit m_FromDate;
	CMaskDateEdit m_ToDate;
	COptComboBox *m_Portfolio;
	CNumEdit m_Alloc;

	CAutoAllocationDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAutoAllocationDlg();

// Dialog Data
	enum { IDD = IDD_AUTOALLOCATION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void EnableCtrls();

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	virtual BOOL OnInitDialog();
	void OnDblClickAaList(long Col, long Row);
	void OnBlockSelectedAaList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnBnClickedAaFindButton();
	afx_msg void OnBnClickedAaClearButton();
	afx_msg void OnBnClickedAaAddButton();
	afx_msg void OnBnClickedAaUpdateButton();
	afx_msg void OnBnClickedAaDeleteButton();
};
