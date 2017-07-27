#ifndef __OTHERFEEDLG_H__
#define __OTHERFEEDLG_H__
#pragma once

#ifndef __NUMEDIT_H__
#include "numedit.h"
#endif

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __DOCDATA_H__
#include "DocData.h"
#endif 

// COtherFeeDlg dialog

class COtherFeeDlg : public CDialog
{
	DECLARE_DYNAMIC(COtherFeeDlg)

public:
	COtherFeeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COtherFeeDlg();

// Dialog Data
	enum { IDD = IDD_OTHERFEE_DIALOG };
	CNumEdit m_SecFee;
	CNumEdit m_OrFee;
	CNumEdit m_TotalFee;

	CString m_PB;
	CString m_CP;
	
	CString m_TransType;
	CString m_Dir;
	CString m_Asset;
	double m_dNomAmount;
	double m_dPrice;
	double m_dFxrate;
	double m_dBrFee;
	double m_dOtherFee;

	double m_dSecFee;
	double m_dOrFee;
	BOOL m_bSecFee;
	BOOL m_bOrFee;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void ComputeFees();

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
#endif