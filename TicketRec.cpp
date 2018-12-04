#include "stdafx.h"
#include "resource.h"
#include "ticketrec.h"
#include "nwivar.h"
#include "sqlstmt.h"
#include "qdata.h"

#define TICKETREC_ADD 1
#define TICKETREC_UPDATE 2
#define TICKETREC_DELETE 3


IMPLEMENT_DYNAMIC(CInvRec, CRecObj)
void CInvRec::Copy(CInvRec &InvRec)
{
	SetID(InvRec.GetID());
	SetTransNum(InvRec.GetTransNum());
	SetAsset(InvRec.GetAsset());
	SetNomAmount(InvRec.GetNomAmount());
	SetCloseDate(InvRec.GetCloseDate());
	SetValueDate(InvRec.GetValueDate());
	SetInterest(InvRec.GetInterest());
	SetCash(InvRec.GetCash());
	SetCurrency(InvRec.GetCurrency());
	SetTrDesc(InvRec.GetTrDesc());
	SetAccount(InvRec.GetAccount());
	SetPostDate(InvRec.GetPostDate());
	SetForexRef(InvRec.GetForexRef());
	SetCrossRate(InvRec.GetCrossRate());
	SetAssignCP(InvRec.GetAssignCP());
	SetAssignCT(InvRec.GetAssignCT());
}

void CInvRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();
	Rec.Add(GetTransNum());
	Rec.Add(GetAsset());
	Rec.Add(GetTrDesc());
	Rec.Add(GetNomAmount());
	Rec.Add(GetValueDate());
	Rec.Add(GetCloseDate());
	Rec.Add(GetPostDate());
	Rec.Add(GetCurrency());
	Rec.Add(GetForexRef());
	Rec.Add(GetCrossRate());
	Rec.Add(GetCash());
	Rec.Add(GetInterest());
	Rec.Add(GetAccount());
	Rec.Add(GetAssignCP());
	Rec.Add(GetAssignCT());
}

BOOL CInvRec::AddRec(COraLoader &OraLoader)
{
	if(GetTransNum().IsEmpty())
		return FALSE;
	
	if(!OraLoader.Open("SELECT TRANS_NUM, ASSET_CODE, TR_DESC, NOM_AMOUNT, ACTUAL_VDATE, "
						"ACTUAL_CLOSING, POST_DATE, CURRENCY,  FOREX_REF_NUM, CROSS_RATE, "
						"CASH_AMOUNT, INT_AMOUNT, ACC_CODE, ASSIGN_CP, ASSIGN_CT "
						"FROM SEMAM.NW_TR_INV ", ODYNASET_DEFAULT))
		return FALSE;
	
	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CInvRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetTransNum().IsEmpty() || strlen(GetID()) == 0)
		return FALSE;
	
	CQData QData;

	OraLoader.GetSql().Format("SELECT TRANS_NUM, ASSET_CODE, TR_DESC, NOM_AMOUNT, ACTUAL_VDATE, "
								"ACTUAL_CLOSING, POST_DATE, CURRENCY, FOREX_REF_NUM, CROSS_RATE, "
								"CASH_AMOUNT, INT_AMOUNT, ACC_CODE, ASSIGN_CP, ASSIGN_CT "
								"FROM SEMAM.NW_TR_INV WHERE INV_NUM = %s ", 
								QData.GetQueryNumber(GetID())); // Rowid 
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;
	
	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec);
}

BOOL CInvRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetTransNum().IsEmpty())
		return FALSE;

	CQData QData;

	if(bByID)
		OraLoader.GetSql().Format("DELETE FROM SEMAM.NW_TR_INV WHERE INV_NUM = %s ", QData.GetQueryNumber(GetID()));
	else
		OraLoader.GetSql().Format("DELETE FROM SEMAM.NW_TR_INV WHERE TRANS_NUM = %s ", QData.GetQueryNumber(GetTransNum()));
	return OraLoader.ExecuteSql();
}

BOOL CInvRec::Closing(COraLoader &OraLoader, LPCTSTR CloseDate, LPCTSTR PostDate, BOOL bClose)
{
	if(strlen(GetID()) == 0)
		return FALSE;
	
	CQData QData;
	LPCTSTR p;
	CString TransType, strCloseDate, strPostDate;

	strCloseDate = QData.GetQueryDate(CloseDate);
	strPostDate = QData.GetQueryDate(PostDate);
	p = QData.GetQueryNumber(GetID());
	if(bClose)
		OraLoader.GetSql().Format("UPDATE SEMAM.NW_TR_INV A SET (ACTUAL_CLOSING, CURRENCY, "
								"CASH_AMOUNT, INT_AMOUNT, POST_DATE, USER_STAMP, DATE_STAMP) "
								"= (SELECT %s, B.PAY_CUR, B.AMOUNT, B.INTEREST, "
								"%s, USER, SYSDATE FROM SEMAM.NW_OPEN_TRADE_2 B "
								"WHERE B.INV_NUM = A.INV_NUM) "
								"WHERE INV_NUM = %s ", (LPCTSTR) strCloseDate, (LPCTSTR) strPostDate, p);
	else
	{
		OraLoader.GetSql().Format("SELECT TRANS_TYPE FROM SEMAM.ALL_TICKET_INV_V "
								"WHERE INV_NUM = %s ", p);
		OraLoader.Open();
		OraLoader.LoadTextString(TransType);

		if(TransType.IsEmpty())
			return FALSE;

		if(TransType == INTSWAP)
			OraLoader.GetSql().Format("UPDATE SEMAM.NW_TR_INV A SET(ACTUAL_CLOSING, "
								"POST_DATE , USER_STAMP, DATE_STAMP) = (SELECT TO_DATE(NULL), "
								"TO_DATE(NULL), USER, SYSDATE FROM DUAL) WHERE INV_NUM = %s ", p);
		else
			OraLoader.GetSql().Format("UPDATE SEMAM.NW_TR_INV A SET(ACTUAL_CLOSING, CURRENCY, "
								"CASH_AMOUNT, INT_AMOUNT, POST_DATE , USER_STAMP, DATE_STAMP) = "
								"(SELECT TO_DATE(NULL), NULL, TO_NUMBER(NULL), TO_NUMBER(NULL), "
								"TO_DATE(NULL), USER, SYSDATE FROM DUAL) WHERE INV_NUM = %s ", p);
	}

	return OraLoader.ExecuteSql();
}



IMPLEMENT_DYNAMIC(CTicketRec, CRecObj)
void CTicketRec::SetVersion()
{
	GetVersion().Format("%d", atoi(GetVersion()) + 1);
}

void CTicketRec::Copy(CTicketRec &TicketRec)
{
	SetTransNum(TicketRec.GetTransNum());
    SetPortfolio(TicketRec.GetPortfolio());
	SetTicket(TicketRec.GetTicket());
	SetCP(TicketRec.GetCP());
	SetAsset(TicketRec.GetAsset());
	SetTrader(TicketRec.GetTrader());
	SetTradeDate(TicketRec.GetTradeDate());
	SetExecTime(TicketRec.GetExecTime());
	SetValueDate(TicketRec.GetValueDate());
	SetTransType(TicketRec.GetTransType());
	SetDir(TicketRec.GetDir());
	SetAmount(TicketRec.GetAmount());
	SetPrice(TicketRec.GetPrice());
	SetCurrency(TicketRec.GetCurrency());
	SetFxRate(TicketRec.GetFxRate());
	SetCT(TicketRec.GetCT());
	SetTRS(TicketRec.GetTRS());
	SetStrike(TicketRec.GetStrike());
	SetExerciseDate(TicketRec.GetExerciseDate());
	SetOptExp(TicketRec.GetOptExp());
	SetFXDate(TicketRec.GetFXDate());
	SetOptTicker(TicketRec.GetOptTicker());
	SetOptSet(TicketRec.GetOptSet());
	SetOptSet2(TicketRec.GetOptSet2());
	SetSetConvention(TicketRec.GetSetConvention());
	SetOptID(TicketRec.GetOptID());
	SetEuroOpt(TicketRec.GetEuroOpt());
	SetOptAuto(TicketRec.GetOptAuto());
	SetBroker(TicketRec.GetBroker());
	SetBrokerFee(TicketRec.GetBrokerFee());
	SetSoftDollar(TicketRec.GetSoftDollar());
	SetOtherFee(TicketRec.GetOtherFee());
	SetVAR(TicketRec.GetVAR());
	SetRate(TicketRec.GetRate());
	SetRateBasis(TicketRec.GetRateBasis());
	SetMaturity(TicketRec.GetMaturity());
	SetIndex(TicketRec.GetIndex());
	SetRepoFormula(TicketRec.GetRepoFormula());
	SetSWBooking(TicketRec.GetSWBooking());
	SetSWMaturity(TicketRec.GetSWMaturity());
    SetFxAcc(TicketRec.GetFxAcc());
    SetUSDAcc(TicketRec.GetUSDAcc());
	SetPFU(TicketRec.GetPFU());
	SetIPO(TicketRec.GetIPO());
	SetCustodian(TicketRec.GetCustodian());
	SetLink(TicketRec.GetLink());
	SetTransSeq(TicketRec.GetTransSeq());
	SetNote(TicketRec.GetNote());
	SetNote2(TicketRec.GetNote2());
	SetCPID(TicketRec.GetCPID());
	SetDocReq(TicketRec.GetDocReq());
	SetDocRecvd(TicketRec.GetDocRecvd());
	SetWI(TicketRec.GetWI());
	SetConfirm(TicketRec.GetConfirm());
	SetSwapTicket(TicketRec.GetSwapTicket());
	SetDeliveryDate(TicketRec.GetDeliveryDate());
	SetMargin(TicketRec.GetMargin());
	SetMarginCurrency(TicketRec.GetMarginCurrency());
	SetMarginAmount(TicketRec.GetMarginAmount());
	SetListed(TicketRec.GetListed());
	SetFunded(TicketRec.GetFunded());
	SetBinary(TicketRec.GetBinary());
	SetDV01(TicketRec.GetDV01());
	SetBooker(TicketRec.GetBooker());
	SetBookDate(TicketRec.GetBookDate());
	SetBestExecution(TicketRec.GetBestExecution());
	SetShortSale(TicketRec.GetShortSale());
	SetCSPBShort(TicketRec.GetCSPBShort());
	SetCancelTicket(TicketRec.GetCancelTicket());
	SetCorrectTicket(TicketRec.GetCorrectTicket());
	SetSecFee(TicketRec.GetSecFee());
	SetOrFee(TicketRec.GetOrFee());
	SetETrade(TicketRec.GetETrade());
    SetVersion(TicketRec.GetVersion());

    SetAssetDesc(TicketRec.GetAssetDesc());
    SetAssetCurr(TicketRec.GetAssetCurr());
	SetAccount(TicketRec.GetAccount());
}

void CTicketRec::FromAssetRec(CAssetRec &AssetRec)
{
	SetAsset(AssetRec.GetAsset());
	if(GetTransType() == CALL || GetTransType() == PUT || 
		GetTransType() == SPRSWCLL || GetTransType() == SPRSWPUT)
		SetOptTicker(AssetRec.GetOptTicker());
	else
		SetOptTicker(EMPTYSTRING);

	SetAssetDesc(AssetRec.GetDesc());
	SetAssetCurr(AssetRec.GetCurrency());
	SetAssetClass(AssetRec.GetClass());
}

void CTicketRec::ToAssetRec(CAssetRec &AssetRec)
{
	AssetRec.SetAsset(GetAsset());
	AssetRec.SetOptTicker(GetOptTicker());
	AssetRec.SetDesc(GetAssetDesc());
	AssetRec.SetCurrency(GetAssetCurr());
	AssetRec.SetClass(GetAssetClass());
}

void CTicketRec::ToDBRec(CDBRec &Rec)
{
	char buf[100], *p;

	Rec.RemoveAll();

    Rec.Add(GetTransNum());
    Rec.Add(GetPortfolio());
	strcpy_s(buf, 100, GetTicket());
	p = strstr(buf, PERIOD);
	if(p)
		p[0] = 0;
	Rec.Add(buf);
	Rec.Add(GetCP());
	Rec.Add(GetAsset());
	Rec.Add(GetTrader());
	Rec.Add(GetTradeDate());
	Rec.Add(GetExecTime());
	Rec.Add(GetValueDate());
	Rec.Add(GetTransType());
	Rec.Add(GetDir());
	Rec.Add(GetAmount());
	Rec.Add(GetPrice());
	Rec.Add(GetCurrency());
	Rec.Add(GetFxRate());
	Rec.Add(GetCT());
	Rec.Add(GetTRS());
	Rec.Add(GetStrike());
	Rec.Add(GetExerciseDate());
	Rec.Add(GetOptExp());
	Rec.Add(GetFXDate());
	Rec.Add(GetOptTicker());
	Rec.Add(GetOptSet());
	Rec.Add(GetOptSet2());
	Rec.Add(GetSetConvention());
	Rec.Add(GetOptID());
	Rec.Add(GetEuroOpt());
	Rec.Add(GetOptAuto());
//	Rec.Add(GetBroker());
	Rec.Add(GetBrokerFee());
	Rec.Add(GetSoftDollar());
	Rec.Add(GetOtherFee());
	Rec.Add(GetVAR());
	Rec.Add(GetRate());
	Rec.Add(GetRateBasis());
	Rec.Add(GetMaturity());
	Rec.Add(GetIndex());
	Rec.Add(GetRepoFormula());
	Rec.Add(GetSWBooking());
	Rec.Add(GetSWMaturity());
    Rec.Add(GetFxAcc());
    Rec.Add(GetUSDAcc());
	Rec.Add(GetPFU());
	Rec.Add(GetIPO());
	Rec.Add(GetCustodian());
	if(GetTransType() == SECURITIES || GetTransType() == CALL || GetTransType() == PUT || 
		GetTransType() == SPRSWCLL || GetTransType() == SPRSWPUT)
		Rec.Add(GetLink());
	else
		Rec.Add(EMPTYSTRING);
	Rec.Add(GetTransSeq());
	Rec.Add(GetNote());
	Rec.Add(GetNote2());
	Rec.Add(GetCPID());
	Rec.Add(GetDocReq());
	Rec.Add(GetDocRecvd());
	Rec.Add(GetWI());
	Rec.Add(GetConfirm());
	Rec.Add(GetSwapTicket());
	Rec.Add(GetDeliveryDate());
	Rec.Add(GetMargin());
	Rec.Add(GetMarginCurrency());
	Rec.Add(GetMarginAmount());
	Rec.Add(GetListed());
	Rec.Add(GetFunded());
	Rec.Add(GetBinary());
	Rec.Add(GetDV01());
	Rec.Add(GetBooker());
	Rec.Add(GetBookDate());
	Rec.Add(GetBestExecution());
	Rec.Add(GetShortSale());
	Rec.Add(GetCSPBShort());
	Rec.Add(GetCancelTicket());
	Rec.Add(GetCorrectTicket());
	Rec.Add(GetSecFee());
	Rec.Add(GetOrFee());
	Rec.Add(GetETrade());
    SetVersion();
	Rec.Add(GetVersion());
}

BOOL CTicketRec::GenerateInv(COraLoader &OraLoader)
{
	CInvRec InvRec;
	CQData QData;

	InvRec.SetTransNum(GetTransNum());
	InvRec.SetAsset(QData.RemoveString(GetAsset(), "'"));
	InvRec.SetCloseDate(EMPTYSTRING);
	InvRec.SetInterest(EMPTYSTRING);
	InvRec.SetCash(EMPTYSTRING);
	InvRec.SetCurrency(EMPTYSTRING);
	InvRec.SetAccount(GetAccount());
	InvRec.SetAssignCP(GetAssignCP());
	InvRec.SetAssignCT(GetAssignCT());
	InvRec.SetPostDate(EMPTYSTRING);
	InvRec.SetNomAmount(GetAmount()); 

	if(GetTransType() == INTSWAP)
	{
		InvRec.SetTrDesc(MATURITY);
		InvRec.SetValueDate(GetMaturity());
	}
	else
	{
		InvRec.SetTrDesc(BOOKING);
		InvRec.SetValueDate(GetValueDate());
	}
		
	if(!InvRec.AddRec(OraLoader))
		return FALSE;
		
	if(GetTransType() == FOREX || GetTransType() == LEVERAGE || GetTransType() == REPO || 
		(GetTransType() == SECURITIES && (GetDir() == B || GetDir() == L)))
	{
		if(GetTransType() == FOREX)
			InvRec.SetTrDesc(FXBKNG);
		else
		{
			InvRec.SetTrDesc(MATURITY);
			InvRec.SetValueDate(GetMaturity());
		}
		
	    if(!InvRec.AddRec(OraLoader))
			return FALSE;
	}

	return TRUE;
}

BOOL CTicketRec::UpdateFloatRepoRateRec(COraLoader &OraLoader, BOOL bDelete)
{
	BOOL bAdd = FALSE;
	int Count;
	CDBRec Rec;

	Rec.RemoveAll();

	OraLoader.GetSql().Format("SELECT COUNT(*) FROM SEMAM.NW_REPO_RATES "
						 "WHERE TRANS_NUM = %s ", GetTransNum());
	Count = OraLoader.GetCount();

	if(!bDelete && (GetTransType() == REPO || GetTransType() == LEVERAGE) &&
		(GetRepoFormula() == "DAILY FLOAT" || GetRepoFormula() == "DAILY FLOAT A")) // If daily repo, then add or modify
	{
		OraLoader.GetSql().Format("SELECT COUNT(*) FROM SEMAM.NW_REPO_RATES "
							 "WHERE TRANS_NUM = %s ", GetTransNum());
		if(Count == 0)  // Only update single record.
			bAdd = TRUE;
		else
			if(Count > 1) // If Multiple rates, do nothing.
				return TRUE;

		Rec.Add(GetValueDate());
		Rec.Add(EMPTYSTRING);
		Rec.Add(GetPortfolio());
		Rec.Add(GetTransNum());
		Rec.Add(GetRate());

		if(bAdd)
			OraLoader.GetSql().Format("SELECT INT_FROM, INT_TO, PORTFOLIO, TRANS_NUM, RATE "
							 "FROM SEMAM.NW_REPO_RATES ");
		else
			OraLoader.GetSql().Format("SELECT INT_FROM, INT_TO, PORTFOLIO, TRANS_NUM, RATE "
							 "FROM SEMAM.NW_REPO_RATES WHERE TRANS_NUM = %s ", GetTransNum());

		if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
			return FALSE;

		return OraLoader.UpdateRecord(Rec, bAdd);
	}
	else	// if it is not daily repo, remove
	{
		OraLoader.GetSql().Format("DELETE FROM SEMAM.NW_REPO_RATES WHERE TRANS_NUM = %s ", GetTransNum());
		return OraLoader.ExecuteSql();
	}
}

BOOL CTicketRec::AddRec(COraLoader &OraLoader)
{
	if(GetTransNum().IsEmpty())
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);

	OraLoader.GetSql().Format("SELECT TRANS_NUM, PORTFOLIO, TICKET_NUM, COUNTERPARTY, ASSET_CODE, TRADER_INI, TRADE_DATE, "
								"EXEC_TIME, VALUE_DATE, TRANS_TYPE, TRANS_DIRECTION, NOM_AMOUNT, PRICE, CURRENCY, FXRATE, "
								"CON_CODE, DEAL_TYPE, EXERCISE_PRICE, EXERCISE_DATE, OPT_EXPIRATION, FX_DATE, OPT_TICK, "
								"OPT_SET_CODE, OPT_SET_CODE2, SET_CONVENTION, OPT_ID, EURO_OPT, OPT_AUTO, BR_FEES, "
								"SOFTDOLLAR, LEV_RATE, VAR, TR_RATE, RATE_BASIS, MATURITY_DATE, FLOAT_RATE_INDEX, "
								"FLOAT_RATE_FORMULA, ORIG_SW_BOOKING, ORIG_SW_MATURITY, FX_ACCNUM, USD_ACCNUM, PFU, "
								"NEW_ISSUE, CUSTODIAN, OPTION_BACKING, TRANS_SEQ, HAN_DETAIL1, HAN_DETAIL2, CP_TRADE_ID, "
								"DOC_REQ, DOC_RECVD, WI, CONFIRM, SWAP_TICKET, DELIVERY_DATE, MARGIN, MARGIN_CURRENCY, "
								"MARGIN_AMOUNT, LISTED, FUNDED_SWAP, BINARY, DV01, BOOKER, BOOK_DATE, BEST_EXECUTION, "
								"SHORT_SALE, CSPB_SHORT, CANCEL_TICKET, CORRECT_TICKET, SEC_FEE, OR_FEE, ETRADE, TR_VERSION "
								"FROM SEMAM.NW_TR_TICKETS ");

	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	if(!OraLoader.UpdateRecord(Rec, TRUE))
		return FALSE;

	if(!GenerateInv(OraLoader))
		return FALSE;

	if(!UpdateFloatRepoRateRec(OraLoader))
		return FALSE;
	
	return UpdateCurrPos(OraLoader, TICKETREC_ADD);
}

BOOL CTicketRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetTransNum().IsEmpty())
		return FALSE;
	
	CQData QData;
	CDBRec Rec;	
						
	OraLoader.GetSql().Format("SELECT TRANS_NUM, PORTFOLIO, TICKET_NUM, COUNTERPARTY, ASSET_CODE, TRADER_INI, TRADE_DATE, "
								"EXEC_TIME, VALUE_DATE, TRANS_TYPE, TRANS_DIRECTION, NOM_AMOUNT, PRICE, CURRENCY, FXRATE, "
								"CON_CODE, DEAL_TYPE, EXERCISE_PRICE, EXERCISE_DATE, OPT_EXPIRATION, FX_DATE, OPT_TICK, "
								"OPT_SET_CODE, OPT_SET_CODE2, SET_CONVENTION, OPT_ID, EURO_OPT, OPT_AUTO, BR_FEES, "
								"SOFTDOLLAR, LEV_RATE, VAR, TR_RATE, RATE_BASIS, MATURITY_DATE, FLOAT_RATE_INDEX, "
								"FLOAT_RATE_FORMULA, ORIG_SW_BOOKING, ORIG_SW_MATURITY, FX_ACCNUM, USD_ACCNUM, PFU, "
								"NEW_ISSUE, CUSTODIAN, OPTION_BACKING, TRANS_SEQ, HAN_DETAIL1, HAN_DETAIL2, CP_TRADE_ID, "
								"DOC_REQ, DOC_RECVD, WI, CONFIRM, SWAP_TICKET, DELIVERY_DATE, MARGIN, MARGIN_CURRENCY, "
								"MARGIN_AMOUNT, LISTED, FUNDED_SWAP, BINARY, DV01, BOOKER, BOOK_DATE, BEST_EXECUTION, "
								"SHORT_SALE, CSPB_SHORT, CANCEL_TICKET, CORRECT_TICKET, SEC_FEE, OR_FEE, ETRADE, TR_VERSION "
								"FROM SEMAM.NW_TR_TICKETS WHERE TRANS_NUM = %s ", 
								QData.GetQueryNumber(GetTransNum()));

	ToDBRec(Rec);
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	if(!OraLoader.UpdateRecord(Rec))
		return FALSE;

	if(!UpdateFloatRepoRateRec(OraLoader))
		return FALSE;

	return UpdateCurrPos(OraLoader, TICKETREC_UPDATE);
}

BOOL CTicketRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetTransNum().IsEmpty())
		return FALSE;

	CInvRec Inv;
	CQData QData;

	Inv.SetTransNum(GetTransNum());
	if(!Inv.DeleteRec(OraLoader))
		return FALSE;

	OraLoader.GetSql().Format("DELETE FROM SEMAM.NW_TR_TICKETS WHERE TRANS_NUM = %s ", QData.GetQueryNumber(GetTransNum()));
	if(!OraLoader.ExecuteSql())
		return FALSE;
	
	if(!UpdateFloatRepoRateRec(OraLoader, TRUE))
		return FALSE;

	return UpdateCurrPos(OraLoader, TICKETREC_DELETE);
}

BOOL CTicketRec::UpdateCurrPos(COraLoader &OraLoader, int nAction)
{
	CQData QData;
	CString Asset, TransType, Ticket, OldAsset, OldTransType;
	BOOL bRet;

	if(nAction >= TICKETREC_ADD && nAction <= TICKETREC_DELETE)
	{
		Asset = QData.GetQueryText(GetAsset());
		TransType = GetTransType();
		Ticket = QData.GetQueryText(GetTicket());
		OraLoader.ExecuteSql("DELETE FROM SEMAM.NW_CURR_POS "
							"WHERE TICKET IS NULL AND ASSET_CODE = " + Asset);
		OraLoader.ExecuteSql("DELETE FROM SEMAM.NW_CURR_POS WHERE TICKET = " + Ticket);
		if(nAction == TICKETREC_UPDATE)
		{
			CString TransNum = QData.GetQueryNumber(GetTransNum());
				
			OraLoader.Open("SELECT ASSET_CODE, TRANS_TYPE FROM SEMAM.NW_TR_HIST "
							"WHERE TRANS_NUM = " + TransNum + " ORDER BY TR_VERSION DESC ");
			OraLoader.MoveFirst();
			while(!OraLoader.IsEOF())
			{
				OldAsset = OraLoader.GetDBString(0);
				OldTransType = OraLoader.GetDBString(1);
				break;
			}
		}
		
		bRet = UpdateCurrPosRecs(OraLoader, GetAsset(), GetTransType(), GetTicket());
		if(nAction == TICKETREC_UPDATE && Asset != OldAsset)
		{
			Ticket.Empty();
			bRet = UpdateCurrPosRecs(OraLoader, OldAsset, OldTransType, Ticket);
		}
		
		return bRet;
	}

	return FALSE;
}

BOOL CTicketRec::UpdateCurrPosRecs(COraLoader &OraLoader, CString &Asset, 
								   CString &TransType, CString &Ticket)
{
	CQData QData;
	CString Sql;

	Asset = QData.GetQueryText(Asset);
	TransType = QData.GetQueryText(TransType);
	if(!Ticket.IsEmpty())
		Ticket = QData.GetQueryText(Ticket);

	Sql = "INSERT INTO SEMAM.NW_CURR_POS (INDATE, TRANS_TYPE, ASSET_CODE, TRS, IDX, TICKET, RATE, RATE_TYPE, "
			"STRIKE, MATURITY, DIR, NOM_AMOUNT, OPT_ID, OPT_TICK, DELIVERY_DATE, OPT_SET_CODE, OPT_LISTED, ASSIGN_CP) "
			"SELECT TRUNC(SYSDATE), A.TRANS_TYPE, A.ASSET_CODE, A.DEAL_TYPE, 1, NULL, TO_NUMBER(NULL), NULL, "
			"TO_NUMBER(NULL), TO_DATE(NULL), DECODE(SIGN(SUM(DECODE(A.DIR, 'P', 1, 'S', -1)*A.NOM_AMOUNT)), 1, 'P', -1, 'S'), "
			"ABS(SUM(DECODE(A.DIR, 'P', 1, 'S', -1)*A.NOM_AMOUNT)), NULL, NULL, TO_DATE(NULL), NULL, NULL, A.ASSIGN_CP "
			"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B, SEMAM.NW_PORTFOLIOS Z "
			"WHERE B.ASS_CODE = A.ASSET_CODE "
			"AND B.ASS_INDUSTRY != 'CURRENCY FWDS' "
			"AND Z.PORTFOLIO = A.PORTFOLIO "
			"AND Z.STATUS IS NULL "
			"AND A.TRANS_TYPE IN ('SECURITIES') "
			"AND A.DIR IN ('P', 'S') "
			"AND A.TRANS_TYPE = " + TransType + 
			" AND A.ASSET_CODE = " + Asset + 
			" GROUP BY A.TRANS_TYPE, A.ASSET_CODE, A.DEAL_TYPE, A.ASSIGN_CP "
			"HAVING ABS(SUM(DECODE(A.DIR, 'P', 1, 'S', -1)*A.NOM_AMOUNT)) > 0 "
			"UNION "
			"SELECT TRUNC(SYSDATE), A.TRANS_TYPE, A.ASSET_CODE, NULL, 1, NULL, TO_NUMBER(NULL), NULL, "
			"TO_NUMBER(NULL), ASS_MATURITY_DATE, DECODE(SIGN(SUM(DECODE(A.DIR, 'P', 1, 'S', -1)*A.NOM_AMOUNT)), 1, 'P', -1, 'S'), "
			"ABS(SUM(DECODE(A.DIR, 'P', 1, 'S', -1)*A.NOM_AMOUNT)), NULL, NULL, TO_DATE(NULL), NULL, NULL, A.ASSIGN_CP "
			"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B, SEMAM.NW_PORTFOLIOS Z "
			"WHERE B.ASS_CODE = A.ASSET_CODE "
			"AND B.ASS_INDUSTRY != 'CURRENCY FWDS' "
			"AND Z.PORTFOLIO = A.PORTFOLIO "
			"AND Z.STATUS IS NULL "
			"AND A.TRANS_TYPE IN ('CDS') "
			"AND A.DIR IN ('P', 'S') "
			"AND A.TRANS_TYPE = " + TransType + 
			" AND A.ASSET_CODE = " + Asset + 
			" GROUP BY A.TRANS_TYPE, A.ASSET_CODE, ASS_MATURITY_DATE, A.ASSIGN_CP "
			"HAVING ABS(SUM(DECODE(A.DIR, 'P', 1, 'S', -1)*A.NOM_AMOUNT)) > 0 "
			"UNION "
			"SELECT TRUNC(SYSDATE), A.TRANS_TYPE, A.ASSET_CODE, NULL, 1, NULL, TO_NUMBER(NULL), NULL, "
			"TO_NUMBER(NULL), A.VALUE_DATE, DECODE(SIGN(SUM(DECODE(A.DIR, 'P', 1, 'S', -1)*A.NOM_AMOUNT)), 1, 'P', -1, 'S'), "
			"ABS(SUM(DECODE(A.DIR, 'P', 1, 'S', -1)*A.NOM_AMOUNT)), NULL, NULL, TO_DATE(NULL), NULL, NULL, A.ASSIGN_CP "
			"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B, SEMAM.NW_PORTFOLIOS Z "
			"WHERE B.ASS_CODE = A.ASSET_CODE "
			"AND B.ASS_INDUSTRY = 'CURRENCY FWDS' "
			"AND Z.PORTFOLIO = A.PORTFOLIO "
			"AND Z.STATUS IS NULL "
			"AND A.TRANS_TYPE IN ('CDS', 'SECURITIES') "
			"AND A.DIR IN ('P', 'S') "
			"AND A.VALUE_DATE > TRUNC(SYSDATE) "
			"AND A.TRANS_TYPE = " + TransType + 
			"AND A.ASSET_CODE = " + Asset + 
			" GROUP BY A.TRANS_TYPE, A.ASSET_CODE, A.VALUE_DATE, A.ASSIGN_CP "
			"HAVING ABS(SUM(DECODE(A.DIR, 'P', 1, 'S', -1)*A.NOM_AMOUNT)) > 0 ";
	if(!Ticket.IsEmpty())
		Sql += "UNION "
			"SELECT TRUNC(SYSDATE), A.TRANS_TYPE, A.ASSET_CODE, NULL, 3, A.TICKET_NUM, RATE, ASS_INT_TYPE, "
			"TO_NUMBER(NULL), NVL(A.ACTUAL_VDATE, NVL(A.MATURITY_DATE, ASS_MATURITY_DATE)), A.DIR, "
			"SUM(A.NOM_AMOUNT), NULL, NULL, TO_DATE(NULL), NULL, NULL, A.ASSIGN_CP " 
			"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B, SEMAM.NW_ASS_PERIODS C, SEMAM.NW_PORTFOLIOS Z "
			"WHERE B.ASS_CODE = A.ASSET_CODE "
			"AND B.ASS_INT_TYPE = 'FIXED' "
			"AND C.ASS_CODE(+) = A.ASSET_CODE "
			"AND C.ASS_FROM(+) <= A.VALUE_DATE "
			"AND C.ASS_TO(+) > A.VALUE_DATE "
			"AND Z.PORTFOLIO = A.PORTFOLIO "
			"AND Z.STATUS IS NULL "
			"AND A.TRANS_TYPE IN ('INT. SWAPS') "
			"AND A.DIR IN ('P', 'S') "
			"AND NVL(A.ACTUAL_VDATE, NVL(A.MATURITY_DATE, NVL(ASS_MATURITY_DATE, SYSDATE))) > TRUNC(SYSDATE) "
			"AND A.TRANS_TYPE = " + TransType +  
			" AND A.ASSET_CODE = " + Asset + 
			" AND A.TICKET_NUM = " + Ticket + 
			" GROUP BY A.TRANS_TYPE, A.ASSET_CODE, A.TICKET_NUM, RATE, ASS_INT_TYPE, "
			"NVL(A.ACTUAL_VDATE, NVL(A.MATURITY_DATE, ASS_MATURITY_DATE)), A.DIR, A.ASSIGN_CP "
			"UNION "
			"SELECT TRUNC(SYSDATE), A.TRANS_TYPE, A.ASSET_CODE, NULL, 4, A.TICKET_NUM, TR_RATE, "
			"FLOAT_RATE_FORMULA, TO_NUMBER(NULL), NVL(A.ACTUAL_VDATE, A.MATURITY_DATE), A.DIR, "
			"SUM(A.NOM_AMOUNT), NULL, NULL, TO_DATE(NULL), NULL, NULL, A.ASSIGN_CP " 
			"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B, SEMAM.NW_PORTFOLIOS Z "
			"WHERE B.ASS_CODE = A.ASSET_CODE "
			"AND Z.PORTFOLIO = A.PORTFOLIO "
			"AND Z.STATUS IS NULL "
			"AND A.TRANS_TYPE IN ('REPO') "
			"AND A.DIR IN ('P', 'S') "
			"AND NVL(A.ACTUAL_VDATE, NVL(A.MATURITY_DATE, SYSDATE)) > TRUNC(SYSDATE) "
			"AND A.TRANS_TYPE = " + TransType +  
			" AND A.ASSET_CODE = " + Asset +  
			" AND A.TICKET_NUM = " + Ticket +  
			" GROUP BY A.TRANS_TYPE, A.ASSET_CODE, A.TICKET_NUM, TR_RATE, FLOAT_RATE_FORMULA, "
			"NVL(A.ACTUAL_VDATE, A.MATURITY_DATE), A.DIR, A.ASSIGN_CP "
			"UNION "
			"SELECT TRUNC(SYSDATE), A.TRANS_TYPE, A.ASSET_CODE, NULL, 5, A.TICKET_NUM, TO_NUMBER(NULL), "
			"NULL, EXERCISE_PRICE, OPT_EXPIRATION, A.TRANS_DIRECTION, SUM(A.NOM_AMOUNT), A.OPT_ID, "
			"A.OPT_TICK, A.DELIVERY_DATE, A.OPT_SET_CODE, A.LISTED, B.ASSIGN_CP "
			"FROM SEMAM.NW_OPTION_VIEW A, SEMAM.NW_TR_INV B "
			"WHERE B.TRANS_NUM = A.TRANS_NUM "
			"AND A.NOM_AMOUNT > 0 "
			"AND A.TRANS_TYPE = " + TransType +  
			" AND A.ASSET_CODE = " + Asset +  
			" AND A.TICKET_NUM = " + Ticket +  
			" GROUP BY A.TRANS_TYPE, A.ASSET_CODE, A.TICKET_NUM, A.EXERCISE_PRICE, A.OPT_EXPIRATION, "
			"A.TRANS_DIRECTION, A.OPT_ID, A.OPT_TICK, A.DELIVERY_DATE, A.OPT_SET_CODE, A.LISTED, B.ASSIGN_CP ";
	return OraLoader.ExecuteSql(Sql);
}