// FundSizing.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "FundSizing.h"
#include "fsrec.h"
#include "qdata.h"


// CFundSizing

IMPLEMENT_DYNCREATE(CFundSizing, CFormView)

CFundSizing::CFundSizing()
	: CFormView(CFundSizing::IDD)
{
	m_Date = NULL;
	m_Trader = NULL;
	m_Portfolio = NULL;
	m_TransType = NULL;
	m_Reason = NULL;
	m_bLoaded = FALSE;
}

CFundSizing::~CFundSizing()
{
	if(m_Date)
		delete m_Date;
	if(m_Trader)
		delete m_Trader;
	if(m_Portfolio)
		delete m_Portfolio;
	if(m_TransType)
		delete m_TransType;
	if(m_Reason)
		delete m_Reason;
}

void CFundSizing::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FS_LIST, m_SS);
}

void CFundSizing::InitControls()
{
	m_SS.SetVisibleRows(10);
	m_SS.SetVisibleCols(12);
	m_SS.SetMaxRows(500);

	m_Maturity.Setup(this, IDC_FS_MATURITY_EDIT);

	m_Date = new COptComboBox(this, IDC_FS_INDATE_COMBO);
	m_Trader = new COptComboBox(this, IDC_FS_TRADER_COMBO, TRUE);
	m_Portfolio = new COptComboBox(this, IDC_FS_PORTFOLIO_COMBO);
	m_TransType = new COptComboBox(this, IDC_FS_TRANS_TYPE_COMBO);
	m_Reason = new COptComboBox(this, IDC_FS_REASON_COMBO, TRUE); 

	m_TransNum.Setup(this, IDC_FS_TRANSNUM_EDIT);
	m_Bucket.Setup(this, IDC_FS_BUCKET_COMBO);
	m_Strike.Setup(this, IDC_FS_STRIKE_EDIT);
	m_Rate.Setup(this, IDC_FS_RATE_EDIT);
	m_Nominal.Setup(this, IDC_FS_NOMINAL_EDIT, NULL, 2);
	m_Amount.Setup(this, IDC_FS_AMOUNT_EDIT, NULL, 2);
	m_Price.Setup(this, IDC_FS_PRICE_EDIT, NULL);
	m_Asset.Setup(this, IDC_FS_ASSET_EDIT);
	m_Desc.Setup(this, IDC_FS_ASSETDESC_EDIT);
	m_Note.Setup(this, IDC_FS_COMMENT_EDIT);
}

BOOL CFundSizing::UpdateData(BOOL bSaveAndValid)
{
	if(bSaveAndValid)
	{
		CString Data;
		if(m_SS.GetSheetCurRow() < 0)
			return FALSE;

		if(UpdateRecord())
		{
			m_Reason->GetSelString(Data);
			m_SS.SetSheetText(6, m_SS.GetSheetCurRow(), Data);
			m_Note.GetWindowText(Data);
			m_SS.SetSheetText(7, m_SS.GetSheetCurRow(), Data);
		}
		return TRUE;
	}
	else
	{
		BeginWaitCursor();
		CString Date, Trader, Portfolio;
		CQData QData;
		
		if(!m_bLoaded)
		{
			GetData().LoadSupportData();
			m_OraLoader.Open("SELECT DISTINCT TRADE_DATE FROM SEMAM.NW_TR_TICKETS A "
							"WHERE TRADE_DATE BETWEEN TRUNC(SYSDATE) - 30 AND TRUNC(SYSDATE) "
							"ORDER BY 1 DESC ");
			m_OraLoader.LoadComboBox(*m_Date);
			if(m_Date->GetCount() > 0)
				m_Date->SetCurSel(0);
			m_Portfolio->AddString("EMCF_B");
			m_Portfolio->AddString("BNAMTR");
			GetData().GetTransTypeArr().CopyToComboBox(*m_TransType); /* Trans Type */
			m_TransType->AddString("TRS");
			m_OraLoader.Open("SELECT BUCKET, BUCKET_DESC FROM SEMAM.NW_BUCKETS3 ORDER BY 1");
			m_OraLoader.LoadMCComboBox(m_Bucket);
			m_OraLoader.Open("SELECT REASON FROM SEMAM.NW_FUND_SIZING_REASON ORDER BY 1 ");
			m_OraLoader.LoadComboBox(*m_Reason);
			GetData().GetTraderArr().CopyToComboBox(*m_Trader);
			m_bLoaded = TRUE;
		}
		
		m_Date->GetSelString(Date);
		Date = QData.GetQueryText(Date);
		m_Trader->GetSelString(Trader);
		Trader = QData.GetQueryText(Trader);
		m_Portfolio->GetSelString(Portfolio);
		Portfolio = QData.GetQueryText(Portfolio);

		m_OraLoader.GetSql() = "SELECT A.ASSET_CODE, A.PORTFOLIO, A.TRADE_DATE, A.TRADER_INI, "
					"A.TRANS_TYPE, A.REASON, A.NOTE, B.BUCKET3, B.ASS_DESC, "
					"DECODE(A.TRANS_TYPE, 'CALL', RATE_STRIKE, 'PUT', RATE_STRIKE) \"STRIKE\", "
					"DECODE(A.TRANS_TYPE, 'CALL', TO_NUMBER(NULL), 'PUT', TO_NUMBER(NULL), RATE_STRIKE) \"RATE\", "
					"MATURITY_OPT_EXP \"MATURITY\", A.TRANS_NUM, "
					"DECODE(A.DIR, 'P', 1, 'S', -1)*A.NOM_AMOUNT \"NOMINAL\", "
					"DECODE(A.DIR, 'P', 1, 'S', -1)*A.NOM_AMOUNT*NVL(A.PRICE, 1)*"
							"B.ASSET_FACTOR*A.AMORT_FACT \"AMOUNT\", A.PRICE "
					"FROM SEMAM.NW_DAILY_TRADE_BNAM_V A, SEMAM.NW_ASSETS_V B, "
					"SEMAM.NW_TR_TICKETS C "
					"WHERE B.ASS_CODE = A.ASSET_CODE "
					"AND C.TRANS_NUM = A.TRANS_NUM "
					"AND A.TRADE_DATE = " + Date;
		if(m_Trader->GetCurSel() >= 0)
			m_OraLoader.GetSql() += " AND A.TRADER_INI = " + Trader;

		if(m_Portfolio->GetCurSel() >= 0)
			m_OraLoader.GetSql() += " AND A.PORTFOLIO = " + Portfolio;

		m_OraLoader.GetSql() += " ORDER BY 1, 2 ";

		if(!m_OraLoader.Open(m_OraLoader.GetSql()))
			return FALSE;
		m_OraLoader.GetFieldArray().GetAt(12)->SetWithComma(FALSE);
		m_OraLoader.LoadDBSheet(m_SS);
		EndWaitCursor();
	}

	m_SS.SetSheetCurRow(-1);

	Invalidate();
	UpdateWindow();
	return TRUE;
}

BOOL CFundSizing::UpdateRecord()
{
	CString QDate, Date, Reason, Note;
	CQData QData;
	CDBRec Rec;

	if(m_Reason && m_Reason->GetCurSel() >= 0 && m_TransNum.GetData().IsEmpty())
		return FALSE;

	m_OraLoader.Today(Date);
	QDate = QData.GetQueryDate(Date);

	m_Reason->GetSelString(Reason);
	m_Note.GetWindowText(Note);

	Rec.Add(Date);
	Rec.Add(m_TransNum.GetData());
	Rec.Add(Reason);
	Rec.Add(Note);

	m_OraLoader.Open("SELECT COUNT(*) FROM SEMAM.NW_DAILY_FUND_SIZING "
						"WHERE TRANS_NUM = " + m_TransNum.GetData() + 
						" AND INDATE = " + QDate);
	if(m_OraLoader.GetCount("SELECT COUNT(*) FROM SEMAM.NW_DAILY_FUND_SIZING "
						"WHERE TRANS_NUM = " + m_TransNum.GetData() + 
						" AND INDATE = " + QDate) <= 0)
	{
		m_OraLoader.Open("SELECT INDATE, TRANS_NUM, REASON, NOTE FROM SEMAM.NW_DAILY_FUND_SIZING ", 
						ODYNASET_DEFAULT);
		return m_OraLoader.UpdateRecord(Rec, TRUE);
	}
	else
	{
		m_OraLoader.Open("SELECT INDATE, TRANS_NUM, REASON, NOTE FROM SEMAM.NW_DAILY_FUND_SIZING "
						  "WHERE TRANS_NUM = " + m_TransNum.GetData() + 
						  " AND INDATE = " + QDate, ODYNASET_DEFAULT);
		return m_OraLoader.UpdateRecord(Rec);
	}
}

BEGIN_MESSAGE_MAP(CFundSizing, CFormView)
	ON_BN_CLICKED(IDC_FS_LOAD_BUTTON, &CFundSizing::OnBnClickedFsLoadButton)
	ON_BN_CLICKED(IDC_FS_UPDATE_BUTTON, &CFundSizing::OnBnClickedFsUpdateButton)
	ON_BN_CLICKED(IDC_FS_CLEAR_BUTTON, &CFundSizing::OnBnClickedFsClearButton)
END_MESSAGE_MAP()


// CFundSizing diagnostics

#ifdef _DEBUG
void CFundSizing::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFundSizing::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif

CNWIFMS70Doc* CFundSizing::GetDocument() // non-debug version is inline
{
	return STATIC_DOWNCAST(CNWIFMS70Doc, m_pDocument);
}
#endif //_DEBUG


// CFundSizing message handlers
void CFundSizing::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	BeginWaitCursor();
	m_OraLoader.SetDB(&theDB);
	InitControls();

	UpdateData(FALSE);
	EndWaitCursor();
	
}

void CFundSizing::OnBnClickedFsLoadButton()
{
	UpdateData(FALSE);
}

void CFundSizing::OnBnClickedFsUpdateButton()
{
	UpdateData(TRUE);
}
BEGIN_EVENTSINK_MAP(CFundSizing, CFormView)
	ON_EVENT(CFundSizing, IDC_FS_LIST, 11, CFundSizing::OnDblClickFsList, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

void CFundSizing::OnDblClickFsList(long Col, long Row)
{
	CString Data;
	BOOL bOk = TRUE;

	if(Row < 1 || Row > m_SS.GetSheetRows())
		bOk = FALSE;
	
	m_SS.SetSheetCurRow(Row);
	for(int i = 1; i <= m_SS.GetSheetCols(); i++)
	{
		if(bOk)
			Data = m_SS.GetSheetText(i, Row);
		else
			Data.Empty();
		
		switch(i)
		{
			case 1:
				m_Asset.SetWindowText(Data);
				break;
			case 2:
				m_Portfolio->SelectString(0, Data);
				break;
			case 3:
				m_Date->SelectString(0, Data);
				break;
			case 4:
				m_Trader->SelectString(0, Data);
				break;
			case 5: 
				m_TransType->SelectString(0, Data);
				break;
			case 6:
				m_Reason->SelectString(0, Data);
				break;
			case 7:
				m_Note.SetWindowText(Data);
				break;
			case 8:
				m_Bucket.SelectString(0, Data);
				break;
			case 9:
				m_Desc.SetWindowText(Data);
				break;
			case 10:
				m_Strike.SetWindowText(Data);
				break;
			case 11:
				m_Rate.SetWindowText(Data);
				break;
			case 12:
				m_Maturity.SetData(Data);
				break;
			case 13:
				m_TransNum.SetData(Data);
				break;
			case 14:
				m_Nominal.SetWindowText(Data);
				break;
			case 15:
				m_Amount.SetWindowText(Data);
				break;
			case 16:
				m_Price.SetWindowText(Data);
				break;
			default:
				break;
		}
	}
}


void CFundSizing::OnBnClickedFsClearButton()
{
	OnDblClickFsList(-1, -1);
}
