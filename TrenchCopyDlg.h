#ifndef __TRECHCOPYDLG_H__ 
#define __TRECHCOPYDLG_H__ 

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AgentDlg.h : header file
//

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

#ifndef __CNTLEDIT_H__
#include "cntledit.h"
#endif

#ifndef __COMBOX_H__
#include "combox.h"
#endif

#ifndef __CHECKBOX_H__
#include "checkbox.h"
#endif

// CTrenchCopyDlg dialog

class CTrenchCopyDlg : public CDialog
{
	DECLARE_DYNAMIC(CTrenchCopyDlg)

public:
	CTrenchCopyDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTrenchCopyDlg();

// Dialog Data
	enum { IDD = IDD_TRENCHCOPY_DIALOG };
	COraLoader m_OraLoader;
	CDBSheet m_SS;

	CCntlEdit m_Ticket;
	COptComboBox *m_TrenchCode;
	CString m_sTicket;
	CString m_sTrenchCode;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeTrenchCodeCombo();
	afx_msg void OnBnClickedOk();
};
#endif