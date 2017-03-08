#if !defined(AFX_DAILYINPUT_H__135E6CD4_065D_11D4_A442_00062926CA4A__INCLUDED_)
#define AFX_DAILYINPUT_H__135E6CD4_065D_11D4_A442_00062926CA4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DailyInput.h : header file
//

#ifndef __NWIFMS70DOC_H__
#include "nwifms70doc.h"
#endif

#ifndef __PRICE_H__
#include "price.h"
#endif

#ifndef __MASKDATEEDIT_H__
#include "maskdateedit.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// CDailyInput form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#ifndef __PRICE_H__
#include "price.h"
#endif

#ifndef __MASKDATEEDIT_H__
#include "maskdateedit.h"
#endif

class CDailyInput : public CFormView
{
protected:
	CDailyInput();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDailyInput)

// Form Data
public:
	//{{AFX_DATA(CDailyInput)
	enum { IDD = IDD_INPUT_FORM };
	CPriceDataArray m_DataArray;
	CDBSheet m_SS;
	CMaskDateEdit m_Date;
	int	m_nEditCol;
	CString m_Title;
	BOOL m_bReload;
	int m_MenuID;
	//}}AFX_DATA

// Attributes
public:
	CNWIFMS70Doc *GetDocument();

protected:
	void SetSheetCellColor(int Row, PriceStatus_t &Status);
	void SetSheetRow(int Row, CPriceData *PriceData, BOOL Beep = FALSE);
	void SetSheetSize();

// Operations
public:
	BOOL UpdateData(BOOL bSaveandValid = TRUE);
	void LoadData(int bUpload = FALSE);
	void SaveData();
	void LoadSpreadSheet();
	void SetTitle();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDailyInput)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange *pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDailyInput();
	void DoInput(BOOL bEnable, double WarnLimit, int MenuID, int DataRange);
	void DoSave();
	void UpdateOptionPosition(LPCTSTR QDate, BOOL bFxOpt = FALSE);
	void DoUpdate(LPCTSTR Title, int MenuID, int DataRange);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext &dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDailyInput)
	afx_msg void OnDailyInputFxrate();
	afx_msg void OnDailyInputOption();
	afx_msg void OnDailyInputOptionDelta();
	afx_msg void OnDailyInputVarFactor();
	afx_msg void OnDailyInputPrice();
	afx_msg void OnDailyInputCost();
	afx_msg void OnBlockSelectedInputList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2);
	afx_msg void OnDblClickInputList(long Col, long Row);
	afx_msg void OnEditModeInputList(long Col, long Row, short Mode, BOOL ChangeMade);
	afx_msg void OnDailyInputDelete();
	afx_msg void OnDailyInputCopy();
	afx_msg void OnDailyInputPaste();
	afx_msg void OnUpdateDailyInputDelete(CCmdUI *pCmdUI);
	afx_msg void OnDailyInputFind();
	afx_msg void OnUpdateDailyInputFind(CCmdUI *pCmdUI);
	afx_msg void OnDailyInputSave();
	afx_msg void OnUpdateDailyInputSave(CCmdUI *pCmdUI);
	afx_msg void OnDailyUpdateFxrate();
	afx_msg void OnDailyUpdateSwap();
	afx_msg void OnDailyUpdateOption();
	afx_msg void OnDailyUpdateOptionDelta();
	afx_msg void OnDailyUpdateVarFactor();
	afx_msg void OnDailyUpdatePrice();
	afx_msg void OnDailyInputRepo();
	afx_msg void OnDailyInputNav();
	afx_msg void OnDailyUpdateNav();
	afx_msg void OnDailyInputSwap();
	afx_msg void OnDailyInputDuration();
	afx_msg void OnDailyUpdateDuration();
	afx_msg void OnDailyInputRepoCoupon();
	afx_msg void OnDailyInputBloombergId();
	afx_msg void OnDailyInputRate();
	afx_msg void OnDailyInputVol();
	afx_msg void OnDailyUpdateRate();
	afx_msg void OnDailyUpdateVol();
	afx_msg void OnDailyUpdteIssuerAmount();
	afx_msg void OnUpdateDailyUpdateDuration(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDailyUpdateFxrate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDailyUpdateNav(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDailyUpdateOption(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDailyUpdateOptionDelta(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDailyUpdatePrice(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDailyUpdateRate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDailyUpdateSwap(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDailyUpdateVarFactor(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDailyUpdateVol(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDailyUpdateIssuerAmount(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDailyInputRepo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDailyInputRepoCoupon(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDailyInputBloombergId(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDailyInputNav(CCmdUI* pCmdUI);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDailyUploadOptionvalue();
	afx_msg void OnDailyInputOptionvalue();
	afx_msg void OnDailyUpdateAccrualfactor();
	afx_msg void OnDailyInputAccrualfactor();
	afx_msg void OnUpdateDailyUpdateAccrualfactor(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDailyInputAccrualfactor(CCmdUI *pCmdUI);
	afx_msg void OnDailyUpdateDv01();
	afx_msg void OnDailyInputDv01();
	afx_msg void OnUpdateDailyInputDv01(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDailyUpdateDv01(CCmdUI *pCmdUI);
	afx_msg void OnDailyUpdateAssetrate();
	afx_msg void OnDailyQuickfxrate();
	afx_msg void OnDailyQuickprice();
	afx_msg void OnDailyQuickdividend();
	afx_msg void OnUpdateDailyQuickfxrate(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDailyQuickprice(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDailyQuickdividend(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in DailyInput.cpp
inline CNWIFMS70Doc* CDailyInput::GetDocument()
   { return (CNWIFMS70Doc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DAILYINPUT_H__135E6CD4_065D_11D4_A442_00062926CA4A__INCLUDED_)
