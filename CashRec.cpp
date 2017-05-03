#include "stdafx.h"
#include "resource.h"
#include "cashrec.h"
#include "nwivar.h"
#include "qdata.h"
#include "ticketrec.h"
#include "paymentrec.h"


IMPLEMENT_DYNAMIC(CCashInvRec, CRecObj)
void CCashInvRec::Copy(CCashInvRec &CashInvRec)
{
	SetID(CashInvRec.GetID());
	SetCashNum(CashInvRec.GetCashNum());
	SetTransNum(CashInvRec.GetTransNum());
	SetAmount(CashInvRec.GetAmount());
}

void CCashInvRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();

	Rec.Add(GetID());
	Rec.Add(GetCashNum());
	Rec.Add(GetTransNum());
	Rec.Add(GetAmount());
}

BOOL CCashInvRec::AddRec(COraLoader &OraLoader)
{
	if(GetCashNum().IsEmpty() || GetTransNum().IsEmpty() || GetID().IsEmpty())
		return FALSE;
	
	CInvRec InvRec;

	InvRec.SetID(GetID());
	if(!InvRec.Closing(OraLoader, GetCloseDate(), GetPostDate()))
		return FALSE;

	if(!OraLoader.Open("SELECT INV_NUM, CASH_NUM, TRANS_NUM, AMOUNT FROM SEMAM.NW_CASH_MAPPINGS ", 
						ODYNASET_DEFAULT))
		return FALSE;
	
	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CCashInvRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(strlen(GetCashNum()) == 0 || strlen(GetTransNum()) == 0 || strlen(GetID()) == 0)
		return FALSE;
	
	CQData QData;
	
	// if we have open trade
	OraLoader.GetSql().Format("SELECT COUNT(*) FROM SEMAM.NW_OPEN_TRADE_2 "
							"WHERE INV_NUM = %s ", QData.GetQueryText(GetID()));
	if(OraLoader.GetCount() > 0)
	{
		CInvRec InvRec;
		InvRec.SetID(GetID());
		InvRec.Closing(OraLoader, GetCloseDate(), GetPostDate());
	}

	OraLoader.GetSql().Format("SELECT INV_NUM, CASH_NUM, TRANS_NUM, AMOUNT "
							"FROM SEMAM.NW_CASH_MAPPINGS WHERE INV_NUM = %s", 
							QData.GetQueryNumber(GetID())); // InvNum
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;
	
	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec);
}

BOOL CCashInvRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(strlen(GetCashNum()) == 0 && strlen(GetID()) == 0)
		return FALSE;
	
	CQData QData;
	CInvRec InvRec;

	InvRec.SetID(GetID());
	if(bByID)
	{
		InvRec.SetID(GetID());
		InvRec.Closing(OraLoader, GetCloseDate(), GetPostDate(), FALSE); // Reopen Trades
		OraLoader.GetSql().Format("DELETE SEMAM.NW_CASH_MAPPINGS WHERE INV_NUM = %s ", 
									QData.GetQueryNumber(GetID()));
		return OraLoader.ExecuteSql();
	}

	CDBRecArray IDArr;
	OraLoader.GetSql().Format("SELECT INV_NUM FROM SEMAM.NW_CASH_MAPPINGS WHERE CASH_NUM = %s ", 
								QData.GetQueryNumber(GetCashNum()));
	OraLoader.Open();
	OraLoader.LoadDBRecArray(IDArr);

	for(int i = 0; i < IDArr.GetSize(); i ++)
	{
		SetID(IDArr.GetAt(i)->GetAt(0));
		DeleteRec(OraLoader, TRUE);
	}
	
	return TRUE;
}



IMPLEMENT_DYNAMIC(CCashRec, CRecObj)
void CCashRec::Copy(CCashRec &CashRec)
{
	SetCashNum(CashRec.GetCashNum());
    SetPortfolio(CashRec.GetPortfolio());
	SetAccount(CashRec.GetAccount());
	SetRef(CashRec.GetRef());
	SetCurrency(CashRec.GetCurrency());
	SetCloseDate(CashRec.GetCloseDate());
	SetPostDate(CashRec.GetPostDate());
	SetPayType(CashRec.GetPayType());
	SetAmount(CashRec.GetAmount());
	SetFxRate(CashRec.GetFxRate());
	SetRemain(CashRec.GetRemain());
	SetDiffReason(CashRec.GetDiffReason());
	SetAsset(CashRec.GetAsset());
	SetPayID(CashRec.GetPayID());
	SetToAccount(CashRec.GetToAccount());
	SetToPortfolio(CashRec.GetToPortfolio());
	SetLink(CashRec.GetLink());
	SetIRSID(CashRec.GetIRSID());
	SetComment(CashRec.GetComment());
	SetCouponPaid(CashRec.GetCouponPaid());

	SetPayID(CashRec.GetPayID());
}

void CCashRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();

    Rec.Add(GetCashNum());
    Rec.Add(GetPortfolio());
	Rec.Add(GetAccount());
	Rec.Add(GetRef());
	Rec.Add(GetCurrency());
	Rec.Add(GetCloseDate());
	Rec.Add(GetPostDate());
	Rec.Add(GetToDate());
	Rec.Add(GetPayType());
	Rec.Add(GetAmount());
	Rec.Add(GetFxRate());
	Rec.Add(GetRemain());
	Rec.Add(GetDiffReason());
	Rec.Add(GetAsset());
	Rec.Add(GetPayID());
	Rec.Add(GetToAccount());
	Rec.Add(GetToPortfolio());
	Rec.Add(GetLink());
	Rec.Add(GetIRSID());
	Rec.Add(GetComment());
	Rec.Add(GetCouponPaid());
}

BOOL CCashRec::AddRec(COraLoader &OraLoader)
{
	if(GetCashNum().IsEmpty())
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);

	if(!OraLoader.Open("SELECT CASH_NUM, PORTFOLIO, ACC_CODE, CUST_REFERENCE, "
				"CURRENCY, CLOSE_DATE, POST_DATE, TO_DATE, PMNT_TYPE, AMOUNT, "
				"FXRATE, REMAINING, DIFF_REASON, ASS_CODE, PAY_ID, ACC_TO, "
				"TO_PORTFOLIO, LINK, SWAP_TRANS_NUM, COMMENTS, COUPON_PAID "
				"FROM SEMAM.NW_CASH_PMNTS ", ODYNASET_DEFAULT))
		return FALSE;

	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CCashRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetCashNum().IsEmpty())
		return FALSE;
	
	CQData QData;
	CDBRec Rec;	

	OraLoader.GetSql().Format("SELECT CASH_NUM, PORTFOLIO, ACC_CODE, CUST_REFERENCE, "
					"CURRENCY, CLOSE_DATE, POST_DATE, TO_DATE, PMNT_TYPE, AMOUNT, "
					"FXRATE, REMAINING, DIFF_REASON, ASS_CODE, PAY_ID, ACC_TO, "
					"TO_PORTFOLIO, LINK, SWAP_TRANS_NUM, COMMENTS, COUPON_PAID "
					"FROM SEMAM.NW_CASH_PMNTS WHERE CASH_NUM = %s", 
					QData.GetQueryNumber(GetCashNum()));

	ToDBRec(Rec);
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	return OraLoader.UpdateRecord(Rec);
}

BOOL CCashRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetCashNum().IsEmpty())
		return FALSE;

	CCashInvRec Inv;
	CQData QData;

	Inv.SetCashNum(GetCashNum());  // if transact related, open trades first.
	if(!Inv.DeleteRec(OraLoader, FALSE)) 
		return FALSE;

	if(!GetPayID().IsEmpty())  // if payment instrunction related, open it first.
	{
		CPaymentRec Rec;

		Rec.SetPayID(GetPayID());
		Rec.Closing(OraLoader, FALSE);
	}

	if(GetPayType() == "FUTU COST" && !GetIRSID().IsEmpty())
	{
		OraLoader.GetSql().Format("UPDATE SEMAM.NW_FUTURE_COMMISSION SET CASH_NUM = NULL WHERE TRANS_NUM = %s ", QData.GetQueryNumber(GetIRSID()));
		OraLoader.ExecuteSql();
	}

	OraLoader.GetSql().Format("DELETE SEMAM.NW_CASH_PMNTS WHERE CASH_NUM = %s ", 
								QData.GetQueryNumber(GetCashNum()));
	return OraLoader.ExecuteSql();
}
