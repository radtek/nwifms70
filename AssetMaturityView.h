#pragma once

#ifndef __NWIFMS70DOC_H__
#include "nwifms70doc.h"
#endif

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

// CAssetMaturityView form view

class CAssetMaturityView : public CFormView
{
	DECLARE_DYNCREATE(CAssetMaturityView)

// Form Data
public:
	//{{AFX_DATA(CAssetMaturityView)
	enum { IDD = IDD_MATURITY_FORM };
	CDBSheet m_SS;

	CCheckBox *m_Select;

	CMaskIDEdit m_Ticket;
	CMaskIDEdit m_TransNum;

	CCntlEdit m_Asset;
	CCntlEdit m_AssetDesc;

	CMaskDateEdit m_Maturity;
	
	CNumEdit m_Amount;
	CNumEdit m_Rate;

	COptComboBox *m_TransType;
    COptComboBox *m_CP;
	COptComboBox *m_Portfolio;
	COptComboBox *m_OriMaturity;

	CMCComboBox m_Dir;
	CString m_InvNum;
	//}}AFX_DATA

// Attributes
public:
	CNWIFMS70Doc *GetDocument();
	CDocData &GetData() { return GetDocument()->m_Data; }

protected:
	CAssetMaturityView();           // protected constructor used by dynamic creation
	virtual ~CAssetMaturityView();

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
	void LoadData();
	void SetSearchString(CString &SearchString, CString &Data, BOOL bApply = TRUE);
	void Search();

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	void OnDblClickAssetmaturityList(long Col, long Row);
	afx_msg void OnEnChangeAssetmaturityMaturityEdit();
	afx_msg void OnEnKillfocusAssetmaturityTicketEdit();
	afx_msg void OnEnKillfocusAssetmaturityTransnumEdit();
	afx_msg void OnEnKillfocusAssetmaturityAssetcodeEdit();
	afx_msg void OnEnKillfocusAssetmaturityAssetEdit();
	afx_msg void OnCbnSelchangeAssetmaturityFundCombo();
	afx_msg void OnEnKillfocusAssetmaturityOrimaturityEdit();
	afx_msg void OnBnClickedAssetmaturityClearscrnButton();
	afx_msg void OnBnClickedAssetmaturitySelectCheck();
	afx_msg void OnCbnSelchangeAssetmaturityTranstypeCombo();
	afx_msg void OnCbnSelchangeAssetmaturityDirCombo();
	afx_msg void OnCbnSelchangeAssetmaturityCpCombo();
	afx_msg void OnCbnSelchangeAssetmaturityOrimaturityCombo();
};

#ifndef _DEBUG  // debug version in CTicketEntry.cpp
inline CNWIFMS70Doc* CAssetMaturityView::GetDocument()
   { return (CNWIFMS70Doc*)m_pDocument; }
#endif
