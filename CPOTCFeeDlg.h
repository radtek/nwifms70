#ifndef __CPOTCFEEDLG_H__
#define __CPOTCFEEDLG_H__

#pragma once

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __COUNTRYDATA_H__
#include "countrydata.h"
#endif

#ifndef __COMBOX_H__
#include "combobox.h"
#endif

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

// CCPOTCFeeDlg dialog

class CCPOTCFeeDlg : public CDialog
{
	DECLARE_DYNAMIC(CCPOTCFeeDlg)

public:
	CCPOTCFeeDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCPOTCFeeDlg() {}

// Dialog Data
	enum { IDD = IDD_OTC_FEE_SCHEDULE_DIALOG };
	COraLoader m_OraLoader;
	CDBSheet m_SS;
	CDocData *m_pData;
	COptComboBox m_CP;
	COptComboBox m_TransType;
	COptComboBox m_Currency;
	COptComboBox m_FeeType;
	CMaskDateEdit m_FromDate;
	CMaskDateEdit m_ToDate;
	CNumEdit m_LowLimit;
	CNumEdit m_HighLimit;
	CNumEdit m_Fee;
	CString m_RowID;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	void EnableCtrls();
	BOOL IsOK(int nAction);
	void RowToControls();

	// Generated message map functions
	//{{AFX_MSG()
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void BlockSelectedOtcList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	void DblClickOtcList(long Col, long Row);
	afx_msg void OnBnClickedOtcLoadButton();
	afx_msg void OnBnClickedOtcAddButton();
	afx_msg void OnBnClickedOtcUpdateButton();
	afx_msg void OnBnClickedOtcDeleteButton();
};
#endif