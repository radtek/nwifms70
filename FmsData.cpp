#include "stdafx.h"
#include "NWIFMS70.h"
#include <math.h>
#include "fmsdata.h"
#include "qdata.h"


IMPLEMENT_DYNAMIC(CFmsTicketData, CMSRowCtrl)
void CFmsTicketData::Setup(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS, LPCTSTR User, int KeyIndex)
{
	CMSRowCtrl::Setup(OraLoader, pSS, pSSS, KeyIndex);
	m_bExact = FALSE;
}

BOOL CFmsTicketData::IsReady()
{
	if(!CMSRowCtrl::IsReady())
		return FALSE;
	
	return TRUE;
}

BOOL CFmsTicketData::GetSelInvRec(int Index, CInvRec &InvRec)
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
				InvRec.SetForexRef(Text);
				break;
			case 10:
				InvRec.SetCrossRate(Text);
				break;
			case 11:
				InvRec.SetCash(Text);
				break;
			case 12:
				InvRec.SetInterest(Text);
				break;
			case 13:
				InvRec.SetAccount(Text);
				break;
			case 14:
				InvRec.SetAssignCP(Text);
				break;
			case 15:
				InvRec.SetAssignCT(Text);
				break;
			default:
				break;
		}
	}

	return TRUE;
}

int CFmsTicketData::LoadTickets()
{
	CDBRec Rec;
	CString Sql;
	CStringArray QRec;

	if(!IsReady())
		return -1;

	UpdateData(&GetTicket(), &GetInv());
	
	ToDBRec(Rec, TRUE);
	Rec.RemoveAt(Rec.GetSize() - 1); // Remove Class
	if(Rec.GetSize() <= 0)
		return -1;

	Sql = "SELECT TRANS_NUM, PORTFOLIO, TICKET_NUM, COUNTERPARTY, ASSET_CODE, TRADER_INI, TRADE_DATE, "
			"VALUE_DATE, TRANS_TYPE, TRANS_DIRECTION, NOM_AMOUNT, PRICE, CURRENCY, FXRATE, CON_CODE, "
			"DEAL_TYPE, EXERCISE_PRICE, EXERCISE_DATE, OPT_EXPIRATION, FX_DATE, OPT_TICK, OPT_SET_CODE, "
			"OPT_SET_CODE2, SET_CONVENTION, OPT_ID, EURO_OPT, OPT_AUTO, BR_FEES, A.SOFTDOLLAR, LEV_RATE, "
			"VAR, TR_RATE, RATE_BASIS, MATURITY_DATE, FLOAT_RATE_INDEX, FLOAT_RATE_FORMULA, "
			"ORIG_SW_BOOKING, ORIG_SW_MATURITY, FX_ACCNUM, USD_ACCNUM, PFU, NEW_ISSUE, CUSTODIAN, "
			"OPTION_BACKING, TRANS_SEQ, HAN_DETAIL1, HAN_DETAIL2, CP_TRADE_ID, DOC_REQ, DOC_RECVD, WI, "
			"CONFIRM, SWAP_TICKET, DELIVERY_DATE, MARGIN, MARGIN_CURRENCY, MARGIN_AMOUNT, LISTED, FUNDED_SWAP, "
			"BINARY, DV01, BEST_EXECUTION, SHORT_SALE, A.CSPB_SHORT, CANCEL_TICKET, CORRECT_TICKET, "
			"SEC_FEE, OR_FEE, ETRADE, BOOKER, TO_CHAR(BOOK_DATE, 'MM/DD/YYYY HH24:MI:SS'), "
			"TR_VERSION, ASS_DESC, ASS_CURRENCY, ASS_INDUSTRY "
			"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_ASSETS B "
			"WHERE B.ASS_CODE(+) = A.ASSET_CODE ";
	
	GetSRowCtrl().GetSS()->ClearSheet();
	if(GetSS()->GetFieldArray().GetSize() <= 0)
	{
		GetOraLoader().GetSql() = Sql;
		GetOraLoader().GetSql() += " 1 = 2";
		GetOraLoader().Open();
		GetOraLoader().CopyFieldArray(GetSS()->GetFieldArray());
	}

	if(IsExact())
		GetOraLoader().RecToQueryRec(QRec, Rec, &GetSS()->GetFieldArray());
	else
		GetOraLoader().RecToQueryRec(QRec, Rec, &GetSS()->GetFieldArray(), PERCENT);

	GetOraLoader().GetSql() = Sql;
	GetOraLoader().SetSqlSearchInfo(GetOraLoader().GetSql(), QRec, &GetSS()->GetFieldArray(), TRUE, IsExact());

	GetOraLoader().GetSql() += " ORDER BY 2,1 ";
	
	if(GetOraLoader().Open())
		return GetOraLoader().LoadRowCtrl(*this, TRUE, TRUE);

	return -1;
}

void CFmsTicketData::LoadSheet()
{
	if(!IsReady())
		return;

	GetOraLoader().GetSql().Format("SELECT INV_NUM, ASSET_CODE, TR_DESC, NOM_AMOUNT, "
					"ACTUAL_VDATE, ACTUAL_CLOSING, POST_DATE, CURRENCY, FOREX_REF_NUM, "
					"CROSS_RATE, CASH_AMOUNT, INT_AMOUNT, ACC_CODE, ASSIGN_CP, ASSIGN_CT "
					"FROM SEMAM.NW_TR_INV WHERE TRANS_NUM = %s ", (LPCTSTR) GetKey());
	if(GetOraLoader().Open())
		GetOraLoader().LoadRowCtrl(GetSRowCtrl(), TRUE);
}

void CFmsTicketData::SetHeaders()
{
	GetOraLoader().GetSql() = "SELECT TRANS_NUM, PORTFOLIO, TICKET_NUM, COUNTERPARTY, ASSET_CODE, "
								"TRADER_INI, TRADE_DATE, VALUE_DATE, TRANS_TYPE, TRANS_DIRECTION, "
								"NOM_AMOUNT, PRICE, CURRENCY, FXRATE, CON_CODE, DEAL_TYPE, "
								"EXERCISE_PRICE, EXERCISE_DATE, OPT_EXPIRATION, FX_DATE, OPT_TICK, "
								"OPT_SET_CODE, OPT_SET_CODE2, SET_CONVENTION, OPT_ID, EURO_OPT, "
								"OPT_AUTO, BR_FEES, A.SOFTDOLLAR, LEV_RATE, VAR, TR_RATE, "
								"RATE_BASIS, MATURITY_DATE, FLOAT_RATE_INDEX, FLOAT_RATE_FORMULA, "
								"ORIG_SW_BOOKING, ORIG_SW_MATURITY, FX_ACCNUM, USD_ACCNUM, PFU, "
								"NEW_ISSUE, CUSTODIAN, OPTION_BACKING, TRANS_SEQ, HAN_DETAIL1, "
								"HAN_DETAIL2, CP_TRADE_ID, DOC_REQ, DOC_RECVD, WI, CONFIRM, "
								"SWAP_TICKET, DELIVERY_DATE, MARGIN, MARGIN_CURRENCY, MARGIN_AMOUNT, "
								"LISTED, FUNDED_SWAP, BINARY, DV01, BEST_EXECUTION, SHORT_SALE, "
								"A.CSPB_SHORT, CANCEL_TICKET, CORRECT_TICKET, SEC_FEE, OR_FEE, "
								"ETRADE, BOOKER, BOOK_DATE, TR_VERSION, ASS_DESC, ASS_CURRENCY, "
								"ASS_INDUSTRY "
								"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_ASSETS B "
								"WHERE B.ASS_CODE(+) = A.ASSET_CODE ";

	GetOraLoader().GetSql() += "AND 1 = 2";
	GetOraLoader().Open();
	GetOraLoader().CopyFieldArray(GetSS()->GetFieldArray());
	GetSS()->SetSheetColHeaders();

	GetOraLoader().GetSql() = "SELECT INV_NUM, ASSET_CODE, TR_DESC, NOM_AMOUNT, ACTUAL_VDATE, "
								"ACTUAL_CLOSING, POST_DATE, CURRENCY, FOREX_REF_NUM, CROSS_RATE, "
								"CASH_AMOUNT, INT_AMOUNT, ACC_CODE, ASSIGN_CP, ASSIGN_CT "
								"FROM SEMAM.NW_TR_INV "
								"WHERE TRANS_NUM = 1 ";
	GetOraLoader().Open();
	GetOraLoader().CopyFieldArray(GetSRowCtrl().GetSS()->GetFieldArray());
	GetSRowCtrl().GetSS()->GetFieldArray().RemoveAt(0);
	GetSRowCtrl().GetSS()->SetSheetColHeaders();
}

BOOL CFmsTicketData::AddRow()
{
	BOOL bNew;
	
	if(!IsReady())
		return FALSE;

	bNew = strlen(GetKey()) == 0 ? TRUE : FALSE;
	UpdateData(&GetTicket(), &GetInv());
	
	if(bNew)
	{
		GetOraLoader().BeginTransaction();
		if(GetTicket().AddRec(GetOraLoader()))
		{
			GetOraLoader().Commit(); // Successfully Updated
			AddSheetRow();
			LoadSheet();
			SetModify(FALSE);
			return TRUE;
		}

		GetOraLoader().Rollback();
		return FALSE;
	}
	else
		return AddSRow(); // Add Inventory
}

BOOL CFmsTicketData::DeleteRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData(&GetTicket(), &GetInv());
	if(strlen(GetKey()) == 0)
		return FALSE;

	if(GetCurrentRow() > 0 && GetSS()->GetIsBlockSelected())
	{
		GetInv().SetTransNum(GetKey());

		GetOraLoader().BeginTransaction();
		if(!GetInv().DeleteRec(GetOraLoader()))
		{
			GetOraLoader().Rollback();
			return FALSE;
		}

		if(GetTicket().DeleteRec(GetOraLoader()))
		{
			GetOraLoader().Commit();
			DeleteSheetRow(GetCurrentRow());
			SetCurrentRow(-1);
			Refresh();
			SetModify(FALSE);
			return TRUE;
		}
			
		GetOraLoader().Rollback();
		return FALSE;
	}
	else
		return DeleteSRow();
}

BOOL CFmsTicketData::UpdateRow()
{
	if(!IsReady())
		return FALSE;
	
	if(strlen(GetKey()) == 0)
		return FALSE;

	UpdateData(&GetTicket(), &GetInv());

	UpdateSRow();
	if(GetModify(FALSE))
	{
		SetModify(FALSE);
		GetOraLoader().BeginTransaction();
		if(GetTicket().UpdateRec(GetOraLoader())) // Successfully Updated
		{
			GetOraLoader().Commit();
			UpdateSheetRow(FALSE);			
			return TRUE;
		}
		else
		{
			GetOraLoader().Rollback();
			return FALSE;
		}
	}
	
	return TRUE;
}

BOOL CFmsTicketData::AddSRow()
{
	if(!IsReady())
		return FALSE;

	if(strlen(GetKey()) == 0)
		return FALSE;

	GetInv().SetTransNum(GetKey());
	GetOraLoader().BeginTransaction();
	if(GetInv().AddRec(GetOraLoader()))
	{
		GetOraLoader().Commit();
		LoadSheet();
		return TRUE;
	}
	
	GetOraLoader().Rollback();
	return FALSE;
}

BOOL CFmsTicketData::DeleteSRow(int CurrentRow)
{
	if(!IsReady())
		return FALSE;

	if(GetSRowCtrl().GetCurrentRow() > 0 && GetSRowCtrl().GetSS()->GetIsBlockSelected())
	{
		if(strlen(GetKey()) == 0 || strlen(GetInv().GetID()) == 0)
			return FALSE;

		GetInv().SetTransNum(GetKey());

		GetOraLoader().BeginTransaction();
		if(GetInv().DeleteRec(GetOraLoader(), TRUE))
		{
			GetOraLoader().Commit();
			GetSRowCtrl().DeleteSheetRow(CurrentRow);
			LoadSheet();
			GetSRowCtrl().SetCurrentRow(-1);
			GetSRowCtrl().Refresh();
			return TRUE;
		}
				
		GetOraLoader().Rollback();
		return FALSE;
	}

	return TRUE;
}

BOOL CFmsTicketData::UpdateSRow()
{
	if(!IsReady())
		return FALSE;

	if(strlen(GetKey()) == 0 || strlen(GetInv().GetID()) == 0)
		return FALSE;

	if(GetSRowCtrl().GetModify(FALSE))
	{
		GetInv().SetTransNum(GetKey());

		GetOraLoader().BeginTransaction();
		if(GetInv().UpdateRec(GetOraLoader()))
		{
			GetOraLoader().Commit();
			GetSRowCtrl().UpdateSheetRow(FALSE);
			GetSRowCtrl().SetModify(TRUE);
			return TRUE;
		}
		
		GetOraLoader().Rollback();
		return FALSE;
	}

	return TRUE;
}

void CFmsTicketData::SetupAssetInfo(CAssetVal &Val)
{		
	CQData QData;
	CString Date, Fx;

	if(GetTicket().GetTransType() == REPO || GetTicket().GetTransType() == LEVERAGE || GetTicket().GetTransType() == INTSWAP)
	{
		if(GetInv().GetTrDesc() == BOOKING)
			Date = GetTicket().GetMaturity();
		else
			if(GetInv().GetValueDate().IsEmpty())
				Date = GetTicket().GetMaturity();
			else
				Date = GetInv().GetValueDate();
	}
	else
		Date = GetTicket().GetValueDate();

	if(GetInv().GetCrossRate().GetLength() > 0)
	{
		if(GetTicket().GetFxRate().GetLength() > 0)
			Fx = QData.WriteNumber(atof(GetTicket().GetFxRate())*atof(GetInv().GetCrossRate()), FALSE);
		else
			Fx = GetInv().GetCrossRate();
	}
	else
		Fx = GetTicket().GetFxRate();

	Val.Setup(GetOraLoader(), GetTicket().GetTransType(), GetTicket().GetDir(), 
			GetInv().GetAsset(), GetTicket().GetValueDate(), Date, GetInv().GetNomAmount(), 
			GetTicket().GetPrice(), Fx, GetTicket().GetRateBasis(), GetTicket().GetRate(), 
			GetTicket().GetFormula());
}

void CFmsTicketData::Compute()
{
	CAssetVal Val;

	SetupAssetInfo(Val);

	if(Val.GetFxRate() != 1)
		m_RecCurrency = "USD";
	else
		m_RecCurrency = GetTicket().GetCurrency();

	if(strcmp(Val.GetType(), REPO) == 0 || strcmp(Val.GetType(), LEVERAGE) == 0 ||
		strcmp(Val.GetType(), BORROW) == 0 || strcmp(Val.GetType(), LEND) == 0 || 
		strcmp(Val.GetType(), INTSWAP) == 0)
	{
		if(strcmp(Val.GetType(), LEVERAGE) == 0)
			SetCash(Val.GetNomAmount()/Val.GetFxRate());
		else 
			if(strcmp(Val.GetType(), INTSWAP) == 0)
				SetCash(0);
			else
				SetCash(Val.GetLevAmount()/Val.GetFxRate());

		SetAccretion(0);
		SetPrePaid(0);
		if(strcmp(GetInv().GetTrDesc(), BOOKING) == 0 || 
			(strcmp(GetInv().GetTrDesc(), MATURITY) == 0 && 
			 GetInv().GetValueDate().IsEmpty() && GetTicket().GetMaturity().IsEmpty()))
			SetPrePaid(0);
		else
		{
			if(strcmp(Val.GetType(), INTSWAP)) // if other than interest rate swap
			{
				if(strlen(Val.GetFormula()) > 0)
					SetPrePaid(Val.GetDailyLevInterest(GetOraLoader(), GetTicket().GetPortfolio(), 
								GetTicket().GetTransNum(), GetInvNum())/Val.GetFxRate());
				else
					SetPrePaid(Val.GetLevInterest()/Val.GetFxRate());
			}
		}
	}
	else
	{
		if(strcmp(Val.GetType(), FOREX) == 0 && strcmp(GetInv().GetTrDesc(), FXBKNG) == 0)
			SetCash(Val.GetNomAmount());
		else
			SetCash(Val.GetValue()/Val.GetFxRate());

		SetPrePaid(Val.GetPrePaidInt()/Val.GetFxRate());
		SetAccretion(Val.GetPrePaidAccretionValue()/Val.GetFxRate());
	}
}

BOOL CFmsTicketData::CancelCorrect(const CString Ticket, const CString TransNum, CString &CanTicket, CString &CorrTicket)
{
	CQData QData;
	double Nominal, MarginAmt, Comm, OtherFee, DV01, VAR;
	CString sTicket, sTransNum;
	// GET TRANS_NUM AND SUM

	sTicket = QData.GetQueryNumber(Ticket);
	sTransNum = QData.GetQueryNumber(TransNum);

	if(GetOraLoader().GetCount("SELECT COUNT(*) FROM SEMAM.NW_TR_TICKETS A "
								"WHERE NVL(A.CANCEL_TICKET, CORRECT_TICKET) = " + sTicket) > 0)
	{
		MessageBox(NULL, "Cannot processed because select the ticket has been cancel/corrected", "Transaction Processing", MB_OK);
		return FALSE;
	}
	
	if(GetOraLoader().GetCount("SELECT COUNT(*) FROM SEMAM.NW_RAW_TICKETS A "
						"WHERE NVL(A.CANCEL_TICKET, CORRECT_TICKET) = " + sTicket) > 0)
	{
		MessageBox(NULL, "Cannot processed because select the ticket has been cancel/corrected", "Transaction Processing", MB_OK);
		return FALSE;
	}

	if(!GetOraLoader().Open("SELECT SUM(NOM_AMOUNT), SUM(MARGIN_AMOUNT), SUM(NVL(BR_FEES, 0)), SUM(NVL(LEV_RATE, 0)), "
							"SUM(NVL(DV01, 0)), SUM(NVL(VAR, 0)) "
							"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_PORTFOLIOS Z "
							"WHERE Z.PORTFOLIO = A.PORTFOLIO "
							"AND Z.STATUS IS NULL "
							"AND A.TICKET_NUM = " + sTicket + 
							"AND A.DEAL_TYPE IS NULL "
							"AND A.TRANS_TYPE NOT IN ('INT. SWAP') "
							"GROUP BY TICKET_NUM "
							"UNION "
							"SELECT SUM(NOM_AMOUNT), SUM(MARGIN_AMOUNT), SUM(NVL(BR_FEES, 0)), SUM(NVL(LEV_RATE, 0)), "
							"SUM(NVL(DV01, 0)), SUM(NVL(VAR, 0)) "
							"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_ASSETS B, SEMAM.NW_PORTFOLIOS Z "
							"WHERE B.ASS_CODE = A.ASSET_CODE "
							"AND B.ASS_INT_TYPE = 'FIXED' "
							"AND Z.PORTFOLIO = A.PORTFOLIO "
							"AND Z.STATUS IS NULL "
							"AND A.TICKET_NUM = " + sTicket +
							"AND A.DEAL_TYPE IS NULL "
							"AND A.TRANS_TYPE IN ('INT. SWAP') "
							"GROUP BY TICKET_NUM "
							"UNION "
							"SELECT SUM(NOM_AMOUNT), SUM(MARGIN_AMOUNT), SUM(NVL(BR_FEES, 0)), SUM(NVL(LEV_RATE, 0)), "
							"SUM(NVL(DV01, 0)), SUM(NVL(VAR, 0)) "
							"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_PORTFOLIOS Z "
							"WHERE Z.PORTFOLIO = A.PORTFOLIO "
							"AND Z.STATUS IS NULL "
							"AND A.TICKET_NUM = " + sTicket + 
							"AND A.DEAL_TYPE IS NOT NULL "
							"AND A.TRANS_TYPE NOT IN ('SECURITIES') "
							"GROUP BY TICKET_NUM "))
			return FALSE;

	GetOraLoader().MoveFirst();
	while(!GetOraLoader().IsEOF())
	{
		GetOraLoader().GetFieldValue(0, &Nominal);
		GetOraLoader().GetFieldValue(1, &MarginAmt);
		GetOraLoader().GetFieldValue(2, &Comm);
		GetOraLoader().GetFieldValue(3, &OtherFee);
		GetOraLoader().GetFieldValue(4, &DV01);
		GetOraLoader().GetFieldValue(5, &VAR);
		GetOraLoader().MoveNext();
	}

	// CORRECTION 
	GetOraLoader().GenerateUniqueID(TICKETNUM, CanTicket);
	CanTicket = QData.RemoveComma(CanTicket);

	GetOraLoader().GetSql().Format("INSERT INTO SEMAM.NW_RAW_TICKETS (TICKET_NUM, TRADER_INI, TRADE_DATE, VALUE_DATE, DIR, TRANS_TYPE, "
									"NOM_AMOUNT, STRIKE, OPT_EXPIRATION, OTC, CURRENCY, FXRATE, COUNTERPARTY, BROKER, ASSET_CODE, "
									"ASSET_DESC, ASSET_COUNTRY, ASSET_CURRENCY, ASSET_CLASS, ASSET_CATEGORY, ASSET_BUCKET, ASSET_IND_AREA, "
									"ASSET_MATURITY, ASSET_COMMON_CODE, ASSET_MSTC_CODE, ASSET_ISIN_CODE, ASSET_SEDOL_NUM, IPO, TRS, "
									"REPO_CP, TR_RATE, RATE_BASIS, MATURITY, SW_BOOKING, SW_MATURITY, FLOAT_RATE, FLOAT_RATE_BASIS, "
									"FLOAT_MATURITY, NOTE, NOTE2, SOURCE, OPT_TICK, EURO_OPT, WI, FX_DATE, ASSET_PID, MARGIN, "
									"MARGIN_CURRENCY, MARGIN_AMOUNT, FUNDED_SWAP, BINARY, OPT_SET_CODE, OPT_SET_CODE2, OPT_AUTO, "
									"DELIVERY_DATE, SET_CONVENTION, BEST_EXECUTION, SHORT_SALE, PRICE, DOWN_PYMNT, BROKER_FEE, OTHER_FEE, "
									"ASSIGN_CP, DV01, VAR, SOFTDOLLAR, CANCEL_TICKET, CORRECT_TICKET, SEC_FEE, OR_FEE, EXEC_TIME, ETRADE, "
									"CSPB_SHORT, SWAP_TICKET, TRADER_SIGN, TRADER_SIGN_DATE) "
									"SELECT " + CanTicket + ", A.TRADER_INI, TRUNC(SYSDATE), A.VALUE_DATE, "
									"DECODE(A.DIR, 'P', 'S', 'S', 'P', 'B', 'L', 'L', 'B'), A.TRANS_TYPE, %.2lf, A.EXERCISE_PRICE, "
									"A.OPT_EXPIRATION, NULL, A.CURRENCY, A.FXRATE, A.COUNTERPARTY, A.BROKER, A.ASSET_CODE, ASS_DESC, "
									"ASS_COUNTRY, ASS_CURRENCY, ASS_INDUSTRY, ASS_CATEGORY, BUCKET, ASS_IND_AREA, ASS_MATURITY_DATE, "
									"ASS_COMMON_CODE, ASS_MSTC_CODE, ASS_ISIN_CODE, ASS_SEDOL_NUM, A.NEW_ISSUE, A.DEAL_TYPE, NULL, "
									"A.TR_RATE, A.RATE_BASIS, A.MATURITY_DATE, A.ORIG_SW_BOOKING, A.ORIG_SW_MATURITY, TO_NUMBER(NULL), "
									"NULL, TO_DATE(NULL), A.HAN_DETAIL1, A.HAN_DETAIL2, B.SOURCE, A.OPT_TICK, A.EURO_OPT, A.WI, "
									"ASS_FX_DATE, NVL(A.OPT_ID, ASS_PID), A.MARGIN, A.MARGIN_CURRENCY, 0, A.FUNDED_SWAP, A.BINARY, "
									"A.OPT_SET_CODE, A.OPT_SET_CODE2, A.OPT_AUTO, A.DELIVERY_DATE, A.SET_CONVENTION, A.BEST_EXECUTION, "
									"A.SHORT_SALE, A.PRICE, 100, 0, 0, A.ASSIGN_CP, 0, 0, TO_NUMBER(NULL), A.TICKET_NUM, TO_NUMBER(NULL), "
									"A.SEC_FEE, A.OR_FEE, D.EXEC_TIME, A.ETRADE, A.CSPB_SHORT, A.SWAP_TICKET, D.TRADER_SIGN, D.TRADER_SIGN_DATE "
									"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B, SEMAM.NW_RAW_TICKETS D "
									"WHERE B.ASS_CODE(+) = A.ASSET_CODE "
									"AND D.TICKET_NUM(+) = A.TICKET_NUM "
									"AND A.TR_DESC = 'BOOKING' "
									"AND A.TRANS_NUM = " + sTransNum + 
									" AND A.TRANS_TYPE NOT IN ('INT. SWAP') "
									"AND A.DEAL_TYPE IS NULL "
									"UNION "
									"SELECT " + CanTicket + ", A.TRADER_INI, TRUNC(SYSDATE), A.VALUE_DATE, "
									"DECODE(A.DIR, 'P', 'S', 'S', 'P', 'B', 'L', 'L', 'B'), A.TRANS_TYPE, %.2lf, "
									"A.EXERCISE_PRICE, A.OPT_EXPIRATION, NULL, A.CURRENCY, A.FXRATE, A.COUNTERPARTY, A.BROKER, A.ASSET_CODE, "
									"ASS_DESC, ASS_COUNTRY, ASS_CURRENCY, ASS_INDUSTRY, ASS_CATEGORY, BUCKET, ASS_IND_AREA, ASS_MATURITY_DATE, "
									"ASS_COMMON_CODE, ASS_MSTC_CODE, ASS_ISIN_CODE, ASS_SEDOL_NUM, A.NEW_ISSUE, A.DEAL_TYPE, NULL, A.TR_RATE, "
									"A.RATE_BASIS, A.MATURITY_DATE, A.ORIG_SW_BOOKING, A.ORIG_SW_MATURITY, C.TR_RATE, C.RATE_BASIS, "
									"C.MATURITY_DATE, A.HAN_DETAIL1, A.HAN_DETAIL2, B.SOURCE, A.OPT_TICK, A.EURO_OPT, A.WI, B.ASS_FX_DATE, "
									"NVL(A.OPT_ID, B.ASS_PID), A.MARGIN, A.MARGIN_CURRENCY, 0, A.FUNDED_SWAP, A.BINARY, A.OPT_SET_CODE, "
									"A.OPT_SET_CODE2, A.OPT_AUTO, A.DELIVERY_DATE, A.SET_CONVENTION, A.BEST_EXECUTION, A.SHORT_SALE, A.PRICE, "
									"100, 0, 0, A.ASSIGN_CP, 0, 0, TO_NUMBER(NULL), A.TICKET_NUM, TO_NUMBER(NULL), A.SEC_FEE, A.OR_FEE, "
									"D.EXEC_TIME, A.ETRADE, A.CSPB_SHORT, A.SWAP_TICKET, D.TRADER_SIGN, D.TRADER_SIGN_DATE "
									"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B, SEMAM.ALL_TICKET_INV_V C, SEMAM.NW_RAW_TICKETS D "
									"WHERE B.ASS_CODE(+) = A.ASSET_CODE "
									"AND C.PORTFOLIO(+) = A.PORTFOLIO "
									"AND C.TRANS_TYPE(+) = A.TRANS_TYPE "
									"AND C.TICKET_NUM(+) = A.TICKET_NUM "
									"AND C.TRADE_DATE(+) = A.TRADE_DATE "
									"AND C.TRANS_NUM(+) != A.TRANS_NUM "
									"AND D.TICKET_NUM(+) = A.TICKET_NUM "
									"AND A.TR_DESC = 'BOOKING' "
									"AND A.TRANS_NUM = " + sTransNum + 
									" AND A.TRANS_TYPE IN ('INT. SWAP') "
									"AND A.DEAL_TYPE IS NULL "
									"UNION "
									"SELECT " + CanTicket + ", A.TRADER_INI, TRUNC(SYSDATE), A.VALUE_DATE, "
									"DECODE(A.DIR, 'P', 'S', 'S', 'P', 'B', 'L', 'L', 'B'), A.TRANS_TYPE, %.2lf, A.EXERCISE_PRICE, "
									"A.OPT_EXPIRATION, NULL, A.CURRENCY, A.FXRATE, A.COUNTERPARTY, A.BROKER, A.ASSET_CODE, ASS_DESC, "
									"ASS_COUNTRY, ASS_CURRENCY, ASS_INDUSTRY, ASS_CATEGORY, BUCKET, ASS_IND_AREA, ASS_MATURITY_DATE, "
									"ASS_COMMON_CODE, ASS_MSTC_CODE, ASS_ISIN_CODE, ASS_SEDOL_NUM, A.NEW_ISSUE, A.DEAL_TYPE, "
									"C.COUNTERPARTY, C.TR_RATE, C.RATE_BASIS, C.MATURITY_DATE, A.ORIG_SW_BOOKING, A.ORIG_SW_MATURITY, "
									"TO_NUMBER(NULL), NULL, TO_DATE(NULL), A.HAN_DETAIL1, A.HAN_DETAIL2, B.SOURCE, A.OPT_TICK, "
									"A.EURO_OPT, A.WI, B.ASS_FX_DATE, NVL(A.OPT_ID, B.ASS_PID), A.MARGIN, A.MARGIN_CURRENCY, 0, "
									"A.FUNDED_SWAP, A.BINARY, A.OPT_SET_CODE, A.OPT_SET_CODE2, A.OPT_AUTO, A.DELIVERY_DATE, "
									"A.SET_CONVENTION, A.BEST_EXECUTION, A.SHORT_SALE, A.PRICE, 0, 0, 0, A.ASSIGN_CP, 0, 0, "
									"TO_NUMBER(NULL), A.TICKET_NUM, TO_NUMBER(NULL), A.SEC_FEE, A.OR_FEE, D.EXEC_TIME, A.ETRADE, "
									"A.CSPB_SHORT, A.SWAP_TICKET, D.TRADER_SIGN, D.TRADER_SIGN_DATE "
									"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B, SEMAM.ALL_TICKET_INV_V C, SEMAM.NW_RAW_TICKETS D "
									"WHERE B.ASS_CODE(+) = A.ASSET_CODE "
									"AND C.PORTFOLIO(+) = A.PORTFOLIO "
									"AND C.TICKET_NUM(+) = A.TICKET_NUM "
									"AND C.TRADE_DATE(+) = A.TRADE_DATE "
									"AND C.TRANS_TYPE(+) = 'LEVERAGE' "
									"AND C.TRANS_NUM(+) != A.TRANS_NUM "
									"AND C.PORTFOLIO(+) = A.PORTFOLIO "
									"AND C.TICKET_NUM(+) = A.TICKET_NUM "
									"AND C.TRADE_DATE(+) = A.TRADE_DATE "
									"AND C.TRANS_NUM(+) != A.TRANS_NUM "
									"AND D.TICKET_NUM(+) = A.TICKET_NUM "
									"AND A.TR_DESC = 'BOOKING' "
									"AND A.TRANS_TYPE = 'SECURITIES' "
									"AND A.TRANS_NUM = " + sTransNum + 
									" AND A.DEAL_TYPE IS NOT NULL ", Nominal, Nominal, Nominal);
	if(!GetOraLoader().ExecuteSql(GetOraLoader().GetSql()))
		return FALSE;

	if(!GetOraLoader().ExecuteSql("INSERT INTO SEMAM.NW_RAW_TICKET_DETAIL (TICKET_NUM, PORTFOLIO, NOM_AMOUNT, OPT_BACK, CUSTODIAN, "
									"ACCOUNT, PRICE, DOWN_PYMNT) "
									"SELECT " + CanTicket + ", PORTFOLIO, NOM_AMOUNT, OPTION_BACKING, CUSTODIAN, ACC_CODE, PRICE, 100 "
									"FROM SEMAM.ALL_TICKET_INV_V A "
									"WHERE TICKET_NUM = " + sTicket + 
									"AND TR_DESC = 'BOOKING' "))
		return FALSE;

	// CORRECTION
	GetOraLoader().GenerateUniqueID(TICKETNUM, CorrTicket);
	CorrTicket = QData.RemoveComma(CorrTicket);

	GetOraLoader().GetSql().Format("INSERT INTO SEMAM.NW_RAW_TICKETS (TICKET_NUM, TRADER_INI, TRADE_DATE, VALUE_DATE, DIR, TRANS_TYPE, "
									"NOM_AMOUNT, STRIKE, OPT_EXPIRATION, OTC, CURRENCY, FXRATE, COUNTERPARTY, BROKER, ASSET_CODE, "
									"ASSET_DESC, ASSET_COUNTRY, ASSET_CURRENCY, ASSET_CLASS, ASSET_CATEGORY, ASSET_BUCKET, ASSET_IND_AREA, "
									"ASSET_MATURITY, ASSET_COMMON_CODE, ASSET_MSTC_CODE, ASSET_ISIN_CODE, ASSET_SEDOL_NUM, IPO, TRS, "
									"REPO_CP, TR_RATE, RATE_BASIS, MATURITY, SW_BOOKING, SW_MATURITY, FLOAT_RATE, FLOAT_RATE_BASIS, "
									"FLOAT_MATURITY, NOTE, NOTE2, SOURCE, OPT_TICK, EURO_OPT, WI, FX_DATE, ASSET_PID, MARGIN, "
									"MARGIN_CURRENCY, MARGIN_AMOUNT, FUNDED_SWAP, BINARY, OPT_SET_CODE, OPT_SET_CODE2, OPT_AUTO, "
									"DELIVERY_DATE, SET_CONVENTION, BEST_EXECUTION, SHORT_SALE, PRICE, DOWN_PYMNT, BROKER_FEE, "
									"OTHER_FEE, ASSIGN_CP, DV01, VAR, SOFTDOLLAR, CANCEL_TICKET, CORRECT_TICKET, SEC_FEE, OR_FEE, "
									"EXEC_TIME, ETRADE, CSPB_SHORT, SWAP_TICKET, TRADER_SIGN, TRADER_SIGN_DATE) "
									"SELECT " + CorrTicket + ", A.TRADER_INI, TRUNC(SYSDATE), A.VALUE_DATE, A.DIR, A.TRANS_TYPE, %.2lf, "
									"A.EXERCISE_PRICE, A.OPT_EXPIRATION, NULL, A.CURRENCY, A.FXRATE, A.COUNTERPARTY, A.BROKER, "
									"A.ASSET_CODE, ASS_DESC, ASS_COUNTRY, ASS_CURRENCY, ASS_INDUSTRY, ASS_CATEGORY, BUCKET, ASS_IND_AREA, "
									"ASS_MATURITY_DATE, ASS_COMMON_CODE, ASS_MSTC_CODE, ASS_ISIN_CODE, ASS_SEDOL_NUM, A.NEW_ISSUE, "
									"A.DEAL_TYPE, NULL, A.TR_RATE, A.RATE_BASIS, A.MATURITY_DATE, A.ORIG_SW_BOOKING, A.ORIG_SW_MATURITY, "
									"TO_NUMBER(NULL), NULL, TO_DATE(NULL), A.HAN_DETAIL1, A.HAN_DETAIL2, B.SOURCE, A.OPT_TICK, A.EURO_OPT, "
									"A.WI, ASS_FX_DATE, NVL(A.OPT_ID, ASS_PID), A.MARGIN, A.MARGIN_CURRENCY, %.2lf, A.FUNDED_SWAP, A.BINARY, "
									"A.OPT_SET_CODE, A.OPT_SET_CODE2, A.OPT_AUTO, A.DELIVERY_DATE, A.SET_CONVENTION, A.BEST_EXECUTION, "
									"A.SHORT_SALE, A.PRICE, 100, %.2lf, %.2lf, A.ASSIGN_CP, %lf, %lf, TO_NUMBER(NULL), TO_NUMBER(NULL), "
									"A.TICKET_NUM, A.SEC_FEE, A.OR_FEE, D.EXEC_TIME, A.ETRADE, A.CSPB_SHORT, A.SWAP_TICKET, D.TRADER_SIGN, "
									"D.TRADER_SIGN_DATE "
									"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B, SEMAM.NW_RAW_TICKETS D "
									"WHERE B.ASS_CODE(+) = A.ASSET_CODE "
									"AND D.TICKET_NUM(+) = A.TICKET_NUM "
									"AND A.TR_DESC = 'BOOKING' "
									"AND A.TRANS_NUM = " + sTransNum + 
									" AND A.TRANS_TYPE NOT IN ('INT. SWAP') "
									"AND A.DEAL_TYPE IS NULL "
									"UNION "
									"SELECT " + CorrTicket + ", A.TRADER_INI, TRUNC(SYSDATE), A.VALUE_DATE, A.DIR, A.TRANS_TYPE, %.2lf, "
									"A.EXERCISE_PRICE, A.OPT_EXPIRATION, NULL, A.CURRENCY, A.FXRATE, A.COUNTERPARTY, A.BROKER, A.ASSET_CODE, "
									"ASS_DESC, ASS_COUNTRY, ASS_CURRENCY, ASS_INDUSTRY, ASS_CATEGORY, BUCKET, ASS_IND_AREA, ASS_MATURITY_DATE, "
									"ASS_COMMON_CODE, ASS_MSTC_CODE, ASS_ISIN_CODE, ASS_SEDOL_NUM, A.NEW_ISSUE, A.DEAL_TYPE, NULL, A.TR_RATE, "
									"A.RATE_BASIS, A.MATURITY_DATE, A.ORIG_SW_BOOKING, A.ORIG_SW_MATURITY, C.TR_RATE, C.RATE_BASIS, C.MATURITY_DATE, "
									"A.HAN_DETAIL1, A.HAN_DETAIL2, B.SOURCE, A.OPT_TICK, A.EURO_OPT, A.WI, B.ASS_FX_DATE, NVL(A.OPT_ID, B.ASS_PID), "
									"A.MARGIN, A.MARGIN_CURRENCY, %.2lf, A.FUNDED_SWAP, A.BINARY, A.OPT_SET_CODE, A.OPT_SET_CODE2, A.OPT_AUTO, "
									"A.DELIVERY_DATE, A.SET_CONVENTION, A.BEST_EXECUTION, A.SHORT_SALE, A.PRICE, 100, %.2lf, %.2lf, A.ASSIGN_CP, "
									"%lf, %lf, TO_NUMBER(NULL), TO_NUMBER(NULL), A.TICKET_NUM, A.SEC_FEE, A.OR_FEE, D.EXEC_TIME, A.ETRADE, "
									"A.CSPB_SHORT, A.SWAP_TICKET, D.TRADER_SIGN, D.TRADER_SIGN_DATE "
									"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B, SEMAM.ALL_TICKET_INV_V C, SEMAM.NW_RAW_TICKETS D "
									"WHERE B.ASS_CODE(+) = A.ASSET_CODE "
									"AND C.PORTFOLIO(+) = A.PORTFOLIO "
									"AND C.TRANS_TYPE(+) = A.TRANS_TYPE "
									"AND C.TICKET_NUM(+) = A.TICKET_NUM "
									"AND C.TRADE_DATE(+) = A.TRADE_DATE "
									"AND C.TRANS_NUM(+) != A.TRANS_NUM "
									"AND D.TICKET_NUM(+) = A.TICKET_NUM "
									"AND A.TR_DESC = 'BOOKING' "
									"AND A.TRANS_NUM = " + sTransNum + 
									" AND A.TRANS_TYPE IN ('INT. SWAP') "
									"AND A.DEAL_TYPE IS NULL "
									"UNION "
									"SELECT " + CorrTicket + ", A.TRADER_INI, TRUNC(SYSDATE), A.VALUE_DATE, A.DIR, A.TRANS_TYPE, %.2lf, "
									"A.EXERCISE_PRICE, A.OPT_EXPIRATION, NULL, A.CURRENCY, A.FXRATE, A.COUNTERPARTY, A.BROKER, A.ASSET_CODE, "
									"ASS_DESC, ASS_COUNTRY, ASS_CURRENCY, ASS_INDUSTRY, ASS_CATEGORY, BUCKET, ASS_IND_AREA, ASS_MATURITY_DATE, "
									"ASS_COMMON_CODE, ASS_MSTC_CODE, ASS_ISIN_CODE, ASS_SEDOL_NUM, A.NEW_ISSUE, A.DEAL_TYPE, C.COUNTERPARTY, "
									"C.TR_RATE, C.RATE_BASIS, C.MATURITY_DATE, A.ORIG_SW_BOOKING, A.ORIG_SW_MATURITY, TO_NUMBER(NULL), NULL, "
									"TO_DATE(NULL), A.HAN_DETAIL1, A.HAN_DETAIL2, B.SOURCE, A.OPT_TICK, A.EURO_OPT, A.WI, B.ASS_FX_DATE, "
									"NVL(A.OPT_ID, B.ASS_PID), A.MARGIN, A.MARGIN_CURRENCY, %.2lf, A.FUNDED_SWAP, A.BINARY, A.OPT_SET_CODE, "
									"A.OPT_SET_CODE2, A.OPT_AUTO, A.DELIVERY_DATE, A.SET_CONVENTION, A.BEST_EXECUTION, A.SHORT_SALE, A.PRICE, "
									"0, %.2lf, %.2lf, A.ASSIGN_CP, %lf, %lf, TO_NUMBER(NULL), TO_NUMBER(NULL), A.TICKET_NUM, A.SEC_FEE, A.OR_FEE, "
									"D.EXEC_TIME, A.ETRADE, A.CSPB_SHORT, A.SWAP_TICKET, D.TRADER_SIGN, D.TRADER_SIGN_DATE "
									"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B, SEMAM.ALL_TICKET_INV_V C, SEMAM.NW_RAW_TICKETS D "
									"WHERE B.ASS_CODE(+) = A.ASSET_CODE "
									"AND C.PORTFOLIO(+) = A.PORTFOLIO "
									"AND C.TICKET_NUM(+) = A.TICKET_NUM "
									"AND C.TRADE_DATE(+) = A.TRADE_DATE "
									"AND C.TRANS_TYPE(+) = 'LEVERAGE' "
									"AND C.TRANS_NUM(+) != A.TRANS_NUM "
									"AND C.PORTFOLIO(+) = A.PORTFOLIO "
									"AND C.TICKET_NUM(+) = A.TICKET_NUM "
									"AND C.TRADE_DATE(+) = A.TRADE_DATE "
									"AND C.TRANS_NUM(+) != A.TRANS_NUM "
									"AND D.TICKET_NUM(+) = A.TICKET_NUM "
									"AND A.TR_DESC = 'BOOKING' "
									"AND A.TRANS_TYPE = 'SECURITIES' "
									"AND A.TRANS_NUM = " + sTransNum + 
									" AND A.DEAL_TYPE IS NOT NULL ", Nominal, MarginAmt, Comm, OtherFee, DV01, VAR, Nominal, MarginAmt, 
									Comm, OtherFee, DV01, VAR, Nominal, MarginAmt, Comm, OtherFee, DV01, VAR);
	
	if(!GetOraLoader().ExecuteSql(GetOraLoader().GetSql()))
		return FALSE;

	if(!GetOraLoader().ExecuteSql("INSERT INTO SEMAM.NW_RAW_TICKET_DETAIL (TICKET_NUM, PORTFOLIO, NOM_AMOUNT, OPT_BACK, CUSTODIAN, "
								"ACCOUNT, PRICE, DOWN_PYMNT) "
								"SELECT " + CorrTicket + ", PORTFOLIO, NOM_AMOUNT, OPTION_BACKING, CUSTODIAN, ACC_CODE, PRICE, 100 "
								"FROM SEMAM.ALL_TICKET_INV_V A "
								"WHERE TICKET_NUM = " + sTicket + 
								" AND TR_DESC = 'BOOKING' "))
		return FALSE;
	
	return TRUE;
}