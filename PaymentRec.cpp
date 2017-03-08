#include "stdafx.h"
#include "resource.h"
#include "paymentrec.h"
#include "sqlstmt.h"
#include "qdata.h"


IMPLEMENT_DYNAMIC(CPayTemplateRec, CRecObj)
void CPayTemplateRec::Copy(CPayTemplateRec &PayTemplateRec)
{
	SetRefID(PayTemplateRec.GetRefID());
	SetCP(PayTemplateRec.GetCP());
	SetCurrency(PayTemplateRec.GetCurrency());
	SetBank(PayTemplateRec.GetBank());
	SetAccount(PayTemplateRec.GetAccount());
	SetCPRef(PayTemplateRec.GetCPRef());
	SetBeneficiary(PayTemplateRec.GetBeneficiary());
	SetBeneficiaryID(PayTemplateRec.GetBeneficiaryID());
	SetBeneficiaryBIC(PayTemplateRec.GetBeneficiaryBIC());
	SetBeneficiaryAccount(PayTemplateRec.GetBeneficiaryAccount());
	SetFFC(PayTemplateRec.GetFFC());
	SetFFCID(PayTemplateRec.GetFFCID());
	SetFFCBIC(PayTemplateRec.GetFFCBIC());
	SetFFCAccount(PayTemplateRec.GetFFCAccount());
	SetActive(PayTemplateRec.GetActive());

	SetBankID(PayTemplateRec.GetBankID());
	SetBankBIC(PayTemplateRec.GetBankBIC());
}

void CPayTemplateRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();

	Rec.Add(GetRefID());
	Rec.Add(GetCP());
	Rec.Add(GetCurrency());
	Rec.Add(GetBank());
	Rec.Add(GetAccount());
	Rec.Add(GetBeneficiary());
	Rec.Add(GetBeneficiaryID());
	Rec.Add(GetBeneficiaryBIC());
	Rec.Add(GetBeneficiaryAccount());
	Rec.Add(GetFFC());
	Rec.Add(GetFFCID());
	Rec.Add(GetFFCBIC());
	Rec.Add(GetFFCAccount());
	Rec.Add(GetCPRef());
	Rec.Add(GetActive());
}

BOOL CPayTemplateRec::AddRec(COraLoader &OraLoader)
{
	if(GetRefID().IsEmpty())
		return FALSE;

	if(!OraLoader.Open("SELECT REF_ID, COUNTERPARTY, CURRENCY, BANK, ACCOUNT, BENEFICIARY, "
						"BENEFICIARY_ID, BENEFICIARY_BIC, BENEFICIARY_ACCOUNT, FFC, FFC_ID, "
						"FFC_BIC, FFC_ACCOUNT, CP_REF, ACTIVE FROM SEMAM.NW_PAY_TEMPLATE ", 
						ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CPayTemplateRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetRefID().IsEmpty())
		return FALSE;
	
	CQData QData;

	OraLoader.GetSql().Format("SELECT REF_ID, COUNTERPARTY, CURRENCY, BANK, ACCOUNT, "
						"BENEFICIARY, BENEFICIARY_ID, BENEFICIARY_BIC, BENEFICIARY_ACCOUNT, "
						"FFC, FFC_ID, FFC_BIC, FFC_ACCOUNT, CP_REF, ACTIVE "
						"FROM SEMAM.NW_PAY_TEMPLATE WHERE REF_ID = %s ", 
						QData.GetQueryText(GetRefID()));

	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec);
}

BOOL CPayTemplateRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetRefID().IsEmpty())
		return FALSE;
	CQData QData;

	OraLoader.GetSql().Format("DELETE SEMAM.NW_PAY_TEMPLATE WHERE REF_ID = %s ", 
								QData.GetQueryText(GetRefID()));
	return OraLoader.ExecuteSql();
}


IMPLEMENT_DYNAMIC(CPayInvRec, CRecObj)
void CPayInvRec::Copy(CPayInvRec &PayInvRec)
{
	SetID(PayInvRec.GetID());
	SetPayID(PayInvRec.GetPayID());
	SetTransNum(PayInvRec.GetTransNum());
	SetAmount(PayInvRec.GetAmount());
}

void CPayInvRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();

	Rec.Add(GetID());
	Rec.Add(GetPayID());
	Rec.Add(GetTransNum());
	Rec.Add(GetAmount());
}

BOOL CPayInvRec::AddRec(COraLoader &OraLoader)
{
	if(GetPayID().IsEmpty())
		return FALSE;

	if(!OraLoader.Open("SELECT INV_NUM, PAY_ID, TRANS_NUM, AMOUNT "
						"FROM SEMAM.NW_PAYMENT_DETAILS ", ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CPayInvRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(strlen(GetPayID()) == 0 || strlen(GetID()) == 0)
		return FALSE;

	CQData QData;

	OraLoader.GetSql().Format("SELECT INV_NUM, PAY_ID, TRANS_NUM, AMOUNT "
						"FROM SEMAM.NW_PAYMENT_DETAILS WHERE INV_NUM = %s", 
						QData.GetQueryNumber(GetID()));
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec);
}

BOOL CPayInvRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetPayID().IsEmpty())
		return FALSE;
	
	CQData QData;

	if(bByID)
	{
		if(strlen(GetID()) == 0)
			return FALSE;
		OraLoader.GetSql().Format("DELETE SEMAM.NW_PAYMENT_DETAILS WHERE INV_NUM = %s", 
									QData.GetQueryNumber(GetID()));
	}
	else
		OraLoader.GetSql().Format("DELETE SEMAM.NW_PAYMENT_DETAILS WHERE PAY_ID = %s", 
									QData.GetQueryNumber(GetPayID()));

	return OraLoader.ExecuteSql();
}


IMPLEMENT_DYNAMIC(CPayInvCouponRec, CRecObj)
void CPayInvCouponRec::Copy(CPayInvCouponRec &PayInvCouponRec)
{
	SetID(PayInvCouponRec.GetID());
	SetPortfolio(PayInvCouponRec.GetPortfolio());
	SetAsset(PayInvCouponRec.GetAsset());
	SetTransNum(PayInvCouponRec.GetTransNum());
	SetInvNum(PayInvCouponRec.GetInvNum());
	SetPayID(PayInvCouponRec.GetPayID());
	SetDate(PayInvCouponRec.GetDate());
	SetAmount(PayInvCouponRec.GetAmount());
	SetFxrate(PayInvCouponRec.GetFxrate());
	SetRemaining(PayInvCouponRec.GetRemaining());
	SetCouponPaid(PayInvCouponRec.GetCouponPaid());
}

void CPayInvCouponRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();

	Rec.Add(GetPortfolio());
	Rec.Add(GetAsset());
	Rec.Add(GetTransNum());
	Rec.Add(GetInvNum());
	Rec.Add(GetPayID());
	Rec.Add(GetDate());
	Rec.Add(GetAmount());
	Rec.Add(GetFxrate());
	Rec.Add(GetRemaining());
	Rec.Add(GetCouponPaid());
}

BOOL CPayInvCouponRec::AddRec(COraLoader &OraLoader)
{
	if(GetPayID().IsEmpty())
		return FALSE;

	if(!OraLoader.Open("SELECT PORTFOLIO, ASSET_CODE, TRANS_NUM, INV_NUM, PAY_ID, "
						"TO_DATE, AMOUNT, FXRATE, REMAINING, COUPON_PAID "
						"FROM SEMAM.NW_PAYMENT_COUPON ", ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CPayInvCouponRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(strlen(GetPayID()) == 0 || GetPortfolio().IsEmpty() || GetAsset().IsEmpty() || GetDate().IsEmpty())
		return FALSE;

	CQData QData;
	CString Asset;

	Asset = QData.GetQueryText(GetAsset());
	OraLoader.GetSql().Format("SELECT PORTFOLIO, ASSET_CODE, TRANS_NUM, INV_NUM, "
						"PAY_ID, TO_DATE, AMOUNT, FXRATE, REMAINING, COUPON_PAID "
						"FROM SEMAM.NW_PAYMENT_COUPON WHERE PAY_ID = %s ", 
						QData.GetQueryNumber(GetPayID()));
	
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec);
}

BOOL CPayInvCouponRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetPayID().IsEmpty())
		return FALSE;
	
	CQData QData;
	CString Port;
	CString Asset;

	if(bByID)
	{
		if(GetPortfolio().IsEmpty() || GetAsset().IsEmpty() || GetDate().IsEmpty())		
			return FALSE;
		
		Port = QData.GetQueryText(GetPortfolio());
		Asset = QData.GetQueryText(GetAsset());
		OraLoader.GetSql().Format("DELETE SEMAM.NW_PAYMENT_COUPON WHERE PORTFOLIO = %s "
							"AND ASSET = %s AND TO_DATE = %s ", (LPCTSTR) Port, 
							(LPCTSTR) Asset, QData.GetQueryDate(GetDate()));
	}
	else
		OraLoader.GetSql().Format("DELETE SEMAM.NW_PAYMENT_COUPON WHERE PAY_ID = %s ", 
							QData.GetQueryNumber(GetPayID()));

	return OraLoader.ExecuteSql();
}



IMPLEMENT_DYNAMIC(CPaymentRec, CRecObj)
void CPaymentRec::Copy(CPaymentRec &PaymentRec)
{
	SetID(PaymentRec.GetID());
	SetRefID(PaymentRec.GetRefID());
	SetPortfolio(PaymentRec.GetPortfolio());
	SetAccount(PaymentRec.GetAccount());
	SetCP(PaymentRec.GetCP());
	SetDir(PaymentRec.GetDir());
	SetDate(PaymentRec.GetDate());
	SetCurrency(PaymentRec.GetCurrency());
	SetAmount(PaymentRec.GetAmount());
	SetAmount(PaymentRec.GetAdjust());
	SetNote(PaymentRec.GetNote());
	SetValueDate(PaymentRec.GetValueDate());
	SetCashType(PaymentRec.GetCashType());
	SetAssetCode(PaymentRec.GetAssetCode());
	SetCPRef(PaymentRec.GetCPRef());
	SetCloseFlag(PaymentRec.GetCloseFlag());
	SetToAccount(PaymentRec.GetToAccount());
	SetFxrate(PaymentRec.GetFxrate());
}

void CPaymentRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();
	CQData QData;

	Rec.Add(GetPayID());
	Rec.Add(GetRefID());
	Rec.Add(GetPortfolio());
	Rec.Add(GetAccount());
	Rec.Add(GetCP());
	if(GetDir().IsEmpty())
		Rec.Add(GetDir());
	else
		Rec.Add(GetDir().Left(1));
	Rec.Add(GetDate());
	Rec.Add(GetCurrency());
	Rec.Add(QData.Abs(GetAmount()));
	Rec.Add(GetAdjust());
	Rec.Add(GetNote());
	Rec.Add(GetValueDate());
	Rec.Add(GetCashType());
	Rec.Add(GetAssetCode());
	Rec.Add(GetCPRef());
	Rec.Add(GetCloseFlag());
	Rec.Add(GetToAccount());
	Rec.Add(GetFxrate());
}

BOOL CPaymentRec::AddRec(COraLoader &OraLoader)
{
	if(GetPayID().IsEmpty())
		return FALSE;

	if(GetDate().IsEmpty())
		OraLoader.Today(GetDate());

	if(!OraLoader.Open("SELECT PAY_ID, REF_ID, PORTFOLIO, ACC_CODE, COUNTERPARTY, "
						"DIR, INDATE, CURRENCY, AMOUNT, ADJUSTMENT, NOTE, VALUE_DATE, "
						"PMNT_TYPE, ASSET_CODE, CP_REF_NOTE,  PAYMENT_MADE_FLAG, "
						"TO_ACCOUNT, FXRATE FROM SEMAM.NW_PAYMENTS ", 
						ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CPaymentRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(strlen(GetPayID()) == 0 || strlen(GetID()) == 0)
		return FALSE;

	CQData QData;

	if(GetDate().IsEmpty())
		OraLoader.Today(GetDate());

	OraLoader.GetSql().Format("SELECT PAY_ID, REF_ID, PORTFOLIO, ACC_CODE, COUNTERPARTY, "
								"DIR, INDATE, CURRENCY, AMOUNT, ADJUSTMENT, NOTE, VALUE_DATE, "
								"PMNT_TYPE, ASSET_CODE, CP_REF_NOTE, PAYMENT_MADE_FLAG, "
								"TO_ACCOUNT, FXRATE FROM SEMAM.NW_PAYMENTS "
								"WHERE PAY_ID = %s", QData.GetQueryNumber(GetPayID()));
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec);
}

BOOL CPaymentRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(strlen(GetPayID()) == 0 || strlen(GetID()) == 0)
		return FALSE;

	CQData QData;
	CPayInvRec PayInvRec;
	CPayInvCouponRec PayInvCouponRec;

	PayInvRec.SetPayID(GetPayID());

	if(!PayInvRec.DeleteRec(OraLoader))
		return FALSE;

	PayInvCouponRec.SetPayID(GetPayID());
	if(!PayInvCouponRec.DeleteRec(OraLoader))
		return FALSE;
	if(bByID)
		OraLoader.GetSql().Format("DELETE SEMAM.NW_PAYMENTS WHERE ROWIDTOCHAR(ROWID) = %s", 
									QData.GetQueryText(GetID()));
	else
		OraLoader.GetSql().Format("DELETE SEMAM.NW_PAYMENTS WHERE PAY_ID = %s", 
									QData.GetQueryNumber(GetPayID()));

	return OraLoader.ExecuteSql();
}

BOOL CPaymentRec::Closing(COraLoader &OraLoader, BOOL bClose)
{
	if(GetPayID().IsEmpty())
		return FALSE;
	
	CQData QData;
	CDBRec Rec;
	if(bClose)
		Rec.Add(Y);
	else
		Rec.Add(EMPTYSTRING);
	OraLoader.GetSql().Format("SELECT PAYMENT_MADE_FLAG FROM SEMAM.NW_PAYMENTS "
								"WHERE PAY_ID = %s", QData.GetQueryNumber(GetPayID()));

	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	return OraLoader.UpdateRecord(Rec);
}