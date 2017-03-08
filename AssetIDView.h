#ifndef __ASSETIDVIEW_H__
#define __ASSETIDVIEW_H__
#pragma once

#ifndef __NWIFMS70DOC_H__
#include "nwifms70doc.h"
#endif

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif


// CAssetIDView form view

class CAssetIDView : public CFormView
{
	DECLARE_DYNCREATE(CAssetIDView)

protected:
	CAssetIDView();           // protected constructor used by dynamic creation
	virtual ~CAssetIDView();

public:
	enum { IDD = IDD_ASSETID_FORM };
	CDBSheet m_SS;

	COptComboBox *m_Portfolio;
	COptComboBox *m_TransType;
	
	CCntlEdit m_AssetCode;
	CCntlEdit m_Asset;
	CCntlEdit m_PID;

	CNumEdit m_Nominal;
	CNumEdit m_Rate;
	CMaskDateEdit m_Maturity;
	int m_Row;

// Attributes
public:
	CNWIFMS70Doc *GetDocument();
	CDocData &GetData() { return GetDocument()->m_Data; }

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	virtual void OnInitialUpdate();
	BOOL UpdateData(BOOL bSaveandValid = TRUE);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void OnDblClickAssetidList(long Col, long Row);
	void OnBlockSelectedAssetidList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnEnChangeAssetidPidEdit();
	afx_msg void OnBnClickedAssetidUpdateButton();
	afx_msg void OnBnClickedAssetidDeleteButton();
};

#ifndef _DEBUG  // debug version in CTicketEntry.cpp
inline CNWIFMS70Doc* CAssetIDView::GetDocument()
   { return (CNWIFMS70Doc*)m_pDocument; }
#endif

#endif