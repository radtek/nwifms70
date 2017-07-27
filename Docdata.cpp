#include "stdafx.h"
#include "resource.h"
#include "docdata.h"
#include "datedlg.h"
#include "nwivar.h"
#include "equity.h"

IMPLEMENT_DYNAMIC(CDocData, CDataObj)
CDocData::CDocData(COraLoader &OraLoader) : CDataObj(OraLoader)
{
	m_Date.Empty();
	m_bPastDue = TRUE;
	m_bBlock = FALSE;
	m_bBlotter = FALSE;
	m_bRisk = FALSE;

	GetSecFee() = 0;
	GetOptFee() = 0;
}

CDocData::~CDocData()
{
	m_TransTypeArr.RemoveAll();
	m_TransDirArr.RemoveAll();
	m_CurrencyArr.RemoveAll();
	m_CountryArr.RemoveAll();
	m_ProfileCountryArr.RemoveAll();
	m_IndustryArr.RemoveAll();
	m_AssetTypeArr.RemoveAll();
	m_AssetProfileTypeArr.RemoveAll();
	m_AccountArr.RemoveAll();
	m_BlmKeyArr.RemoveAll();
	m_BucketArr.RemoveAll();
	m_CategoryArr.RemoveAll();
	m_CDSFeeTypeArr.RemoveAll();
	m_RateBasisArr.RemoveAll();
	m_TraderArr.RemoveAll();
	m_PFUArr.RemoveAll();
	m_CashTypeArr.RemoveAll();
	m_PmntFrqArr.RemoveAll();
	m_IssuerArr.RemoveAll();
	m_ActionArr.RemoveAll();
	m_RateTypeArr.RemoveAll();
	m_MethodArr.RemoveAll();
	m_IndexArr.RemoveAll();
	m_LevelArr.RemoveAll();
	m_OptSetArr.RemoveAll();
	m_FxSetConvArr.RemoveAll();
	m_CTArr.RemoveAll();
	m_FutureTypeArr.RemoveAll();
	m_TrDescArr.RemoveAll();
	m_ContactList.RemoveAll();
	m_BestExecutionArr.RemoveAll();
	m_PFClassArr.RemoveAll();
	m_PFSubClassArr.RemoveAll();
	m_ContactList.RemoveAll();
	m_YieldCurveArr.RemoveAll();
}

void CDocData::LoadAssetData(BOOL Refresh)
{
	LoadDBRecArray(m_CountryArr, "SELECT COUNTRY FROM SEMAM.NW_COUNTRIES WHERE SOV_FLAG = 'Y' ORDER BY 1", Refresh);
	LoadDBRecArray(m_ProfileCountryArr, "SELECT COUNTRY FROM SEMAM.NW_COUNTRIES ORDER BY 1", Refresh);
	LoadDBRecArray(m_CDSFeeTypeArr, "SELECT FEE_TYPE, FEE_DESC FROM SEMAM.NW_DF_FEE_TYPE ORDER BY 1", Refresh);
	LoadDBRecArray(m_IndustryArr, "SELECT IND_AREAS FROM SEMAM.NW_IND_AREAS ORDER BY 1", Refresh);
	LoadDBRecArray(m_BucketArr, "SELECT BUCKET, BUCKET_DESC FROM SEMAM.NW_BUCKETS ORDER BY BUCKET_INDEX", Refresh);
	LoadDBRecArray(m_AssetTypeArr, "SELECT IND_CODE FROM SEMAM.NW_INDUSTRIES WHERE TYPE_FLAG = 'Y' ORDER BY 1", Refresh);
	LoadDBRecArray(m_AssetProfileTypeArr, "SELECT IND_CODE FROM SEMAM.NW_INDUSTRIES ORDER BY 1", Refresh);
	LoadDBRecArray(m_FutureTypeArr, "SELECT FUTURE_TYPE FROM SEMAM.NW_FUTURE_TYPE ORDER BY 1 ", Refresh);
	LoadDBRecArray(m_PmntFrqArr, "SELECT PAYMENT_FREQ, FREQ_DESC FROM SEMAM.NW_PAYMENT_FREQ ORDER BY 1", Refresh);
	LoadDBRecArray(m_IssuerArr, "SELECT ISSUER FROM SEMAM.NW_ISSUERS ORDER BY 1", Refresh);
	LoadDBRecArray(m_ActionArr, "SELECT ACTION FROM SEMAM.NW_ACTIONS ORDER BY 1", Refresh);
	LoadDBRecArray(m_RateTypeArr, "SELECT ASS_INT_TYPE FROM SEMAM.NW_INT_TYPES ORDER BY 1", Refresh);
	LoadDBRecArray(m_MethodArr, "SELECT METHOD FROM SEMAM.NW_ASSET_METHODS ORDER BY 1", Refresh);
	LoadDBRecArray(m_IndexArr, "SELECT REF_IDX, REF_IDX_DESC, CICO_ID, YC_NAME FROM SEMAM.NW_REF_IDX ORDER BY 1 ", Refresh);
	LoadDBRecArray(m_LevelArr, "SELECT ASSET_LEVEL, LEVEL_DESC FROM SEMAM.NW_ASSET_LEVEL ORDER BY 1", Refresh);
	LoadDBRecArray(m_PFClassArr, "SELECT PF_CLASS FROM SEMAM.NW_PF_CLASS ORDER BY 1", Refresh);
	LoadDBRecArray(m_PFSubClassArr, "SELECT PF_SUBCLASS FROM SEMAM.NW_PF_SUBCLASS ORDER BY 1", Refresh);
	LoadDBRecArray(m_BlmKeyArr, "SELECT BLM_KEY FROM SEMAM.NW_BLM_KEY ORDER BY 1", Refresh);
	LoadDBRecArray(m_YieldCurveArr, "SELECT YC_NAME, YC_SYMBOL, YC_INDEX, YC_TERM, YC_SOURCE, YC_DEFAULT "
					"FROM SEMAM.NW_YIELD_CURVE ORDER BY 1 ", Refresh);
}

void CDocData::LoadCashData(BOOL Refresh)
{
	LoadDBRecArray(m_CashTypeArr, "SELECT PMNT_TYPE FROM SEMAM.NW_PMNT_TYPES ORDER BY 1", Refresh);
}

void CDocData::LoadDates()
{
	GetOraLoader().Today(m_Date);
	GetOraLoader().TPlus3(m_Plus3Date, m_Date);
	GetOraLoader().TPlus2(m_Plus2Date, m_Date);
	GetOraLoader().TPlus1(m_Plus1Date, m_Date);
}

void CDocData::LoadSupportData(BOOL Refresh)
{
	LoadDBRecArray(m_PortfolioArr, "SELECT PORTFOLIO, DEF_CUSTODIAN, DEF_ACCOUNT "
					"FROM SEMAM.NW_PORTFOLIOS WHERE STATUS IS NULL ORDER BY 1", Refresh);
	LoadDBRecArray(m_TraderArr, "SELECT TRADER_INI, TRADER FROM SEMAM.NW_TRADERS "
							"WHERE ACTIVE = 'Y' ORDER BY 1", Refresh);
	LoadDBRecArray(m_TransDirArr, "SELECT DIRECTION, DIR_DESCRIPTION FROM SEMAM.NW_DIRECTION WHERE DIRECTION IN ('P', 'S') ORDER BY 1", Refresh);
	LoadDBRecArray(m_CurrencyArr, "SELECT CURRENCY FROM SEMAM.NW_CURRENCY ORDER BY 1", Refresh);

	if(Refresh)
		m_ContactList.RemoveAll();

	if(m_ContactList.GetSize() <= 0)
	{
		GetOraLoader().Open("SELECT A.CP_CODE, CT_CODE, "
							"CT_CODE || ' -- '||CT_NAME ||' -- '||CT_CONTACT \"CP_CT\"  "
							"FROM SEMAM.NW_COUNTER_CONTACTS A "
							"JOIN SEMAM.NW_COUNTER_PARTY B ON (A.CP_CODE = B.CP_CODE) "
							"WHERE B.ISVALID = 'Y' AND A.ISVALID = 'Y' "
							"ORDER BY 1, 2, 3 ");
		GetOraLoader().LoadKeyDBList(m_ContactList);
	}

	LoadDBRecArray(m_CTArr, "SELECT A.CP_CODE, CT_CODE, CT_NAME, CT_CONTACT, CT_MSTC_CODE, "
					"A.CP_CODE||CT_CODE "
					"FROM SEMAM.NW_COUNTER_CONTACTS A, SEMAM.NW_COUNTER_PARTY B "
					"WHERE B.CP_CODE = A.CP_CODE AND B.ISVALID = 'Y' "
					"AND A.ISVALID = 'Y' ORDER BY 1, 2, 3", Refresh);

	LoadDBRecArray(m_IndexArr, "SELECT REF_IDX, REF_IDX_DESC FROM SEMAM.NW_REF_IDX ORDER BY 1 ", Refresh);
	LoadDBRecArray(m_AccountArr, "SELECT ACC_CODE FROM SEMAM.NW_CASH_ACCOUNTS WHERE ACTIVE = 'Y' ORDER BY 1", Refresh);
	LoadDBRecArray(m_OptSetArr, "SELECT OPT_SET_CODE, OPT_SET_ZONE, OPT_SET_TIME FROM SEMAM.NW_OPT_SETTLEMENT ORDER BY 2, 3", Refresh);
	LoadDBRecArray(m_TransTypeArr, "SELECT TRANS_TYPE FROM LIN.NW_TRANS_TYPES ORDER BY 1", Refresh);
	LoadDBRecArray(m_TrDescArr, "SELECT TR_DESC FROM SEMAM.NW_TR_DESC ORDER BY 1 ", Refresh);
	LoadDBRecArray(m_CategoryArr, "SELECT STRATEGY FROM SEMAM.NW_INV_STRATEGIES WHERE TRANS_BUCKET IS NULL ORDER BY CAT_INDEX", Refresh);
	LoadDBRecArray(m_RateBasisArr, "SELECT RATE_BASIS FROM SEMAM.NW_RATE_BASES ORDER BY 1", Refresh);
	LoadDBRecArray(m_PFUArr, "SELECT PFU FROM SEMAM.NW_PFU ORDER BY 1", Refresh);
	LoadDBRecArray(m_BestExecutionArr, "SELECT BEST_EXECUTION FROM SEMAM.NW_BEST_EXECUTION ORDER BY 1", Refresh);
	LoadDBRecArray(m_ETradeTypeArr, "SELECT ETRADE_TYPE, ETRADE_DESC FROM SEMAM.NW_ETRADE_TYPE ORDER BY 2", Refresh);
}

void CDocData::LoadUser(BOOL Refresh)
{
	const int Trader = 1;
	const int Supervisor = 2;
	const int Operation = 4;
	const int PowerUser = 8;
	const int Guest = 16;

	if(Refresh)
		m_User.Empty();

	if(m_User.IsEmpty())
	{
		if(GetOraLoader().Open("SELECT USER_NAME, TRADER, RISK_MAN, DECODE(SUPERVISOR, 'Y', 3, 1) \"STATUS\", "
							"TRADER_INI "
							"FROM SEMAM.NW_TRADERS WHERE USER_NAME = USER AND ACTIVE = 'Y' "
							"UNION "
							"SELECT USER_NAME, OPS_NAME, NULL, "
							"DECODE(GUEST, 'Y', 16, DECODE(POWERUSER, 'Y', 12, 4)), NULL "
							"FROM SEMAM.NW_OPERATIONS "
							"WHERE USER_NAME = USER AND ACTIVE = 'Y' ORDER BY 1, 2, 3 "))
		{	
			OValue Value;
			m_nUserStatus = 0;
			GetOraLoader().MoveFirst();
			while(!GetOraLoader().IsEOF())
			{
				SetOraUser(GetOraLoader().GetDBString(0));
				SetUser(GetOraLoader().GetDBString(1));
				GetOraLoader().GetFieldValue(2, &Value);
				if(Value.IsNull())
					m_bRiskManager = FALSE;
				else
					m_bRiskManager = (int) Value;
				GetOraLoader().GetFieldValue(3, &Value);
				m_nUserStatus |= (int) Value;
				GetOraLoader().GetFieldValue(4, &Value);
				if(!Value.IsNull())
					SetTraderIni((LPCTSTR) Value);
				GetOraLoader().MoveNext();
			}
		}
	}

	if(m_User.IsEmpty())
	{	
		MessageBox(NULL, _T("UnAuthorized User"), NULL, MB_OK);
		PostQuitMessage(0);
	}

	SetTrader(GetUserType(Trader));
	SetOperation(GetUserType(Operation));
	SetPowerUser(GetUserType(PowerUser));
	SetSupervisor(GetUserType(Supervisor));
	SetGuest(GetUserType(Guest));
}

BOOL CDocData::InputDate(CString &Date)
{
	CDateDlg DateDlg;
	BOOL bOk = FALSE;

	if(Date.IsEmpty())
		Date = GetDate();

	DateDlg.m_Data = Date;

	if(DateDlg.DoModal() == IDOK)
	{
		Date = DateDlg.m_Data;
		bOk = TRUE;
	}

	return bOk;
}

LPCTSTR CDocData::DefineTransType(CString &TransType, LPCTSTR Dir)
{
	CEquity Eq;

	TransType = Eq.DefineTransType(TransType, Dir);
	return (LPCTSTR) TransType;
}

int CDocData::GetUnSignTicketCount()
{
	GetOraLoader().GetSql() = "SELECT COUNT(*) FROM SEMAM.RAW_TICKET_V A, SEMAM.NW_TRADERS B "
			"WHERE B.TRADER_INI = A.TRADER_INI "
			"AND B.USER_NAME = USER "
			"AND A.SIGN IS NOT NULL "
			"AND A.TRADER_SIGN IS NULL "
			"AND A.BOOK_DATE < TRUNC(SYSDATE) ";

	return GetOraLoader().GetCount();
}

void CDocData::LoadFees()
{
	OValue Value;

	GetOraLoader().Open("SELECT SEC_FEE/1000000 FROM SEMAM.NW_SEC_FEE "
						"WHERE FROM_DATE <= TRUNC(SYSDATE) "
						"AND nvl(TO_DATE, SYSDATE) > TRUNC(SYSDATE) ");
	while(!GetOraLoader().IsEOF())
	{
		GetOraLoader().GetFieldValue(0, &Value);
		m_SecFee = (double) Value;
		GetOraLoader().MoveNext();
	}
	
/*	GetOraLoader().Open("SELECT OR_FEE FROM SEMAM.NW_OR_FEE "
						"WHERE FROM_DATE <= TRUNC(SYSDATE) "
						"AND NVL(TO_DATE, SYSDATE) > TRUNC(SYSDATE) ");
	while(!GetOraLoader().IsEOF())
	{
		GetOraLoader().GetFieldValue(0, &Value);
		m_OptFee = (double) Value;
		GetOraLoader().MoveNext();
	} */
}

void CDocData::LoadOrFeeArray(COrFeeDataArray &OrFeeArray)
{
	OrFeeArray.LoadData(GetOraLoader());
}

void CDocData::PasswordExpiryWarning()
{
	OValue Value;
	int nPassDays;
	CString Text;

	nPassDays = 1000;

	GetOraLoader().Open("SELECT EXPIRY_DATE - TRUNC(SYSDATE) FROM USER_USERS "
						  "WHERE USERNAME = USER ");
	
	while(!GetOraLoader().IsEOF())
	{
		GetOraLoader().GetFieldValue(0, &Value);
		if(Value.IsNull())
			nPassDays = 100;
		else
			nPassDays = (int) Value;
		GetOraLoader().MoveNext();
	}

	Text.Empty();
	
	if(nPassDays >= 0 && nPassDays <= 5)
		Text.Format("Your password will expire in %d Days.  Please change your password!", nPassDays);
	else
		if(nPassDays < 0)
			Text.Format("Your password expired.  You are in %d Grace Days.  Please change your password now!!!", abs(nPassDays));
	
	if(!Text.IsEmpty())
		MessageBox(NULL, Text, "Password Warning", MB_OK);
}