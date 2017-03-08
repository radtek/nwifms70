#ifndef __PRIMECPMARGIN_H__
#define __PRIMECPMARGIN_H__
#pragma once

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

#ifndef __COMBOX_H__
#include "combox.h"
#endif

#ifndef __CHECKBOX_H__
#include "checkbox.h"
#endif

// CPrimeCPMargin dialog

class CPrimeCPMargin : public CDialog
{
	DECLARE_DYNAMIC(CPrimeCPMargin)

public:
	CPrimeCPMargin(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPrimeCPMargin();

// Dialog Data
	enum { IDD = IDD_PMMARGIN_DIALOG };
	COraLoader m_OraLoader;
	CDBSheet m_SS;
	COptComboBox *m_Fund;
	COptComboBox *m_AssetClass;
	COptComboBox *m_TransType;
	CCheckBox *m_MarginCheck;
	int m_nRow;
	BOOL m_bPowerUser;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void LoadData();
	BOOL UpdateRecord(BOOL bAddNew = TRUE);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrimCPMargin)
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void OnDblClickPmList(long Col, long Row);
	void OnBlockSelectedPmList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnCbnSelchangePmFundCombo();
	afx_msg void OnCbnSelchangePmAssetclassCombo();
	afx_msg void OnBnClickedPmAddButton();
	afx_msg void OnBnClickedPmUpdateButton();
	afx_msg void OnBnClickedPmDeleteButton();
	afx_msg void OnCbnSelchangePmTranstypeCombo();
};
#endif