#include "stdafx.h"
#include "resource.h"
#include "cashdata.h"
#include "qdata.h"
#include "nwivar.h"
#include "ticketrec.h"
#include "paymentrec.h"



IMPLEMENT_DYNAMIC(CCashData, CMSRowCtrl)
void CCashData::Setup(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS, int KeyIndex)
{
	CMSRowCtrl::Setup(OraLoader, pSS, pSSS, 0);

	CString Date;
	GetOraLoader().Today(Date);
	GetOraLoader().TMinus1(m_TMinus1Date, Date);
	SetCashStatus(0);  // Nothing Load Yet
	SetInit(TRUE);
	SetCoupon(FALSE);
	SetLoaded(FALSE);
}

BOOL CCashData::LoadOpenTrade(CString &Portfolio, CString &Account, CString &Currency, CString &PostDate, CString &Asset)
{
	if(!IsReady())
		return FALSE;
	
	CString Sql;
	CQData QData;

	SetCashStatus(1);  // Load Open Trade
	
	GetOraLoader().GetSql() = "SELECT A.INV_NUM, A.TRANS_NUM, A.PORTFOLIO, NVL(ACC_CODE, DEF_ACCOUNT) \"ACCOUNT\", "
								"'TRANSACT' \"PMNT_TYPE\", PAY_CUR, ASS_CODE, "
								"DECODE(A.TR_DESC, 'MATURITY', MATURITY_DATE,  VALUE_DATE) \"VALUE_DATE\", "
								"DECODE(DIR, 'P', -1, 'S', 1, 'B', 1, 'L', -1)*DECODE(TRANS_TYPE, 'LEVERAGE', -1, 'INT. SWAP', -1, 1)*"
								"DECODE(A.TR_DESC,'BOOKING',1,'MATURITY',-1,'FX-BKNG',-1)*(NVL(AMOUNT,0)+NVL(INTEREST,0)) \"AMOUNT\", "
								"FXRATE, TO_NUMBER(NULL) \"REMAINING\", NULL \"DIFF_REASON\", NULL \"NOTE\", NULL \"CUST_REFERENCE\", "
								"NULL \"ACCOUNT_TO\", 1 \"USER_CLASS\" "
								"FROM SEMAM.NW_OPEN_TRADE_2 A, SEMAM.NW_TR_INV B, SEMAM.NW_PORTFOLIOS Z "
								"WHERE B.INV_NUM = A.INV_NUM "
								"AND Z.PORTFOLIO = A.PORTFOLIO "
								"AND Z.STATUS IS NULL "
								"AND B.ACTUAL_CLOSING IS NULL "
								"AND 0 = (SELECT COUNT(*) FROM SEMAM.NW_PAYMENT_DETAILS D "
											"WHERE D.INV_NUM = A.INV_NUM) ";
	
	if(!Portfolio.IsEmpty())
	{
		Sql.Format("AND A.PORTFOLIO = %s ", QData.GetQueryText(Portfolio));
		GetOraLoader().GetSql() += Sql;
	}

	if(!Portfolio.IsEmpty())
	{
		Sql.Format("AND NVL(ACC_CODE, DEF_ACCOUNT) = %s ", QData.GetQueryText(Account));
		GetOraLoader().GetSql() += Sql;
	}

	if(!Currency.IsEmpty())
	{
		Sql.Format("AND A.PAY_CUR = %s ", QData.GetQueryText(Currency));
		GetOraLoader().GetSql() += Sql;
	}

	if(!PostDate.IsEmpty())
	{
		Sql.Format("AND DECODE(A.TR_DESC, 'MATURITY', MATURITY_DATE, VALUE_DATE) < %s ", QData.GetQueryDate(PostDate));
		GetOraLoader().GetSql() += Sql;
	}

	if(!Asset.IsEmpty())
	{
		Sql.Format("AND A.ASS_CODE = %s ", QData.GetQueryText(Asset));
		GetOraLoader().GetSql() += Sql;
	}

	GetOraLoader().GetSql() += "ORDER BY 3, 4, 6, 7, 8, 2 ";

	if(!GetOraLoader().Open())
		return FALSE;

	GetOraLoader().GetFieldArray().GetAt(0)->SetWithComma(FALSE);
	GetOraLoader().GetFieldArray().GetAt(1)->SetWithComma(FALSE);
	return GetOraLoader().LoadRowCtrl(GetCashSubData(), TRUE);
}

BOOL CCashData::LoadCash(CString &Portfolio, CString &Account, CString &Currency, CString &PostDate)
{
	if(!IsReady())
		return FALSE;
	
	CQData QData;
	CString Sql;

	SetCashStatus(2);  // Load Payment Instruction

	GetOraLoader().GetSql() = "SELECT PAY_ID, A.PORTFOLIO, ACC_CODE, NVL(PMNT_TYPE, 'TRANSACT') \"PMNT_TYPE\", "
								"CURRENCY, ASSET_CODE, VALUE_DATE, DECODE(DIR, 'R', 1, -1)*AMOUNT \"AMOUNT\", "
								"FXRATE, ADJUSTMENT \"REMAINING\", NULL \"DIFF_REASON\", NOTE, "
								"NULL \"CUST_REFERENCE\", TO_ACCOUNT \"ACCOUNT_TO\", 1 \"USER_CLASS\" "
								"FROM SEMAM.NW_PAYMENTS A, SEMAM.NW_PORTFOLIOS Z "
								"WHERE Z.PORTFOLIO = A.PORTFOLIO "
								"AND Z.STATUS IS NULL "
								"AND A.PAYMENT_MADE_FLAG IS NULL ";

	if(!Portfolio.IsEmpty())
	{
		Sql.Format("AND A.PORTFOLIO = %s ", QData.GetQueryText(Portfolio));
		GetOraLoader().GetSql() += Sql;
	}

	if(!Account.IsEmpty())
	{
		Sql.Format("AND A.ACC_CODE = %s ", QData.GetQueryText(Account));
		GetOraLoader().GetSql() += Sql;
	}

	if(!Currency.IsEmpty())
	{
		Sql.Format("AND A.CURRENCY = %s ", QData.GetQueryText(Currency));
		GetOraLoader().GetSql() += Sql;
	}
			
	if(!PostDate.IsEmpty())
	{
		Sql.Format("AND A.VALUE_DATE < %s ", QData.GetQueryDate(PostDate));
		GetOraLoader().GetSql() += Sql;
	}

	GetOraLoader().GetSql() += "ORDER BY PORTFOLIO, PAY_ID ";

	if(!GetOraLoader().Open())
		return FALSE;

	GetOraLoader().GetFieldArray().GetAt(0)->SetWithComma(FALSE);
	return GetOraLoader().LoadRowCtrl(GetCashSubData(), TRUE, TRUE);
}

BOOL CCashData::LoadCashDetail()
{
	if(!IsReady())
		return FALSE;

	CQData QData;
	CDBRec DBRec;
	int i;

	switch(GetCashStatus())
	{
		case 1:  // Open Trade
			if(strlen(GetInvRec().GetID()) == 0)
				return FALSE;
			
			GetOraLoader().GetSql().Format("SELECT INV_NUM, TRANS_NUM, DECODE(DIR, 'P', -1, 'S', 1, 'B', 1, 'L', -1)*"
											"DECODE(TRANS_TYPE, 'LEVERAGE', -1, 'INT. SWAP', -1, 1)*"
											"DECODE(TR_DESC, 'BOOKING', 1, 'MATURITY', -1, 'FX-BKNG', -1)*("
											"NVL(AMOUNT, 0) + NVL(INTEREST, 0)) \"AMOUNT\" "
											"FROM SEMAM.NW_OPEN_TRADE_2 WHERE INV_NUM = %s", 
											QData.GetQueryNumber(GetInvRec().GetID()));
			if(!GetOraLoader().Open())
				return FALSE;
			
			GetOraLoader().GetFieldArray().GetAt(0)->SetWithComma(FALSE);
			GetOraLoader().GetFieldArray().GetAt(1)->SetWithComma(FALSE);
			GetOraLoader().GetFieldArray().GetAt(2)->SetPrecision(2);

			if(GetSRowCtrl().GetSS()->GetSheetRows() <= 0)
				return GetOraLoader().LoadRowCtrl(GetSRowCtrl(), TRUE, TRUE);
			else
			{
				for (i = 0; i < GetSRowCtrl().GetIDArray().GetSize(); i++)
				{
					if(strcmp(GetInvRec().GetID(), GetSRowCtrl().GetIDArray().GetAt(i)->GetID()) == 0)
						return FALSE;
				}

				DBRec.SetID(GetInvRec().GetID());
				GetOraLoader().LoadDBRec(DBRec);
				GetSRowCtrl().AddSheetRow(DBRec);
				return TRUE;
			}
			break;
		case 2:  // Payment Instruction
			if(strlen(GetRec().GetPayID()) == 0)
				return FALSE;
			GetOraLoader().GetSql().Format("SELECT INV_NUM, TRANS_NUM, AMOUNT "
											"FROM SEMAM.NW_PAYMENT_DETAILS WHERE PAY_ID = %s ", 
											QData.GetQueryNumber(GetRec().GetPayID()));
			if(!GetOraLoader().Open())
				return FALSE;

			GetOraLoader().GetFieldArray().GetAt(0)->SetWithComma(FALSE);
			GetOraLoader().GetFieldArray().GetAt(1)->SetWithComma(FALSE);
			GetOraLoader().GetFieldArray().GetAt(2)->SetPrecision(2);
			return GetOraLoader().LoadRowCtrl(GetSRowCtrl(), TRUE, TRUE);
			break;
		default:
			return FALSE;
	}

}

void CCashData::LoadSheet()
{
	if(!IsReady())
		return;

	CQData QData;

	if(strlen(GetKey()) == 0)
		return;

	GetOraLoader().GetSql().Format("SELECT INV_NUM, TRANS_NUM, AMOUNT "
									"FROM SEMAM.NW_CASH_MAPPINGS "
									"WHERE CASH_NUM = %s ORDER BY 1", QData.GetQueryNumber(GetKey()));

	if(!GetOraLoader().Open())
		return;
	
	GetOraLoader().GetFieldArray().GetAt(0)->SetWithComma(FALSE);
	GetOraLoader().GetFieldArray().GetAt(1)->SetWithComma(FALSE);
	GetOraLoader().GetFieldArray().GetAt(2)->SetPrecision(2);
	GetOraLoader().LoadRowCtrl(GetSRowCtrl(), TRUE, TRUE);
}

int CCashData::LoadDBData()
{
	CDBRec Rec;
	CStringArray QRec;
	CQData QData;

	if(!IsReady())
		return -1;

	if(GetSS()->GetFieldArray().GetSize() <= 0 || GetInit())
	{
		GetOraLoader().GetSql() = "SELECT CASH_NUM, PORTFOLIO, ACC_CODE, PMNT_TYPE, CURRENCY, "
									"ASS_CODE, CLOSE_DATE, POST_DATE, TO_DATE, AMOUNT, FXRATE, "
									"REMAINING, DIFF_REASON, COMMENTS, PAY_ID, CUST_REFERENCE, "
									"ACC_TO, TO_PORTFOLIO, LINK, SWAP_TRANS_NUM, COUPON_PAID, "
									"BOOK_DATE, "
									"DECODE(SIGN(POST_DATE - TO_DATE('01/'||TO_CHAR(SYSDATE, 'MM/YYYY'))), "
										"-1, 3, DECODE(SIGN(POST_DATE - TRUNC(SYSDATE)), -1, 2, 1)) \"USER_CLASS\", "
									"ASS_CODE FROM SEMAM.NW_CASH_PMNTS WHERE ";
		GetOraLoader().GetSql() += " 1 = 2 ";
		GetOraLoader().Open();
		GetOraLoader().CopyFieldArray(GetSS()->GetFieldArray());
		GetSS()->SetSheetColHeaders();
	}

	if(GetInit())
		return 0;

	UpdateData(&GetRec(), &GetInvRec());
	GetInvRec().SetPostDate(GetRec().GetPostDate());
	GetInvRec().SetCloseDate(GetRec().GetCloseDate());

	ToDBRec(Rec, TRUE);
	if(Rec.GetSize() <= 0)
		return -1;

	if(FindExact())
		GetOraLoader().RecToQueryRec(QRec, Rec, &GetSS()->GetFieldArray());
	else
		GetOraLoader().RecToQueryRec(QRec, Rec, &GetSS()->GetFieldArray(), PERCENT);

	QRec.SetAt(1, QData.GetQueryText(Rec.GetAt(1)));  // Use Portfolio as it is.
	if(GetRec().GetPayType() == "COUPON PAY" || GetRec().GetPayType() == "SWAP INT")
	{
		GetOraLoader().GetSql() = "SELECT CASH_NUM, PORTFOLIO, ACC_CODE, PMNT_TYPE, CURRENCY, "
									"ASS_CODE, CLOSE_DATE, POST_DATE, TO_DATE, AMOUNT, FXRATE, "
									"REMAINING, DIFF_REASON, COMMENTS, PAY_ID, CUST_REFERENCE, "
									"ACC_TO, TO_PORTFOLIO, LINK, SWAP_TRANS_NUM, COUPON_PAID, "
									"BOOK_DATE, "
									"DECODE(SIGN(POST_DATE - TO_DATE('01/'||TO_CHAR(SYSDATE, 'MM/YYYY'))), "
									"-1, 3, DECODE(SIGN(POST_DATE - TRUNC(SYSDATE)), -1, 2, 1)) \"USER_CLASS\", "
									"CASH_ASSET_CODE FROM SEMAM.NW_CASH_COUPON_V WHERE ";
		SetCoupon(TRUE);
	}
	else
	{
		GetOraLoader().GetSql()= "SELECT CASH_NUM, PORTFOLIO, ACC_CODE, PMNT_TYPE, CURRENCY, "
									"ASS_CODE, CLOSE_DATE, POST_DATE, TO_DATE, AMOUNT, FXRATE, "
									"REMAINING, DIFF_REASON, COMMENTS, PAY_ID, CUST_REFERENCE, "
									"ACC_TO, TO_PORTFOLIO, LINK, SWAP_TRANS_NUM, COUPON_PAID, "
									"BOOK_DATE, "
									"DECODE(SIGN(POST_DATE - TO_DATE('01/'||TO_CHAR(SYSDATE, 'MM/YYYY'))), "
										"-1, 3, DECODE(SIGN(POST_DATE - TRUNC(SYSDATE)), -1, 2, 1)) \"USER_CLASS\", "
									"ASS_CODE FROM SEMAM.NW_CASH_PMNTS WHERE ";
		SetCoupon(FALSE);
	}
	
	GetOraLoader().SetSqlSearchInfo(GetOraLoader().GetSql(), QRec, &GetSS()->GetFieldArray(), FALSE, FindExact());
	GetOraLoader().GetSql() += " ORDER BY 2,1 ";

	if(!GetOraLoader().Open())
		return -1;

	if(GetOraLoader().LoadRowCtrl(*this, TRUE, TRUE) > 0)
		SetLoaded(TRUE);
	else
		SetLoaded(FALSE);
	
	return GetSS()->GetSheetRows();
}

BOOL CCashData::AddRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData(&GetRec(), &GetInvRec());
	GetInvRec().SetPostDate(GetRec().GetPostDate());
	GetInvRec().SetCloseDate(GetRec().GetCloseDate());

	GetOraLoader().BeginTransaction();
	if(GetRec().AddRec(GetOraLoader()) && AddSRows())
	{
		if(GetCashStatus() == 2)  // Payment Instruction
		{
			CPaymentRec PayRec;
			
			PayRec.SetPayID(GetRec().GetPayID());
			PayRec.Closing(GetOraLoader());
		}

		GetOraLoader().Commit(); // Successfully Updated
		AddSheetRow();
		LoadSheet();
		return TRUE;
	}

	GetOraLoader().Rollback();
	return FALSE;
}

BOOL CCashData::DeleteRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData(&GetRec(), &GetInvRec());
	GetInvRec().SetPostDate(GetRec().GetPostDate());
	GetInvRec().SetCloseDate(GetRec().GetCloseDate());

	if(strlen(GetKey()) == 0)
		return FALSE;

	if(GetCurrentRow() > 0 && GetSS()->GetIsBlockSelected())
	{
		GetInvRec().SetCashNum(GetKey());

		GetOraLoader().BeginTransaction();
		if(!GetInvRec().DeleteRec(GetOraLoader(), FALSE))
		{
			if(GetCashStatus() == 2) // Payment Instruction.
			{
				CPaymentRec PayRec;
				
				PayRec.SetPayID(GetRec().GetPayID());
				PayRec.Closing(GetOraLoader(), FALSE);
			}

			GetOraLoader().Rollback();
			return FALSE;
		}

		if(GetRec().DeleteRec(GetOraLoader()))
		{
			GetOraLoader().Commit();
			DeleteSheetRow(GetCurrentRow());
			SetCurrentRow(-1);
			Refresh();
			return TRUE;
		}
			
		GetOraLoader().Rollback();
		return FALSE;
	}
	else
		return DeleteSRow();
}

BOOL CCashData::UpdateRow()
{
	if(!IsReady())
		return FALSE;

	if(strlen(GetKey()) == 0)
		return FALSE;

	UpdateData(&GetRec(), &GetInvRec());
	GetInvRec().SetPostDate(GetRec().GetPostDate());
	GetInvRec().SetCloseDate(GetRec().GetCloseDate());

	if(GetModify(FALSE))
	{
		GetOraLoader().BeginTransaction();
		GetInvRec().SetCashNum(GetKey());
		for(int i = 1; i <= GetSRowCtrl().GetSS()->GetSheetRows(); i ++)
		{
			GetSRowCtrl().SetCurrentRow(i);
			GetSRowCtrl().UpdateSheetRow(TRUE);
			GetInvRec().SetID(GetSRowCtrl().GetID());
			if(strlen(GetInvRec().GetID()) <= 0)
				GetInvRec().AddRec(GetOraLoader());
			else
				GetInvRec().UpdateRec(GetOraLoader(), TRUE);
		}

		if(GetRec().UpdateRec(GetOraLoader())) // Successfully Updated
		{
			GetOraLoader().Commit();
			UpdateSheetRow(FALSE);
			SetModify(FALSE);
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

BOOL CCashData::UpdateInvData()
{
	if(!IsReady())
		return FALSE;
	
	if(strlen(GetKey()) == 0)
		return FALSE;

	if(strlen(GetInvRec().GetID()) == 0)
		return TRUE;
	else
	{
		CInvRec InvRec;

		InvRec.SetID(GetInvRec().GetID());
		InvRec.SetTransNum(GetInvRec().GetTransNum());
		return InvRec.Closing(GetOraLoader(), GetInvRec().GetCloseDate(), GetInvRec().GetPostDate());
	}
}

BOOL CCashData::AddSRow()
{
	if(!IsReady())
		return FALSE;

	if(strlen(GetKey()) == 0)
		return FALSE;

	GetInvRec().SetCashNum(GetKey());
	GetOraLoader().BeginTransaction();
	if(GetInvRec().AddRec(GetOraLoader()))
	{
		GetOraLoader().Commit();
		LoadSheet();
		return TRUE;
	}
	
	GetOraLoader().Rollback();
	return FALSE;
}

BOOL CCashData::DeleteSRow(int CurrentRow)
{
	if(!IsReady())
		return FALSE;

	if(GetSRowCtrl().GetCurrentRow() > 0 && GetSRowCtrl().GetSS()->GetIsBlockSelected())
	{
		if(strlen(GetKey()) == 0 || strlen(GetInvRec().GetID()) == 0)
			return FALSE;

		GetInvRec().SetCashNum(GetKey());

		GetOraLoader().BeginTransaction();
		if(GetInvRec().DeleteRec(GetOraLoader()))
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

BOOL CCashData::UpdateSRow()
{
	if(!IsReady())
		return FALSE;

	if(strlen(GetKey()) == 0 || strlen(GetInvRec().GetID()) == 0)
		return FALSE;

	if(GetSRowCtrl().GetModify(FALSE))
	{
		GetInvRec().SetCashNum(GetKey());

		GetOraLoader().BeginTransaction();
		if(GetInvRec().UpdateRec(GetOraLoader()))
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


BOOL CCashData::AddSRows()
{
	int i;
	
	for(i = 1; i <= GetSRowCtrl().GetSS()->GetSheetRows(); i ++)
	{
		GetInvRec().SetCashNum(GetKey());
		GetInvRec().SetID(GetSRowCtrl().GetSS()->GetSheetText(1, i));
		GetInvRec().SetTransNum(GetSRowCtrl().GetSS()->GetSheetText(2, i));
		GetInvRec().SetAmount(GetSRowCtrl().GetSS()->GetSheetText(3, i));
		if(!GetInvRec().AddRec(GetOraLoader()))
			return FALSE;
	}
	
	return TRUE;
}

BOOL CCashData::DeleteSRows()
{
	GetInvRec().SetCashNum(GetKey());
	return GetInvRec().DeleteRec(GetOraLoader(), FALSE);
}

