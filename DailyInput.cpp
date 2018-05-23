// DailyInput.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include <math.h>
#include <ssocx.h>
#include "DailyInput.h"
#include "qdata.h"
#include "sqlstmt.h"
#include "floatrepo.h"
#include "navchange.h"
#include "blmid.h"
#include "assetratedata.h"
#include "odbcdb.h"
#include "BlmFXData.h"
#include "BlmPriceData.h"
#include "BlmDividendData.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDailyInput

IMPLEMENT_DYNCREATE(CDailyInput, CFormView)

CDailyInput::CDailyInput() : CFormView(CDailyInput::IDD)
{
	//{{AFX_DATA_INIT(CDailyInput)
	//}}AFX_DATA_INIT
}

CDailyInput::~CDailyInput()
{
}

void CDailyInput::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDailyInput)
	DDX_Control(pDX, IDC_DAILY_INPUT_LIST, m_SS);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDailyInput, CFormView)
	//{{AFX_MSG_MAP(CDailyInput)
	ON_COMMAND(ID_DAILY_INPUT_FXRATE, OnDailyInputFxrate)
	ON_COMMAND(ID_DAILY_INPUT_OPTION, OnDailyInputOption)
	ON_COMMAND(ID_DAILY_INPUT_OPTION_DELTA, OnDailyInputOptionDelta)
	ON_COMMAND(ID_DAILY_INPUT_VAR_FACTOR, OnDailyInputVarFactor)
	ON_COMMAND(ID_DAILY_INPUT_PRICE, OnDailyInputPrice)
	ON_COMMAND(ID_DAILY_INPUT_COST, OnDailyInputCost)
	ON_COMMAND(ID_DAILY_INPUT_DELETE, OnDailyInputDelete)
	ON_COMMAND(ID_DAILY_INPUT_COPY, OnDailyInputCopy)
	ON_COMMAND(ID_DAILY_INPUT_PASTE, OnDailyInputPaste)
	ON_UPDATE_COMMAND_UI(ID_DAILY_INPUT_DELETE, OnUpdateDailyInputDelete)
	ON_COMMAND(ID_DAILY_INPUT_FIND, OnDailyInputFind)
	ON_UPDATE_COMMAND_UI(ID_DAILY_INPUT_FIND, OnUpdateDailyInputFind)
	ON_COMMAND(ID_DAILY_INPUT_SAVE, OnDailyInputSave)
	ON_UPDATE_COMMAND_UI(ID_DAILY_INPUT_SAVE, OnUpdateDailyInputSave)
	ON_COMMAND(ID_DAILY_UPDATE_FXRATE, OnDailyUpdateFxrate)
	ON_COMMAND(ID_DAILY_UPDATE_SWAP, OnDailyUpdateSwap)
	ON_COMMAND(ID_DAILY_UPDATE_OPTION, OnDailyUpdateOption)
	ON_COMMAND(ID_DAILY_UPDATE_OPTION_DELTA, OnDailyUpdateOptionDelta)
	ON_COMMAND(ID_DAILY_UPDATE_VAR_FACTOR, OnDailyUpdateVarFactor)
	ON_COMMAND(ID_DAILY_UPDATE_PRICE, OnDailyUpdatePrice)
	ON_COMMAND(ID_DAILY_INPUT_REPO, OnDailyInputRepo)
	ON_COMMAND(ID_DAILY_INPUT_NAV, OnDailyInputNav)
	ON_COMMAND(ID_DAILY_UPDATE_NAV, OnDailyUpdateNav)
	ON_COMMAND(ID_DAILY_INPUT_SWAP, OnDailyInputSwap)
	ON_COMMAND(ID_DAILY_INPUT_DURATION, OnDailyInputDuration)
	ON_COMMAND(ID_DAILY_UPDATE_DURATION, OnDailyUpdateDuration)
	ON_COMMAND(ID_DAILY_INPUT_REPO_COUPON, OnDailyInputRepoCoupon)
	ON_COMMAND(ID_DAILY_INPUT_BLOOMBERG_ID, OnDailyInputBloombergId)
	ON_COMMAND(ID_DAILY_INPUT_RATE, OnDailyInputRate)
	ON_COMMAND(ID_DAILY_INPUT_VOL, OnDailyInputVol)
	ON_COMMAND(ID_DAILY_UPDATE_RATE, OnDailyUpdateRate)
	ON_COMMAND(ID_DAILY_UPDATE_VOL, OnDailyUpdateVol)
	ON_COMMAND(ID_DAILY_UPDATE_ISSUER_AMOUNT, OnDailyUpdteIssuerAmount)
	ON_UPDATE_COMMAND_UI(ID_DAILY_UPDATE_DURATION, OnUpdateDailyUpdateDuration)
	ON_UPDATE_COMMAND_UI(ID_DAILY_UPDATE_FXRATE, OnUpdateDailyUpdateFxrate)
	ON_UPDATE_COMMAND_UI(ID_DAILY_UPDATE_NAV, OnUpdateDailyUpdateNav)
	ON_UPDATE_COMMAND_UI(ID_DAILY_UPDATE_OPTION, OnUpdateDailyUpdateOption)
	ON_UPDATE_COMMAND_UI(ID_DAILY_UPDATE_OPTION_DELTA, OnUpdateDailyUpdateOptionDelta)
	ON_UPDATE_COMMAND_UI(ID_DAILY_UPDATE_PRICE, OnUpdateDailyUpdatePrice)
	ON_UPDATE_COMMAND_UI(ID_DAILY_UPDATE_RATE, OnUpdateDailyUpdateRate)
	ON_UPDATE_COMMAND_UI(ID_DAILY_UPDATE_SWAP, OnUpdateDailyUpdateSwap)
	ON_UPDATE_COMMAND_UI(ID_DAILY_UPDATE_VAR_FACTOR, OnUpdateDailyUpdateVarFactor)
	ON_UPDATE_COMMAND_UI(ID_DAILY_UPDATE_VOL, OnUpdateDailyUpdateVol)
	ON_UPDATE_COMMAND_UI(ID_DAILY_INPUT_REPO, OnUpdateDailyInputRepo)
	ON_UPDATE_COMMAND_UI(ID_DAILY_INPUT_REPO_COUPON, OnUpdateDailyInputRepoCoupon)
	ON_UPDATE_COMMAND_UI(ID_DAILY_INPUT_BLOOMBERG_ID, OnUpdateDailyInputBloombergId)
	ON_UPDATE_COMMAND_UI(ID_DAILY_INPUT_NAV, OnUpdateDailyInputNav)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_DAILY_UPLOAD_OPTIONVALUE, OnDailyUploadOptionvalue)
	ON_COMMAND(ID_DAILY_INPUT_OPTIONVALUE, OnDailyInputOptionvalue)
	ON_COMMAND(ID_DAILY_UPDATE_ACCRUALFACTOR, OnDailyUpdateAccrualfactor)
	ON_COMMAND(ID_DAILY_INPUT_ACCRUALFACTOR, OnDailyInputAccrualfactor)
	ON_UPDATE_COMMAND_UI(ID_DAILY_UPDATE_ACCRUALFACTOR, OnUpdateDailyUpdateAccrualfactor)
	ON_UPDATE_COMMAND_UI(ID_DAILY_INPUT_ACCRUALFACTOR, OnUpdateDailyInputAccrualfactor)
	ON_COMMAND(ID_DAILY_UPDATE_DV01, OnDailyUpdateDv01)
	ON_COMMAND(ID_DAILY_INPUT_DV01, OnDailyInputDv01)
	ON_UPDATE_COMMAND_UI(ID_DAILY_INPUT_DV01, OnUpdateDailyInputDv01)
	ON_UPDATE_COMMAND_UI(ID_DAILY_UPDATE_DV01, OnUpdateDailyUpdateDv01)
	ON_UPDATE_COMMAND_UI(ID_DAILY_UPDATE_ISSUER_AMOUNT, OnUpdateDailyUpdateIssuerAmount)
	ON_COMMAND(ID_DAILY_UPDATE_ASSETRATE, OnDailyUpdateAssetrate)
	ON_COMMAND(ID_DAILY_QUICKFXRATE, OnDailyQuickfxrate)
	ON_COMMAND(ID_DAILY_QUICKPRICE, OnDailyQuickprice)
	ON_COMMAND(ID_DAILY_QUICKDIVIDEND, &CDailyInput::OnDailyQuickdividend)
	ON_UPDATE_COMMAND_UI(ID_DAILY_QUICKFXRATE, &CDailyInput::OnUpdateDailyQuickfxrate)
	ON_UPDATE_COMMAND_UI(ID_DAILY_QUICKPRICE, &CDailyInput::OnUpdateDailyQuickprice)
	ON_UPDATE_COMMAND_UI(ID_DAILY_QUICKDIVIDEND, &CDailyInput::OnUpdateDailyQuickdividend)
	ON_COMMAND(ID_DAILY_QUICKOPTPRICE, &CDailyInput::OnDailyQuickoptprice)
	ON_COMMAND(ID_DAILY_QUICKSWAPPRICE, &CDailyInput::OnDailyQuickswapprice)
	ON_UPDATE_COMMAND_UI(ID_DAILY_QUICKSWAPPRICE, &CDailyInput::OnUpdateDailyQuickswapprice)
	ON_UPDATE_COMMAND_UI(ID_DAILY_QUICKOPTPRICE, &CDailyInput::OnUpdateDailyQuickoptprice)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDailyInput diagnostics

#ifdef _DEBUG
void CDailyInput::AssertValid() const
{
	CFormView::AssertValid();
}

void CDailyInput::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CNWIFMS70Doc* CDailyInput::GetDocument() // non-debug version is inline
{
	return STATIC_DOWNCAST(CNWIFMS70Doc, m_pDocument);
}
#endif //_DEBUG

void CDailyInput::DoInput(BOOL bEnable, double WarnLimit, const int MenuID, int DataRange)
{
	CString Date, Sql;
	
	DoSave();
	Date = m_Date.GetData();
	if(Date.IsEmpty())
		return;
	
	m_MenuID = MenuID;
	m_Date.EnableWindow(bEnable);
	m_DataArray.Setup(MenuID, Date, WarnLimit, DataRange);
	m_DataArray.SetOptionValue(m_MenuID == INPUT_OPTIONVALUE ? TRUE : FALSE);
	switch(m_MenuID)
	{
		case INPUT_FXRATE:
			Sql = "SELECT CURRENCY, EXRATE \"PREV FXRATE\", EXRATE \"FXRATE\", "
					"0 \"DIFFERENCE\", 1 \"% CHANGE\" FROM SEMAM.NW_EXRATES";
			break;
		case INPUT_COST:
			Sql = "SELECT ASSET_CODE \"ASSET\", NULL \"SOURCE\", COST \"PREV COST\", COST, "
					"0 \"DIFFERENCE\", DECODE(COST, NULL, 1, -1) \"% CHANGE\" "
					"FROM SEMAM.NW_ASSET_COST ";
			break;
		case INPUT_INTRATE:
//			SqlID = IDS_SQL_86;
			break;
		case INPUT_SWAP:
			Sql = "SELECT 'SEMAM' \"FILESET\", TICKET_NUM||'\\'||TO_NUMBER(TRANS_NUM) \"ID\", "
					"VALUE \"PREV VALUE\", VALUE, 0 \"DIFFERENCE\", "
					"DECODE(TRANS_NUM, NULL, 1, -1) \"% CHANGE\" "
					"FROM SEMAM.NW_DEAL_NAV ";
			break;
		case INPUT_OPTION:
			Sql = "SELECT PORTFOLIO, TRANS_NUM, PRICE \"PREV MARK\", PRICE \"MARKET_MARK\", "
					"0 \"DIFFERENCE\", DECODE(TRANS_NUM, NULL, 1, -1) \"% CHANGE\" "
					"FROM SEMAM.NW_TR_TICKETS ";
			break;
		case INPUT_OPTION_DELTA:
			Sql = "SELECT TRANS_NUM, DELTA \"PREV DELTA\", DELTA, 0 \"DIFFERENCE\", 1 \"% CHANGE\" "
					"FROM SEMAM.NW_OPT_DELTA ";
			break;
		case INPUT_OPTIONVALUE:
			Sql = "SELECT PORTFOLIO, TRANS_NUM, NOM_AMOUNT \"PREV VALUE\", "
					"NOM_AMOUNT \"MARKET_VALUE\", 0 \"DIFFERENCE\", "
					"DECODE(TRANS_NUM, NULL, 1, -1) \"% CHANGE\" FROM SEMAM.NW_TR_TICKETS ";
			break;
		case INPUT_VAR_FACTOR:
			Sql = "SELECT ASSET, VAR_FACT \"PREV_VAR_FACT\", VAR_FACT, 0 \"DIFFERENCE\", "
					"1 \"% CHANGE\" FROM SEMAM.NW_ASSET_VAR ";
			break;
		case INPUT_PRICE:
			Sql = "SELECT ASSET, 'ASSET_ID' \"ASSET_ID\", MARKET \"PREV_PRICE\", MARKET \"PRICE\", 0 \"DIFFERENCE\", "
					"1 \"% CHANGE\" FROM SEMAM.NW_MARKET_PRICES ";
			break;
		case INPUT_DURATION:
			Sql = "SELECT ASSET_CODE, DURATION \"PREV_DURATION\", DURATION, 0 \"DIFFERENCE\", "
					"1 \"% CHANGE\" FROM SEMAM.NW_DURATION ";
			break;
		case INPUT_RATE:
			Sql = "SELECT A.CURRENCY, A.CASHDAYS, A.RATE \"PREV RATE\", A.RATE \"RATE\", "
					"0 \"DIFFERENCE\", DECODE(A.RATE, NULL, 1, -1) \"% CHANGE\" "
					"FROM SEMAM.NW_RISKFREE_RATES A "; 
			break;
//		case INPUT_VOL:
//			Sql = "SELECT ASSET, TERM, BID_CALL, OFFER_CALL, BID_PUT, OFFER_PUT "
//					"FROM SEMAM.NW_VOL_GRID ";
//			break;
		case INPUT_ACCR_FACT:
			Sql = "SELECT ASSET_CODE, ACCR_FACTOR \"PREV_FACTOR\", ACCR_FACTOR, 0 \"DIFFERENCE\", "
					"1 \"% CHANGE\" FROM SEMAM.NW_ACCR_FACTOR ";
			break;
		case INPUT_DV01:
			Sql = "SELECT ASSET_CODE, TICKET_NUM, DV01 \"PREV_DV01\", DV01, 0 \"DIFFERENCE\", "
					"1 \"% CHANGE\" FROM SEMAM.NW_DV01 ";
			break;
		case INPUT_ISSUER_AMOUNT:
			Sql = "SELECT ISSUER, AMOUNT \"PREV AMOUNT\", AMOUNT \"AMOUNT\", "
					"0 \"DIFFERENCE\", 1 \"% CHANGE\" FROM SEMAM.NW_ISSUER_AMOUNT";
			break;
		default:
			break;
	}

	if(Sql.GetLength() > 0)
		m_DataArray.SetupVisual(Sql, m_SS);
	SetTitle();
	UpdateData(FALSE);
}

void CDailyInput::DoSave()
{
	if(m_DataArray.GetModify() && 
		MessageBox("Input Data have been modify. Do you want to save ? ", NULL, MB_YESNO) == IDYES)
	{
		UpdateData();
		if(m_bReload)
			UpdateData(FALSE);
	}
}

void CDailyInput::DoUpdate(LPCTSTR Title, int MenuID, int DataRange)
{
	CString Date, Path, Today, Msg;
	LPCTSTR Template = "Template";
	LPCTSTR CSV = "CSV";
	CAssetRateData AssetRateData;
	CQData QData;
	
	m_MenuID = MenuID;
	Path = "Template Files (*.CSV) || ";
	// Get the user name and host from the private profile	
	if(!QData.GetFileName(AfxGetApp(), Path, Template, Title, CSV))
    	return;

	if(MenuID == INPUT_ASSETRATE) // Upload Asset Rate
	{
		AssetRateData.Setup(Path, DataRange);
		AssetRateData.ProcessData();
		return;
	}

	m_DataArray.GetOraLoader().Today(Today);
	Date = m_Date.GetData();
	if(Date != Today)
	{
		Msg = "You are about to update for " + Date + ".  Are sure you want to continue? ";
		if(MessageBox(Msg, Today, MB_YESNO) == IDNO)
			return;
	}

	if(GetDocument()->GetData().InputDate(Date) != IDOK)
		return;

	if(!m_DataArray.Setup(MenuID, Date, Path, DataRange))
		return;
	
	BeginWaitCursor();
	SetTitle();
	
	m_DataArray.SetOptionValue(m_MenuID == INPUT_OPTIONVALUE ? TRUE : FALSE);
	LoadData(m_MenuID == INPUT_PRICE ? TRUE : FALSE);

	if(m_MenuID == INPUT_PRICE)
		m_DataArray.LoadFileData(TRUE);
	else
		if(m_MenuID == INPUT_SWAP)
			m_DataArray.LoadFileData(FALSE, TRUE);
		else
		{
			if(m_MenuID == INPUT_DV01)
				m_DataArray.SetTwo(TRUE);

			m_DataArray.LoadFileData();
		}
	
	if(m_DataArray.GetModify())
	{
		SaveData();
		m_DataArray.DisplayErrorFile();
	}

	m_DataArray.GetInFile().Close();

    EndWaitCursor();
}

void CDailyInput::LoadData(BOOL bUpload)
{
	CSqlStmt PriceSql;
	CString Sql;
	BOOL bPrice = FALSE;
	LPCTSTR PrevDate, QDate;

	Sql.Empty();
	switch(m_MenuID)
	{
		case INPUT_FXRATE:
			Sql = "SELECT MAX(INDATE) FROM SEMAM.NW_EXRATES WHERE INDATE < %s ";
			break;
		case INPUT_COST:			
			break;
		case INPUT_INTRATE:
			break;
		case INPUT_SWAP:
			Sql = "SELECT MAX(INDATE) FROM SEMAM.NW_DEAL_NAV WHERE INDATE < %s ";
			break;
		case INPUT_OPTIONVALUE:
		case INPUT_OPTION:
			Sql = "SELECT MAX(MM_DATE) FROM SEMAM.NW_OPT_PRICES WHERE MM_DATE < %s ";
			break;
		case INPUT_OPTION_DELTA:
			Sql = "SELECT MAX(INDATE) FROM SEMAM.NW_OPT_DELTA WHERE INDATE < %s ";
			break;
		case INPUT_VAR_FACTOR:
			Sql = "SELECT MAX(INDATE) FROM SEMAM.NW_ASSET_VAR WHERE INDATE < %s ";
			break;
		case INPUT_PRICE:
			Sql = "SELECT MAX(PR_DATE) FROM SEMAM.NW_MARKET_PRICES WHERE PR_DATE < %s ";
			break;
		case INPUT_DURATION:
			Sql = "SELECT MAX(INDATE) FROM SEMAM.NW_DURATION WHERE INDATE < %s ";
			break;
		case INPUT_RATE:
			Sql = "SELECT MAX(INDATE) FROM SEMAM.NW_RISKFREE_RATES WHERE INDATE < %s ";
			break;
//		case INPUT_VOL:
//			Sql = "SELECT MAX(PR_DATE) FROM SEMAM.NW_VOL_GRID WHERE PR_DATE < %s ";
//			break;
		case INPUT_ACCR_FACT:
			Sql = "SELECT MAX(INDATE) FROM SEMAM.NW_ACCR_FACTOR WHERE INDATE < %s ";
			break;
		case INPUT_DV01:
			Sql = "SELECT MAX(INDATE) FROM SEMAM.NW_DV01 WHERE INDATE < %s ";
			break;
		case INPUT_ISSUER_AMOUNT:
			Sql = "SELECT MAX(INDATE) FROM SEMAM.NW_ISSUER_AMOUNT WHERE INDATE < %s ";
		default:
			break;
	}

	PrevDate = m_DataArray.GetPrevDate(TRUE, Sql);
	QDate = m_DataArray.GetQDate();

	switch(m_MenuID)
	{
		case INPUT_FXRATE:
			Sql.Format("SELECT A.CURRENCY, B.EXRATE \"PREV FXRATE\", C.EXRATE \"FXRATE\", "
				"DECODE(C.CURRENCY, NULL, 1, -1) \"STATUS\" "
				"FROM SEMAM.NW_COUNTRIES A, SEMAM.NW_EXRATES B, SEMAM.NW_EXRATES C "
				"WHERE B.CURRENCY(+) = A.CURRENCY "
				"AND B.INDATE(+) = %s "
				"AND C.CURRENCY(+) = A.CURRENCY "
				"AND C.INDATE(+) = %s "
				"ORDER BY 1, 2, 3, 4 ",	PrevDate, QDate);
			break;
		case INPUT_COST:
			m_DataArray.UpdateAssetPos("DELETE FROM SEMAM.NW_ASSET_POS_2 ", 
						"INSERT INTO SEMAM.NW_ASSET_POS_2 (INDATE, PORTFOLIO, ASSET_CODE, "
						"DEAL_TYPE, NOM_AMOUNT) "
						"SELECT TO_DATE(%s), A.PORTFOLIO, ASSET_CODE, DEAL_TYPE, "
						"SUM(DECODE(DIR, 'P', 1, 'S', -1)*NOM_AMOUNT/"
						"DECODE(ASS_TYPE, 'EQUITY TYPE CAL', 1, NVL(CONVERSION, 1))) "
						"FROM SEMAM.NW_CF_ALL_TICKETS A, SEMAM.NW_ASSETS B, "
						"SEMAM.NW_CURRENCY X, SEMAM.NW_PORTFOLIOS Z "
						"WHERE B.ASS_CODE = A.ASSET_CODE "
						"AND X.CURRENCY = A.CURRENCY "
						"AND Z.PORTFOLIO = A.PORTFOLIO "
						"AND Z.STATUS IS NULL "
						"AND A.TRADE_DATE <= %s "
						"GROUP BY A.PORTFOLIO, ASSET_CODE, DEAL_TYPE "
						"HAVING (TRUNC(SUM(DECODE(DIR, 'P', 1, 'S', -1)*NOM_AMOUNT/"
						"DECODE(ASS_TYPE, 'EQUITY TYPE CAL', 1, NVL(X.CONVERSION, 1))), 0) != 0) ");

			Sql.Format("SELECT A.ASSET_CODE||'\t'||B.SOURCE \"ASSET\", C.COST \"PREV_COST\", "
						"C.COST, DECODE(C.COST, NULL, 1, -1) \"STATUS\", "
						"DECODE(A.PORTFOLIO, NULL,1,1), "
						"DECODE(SUM(DECODE(TRANS_DIRECTION,'P',1,'S',-1)*A.NOM_AMOUNT), NULL,1,1) "
						"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_ASSETS B, SEMAM.NW_ASSET_COST C, "
						"SEMAM.NW_ASSET_POS_2 D, SEMAM.NW_CURRENCY X, SEMAM.NW_PORTFOLIOS Z "
						"WHERE B.ASS_CODE = A.ASSET_CODE "
						"AND C.ASSET_CODE(+) = B.ASS_CODE "
						"AND D.ASSET_CODE = A.ASSET_CODE "
						"AND D.INDATE >= A.TRADE_DATE "
						"AND X.CURRENCY = A.CURRENCY "
						"AND Z.PORTFOLIO = A.PORTFOLIO "
						"AND Z.STATUS IS NULL "
						"AND A.TRANS_TYPE IN ('CDS', 'SECURITIES') "
						"GROUP BY A.ASSET_CODE, B.SOURCE, C.COST, A.PORTFOLIO "
						"HAVING (SUM(DECODE(TRANS_DIRECTION, 'P', 1, 'S', -1)*A.NOM_AMOUNT/"
							"DECODE(B.ASS_TYPE, 'BOND TYP(%%) CAL', NVL(X.CONVERSION, 1), 1)) != 0) "
						"UNION "
						"SELECT A.ASSET_CODE||'\t'||C.SOURCE, D.COST, D.COST, DECODE(D.COST, 1, -1), "
						"DECODE(A.TRANS_NUM, NULL,1,1), SIGN(A.NOM_AMOUNT - NVL(SUM(B.NOM_AMOUNT), 0)) "
						"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_TR_TICKETS B, SEMAM.NW_ASSETS C, "
						"SEMAM.NW_ASSET_COST D, SEMAM.NW_ASSET_POS_2 E, SEMAM.NW_PORTFOLIOS Z "
						"WHERE B.OPTION_BACKING(+) = A.TRANS_NUM "
						"AND B.TRADE_DATE(+) <= %s "
						"AND C.ASS_CODE = A.ASSET_CODE "
						"AND D.ASSET_CODE(+) = C.ASS_CODE "
						"AND E.ASSET_CODE = A.ASSET_CODE "
						"AND (E.INDATE >= A.TRADE_DATE OR E.INDATE > A.VALUE_DATE) "
						"AND (A.OPT_EXPIRATION IS NULL OR E.INDATE > A.OPT_EXPIRATION) "
						"AND (A.EXERCISE_DATE IS NULL OR E.INDATE > A.EXERCISE_DATE) "
						"AND Z.PORTFOLIO = A.PORTFOLIO "
						"AND Z.STATUS IS NULL "
						"AND A.OPTION_BACKING IS NULL "
						"AND A.TRANS_TYPE IN ('CALL', 'PUT') "
						"GROUP BY A.ASSET_CODE, C.SOURCE, D.COST, A.TRANS_NUM, A.NOM_AMOUNT "
						"HAVING (A.NOM_AMOUNT - NVL(SUM(B.NOM_AMOUNT), 0) > 0) "
						"UNION "
						"SELECT DISTINCT A.ASSET_CODE||'\t'||B.SOURCE \"ASSET\", C.COST \"PREV_COST\", "
						"C.COST, DECODE(C.COST, NULL, 1, -1) \"STATUS\", "
						"DECODE(A.PORTFOLIO, NULL,1,1), 1 "
						"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B, SEMAM.NW_ASSET_COST C, "
						"SEMAM.NW_CURRENCY X, SEMAM.NW_PORTFOLIOS Z "
						"WHERE B.ASS_CODE = A.ASSET_CODE "
						"AND C.ASSET_CODE(+) = A.ASSET_CODE "
						"AND X.CURRENCY = A.CURRENCY "
						"AND Z.PORTFOLIO = A.PORTFOLIO "
						"AND Z.STATUS IS NULL "
						"AND A.TRANS_TYPE IN ('REPO', 'LEVERAGE') "
						"AND A.TRADE_DATE <= %s "
						"AND NVL(ACTUAL_VDATE, NVL(MATURITY_DATE, TO_DATE(%s) + 1)) > %s ", 
						QDate, QDate, QDate, QDate);
			break;
		case INPUT_INTRATE:
//			GetPrevDate(IDS_SQL_87);
//			Sql.Format(IDS_SQL_88, GetQDate(), GetQDate(), GetPrevDate(), GetQDate(), 
//						GetQDate(),	GetQDate(), GetPrevDate(), GetQDate());
			break;
		case INPUT_SWAP:
			Sql.Format("SELECT 'SEMAM' ||'\t'|| A.TICKET_NUM||'\\'||TO_CHAR(A.TRANS_NUM), B.VALUE, "
						"C.VALUE, DECODE(C.TRANS_NUM, NULL, 1, -1) \"STATUS\" "
						"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_DEAL_NAV B, SEMAM.NW_DEAL_NAV C, "
						"SEMAM.NW_ASSETS D, SEMAM.NW_PORTFOLIOS Z "
						"WHERE B.TRANS_NUM(+) = A.TRANS_NUM "
						"AND B.INDATE(+) = %s "
						"AND C.TRANS_NUM(+) = A.TRANS_NUM "
						"AND C.INDATE(+) = %s "
						"AND D.ASS_CODE = A.ASSET_CODE "
						"AND D.ASS_INT_TYPE = 'FIXED' "
						"AND Z.PORTFOLIO = A.PORTFOLIO "
						"AND Z.STATUS IS NULL "
						"AND A.TRANS_TYPE = 'INT. SWAP' "
						"AND NVL(A.ACTUAL_VDATE, NVL(A.MATURITY_DATE, NVL(D.ASS_MATURITY_DATE, TO_DATE(%s)+1))) > %s "
						"AND A.TRANS_NUM >= 80000 "
						"ORDER BY 1, 2, 3, 4", PrevDate, QDate, QDate, QDate);
			break;
		case INPUT_OPTION:
			UpdateOptionPosition(QDate, TRUE);
			Sql.Format("SELECT A.PORTFOLIO||'\t'||TO_CHAR(A.TRANS_NUM) \"ASSET\", B.MARKET_MARK, "
						"C.MARKET_MARK, DECODE(C.TRANS_NUM, NULL, 1, -1) \"STATUS\", "
						"SIGN(A.NOM_AMOUNT) \"ID\" "
						"FROM SEMAM.NW_TEMP_OPTIONS A, SEMAM.NW_OPT_PRICES B, SEMAM.NW_OPT_PRICES C, " 
						"SEMAM.NW_PORTFOLIOS Z "
						"WHERE B.TRANS_NUM(+) = A.TRANS_NUM "
						"AND B.MM_DATE(+) = %s "
						"AND C.TRANS_NUM(+) = A.TRANS_NUM "
						"AND C.MM_DATE(+) = %s "
						"AND Z.PORTFOLIO = A.PORTFOLIO "
						"AND Z.STATUS IS NULL "
						"AND A.NOM_AMOUNT > 0 "
						"ORDER BY A.PORTFOLIO, A.TRANS_NUM ", PrevDate, QDate);
			break;
		case INPUT_OPTION_DELTA:
			UpdateOptionPosition(QDate);
			Sql.Format("SELECT A.TRANS_NUM, B.DELTA, C.DELTA, DECODE(C.TRANS_NUM, NULL, 1, -1) \"STATUS\", "
					"SIGN(A.NOM_AMOUNT) \"ID\", DECODE(A.PORTFOLIO, NULL, 1, 1), "
					"DECODE(A.TRANS_NUM, NULL, 1, 1) "
					"FROM SEMAM.NW_TEMP_OPTIONS A, SEMAM.NW_OPT_DELTA B, SEMAM.NW_OPT_DELTA C, "
					"SEMAM.NW_PORTFOLIOS Z "
					"WHERE B.TRANS_NUM(+) = A.TRANS_NUM "
					"AND B.INDATE(+) = %s "
					"AND C.TRANS_NUM(+) = A.TRANS_NUM "
					"AND C.INDATE(+) = %s "
					"AND Z.PORTFOLIO = A.PORTFOLIO "
					"AND Z.STATUS IS NULL "
					"AND A.NOM_AMOUNT > 0 ORDER BY 1 ", PrevDate, QDate);
			break;
		case INPUT_OPTIONVALUE:
			UpdateOptionPosition(QDate, TRUE);
			Sql.Format("SELECT A.PORTFOLIO||'\t'||TO_CHAR(A.TRANS_NUM) \"ASSET\", "
						"A.NOM_AMOUNT*NVL(AMORT_FACT, 1)*B.MARKET_MARK*ASSET_FACTOR \"MARKET_MARK\", "
						"A.NOM_AMOUNT*NVL(AMORT_FACT, 1)*C.MARKET_MARK*ASSET_FACTOR \"MARKET_MARK\", "
						"DECODE(C.TRANS_NUM, NULL, 1, -1) \"STATUS\", "
						"SIGN(A.NOM_AMOUNT) \"ID\" "
						"FROM SEMAM.NW_TEMP_OPTIONS A, SEMAM.NW_OPT_PRICES B, "
						"SEMAM.NW_OPT_PRICES C, SEMAM.NW_ASSETS_V D, SEMAM.NW_ASS_PERIODS E, "
						"SEMAM.NW_PORTFOLIOS Z "
						"WHERE B.TRANS_NUM(+) = A.TRANS_NUM "
						"AND B.MM_DATE(+) = %s "
						"AND C.TRANS_NUM(+) = A.TRANS_NUM "
						"AND C.MM_DATE(+) = %s "
						"AND D.ASS_CODE = A.ASSET_CODE "
						"AND E.ASS_CODE(+) = A.ASSET_CODE "
						"AND E.ASS_FROM(+) <= %s "
						"AND E.ASS_TO(+) > %s "
						"AND Z.PORTFOLIO = A.PORTFOLIO "
						"AND Z.STATUS IS NULL "
						"AND A.NOM_AMOUNT > 0 ORDER BY A.PORTFOLIO, A.TRANS_NUM ", 
						PrevDate, QDate, QDate, QDate);
			break;
		case INPUT_VAR_FACTOR:
			Sql.Format("SELECT DISTINCT A.ASSET_CODE, C.VAR_FACT, D.VAR_FACT, "
						"DECODE(A.PORTFOLIO,NULL,1,1) \"STATUS\", "
						"DECODE(NVL(A.DEAL_TYPE, '999'), 'TRS', 1, 1) \"TRS\", "
						"DECODE(SUM(A.NOM_AMOUNT), NULL, 1,1) \"ID|\" "
						"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_ASSETS B, "
						"SEMAM.NW_ASSET_VAR C, SEMAM.NW_ASSET_VAR D, "
						"SEMAM.NW_CURRENCY X, SEMAM.NW_PORTFOLIOS Z "
						"WHERE B.ASS_CODE = A.ASSET_CODE "
						"AND C.ASSET(+) = B.ASS_CODE "
						"AND C.INDATE(+) = %s "
						"AND D.ASSET(+) = B.ASS_CODE "
						"AND D.INDATE(+) = %s "
						"AND X.CURRENCY = A.CURRENCY "
						"AND Z.PORTFOLIO = A.PORTFOLIO "
						"AND Z.STATUS IS NULL "
						"AND A.TRANS_TYPE = 'SECURITIES' "
						"AND A.TRADE_DATE <= %s "
						"GROUP BY A.ASSET_CODE, A.PORTFOLIO, NVL(A.DEAL_TYPE, '999'), C.VAR_FACT, D.VAR_FACT "
						"HAVING SUM(DECODE(TRANS_DIRECTION,'P',1,'S',-1)*A.NOM_AMOUNT/"
						"DECODE(ASS_TYPE, 'BOND TYP(%%) CAL', NVL(X.CONVERSION, 1), 1)) != 0 "
						"UNION "
						"SELECT DISTINCT A.ASSET_CODE, C.VAR_FACT, D.VAR_FACT, "
						"DECODE(A.TRANS_NUM, 1, 1, 1), 1, "
						"DECODE(SIGN(A.NOM_AMOUNT - SUM(NVL(B.NOM_AMOUNT, 0))), 0, 1, 1) "
						"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_TR_TICKETS B, SEMAM.NW_ASSET_VAR C, "
						"SEMAM.NW_ASSET_VAR D, SEMAM.NW_PORTFOLIOS Z "
						"WHERE B.OPTION_BACKING(+) = A.TRANS_NUM "
						"AND B.TRADE_DATE(+) <= %s "
						"AND C.ASSET(+) = A.ASSET_CODE "
						"AND C.INDATE(+) = %s "
						"AND D.ASSET(+) = A.ASSET_CODE "
						"AND D.INDATE(+) = %s "
						"AND Z.PORTFOLIO = A.PORTFOLIO "
						"AND Z.STATUS IS NULL "
						"AND A.TRANS_TYPE IN ('CALL', 'PUT') "
						"AND A.OPTION_BACKING IS NULL AND A.TRADE_DATE <= %s "
						"AND (A.OPT_EXPIRATION IS NULL OR A.OPT_EXPIRATION > %s) "
						"AND (A.EXERCISE_DATE IS NULL OR A.EXERCISE_DATE > %s) "
						"GROUP BY A.ASSET_CODE, A.TRANS_NUM, C.VAR_FACT, D.VAR_FACT, A.NOM_AMOUNT "
						"HAVING (A.NOM_AMOUNT - SUM(NVL(B.NOM_AMOUNT, 0)) > 0) ", PrevDate, QDate, QDate, 
						QDate, PrevDate, QDate, QDate, QDate, QDate);
			break;
		case INPUT_PRICE:
			UpdateOptionPosition(QDate, TRUE);
			if(bUpload)
				Sql.Format("SELECT DISTINCT A.ASSET_CODE, C.MARKET, D.MARKET, E.COST, B.SOURCE, DECODE(A.PORTFOLIO,NULL,1,1) \"STATUS\", "
						"DECODE(NVL(A.DEAL_TYPE, '9'), 'TRS', 1, 1) \"TRS\", "
						"DECODE(SUM(DECODE(DIR, 'P', 1, 'S', -1)*A.NOM_AMOUNT), NULL, 1,1) \"ID\" "
						"FROM SEMAM.NW_CF_ALL_TICKETS A, SEMAM.NW_ASSETS B, "
						"SEMAM.NW_MARKET_PRICES C, SEMAM.NW_MARKET_PRICES D, "
						"SEMAM.NW_ASSET_COST E, SEMAM.NW_PORTFOLIOS Z "
						"WHERE B.ASS_CODE = A.ASSET_CODE "
						"AND B.ASS_INDUSTRY NOT IN ('CURRENCY FWDS') "
						"AND C.ASSET(+) = B.ASS_CODE "
						"AND C.PR_DATE(+) = %s "
						"AND D.ASSET(+) = B.ASS_CODE "
						"AND D.PR_DATE(+) = %s "
						"AND E.ASSET_CODE(+) = B.ASS_CODE "
						"AND Z.PORTFOLIO = A.PORTFOLIO "
						"AND Z.STATUS IS NULL "
						"AND A.TRADE_DATE <= %s "
						"GROUP BY A.ASSET_CODE, A.PORTFOLIO, NVL(A.DEAL_TYPE, '9'), C.MARKET, D.MARKET, E.COST, B.SOURCE "
						"HAVING TRUNC(SUM(DECODE(DIR, 'P', 1, 'S', -1)*A.NOM_AMOUNT), 0) != 0 "
						"UNION "
						"SELECT DISTINCT A.ASSET_CODE, C.MARKET, D.MARKET, E.COST, B.SOURCE, DECODE(A.PORTFOLIO,NULL,1,1) \"STATUS\", "
						"DECODE(NVL(A.DEAL_TYPE, '9'), 'TRS', 1, 1) \"TRS\", 1 \"ID\" "
						"FROM SEMAM.NW_CF_ALL_TICKETS A, SEMAM.NW_ASSETS B, "
						"SEMAM.NW_MARKET_PRICES C, SEMAM.NW_MARKET_PRICES D, "
						"SEMAM.NW_ASSET_COST E, SEMAM.NW_PORTFOLIOS Z "
						"WHERE B.ASS_CODE = A.ASSET_CODE "
						"AND B.ASS_INDUSTRY IN ('CURRENCY FWDS') "
						"AND C.ASSET(+) = B.ASS_CODE "
						"AND C.PR_DATE(+) = %s "
						"AND D.ASSET(+) = B.ASS_CODE "
						"AND D.PR_DATE(+) = %s "
						"AND E.ASSET_CODE(+) = B.ASS_CODE "
						"AND A.TRADE_DATE <= %s "
						"AND A.VALUE_DATE > %s "
						"AND Z.PORTFOLIO = A.PORTFOLIO "
						"AND Z.STATUS IS NULL "
						"UNION "
						"SELECT A.ASSET_CODE, C.MARKET, D.MARKET, E.COST, B.SOURCE, DECODE(A.TRANS_NUM, 1,1,1), 1, 1 "
						"FROM SEMAM.NW_TEMP_OPTIONS A, SEMAM.NW_ASSETS B, "
						"SEMAM.NW_MARKET_PRICES C, SEMAM.NW_MARKET_PRICES D, "
						"SEMAM.NW_ASSET_COST E, SEMAM.NW_CURRENCY X, SEMAM.NW_PORTFOLIOS Z "
						"WHERE B.ASS_CODE = A.ASSET_CODE "
						"AND C.ASSET(+) = A.ASSET_CODE "
						"AND C.PR_DATE(+) = %s "
						"AND D.ASSET(+) = A.ASSET_CODE "
						"AND D.PR_DATE(+) = %s "
						"AND E.ASSET_CODE(+) = A.ASSET_CODE "
						"AND X.CURRENCY = A.CURRENCY "
						"AND Z.PORTFOLIO = A.PORTFOLIO "
						"AND Z.STATUS IS NULL "
						"AND A.TRANS_TYPE IN ('CALL', 'PUT') "
						"AND A.NOM_AMOUNT > 0 "
						"UNION "
						"SELECT DISTINCT A.ASSET_CODE, C.MARKET, D.MARKET, E.COST, B.SOURCE, 1 \"STATUS\", 1 \"TRS\", 1 \"ID\" "
						"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B, "
						"SEMAM.NW_MARKET_PRICES C, SEMAM.NW_MARKET_PRICES D, "
						"SEMAM.NW_ASSET_COST E, SEMAM.NW_PORTFOLIOS Z "
						"WHERE B.ASS_CODE = A.ASSET_CODE "
						"AND C.ASSET(+) = B.ASS_CODE "
						"AND C.PR_DATE(+) = %s "
						"AND D.ASSET(+) = B.ASS_CODE "
						"AND D.PR_DATE(+) = %s "
						"AND E.ASSET_CODE(+) = B.ASS_CODE "
						"AND Z.PORTFOLIO = A.PORTFOLIO "
						"AND Z.STATUS IS NULL "
						"AND A.TRADE_DATE <= %s "
						"AND A.TRANS_TYPE IN ('REPO', 'LEVERAGE') "
						"AND NVL(ACTUAL_VDATE, NVL(MATURITY_DATE, TO_DATE(%s) + 1)) > %s "
						"ORDER BY 1 ", PrevDate, QDate, QDate, PrevDate, QDate, QDate, QDate, 
						PrevDate, QDate, PrevDate, QDate, QDate, QDate, QDate);
			else
				Sql.Format("SELECT DISTINCT A.ASSET_CODE||'\t'||NVL(B.ASS_ISIN_CODE, NVL(B.ASS_MSTC_CODE, NVL(B.ASS_SEDOL_NUM, NVL(B.ASS_COMMON_CODE, NVL(B.RED_CODE, NVL(B.ASS_PID, '__')))))) \"ASSET\", "
						"C.MARKET, D.MARKET, E.COST, B.SOURCE, DECODE(A.PORTFOLIO,NULL,1,1) \"STATUS\", "
						"DECODE(NVL(A.DEAL_TYPE, '9'), 'TRS', 1, 1) \"TRS\", "
						"DECODE(SUM(DECODE(DIR, 'P', 1, 'S', -1)*A.NOM_AMOUNT), NULL, 1,1) \"ID\" "
						"FROM SEMAM.NW_CF_ALL_TICKETS A, SEMAM.NW_ASSETS B, "
						"SEMAM.NW_MARKET_PRICES C, SEMAM.NW_MARKET_PRICES D, "
						"SEMAM.NW_ASSET_COST E, SEMAM.NW_PORTFOLIOS Z "
						"WHERE B.ASS_CODE = A.ASSET_CODE "
						"AND B.ASS_INDUSTRY NOT IN ('CURRENCY FWDS') "
						"AND C.ASSET(+) = B.ASS_CODE "
						"AND C.PR_DATE(+) = %s "
						"AND D.ASSET(+) = B.ASS_CODE "
						"AND D.PR_DATE(+) = %s "
						"AND E.ASSET_CODE(+) = B.ASS_CODE "
						"AND Z.PORTFOLIO = A.PORTFOLIO "
						"AND Z.STATUS IS NULL "
						"AND A.TRADE_DATE <= %s "
						"GROUP BY A.ASSET_CODE||'\t'||NVL(B.ASS_ISIN_CODE, NVL(B.ASS_MSTC_CODE, NVL(B.ASS_SEDOL_NUM, NVL(B.ASS_COMMON_CODE, NVL(B.RED_CODE, NVL(B.ASS_PID, '__')))))), "
						"A.PORTFOLIO, NVL(A.DEAL_TYPE, '9'), C.MARKET, D.MARKET, E.COST, B.SOURCE "
						"HAVING TRUNC(SUM(DECODE(DIR, 'P', 1, 'S', -1)*A.NOM_AMOUNT), 0) != 0 "
						"UNION "
						"SELECT DISTINCT A.ASSET_CODE||'\t'||NVL(B.ASS_ISIN_CODE, NVL(B.ASS_MSTC_CODE, NVL(B.ASS_SEDOL_NUM, NVL(B.ASS_COMMON_CODE, NVL(B.RED_CODE, NVL(B.ASS_PID, '__')))))) \"ASSET\", "
						"C.MARKET, D.MARKET, E.COST, B.SOURCE, DECODE(A.PORTFOLIO,NULL,1,1) \"STATUS\", "
						"DECODE(NVL(A.DEAL_TYPE, '9'), 'TRS', 1, 1) \"TRS\", 1 \"ID\" "
						"FROM SEMAM.NW_CF_ALL_TICKETS A, SEMAM.NW_ASSETS B, "
						"SEMAM.NW_MARKET_PRICES C, SEMAM.NW_MARKET_PRICES D, "
						"SEMAM.NW_ASSET_COST E, SEMAM.NW_PORTFOLIOS Z "
						"WHERE B.ASS_CODE = A.ASSET_CODE "
						"AND B.ASS_INDUSTRY IN ('CURRENCY FWDS') "
						"AND C.ASSET(+) = B.ASS_CODE "
						"AND C.PR_DATE(+) = %s "
						"AND D.ASSET(+) = B.ASS_CODE "
						"AND D.PR_DATE(+) = %s "
						"AND E.ASSET_CODE(+) = B.ASS_CODE "
						"AND A.TRADE_DATE <= %s "
						"AND A.VALUE_DATE > %s "
						"AND Z.PORTFOLIO = A.PORTFOLIO "
						"AND Z.STATUS IS NULL "
						"UNION "
						"SELECT A.ASSET_CODE||'\t'||NVL(B.ASS_ISIN_CODE, NVL(B.ASS_MSTC_CODE, NVL(B.ASS_SEDOL_NUM, NVL(B.ASS_COMMON_CODE, NVL(B.RED_CODE, NVL(B.ASS_PID, '__')))))) \"ASSET\", "
						"C.MARKET, D.MARKET, E.COST, B.SOURCE, DECODE(A.TRANS_NUM, 1,1,1), 1, 1 "
						"FROM SEMAM.NW_TEMP_OPTIONS A, SEMAM.NW_ASSETS B, "
						"SEMAM.NW_MARKET_PRICES C, SEMAM.NW_MARKET_PRICES D, "
						"SEMAM.NW_ASSET_COST E, SEMAM.NW_CURRENCY X, SEMAM.NW_PORTFOLIOS Z "
						"WHERE B.ASS_CODE = A.ASSET_CODE "
						"AND C.ASSET(+) = A.ASSET_CODE "
						"AND C.PR_DATE(+) = %s "
						"AND D.ASSET(+) = A.ASSET_CODE "
						"AND D.PR_DATE(+) = %s "
						"AND E.ASSET_CODE(+) = A.ASSET_CODE "
						"AND X.CURRENCY = A.CURRENCY "
						"AND Z.PORTFOLIO = A.PORTFOLIO "
						"AND Z.STATUS IS NULL "
						"AND A.TRANS_TYPE IN ('CALL', 'PUT') "
						"AND A.NOM_AMOUNT > 0 "
						"UNION "
						"SELECT DISTINCT A.ASSET_CODE||'\t'||NVL(B.ASS_ISIN_CODE, NVL(B.ASS_MSTC_CODE, NVL(B.ASS_SEDOL_NUM, NVL(B.ASS_COMMON_CODE, NVL(B.RED_CODE, NVL(B.ASS_PID, '__')))))) \"ASSET\", "
						"C.MARKET, D.MARKET, E.COST, B.SOURCE, 1 \"STATUS\", 1 \"TRS\", 1 \"ID\" "
						"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B, "
						"SEMAM.NW_MARKET_PRICES C, SEMAM.NW_MARKET_PRICES D, "
						"SEMAM.NW_ASSET_COST E, SEMAM.NW_PORTFOLIOS Z "
						"WHERE B.ASS_CODE = A.ASSET_CODE "
						"AND C.ASSET(+) = B.ASS_CODE "
						"AND C.PR_DATE(+) = %s "
						"AND D.ASSET(+) = B.ASS_CODE "
						"AND D.PR_DATE(+) = %s "
						"AND E.ASSET_CODE(+) = B.ASS_CODE "
						"AND Z.PORTFOLIO = A.PORTFOLIO "
						"AND Z.STATUS IS NULL "
						"AND A.TRADE_DATE <= %s "
						"AND A.TRANS_TYPE IN ('REPO', 'LEVERAGE') "
						"AND NVL(ACTUAL_VDATE, NVL(MATURITY_DATE, TO_DATE(%s) + 1)) > %s "
						"ORDER BY 1 ", PrevDate, QDate, QDate, PrevDate, QDate, QDate, QDate, 
						PrevDate, QDate, PrevDate, QDate, QDate, QDate, QDate);
			bPrice = TRUE;
			break;
		case INPUT_DURATION:
			Sql.Format("SELECT DISTINCT A.ASSET_CODE, C.DURATION, D.DURATION, DECODE(A.PORTFOLIO,NULL,1,1) \"STATUS\", "
					"DECODE(SUM(DECODE(A.DIR, 'P', 1, 'S', -1)*A.NOM_AMOUNT), NULL, 1,1) \"ID\" "
					"FROM SEMAM.NW_CF_ALL_TICKETS A, SEMAM.NW_ASSETS B, SEMAM.NW_DURATION C, "
					"SEMAM.NW_DURATION D, SEMAM.NW_PORTFOLIOS Z "
					"WHERE B.ASS_CODE = A.ASSET_CODE "
					"AND C.ASSET_CODE(+) = B.ASS_CODE "
					"AND C.INDATE(+) = %s "
					"AND D.ASSET_CODE(+) = B.ASS_CODE "
					"AND D.INDATE(+) = %s "
					"AND Z.PORTFOLIO = A.PORTFOLIO "
					"AND Z.STATUS IS NULL "
					"AND A.TRADE_DATE <= %s "
					"AND B.ASS_ACCRUABLE = 'Y' "
					"AND B.ASS_PPAID_INT = 'Y' "
					"AND (ASS_MSTC_CODE IS NOT NULL OR ASS_ISIN_CODE IS NOT NULL OR "
					"ASS_COMMON_CODE IS NOT NULL OR ASS_SEDOL_NUM IS NOT NULL ) "
					"GROUP BY A.ASSET_CODE, A.PORTFOLIO, NVL(A.DEAL_TYPE, '9'), C.DURATION, D.DURATION "
					"HAVING TRUNC(SUM(DECODE(DIR,  'P', 1, 'S', -1)*A.NOM_AMOUNT), 0) != 0 "
					"UNION ALL "
					"SELECT C.ASS_CODE, D.DURATION, E.DURATION, DECODE(A.TRANS_NUM, 1,1,1), "
					"SIGN(A.NOM_AMOUNT - SUM(NVL(B.NOM_AMOUNT, 0))) "
					"FROM SEMAM.ALL_TICKETS A, SEMAM.ALL_TICKETS B, SEMAM.NW_ASSETS C, "
					"SEMAM.NW_DURATION D, SEMAM.NW_DURATION E, SEMAM.NW_CURRENCY X, "
					"SEMAM.NW_PORTFOLIOS Z "
					"WHERE B.OPTION_BACKING(+) = A.TRANS_NUM "
					"AND B.TRADE_DATE(+) = %s "
					"AND C.ASS_CODE = A.ASSET_CODE "
					"AND D.ASSET_CODE(+) = C.ASS_CODE "
					"AND D.INDATE(+) = %s "
					"AND E.ASSET_CODE(+) = A.ASSET_CODE "
					"AND E.INDATE(+) = %s "
					"AND X.CURRENCY = A.CURRENCY "
					"AND Z.PORTFOLIO = A.PORTFOLIO "
					"AND Z.STATUS IS NULL "
					"AND A.OPTION_BACKING IS NULL "
					"AND A.TRANS_TYPE IN ('CALL', 'PUT') "
					"AND (A.TRADE_DATE <= %s OR A.VALUE_DATE <= %s) "
					"AND (A.OPT_EXPIRATION IS NULL OR A.OPT_EXPIRATION >  %s) "
					"AND (A.EXERCISE_DATE IS NULL OR A.EXERCISE_DATE >  %s) "
					"AND A.CURRENCY = X.CURRENCY AND ASS_ACCRUABLE = 'Y' "
					"AND ASS_PPAID_INT = 'Y' "
					"AND (ASS_MSTC_CODE IS NOT NULL OR ASS_ISIN_CODE IS NOT NULL OR "
							"ASS_COMMON_CODE IS NOT NULL OR ASS_SEDOL_NUM IS NOT NULL) "
					"GROUP BY C.ASS_CODE, D.DURATION, E.DURATION, A.TRANS_NUM, A.NOM_AMOUNT "
					"HAVING A.NOM_AMOUNT - SUM(NVL(B.NOM_AMOUNT, 0)) > 0 "
					"UNION ALL "
					"SELECT ASS_CODE, C.DURATION, D.DURATION, 1, 1 "
					"FROM SEMAM.NW_ASSETS A, SEMAM.NW_DURATION C, SEMAM.NW_DURATION D "
					"WHERE C.ASSET_CODE(+) = A.ASS_CODE "
					"AND C.INDATE(+) = %s "
					"AND D.ASSET_CODE(+) = A.ASS_CODE "
					"AND D.INDATE(+) = %s "
					"AND A.ASS_CODE IN ('US T3 YR', 'US T5 YR', 'US T10 YR', 'US T30 YR') "
					"ORDER BY 1 ", PrevDate, QDate, QDate, QDate, QDate, QDate, QDate, QDate, 
					PrevDate, QDate, PrevDate, QDate);
			bPrice = TRUE;
			break;
		case INPUT_RATE:
			UpdateOptionPosition(QDate);
			Sql.Format("SELECT A.CURRENCY||'\t'||TO_CHAR(A.CASHDAYS), B.RATE, C.RATE, "
						"DECODE(C.RATE, NULL, 1, -1) " 
						"FROM SEMAM.RISKFREE_INPUT_V A, SEMAM.NW_RISKFREE_RATES B, "
						"SEMAM.NW_RISKFREE_RATES C "
						"WHERE B.CURRENCY(+) = A.CURRENCY "
						"AND B.CASHDAYS(+) = A.CASHDAYS "
						"AND B.INDATE(+) = %s "
						"AND B.CURRENCY(+) = A.CURRENCY "
						"AND B.CASHDAYS(+) = A.CASHDAYS "
						"AND B.INDATE(+) = %s "
						"ORDER BY A.CURRENCY, A.CASHDAYS ", PrevDate, QDate);
			break;
		case INPUT_VOL:
			break;
		case INPUT_ACCR_FACT:
			Sql.Format("SELECT DISTINCT A.ASSET_CODE, C.ACCR_FACTOR, D.ACCR_FACTOR, "
				"DECODE(A.PORTFOLIO,NULL,1,1) \"STATUS\", " 
				"DECODE(NVL(A.DEAL_TYPE, '9'), 'TRS', 1, 1) \"TRS\", "
				"DECODE(SUM(DECODE(DIR, 'P', 1, 'S', -1)*A.NOM_AMOUNT), NULL, 1,1) \"ID\" "
				"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B, SEMAM.NW_ACCR_FACTOR C, "
				"SEMAM.NW_ACCR_FACTOR D, SEMAM.NW_PORTFOLIOS Z " 
				"WHERE B.ASS_CODE = A.ASSET_CODE "
				"AND B.ASS_ACCRUABLE = 'Y' "
				"AND B.ASS_INT_TYPE = 'FLOAT' "
				"AND C.ASSET_CODE(+) = B.ASS_CODE "
				"AND C.INDATE(+) = %s "
				"AND D.ASSET_CODE(+) = B.ASS_CODE "
				"AND D.INDATE(+) = %s "
				"AND Z.PORTFOLIO = A.PORTFOLIO "
				"AND Z.STATUS IS NULL "
				"AND A.TRADE_DATE <= %s "
				"AND A.TRANS_TYPE IN ('CDS', 'SECURITIES') "
				"AND 0 = (SELECT COUNT(*) FROM SEMAM.NW_ASS_PERIODS E "
							"WHERE E.ASS_CODE = A.ASSET_CODE "
							"AND E.ASS_FROM <= %s AND E.ASS_TO > %s) "
				"GROUP BY A.ASSET_CODE, A.PORTFOLIO, NVL(A.DEAL_TYPE, '9'), C.ACCR_FACTOR, D.ACCR_FACTOR "
				"HAVING TRUNC(SUM(DECODE(DIR, 'P', 1, 'S', -1)*A.NOM_AMOUNT), 0) != 0 "
				"UNION "
				"SELECT DISTINCT A.ASSET_CODE, C.ACCR_FACTOR, D.ACCR_FACTOR, "
				"DECODE(A.PORTFOLIO,NULL,1,1) \"STATUS\", "
				"DECODE(NVL(A.DEAL_TYPE, '9'), 'TRS', 1, 1) \"TRS\", "
				"DECODE(DECODE(DIR, 'P', 1, 'S', -1)*A.NOM_AMOUNT, NULL, 1,1) \"ID\" "
				"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B, SEMAM.NW_ACCR_FACTOR C, "
				"SEMAM.NW_ACCR_FACTOR D, SEMAM.NW_PORTFOLIOS Z "
				"WHERE B.ASS_CODE = A.ASSET_CODE "
				"AND B.ASS_ACCRUABLE = 'Y' "
				"AND B.ASS_INT_TYPE = 'FLOAT' "
				"AND C.ASSET_CODE(+) = B.ASS_CODE "
				"AND C.INDATE(+) = %s "
				"AND D.ASSET_CODE(+) = B.ASS_CODE "
				"AND D.INDATE(+) = %s "
				"AND Z.PORTFOLIO = A.PORTFOLIO "
				"AND Z.STATUS IS NULL "
				"AND A.TRADE_DATE <= %s "
				"AND A.TRANS_TYPE IN ('INT. SWAP') "
				"AND NVL(ACTUAL_VDATE, NVL(MATURITY_DATE, TO_DATE(%s) + 1)) > %s "
				"AND 0 = (SELECT COUNT(*) FROM SEMAM.NW_ASS_PERIODS E WHERE E.ASS_CODE = A.ASSET_CODE "
							"AND E.ASS_FROM <= %s AND E.ASS_TO > %s) "
				"ORDER BY 1 ", PrevDate, QDate, QDate, QDate, QDate, 
				PrevDate, QDate, QDate, QDate, QDate, QDate, QDate);
			break;
		case INPUT_DV01:
			UpdateOptionPosition(QDate);
			Sql.Format("SELECT DISTINCT A.ASSET_CODE||'\tZ' \"ASSET_CODE\", D.DV01, E.DV01, "
				"DECODE(A.PORTFOLIO,NULL, 1, 1) \"STATUS\", " 
				"DECODE(SUM(DECODE(A.TRANS_DIRECTION, 'P', 1, 'S', -1)*A.NOM_AMOUNT), NULL, 1,1) \"ID\" "
				"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_ASSETS B, SEMAM.NW_INDUSTRIES C, "
				"SEMAM.NW_DV01 D, SEMAM.NW_DV01 E, SEMAM.NW_PORTFOLIOS Z "
				"WHERE B.ASS_CODE = A.ASSET_CODE "
				"AND C.IND_CODE = B.ASS_INDUSTRY "
				"AND C.PROFILE_GROUP IN ('DM BONDS', 'EM BONDS') "
				"AND D.ASSET_CODE(+) = B.ASS_CODE "
				"AND D.INDATE(+) = %s "
				"AND E.ASSET_CODE(+) = B.ASS_CODE "
				"AND E.INDATE(+) = %s "
				"AND Z.PORTFOLIO = A.PORTFOLIO "
				"AND Z.STATUS IS  NULL "
				"AND A.TRADE_DATE <= %s "
				"AND A.TRANS_TYPE IN ('CDS', 'SECURITIES') "
  				"GROUP BY A.ASSET_CODE, A.PORTFOLIO, NVL(A.DEAL_TYPE, '9'), D.DV01, E.DV01 "
				"HAVING TRUNC(SUM(DECODE(A.TRANS_DIRECTION, 'P', 1, 'S', -1)*A.NOM_AMOUNT), 0) != 0 "
				"UNION "
				"SELECT DISTINCT A.ASSET_CODE|| '\tZ' \"ASSET_CODE\", D.DV01, E.DV01, "
				"DECODE(A.PORTFOLIO, NULL, 1, 1) \"STATUS\", 1 "
				"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B, SEMAM.NW_DV01 D, SEMAM.NW_DV01 E, "
				"SEMAM.NW_PORTFOLIOS Z "
				"WHERE B.ASS_CODE = A.ASSET_CODE "
				"AND B.ASS_INT_TYPE = 'FIXED' "
				"AND D.ASSET_CODE(+) = B.ASS_CODE "
				"AND D.INDATE(+) = %s "
				"AND E.ASSET_CODE(+) = B.ASS_CODE "
				"AND E.INDATE(+) = %s "
				"AND Z.PORTFOLIO = A.PORTFOLIO "
				"AND Z.STATUS IS  NULL "
				"AND A.TRADE_DATE <= %s " 
				"AND A.TRANS_TYPE IN ('INT. SWAP') "
				"AND NVL(A.ACTUAL_VDATE, NVL(A.MATURITY_DATE, NVL(ASS_MATURITY_DATE, SYSDATE))) > %s " 
				"UNION "
				"SELECT DISTINCT A.ASSET_CODE||'\t'||A.TICKET_NUM \"ASSET_CODE\", D.DV01, E.DV01, "
				"DECODE(A.PORTFOLIO, NULL, 1, 1) \"STATUS\", 1 "
  				"FROM SEMAM.NW_TEMP_OPTIONS A, SEMAM.NW_ASSETS B, SEMAM.NW_DV01 D, SEMAM.NW_DV01 E, "
				"SEMAM.NW_PORTFOLIOS Z "
				"WHERE B.ASS_CODE = A.ASSET_CODE "
				"AND B.ASS_INDUSTRY IN ('DM SWAPS', 'EM SWAPS', 'EM CDS SWAPS', 'G7 CDS SWAPS', 'FI FUTURES', 'G7 FI FUTURES') "
				"AND D.TICKET_NUM(+) = A.TICKET_NUM "
				"AND D.INDATE(+) = %s "
				"AND E.TICKET_NUM(+) = A.TICKET_NUM "
				"AND E.INDATE(+) = %s "
				"AND Z.PORTFOLIO = A.PORTFOLIO "
				"AND Z.STATUS IS  NULL "
				"AND A.NOM_AMOUNT > 0 "
				"UNION "
				"SELECT 'US TYA\tZ' \"ASSET_CODE\", D.DV01, E.DV01, 1, 1 "
				"FROM SEMAM.NW_DV01 D, SEMAM.NW_DV01 E "
				"WHERE D.ASSET_CODE(+) = 'US TYA' AND D.INDATE(+) = %s "
				"AND E.ASSET_CODE(+) = D.ASSET_CODE AND E.INDATE(+) = %s ORDER BY 1 ", 
				PrevDate, QDate, QDate, PrevDate, QDate, QDate, QDate, PrevDate, QDate, PrevDate, QDate);
			break;
		case INPUT_ISSUER_AMOUNT:
			Sql.Format("SELECT A.ISSUER, B.AMOUNT \"PREV AMOUNT\", C.AMOUNT, DECODE(C.ISSUER, NULL, 1, -1) \"STATUS\" "
				"FROM SEMAM.NW_ISSUERS A, SEMAM.NW_ISSUER_AMOUNT B, SEMAM.NW_ISSUER_AMOUNT C "
				"WHERE B.ISSUER(+) = A.ISSUER "
				"AND B.INDATE(+) = %s "
				"AND C.ISSUER(+) = A.ISSUER "
				"AND C.INDATE(+) = %s "
				"ORDER BY 1, 2, 3, 4 ",	PrevDate, QDate);
			break;
		default:
			break;
	}

	if(Sql.IsEmpty())
		return;

	m_DataArray.LoadPriceDataArray(Sql, bPrice);
	m_DataArray.LoadBlmPriceArray(m_MenuID);
}

void CDailyInput::SaveData()
{
	m_bReload = FALSE;

	switch(m_MenuID)
	{
		case INPUT_FXRATE:
			if(m_DataArray.OpenInsertSql("SELECT EXRATE, CURRENCY, INDATE FROM SEMAM.NW_EXRATES "))
				m_DataArray.InsertPriceData();
			m_DataArray.UpdatePriceData("UPDATE SEMAM.NW_EXRATES SET EXRATE = %s "
										"WHERE CURRENCY = %s AND INDATE = %s ");
			break;
		case INPUT_COST:
			if(m_DataArray.DeletePriceData("DELETE FROM SEMAM.NW_ASSET_COST WHERE ASSET_CODE = %s "))
				m_bReload = TRUE;
			if(m_DataArray.OpenInsertSql("SELECT COST, ASSET_CODE FROM SEMAM.NW_ASSET_COST "))
				m_DataArray.InsertPriceData();
			m_DataArray.UpdatePriceData("UPDATE SEMAM.NW_ASSET_COST SET COST = %s "
										"WHERE ASSET_CODE = %s ", TRUE);
			break;
		case INPUT_INTRATE:
//			m_DataArray.OpenInsertSql(IDS_SQL_89);
			break;
		case INPUT_SWAP:
			m_DataArray.InsertSwapData("INSERT INTO SEMAM.NW_DEAL_NAV "
							"(INDATE, TRANS_NUM, VALUE, TICKET_NUM, ASSET_CODE, TR_DIR, TR_TYPE) "
							"SELECT %s, TRANS_NUM, %s, TICKET_NUM, ASSET_CODE, TRANS_DIRECTION, "
							"TRANS_TYPE FROM SEMAM.NW_TR_TICKETS WHERE TRANS_NUM = %s ");
			m_DataArray.UpdateSwapData("UPDATE SEMAM.NW_DEAL_NAV SET VALUE = %s "
										"WHERE TRANS_NUM = %s AND INDATE = %s ");
			break;
		case INPUT_OPTION:
			m_DataArray.InsertOptionData("SELECT MARKET_MARK, TRANS_NUM, MM_DATE "
										"FROM SEMAM.NW_OPT_PRICES ");
			m_DataArray.UpdateOptionData("UPDATE SEMAM.NW_OPT_PRICES SET MARKET_MARK = %s "
										"WHERE TRANS_NUM = %s AND MM_DATE = %s ");
			break;
		case INPUT_OPTION_DELTA:
			if(m_DataArray.OpenInsertSql("SELECT DELTA, TRANS_NUM, INDATE FROM SEMAM.NW_OPT_DELTA "))
				m_DataArray.InsertPriceData();
			m_DataArray.UpdatePriceData("UPDATE SEMAM.NW_OPT_DELTA SET DELTA = %s "
										"WHERE TRANS_NUM = %s AND INDATE = %s ");
			m_DataArray.GetOraLoader().ExecuteSql("DELETE FROM SEMAM.NW_OPT_DELTA WHERE ABS(DELTA) >= 1 ");
			break;
		case INPUT_OPTIONVALUE:
			UpdateOptionPosition(m_DataArray.GetQDate());
			m_DataArray.InsertOptionData("SELECT MARKET_MARK, TRANS_NUM, MM_DATE "
										"FROM SEMAM.NW_OPT_PRICES ");
			m_DataArray.UpdateOptionData("UPDATE SEMAM.NW_OPT_PRICES SET MARKET_MARK = %s "
										"WHERE TRANS_NUM = %s AND MM_DATE = %s ");
			break;
		case INPUT_VAR_FACTOR:
			if(m_DataArray.OpenInsertSql("SELECT VAR_FACT, ASSET, INDATE FROM SEMAM.NW_ASSET_VAR "))
				m_DataArray.InsertPriceData();			
			m_DataArray.UpdatePriceData("UPDATE SEMAM.NW_ASSET_VAR SET VAR_FACT = %s "
										"WHERE ASSET = %s AND INDATE = %s ");
			break;
		case INPUT_PRICE:
			if(m_DataArray.OpenInsertSql("SELECT MARKET, ASSET, PR_DATE FROM SEMAM.NW_MARKET_PRICES "))
				m_DataArray.InsertPriceData();
			m_DataArray.UpdatePriceData("UPDATE SEMAM.NW_MARKET_PRICES SET MARKET = %s "
										"WHERE ASSET = %s AND PR_DATE = %s");
			break;
		case INPUT_DURATION:
			if(m_DataArray.OpenInsertSql("SELECT DURATION, ASSET_CODE, INDATE FROM SEMAM.NW_DURATION "))
				m_DataArray.InsertPriceData();
			m_DataArray.UpdatePriceData("UPDATE SEMAM.NW_DURATION SET DURATION = %s "
										"WHERE ASSET_CODE = %s AND INDATE = %s ");
			break;
		case INPUT_RATE:
			m_DataArray.InsertRiskFreeRateData("SELECT CURRENCY, CASHDAYS, RATE, INDATE "
												"FROM SEMAM.NW_RISKFREE_RATES ");	
			m_DataArray.UpdateRiskFreeRateData("UPDATE SEMAM.NW_RISKFREE_RATES SET RATE = %s "
										"WHERE CURRENCY = '%s' AND CASHDAYS = %s AND INDATE = %s ");
			break;
		case INPUT_VOL:
			break;
		case INPUT_ACCR_FACT: 
			if(m_DataArray.OpenInsertSql("SELECT ACCR_FACTOR, ASSET_CODE, INDATE FROM SEMAM.NW_ACCR_FACTOR "))
				m_DataArray.InsertPriceData();
			m_DataArray.UpdatePriceData("UPDATE SEMAM.NW_ACCR_FACTOR SET ACCR_FACTOR = %s "
										"WHERE ASSET_CODE = %s AND INDATE = %s ");
			break;
		case INPUT_DV01:
			m_DataArray.InsertDV01Data("INSERT INTO SEMAM.NW_DV01 (DV01, ASSET_CODE, TICKET_NUM, INDATE) "
										"VALUES (%s, %s, %s, %s) ");
			m_DataArray.UpdateDV01Data("UPDATE SEMAM.NW_DV01 SET DV01 = %s "
						"WHERE ASSET_CODE = %s AND TICKET_NUM = %s AND INDATE = %s ",
						"UPDATE SEMAM.NW_DV01 SET DV01 = %s WHERE ASSET_CODE = %s AND INDATE = %s");
			break;
		case INPUT_ISSUER_AMOUNT:
			if(m_DataArray.OpenInsertSql("SELECT AMOUNT, ISSUER, INDATE FROM SEMAM.NW_ISSUER_AMOUNT "))
				m_DataArray.InsertPriceData();
			m_DataArray.UpdatePriceData("UPDATE SEMAM.NW_ISSUER_AMOUNT SET AMOUNT = %s "
										"WHERE ISSUER= %s AND INDATE = %s ");
			break;
		default:
			break;
	}
}

void CDailyInput::LoadSpreadSheet()
{
    if(m_SS.GetSheetRows() > 0)
    	m_SS.ClearSheet();

	m_SS.SetSheetRows(m_DataArray.GetSize());
    if(m_SS.GetSheetRows() <= 0)
    	return;

	m_SS.SetCol(1);
	m_SS.SetCol2(m_nEditCol - 2);
	m_SS.SetRow(-1);
	m_SS.SetRow2(-1);
	m_SS.SetBlockMode(TRUE);
	m_SS.SetCellType(SS_CELL_TYPE_EDIT);
	m_SS.SetBlockMode(FALSE);

	m_SS.SetCol(m_nEditCol - 1); // Previous, Current, Diff, Status
	m_SS.SetCol2(m_nEditCol + 2);
	m_SS.SetBlockMode(TRUE);
    m_SS.SetCellType(SS_CELL_TYPE_NUMBER);
    m_SS.SetTypeHAlign(SS_CELL_H_ALIGN_RIGHT);
    m_SS.SetTypeNumberMin(-9999999999.0);
    m_SS.SetTypeNumberMax((double) 9999999999);
	m_SS.SetTypeNumberDecPlaces(9);
    m_SS.SetTypeNumberSeparator(",");
	m_SS.SetTypeNumberShowSep(TRUE);
//    m_SS.SetTypeCurrencySymbol("$");
    m_SS.SetBlockMode(FALSE);

	m_SS.SetCol(m_nEditCol + 2); // Status
	m_SS.SetCol2(m_nEditCol + 2);
	m_SS.SetBlockMode(TRUE);
	m_SS.SetTypeNumberDecPlaces(2);
	m_SS.SetTypeNumberMin(-10000.0);
	m_SS.SetTypeNumberMax(10000.0);
	m_SS.SetBlockMode(FALSE);

   	m_SS.SetMaxRows(m_SS.GetSheetRows() > 22 ? m_SS.GetSheetRows() : 22);
	m_SS.SetVisibleCols(m_nEditCol + 2);
	m_SS.SetVisibleRows(22);

	for(int i = 1; i <= m_SS.GetSheetRows(); i ++) // set data on the spread sheet
		SetSheetRow(i, m_DataArray.GetAt(i - 1));

	SetSheetSize();
}

void CDailyInput::SetSheetRow(int Row, CPriceData *pData, BOOL Beep)
{
	double Result;
	CStringArray Rec;
	PriceStatus_t Status;
	CString Current, Diff;
	CQData QData;

	if(!pData || pData->GetSize() <= 0)
		return;

	Rec.Add(pData->GetAsset());
	Rec.Add(pData->GetPrevPrice());
	if(!pData->GetPrice() || strlen(pData->GetPrice()) == 0)
		Current.Empty();
	else
		Current = pData->GetPrice();
	
	Rec.Add(QData.WriteNumber(Current, TRUE, 9));

	if(strlen(pData->GetPrevPrice()) == 0 || Current.IsEmpty())
		Diff.Empty();
	else
		Diff = QData.WriteNumber(atof(Current) - atof(pData->GetPrevPrice()), TRUE, 9);

	Rec.Add(Diff);

	if(m_DataArray.ComputePriceChange(*pData, Result, Status))
	{
		if(Beep && fabs(Result) >= m_DataArray.GetWarnLimit())
			MessageBeep(0);

		Rec.Add(QData.WriteNumber(Result, TRUE, 2));
	}
	else
		Rec.Add(EMPTYSTRING);

	m_SS.UpdateSheetRow(Row, Rec);
	SetSheetCellColor(Row, Status);
	m_SS.LockSheetRow(Row, TRUE);
	m_SS.LockSheetCell(m_nEditCol, Row, Status.m_bLock);
}

void CDailyInput::SetSheetCellColor(int Row, PriceStatus_t &Status)
{
	m_SS.SetCol(1);
	m_SS.SetRow(Row);
	m_SS.SetCol2(m_nEditCol + 2);
	m_SS.SetRow2(Row);
	m_SS.SetBlockMode(TRUE);
	m_SS.SetBackColor(RGB(255, 255, 255));// Normal

	if(Status.m_bLock) // Locked
		m_SS.SetBackColor(RGB(192, 192, 192));
	m_SS.SetBlockMode(FALSE);
	
/*	if(Status.m_bOverLimit) // Exceeds limit
	{
		m_SS.SetCol(1);
		m_SS.SetCol2(m_nEditCol);
		m_SS.SetRow(Row);
		m_SS.SetRow2(Row);
		m_SS.SetBlockMode(TRUE);
		m_SS.SetForeColor(RGB(255, 0, 0));
		m_SS.SetBlockMode(FALSE);
	}

	m_SS.SetCol(m_nEditCol + 1);
	m_SS.SetCol2(m_nEditCol + 2);
	m_SS.SetRow(Row);
	m_SS.SetRow2(Row);
	m_SS.SetBlockMode(TRUE);
	switch(Status.m_nOverUnderEqual)
	{
		case 1: // Price up
			m_SS.SetBackColor(RGB(0, 255, 0));
			break;
		case -1: // Price down
			m_SS.SetBackColor(RGB(255, 0, 0));
			break;		
		default: // Price unchange
			m_SS.SetBackColor(RGB(255, 255, 255));  // Normal
			break;
	} */

	if(Status.m_bBlank)
		m_SS.SetCol(m_nEditCol);
	else
		m_SS.SetCol(m_nEditCol + 1);
	
	m_SS.SetCol2(m_nEditCol + 2);
	m_SS.SetRow(Row);
	m_SS.SetRow2(Row);
	m_SS.SetBlockMode(TRUE);

	if(Status.m_bOverLimit || Status.m_bBlank) // Exceeds Limit or Blank
		m_SS.SetBackColor(RGB(255, 0, 0));
	else
		m_SS.SetBackColor(RGB(255, 255, 255));

	m_SS.SetBlockMode(FALSE);
}

void CDailyInput::SetSheetSize()
{
	m_SS.SetSheetToFit(m_SS.GetVisibleCols(), m_SS.GetVisibleRows());
	m_SS.SetColWidth(m_nEditCol, m_SS.GetMaxTextColWidth(m_nEditCol - 1)*1.2);
	m_SS.SetColWidth(m_nEditCol+1, m_SS.GetMaxTextColWidth(m_nEditCol - 1)*1.2);
	m_SS.SetColWidth(m_nEditCol+2, m_SS.GetMaxTextColWidth(m_nEditCol - 1));
	m_SS.SetAllowCellOverflow(TRUE);
	m_SS.SetSheetFontBold(TRUE);
	m_SS.SetAutoSize(TRUE);
}

void CDailyInput::SetTitle()
{
	CString T;

	switch(m_MenuID)
	{
		case INPUT_FXRATE:
			 T = "INPUT FXRATE";
			 break;
		case INPUT_PRICE:
			 T = "INPUT PRICE";
			 break;
		case INPUT_COST:
			 T = "INPUT COST";
			 break;
		case INPUT_OPTION:
			 T = "INPUT OPTION";
			 break;
		case INPUT_OPTION_DELTA:
			 T = "INPUT OPTION DELTA";
			 break;
		case INPUT_OPTIONVALUE:
			 T = "INPUT OPTION VALUE";
			 break;
		case INPUT_DURATION:
			 T = "INPUT DURATION";
			 break;
		case INPUT_SWAP:
			 T = "INPUT SWAP";
			 break;
		case INPUT_VAR_FACTOR:
			 T = "INPUT VAR FACTOR";
			 break;
		case INPUT_RATE:
			 T = "INPUT RISK FREE RATE";
			 break;
		case INPUT_VOL: 
			 T = "INPUT OPTION VOL";
			 break;
		case INPUT_ACCR_FACT:
			 "INPUT ACCRUAL FACTOR";
			 break;
		case INPUT_DV01:
			 T = "INPUT DV01";
			 break;
		default:
			 break;
	}
																					
	GetDocument()->SetViewTitle(T);
}

BOOL CDailyInput::UpdateData(BOOL bSaveandValid)
{
	BeginWaitCursor();
	if(bSaveandValid)
	{
		if(m_DataArray.GetModify())
		{
			SaveData();
		}
	}
	else
	{
		LoadData();		
		LoadSpreadSheet();
	}
	EndWaitCursor();
	
	return TRUE;
}

void CDailyInput::UpdateOptionPosition(LPCTSTR QDate, BOOL bFxOpt)
{
	CString Sql;
	COraLoader OraLoader;

	OraLoader = GetDocument()->GetData().GetOraLoader();
	OraLoader.GetSql() = "DELETE FROM SEMAM.NW_TEMP_OPTIONS ";
	OraLoader.ExecuteSql();
	
	if(bFxOpt)
	{
		Sql.Format("INSERT INTO SEMAM.NW_TEMP_OPTIONS (TRANS_NUM, PORTFOLIO, TICKET_NUM, TRADE_DATE, TRANS_TYPE, "
					"TRANS_DIRECTION, NOM_AMOUNT, PRICE, ASSET_CODE, COUNTERPARTY, CON_CODE, OPTION_BACKING, "
					"EXERCISE_PRICE, EXERCISE_DATE, VALUE_DATE, CURRENCY, FXRATE, BROKER, BR_FEES, OPT_EXPIRATION, "
					"DEAL_TYPE, TRADER_INI, CUSTODIAN, OPT_TICK, OPT_ID, EURO_OPT, OPT_AUTO, OPT_SET_CODE, "
					"OPT_SET_CODE2, SET_CONVENTION, DELIVERY_DATE, CP_TRADE_ID, MARGIN, MARGIN_CURRENCY, "
					"MARGIN_AMOUNT, BINARY, VAR, SHORT_SALE, HAN_DETAIL1, HAN_DETAIL2) "
					"SELECT A.TRANS_NUM, A.PORTFOLIO, A.TICKET_NUM, A.TRADE_DATE, A.TRANS_TYPE, A.TRANS_DIRECTION, "
					"A.NOM_AMOUNT - NVL(SUM(B.NOM_AMOUNT),0), A.PRICE, A.ASSET_CODE, A.COUNTERPARTY, A.CON_CODE, "
					"A.OPTION_BACKING, A.EXERCISE_PRICE, A.EXERCISE_DATE, A.VALUE_DATE, A.CURRENCY, A.FXRATE, "
					"A.BROKER, A.BR_FEES, A.OPT_EXPIRATION, A.DEAL_TYPE, A.TRADER_INI, A.CUSTODIAN, A.OPT_TICK, "
					"A.OPT_ID, A.EURO_OPT, A.OPT_AUTO, A.OPT_SET_CODE, A.OPT_SET_CODE2, A.SET_CONVENTION, "
					"A.DELIVERY_DATE, A.CP_TRADE_ID, A.MARGIN, A.MARGIN_CURRENCY, A.MARGIN_AMOUNT, A.BINARY, "
					"A.VAR, A.SHORT_SALE, A.HAN_DETAIL1, A.HAN_DETAIL2 "
					"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_TR_TICKETS B, SEMAM.NW_ASSETS C, "
					"SEMAM.NW_PORTFOLIOS Z "
					"WHERE B.OPTION_BACKING(+) = A.TRANS_NUM "
					"AND B.TRADE_DATE(+) <= %s "
					"AND C.ASS_CODE = A.ASSET_CODE "
					"AND C.ASS_INDUSTRY NOT IN ('CURRENCY OPTION') "
					"AND Z.PORTFOLIO = A.PORTFOLIO "
					"AND Z.STATUS IS NULL "
					"AND A.OPTION_BACKING IS NULL "
					"AND A.TRANS_TYPE IN  ('CALL','PUT') "
					"AND LEAST(A.TRADE_DATE, A.VALUE_DATE) <= %s "
					"AND (A.OPT_EXPIRATION IS NULL OR A.OPT_EXPIRATION > %s) "
					"AND (A.EXERCISE_DATE IS NULL OR A.EXERCISE_DATE > %s) "
					"GROUP BY A.TRANS_NUM, A.PORTFOLIO, A.TICKET_NUM, A.TRADE_DATE, A.TRANS_TYPE, A.TRANS_DIRECTION, "
					"A.NOM_AMOUNT, A.PRICE, A.ASSET_CODE, A.COUNTERPARTY, A.CON_CODE, A.OPTION_BACKING, A.EXERCISE_PRICE, "
					"A.EXERCISE_DATE, A.VALUE_DATE, A.CURRENCY, A.FXRATE, A.BROKER, A.BR_FEES, A.OPT_EXPIRATION, "
					"A.DEAL_TYPE, A.TRADER_INI, A.CUSTODIAN, A.OPT_TICK, A.OPT_ID, A.EURO_OPT, A.OPT_AUTO, A.OPT_SET_CODE, "
					"A.OPT_SET_CODE2, A.SET_CONVENTION, A.DELIVERY_DATE, A.CP_TRADE_ID, A.MARGIN, A.MARGIN_CURRENCY, "
					"A.MARGIN_AMOUNT, A.BINARY, A.VAR, A.SHORT_SALE, A.HAN_DETAIL1, A.HAN_DETAIL2 "
					"HAVING A.NOM_AMOUNT - NVL(SUM(B.NOM_AMOUNT),0) > 0 "
					"UNION "
					"SELECT A.TRANS_NUM, A.PORTFOLIO, A.TICKET_NUM, A.TRADE_DATE, A.TRANS_TYPE, A.TRANS_DIRECTION, "
					"A.NOM_AMOUNT, A.PRICE, A.ASSET_CODE, A.COUNTERPARTY, A.CON_CODE, A.OPTION_BACKING, "
					"A.EXERCISE_PRICE, A.EXERCISE_DATE, A.VALUE_DATE, A.CURRENCY, A.FXRATE, A.BROKER, A.BR_FEES, "
					"A.OPT_EXPIRATION, A.DEAL_TYPE, A.TRADER_INI, A.CUSTODIAN, A.OPT_TICK, A.OPT_ID, A.EURO_OPT, "
					"A.OPT_AUTO, A.OPT_SET_CODE, A.OPT_SET_CODE2, A.SET_CONVENTION, A.DELIVERY_DATE, A.CP_TRADE_ID, "
					"A.MARGIN, A.MARGIN_CURRENCY, A.MARGIN_AMOUNT, A.BINARY, A.VAR, A.SHORT_SALE, A.HAN_DETAIL1, "
					"A.HAN_DETAIL2 "
					"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_ASSETS C, SEMAM.NW_PORTFOLIOS Z "
					"WHERE C.ASS_CODE = A.ASSET_CODE "
					"AND C.ASS_INDUSTRY IN ('CURRENCY OPTION') "
					"AND Z.PORTFOLIO = A.PORTFOLIO "
					"AND Z.STATUS IS NULL "
					"AND A.TRANS_TYPE IN ('CALL','PUT') "
					"AND LEAST(A.TRADE_DATE, A.VALUE_DATE) <= %s "
					"AND (A.OPT_EXPIRATION IS NULL OR A.OPT_EXPIRATION > %s) ", QDate, QDate, QDate, QDate, QDate, QDate);
	}
	else
	{
		Sql.Format("INSERT INTO SEMAM.NW_TEMP_OPTIONS (TRANS_NUM, PORTFOLIO, TICKET_NUM, TRADE_DATE, TRANS_TYPE, "
					"TRANS_DIRECTION, NOM_AMOUNT, PRICE, ASSET_CODE, COUNTERPARTY, CON_CODE, OPTION_BACKING, "
					"EXERCISE_PRICE, EXERCISE_DATE, VALUE_DATE, CURRENCY, FXRATE, BROKER, BR_FEES, OPT_EXPIRATION, "
					"DEAL_TYPE, TRADER_INI, CUSTODIAN, OPT_TICK, OPT_ID, EURO_OPT, OPT_AUTO, OPT_SET_CODE, "
					"OPT_SET_CODE2, SET_CONVENTION, DELIVERY_DATE, CP_TRADE_ID, MARGIN, MARGIN_CURRENCY, "
					"MARGIN_AMOUNT, BINARY, VAR, SHORT_SALE, HAN_DETAIL1, HAN_DETAIL2) "
					"SELECT A.TRANS_NUM, A.PORTFOLIO, A.TICKET_NUM, A.TRADE_DATE, A.TRANS_TYPE, A.TRANS_DIRECTION, "
					"A.NOM_AMOUNT - NVL(SUM(B.NOM_AMOUNT),0), A.PRICE, A.ASSET_CODE, A.COUNTERPARTY, A.CON_CODE, "
					"A.OPTION_BACKING, A.EXERCISE_PRICE, A.EXERCISE_DATE, A.VALUE_DATE, A.CURRENCY, A.FXRATE, "
					"A.BROKER, A.BR_FEES, A.OPT_EXPIRATION, A.DEAL_TYPE, A.TRADER_INI, A.CUSTODIAN, A.OPT_TICK, "
					"A.OPT_ID, A.EURO_OPT, A.OPT_AUTO, A.OPT_SET_CODE, A.OPT_SET_CODE2, A.SET_CONVENTION, "
					"A.DELIVERY_DATE, A.CP_TRADE_ID, A.MARGIN, A.MARGIN_CURRENCY, A.MARGIN_AMOUNT, A.BINARY, A.VAR, "
					"A.SHORT_SALE, A.HAN_DETAIL1, A.HAN_DETAIL2 "
					"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_TR_TICKETS B, SEMAM.NW_PORTFOLIOS Z "
					"WHERE B.OPTION_BACKING(+) = A.TRANS_NUM "
					"AND B.TRADE_DATE(+) <= %s "
					"AND Z.PORTFOLIO = A.PORTFOLIO "
					"AND Z.STATUS IS NULL "
					"AND A.OPTION_BACKING IS NULL "
					"AND A.TRANS_TYPE IN ('CALL','PUT') "
					"AND LEAST(A.TRADE_DATE, A.VALUE_DATE) <= %s "
					"AND (A.OPT_EXPIRATION IS NULL OR A.OPT_EXPIRATION > %s) "
					"AND (A.EXERCISE_DATE IS NULL OR A.EXERCISE_DATE > %s) "
					"GROUP BY A.TRANS_NUM, A.PORTFOLIO, A.TICKET_NUM, A.TRADE_DATE, A.TRANS_TYPE, A.TRANS_DIRECTION, "
					"A.NOM_AMOUNT, A.PRICE, A.ASSET_CODE, A.COUNTERPARTY, A.CON_CODE, A.OPTION_BACKING, A.EXERCISE_PRICE, "
					"A.EXERCISE_DATE, A.VALUE_DATE, A.CURRENCY, A.FXRATE, A.BROKER, A.BR_FEES, A.OPT_EXPIRATION, A.DEAL_TYPE, "
					"A.TRADER_INI, A.CUSTODIAN, A.OPT_TICK, A.OPT_ID, A.EURO_OPT, A.OPT_AUTO, A.OPT_SET_CODE, A.OPT_SET_CODE2, "
					"A.SET_CONVENTION, A.DELIVERY_DATE, A.CP_TRADE_ID, A.MARGIN, A.MARGIN_CURRENCY, A.MARGIN_AMOUNT, A.BINARY, "
					"A.VAR, A.SHORT_SALE, A.HAN_DETAIL1, A.HAN_DETAIL2 "
					"HAVING A.NOM_AMOUNT - NVL(SUM(B.NOM_AMOUNT),0) > 0 ", QDate, QDate, QDate, QDate);
	}
	OraLoader.ExecuteSql(Sql);
}
/////////////////////////////////////////////////////////////////////////////
// CDailyInput message handlers
void CDailyInput::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	CQData QData;

	GetDocument()->GetData().LoadDates();
	m_DataArray.SetOraLoader(GetDocument()->GetData().GetOraLoader());
	GetDocument()->GetData().LoadSupportData();
	QData.IsBloombergAvailbe();

	m_Date.Setup(this, IDC_DAILY_INPUT_DATE_EDIT);

	m_Date.SetData(GetDocument()->GetData().GetDate());

	GetDocument()->SetViewTitle("DAILY INPUT");
	m_SS.SetAutoClipboard(TRUE);
}

void CDailyInput::OnDailyInputFxrate() 
{
	m_nEditCol = 3;
	DoInput(TRUE, 2, INPUT_FXRATE, 1); // Exrate must be > 0, 2% Limit
}

void CDailyInput::OnDailyInputOption() 
{
	m_nEditCol = 4;
	DoInput(TRUE, 1, INPUT_OPTION, -1); // Opt Price must be a number, $1
}

void CDailyInput::OnDailyInputOptionvalue()
{
	m_nEditCol = 4;
	DoInput(TRUE, 5.0, INPUT_OPTIONVALUE, -1); // Opt Price must be a number, 5% Limit
}

void CDailyInput::OnDailyInputOptionDelta()
{
	m_nEditCol = 3;
	DoInput(TRUE, 5.0, INPUT_OPTION_DELTA, -1); // Opt Price must be a number, 5% Limit
}

void CDailyInput::OnDailyInputVarFactor() 
{
	m_nEditCol = 3;	
	DoInput(TRUE, 5, INPUT_VAR_FACTOR, 0); // Price must be >= 0, 5% Limit
}

void CDailyInput::OnDailyInputPrice() 
{
	m_nEditCol = 4;	
	DoInput(TRUE, 3, INPUT_PRICE, 0); // Price must be >= 0, 5% Limit
}

void CDailyInput::OnDailyInputCost() 
{
	m_nEditCol = 4;
	DoInput(FALSE, 5, INPUT_COST, 0); // Price must be >= 0, 5% Limit
}

void CDailyInput::OnDailyInputDuration() 
{
	m_nEditCol = 3;
	DoInput(TRUE, 5, INPUT_DURATION, -1);
}

void CDailyInput::OnDailyInputRate() 
{
	m_nEditCol = 4;
	DoInput(TRUE, 1, INPUT_RATE, -1);
}

void CDailyInput::OnDailyInputVol() 
{
	m_nEditCol = 4;
	DoInput(TRUE, 2, INPUT_VOL, -1);
}

void CDailyInput::OnDailyInputAccrualfactor()
{
	m_nEditCol = 3;	
	DoInput(TRUE, -1, INPUT_ACCR_FACT, -1);
}

void CDailyInput::OnDailyInputRepo() 
{
	CFloatRepo RepoDlg;
	
	DoSave();
	RepoDlg.m_pData = &GetDocument()->GetData();
	RepoDlg.DoModal();
}

void CDailyInput::OnDailyInputNav()
{
	CNavChange Dlg;

	Dlg.m_OraLoader.Copy(GetDocument()->GetData().GetOraLoader());
	Dlg.DoModal();
}

void CDailyInput::OnDailyInputSwap() 
{
	m_nEditCol = 4;
	DoInput(TRUE, 5, INPUT_SWAP, -1); 
}

void CDailyInput::OnDailyInputDv01()
{
	m_nEditCol = 4;
	DoInput(TRUE, 5, INPUT_DV01, -1); 
}

BEGIN_EVENTSINK_MAP(CDailyInput, CFormView)
    //{{AFX_EVENTSINK_MAP(CDailyInput)
	ON_EVENT(CDailyInput, IDC_DAILY_INPUT_LIST, 2 /* BlockSelected */, OnBlockSelectedInputList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CDailyInput, IDC_DAILY_INPUT_LIST, 11 /* DblClick */, OnDblClickInputList, VTS_I4 VTS_I4)
	ON_EVENT(CDailyInput, IDC_DAILY_INPUT_LIST, 15 /* EditMode */, OnEditModeInputList, VTS_I4 VTS_I4 VTS_I2 VTS_BOOL)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CDailyInput::OnBlockSelectedInputList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickInputList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
}

void CDailyInput::OnDblClickInputList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
		m_SS.SetSheetCurRow(Row);
	else
		m_SS.SetSheetCurRow(0);
}

void CDailyInput::OnEditModeInputList(long Col, long Row, short Mode, BOOL ChangeMade) 
{
	if(!ChangeMade)
		return;

	CPriceData *PriceData;
	CQData QData;

	PriceData = m_DataArray.GetAt(Row - 1);
	if(!PriceData)
		return;

	CString Text;
	Text = m_SS.GetSheetText(m_nEditCol, Row);
	if(!QData.IsValid(Text, "-,.0123456789"))
		Text.Empty();
	PriceData->SetPrice(Text, m_DataArray.GetDataRange());
	PriceData->SetModify(TRUE);
	m_DataArray.SetModify();

	SetSheetRow(Row, PriceData, TRUE);
	SetSheetSize();
}

void CDailyInput::OnDailyInputDelete() 
{
	if(m_SS.GetSheetCurRow() <= 0)
		return;

	CPriceData *PriceData;
	
	PriceData = m_DataArray.GetAt(m_SS.GetSheetCurRow() - 1);
	if(!PriceData)
		return;
	
	PriceData->SetPrice(EMPTYSTRING, m_DataArray.GetDataRange());
	PriceData->SetModify(TRUE);
	PriceData->SetDeleted(TRUE);
	m_DataArray.SetModify();

	SetSheetRow(m_SS.GetSheetCurRow(), PriceData, TRUE);
	SetSheetSize();
	m_SS.SetSheetCurRow(0);
}

void CDailyInput::OnUpdateDailyInputDelete(CCmdUI* pCmdUI) 
{
	if(GetDocument()->GetData().IsGuest())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(m_SS.GetIsBlockSelected() && m_SS.GetSheetCurRow() > 0 && 
						m_MenuID == INPUT_COST ? TRUE : FALSE);
}

void CDailyInput::OnDailyInputFind() 
{
	DoSave();	
	UpdateData(FALSE);
}

void CDailyInput::OnUpdateDailyInputFind(CCmdUI* pCmdUI) 
{
	CString Date;
	BOOL Enable = FALSE;

	Date = m_Date.GetData();
	if(m_MenuID != 0 && !Date.IsEmpty() && Date != m_DataArray.GetDate())
		Enable = TRUE;
	
	pCmdUI->Enable(Enable);
}

void CDailyInput::OnDailyInputSave() 
{
	UpdateData();
}

void CDailyInput::OnUpdateDailyInputSave(CCmdUI* pCmdUI) 
{
	if(GetDocument()->GetData().IsGuest())
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(m_DataArray.GetModify());
}

void CDailyInput::OnDailyUpdateFxrate() 
{
	DoUpdate("Fxrate", INPUT_FXRATE, 1);
}

void CDailyInput::OnDailyUpdteIssuerAmount()
{
	DoUpdate("Issuer Amount", INPUT_ISSUER_AMOUNT, 1);
}

void CDailyInput::OnDailyUpdateSwap() 
{
	DoUpdate("Interest Swap", INPUT_SWAP, -1);
}

void CDailyInput::OnDailyUpdateOption() 
{
	m_DataArray.SetOptionValue(FALSE);
	DoUpdate("Option", INPUT_OPTION, -1);
}

void CDailyInput::OnDailyUploadOptionvalue()
{
	DoUpdate("Option Value", INPUT_OPTIONVALUE, -1);
}

void CDailyInput::OnDailyUpdateOptionDelta()
{
	DoUpdate("Option Delta", INPUT_OPTION_DELTA, -1);
}

void CDailyInput::OnDailyUpdateVarFactor() 
{
	DoUpdate("Var Factor", INPUT_VAR_FACTOR, 0);
}

void CDailyInput::OnDailyUpdatePrice() 
{
	DoUpdate("Price", INPUT_PRICE, 0);
}

void CDailyInput::OnDailyUpdateDuration() 
{
	DoUpdate("Duration", INPUT_DURATION, 0);
}

void CDailyInput::OnDailyUpdateRate() 
{
	DoUpdate("RiskFreeRate", INPUT_RATE, 0);
}

void CDailyInput::OnDailyUpdateVol() 
{
	DoUpdate("OptionVol", INPUT_RATE, 0);
}

void CDailyInput::OnDailyUpdateAccrualfactor()
{
	DoUpdate("AccrualFactor", INPUT_ACCR_FACT, 0);
}

void CDailyInput::OnDailyUpdateDv01()
{
	DoUpdate("DV01", INPUT_DV01, -1);
}

void CDailyInput::OnDailyUpdateAssetrate()
{
	DoUpdate("AssetRate", INPUT_ASSETRATE, 0);
}

void CDailyInput::OnDailyInputCopy()
{
	m_SS.SetBlockMode(TRUE);
	m_SS.SetRow(m_SS.GetActiveRow());
	m_SS.SetRow2(m_SS.GetSelBlockRow2());
	m_SS.SetBlockMode(FALSE);
	m_SS.ClipboardCopy();
}

void CDailyInput::OnDailyInputPaste()
{
	CString Text;
	int Rows;
	CPriceData *PriceData;
	CQData QData;

	m_SS.ClipboardPaste();
	Rows = m_SS.GetRow2() - m_SS.GetRow();
	for(int i = m_SS.GetActiveRow(); i <= m_SS.GetActiveRow() + Rows; i ++)
	{
		Text = m_SS.GetSheetText(m_SS.GetActiveCol(), i);
		Text = QData.RemoveComma(Text);
		PriceData = m_DataArray.GetAt(i - 1);
		if(PriceData)
		{
			if(!QData.IsValid(Text, "-,.0123456789"))
				Text.Empty();
			PriceData->SetPrice(Text, m_DataArray.GetDataRange());
			PriceData->SetModify(TRUE);
			SetSheetRow(i, PriceData, TRUE);
		}
	}

	m_DataArray.SetModify();
	SetSheetSize();
}

void CDailyInput::OnDailyUpdateNav() 
{
	CString Date, Path, Today, Msg, Portfolio, Category, Nav;
	CString Sql, Text;
	LPCTSTR Template = "Template";
	LPCTSTR TXT = "TXT", pDate;
	char Buffer[2000];
	char *p, *q;
	int i, Count;
	COraLoader OraLoader;
	CStdioFile InFile;
	CQData QData;
	CDBRec Rec;
	CFileException e;

	Path = "Template Files (*.TXT) || ";
	// Get the user name and host from the private profile	
	Text = "DAILY NAV";
	QData.GetFileName(AfxGetApp(), Path, Template, Text, TXT);
	if(Path.IsEmpty())
    	return;

	OraLoader.Copy(GetDocument()->GetData().GetOraLoader());
	OraLoader.Today(Today);
	Date = m_Date.GetData();
	if(Date != Today)
	{
		Msg = "You are about to update for " + Date + ".  Are sure you want to continue? ";
		if(MessageBox(Msg, Today, MB_YESNO) == IDNO)
			return;
	}

	if(GetDocument()->GetData().InputDate(Date) != IDOK)
		return;

	if(!InFile.Open(Path, CFile::modeRead, &e))
	{
		e.GetErrorMessage(Buffer, sizeof(Buffer) - 1);
		MessageBox(Buffer, "File could not be opened ", MB_OK);
		return;
	}

	Sql = "SELECT COUNT(*) FROM SEMAM.NW_DAILY_CATEGORY_NAV "
			"WHERE INDATE = %s AND PORTFOLIO = '%s' AND CATEGORY = '%s' AND ACTUAL IS NOT NULL ";
	pDate = QData.GetQueryDate(Date);
	InFile.ReadString(Buffer, sizeof(Buffer));
	InFile.ReadString(Buffer, sizeof(Buffer));
	while(InFile.ReadString(Buffer, sizeof(Buffer)))
	{
		p = strstr(Buffer, "\n");
		if(p)
			p[0] = 0;
		p = strstr(Buffer, "\r");
		if(p)
			p[0] = 0;
		p = strtok_s(Buffer, TAB, &q);
		Rec.RemoveAll();
		Rec.Add(Date);
		i = 0;
		while(p)
		{
			switch(i)
			{
				case 0:
					Portfolio = p;
					if(!Portfolio.IsEmpty())
						Rec.Add(p);
					break;
				case 1:
					Category = p;
					if(!Category.IsEmpty())
						Rec.Add(p);
					break;
				case 2:
					Nav = p;
					Nav.Remove(',');
					if(!Category.IsEmpty() && atof(Nav) != 0)
						Rec.Add(Nav);
					break;
				default:
					break;
			}

			i++;
			p = strtok_s(NULL, TAB, &q);
		}

		if(Rec.GetSize() == 4)
		{
			OraLoader.GetSql().Format((LPCTSTR ) Sql, pDate, (LPCTSTR ) Portfolio, 
									(LPCTSTR ) Category);
			OraLoader.Open();
			OraLoader.LoadTextString(Text);
			Count = atoi(Text);
			if(Count > 0)
				OraLoader.GetSql().Format("SELECT INDATE, PORTFOLIO, CATEGORY, ACTUAL "
								"FROM SEMAM.NW_DAILY_CATEGORY_NAV "
								"WHERE INDATE = %s AND PORTFOLIO = '%s' AND CATEGORY = '%s' ", 
								pDate, (LPCTSTR) Portfolio, (LPCTSTR) Category);
			else
				OraLoader.GetSql().Format("SELECT INDATE, PORTFOLIO, CATEGORY, ACTUAL "
										"FROM SEMAM.NW_DAILY_CATEGORY_NAV ");
			OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT);
			if(Count > 0)
				OraLoader.UpdateRecord(Rec);
			else
				OraLoader.UpdateRecord(Rec, TRUE);
		}
	}
}

void CDailyInput::OnDailyInputRepoCoupon() 
{
//	CRepoCouponDlg RCDlg;
	
//	DoSave();
//	RCDlg.m_pData = &GetDocument()->GetData();
//	RCDlg.DoModal();
}

void CDailyInput::OnDailyInputBloombergId() 
{
	CBlmID Dlg;

	Dlg.m_OraLoader = GetDocument()->GetData().GetOraLoader();
	Dlg.DoModal();
}



void CDailyInput::OnUpdateDailyUpdateDuration(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetDocument()->GetData().IsGuest());
}

void CDailyInput::OnUpdateDailyUpdateFxrate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetDocument()->GetData().IsGuest());
}

void CDailyInput::OnUpdateDailyUpdateNav(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetDocument()->GetData().IsGuest());
}

void CDailyInput::OnUpdateDailyUpdateOption(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetDocument()->GetData().IsGuest());
}

void CDailyInput::OnUpdateDailyUpdateOptionDelta(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetDocument()->GetData().IsGuest());
}

void CDailyInput::OnUpdateDailyUpdatePrice(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetDocument()->GetData().IsGuest());
}

void CDailyInput::OnUpdateDailyUpdateRate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetDocument()->GetData().IsGuest());
}

void CDailyInput::OnUpdateDailyUpdateSwap(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetDocument()->GetData().IsGuest());
}

void CDailyInput::OnUpdateDailyUpdateVarFactor(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetDocument()->GetData().IsGuest());
}

void CDailyInput::OnUpdateDailyUpdateVol(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetDocument()->GetData().IsGuest());
}

void CDailyInput::OnUpdateDailyInputRepo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetDocument()->GetData().IsGuest());
}

void CDailyInput::OnUpdateDailyInputRepoCoupon(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetDocument()->GetData().IsGuest());
}

void CDailyInput::OnUpdateDailyInputBloombergId(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetDocument()->GetData().IsGuest());
}

void CDailyInput::OnUpdateDailyInputNav(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!GetDocument()->GetData().IsGuest());
}

void CDailyInput::OnUpdateDailyUpdateAccrualfactor(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!GetDocument()->GetData().IsGuest());
}

void CDailyInput::OnUpdateDailyInputAccrualfactor(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!GetDocument()->GetData().IsGuest());
}

void CDailyInput::OnUpdateDailyInputDv01(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!GetDocument()->GetData().IsGuest());
}

void CDailyInput::OnUpdateDailyUpdateDv01(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!GetDocument()->GetData().IsGuest());
}

void CDailyInput::OnUpdateDailyUpdateIssuerAmount(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!GetDocument()->GetData().IsGuest());
}

void CDailyInput::OnDailyQuickfxrate()
{
	CBlmFxrate BlmFxrate;
	int Count;

	if(BLOOMBERG_AVAILABLE)
		Count = BlmFxrate.GetOraLoader().GetCount("SELECT COUNT(*) FROM SEMAM.NW_EXRATES WHERE INDATE = '" + BlmFxrate.GetDate() + "' ");

	if(Count > 0)
	{
		if(MessageBox("Exrates have been loaded. Do you want to over write them ?", "Quick Exrate", MB_YESNO) == IDYES)
			BlmFxrate.GetOraLoader().ExecuteSql("DELETE FROM SEMAM.NW_EXRATES WHERE INDATE = '" + BlmFxrate.GetDate() + "' ");
		else
			return;
	}

	BeginWaitCursor();
	BlmFxrate.LoadCurrencys();
	BlmFxrate.LoadLiveExrates();
	EndWaitCursor();
}

void CDailyInput::OnDailyQuickprice()
{
	CBlmPrice BlmPrice;
	BOOL bOverRite = FALSE;
	int Count;

	Count = BlmPrice.GetOraLoader().GetCount("SELECT COUNT(*) FROM SEMAM.NW_EXRATES WHERE INDATE = '" + BlmPrice.GetDate() + "' ");

	if(Count <= 0)
	{
		MessageBox("Please load Exrates first!", "Quick Price", MB_OK);
		return;
	}

	Count = BlmPrice.GetOraLoader().GetCount("SELECT COUNT(*) FROM SEMAM.NW_MARKET_PRICES WHERE PR_DATE = '" + BlmPrice.GetDate() + "' ");
	if(Count > 0)
	{
		if(MessageBox("Prices have been loaded. Do you want to over write them ?", "Quick Price", MB_YESNO) == IDYES)
			bOverRite = TRUE;
		else
			return;
	}

	BeginWaitCursor();
	if(bOverRite)
		BlmPrice.GetOraLoader().ExecuteSql("DELETE FROM SEMAM.NW_MARKET_PRICES WHERE PR_DATE = '" + BlmPrice.GetDate() + "' ");
	BlmPrice.LoadLivePrices(BLM_LOAD_SECURITIES);
	
	EndWaitCursor();
}

void CDailyInput::OnDailyQuickdividend()
{
	CBlmDividend BlmDividend;

	BeginWaitCursor();
	BlmDividend.LoadDividend();
	EndWaitCursor();
}


void CDailyInput::OnUpdateDailyQuickfxrate(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(BLOOMBERG_AVAILABLE);
}


void CDailyInput::OnUpdateDailyQuickprice(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(BLOOMBERG_AVAILABLE);
}


void CDailyInput::OnUpdateDailyQuickdividend(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(BLOOMBERG_AVAILABLE);
}


void CDailyInput::OnDailyQuickoptprice()
{
	CBlmPrice BlmPrice;
	BOOL bOverRite = FALSE;
	int Count;

	Count = BlmPrice.GetOraLoader().GetCount("SELECT COUNT(*) FROM SEMAM.NW_EXRATES WHERE INDATE = '" + BlmPrice.GetDate() + "' ");

	if(Count <= 0)
	{
		MessageBox("Please load Exrates first!", "Quick Price", MB_OK);
		return;
	}

	Count = BlmPrice.GetOraLoader().GetCount("SELECT COUNT(*) FROM SEMAM.NW_OPT_PRICES WHERE MM_DATE = '" + BlmPrice.GetDate() + "' ");
	if(Count > 0)
	{
		if(MessageBox("Option Prices have been loaded. Do you want to over write them ?", "Quick Option Price", MB_YESNO) == IDYES)
			bOverRite = TRUE;
		else
			return;
	}

	BeginWaitCursor();

	if(bOverRite)
		BlmPrice.GetOraLoader().ExecuteSql("DELETE FROM SEMAM.NW_OPT_PRICES WHERE MM_DATE = '" + BlmPrice.GetDate() + "' ");
	BlmPrice.LoadLivePrices(BLM_LOAD_OPTION);
	
	EndWaitCursor();
}


void CDailyInput::OnDailyQuickswapprice()
{
	CBlmPrice BlmPrice;
	BOOL bOverRite = FALSE;
	int Count;

	Count = BlmPrice.GetOraLoader().GetCount("SELECT COUNT(*) FROM SEMAM.NW_EXRATES WHERE INDATE = '" + BlmPrice.GetDate() + "' ");

	if(Count <= 0)
	{
		MessageBox("Please load Exrates first!", "Quick Price", MB_OK);
		return;
	}

	Count = BlmPrice.GetOraLoader().GetCount("SELECT COUNT(*) FROM SEMAM.NW_DEAL_NAV WHERE INDATE = '" + BlmPrice.GetDate() + "' ");
	if(Count > 0)
	{
		if(MessageBox("IRS Prices have been loaded. Do you want to over write them ?", "Quick Swap Price", MB_YESNO) == IDYES)
			bOverRite = TRUE;
		else
			return;
	}

	BeginWaitCursor();

	if(bOverRite)
		BlmPrice.GetOraLoader().ExecuteSql("DELETE FROM SEMAM.NW_DEAL_NAV WHERE INDATE = '" + BlmPrice.GetDate() + "' ");
	BlmPrice.LoadLivePrices(BLM_LOAD_IRS);
	
	EndWaitCursor();
}

void CDailyInput::OnUpdateDailyQuickoptprice(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(BLOOMBERG_AVAILABLE);
}

void CDailyInput::OnUpdateDailyQuickswapprice(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(BLOOMBERG_AVAILABLE);
}



