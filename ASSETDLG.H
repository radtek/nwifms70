#ifndef __ASSETDLG_H__
#define __ASSETDLG_H__

// assetdlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAssetDlg dialog
#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

#ifndef __COMBOX_H__
#include "combox.h"
#endif

#ifndef __MASKDATEEDIT_H__
#include "maskdateedit.h"
#endif

#ifndef __NUMEDIT_H__
#include "numedit.h"
#endif

#ifndef __ASSETFINDDATA_H__
#include "assetfinddata.h"
#endif

#ifndef __ASSETREC_H__
#include "assetrec.h"
#endif

#define FINDACTION_OK		1
#define FINDACTION_ADD		2
#define FINDACTION_UNWIND	3
#define FINDACTION_OPTEX	4


class CAssetDlg : public CDialog
{
// Construction
public:
	CAssetDlg(CWnd* pParent = NULL);        // standard constructor
	~CAssetDlg() {}

// Dialog Data
	//{{AFX_DATA(CAssetDlg)
	enum { IDD = IDD_ASSET_DIALOG };
	CDocData *m_pData;
	CAssetFindData m_FindData;
	CDBSheet m_SS;
	CDBSheet m_HoldingSS;
	CCntlEdit m_AssetEdit;
	CCntlEdit m_AssetDescEdit;
	CCntlEdit m_AssetDesc2Edit;
	CCntlEdit m_CommonEdit;
	CCntlEdit m_CusipEdit;
	CCntlEdit m_IsinEdit;
	CCntlEdit m_SedolEdit;
	CCntlEdit m_PID;
	CMaskDateEdit m_MaturityEdit;
	CCntlEdit m_SourceEdit;
	CCntlEdit m_Underline;
	CCntlEdit m_UnderlineID;
	CNumEdit m_PositionEdit;
	CNumEdit m_ItemsEdit;
	CNumEdit m_ParEdit;
	CNumEdit m_StrikeEdit;
	CMaskIDEdit m_TicketEdit;
	COptComboBox m_ClassCombo;
	COptComboBox m_CountryCombo;
	COptComboBox m_CurrencyCombo;
	COptComboBox m_IndustryCombo;
	COptComboBox m_BucketCombo;
	COptComboBox m_CategoryCombo;
	COptComboBox m_TransTypeCombo;
	COptComboBox m_DirCombo;

	BOOL m_bCash;
	BOOL m_bTicket;
	int m_nActionID;
	CString m_Future;
	CString m_EuropeBond;
	CString m_OptListed;
	CString m_Rev;
	BOOL m_bWithNew;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);        // DDX/DDV support

	void EnableButtons(BOOL bEnable = TRUE);
	void EnableCtrls(BOOL bEnable = TRUE);
	void EnableFind(CEdit& Edit, BOOL bAsset = FALSE);
	void EnableFind(COptComboBox &Combo);
	void SetControls(BOOL bEnableDesc);
	CDocData &GetData() { return *m_pData; }

	// Generated message map functions
	//{{AFX_MSG(CAssetDlg)
	afx_msg void OnFindButton();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnRefreshButton();
	afx_msg void OnSelchangeAssetBucketCombo();
	afx_msg void OnSelchangeAssetCategoryCombo();
	afx_msg void OnSelchangeAssetClassCombo();
	afx_msg void OnSelchangeAssetCountryCombo();
	afx_msg void OnSelchangeAssetCurrencyCombo();
	afx_msg void OnSelchangeAssetIndustryCombo();
	afx_msg void OnChangeAssetdescEdit();
	afx_msg void OnChangeAssetCommonEdit();
	afx_msg void OnChangeAssetIsinEdit();
	afx_msg void OnChangeAssetMaturityEdit();
	afx_msg void OnChangeAssetMstcEdit();
	afx_msg void OnChangeAssetSedolEdit();
	afx_msg void OnChangeAssetSourceEdit();
	afx_msg void OnDblClickItemList(long Col, long Row);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void OnDblClickAssetdlgHoldingList(long Col, long Row);
	afx_msg void OnBnClickedAssetdlgUnwindButton();
	afx_msg void OnBnClickedAssetdlgOptexButton();
	afx_msg void OnBnClickedAssetdlgAddposButton();
	afx_msg void OnEnChangeAssetdlgAssetEdit();
};
#endif