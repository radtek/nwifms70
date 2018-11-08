#ifndef __YIELDCURVE__H__
#define __YIELDCURVE__H__
#pragma once

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

// CYieldCurveDlg dialog

class CYieldCurveDlg : public CDialog
{
	DECLARE_DYNAMIC(CYieldCurveDlg)

public:
	CYieldCurveDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CYieldCurveDlg();

// Dialog Data
	enum { IDD = IDD_YIELDCURVE_DIALOG };
	CDBSheet m_SS;
	COraLoader m_OraLoader;
	CCntlEdit m_Name;
	CCntlEdit m_Symbol;
	COptComboBox m_Index;
	CMCComboBox m_Term;
	COptComboBox m_Source;
	COptComboBox m_Default;
	COptComboBox m_BBTick;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL UpdateRecord();
	BOOL DeleteRecord();

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	virtual BOOL OnInitDialog();
	void OnDblClickYieldcurveList(long Col, long Row);
	void OnBlockSelectedYieldcurveList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnBnClickedYieldcurveAddButton();
	afx_msg void OnBnClickedYieldcurveUpdateButton();
	afx_msg void OnBnClickedYieldcurveDeleteButton();
	afx_msg void OnEnChangeYieldcurveNameEdit();
};
#endif