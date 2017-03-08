#include "stdafx.h"
#include "NWIFMS70.h"
#include <math.h>
#include "Confirmdata.h"
#include "qdata.h"


IMPLEMENT_DYNAMIC(CConfirmData, CMSRowCtrl)
void CConfirmData::Setup(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS, 
						   LPCTSTR User, int KeyIndex)
{
	CMSRowCtrl::Setup(OraLoader, pSS, pSSS, KeyIndex);
}

BOOL CConfirmData::IsReady()
{
	if(!CMSRowCtrl::IsReady())
		return FALSE;
	
	return TRUE;
}

BOOL CConfirmData::GetSelInvRec(int Index, CInvRec &InvRec)
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

int CConfirmData::LoadTickets()
{
	CDBRec Rec;
	CStringArray QRec;

	if(!IsReady())
		return -1;

	UpdateData(&GetTicket(), &GetInv());

	ToDBRec(Rec, TRUE);

	Rec.RemoveAt(Rec.GetSize() - 1); // Remove Class
	if(Rec.GetSize() <= 0)
		return -1;

	GetSRowCtrl().GetSS()->ClearSheet();
	if(GetSS()->GetFieldArray().GetSize() <= 0)
	{
		GetOraLoader().GetSql() = "SELECT COUNTERPARTY, TICKET_NUM, PORTFOLIO, TRANS_NUM, TRADE_DATE, ASSET_CODE, "
									"TRADER_INI, VALUE_DATE, TRANS_TYPE, TRANS_DIRECTION, NOM_AMOUNT, PRICE, "
									"CURRENCY, FXRATE, CON_CODE, DEAL_TYPE, EXERCISE_PRICE, EXERCISE_DATE, "
									"OPT_EXPIRATION, FX_DATE, OPT_TICK, OPT_SET_CODE, OPT_SET_CODE2, SET_CONVENTION, "
									"OPT_ID, EURO_OPT, OPT_AUTO, BR_FEES, A.SOFTDOLLAR, LEV_RATE, VAR, TR_RATE, "
									"RATE_BASIS, MATURITY_DATE, FLOAT_RATE_INDEX, FLOAT_RATE_FORMULA, ORIG_SW_BOOKING, "
									"ORIG_SW_MATURITY, FX_ACCNUM, USD_ACCNUM, PFU, NEW_ISSUE, CUSTODIAN, OPTION_BACKING, "
									"TRANS_SEQ, HAN_DETAIL1, HAN_DETAIL2, CP_TRADE_ID, DOC_REQ, DOC_RECVD, WI, CONFIRM, "
									"SWAP_TICKET, DELIVERY_DATE, MARGIN, MARGIN_CURRENCY, MARGIN_AMOUNT, LISTED, "
									"FUNDED_SWAP, BINARY, DV01, BEST_EXECUTION, SHORT_SALE, A.CSPB_SHORT, CANCEL_TICKET, "
									"CORRECT_TICKET, SEC_FEE, OR_FEE, ETRADE, BOOKER, TO_CHAR(BOOK_DATE, 'MM/DD/YYYY HH24:MI:SS'), "
									"TR_VERSION, ASS_DESC, ASS_CURRENCY FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_ASSETS B "
									"WHERE B.ASS_CODE(+) = A.ASSET_CODE "
									"AND 1 = 2";

		GetOraLoader().Open();
		GetOraLoader().CopyFieldArray(GetSS()->GetFieldArray());
	}

 	if(IsExact())
		GetOraLoader().RecToQueryRec(QRec, Rec, &GetSS()->GetFieldArray());
	else
		GetOraLoader().RecToQueryRec(QRec, Rec, &GetSS()->GetFieldArray(), PERCENT);

	GetOraLoader().GetSql() = "SELECT TRANS_NUM, COUNTERPARTY, TICKET_NUM, PORTFOLIO, TRANS_NUM, TRADE_DATE, ASSET_CODE, "
								"TRADER_INI, VALUE_DATE, TRANS_TYPE, TRANS_DIRECTION, NOM_AMOUNT, PRICE, CURRENCY, FXRATE, "
								"CON_CODE, DEAL_TYPE, EXERCISE_PRICE, EXERCISE_DATE, OPT_EXPIRATION, FX_DATE, OPT_TICK, "
								"OPT_SET_CODE, OPT_SET_CODE2, SET_CONVENTION, OPT_ID, EURO_OPT, OPT_AUTO, BR_FEES, "
								"A.SOFTDOLLAR, LEV_RATE, VAR, TR_RATE, RATE_BASIS, MATURITY_DATE, FLOAT_RATE_INDEX, "
								"FLOAT_RATE_FORMULA, ORIG_SW_BOOKING, ORIG_SW_MATURITY, FX_ACCNUM, USD_ACCNUM, PFU, "
								"NEW_ISSUE, CUSTODIAN, OPTION_BACKING, TRANS_SEQ, HAN_DETAIL1, HAN_DETAIL2, CP_TRADE_ID, "
								"DOC_REQ, DOC_RECVD, WI, CONFIRM, SWAP_TICKET, DELIVERY_DATE, MARGIN, MARGIN_CURRENCY, "
								"MARGIN_AMOUNT, LISTED, FUNDED_SWAP, BINARY, DV01, BEST_EXECUTION, SHORT_SALE, A.CSPB_SHORT, "
								"CANCEL_TICKET, CORRECT_TICKET, SEC_FEE, OR_FEE, ETRADE, BOOKER, "
								"TO_CHAR(BOOK_DATE, 'MM/DD/YYYY HH24:MI:SS'), TR_VERSION, ASS_DESC, ASS_CURRENCY, ASS_INDUSTRY "
								"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_ASSETS B "
								"WHERE B.ASS_CODE(+) = A.ASSET_CODE ";
	
	GetOraLoader().SetSqlSearchInfo(GetOraLoader().GetSql(), QRec, &GetSS()->GetFieldArray(), TRUE, IsExact());
	GetOraLoader().GetSql() += " ORDER BY 2,3,4,5 ";

	if(GetOraLoader().Open())
		return GetOraLoader().LoadRowCtrl(*this, TRUE);

	return -1;
}

void CConfirmData::LoadSheet()
{
	if(!IsReady())
		return;

	CQData QData;

	GetOraLoader().GetSql().Format("SELECT INV_NUM, ASSET_CODE, TR_DESC, NOM_AMOUNT, ACTUAL_VDATE, ACTUAL_CLOSING, POST_DATE, "
									"CURRENCY, FOREX_REF_NUM, CROSS_RATE, CASH_AMOUNT, INT_AMOUNT, ACC_CODE, ASSIGN_CP, ASSIGN_CT "
									"FROM SEMAM.NW_TR_INV WHERE TRANS_NUM = %s", QData.GetQueryNumber(GetID()));
	if(GetOraLoader().Open())
		GetOraLoader().LoadRowCtrl(GetSRowCtrl(), TRUE);
}

void CConfirmData::SetHeaders()
{
	GetOraLoader().GetSql() = "SELECT COUNTERPARTY, TICKET_NUM, PORTFOLIO, TRANS_NUM, TRADE_DATE, ASSET_CODE, TRADER_INI, "
								"VALUE_DATE, TRANS_TYPE, TRANS_DIRECTION, NOM_AMOUNT, PRICE, CURRENCY, FXRATE, CON_CODE, "
								"DEAL_TYPE, EXERCISE_PRICE, EXERCISE_DATE, OPT_EXPIRATION, FX_DATE, OPT_TICK, OPT_SET_CODE, "
								"OPT_SET_CODE2, SET_CONVENTION, OPT_ID, EURO_OPT, OPT_AUTO, BR_FEES, A.SOFTDOLLAR, LEV_RATE, "
								"VAR, TR_RATE, RATE_BASIS, MATURITY_DATE, FLOAT_RATE_INDEX, FLOAT_RATE_FORMULA, "
								"ORIG_SW_BOOKING, ORIG_SW_MATURITY, FX_ACCNUM, USD_ACCNUM, PFU, NEW_ISSUE, CUSTODIAN, "
								"OPTION_BACKING, TRANS_SEQ, HAN_DETAIL1, HAN_DETAIL2, CP_TRADE_ID, DOC_REQ, DOC_RECVD, WI, "
								"CONFIRM, SWAP_TICKET, DELIVERY_DATE, MARGIN, MARGIN_CURRENCY, MARGIN_AMOUNT, LISTED, "
								"FUNDED_SWAP, BINARY, DV01, BEST_EXECUTION, SHORT_SALE, A.CSPB_SHORT, CANCEL_TICKET, "
								"CORRECT_TICKET, SEC_FEE, OR_FEE, ETRADE, BOOKER, BOOK_DATE, TR_VERSION, ASS_DESC, "
								"ASS_CURRENCY, ASS_INDUSTRY "
								"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_ASSETS B "
								"WHERE B.ASS_CODE(+) = A.ASSET_CODE "
								"AND 1 = 2 ";

	GetOraLoader().Open();
	GetOraLoader().CopyFieldArray(GetSS()->GetFieldArray());
	GetSS()->SetSheetColHeaders();

	GetOraLoader().GetSql() = "SELECT INV_NUM, ASSET_CODE, TR_DESC, NOM_AMOUNT, ACTUAL_VDATE, ACTUAL_CLOSING, POST_DATE, "
								"CURRENCY, FOREX_REF_NUM, CROSS_RATE, CASH_AMOUNT, INT_AMOUNT, ACC_CODE, ASSIGN_CP, ASSIGN_CT "
								"FROM SEMAM.NW_TR_INV WHERE TRANS_NUM = 1 ";
	GetOraLoader().Open();
	GetOraLoader().CopyFieldArray(GetSRowCtrl().GetSS()->GetFieldArray());
	GetSRowCtrl().GetSS()->GetFieldArray().RemoveAt(0);
	GetSRowCtrl().GetSS()->SetSheetColHeaders();
}

BOOL CConfirmData::AddRow()
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

BOOL CConfirmData::DeleteRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData(&GetTicket(), &GetInv());
	if(strlen(GetKey()) == 0)
		return FALSE;

	if(GetCurrentRow() > 0 && GetSS()->GetIsBlockSelected())
	{
		GetInv().SetTransNum(GetTicket().GetTransNum());

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

BOOL CConfirmData::UpdateRow()
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

BOOL CConfirmData::AddSRow()
{
	if(!IsReady())
		return FALSE;

	if(strlen(GetKey()) == 0)
		return FALSE;

	GetInv().SetTransNum(GetID());
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

BOOL CConfirmData::DeleteSRow(int CurrentRow)
{
	if(!IsReady())
		return FALSE;

	if(GetSRowCtrl().GetCurrentRow() > 0 && GetSRowCtrl().GetSS()->GetIsBlockSelected())
	{
		if(strlen(GetID()) == 0 || strlen(GetInv().GetID()) == 0)
			return FALSE;

		GetInv().SetTransNum(GetID());

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

BOOL CConfirmData::UpdateSRow()
{
	if(!IsReady())
		return FALSE;

	if(strlen(GetKey()) == 0 || strlen(GetInv().GetID()) == 0)
		return FALSE;

	if(GetSRowCtrl().GetModify(FALSE))
	{
		GetInv().SetTransNum(GetID());

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

void CConfirmData::SetupAssetInfo(CAssetVal &Val)
{		
	CString Date, Fx;

	if(GetTicket().GetTransType() == REPO || GetTicket().GetTransType() == LEVERAGE || 
		GetTicket().GetTransType() == INTSWAP)
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

/*	if(GetInv().GetForexRef().GetLength() > 0)
	{
		GetOraLoader().GetSql().Format("SELECT FXRATE FROM SEMAM.NW_TR_TICKETS "
						"WHERE TRANS_NUM = %s ", (LPCTSTR ) GetInv().GetForexRef());
		GetOraLoader().Open();
		GetOraLoader().LoadTextString(Fx);
	}
	else */
		if(GetInv().GetCrossRate().GetLength() > 0)
		{
			if(GetTicket().GetFxRate().GetLength() > 0)
				Fx.Format("%.9lf", atof(GetTicket().GetFxRate())*atof(GetInv().GetCrossRate()));
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

void CConfirmData::Compute()
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
		if(strcmp(GetInv().GetTrDesc(), BOOKING) == 0 || GetTicket().GetMaturity().IsEmpty())
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
