#ifndef __ORFEEDLG_H__
#define __ORFEEDLG_H__
#pragma once

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __DOCDATA_H__
#include "DocData.h"
#endif

// COrFeeDlg dialog

class COrFeeDlg : public CDialog
{
public:
	COrFeeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COrFeeDlg();

// Dialog Data
	enum { IDD = IDD_OR_FEE_DIALOG };
	COraLoader m_OraLoader;
	CDBSheet m_SS;
	CDocData *m_Data;
	
	COptComboBox m_PB;
	COptComboBox m_CP;
	CNumEdit m_LowLimit;
	CNumEdit m_HighLimit;
	CMaskDateEdit m_FromDate;
	CMaskDateEdit m_ToDate;
	CNumEdit m_OrFee;
	CNumEdit m_OCCFee;
	CString m_ID;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	COraLoader &GetOraLoader() { return m_OraLoader; }
	void LoadData();
	void UpdateData(BOOL bAdd);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COrFeeDlg)
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnEnChangeOrfeeFromEdit();
	afx_msg void OnEnChangeOrfeeToEdit();
	afx_msg void OnChangeOrFeeEdit();
	afx_msg void OnBnClickedOrfeeAddButton();
	afx_msg void OnBnClickedOrfeeUpdateButton();
	afx_msg void OnBnClickedOrfeeDeleteButton();
	void OnDblClickOrfeeList(long Col, long Row);
	void OnBlockSelectedOrfeeList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	DECLARE_EVENTSINK_MAP()
};
#endif
