#include "stdafx.h"
#include "resource.h"
#include "rawticketrec.h"
#include "nwivar.h"
#include "qdata.h"



IMPLEMENT_DYNAMIC(CRawInvRec, CRecObj)
void CRawInvRec::Copy(CRawInvRec &RawInvRec)
{
	CRecObj::Copy(RawInvRec);

	SetTicket(RawInvRec.GetTicket());
	SetPortfolio(RawInvRec.GetPortfolio());
	SetNomAmount(RawInvRec.GetNomAmount());
	SetCustodian(RawInvRec.GetCustodian());
	SetAccount(RawInvRec.GetAccount());
	SetPrice(RawInvRec.GetPrice());
	SetLink(RawInvRec.GetLink());
	SetDownPymnt(RawInvRec.GetDownPymnt());
//	SetAssignCT(RawInvRec.GetAssignCT());
}

void CRawInvRec::ToDBRec(CDBRec &Rec)
{
	char buf[100], *p;

	Rec.RemoveAll();
	
	strcpy_s(buf, sizeof(buf), GetTicket());
	p = strstr(buf, PERIOD);
	if(p)
		p[0] = 0;
	Rec.Add(buf);
	Rec.Add(GetPortfolio());
	Rec.Add(GetNomAmount());
	Rec.Add(GetCustodian());
	Rec.Add(GetAccount());
	Rec.Add(GetPrice());
	Rec.Add(GetLink());
	Rec.Add(GetDownPymnt());
}

BOOL CRawInvRec::AddRec(COraLoader &OraLoader)
{
	if(GetTicket().IsEmpty())
		return FALSE;
	
/*	if(!OraLoader.Open("SELECT TICKET_NUM, PORTFOLIO, NOM_AMOUNT, PRICE, DOWN_PYMNT, BROKER_FEE, "
						"SOFTDOLLAR, OTHER_FEE, VAR, DV01, OPT_BACK, ASSIGN_CP, CUSTODIAN, ACCOUNT "
						"FROM SEMAM.NW_RAW_TICKET_DETAIL ", ODYNASET_DEFAULT))
		return FALSE; */
	if(!OraLoader.Open("SELECT TICKET_NUM, PORTFOLIO, NOM_AMOUNT, CUSTODIAN, ACCOUNT, PRICE, "
						"OPT_BACK, DOWN_PYMNT FROM SEMAM.NW_RAW_TICKET_DETAIL ", ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CRawInvRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(strlen(GetTicket()) == 0 || strlen(GetID()) == 0)
		return FALSE;
	
	CQData QData;
//	OraLoader.GetSql().Format("SELECT TICKET_NUM, PORTFOLIO, NOM_AMOUNT, PRICE, DOWN_PYMNT, BROKER_FEE, "
//								"SOFTDOLLAR, OTHER_FEE, VAR, DV01, OPT_BACK, ASSIGN_CP, CUSTODIAN, ACCOUNT "
//								"FROM SEMAM.NW_RAW_TICKET_DETAIL WHERE ROWIDTOCHAR(ROWID) = %s ", 
//								QData.GetQueryText(GetID())); // Rowid 
	OraLoader.GetSql().Format("SELECT TICKET_NUM, PORTFOLIO, NOM_AMOUNT, CUSTODIAN, ACCOUNT, "
								"PRICE, OPT_BACK, DOWN_PYMNT FROM SEMAM.NW_RAW_TICKET_DETAIL "
								"WHERE ROWIDTOCHAR(ROWID) = %s ", QData.GetQueryText(GetID())); // Rowid 
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;
	
	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec);
}

BOOL CRawInvRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetTicket().IsEmpty())
		return FALSE;

	CQData QData;
	if(bByID)
		OraLoader.GetSql().Format("DELETE FROM SEMAM.NW_RAW_TICKET_DETAIL WHERE ROWIDTOCHAR(ROWID) = %s ", 
								QData.GetQueryText(GetID()));
	else
		OraLoader.GetSql().Format("DELETE FROM SEMAM.NW_RAW_TICKET_DETAIL WHERE TICKET_NUM = %s ", 
								QData.GetQueryNumber(GetTicket()));
	return OraLoader.ExecuteSql(); 
}

BOOL CRawInvRec::UpdateCustodian(COraLoader &OraLoader)
{
	if(GetTicket().IsEmpty())
		return FALSE;

	CQData QData;
	CString Custodian, Account;

	Custodian = QData.GetQueryText(GetCustodian());
	Account = QData.GetQueryText(GetAccount());
	OraLoader.GetSql().Format("UPDATE SEMAM.NW_RAW_TICKET_DETAIL SET CUSTODIAN = %s, "
								"ACCOUNT = %s WHERE TICKET_NUM = %s ", (LPCTSTR) Custodian, 
								(LPCTSTR) Account, QData.GetQueryNumber(GetTicket()));
	return OraLoader.ExecuteSql();
}



IMPLEMENT_DYNAMIC(CRawTicketRec, CRecObj)
void CRawTicketRec::SetAvailAmount(LPCTSTR AvailAmount)
{
	CQData QData;

	m_AvailAmount = atof(QData.RemoveComma(AvailAmount));
}

void CRawTicketRec::Copy(CRawTicketRec &RawTicketRec)
{
	CRecObj::Copy(RawTicketRec);

	SetTicket(RawTicketRec.GetTicket());
	SetTrader(RawTicketRec.GetTrader());
	SetDefTrader(RawTicketRec.GetDefTrader());
	SetTradeDate(RawTicketRec.GetTradeDate());
	SetExecTime(RawTicketRec.GetExecTime());
	SetValueDate(RawTicketRec.GetValueDate());
	SetDir(RawTicketRec.GetDir());
	SetTransType(RawTicketRec.GetTransType());
	SetNomAmount(RawTicketRec.GetNomAmount());
	SetOriNominal(RawTicketRec.GetOriNominal());
	SetStrike(RawTicketRec.GetStrike());
	SetOptExp(RawTicketRec.GetOptExp());
	SetFXDate(RawTicketRec.GetFXDate());
	SetOptTicker(RawTicketRec.GetOptTicker());
	SetOptSetCode(RawTicketRec.GetOptSetCode());
	SetOptSetCode2(RawTicketRec.GetOptSetCode2());
	SetSetConvention(RawTicketRec.GetSetConvention());
	SetListed(RawTicketRec.GetListed());
	SetEuroOpt(RawTicketRec.GetEuroOpt());
	SetOptAuto(RawTicketRec.GetOptAuto());
	SetCurrency(RawTicketRec.GetCurrency());
	SetFxRate(RawTicketRec.GetFxRate());
	SetCP(RawTicketRec.GetCP());
	SetAsset(RawTicketRec.GetAsset());
	SetAssetDesc(RawTicketRec.GetAssetDesc());
	SetAssetCommon(RawTicketRec.GetAssetCommon());
	SetAssetCusip(RawTicketRec.GetAssetCusip());
	SetAssetIsin(RawTicketRec.GetAssetIsin());
	SetAssetSedol(RawTicketRec.GetAssetSedol());
	SetAssetCountry(RawTicketRec.GetAssetCountry());
	SetAssetCurr(RawTicketRec.GetAssetCurr());
	SetAssetClass(RawTicketRec.GetAssetClass());
	SetAssetCategory(RawTicketRec.GetAssetCategory());
	SetAssetBucket(RawTicketRec.GetAssetBucket());
	SetAssetIndustry(RawTicketRec.GetAssetIndustry());
	SetAssetCoupon(RawTicketRec.GetAssetCoupon());
	SetAssetMaturity(RawTicketRec.GetAssetMaturity());
	SetAssetSource(RawTicketRec.GetAssetSource());
	SetAssetImagineCode(RawTicketRec.GetAssetImagineCode());
	SetIPO(RawTicketRec.GetIPO());
	SetTRS(RawTicketRec.GetTRS());
	SetSoftDollar(RawTicketRec.GetSoftDollar());
	SetRepoCP(RawTicketRec.GetRepoCP());
	SetRate(RawTicketRec.GetRate());
	SetRateBasis(RawTicketRec.GetRateBasis());
	SetMaturity(RawTicketRec.GetMaturity());
	SetNote(RawTicketRec.GetNote());
	SetNote2(RawTicketRec.GetNote2());
    SetProcessed(RawTicketRec.GetProcessed());
	SetSWBooking(RawTicketRec.GetSWBooking());
	SetSWMaturity(RawTicketRec.GetSWMaturity());
	SetSWFloatRate(RawTicketRec.GetSWFloatRate());
	SetSWFloatBasis(RawTicketRec.GetSWFloatBasis());
	SetSWFloatMaturity(RawTicketRec.GetSWFloatMaturity());
	SetWI(RawTicketRec.GetWI());
	SetSwapTicket(RawTicketRec.GetSwapTicket());
	SetDeliveryDate(RawTicketRec.GetDeliveryDate());
	SetMargin(RawTicketRec.GetMargin());
	SetMarginCurrency(RawTicketRec.GetMarginCurrency());
	SetMarginAmount(RawTicketRec.GetMarginAmount());
	SetFunded(RawTicketRec.GetFunded());
	SetBinary(RawTicketRec.GetBinary());
	SetAAMethod(RawTicketRec.GetAAMethod());
	SetAAFReason(RawTicketRec.GetAAFReason());
	SetBestExecution(RawTicketRec.GetBestExecution());
	SetShortSale(RawTicketRec.GetShortSale());
	SetCSPBShort(RawTicketRec.GetCSPBShort());
	SetPrice(RawTicketRec.GetPrice());
	SetDownPymnt(RawTicketRec.GetDownPymnt());
	SetBrokerFee(RawTicketRec.GetBrokerFee());
	SetOtherFee(RawTicketRec.GetOtherFee());
	SetSoftDollar(RawTicketRec.GetSoftDollar());
	SetAssignCP(RawTicketRec.GetAssignCP());
	SetDV01(RawTicketRec.GetDV01());
	SetVAR(RawTicketRec.GetVAR());
	SetUnWindTicket(RawTicketRec.GetUnWindTicket());
	SetCancelTicket(RawTicketRec.GetCancelTicket());
	SetCorrectTicket(RawTicketRec.GetCorrectTicket());
	SetSecFee(RawTicketRec.GetSecFee());
	SetOrFee(RawTicketRec.GetOrFee());
	SetETrade(RawTicketRec.GetETrade());
	SetImagineExecID(RawTicketRec.GetImagineExecID());
	SetCPTradeID(RawTicketRec.GetCPTradeID());
	SetOrderID(RawTicketRec.GetOrderID());

	m_RepoCT = RawTicketRec.GetRepoCT();
	m_RepoType = RawTicketRec.GetRepoType();

		
	SetNetPrice(RawTicketRec.GetNetPrice());
}

void CRawTicketRec::FromAssetRec(CAssetRec &AssetRec)
{
	SetAsset(AssetRec.GetAsset());
	SetAssetDesc(AssetRec.GetDesc());
	SetAssetCurr(AssetRec.GetCurrency());
	if(GetTransType() == CALL || GetTransType() == PUT || 
		GetTransType() == SPRSWCLL || GetTransType() == SPRSWPUT)
		SetOptTicker(AssetRec.GetOptTicker());
	else
		SetOptTicker(EMPTYSTRING);

	SetAssetCountry(AssetRec.GetCountry());
	SetAssetCommon(AssetRec.GetCommon());
	SetAssetCusip(AssetRec.GetCusip());
	SetAssetIsin(AssetRec.GetIsin());
	SetAssetSedol(AssetRec.GetSedol());
	SetAssetClass(AssetRec.GetClass());
	SetAssetIndustry(AssetRec.GetIndustry());
	SetAssetCategory(AssetRec.GetCategory());
	SetAssetBucket(AssetRec.GetBucket());
	SetAssetSource(AssetRec.GetSource());
	SetAssetCoupon(AssetRec.GetCoupon());
	SetAssetMaturity(AssetRec.GetMaturity());
}

void CRawTicketRec::ToAssetRec(CAssetRec &AssetRec)
{
	AssetRec.SetAsset(GetAsset());
	AssetRec.SetDesc(GetAssetDesc());
	AssetRec.SetCurrency(GetAssetCurr());
	AssetRec.SetOptTicker(GetOptTicker());
	AssetRec.SetCountry(GetAssetCountry());
	AssetRec.SetCommon(GetAssetCommon());
	AssetRec.SetCusip(GetAssetCusip());
	AssetRec.SetIsin(GetAssetIsin());
	AssetRec.SetSedol(GetAssetSedol());
	AssetRec.SetClass(GetAssetClass());
	AssetRec.SetIndustry(GetAssetIndustry());
	AssetRec.SetCategory(GetAssetCategory());
	AssetRec.SetBucket(GetAssetBucket());
	AssetRec.SetSource(GetAssetSource());
	AssetRec.SetCoupon(GetAssetCoupon());
	AssetRec.SetMaturity(GetAssetMaturity());
}

void CRawTicketRec::ToDBRec(CDBRec &Rec)
{
	char buf[100], *p;
	CString Maturity;

	Rec.RemoveAll();

	strcpy_s(buf, 100, GetTicket());
	p = strstr(buf, PERIOD);
	if(p)
		p[0] = 0;
	Rec.Add(buf);
	if(GetTrader().IsEmpty())
		Rec.Add(GetDefTrader());
	else
		Rec.Add(GetTrader());
	Rec.Add(GetTradeDate());
	Rec.Add(GetExecTime());
	Rec.Add(GetTransType());
	Rec.Add(GetDir());
	Rec.Add(GetValueDate());
	Rec.Add(GetNomAmount());
	if(GetTransType() == CALL || GetTransType() == PUT || 
		GetTransType() == SPRSWCLL || GetTransType() == SPRSWPUT)
	{
		Rec.Add(GetStrike());
		Rec.Add(GetOptExp());
		Rec.Add(GetFXDate());
		Rec.Add(GetListed());
		Rec.Add(GetOptTicker());
		Rec.Add(GetOptID());
		Rec.Add(GetEuroOpt());
		Rec.Add(GetOptAuto());
	}
	else
	{
		Rec.Add(EMPTYSTRING);
		Rec.Add(EMPTYSTRING);
		Rec.Add(EMPTYSTRING);
		Rec.Add(EMPTYSTRING);
		Rec.Add(EMPTYSTRING);
		Rec.Add(EMPTYSTRING);
		Rec.Add(EMPTYSTRING);
		Rec.Add(EMPTYSTRING);
	}

	Rec.Add(GetOptSetCode());
	Rec.Add(GetOptSetCode2());
	Rec.Add(GetSetConvention());
	Rec.Add(GetCP());
	Rec.Add(GetCurrency());
	Rec.Add(GetFxRate());
	Rec.Add(GetAsset());
	Rec.Add(GetAssetDesc());
	Rec.Add(GetAssetCurr());
	Rec.Add(GetAssetCountry());
	Rec.Add(GetAssetCommon());
	Rec.Add(GetAssetCusip());
	Rec.Add(GetAssetIsin());
	Rec.Add(GetAssetSedol());
	Rec.Add(GetAssetClass());
	Rec.Add(GetAssetIndustry());
	Rec.Add(GetAssetCategory());
	Rec.Add(GetAssetBucket());
	Rec.Add(GetAssetSource());
	Rec.Add(GetAssetCoupon());
	Rec.Add(GetAssetMaturity());
	Rec.Add(GetIPO());
	Rec.Add(GetTRS());
	Rec.Add(GetRepoCP());
	Rec.Add(GetRate());
	Rec.Add(GetRateBasis());
	if(GetTransType() == INTSWAP) // Make sure maturity are the same
	{
		Maturity = GetMaturity().IsEmpty() ? GetSWFloatMaturity() : GetMaturity();
		Rec.Add(Maturity);
	}
	else
		Rec.Add(GetMaturity());

	Rec.Add(GetNote());
	Rec.Add(GetNote2());
	Rec.Add(GetSWBooking());
	Rec.Add(GetSWMaturity());
	Rec.Add(GetSWFloatRate());
	Rec.Add(GetSWFloatBasis());
	if(GetTransType() == INTSWAP)
		Rec.Add(Maturity);
	else
		Rec.Add(EMPTYSTRING);

	Rec.Add(GetWI());
	Rec.Add(GetSwapTicket());
	Rec.Add(GetDeliveryDate());
	Rec.Add(GetMargin());
	Rec.Add(GetMarginCurrency());
	Rec.Add(GetMarginAmount());
	Rec.Add(GetFunded());
	Rec.Add(GetBinary());
	Rec.Add(GetAAMethod());
	Rec.Add(GetAAFReason());
	Rec.Add(GetBestExecution());
	Rec.Add(GetShortSale());
	Rec.Add(GetCSPBShort());
	Rec.Add(GetPrice());
	Rec.Add(GetDownPymnt());
	Rec.Add(GetBrokerFee());
	Rec.Add(GetOtherFee());
	Rec.Add(GetSoftDollar());
	Rec.Add(GetAssignCP());
	Rec.Add(GetDV01());
	Rec.Add(GetVAR());
	Rec.Add(GetUnWindTicket());
	Rec.Add(GetCancelTicket());
	Rec.Add(GetCorrectTicket());
	Rec.Add(GetSecFee());
	Rec.Add(GetOrFee());
	Rec.Add(GetETrade());
	Rec.Add(GetOrderID());
	Rec.Add(GetProcessed()); 
}

void CRawTicketRec::ToShortDBRec(CDBRec &Rec, BOOL bFloat)
{
	char buf[100], *p;

	Rec.RemoveAll();

	strcpy_s(buf, 100, GetTicket());
	p = strstr(buf, PERIOD);
	if(p)
		p[0] = 0;
	Rec.Add(buf);
	Rec.Add(GetTrader());
	Rec.Add(GetTradeDate());
	Rec.Add(GetExecTime());
	Rec.Add(GetTransType());
	Rec.Add(GetDir());
	Rec.Add(GetValueDate());
	Rec.Add(GetNomAmount());
	if(GetTransType() == CALL || GetTransType() == PUT || 
		GetTransType() == SPRSWCLL || GetTransType() == SPRSWPUT)
	{
		Rec.Add(GetStrike());
		Rec.Add(GetOptExp());
		Rec.Add(GetFXDate());
		Rec.Add(GetListed());
		Rec.Add(GetOptTicker());
		Rec.Add(GetOptID());
		Rec.Add(GetEuroOpt());
		Rec.Add(GetOptAuto());
	}
	else
	{
		Rec.Add(EMPTYSTRING);
		Rec.Add(EMPTYSTRING);
		Rec.Add(EMPTYSTRING);
		Rec.Add(EMPTYSTRING);
		Rec.Add(EMPTYSTRING);
		Rec.Add(EMPTYSTRING);
		Rec.Add(EMPTYSTRING);
		Rec.Add(EMPTYSTRING);
	}
	Rec.Add(GetOptSetCode());
	Rec.Add(GetOptSetCode2());
	Rec.Add(GetSetConvention());
	Rec.Add(GetCP());
	Rec.Add(GetCurrency());
	Rec.Add(GetFxRate());
	Rec.Add(GetAsset());
	Rec.Add(GetAssetDesc());
	Rec.Add(GetAssetCurr());
	Rec.Add(GetAssetCountry());
	Rec.Add(GetAssetCommon());
	Rec.Add(GetAssetCusip());
	Rec.Add(GetAssetIsin());
	Rec.Add(GetAssetSedol());
	Rec.Add(GetAssetClass());
	Rec.Add(GetAssetIndustry());
	Rec.Add(GetAssetCategory());
	Rec.Add(GetAssetBucket());
	Rec.Add(GetAssetSource());
	Rec.Add(GetAssetCoupon());
	Rec.Add(GetAssetMaturity());
	Rec.Add(GetIPO());
	Rec.Add(GetTRS());
	Rec.Add(GetRepoCP());
	if(!bFloat)
	{
		Rec.Add(GetRate());
		Rec.Add(GetRateBasis());
		Rec.Add(GetMaturity());
	}
	else
	{
		Rec.Add(GetSWFloatRate());
		Rec.Add(GetSWFloatBasis());
		Rec.Add(GetSWFloatMaturity());
	}

	Rec.Add(GetNote());
	Rec.Add(GetNote2());
	Rec.Add(GetSWBooking());
	Rec.Add(GetSWMaturity());
	Rec.Add(GetWI());
	Rec.Add(GetSwapTicket());
	Rec.Add(GetDeliveryDate());
	Rec.Add(GetMargin());
	Rec.Add(GetMarginCurrency());
	Rec.Add(GetMarginAmount());
	Rec.Add(GetFunded());
	Rec.Add(GetBinary());
	Rec.Add(GetAAMethod());
	Rec.Add(GetAAFReason());
	Rec.Add(GetBestExecution());
	Rec.Add(GetShortSale());
	Rec.Add(GetCSPBShort());
	Rec.Add(GetPrice());
	Rec.Add(GetDownPymnt());
	Rec.Add(GetBrokerFee());
	Rec.Add(GetOtherFee());
	Rec.Add(GetSoftDollar());
	Rec.Add(GetAssignCP());
	Rec.Add(GetDV01());
	Rec.Add(GetVAR());
	Rec.Add(GetUnWindTicket());
	Rec.Add(GetCancelTicket());
	Rec.Add(GetCorrectTicket());
	Rec.Add(GetSecFee());
	Rec.Add(GetOrFee());
	Rec.Add(GetETrade());
	Rec.Add(GetOrderID());
	Rec.Add(GetProcessed());
}

BOOL CRawTicketRec::AddRec(COraLoader &OraLoader)
{
	if(GetTicket().IsEmpty())
		return FALSE;

	if(!OraLoader.Open("SELECT TICKET_NUM, TRADER_INI, TRADE_DATE, EXEC_TIME, TRANS_TYPE, DIR, VALUE_DATE, "
						"NOM_AMOUNT, STRIKE, OPT_EXPIRATION, FX_DATE, OTC, OPT_TICK, ASSET_PID, EURO_OPT, "
						"OPT_AUTO, OPT_SET_CODE, OPT_SET_CODE2, SET_CONVENTION, COUNTERPARTY, CURRENCY, "
						"FXRATE, ASSET_CODE, ASSET_DESC, ASSET_CURRENCY, ASSET_COUNTRY, ASSET_COMMON_CODE, "
						"ASSET_MSTC_CODE, ASSET_ISIN_CODE, ASSET_SEDOL_NUM, ASSET_CLASS, ASSET_IND_AREA, "
						"ASSET_CATEGORY, ASSET_BUCKET, SOURCE, ASSET_COUPON, ASSET_MATURITY, IPO, TRS, "
						"REPO_CP, TR_RATE, RATE_BASIS, MATURITY, NOTE, NOTE2, SW_BOOKING, SW_MATURITY, "
						"FLOAT_RATE, FLOAT_RATE_BASIS, FLOAT_MATURITY, WI, SWAP_TICKET, DELIVERY_DATE, "
						"MARGIN, MARGIN_CURRENCY, MARGIN_AMOUNT, FUNDED_SWAP, BINARY, AA_METHOD, AA_FREASON, "
						"BEST_EXECUTION, SHORT_SALE, CSPB_SHORT, PRICE, DOWN_PYMNT, BROKER_FEE, OTHER_FEE, "
						"SOFTDOLLAR, ASSIGN_CP, DV01, VAR, UNWIND_TICKET, CANCEL_TICKET, CORRECT_TICKET, "
						"SEC_FEE, OR_FEE, ETRADE, ORDER_ID, PROCESSED "
						"FROM SEMAM.NW_RAW_TICKETS ", ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;

	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CRawTicketRec::UpdateShortRec(COraLoader &OraLoader, BOOL bFloat)
{
	if(GetTicket().IsEmpty())
		return FALSE;

	CQData QData;
	if(GetTransType() == INTSWAP && bFloat)
		OraLoader.GetSql().Format("SELECT TICKET_NUM, TRADER_INI, TRADE_DATE, EXEC_TIME, TRANS_TYPE, DIR, "
									"VALUE_DATE, NOM_AMOUNT, STRIKE, OPT_EXPIRATION, FX_DATE, OTC, OPT_TICK, "
									"ASSET_PID, EURO_OPT, OPT_AUTO, OPT_SET_CODE, OPT_SET_CODE2, SET_CONVENTION, "
									"COUNTERPARTY, CURRENCY, FXRATE, ASSET_CODE, ASSET_DESC, ASSET_CURRENCY, "
									"ASSET_COUNTRY, ASSET_COMMON_CODE, ASSET_MSTC_CODE, ASSET_ISIN_CODE, "
									"ASSET_SEDOL_NUM, ASSET_CLASS, ASSET_IND_AREA, ASSET_CATEGORY, ASSET_BUCKET, "
									"SOURCE, ASSET_COUPON, ASSET_MATURITY, IPO, TRS, REPO_CP, TR_RATE, RATE_BASIS, "
									"MATURITY, NOTE, NOTE2, SW_BOOKING, SW_MATURITY, WI, SWAP_TICKET, DELIVERY_DATE, "
									"MARGIN, MARGIN_CURRENCY, MARGIN_AMOUNT, FUNDED_SWAP, BINARY, AA_METHOD, "
									"AA_FREASON, BEST_EXECUTION, SHORT_SALE, CSPB_SHORT, PRICE, DOWN_PYMNT, "
									"BROKER_FEE, OTHER_FEE, SOFTDOLLAR, ASSIGN_CP, DV01, VAR, UNWIND_TICKET, "
									"CANCEL_TICKET, CORRECT_TICKET, SEC_FEE, OR_FEE, ETRADE, ORDER_ID, PROCESSED "
									"FROM SEMAM.NW_RAW_TICKETS "
									"WHERE TICKET_NUM = %s", QData.GetQueryNumber(GetTicket()));
	else
		OraLoader.GetSql().Format("SELECT TICKET_NUM, TRADER_INI, TRADE_DATE, EXEC_TIME, TRANS_TYPE, DIR, "
									"VALUE_DATE, NOM_AMOUNT, STRIKE, OPT_EXPIRATION, FX_DATE, OTC, OPT_TICK, "
									"ASSET_PID, EURO_OPT, OPT_AUTO, OPT_SET_CODE, OPT_SET_CODE2, SET_CONVENTION, "
									"COUNTERPARTY, CURRENCY, FXRATE, ASSET_CODE, ASSET_DESC, ASSET_CURRENCY, "
									"ASSET_COUNTRY, ASSET_COMMON_CODE, ASSET_MSTC_CODE, ASSET_ISIN_CODE, "
									"ASSET_SEDOL_NUM, ASSET_CLASS, ASSET_IND_AREA, ASSET_CATEGORY, ASSET_BUCKET, "
									"SOURCE, ASSET_COUPON, ASSET_MATURITY, IPO, TRS, REPO_CP, FLOAT_RATE, "
									"FLOAT_RATE_BASIS, FLOAT_MATURITY, NOTE, NOTE2, SW_BOOKING, SW_MATURITY, WI, "
									"SWAP_TICKET, DELIVERY_DATE, MARGIN, MARGIN_CURRENCY, MARGIN_AMOUNT, FUNDED_SWAP, "
									"BINARY, AA_METHOD, AA_FREASON, BEST_EXECUTION, SHORT_SALE, CSPB_SHORT, PRICE, "
									"DOWN_PYMNT, BROKER_FEE, OTHER_FEE, SOFTDOLLAR, ASSIGN_CP, DV01, VAR, UNWIND_TICKET, "
									"CANCEL_TICKET, CORRECT_TICKET, SEC_FEE, OR_FEE, ETRADE, ORDER_ID, PROCESSED "
									"FROM SEMAM.NW_RAW_TICKETS WHERE TICKET_NUM = %s ", 
									QData.GetQueryNumber(GetTicket()));
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToShortDBRec(Rec);
	return OraLoader.UpdateRecord(Rec);
}

BOOL CRawTicketRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetTicket().IsEmpty())
		return FALSE;
	
	CQData QData;
	OraLoader.GetSql().Format("SELECT TICKET_NUM, TRADER_INI, TRADE_DATE, EXEC_TIME, TRANS_TYPE, DIR, "
								"VALUE_DATE, NOM_AMOUNT, STRIKE, OPT_EXPIRATION, FX_DATE, OTC, OPT_TICK, "
								"ASSET_PID, EURO_OPT, OPT_AUTO, OPT_SET_CODE, OPT_SET_CODE2, SET_CONVENTION, "
								"COUNTERPARTY, CURRENCY, FXRATE, ASSET_CODE, ASSET_DESC, ASSET_CURRENCY, "
								"ASSET_COUNTRY, ASSET_COMMON_CODE, ASSET_MSTC_CODE, ASSET_ISIN_CODE, "
								"ASSET_SEDOL_NUM, ASSET_CLASS, ASSET_IND_AREA, ASSET_CATEGORY, ASSET_BUCKET, "
								"SOURCE, ASSET_COUPON, ASSET_MATURITY, IPO, TRS, REPO_CP, TR_RATE, RATE_BASIS, "
								"MATURITY, NOTE, NOTE2, SW_BOOKING, SW_MATURITY, FLOAT_RATE, FLOAT_RATE_BASIS, "
								"FLOAT_MATURITY, WI, SWAP_TICKET, DELIVERY_DATE, MARGIN, MARGIN_CURRENCY, "
								"MARGIN_AMOUNT, FUNDED_SWAP, BINARY, AA_METHOD, AA_FREASON, BEST_EXECUTION, "
								"SHORT_SALE, CSPB_SHORT, PRICE, DOWN_PYMNT, BROKER_FEE, OTHER_FEE, SOFTDOLLAR, "
								"ASSIGN_CP, DV01, VAR, UNWIND_TICKET, CANCEL_TICKET, CORRECT_TICKET, SEC_FEE, "
								"OR_FEE, ETRADE, ORDER_ID, PROCESSED "
								"FROM SEMAM.NW_RAW_TICKETS WHERE TICKET_NUM = %s", 
								QData.GetQueryNumber(GetTicket()));

	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec);
}

BOOL CRawTicketRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetTicket().IsEmpty())
		return FALSE;
	
	CRawInvRec RawInv;
	LPCTSTR pTicket;

	RawInv.SetTicket(GetTicket());
	if(!RawInv.DeleteRec(OraLoader))
		return FALSE;

	CQData QData;
	pTicket = QData.GetQueryNumber(GetTicket());
	OraLoader.GetSql().Format("DELETE SEMAM.NW_RAW_COMMISSION WHERE TICKET_NUM = %s ", pTicket);
	OraLoader.ExecuteSql();
	OraLoader.GetSql().Format("DELETE SEMAM.NW_RAW_TICKETS WHERE TICKET_NUM = %s ", pTicket);

	return OraLoader.ExecuteSql();
}

BOOL CRawTicketRec::DeleteRawRecOnly(COraLoader &OraLoader, BOOL bByID)
{
	if(GetTicket().IsEmpty())
		return FALSE;
	
	CQData QData;
	OraLoader.GetSql().Format("DELETE SEMAM.NW_RAW_TICKETS WHERE TICKET_NUM = %s ", 
								QData.GetQueryNumber(GetTicket()));
	return OraLoader.ExecuteSql();
}

BOOL CRawTicketRec::SignTicket(COraLoader &OraLoader, BOOL bSign)
{
	if(GetTicket().IsEmpty())
		return FALSE;

	CQData QData;
	CString Signiture, TicketNum;

	if(bSign)
		OraLoader.GetSql().Format("UPDATE SEMAM.NW_RAW_TICKETS SET TRADER_SIGN = '%s', "
								"TRADER_SIGN_DATE = SYSDATE WHERE TICKET_NUM = %s ", 
								(LPCTSTR) GetSign(), QData.GetQueryNumber(GetTicket()));	
	else
	{
		TicketNum = QData.GetQueryNumber(GetTicket());
		OraLoader.GetSql().Format("SELECT SIGN FROM SEMAM.NW_RAW_TICKETS "
									"WHERE TICKET_NUM = %s ", (LPCTSTR) TicketNum);
		OraLoader.Open();
		OraLoader.LoadTextString(Signiture);
		if(!Signiture.IsEmpty() && Signiture != GetSign())
		{
			MessageBox(NULL, "This ticket has been approved by " + Signiture, "Ticket Approval", MB_OK);
			return FALSE;
		}

		OraLoader.GetSql().Format("UPDATE SEMAM.NW_RAW_TICKETS SET SIGN = '%s', "
								"SIGN_DATE = SYSDATE WHERE TICKET_NUM = %s ", 
								(LPCTSTR) GetSign(), TicketNum);
	}
	return OraLoader.ExecuteSql();
}

BOOL CRawTicketRec::IsTicketProcessed(COraLoader &OraLoader)
{
	if(GetTicket().IsEmpty())
		return FALSE;

	CQData QData;
	OraLoader.GetSql().Format("SELECT COUNT(*) FROM SEMAM.NW_RAW_TICKETS "
								"WHERE PROCESSED IS NOT NULL AND TICKET_NUM = %s ",
								QData.GetQueryNumber(GetTicket()));
	
	return OraLoader.GetCount(OraLoader.GetSql()) > 0 ? TRUE : FALSE;
}

const CString &CRawTicketRec::Alloc(CString &Text, const CString Amt, const CString AllocatedNom)
{
	double T_dNom, dNom, dAmt;
	CQData QData;

	Text.Empty();
	T_dNom = atof(QData.RemoveComma(GetNomAmount()));
	dNom = atof(QData.RemoveComma(AllocatedNom));
	dAmt = atof(QData.RemoveComma(Amt));
	
	if(T_dNom != 0)
		Text.Format("%.2lf", dAmt*dNom/T_dNom);
	else
		Text.Empty();

	return Text;
}