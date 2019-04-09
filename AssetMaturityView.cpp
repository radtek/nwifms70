// AssetMaturityView.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "AssetMaturityView.h"
#include "qdata.h"


// CAssetMaturityView

IMPLEMENT_DYNCREATE(CAssetMaturityView, CFormView)

CAssetMaturityView::CAssetMaturityView()
	: CFormView(CAssetMaturityView::IDD)
{
}

void CAssetMaturityView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ASSETMATURITY_LIST, m_SS);
}

void CAssetMaturityView::LoadData()
{
	CString Sql, Fund, TransType, Dir, CP, OriMaturity;
	CQData QData;
	COraLoader OraLoader;

	BeginWaitCursor();

	OraLoader = GetData().GetOraLoader();
	Fund = m_Portfolio.GetData();
	TransType = m_TransType.GetData();
	Dir = m_Dir.GetData();
	CP = m_CP.GetData();
	OriMaturity = m_OriMaturity.GetData();

	Sql = "SELECT DECODE(SIGN(NVL(B.INV_NUM, -1)), 1, 'Y', NULL) \"SELECT\", A.PORTFOLIO, A.TRANS_TYPE, "
			"A.MATURITY_DATE, A.TICKET_NUM, A.ASSET_CODE, C.ASS_DESC, A.COUNTERPARTY, A.DIR, "
			"DECODE(A.TRANS_TYPE, 'INT. SWAP', NVL(D.RATE, A.TR_RATE), A.TR_RATE), B.MATURITY, A.NOM_AMOUNT, "
			"A.TRANS_NUM, A.INV_NUM "
			"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_MATURITY_TRADE B, SEMAM.NW_ASSETS C, "
			"SEMAM.NW_ASS_PERIODS D, SEMAM.NW_PORTFOLIOS Z "
			"WHERE B.INV_NUM(+) = A.INV_NUM "
			"AND B.PROCESSED(+) IS NULL "
			"AND C.ASS_CODE = A.ASSET_CODE "
			"AND D.ASS_CODE(+) = A.ASSET_CODE "
			"AND D.ASS_FROM(+) <= TRUNC(SYSDATE) "
			"AND D.ASS_TO(+) + DECODE(NVL(D.ACTION(+), 'A'), 'INCLUSIVE', 1, 0) > TRUNC(SYSDATE) "
			"AND Z.PORTFOLIO = A.PORTFOLIO "
			"AND Z.STATUS IS NULL "
			"AND A.TRANS_TYPE IN ('LEVERAGE', 'REPO', 'INT. SWAP') "
			"AND A.ACTUAL_CLOSING IS NULL "
			"AND A.TR_DESC = 'MATURITY' ";
	if(!Fund.IsEmpty())
	{
		Fund = QData.GetQueryText(Fund);
		Sql += "AND A.PORTFOLIO = " + Fund + " ";
	}
	if(!TransType.IsEmpty())
	{
		TransType = QData.GetQueryText(TransType);
		Sql += "AND A.TRANS_TYPE = " + TransType + " ";
	}
	
	if(!Dir.IsEmpty())
	{
		Dir = QData.GetQueryText(Dir);
		Sql += "AND A.DIR = " + Dir + " ";
	}

	if(!CP.IsEmpty())
	{
		CP = QData.GetQueryText(CP);
		Sql += "AND A.COUNTERPARTY = " + CP + " ";
	}

	if(!OriMaturity.IsEmpty())
	{
		OriMaturity = QData.GetQueryDate(OriMaturity);
		Sql += "AND B.MATURITY = " + OriMaturity + " ";
	}

	Sql += "ORDER BY 2, 3, 4, 5, 6 ";
	
	m_SS.ClearSheet();
	
	OraLoader.Open(Sql);
	OraLoader.GetFieldArray().GetAt(OraLoader.GetFieldArray().GetSize() - 1)->SetWithComma(FALSE);
	OraLoader.GetFieldArray().GetAt(OraLoader.GetFieldArray().GetSize() - 2)->SetWithComma(FALSE);
	OraLoader.LoadDBSheet(m_SS);
	EndWaitCursor();
}

void CAssetMaturityView::SetSearchString(CString &SearchString, CString &Data, BOOL bApply)
{
	if(bApply && !Data.IsEmpty())
	{
		if(SearchString.IsEmpty())
			SearchString = Data;
		else
			SearchString = "|" + Data;
	}
}

void CAssetMaturityView::Search()
{
	CString Ticket, TransNum, Asset, AssetDesc, Portfolio, OriMaturity, Source, Dest;
	int i, PortfolioCol = 2, TicketCol = 5, AssetCol = 6, AssetDescCol = 7, TransNumCol = 13;

	Portfolio = m_Portfolio.GetData();
	SetSearchString(Source, Portfolio);
	Ticket = m_Ticket.GetData();
	SetSearchString(Source, Ticket);
	Asset = m_Asset.GetData();
	SetSearchString(Source, Asset);
	AssetDesc = m_AssetDesc.GetData();
	SetSearchString(Source, AssetDesc);
	TransNum = m_TransNum.GetData();
	SetSearchString(Source, TransNum);

	for(i = 1; i <= m_SS.GetSheetRows(); i ++)
	{
		Dest.Empty();
		SetSearchString(Dest, m_SS.GetSheetText(PortfolioCol, i), !Portfolio.IsEmpty());
		SetSearchString(Dest, m_SS.GetSheetText(TicketCol, i), !Ticket.IsEmpty());
		SetSearchString(Dest, m_SS.GetSheetText(AssetCol, i), !Asset.IsEmpty());
		SetSearchString(Dest, m_SS.GetSheetText(AssetDescCol, i), !AssetDesc.IsEmpty());
		SetSearchString(Dest, m_SS.GetSheetText(TransNumCol, i), !TransNum.IsEmpty());

		if(Source == Dest)
		{
			m_SS.SetActiveCell(1, i);
			return;
		}
	}
}

BOOL CAssetMaturityView::UpdateData(BOOL bSaveandValid)
{
	COraLoader OraLoader;

	BeginWaitCursor();

	OraLoader = GetData().GetOraLoader();

	if(bSaveandValid)
	{
		if(m_InvNum.IsEmpty())
			return TRUE;

		OraLoader.ExecuteSql("DELETE FROM SEMAM.NW_MATURITY_TRADE WHERE INV_NUM = " + m_InvNum); // Delete

		if(m_Select.GetCheck()) // Check 
		{
			if(m_Maturity.GetData().IsEmpty()||m_TransNum.GetData().IsEmpty())
				return TRUE;

			CString Rate;

			Rate = m_Rate.GetData();

			if(Rate.IsEmpty())
				OraLoader.GetSql() = "INSERT INTO SEMAM.NW_MATURITY_TRADE (PORTFOLIO, TRANS_NUM, INV_NUM, MATURITY, "
									"TICKET_NUM, TRADE_DATE, VALUE_DATE, DIR, TRANS_TYPE, NOM_AMOUNT, CURRENCY, "
									"FXRATE, COUNTERPARTY, TRADER_INI, ASSET_CODE, DEAL_TYPE, TRANS_BUCKET, "
									"RATE_BASIS, PRICE, ASSIGN_CP, NOTE, NOTE2) "
									"SELECT A.PORTFOLIO, A.TRANS_NUM, A.INV_NUM, '" + m_Maturity.GetData() + "' \"MATURITY\", "
									"A.TICKET_NUM, A.TRADE_DATE, A.VALUE_DATE, A.DIR, A.TRANS_TYPE, A.NOM_AMOUNT, A.CURRENCY, "
									"A.FXRATE, A.COUNTERPARTY, A.TRADER_INI, A.ASSET_CODE, A.DEAL_TYPE, A.TRANS_BUCKET, "
									"DECODE(A.TRANS_TYPE, 'INT. SWAP', B.ASS_RATE_BASIS, A.RATE_BASIS), A.PRICE, A.ASSIGN_CP, "
									"A.HAN_DETAIL1, A.HAN_DETAIL2 "
									"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B "
									"WHERE B.ASS_CODE = A.ASSET_CODE "
									"AND TRANS_NUM = " + m_TransNum.GetData() + " AND INV_NUM = " + m_InvNum;
			else
				OraLoader.GetSql() = "INSERT INTO SEMAM.NW_MATURITY_TRADE (PORTFOLIO, TRANS_NUM, INV_NUM, MATURITY, "
									"TICKET_NUM, TRADE_DATE, VALUE_DATE, DIR, TRANS_TYPE, NOM_AMOUNT, CURRENCY, "
									"FXRATE, COUNTERPARTY, ASSET_CODE, DEAL_TYPE, TRANS_BUCKET, RATE, RATE_BASIS, "
									"PRICE, ASSIGN_CP, NOTE, NOTE2) "
									"SELECT A.PORTFOLIO, A.TRANS_NUM, A.INV_NUM, '" + m_Maturity.GetData() + "' \"MATURITY\", "
									"A.TICKET_NUM, A.TRADE_DATE, A.VALUE_DATE, A.DIR, A.TRANS_TYPE, A.NOM_AMOUNT, A.CURRENCY, "
									"A.FXRATE, A.COUNTERPARTY, A.ASSET_CODE, A.DEAL_TYPE, A.TRANS_BUCKET, " + Rate + " \"RATE\", "
									"DECODE(A.TRANS_TYPE, 'INT. SWAP', B.ASS_RATE_BASIS, A.RATE_BASIS), A.PRICE, "
									"A.ASSIGN_CP, A.HAN_DETAIL1, A.HAN_DETAIL2 "
									"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B "
									"WHERE B.ASS_CODE = A.ASSET_CODE  "
									"AND TRANS_NUM = " + m_TransNum.GetData() + 
									"AND INV_NUM = " + m_InvNum;
			OraLoader.ExecuteSql();
		}
	
		m_SS.SetSheetText(1, m_SS.GetSheetCurRow(), m_Select.GetCheckString());
	}
	else
	{
		CQData QData;

		GetData().LoadSupportData();
		GetData().GetTransDirArr().CopyToMCComboBox(m_Dir);	/* TransDir */
		GetData().GetTransTypeArr().CopyToComboBox(m_TransType);
		GetData().GetPortfolioArr().CopyToComboBox(m_Portfolio);
		GetData().GetContactList().CopyKeyToComboBox(m_CP);

		OraLoader.Open("SELECT DISTINCT A.MATURITY_DATE "
						"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_MATURITY_TRADE B "
						"WHERE B.INV_NUM(+) = A.INV_NUM "
						"AND B.PROCESSED(+) IS NULL "
						"AND A.TRANS_TYPE IN ('LEVERAGE', 'REPO', 'INT. SWAP') "
						"AND A.ACTUAL_CLOSING IS NULL "
						"AND A.TR_DESC = 'MATURITY' "
						"ORDER BY 1 ");
		OraLoader.LoadComboBox(m_OriMaturity);
		LoadData();
	}
	EndWaitCursor();

	return TRUE;
}

BEGIN_MESSAGE_MAP(CAssetMaturityView, CFormView)
	ON_EN_KILLFOCUS(IDC_ASSETMATURITY_TICKET_EDIT, &CAssetMaturityView::OnEnKillfocusAssetmaturityTicketEdit)
	ON_EN_KILLFOCUS(IDC_ASSETMATURITY_TRANSNUM_EDIT, &CAssetMaturityView::OnEnKillfocusAssetmaturityTransnumEdit)
	ON_EN_KILLFOCUS(IDC_ASSETMATURITY_ASSETCODE_EDIT, &CAssetMaturityView::OnEnKillfocusAssetmaturityAssetcodeEdit)
	ON_EN_KILLFOCUS(IDC_ASSETMATURITY_ASSET_EDIT, &CAssetMaturityView::OnEnKillfocusAssetmaturityAssetEdit)
	ON_CBN_SELCHANGE(IDC_ASSETMATURITY_FUND_COMBO, &CAssetMaturityView::OnCbnSelchangeAssetmaturityFundCombo)
	ON_EN_KILLFOCUS(IDC_ASSETMATURITY_ORIMATURITY_EDIT, &CAssetMaturityView::OnEnKillfocusAssetmaturityOrimaturityEdit)
	ON_BN_CLICKED(IDC_ASSETMATURITY_CLEARSCRN_BUTTON, &CAssetMaturityView::OnBnClickedAssetmaturityClearscrnButton)
	ON_BN_CLICKED(IDC_ASSETMATURITY_SELECT_CHECK, &CAssetMaturityView::OnBnClickedAssetmaturitySelectCheck)
	ON_CBN_SELCHANGE(IDC_ASSETMATURITY_TRANSTYPE_COMBO, &CAssetMaturityView::OnCbnSelchangeAssetmaturityTranstypeCombo)
	ON_CBN_SELCHANGE(IDC_ASSETMATURITY_DIR_COMBO, &CAssetMaturityView::OnCbnSelchangeAssetmaturityDirCombo)
	ON_CBN_SELCHANGE(IDC_ASSETMATURITY_CP_COMBO, &CAssetMaturityView::OnCbnSelchangeAssetmaturityCpCombo)
	ON_CBN_SELCHANGE(IDC_ASSETMATURITY_ORIMATURITY_COMBO, &CAssetMaturityView::OnCbnSelchangeAssetmaturityOrimaturityCombo)
END_MESSAGE_MAP()


// CAssetMaturityView diagnostics

#ifdef _DEBUG
void CAssetMaturityView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAssetMaturityView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif

CNWIFMS70Doc* CAssetMaturityView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNWIFMS70Doc)));
	return (CNWIFMS70Doc*)m_pDocument;
}
#endif //_DEBUG

void CAssetMaturityView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	BeginWaitCursor();
	CString Date;

	m_SS.SetVisibleCols(10);
	m_SS.SetVisibleRows(20);
	m_SS.LockSheet();

	m_Select.Setup(this, IDC_ASSETMATURITY_SELECT_CHECK, Y);

	m_Dir.Setup(this, IDC_ASSETMATURITY_DIR_COMBO);

	m_TransType.Setup(this, IDC_ASSETMATURITY_TRANSTYPE_COMBO);
	m_Portfolio.Setup(this, IDC_ASSETMATURITY_FUND_COMBO);
	m_CP.Setup(this, IDC_ASSETMATURITY_CP_COMBO);
	m_OriMaturity.Setup(this, IDC_ASSETMATURITY_ORIMATURITY_COMBO);

	m_Amount.Setup(this, IDC_ASSETMATURITY_NOMINAL_EDIT);
	m_Rate.Setup(this, IDC_ASSETMATURITY_RATE_EDIT);
	m_Maturity.Setup(this, IDC_ASSETMATURITY_MATURITY_EDIT);
	
	m_Ticket.Setup(this, IDC_ASSETMATURITY_TICKET_EDIT);
	m_TransNum.Setup(this, IDC_ASSETMATURITY_TRANSNUM_EDIT);

	m_Asset.Setup(this, IDC_ASSETMATURITY_ASSETCODE_EDIT);
	m_AssetDesc.Setup(this, IDC_ASSETMATURITY_ASSET_EDIT);

	GetData().GetOraLoader().Today(Date);
	m_Maturity.SetData(Date);
	UpdateData(FALSE);
	EndWaitCursor();
}

BEGIN_EVENTSINK_MAP(CAssetMaturityView, CFormView)
	ON_EVENT(CAssetMaturityView, IDC_ASSETMATURITY_LIST, 11, CAssetMaturityView::OnDblClickAssetmaturityList, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

void CAssetMaturityView::OnDblClickAssetmaturityList(long Col, long Row)
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		m_SS.SetSheetCurRow(Row);
		m_Select.SetCheck(m_SS.GetSheetText(1, Row));
		m_Portfolio.SetData(m_SS.GetSheetText(2, Row));
		m_TransType.SetData(m_SS.GetSheetText(3, Row));
		m_OriMaturity.SetData(m_SS.GetSheetText(4, Row));
		m_Ticket.SetData(m_SS.GetSheetText(5, Row));
		m_Asset.SetData(m_SS.GetSheetText(6, Row));
		m_AssetDesc.SetData(m_SS.GetSheetText(7, Row));
		m_CP.SetData(m_SS.GetSheetText(8, Row));
		m_Dir.SetData(m_SS.GetSheetText(9, Row));
		m_Rate.SetData(m_SS.GetSheetText(10, Row));
		if(!m_SS.GetSheetText(11, Row).IsEmpty())
			m_Maturity.SetData(m_SS.GetSheetText(11, Row));
		m_Amount.SetData(m_SS.GetSheetText(12, Row));
		m_TransNum.SetData(m_SS.GetSheetText(13, Row));
		m_InvNum = m_SS.GetSheetText(14, Row);
	}
	else
	{
		m_Select.SetCheck(0);
		m_Portfolio.SetCurSel(-1);
		m_TransType.SetCurSel(-1);
		m_OriMaturity.SetCurSel(-1);
		m_Ticket.SetData(EMPTYSTRING);
		m_Asset.SetData(EMPTYSTRING);
		m_AssetDesc.SetData(EMPTYSTRING);
		m_CP.SetCurSel(-1);
		m_Dir.SetCurSel(-1);
		m_Rate.SetData(EMPTYSTRING);
		m_Amount.SetData(EMPTYSTRING);
		m_TransNum.SetData(EMPTYSTRING);
		m_InvNum.Empty();
	}
}

// CAssetMaturityView message handlers

void CAssetMaturityView::OnEnKillfocusAssetmaturityTicketEdit()
{
	Search();
}

void CAssetMaturityView::OnEnKillfocusAssetmaturityTransnumEdit()
{
	Search();
}

void CAssetMaturityView::OnEnKillfocusAssetmaturityAssetcodeEdit()
{
	Search();
}

void CAssetMaturityView::OnEnKillfocusAssetmaturityAssetEdit()
{
	Search();
}

void CAssetMaturityView::OnCbnSelchangeAssetmaturityFundCombo()
{
	LoadData();
	Search();
}

void CAssetMaturityView::OnEnKillfocusAssetmaturityOrimaturityEdit()
{
	Search();
}

void CAssetMaturityView::OnBnClickedAssetmaturityClearscrnButton()
{
	OnDblClickAssetmaturityList(1, -1);
}

void CAssetMaturityView::OnBnClickedAssetmaturitySelectCheck()
{
	UpdateData();
}

void CAssetMaturityView::OnCbnSelchangeAssetmaturityTranstypeCombo()
{
	LoadData();
}

void CAssetMaturityView::OnCbnSelchangeAssetmaturityDirCombo()
{
	LoadData();
}

void CAssetMaturityView::OnCbnSelchangeAssetmaturityCpCombo()
{
	LoadData();
}

void CAssetMaturityView::OnCbnSelchangeAssetmaturityOrimaturityCombo()
{
	LoadData();
}
