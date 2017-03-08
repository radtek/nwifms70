#ifndef __FUNDSIZING_H__
#define __FUNDSIZING_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FundSizing.h : header file
//

#ifndef __NWIFMS70DOC_H__
#include "nwifms70doc.h"
#endif

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

// CFundSizing form view

class CFundSizing : public CFormView
{
	DECLARE_DYNCREATE(CFundSizing)

protected:
	CFundSizing();           // protected constructor used by dynamic creation
	virtual ~CFundSizing();

public:
	enum { IDD = IDD_FUND_SIZING_FORM };
	COraLoader m_OraLoader;
	CDBSheet m_SS;
	
	CMaskDateEdit m_Maturity;
	
	COptComboBox *m_Date;
	COptComboBox *m_Trader;
	COptComboBox *m_Portfolio;
	COptComboBox *m_TransType;
	COptComboBox *m_Reason;
	CMCComboBox m_Bucket;
	CMaskIDEdit m_TransNum;
	CNumEdit m_Strike;
	CNumEdit m_Rate;
	CNumEdit m_Nominal;
	CNumEdit m_Amount;
	CNumEdit m_Price;
	CCntlEdit m_Asset;
	CCntlEdit m_Desc;
	CCntlEdit m_Note;
	BOOL m_bLoaded;

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	void InitControls();
	BOOL UpdateData(BOOL bSaveAndValid = TRUE);
	CNWIFMS70Doc *GetDocument();
	CDocData &GetData() { return GetDocument()->m_Data; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL UpdateRecord();

	DECLARE_MESSAGE_MAP()
public:
	void OnInitialUpdate();
	afx_msg void OnBnClickedFsLoadButton();
	afx_msg void OnBnClickedFsUpdateButton();
	DECLARE_EVENTSINK_MAP()
	void OnDblClickFsList(long Col, long Row);
	afx_msg void OnBnClickedFsClearButton();
};

#ifndef _DEBUG  // debug version in FundSizing.cpp
inline CNWIFMS70Doc* CFundSizing::GetDocument()
   { return (CNWIFMS70Doc*)m_pDocument; }
#endif

#endif