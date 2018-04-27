#pragma once

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

#ifndef __CHECKBOX_H__
#include "checkbox.h"
#endif

#ifndef __CNTLEDIT_H__
#include "cntledit.h"
#endif

// COPsDlg dialog

class COPsDlg : public CDialog
{
	DECLARE_DYNAMIC(COPsDlg)

public:
	COPsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COPsDlg();

// Dialog Data
	enum { IDD = IDD_OPERATION_DIALOG };
	CDBSheet m_SS;
	COraLoader m_OraLoader;

	CCntlEdit m_OpsNameEdit;
	CCntlEdit m_FMSNameEdit;
	CCheckBox m_Active;
	CCheckBox m_PowerUser;
	CCheckBox m_Guest;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void GetData(CDBRec &Rec);
	COraLoader &GetOraLoader() { return m_OraLoader; }
	BOOL IsOK(BOOL bNew);
	int LoadOps();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void DblClickOpsList(long Col, long Row);
	void BlockSelectedOpsList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);

	afx_msg void OnBnClickedOpsLoadButton();
	afx_msg void OnBnClickedOpsAddButton();
	afx_msg void OnBnClickedOpsUpdateButton();
	afx_msg void OnBnClickedOpsDeleteButton();

	afx_msg void OnEnChangeOpsNameEdit();
};
