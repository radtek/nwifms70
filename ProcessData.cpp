#include "stdafx.h"
#include "NWIFMS70.h"
#include <math.h>
#include "processdata.h"
#include "sqlstmt.h"
#include "qdata.h"


IMPLEMENT_DYNAMIC(CProcessData, CEntryData)
void CProcessData::Setup(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS, LPCTSTR User, int KeyIndex)
{
	CEntryData::Setup(OraLoader, pSS, pSSS, EMPTYSTRING, User, KeyIndex);
	GetRawTicket().Setup(GetRawTicket().GetUser());
}

BOOL CProcessData::GetSelInv(int Index, CInvRec &InvRec)
{
	if(Index < 1 || Index > GetSRowCtrl().GetSS()->GetSheetRows())
		return FALSE;

	CString Text;
	for(int i = 1; i <= GetSRowCtrl().GetSS()->GetSheetCols(); i ++)
	{
		Text = GetSRowCtrl().GetSS()->GetSheetText(i, Index);
		switch(i)
		{
			case 1:
				InvRec.SetID(Text);
				break;
			case 2:
				InvRec.SetAsset(Text);
				break;
			case 3:
				InvRec.SetTrDesc(Text);
				break;
			case 4:
				InvRec.SetNomAmount(Text);
				break;
			case 5:
				InvRec.SetValueDate(Text);
				break;
			case 6:
				InvRec.SetCloseDate(Text);
				break;
			case 7:
				InvRec.SetPostDate(Text);
				break;
			case 8:
				InvRec.SetCurrency(Text);
				break;
			case 9:
				InvRec.SetCash(Text);
				break;
			case 10:
				InvRec.SetInterest(Text);
				break;
			case 11:
				InvRec.SetAccount(Text);
				break;
			case 12:
				InvRec.SetAssignCP(Text);
				break;
			case 13:
				InvRec.SetAssignCT(Text);
				break;
			default:
				break;
		}
	}

	return TRUE;
}

BOOL CProcessData::GetSelRawInv(int Index, CRawInvRec &RecInvRec)
{
	if(Index < 1 || Index > GetSRowCtrl().GetSS()->GetSheetRows())
		return FALSE;

	CString Text;
	for(int i = 1; i <= GetSRowCtrl().GetSS()->GetSheetCols(); i++)
	{
		Text = GetSRowCtrl().GetSS()->GetSheetText(i, Index);
		switch(i)
		{
			case 1:	// Ticket;
				break;
			case 2:
				RecInvRec.SetPortfolio(Text);
				break;
			case 3:
				RecInvRec.SetNomAmount(Text);
				break;
			case 4:
				RecInvRec.SetPrice(Text);
				break;
			case 5:
				RecInvRec.SetLink(Text);
				break;
/*			case 5:
				RecInvRec.SetAssignCT(Text);
				break;
			case 4:
				RecInvRec.SetDownPymnt(Text);
				break;
			case 5:
				RecInvRec.SetBrokerFee(Text);
				break;
			case 6:
				RecInvRec.SetSoftDollar(Text);
				break;
			case 7:
				RecInvRec.SetOtherFee(Text);
				break;
			case 8:
				RecInvRec.SetVAR(Text);
				break;
			case 9:
				RecInvRec.SetLink(Text);
				break;
			case 10:
				RecInvRec.SetAssignCP(Text);
				break;
			case 11:
				RecInvRec.SetAssignCT(Text);
				break; */
			default:
				break;
		}
	}
	
	return TRUE;
}

BOOL CProcessData::GenerateTicketRec(CRawInvRec &RawInvRec)
{
	CString TransType, Dir, Buf;
	double Fx;
	CQData QData;
	BOOL bRepoLeg = FALSE;

	if(!GetRawTicket().GetRepoCP().IsEmpty() && RawInvRec.GetDownPymnt() == "100")
	{
		bRepoLeg = TRUE;
		TransType = GetRawTicket().GetRepoType();
		Fx = atof(QData.RemoveComma(GetRawTicket().GetFxRate()));
		if(TransType == REPO)
			Dir = GetRawTicket().GetDir() == "P" ? "S" : "P";
		else
			Dir = GetRawTicket().GetDir();
	}
	else
	{
		TransType = GetRawTicket().GetTransType();
		Dir = GetRawTicket().GetDir();
	}

	if(GetTicket().GetTransNum().IsEmpty()) // No TransNum
		GetOraLoader().GenerateUniqueID(TRANSNUM, GetTicket().GetTransNum());
    GetTicket().SetPortfolio(RawInvRec.GetPortfolio());
	GetTicket().SetTicket(GetRawTicket().GetTicket());
	if(bRepoLeg)
		GetTicket().SetCP(GetRawTicket().GetRepoCP());
	else
		GetTicket().SetCP(GetRawTicket().GetCP());
	if(TransType == CASH)
		GetTicket().SetAsset(EMPTYSTRING);
	else
		GetTicket().SetAsset(GetRawTicket().GetAsset());
	GetTicket().SetTrader(GetRawTicket().GetTrader());
	GetTicket().SetTradeDate(GetRawTicket().GetTradeDate());
	GetTicket().SetExecTime(GetRawTicket().GetExecTime());
	GetTicket().SetValueDate(GetRawTicket().GetValueDate());
	GetTicket().SetLink(RawInvRec.GetLink());
	GetTicket().SetTransType(TransType);
	GetTicket().SetDir(Dir);
	GetTicket().SetAmount(RawInvRec.GetNomAmount());
	GetTicket().SetPrice(RawInvRec.GetPrice());

	if(bRepoLeg && Fx > 0)
		GetTicket().SetCurrency("USD");
	else
		GetTicket().SetCurrency(GetRawTicket().GetCurrency());

	if(GetTicket().GetCurrency() == "USD")
		GetTicket().SetFxRate("1");
	else
		GetTicket().SetFxRate(GetRawTicket().GetFxRate());

	if(bRepoLeg)
		GetTicket().SetCT(GetRawTicket().GetRepoCT());

	if(TransType == SECURITIES || TransType == REPO || TransType == LEVERAGE)
		GetTicket().SetTRS(GetRawTicket().GetTRS() == Y ? TRS : NULL);
	else
		GetTicket().SetTRS(EMPTYSTRING);
	if(TransType == CALL || TransType == PUT || TransType == SPRSWCLL || TransType == SPRSWPUT)
	{
		GetTicket().SetStrike(GetRawTicket().GetStrike());
		GetTicket().SetOptExp(GetRawTicket().GetOptExp());
		GetTicket().SetFXDate(GetRawTicket().GetFXDate());
		GetTicket().SetOptTicker(GetRawTicket().GetOptTicker());
		GetTicket().SetOptID(GetRawTicket().GetOptID());
		GetTicket().SetEuroOpt(GetRawTicket().GetEuroOpt());
		GetTicket().SetOptSet(GetRawTicket().GetOptSetCode());
		GetTicket().SetOptAuto(GetRawTicket().GetOptAuto());
		GetTicket().SetDeliveryDate(GetRawTicket().GetDeliveryDate());
	}
	else
	{
		GetTicket().SetStrike(EMPTYSTRING);
		GetTicket().SetFXDate(EMPTYSTRING);
		GetTicket().SetExerciseDate(EMPTYSTRING);
		GetTicket().SetOptExp(EMPTYSTRING);
		GetTicket().SetOptSet(EMPTYSTRING);
		GetTicket().SetOptID(EMPTYSTRING);
		GetTicket().SetEuroOpt(EMPTYSTRING);
		GetTicket().SetOptSet(EMPTYSTRING);
		GetTicket().SetOptAuto(EMPTYSTRING);
		GetTicket().SetDeliveryDate(EMPTYSTRING);
	}

	GetTicket().SetOptSet(GetRawTicket().GetOptSetCode());
	GetTicket().SetOptSet2(GetRawTicket().GetOptSetCode2());
	GetTicket().SetSetConvention(GetRawTicket().GetSetConvention());

	GetTicket().SetBrokerFee(GetRawTicket().Alloc(Buf, GetRawTicket().GetBrokerFee(), RawInvRec.GetNomAmount()));
	GetTicket().SetSoftDollar(GetRawTicket().Alloc(Buf, GetRawTicket().GetSoftDollar(), RawInvRec.GetNomAmount()));
	GetTicket().SetOtherFee(RawInvRec.GetOtherFee());
	GetTicket().SetMarginAmount(GetRawTicket().Alloc(Buf, GetRawTicket().GetMarginAmount(), RawInvRec.GetNomAmount()));
	if(!GetRawTicket().GetRepoCP().IsEmpty() && RawInvRec.GetDownPymnt() == "100") 
		GetTicket().SetVAR("0.01");
	else
		GetTicket().SetVAR(GetRawTicket().Alloc(Buf, GetRawTicket().GetVAR(), RawInvRec.GetNomAmount()));

	if(TransType == CASH || TransType == INTSWAP || TransType == LEVERAGE || TransType == REPO ||
		(TransType == SECURITIES && (GetTicket().GetDir() == B || GetTicket().GetDir() == L)))
	{
		GetTicket().SetRate(GetRawTicket().GetRate());
		GetTicket().SetRateBasis(GetRawTicket().GetRateBasis());
		GetTicket().SetMaturity(GetRawTicket().GetMaturity());
		
		if(TransType == INTSWAP && GetTicket().GetIndex() != "FIXED")
		{
			GetTicket().SetRate(GetRawTicket().GetSWFloatRate());
			GetTicket().SetRateBasis(GetRawTicket().GetSWFloatBasis());
			if(!GetRawTicket().GetSWFloatMaturity().IsEmpty())
				GetTicket().SetMaturity(GetRawTicket().GetSWFloatMaturity());
		}
	}
	else
	{
		GetTicket().SetRate(EMPTYSTRING);
		GetTicket().SetRateBasis(EMPTYSTRING);
		GetTicket().SetMaturity(EMPTYSTRING);
		GetTicket().SetIndex(EMPTYSTRING);
		GetTicket().SetFormula(EMPTYSTRING);
	}

	GetTicket().SetSWBooking(GetRawTicket().GetSWBooking());
	if(TransType == INTSWAP)
		GetTicket().SetSWMaturity(GetRawTicket().GetSWMaturity());
	else
		GetTicket().SetSWMaturity(EMPTYSTRING);
	
	if(TransType != FOREX)
	{
		GetTicket().SetFxAcc(EMPTYSTRING);
		GetTicket().SetUSDAcc(EMPTYSTRING);
	}

	if(TransType == SECURITIES && (GetTicket().GetDir() == P || GetTicket().GetDir() == S))
		GetTicket().SetIPO(GetRawTicket().GetIPO());
	else
		GetTicket().SetIPO(EMPTYSTRING);

	GetTicket().SetCustodian(RawInvRec.GetCustodian());
	if(TransType == SECURITIES || TransType == CALL || TransType == PUT)
		GetTicket().SetLink(RawInvRec.GetLink());
	else
		GetTicket().SetLink(EMPTYSTRING);
	GetTicket().SetNote(GetRawTicket().GetNote());
	GetTicket().SetNote2(GetRawTicket().GetNote2());
	GetTicket().SetCPID(GetRawTicket().GetCPTradeID());

	GetTicket().SetDV01(GetRawTicket().GetDV01());
    GetTicket().SetVersion("1");

    GetTicket().SetAssetDesc(GetRawTicket().GetAssetDesc());
    GetTicket().SetAssetCurr(GetRawTicket().GetAssetCurr());
	GetTicket().SetAssetClass(GetRawTicket().GetAssetClass());
	GetTicket().SetAccount(RawInvRec.GetAccount());
	GetTicket().SetAssignCP(GetRawTicket().GetAssignCP());
	GetTicket().SetAssignCT(GetRawTicket().GetAssignCT());

	GetTicket().SetWI(GetRawTicket().GetWI());
	GetTicket().SetSwapTicket(GetRawTicket().GetSwapTicket());
	GetTicket().SetDeliveryDate(GetRawTicket().GetDeliveryDate());
	GetTicket().SetMargin(GetRawTicket().GetMargin());
	GetTicket().SetMarginCurrency(GetRawTicket().GetMarginCurrency());
	GetTicket().SetListed(GetRawTicket().GetListed());
	GetTicket().SetBestExecution(GetRawTicket().GetBestExecution());
	GetTicket().SetShortSale(GetRawTicket().GetShortSale());
	GetTicket().SetCSPBShort(GetRawTicket().GetCSPBShort());
	GetTicket().SetBooker(GetBooker());
	GetTicket().SetBookDate(GetBookDate());
	GetTicket().SetBinary(GetRawTicket().GetBinary());

	if(GetTicket().GetTransType() == FOREX && GetFxRecArray().GetSize() > 0)
		UpdateFxRef(GetTicket().GetTransNum(), GetTicket().GetPortfolio());

	GetTicket().SetCancelTicket(GetRawTicket().GetCancelTicket());
	GetTicket().SetCorrectTicket(GetRawTicket().GetCorrectTicket());
	GetTicket().SetSecFee(GetRawTicket().GetSecFee());
	GetTicket().SetOrFee(GetRawTicket().GetOrFee());
	GetTicket().SetETrade(GetRawTicket().GetETrade());

	return TRUE;
}

int CProcessData::LoadTickets()
{
	CSqlStmt Sql;

	Sql = "SELECT A.TICKET_NUM, A.TICKET_NUM, A.COUNTERPARTY, A.ASSET_CODE, A.TRADER_INI, A.TRADE_DATE, A.EXEC_TIME, "
			"A.VALUE_DATE, A.TRANS_TYPE, A.DIR, A.NOM_AMOUNT, A.STRIKE, A.OPT_EXPIRATION, A.FX_DATE, A.OTC, A.OPT_TICK, "
			"A.OPT_SET_CODE, A.OPT_SET_CODE2, A.SET_CONVENTION, A.ASSET_PID, A.EURO_OPT, A.OPT_AUTO, A.CURRENCY, "
			"A.FXRATE, A.ASSET_DESC, A.ASSET_CURRENCY, A.IPO, A.TRS, A.REPO_CP, A.TR_RATE, "
			"DECODE(A.TRANS_TYPE, 'REPO', NVL(A.RATE_BASIS,'A/360'), A.RATE_BASIS) \"RATE_BASIS\", A.MATURITY, "
			"A.SW_BOOKING, A.SW_MATURITY, A.NOTE, A.NOTE2, NULL \"REPO_CT\", NULL \"REPO_TYPE\", "
			"DECODE(A.TRANS_TYPE, 'INT. SWAP','FIXED', NULL) \"FLOAT_RATE_INDEX\", NULL \"FLOAT_RATE_FORMULA\", "
			"NULL \"USD_ACCNUM\", NULL \"FX_ACCNUM\", "
			"DECODE(A.VERIFIED, 'Y', DECODE(A.COUNTERPARTY, 'INTERACTIVE BROKERS', 'U', 'P'), NULL) \"PFU\", "
			"TO_DATE(NULL) \"EXERCISE_DATE\", DECODE(A.VERIFIED, 'Y', '1', NULL) \"CON_CODE\", NULL \"D_REQ\", "
			"NULL \"D_RECVD\", WI, NULL \"CONFIRM\", A.SWAP_TICKET, A.DELIVERY_DATE, A.MARGIN, A.MARGIN_CURRENCY, "
			"A.MARGIN_AMOUNT, A.FUNDED_SWAP, A.BINARY, AA_METHOD, A.AA_FREASON, A.BEST_EXECUTION, A.SHORT_SALE, "
			"A.CSPB_SHORT, A.PRICE, NVL(A.DOWN_PYMNT, 100) \"DOWN_PYMNT\", A.BROKER_FEE, A.SOFTDOLLAR, A.VAR, A.DV01, "
			"A.ASSIGN_CP, NULL \"ASSIGN_CT\", A.UNWIND_TICKET, A.CANCEL_TICKET, A.CORRECT_TICKET, A.SEC_FEE, A.OR_FEE, "
			"A.ETRADE, A.BOOKER, TO_CHAR(A.BOOK_DATE, 'MM/DD/YYYY HH24:MI:SS'), A.ASSET_COUNTRY, A.ASSET_COMMON_CODE, "
			"A.ASSET_MSTC_CODE, A.ASSET_ISIN_CODE, A.ASSET_SEDOL_NUM, A.ASSET_CLASS, A.ASSET_IND_AREA, A.ASSET_CATEGORY, "
			"A.ASSET_BUCKET, A.SOURCE, A.ASSET_COUPON, A.ASSET_MATURITY, B.ASS_PAR_VALUE, A.IMG_NOMINAL, A.CP_TRADE_ID, "
			"A.IMAGINE_ORDER_ID, REV "
			"FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_ASSETS B, SEMAM.NW_CURRENCY C "
			"WHERE B.ASS_CODE(+) = A.ASSET_CODE "
			"AND C.CURRENCY(+) = B.ASS_CURRENCY2 "
			"AND SIGN IS NOT NULL "
			"AND PROCESSED IS NULL "
			"UNION "
			"SELECT A.TICKET_NUM+.2, A.TICKET_NUM, A.COUNTERPARTY, A.ASSET_CODE, A.TRADER_INI, A.TRADE_DATE, A.EXEC_TIME, "
			"A.VALUE_DATE, A.TRANS_TYPE, DECODE(A.DIR, 'P', 'S', 'S', 'P'), A.NOM_AMOUNT, A.STRIKE, A.OPT_EXPIRATION, "
			"A.FX_DATE, A.OTC, A.OPT_TICK, A.OPT_SET_CODE, A.OPT_SET_CODE2, A.SET_CONVENTION, A.ASSET_PID, A.EURO_OPT, "
			"A.OPT_AUTO, A.CURRENCY, A.FXRATE, A.ASSET_DESC, A.ASSET_CURRENCY, A.IPO, A.TRS, A.REPO_CP, TO_CHAR(A.FLOAT_RATE), "
			"A.FLOAT_RATE_BASIS, A.FLOAT_MATURITY, A.SW_BOOKING, A.SW_MATURITY, A.NOTE, A.NOTE2, NULL \"REPO_CT\", "
			"NULL \"REPO_TYPE\", 'FLOAT' \"FLOAT_RATE_INDEX\", NULL \"FLOAT_RATE_FORMULA\", NULL \"USD_ACCNUM\", "
			"NULL \"FX_ACCNUM\", NULL \"PFU\", TO_DATE(NULL) \"EXERCISE_DATE\", NULL \"CON_CODE\", NULL \"D_REQ\", NULL \"D_RECVD\", "
			"A.WI, NULL \"CONFIRM\", A.SWAP_TICKET, A.DELIVERY_DATE, TO_NUMBER(NULL) \"MARGIN\", NULL \"MARGIN_CURRENCY\", "
			"TO_NUMBER(NULL) \"MARGIN_AMOUNT\", A.FUNDED_SWAP, A.BINARY, A.AA_METHOD, A.AA_FREASON, A.BEST_EXECUTION, "
			"A.SHORT_SALE, A.CSPB_SHORT, A.PRICE, NVL(A.DOWN_PYMNT, 100) \"DOWN_PYMNT\", A.BROKER_FEE, A.SOFTDOLLAR, "
			"A.VAR, A.DV01, A.ASSIGN_CP, NULL \"ASSIGN_CT\", UNWIND_TICKET, A.CANCEL_TICKET, A.CORRECT_TICKET, A.SEC_FEE, "
			"A.OR_FEE, A.ETRADE, A.BOOKER, TO_CHAR(A.BOOK_DATE, 'MM/DD/YYYY HH24:MI:SS'), A.ASSET_COUNTRY, A.ASSET_COMMON_CODE, "
			"A.ASSET_MSTC_CODE, A.ASSET_ISIN_CODE, A.ASSET_SEDOL_NUM, A.ASSET_CLASS, A.ASSET_IND_AREA, A.ASSET_CATEGORY, "
			"A.ASSET_BUCKET, A.SOURCE, A.ASSET_COUPON, A.ASSET_MATURITY, B.ASS_PAR_VALUE, A.IMG_NOMINAL, A.CP_TRADE_ID, "
			"A.IMAGINE_ORDER_ID, REV "
			"FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_ASSETS B, SEMAM.NW_CURRENCY C "
			"WHERE B.ASS_CODE(+) = A.ASSET_CODE "
			"AND C.CURRENCY(+) = B.ASS_CURRENCY2 "
			"AND SIGN IS NOT NULL "
			"AND PROCESSED IS NULL AND TRANS_TYPE = 'INT. SWAP' "
			"ORDER BY 1 ";

	if(GetOraLoader().Open(Sql)) // Ticket Num
	{
		GetOraLoader().GetFieldArray().GetAt(0)->SetWithComma(FALSE);
		return GetOraLoader().LoadRowCtrl(*this, TRUE, FALSE);
	}
	return -1;
}

int CProcessData::LoadAllocList()
{
	if(GetOraLoader().Open("SELECT A.TICKET_NUM, ROWIDTOCHAR(A.ROWID) \"ID\", TO_NUMBER(NULL) \"TRANS_NUM\", A.PORTFOLIO, "
							"A.NOM_AMOUNT, DECODE(B.VERIFIED, 'Y', NVL(A.CUSTODIAN, DEF_CUSTODIAN), A.CUSTODIAN) \"CUSTODIAN\", "
							"DECODE(B.VERIFIED, 'Y', NVL(A.ACCOUNT, DEF_ACCOUNT), A.ACCOUNT) \"ACCOUNT\", B.PRICE, "
							"C.TRANS_NUM \"OPT_BACK\", B.DOWN_PYMNT, A.OTHER_FEES "
							"FROM SEMAM.NW_RAW_TICKET_DETAIL A "
							"JOIN SEMAM.NW_RAW_TICKETS B ON (A.TICKET_NUM = B.TICKET_NUM "
															"AND B.SIGN IS NOT NULL AND B.PROCESSED IS NULL) "
							"LEFT OUTER JOIN SEMAM.NW_TR_TICKETS C ON (A.PORTFOLIO = C.PORTFOLIO "
																"AND TO_CHAR(B.UNWIND_TICKET) = C.TICKET_NUM) "
							"JOIN SEMAM.NW_PORTFOLIOS Z ON (Z.PORTFOLIO = A.PORTFOLIO) "
							"UNION " 
							"SELECT A.TICKET_NUM+.2, ROWIDTOCHAR(A.ROWID), TO_NUMBER(NULL), A.PORTFOLIO, A.NOM_AMOUNT, "
							"A.CUSTODIAN, A.ACCOUNT, B.PRICE, TO_NUMBER(NULL) \"OPT_BACK\", B.DOWN_PYMNT, A.OTHER_FEES "
							"FROM SEMAM.NW_RAW_TICKET_DETAIL A "
							"JOIN SEMAM.NW_RAW_TICKETS B ON (A.TICKET_NUM = B.TICKET_NUM "
										"AND B.SIGN IS NOT NULL AND B.PROCESSED IS NULL "
										"AND B.TRANS_TYPE = 'INT. SWAP') ORDER BY 1, 4 "))
	{
		GetOraLoader().GetFieldArray().GetAt(0)->SetWithComma(FALSE); // TicketNum
		GetOraLoader().GetFieldArray().GetAt(2)->SetWithComma(FALSE); // TransNum
		GetOraLoader().GetFieldArray().GetAt(7)->SetWithComma(FALSE); // Price
		GetOraLoader().GetFieldArray().GetAt(8)->SetWithComma(FALSE); // OptBacking
		GetOraLoader().CopyFieldArray(GetSRowCtrl().GetSS()->GetFieldArray());
		GetSRowCtrl().GetSS()->GetFieldArray().RemoveAt(0); // Remove Key
		GetSRowCtrl().GetSS()->GetFieldArray().RemoveAt(0); // Remove ID
		GetSRowCtrl().GetSS()->SetSheetColHeaders();
		return GetOraLoader().LoadKeyDBList(GetAllocList());
	}

	return -1;
}

int CProcessData::ReLoadAllocList(const CString Ticket, const CString OptTicket)
{
	if(OptTicket.IsEmpty())
		GetOraLoader().Open("SELECT A.TICKET_NUM, ROWIDTOCHAR(A.ROWID) \"ID\", TO_NUMBER(NULL) \"TRANS_NUM\", A.PORTFOLIO, "
							"A.NOM_AMOUNT, DECODE(B.VERIFIED, 'Y', NVL(A.CUSTODIAN, DEF_CUSTODIAN), A.CUSTODIAN) \"CUSTODIAN\", "
							"DECODE(B.VERIFIED, 'Y', NVL(A.ACCOUNT, DEF_ACCOUNT), A.ACCOUNT) \"ACCOUNT\", B.PRICE, "
							"TO_NUMBER(NULL) \"OPT_BACK\", B.DOWN_PYMNT, A.OTHER_FEES "
							"FROM SEMAM.NW_RAW_TICKET_DETAIL A, SEMAM.NW_RAW_TICKETS B, SEMAM.NW_PORTFOLIOS Z "
							"WHERE B.TICKET_NUM = A.TICKET_NUM "
							"AND B.SIGN IS NOT NULL "
							"AND B.PROCESSED IS NULL "
							"AND Z.PORTFOLIO = A.PORTFOLIO "
							"AND A.TICKET_NUM = " + Ticket + 
							" UNION " 
							"SELECT A.TICKET_NUM+.2, ROWIDTOCHAR(A.ROWID), TO_NUMBER(NULL), A.PORTFOLIO, A.NOM_AMOUNT, A.CUSTODIAN, "
							"A.ACCOUNT, TO_NUMBER(NULL) \"PRICE\", TO_NUMBER(NULL) \"OPT_BACK\", B.DOWN_PYMNT, A.OTHER_FEES "
							"FROM SEMAM.NW_RAW_TICKET_DETAIL A, SEMAM.NW_RAW_TICKETS B "
							"WHERE B.TICKET_NUM = A.TICKET_NUM "
							"AND B.SIGN IS NOT NULL "
							"AND B.PROCESSED IS NULL "
							"AND B.TRANS_TYPE = 'INT. SWAP' "
							"AND A.TICKET_NUM = " + Ticket + " ORDER BY 1, 4 ");
	else
		GetOraLoader().Open("SELECT A.TICKET_NUM, ROWIDTOCHAR(A.ROWID) \"ID\", TO_NUMBER(NULL) \"TRANS_NUM\", A.PORTFOLIO, "
							"A.NOM_AMOUNT, DECODE(B.VERIFIED, 'Y', NVL(A.CUSTODIAN, DEF_CUSTODIAN), A.CUSTODIAN) \"CUSTODIAN\", "
							"DECODE(B.VERIFIED, 'Y', NVL(A.ACCOUNT, DEF_ACCOUNT), A.ACCOUNT) \"ACCOUNT\", B.PRICE, "
							"C.TRANS_NUM \"OPT_BACK\", B.DOWN_PYMNT, A.OTHER_FEES "
							"FROM SEMAM.NW_RAW_TICKET_DETAIL A "
							"JOIN SEMAM.NW_RAW_TICKETS B ON (A.TICKET_NUM = B.TICKET_NUM "
														"AND B.SIGN IS NOT NULL AND B.PROCESSED IS NULL) "
							"LEFT OUTER JOIN SEMAM.NW_TR_TICKETS C ON (A.PORTFOLIO = C.PORTFOLIO "
														"AND TO_CHAR(B.UNWIND_TICKET) = C.TICKET_NUM "
														"AND B.UNWIND_TICKET = " + OptTicket + ") "
							"JOIN SEMAM.NW_PORTFOLIOS Z ON (Z.PORTFOLIO = A.PORTFOLIO) "
							"WHERE A.TICKET_NUM = " + Ticket + 
							" UNION " 
							"SELECT A.TICKET_NUM+.2, ROWIDTOCHAR(A.ROWID), TO_NUMBER(NULL), A.PORTFOLIO, A.NOM_AMOUNT, A.CUSTODIAN, "
							"A.ACCOUNT, B.PRICE, TO_NUMBER(NULL) \"OPT_BACK\", B.DOWN_PYMNT, A.OTHER_FEES "
							"FROM SEMAM.NW_RAW_TICKET_DETAIL A, SEMAM.NW_RAW_TICKETS B "
							"WHERE B.TICKET_NUM = A.TICKET_NUM "
							"AND B.SIGN IS NOT NULL "
							"AND B.PROCESSED IS NULL "
							"AND B.TRANS_TYPE = 'INT. SWAP' "
							"AND A.TICKET_NUM = " + Ticket + " ORDER BY 1, 4 ");

	GetOraLoader().GetFieldArray().GetAt(0)->SetWithComma(FALSE); // TicketNum
	GetOraLoader().GetFieldArray().GetAt(2)->SetWithComma(FALSE); // TransNum
	GetOraLoader().GetFieldArray().GetAt(7)->SetWithComma(FALSE); // Price
	GetOraLoader().GetFieldArray().GetAt(8)->SetWithComma(FALSE); // OptBacking
	GetOraLoader().CopyFieldArray(GetSRowCtrl().GetSS()->GetFieldArray());
	GetSRowCtrl().GetSS()->GetFieldArray().RemoveAt(0); // Remove Key
	GetSRowCtrl().GetSS()->GetFieldArray().RemoveAt(1); // Remove ID
	GetSRowCtrl().GetSS()->SetSheetColHeaders();
	return GetOraLoader().LoadKeyDBList(GetAllocList());
}

BOOL CProcessData::DeleteDataRow()
{
	for(int i = 1; i <= GetSRowCtrl().GetSS()->GetSheetRows(); i ++)
	{
		GetSRowCtrl().SetCurrentRow(i);
		if(!GetTicket().GetTransNum().IsEmpty())
			if(!GetTicket().DeleteRec(GetOraLoader()))
				return FALSE;
	}

	return CEntryData::DeleteDataRow();
}

void CProcessData::UpdateSheetRow(BOOL bFromSheet)
{
	CMSRowCtrl::UpdateSheetRow(bFromSheet);	
}

BOOL CProcessData::UpdateDataRow()
{
	BOOL bInDatabase, bFloat = FALSE;
	int i;
	
	if(IsTicketProcessed())
	{
		MessageBox(NULL, "This ticket has been processed.  Please reload tickets.", GetRawTicket().GetTicket(), MB_OK);
		return TRUE;
	}
	
	if(strlen(GetKey()) == 0)
		return FALSE;

	if(GetRawTicket().GetTransType() == INTSWAP && GetTicket().GetIndex() == "FLOAT")
		bFloat = TRUE;
	
	if(!GetRawTicket().UpdateShortRec(GetOraLoader(), bFloat)) // Update RawTicket
		return FALSE;

	GetRawInv().SetTicket(GetKey());
	if(!GetRawInv().DeleteRec(GetOraLoader())) // Delete RawDetail
		return FALSE;
			
	for(i = 1; i <= GetSRowCtrl().GetSS()->GetSheetRows(); i ++) // Insert RawDetail
	{
		GetSRowCtrl().SetCurrentRow(i);
		if(!GetRawInv().AddRec(GetOraLoader()))
			return FALSE;
	}

	for(i = 1; i <= GetSRowCtrl().GetSS()->GetSheetRows(); i ++) // Generate Ticket
	{
		GetSRowCtrl().SetCurrentRow(i);
		bInDatabase = !GetTicket().GetTransNum().IsEmpty();
		GenerateTicketRec(GetRawInv());

		if(bInDatabase)
		{
			if(!GetTicket().UpdateRec(GetOraLoader())) // Update Ticket
				return FALSE;

			GetInv().SetTransNum(GetTicket().GetTransNum());
			if(!GetInv().DeleteRec(GetOraLoader()))  // Delete Inventory
				return FALSE;
			if(!GetTicket().GenerateInv(GetOraLoader())) // Regenerate Inventory
				return FALSE;
		}
		else
		{
			if(!GetTicket().AddRec(GetOraLoader())) // Add Ticket and Inventory
				return FALSE;			
			GetSRowCtrl().GetSS()->SetSheetText(1, i, GetTicket().GetTransNum());
		}
	}

	UpdateSheetRow(FALSE);
	DeleteAllocListItem();
	AddAllocListItem();
	return TRUE;
}

BOOL CProcessData::IsTicketProcessed()
{
	UpdateData(&GetRawTicket(), &GetRawInv());

	if(GetRawTicket().GetTransType() == INTSWAP)
		return FALSE;

	return GetRawTicket().IsTicketProcessed(GetOraLoader());
}

BOOL CProcessData::UpdateFxRef(CString TransNum, CString Portfolio)
{
	if(TransNum.IsEmpty() || Portfolio.IsEmpty())
		return TRUE;
	CQData QData;

	for(int i = 0; i < GetFxRecArray().GetSize(); i++)
	{
		if(Portfolio == GetFxRecArray().GetAt(i)->GetAt(1))
		{
			GetOraLoader().GetSql().Format("UPDATE SEMAM.NW_TR_INV A SET FOREX_REF_NUM = %s "
											"WHERE INV_NUM = %s ", (LPCTSTR) TransNum,
											QData.RemoveComma(GetFxRecArray().GetAt(i)->GetAt(0)));
			GetOraLoader().ExecuteSql();
		}
	}

	return TRUE;
}

void CProcessData::SetupAssetInfo()
{
	double Fxrate, LevRate, Amount, Price;
	CString Date;
	CQData QData;
	
	UpdateData(&GetRawTicket(), &GetRawInv());
	Amount = atof(QData.RemoveComma(GetRawTicket().GetNomAmount()));
	Price = atof(QData.RemoveComma(GetRawTicket().GetPrice()));
	if(GetRawTicket().GetFxRate().IsEmpty())
		Fxrate = 1;
	else
		Fxrate = atof(QData.RemoveComma(GetRawTicket().GetFxRate()));
	
	LevRate = atof(QData.RemoveComma(GetRawTicket().GetRate()));
	
	Date = GetRawTicket().GetMaturity().IsEmpty() ? GetRawTicket().GetValueDate() : GetRawTicket().GetMaturity();
	if(!Date.IsEmpty())
		m_Val.Setup(GetOraLoader(), GetRawTicket().GetTransType(), GetRawTicket().GetDir(), 
					GetRawTicket().GetAsset(), GetRawTicket().GetValueDate(), Date, 
					Amount, Price, Fxrate, GetRawTicket().GetRateBasis(), LevRate, 
					GetTicket().GetFormula());
}

double CProcessData::GetParValue()
{
	CString sPar;
	CQData QData;

	if(GetRawTicket().GetAsset().IsEmpty())
		return 0;

	GetOraLoader().GetSql() = "SELECT ASS_PAR_VALUE FROM SEMAM.NW_ASSETS "
							"WHERE ASS_CODE = ";
	GetOraLoader().GetSql() += QData.GetQueryText(GetRawTicket().GetAsset());
	GetOraLoader().Open();
	GetOraLoader().LoadTextString(sPar);
	if(sPar.IsEmpty())
	{
		MessageBox(NULL, "Par Value is missing in FMS.  1 is used ", NULL, MB_OK);
		return 1;
	}

	QData.RemoveComma(sPar);
	return atof(sPar);
}

void CProcessData::ComputeValue(BOOL bSec, BOOL bOrFee)
{
	double Fxrate, Price, BrokerFee;
	CString Buf;
	CQData QData;

	UpdateData(&GetRawTicket(), &GetRawInv());
	Price = atof(QData.RemoveComma(GetRawTicket().GetPrice()));
	BrokerFee = atof(QData.RemoveComma(GetRawTicket().GetBrokerFee()));
	SetOtherFee(0);
//	SetOtherFee(atof(QData.RemoveComma(GetRawTicket().GetOtherFee())));

//	if(GetOtherFee() <= 0.001)
//		SetOtherFee(ComputeOtherFee(Price, bSec, bOrFee));

	if(BrokerFee != 0 || GetOtherFee() != 0)
		SetNetPrice(m_Val.GetNetPrice(Price, BrokerFee, GetOtherFee()));
	else
		SetNetPrice(Price);

	Fxrate = m_Val.GetFxRate();
	if(strcmp(m_Val.GetType(), REPO) == 0 || strcmp(m_Val.GetType(), LEVERAGE) == 0)
	{
		m_Val.SetPrice(GetNetPrice());
		m_Val.ComputeLevAmount(TRUE);
		if(strcmp(m_Val.GetType(), LEVERAGE) == 0)
			SetCash(m_Val.GetNomAmount()/Fxrate);
		else
			SetCash(m_Val.GetLevAmount()/Fxrate);

		SetAccretion(0);
		if(strlen(GetTicket().GetMaturity()) == 0)
			SetPrePaid(0);
		else
			SetPrePaid(m_Val.GetLevInterest()/Fxrate);
	}
	else
	{
		m_Val.SetPrice(GetNetPrice());
		SetCash(m_Val.GetValue()/Fxrate);
		SetPrePaid(m_Val.GetPrePaidInt()/Fxrate);
		SetAccretion(m_Val.GetPrePaidAccretionValue()/Fxrate);
	}
}

double CProcessData::ComputeOtherFee(double Price, BOOL bSec, BOOL bOrFee)
{
	double Par, OtherFee = 0, Contracts;
	CQData QData;
	CString PB, Text;

	if(bSec)
		OtherFee = m_Val.GetSecFees(Price, bSec);

	if(bOrFee && (m_Val.GetType() == CALL || m_Val.GetType() == PUT))
	{
		Par = GetParValue();
		PB = GetRawTicket().GetAssignCP();
		if(PB.IsEmpty())
			PB = GetCustodian();
		
		if(Par > 0)
			Contracts = atof(QData.RemoveComma(GetRawTicket().GetNomAmount()))/Par;
		else
			Contracts = atof(QData.RemoveComma(GetRawTicket().GetNomAmount()));
		
		OtherFee += m_Val.GetOrFees(PB, GetRawTicket().GetCP(), Contracts);
	}

	return OtherFee;
}