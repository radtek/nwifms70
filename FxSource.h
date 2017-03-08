#pragma once

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

// CFxSource dialog

class CFxSource : public CDialog
{
	DECLARE_DYNAMIC(CFxSource)
public:
	CDocData *m_pData;
	CDBSheet m_SelSS;
	CDBSheet m_SS;
	CMaskIDEdit m_Ticket;
	CCntlEdit m_Currency;
	CCntlEdit m_ValueDate;
	CNumEdit m_Fxrate;
	CNumEdit m_NomAmount;
	CNumEdit m_Amount;
	CNumEdit m_Remain;
	
	CDBRecArray m_RecArray;
	CString m_sTicket;
	CString m_sCurrency;
	CString m_sValueDate;
	CString m_sFxrate;
	CString m_sNomAmount;
	CString m_sAmount;

public:
	CFxSource(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFxSource();

// Dialog Data
	enum { IDD = IDD_FX_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void UpdateData(BOOL bAdd);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	DECLARE_EVENTSINK_MAP()
	void OnDblClickFxSelList(long Col, long Row);
	void OnDblClickFxList(long Col, long Row);
};
