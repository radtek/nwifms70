#ifndef __TICKETCLASS_H__
#define __TICKETCLASS_H__

#pragma once
#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __NWIFMS70DOC_H__
#include "nwifms70doc.h"
#endif

// CTicketClass form view

class CTicketClass : public CFormView
{
	DECLARE_DYNCREATE(CTicketClass)

protected:
	CTicketClass();           // protected constructor used by dynamic creation
	virtual ~CTicketClass();

protected:
	CDBSheet m_SS;
	COraLoader m_OraLoader;

	CMaskIDEdit m_Ticket;
	CCntlEdit m_Asset;
	CCntlEdit m_AssetDesc;
	CNumEdit m_Strike;
	CNumEdit m_Rate;
	CMaskDateEdit m_Date;
	CMaskDateEdit m_Maturity;

	COptComboBox *m_TransType;
	COptComboBox *m_CitiClass;
	BOOL m_bLoaded;

public:
	enum { IDD = IDD_TRADE_CLASS_FORM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	CNWIFMS70Doc *GetDocument();
	CDocData &GetData() { return GetDocument()->m_Data; }

// Operations
public:
	void InitControls();
	BOOL IsExist(const CString Ticket);
	BOOL IsOK();
	BOOL UpdateData(BOOL bSaveandValid = TRUE);
	void AddRecord(const CString Ticket, const CString CitiClass);
	void DeleteRecord(const CString Ticket);
	void LoadRecord();
	void UpdateRecord(const CString Ticket, const CString CitiClass);

public:
	virtual void OnInitialUpdate();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void OnDblClickTradeclassList(long Col, long Row);
	void OnBlockSelectedTradeclassList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnBnClickedTradeclassUpdateButton();
	afx_msg void OnBnClickedTradeclassDeleteButton();
	afx_msg void OnEnChangeTradeclassDateEdit();
	afx_msg void OnBnClickedTradeclassLoadButton();
};

#ifndef _DEBUG  // debug version in CTicketClass.cpp
inline CNWIFMS70Doc* CTicketClass::GetDocument()
   { return (CNWIFMS70Doc*)m_pDocument; }
#endif
#endif

