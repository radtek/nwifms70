#ifndef __SETCONVENTIONDLG_H__
#define __SETCONVENTIONDLG_H__

#pragma once

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

#ifndef __NUMEDIT_H__
#include "numedit.h"
#endif

// CSetConventionDlg dialog

class CSetConventionDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetConventionDlg)

public:
	CDocData *m_pData;
	CDBSheet m_SS;
	CString m_sAsset;
	CString m_sPortfolio;
	CString m_sSetConvention;
	CCntlEdit m_SetConvention;
	COptComboBox *m_CP;
	COptComboBox *m_Portfolio;
	CMaskIDEdit m_TransNum;
	COptComboBox *m_Dir;
	CNumEdit m_NomAmount;

public:
	CSetConventionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetConventionDlg();

// Dialog Data
	enum { IDD = IDD_SETCONVENTION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void LoadSetConvention();
	void AssignData();

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	virtual BOOL OnInitDialog();
	void OnDblClickSetconventionList(long Col, long Row);
	virtual void OnOK();
	virtual void OnCancel();
};
#endif