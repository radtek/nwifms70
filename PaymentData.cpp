#include "stdafx.h"
#include "stdlib.h"
#include "math.h"
#include "resource.h"
#include "paymentdata.h"
#include "sqlstmt.h"
#include "qdata.h"


IMPLEMENT_DYNAMIC(CPayTemplate, CRowCtrl)
BOOL CPayTemplate::IsReady()
{
	return CRowCtrl::IsReady() && GetOraLoader().GetDB() ? TRUE : FALSE;
}

int CPayTemplate::LoadBank(COptComboBox &Bank)
{
	if(!IsReady())
		return -1;

	if(!GetOraLoader().Open("SELECT BANK FROM SEMAM.NW_BANKS ORDER BY 1"))
		return -1;

	return GetOraLoader().LoadComboBox(Bank);
}

BOOL CPayTemplate::LoadDBData()
{
	if(!IsReady())
		return FALSE;

	GetOraLoader().GetSql() = "SELECT REF_ID, COUNTERPARTY, CURRENCY, BANK, ACCOUNT, "
						"CP_REF, BENEFICIARY, BENEFICIARY_ID, BENEFICIARY_BIC, "
						"BENEFICIARY_ACCOUNT, FFC, FFC_ID, FFC_BIC, FFC_ACCOUNT, "
						"ACTIVE FROM SEMAM.NW_PAY_TEMPLATE "
						"WHERE ACTIVE = 'Y' ORDER BY 1 ";
	if(!GetOraLoader().Open())
		return FALSE;
	return GetOraLoader().LoadRowCtrl(*this, TRUE, TRUE);
}

BOOL CPayTemplate::AddRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData();
	GetOraLoader().BeginTransaction();
	if(GetRec().AddRec(GetOraLoader()))
	{
		GetOraLoader().Commit();
		AddSheetRow();
		return TRUE;
	}

	GetOraLoader().Rollback();
	return FALSE;
}

BOOL CPayTemplate::DeleteRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData();
	GetOraLoader().BeginTransaction();
	if(GetRec().DeleteRec(GetOraLoader()))
	{
		GetOraLoader().Commit();
		DeleteSheetRow(GetCurrentRow());
		return TRUE;
	}

	GetOraLoader().Rollback();
	return FALSE;
}

BOOL CPayTemplate::UpdateRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData();
	GetOraLoader().BeginTransaction();
	if(GetRec().UpdateRec(GetOraLoader()))
	{
		GetOraLoader().Commit();
		UpdateSheetRow(FALSE);
		return TRUE;
	}

	GetOraLoader().Rollback();
	return FALSE;
}

BOOL CPayTemplate::AddSRows(const CString RefID, CStringArray &FundArray)
{
	CQData QData;
	CString Text;
	
	DeleteSRows(RefID);
	Text = QData.GetQueryText(GetRec().GetRefID());

	for(int i = 0; i < FundArray.GetSize(); i ++)
		if(!GetOraLoader().ExecuteSql("INSERT INTO SEMAM.NW_PAY_TEMPLATE_FUND (REF_ID, PORTFOLIO) "
							"VALUES (" + Text + ", " + QData.GetQueryText(FundArray.GetAt(i)) + ") "))
			return FALSE;
	return TRUE;
}

BOOL CPayTemplate::DeleteSRows(const CString RefID)
{
	CQData QData;
	CString Text;

	Text = QData.GetQueryText(GetRec().GetRefID());
	return GetOraLoader().ExecuteSql("DELETE SEMAM.NW_PAY_TEMPLATE_FUND WHERE REF_ID = " + Text);
}





IMPLEMENT_DYNAMIC(CPaymentData, CMSRowCtrl)
/* int CPaymentData::LoadInstructions(BOOL bLoad)
{
	UpdateData(&GetPayRec(), &GetPayInvRec());
	if(GetOraLoader().Open(IDS_SQL_180))
	{
		GetOraLoader().CopyFieldArray(GetPayInstruct().GetSS()->GetFieldArray());
		GetPayInstruct().GetSS()->SetSheetColHeaders();
	}

	if(!bLoad)
		return 0;

	return GetOraLoader().LoadRowCtrl(GetPayInstruct(), TRUE, TRUE);
}  */

int CPaymentData::LoadPortArr()
{
	if(!IsReady())
		return -1;

	m_PortArr.RemoveAll();
	if(!GetOraLoader().Open("SELECT DISTINCT PORTFOLIO FROM SEMAM.NW_PAY_RESTRICTION ORDER BY 1"))
		return -1;

	return GetOraLoader().LoadStringArray(m_PortArr);
}

int CPaymentData::LoadTemplates(const char *Portfolio, BOOL bLoad)
{
	CQData QData;
	BOOL bRestricted = FALSE;

	UpdateData(&GetPayRec(), &GetPayInvRec());

	for (int i = 0; i < m_PortArr.GetSize(); i++)
	{
		if(m_PortArr.GetAt(i) == Portfolio)
			bRestricted = TRUE;
	}

	if(bRestricted)
		GetOraLoader().GetSql().Format("SELECT REF_ID, COUNTERPARTY, CURRENCY, A.BANK, "
							"BANK_ID_CODE, BANK_BIC, ACCOUNT, BENEFICIARY, BENEFICIARY_ID, "
							"BENEFICIARY_BIC, BENEFICIARY_ACCOUNT, FFC, FFC_ID, FFC_BIC, "
							"FFC_ACCOUNT, CP_REF "
							"FROM SEMAM.NW_PAY_TEMPLATE A "
							"JOIN SEMAM.NW_BANKS B ON (A.BANK = B.BANK) "
							"JOIN SEMAM.NW_PAY_RESTRICTION C ON (A.REF_ID = C.REF_ID "
																"AND C.PORTFOLIO = %s) "
							"ORDER BY 2 ", QData.GetQueryText(Portfolio));
	else
		GetOraLoader().GetSql() = "SELECT REF_ID, COUNTERPARTY, CURRENCY, A.BANK, BANK_ID_CODE, "
							"BANK_BIC, ACCOUNT, BENEFICIARY, BENEFICIARY_ID, BENEFICIARY_BIC, "
							"BENEFICIARY_ACCOUNT, FFC, FFC_ID, FFC_BIC, FFC_ACCOUNT, CP_REF "
							"FROM SEMAM.NW_PAY_TEMPLATE A "
							"JOIN SEMAM.NW_BANKS B ON (A.BANK = B.BANK) "
							"ORDER BY 2 ";
	if(GetOraLoader().Open())
	{
		GetOraLoader().CopyFieldArray(GetPayTemplate().GetSS()->GetFieldArray());
		GetPayTemplate().GetSS()->SetSheetColHeaders();
	}

	if(!bLoad)
		return 0;

	return GetOraLoader().LoadRowCtrl(GetPayTemplate(), TRUE, TRUE);
}

int CPaymentData::LoadTemplates(BOOL bLoad)
{
	UpdateData(&GetPayRec(), &GetPayInvRec());

	if(!GetOraLoader().Open("SELECT REF_ID, COUNTERPARTY, CURRENCY, A.BANK, BANK_ID_CODE, "
							"BANK_BIC, ACCOUNT, BENEFICIARY, BENEFICIARY_ID, BENEFICIARY_BIC, "
							"BENEFICIARY_ACCOUNT, FFC, FFC_ID, FFC_BIC, FFC_ACCOUNT, CP_REF "
							"FROM SEMAM.NW_PAY_TEMPLATE A "
							"JOIN SEMAM.NW_BANKS B ON (A.BANK = B.BANK) "
							"ORDER BY 2 "))
	{
		GetOraLoader().CopyFieldArray(GetPayTemplate().GetSS()->GetFieldArray());
		GetPayTemplate().GetSS()->SetSheetColHeaders();
	}

	if(!bLoad)
		return 0;

	return GetOraLoader().LoadRowCtrl(GetPayTemplate(), TRUE, TRUE);
}

int CPaymentData::LoadTrades(CDBSheet &SS, BOOL bLoad)
{
	CString Date, Currency, CP;
	CQData QData;

	UpdateData(&GetPayRec(), &GetPayInvRec());
	if(!bLoad)
		GetOraLoader().GetSql() = "SELECT PORTFOLIO,  VALUE_DATE, ASS_DESC, TRANS_NUM, "
									"INV_NUM, CURRENCY, "
									"DECODE(DIR, 'P', -1, 'S', 1)*(NVL(AMOUNT, 0)+NVL(INTEREST, 0)) \"AMOUNT\" "
									"FROM SEMAM.NW_OPEN_TRADE_2 A, SEMAM.NW_ASSETS B "
									"WHERE A.ASS_CODE = B.ASS_CODE ";
	else
	{
		Date = QData.GetQueryDate(GetPayRec().GetValueDate());
		Currency = QData.GetQueryText(GetPayRec().GetCurrency());
		CP = QData.GetQueryText(GetPayRec().GetCP());
		GetOraLoader().GetSql().Format("SELECT PORTFOLIO, VALUE_DATE, ASS_DESC, "
										"TRANS_NUM, INV_NUM, PAY_CUR \"CURRENCY\", "
										"DECODE(DIR, 'P', -1, 'S', 1)*(NVL(AMOUNT, 0) + NVL(INTEREST, 0)) \"AMOUNT\" "
										"FROM SEMAM.NW_OPEN_TRADE_2 A, SEMAM.NW_ASSETS B "
										"WHERE A.ASS_CODE = B.ASS_CODE(+) "
										"AND TRANS_TYPE IN ('SECURITIES', 'CALL', 'PUT') "
										"AND DIR IN ('P', 'S') AND VALUE_DATE <= %s "
										"AND COUNTERPARTY = %s AND PAY_CUR = %s "
										"AND 0 = (SELECT COUNT(*) FROM SEMAM.NW_PAYMENT_DETAILS D "
													"WHERE D.INV_NUM = A.INV_NUM) "
										"AND 0 = (SELECT COUNT(*) FROM SEMAM.NW_TR_INV E "
													"WHERE E.INV_NUM = A.INV_NUM "
													"AND E.ACTUAL_CLOSING IS NOT NULL ) "
										"UNION "
										"SELECT A.PORTFOLIO, "
										"DECODE(TR_DESC, 'MATURITY', A.MATURITY_DATE, A.VALUE_DATE), "
										"ASS_DESC, A.TRANS_NUM, INV_NUM, PAY_CUR, "
										"DECODE(A.TRANS_TYPE, 'LEVERAGE', -1, 'INT. SWAP', -1, 1)*"
											"DECODE(TR_DESC, 'MATURITY', -1, 'FX-BKNG', -1, 1)*DECODE(DIR, 'P', -1, 'S', 1)* "
											"(NVL(AMOUNT,0) + NVL(INTEREST,0)) \"AMOUNT\" "
										"FROM SEMAM.NW_OPEN_TRADE_2 A, SEMAM.NW_ASSETS B, "
										"SEMAM.NW_TR_TICKETS C "
										"WHERE A.ASS_CODE = B.ASS_CODE(+) "
										"AND A.TRANS_NUM = C.TRANS_NUM "
										"AND (A.TRANS_TYPE NOT IN ('SECURITIES', 'CALL', 'PUT') OR "
												"A.TRANS_TYPE IN ('SECURITIES', 'CALL', 'PUT') "
												"AND A.DIR NOT IN ('P', 'S')) "
										"AND DECODE(TR_DESC, 'MATURITY', A.MATURITY_DATE, A.VALUE_DATE) <= %s "
										"AND A.COUNTERPARTY = %s AND A.PAY_CUR = %s "
										"AND 0 = (SELECT COUNT(*) FROM SEMAM.NW_PAYMENT_DETAILS D "
													"WHERE D.INV_NUM = A.INV_NUM) "
										"AND 0 = (SELECT COUNT(*) FROM SEMAM.NW_TR_INV E "
													"WHERE E.INV_NUM = A.INV_NUM "
													"AND E.ACTUAL_CLOSING IS NOT NULL ) "
										"ORDER BY 1, 2, 3, 4, 5 ", (LPCTSTR) Date, (LPCTSTR) CP, (LPCTSTR) Currency, 
										(LPCTSTR) Date, (LPCTSTR) CP, (LPCTSTR) Currency);
	}

	if(GetOraLoader().Open())
	{
		GetOraLoader().GetFieldArray().GetAt(3)->SetWithComma(FALSE);
		GetOraLoader().GetFieldArray().GetAt(4)->SetWithComma(FALSE);
		GetOraLoader().GetFieldArray().GetAt(6)->SetPrecision(2);
		GetOraLoader().CopyFieldArray(SS.GetFieldArray());
		SS.SetSheetColHeaders();
	}
	
	if(bLoad)
		return GetOraLoader().LoadDBSheet(SS, TRUE);

	return 0;
}

void CPaymentData::LoadSheet()
{
	if(!IsReady())
		return;

	CQData QData;

	SetKey();
	SetbCoupon(GetPayRec().GetCashType() == "COUPON PAY" ||
				GetPayRec().GetCashType() == "SWAP INT");
	
	if(GetbCoupon())
		GetOraLoader().GetSql().Format("SELECT ASSET_CODE, TO_DATE, AMOUNT, "
					"TRANS_NUM, INV_NUM, FXRATE, REMAINING, COUPON_PAID "
					"FROM SEMAM.NW_PAYMENT_COUPON WHERE PAY_ID = %s "
					"ORDER BY 1, 2 ", QData.GetQueryNumber(GetKey()));
	else
		GetOraLoader().GetSql().Format("SELECT INV_NUM, TRANS_NUM, AMOUNT "
										"FROM SEMAM.NW_PAYMENT_DETAILS "
										"WHERE PAY_ID = %s ORDER BY TRANS_NUM ", 
										QData.GetQueryNumber(GetKey()));
	if(GetOraLoader().Open())
	{
		if(GetbCoupon())
		{
			GetOraLoader().GetFieldArray().GetAt(3)->SetWithComma(FALSE);
			GetOraLoader().GetFieldArray().GetAt(4)->SetWithComma(FALSE);			
		}
		else
		{
			GetOraLoader().GetFieldArray().GetAt(0)->SetWithComma(FALSE);
			GetOraLoader().GetFieldArray().GetAt(1)->SetWithComma(FALSE);
		}
		
		GetOraLoader().GetFieldArray().GetAt(2)->SetWithComma(TRUE);
		GetOraLoader().GetFieldArray().GetAt(2)->SetPrecision(2);
	
		GetOraLoader().LoadRowCtrl(GetSRowCtrl(), TRUE, TRUE);
	}
}

void CPaymentData::SetHeaders()
{
	if(!IsReady())
		return;

	LoadTemplates();

	if(GetOraLoader().Open("SELECT INV_NUM, PAY_ID, TRANS_NUM, AMOUNT "
							"FROM SEMAM.NW_PAYMENT_DETAILS "))
	{		
		GetOraLoader().CopyFieldArray(GetSRowCtrl().GetSS()->GetFieldArray());
		GetSRowCtrl().GetSS()->GetFieldArray().RemoveAt(1); // Remove PayID
		GetSRowCtrl().GetSS()->SetSheetColHeaders();
	}
}

BOOL CPaymentData::LoadDBData()
{
	CDBRec Rec;
	CStringArray QRec;

	if(!IsReady())
		return FALSE;

	UpdateData(&GetPayRec(), &GetPayInvRec());
	GetPayRec().ToDBRec(Rec);
	if(Rec.GetSize() <= 0)
		return FALSE;

	if(GetSS()->GetFieldArray().GetSize() <= 0)
	{
		GetOraLoader().GetSql() = "SELECT PAY_ID, REF_ID, PORTFOLIO, ACC_CODE, COUNTERPARTY, "
							"DECODE(DIR, 'P', 'PAYMENT', 'RECEIPT') \"DIR\", INDATE, CURRENCY, "
							"DECODE(DIR, 'P', 1, 'R', -1)*AMOUNT, ADJUSTMENT, NOTE, VALUE_DATE, "
							"PMNT_TYPE, ASSET_CODE, CP_REF_NOTE, PAYMENT_MADE_FLAG, TO_ACCOUNT, "
							"FXRATE, TO_NUMBER(NULL) \"DELETABLE\" "
							"FROM SEMAM.NW_PAYMENTS "
							"WHERE 1 = 2";
		if(!GetOraLoader().Open())
			return FALSE;
		GetOraLoader().CopyFieldArray(GetSS()->GetFieldArray());
		GetSS()->SetSheetColHeaders();
	}

	GetOraLoader().RecToQueryRec(QRec, Rec, &GetSS()->GetFieldArray(), "%");
	if(QRec.GetSize() <= 0)
		return FALSE;
	GetOraLoader().GetSql() = "SELECT PAY_ID, REF_ID, PORTFOLIO, ACC_CODE, COUNTERPARTY, "
							"DECODE(DIR, 'P', 'PAYMENT', 'RECEIPT') \"DIR\", INDATE, "
							"CURRENCY, DECODE(DIR, 'P', 1, 'R', -1)*AMOUNT, ADJUSTMENT, "
							"NOTE, VALUE_DATE, PMNT_TYPE, ASSET_CODE, CP_REF_NOTE, "
							"PAYMENT_MADE_FLAG, TO_ACCOUNT, FXRATE, "
							"DECODE(PAYMENT_MADE_FLAG, 'Y', -1, "
							"DECODE(SIGN(BOOK_DATE - TRUNC(SYSDATE)), -1, -1, 1)) "
							"FROM SEMAM.NW_PAYMENTS WHERE ";
	GetOraLoader().SetSqlSearchInfo(GetOraLoader().GetSql(), QRec, &GetSS()->GetFieldArray(), FALSE);
	GetOraLoader().GetSql() += " ORDER BY PORTFOLIO, ACC_CODE, CURRENCY, DIR, VALUE_DATE, PAY_ID ";

	if(!GetOraLoader().Open())
		return FALSE;
	return GetOraLoader().LoadRowCtrl(*this, TRUE, TRUE);
}

BOOL CPaymentData::DeleteRow()
{
	if(!IsReady())
		return FALSE;
	
	SetbCoupon(GetPayRec().GetCashType() == "COUPON PAY" || 
				GetPayRec().GetCashType() == "SWAP INT");

	if(GetbCoupon())
	{
		UpdateData(&GetPayRec(), &GetPayInvCouponRec());
		GetPayInvCouponRec().SetPortfolio(GetPayRec().GetPortfolio());
		GetPayInvRec().SetPayID(GetKey());
	}
	else
	{
		UpdateData(&GetPayRec(), &GetPayInvRec());
		GetPayInvCouponRec().SetPayID(GetKey());
	}

	GetOraLoader().BeginTransaction();
	if(GetbCoupon())
		GetPayInvCouponRec().DeleteRec(GetOraLoader(), FALSE);
	
	if(GetPayRec().DeleteRec(GetOraLoader()))
	{
		GetOraLoader().Commit();
		DeleteSheetRow(GetCurrentRow());
		Refresh();
		GetSRowCtrl().GetSS()->ClearSheet();
		return TRUE;
	}

	GetOraLoader().Rollback();
	return FALSE;
}

BOOL CPaymentData::UpdateRow()
{
	BOOL bInDatabase, bOK;

	bInDatabase = strlen(GetKey());

	if(GetbCoupon())
	{
		UpdateData(&GetPayRec(), &GetPayInvCouponRec());
		GetPayInvCouponRec().SetPortfolio(GetPayRec().GetPortfolio());
	}
	else
	{
		UpdateData(&GetPayRec(), &GetPayInvRec());
	}
	if(!IsOK())
	{
		GetPayRec().SetPayID(EMPTYSTRING);
		return FALSE;
	}

	GetOraLoader().BeginTransaction();
	if(bInDatabase)
		bOK = GetPayRec().UpdateRec(GetOraLoader());
	else
		bOK = GetPayRec().AddRec(GetOraLoader());

	if(!bOK)
	{
		GetOraLoader().Rollback();
		return FALSE;
	}

	if(GetbCoupon())
	{
		GetPayInvCouponRec().SetPayID(GetKey());
		if(bInDatabase)
			GetPayInvCouponRec().DeleteRec(GetOraLoader()); 
	}
	else
	{
		GetPayInvRec().SetPayID(GetKey());
		if(bInDatabase)
			GetPayInvRec().DeleteRec(GetOraLoader());
	}

	for(int i = 1; i <= GetSRowCtrl().GetSS()->GetSheetRows(); i ++)
	{
		if(GetbCoupon())
		{
			GetPayInvCouponRec().SetAsset(GetSRowCtrl().GetSS()->GetSheetText(1, i));
			GetPayInvCouponRec().SetPayID(GetKey());
			GetPayInvCouponRec().SetDate(GetSRowCtrl().GetSS()->GetSheetText(2, i));
			GetPayInvCouponRec().SetAmount(GetSRowCtrl().GetSS()->GetSheetText(3, i));
			GetPayInvCouponRec().SetTransNum(GetSRowCtrl().GetSS()->GetSheetText(4, i));
			GetPayInvCouponRec().SetInvNum(GetSRowCtrl().GetSS()->GetSheetText(5, i));
			GetPayInvCouponRec().SetFxrate(GetSRowCtrl().GetSS()->GetSheetText(6, i));
			GetPayInvCouponRec().SetRemaining(GetSRowCtrl().GetSS()->GetSheetText(7, i));
			GetPayInvCouponRec().SetCouponPaid(GetSRowCtrl().GetSS()->GetSheetText(8, i));
			if(!GetPayInvCouponRec().AddRec(GetOraLoader()))
			{
				GetOraLoader().Rollback();
				return FALSE;
			} 
		}
		else
		{
			GetSRowCtrl().SetCurrentRow(i);
			if(!GetPayInvRec().AddRec(GetOraLoader()))
			{
				GetOraLoader().Rollback();
				return FALSE;
			}
		}
	}

	GetOraLoader().Commit();
	if(bInDatabase)
		UpdateSheetRow(FALSE);
	else
		AddSheetRow();

	return TRUE;
}

double CPaymentData::ComputeSum(CDBSheet &SS, CString &Amount)
{
	double Amt = 0;
	CQData QData;

	for(int i = 1; i <= SS.GetSheetRows(); i ++)
	{
		Amt += atof(QData.RemoveComma(SS.GetSheetText(3, i)));
	}
	
	Amt *= -1;
	Amount = QData.WriteNumber(Amt, TRUE, 2);

	return Amt;
}

BOOL CPaymentData::IsOK()
{
	CString CashType;

	if(GetPayRec().GetPayID().IsEmpty())
	{
		MessageBox(NULL, "No PayID Generated", NULL, MB_OK);
		return FALSE;
	}

	if(GetPayRec().GetDir() == "PAYMENT" && GetPayRec().GetRefID().IsEmpty())
	{
		MessageBox(NULL, "No Counterparty Instruction selected", NULL, MB_OK);
		return FALSE;
	}

	if(GetPayRec().GetPortfolio().IsEmpty())
	{
		MessageBox(NULL, "No Portfolio selected", NULL, MB_OK);
		return FALSE;
	}

	if(GetPayRec().GetAmount().IsEmpty())
	{
		MessageBox(NULL, "Amount is missing", NULL, MB_OK);
		return FALSE;
	}

	if(GetPayRec().GetCurrency().IsEmpty())
	{
		MessageBox(NULL, "Currency is missing", NULL, MB_OK);
		return FALSE;
	}

	if(GetPayRec().GetDir() == "PAYMENT")
	{
		if(GetPayRec().GetAmount().Find('-', 0) >= 0)
		{
			MessageBox(NULL, "Payment Amount cannot be negative", NULL, MB_OK);
			return FALSE;
		}
	}
	else
		if(GetPayRec().GetAmount().Find('-', 0) < 0)
		{
			MessageBox(NULL, "Receipt Amount cannot be positive", NULL, MB_OK);
			return FALSE;
		}

	CashType = GetPayRec().GetCashType();
	if(CashType.IsEmpty())
	{
		if(!GetPayRec().GetAssetCode().IsEmpty() || !GetPayRec().GetToAccount().IsEmpty() ||
			!GetPayRec().GetFxrate().IsEmpty())
		{
			MessageBox(NULL, "You cannot use Asset, Fxrate and ToAccount Fields without a payment type", NULL, MB_OK);
			return FALSE;
		}
	}

	if(CashType == "INTEREST" || CashType == "DIVIDENT P" ||
		CashType == "AMORTIZATI" || CashType == "TAX" || CashType == "FUTU COST" || CashType == "COMMISSION")
	{
		if(GetPayRec().GetAssetCode().IsEmpty())
		{
			MessageBox(NULL, "Asset Code is missing", NULL, MB_OK);
			return FALSE;
		}
	}

	if((CashType == "COUPON PAY" || CashType == "SWAP INT") && GetSRowCtrl().GetSS()->GetSheetRows() <= 0)
	{
		MessageBox(NULL, "No coupon payment detail selectd", NULL, MB_OK);
		return FALSE;
	}

	if(CashType == "TRANSFER")
		if(GetPayRec().GetToAccount().IsEmpty())
		{
			MessageBox(NULL, "To Account is missing", NULL, MB_OK);
			return FALSE;
		}

	return TRUE;
}

void CPaymentData::UpdateLines(CStdioFile &PayFile, int &Lines, int AddLines)
{
	Lines += AddLines;
	if(Lines > 68)
	{
		PayFile.WriteString("\f\n");
		Lines = 1;
	}
}

BOOL CPaymentData::ReptOpenFile(CStdioFile &PayFile)
{
	CFileException e;
	char t[1024], FileName[1024];

	FileName[0] = 0;
	GetEnvironmentVariable("FMS_DATA", t, sizeof(t));	
	SetCurrentDirectory(t);
	sprintf_s(FileName, 1024, "%s\\PayFax.doc", t);
	if(!PayFile.Open(FileName, CFile::modeCreate | CFile::modeWrite, &e))
	{
		e.GetErrorMessage(t, sizeof(t) - 1);
		MessageBox(NULL, t, "File could not be opened ", MB_OK);
		return FALSE;
	}

	return TRUE;
}

int CPaymentData::FindAccount(const char *Portfolio, const char *Account)
{
	int i;
	CDBRec *pDataRec;

	for(i = 0; i < m_CustodianArray.GetSize(); i ++)
	{
		pDataRec = m_CustodianArray.GetAt(i);
		if(pDataRec && 
			strcmp(pDataRec->GetAt(0), Portfolio) == 0 &&
			strcmp(pDataRec->GetAt(5), Account) == 0)
			return i;
	}

	return -1;
}

int CPaymentData::ReptWriteHeading(CStdioFile &PayFile)
{
	int n, i;
	CString Text, Date;
	
	GetOraLoader().Today(Date);

	i = FindAccount(GetPayRec().GetPortfolio(), GetPayRec().GetAccount());
	if(i < 0)
		return -1;

	Text.Format("%20.20sNWI MANAGEMENT, LP\n", " ");
	PayFile.WriteString(Text);
	PayFile.WriteString("--------------------------------------------------------------------\n");
	
	Text.Format("To:%17.17s%s\n", " ", m_CustodianArray.GetAt(i)->GetAt(1));
	PayFile.WriteString(Text);
	Text.Format("Attention:%10.10s%s\n", " ", m_CustodianArray.GetAt(i)->GetAt(2));
	PayFile.WriteString(Text);
	Text.Format("Fax Number:%9.9s%s\n", " ", m_CustodianArray.GetAt(i)->GetAt(4));
	PayFile.WriteString(Text);
	if(strlen(m_CustodianArray.GetAt(i)->GetAt(3)) > 0)
	{
		Text.Format("Phone Number:%7.7s%s\n", " ", m_CustodianArray.GetAt(i)->GetAt(3));
		PayFile.WriteString(Text);
		n = 15;
	}
	else
		n = 14;
	Text.Format("Date:%15.15s%s\n", " ", (const char *) Date);
	PayFile.WriteString(Text);
	Text.Format("Re:%17.17s%s A/C %s\n", " ", (const char *) GetPayRec().GetPortfolio(), 
				m_CustodianArray.GetAt(i)->GetAt(6));
	PayFile.WriteString(Text);

	Text.Format("Subject:%12.12sPAYMENTS/RECEIPTS\n", " ");
	PayFile.WriteString(Text);
	PayFile.WriteString("--------------------------------------------------------------------\n\n\n");

	PayFile.WriteString("Please arrange to pay/receive as follows:\n\n\n");
	return n;
}

int CPaymentData::ReptPayInstruction(CStdioFile &PayFile)
{
	int CurrRow, Lines = 0;
	BOOL bFound = FALSE;
	CPayTemplateRec *pRec;
	CString Text;

	CurrRow = GetPayTemplate().GetCurrentRow();
	pRec = &GetPayTemplate().GetRec();

	for(int i = 1; i <= GetPayTemplate().GetSS()->GetSheetRows() && !bFound; i ++)
	{
		GetPayTemplate().SetCurrentRow(i);
		if(GetPayRec().GetRefID() == pRec->GetRefID())
			bFound = TRUE;
	}

	if(bFound)
	{
		if(!pRec->GetBank().IsEmpty())
		{
			if(!pRec->GetBankID().IsEmpty())
				Text.Format("%-s, %-s\n", (const char *) pRec->GetBank(), (const char *) pRec->GetBankID());
			else
				Text.Format("%-s, %-s\n", (const char *) pRec->GetBank(), (const char *) pRec->GetBankBIC());
			PayFile.WriteString(Text);
		}

		Text.Format("%45.45sA/C of %s\n", " ", (const char *) pRec->GetBeneficiary());
		PayFile.WriteString(Text);
		Lines += 2;

		if(!pRec->GetBeneficiaryID().IsEmpty() || !pRec->GetBeneficiaryBIC().IsEmpty() || !pRec->GetBeneficiaryAccount().IsEmpty())
		{
			if(!pRec->GetBeneficiaryID().IsEmpty())
				Text.Format("%45.45sBeneficiary ID: %s\n", " ", (const char *) pRec->GetBeneficiaryID());
			else
				Text.Format("%45.45sBeneficiary ID: %s\n", " ", (const char *) pRec->GetBeneficiaryBIC());
			PayFile.WriteString(Text);
			Lines ++;
			if(!pRec->GetBeneficiaryAccount().IsEmpty())
			{
				Text.Format("%45.45sBeneficiary Account: %s\n", " ", (const char *) pRec->GetBeneficiaryAccount());
				PayFile.WriteString(Text);
				Lines ++;
			}
		}

		Text.Format("%45.45sA/C#: %s\n", " ", (const char *) pRec->GetAccount());
		PayFile.WriteString(Text);
		Lines ++;
		if(!pRec->GetFFC().IsEmpty())
		{
			Text.Format("%45.45sF/C: %s\n", " ", (const char *) pRec->GetFFC());
			PayFile.WriteString(Text);
			Lines ++;

			if(!pRec->GetFFCID().IsEmpty() || !pRec->GetFFCBIC())
			{
				if(!pRec->GetFFCID().IsEmpty())
					Text.Format("%45.45sF/C ID: %s\n", " ", (const char *) pRec->GetFFCID());
				else
					Text.Format("%45.45sF/C ID: %s\n", " ", (const char *) pRec->GetFFCBIC());
				PayFile.WriteString(Text);
				Lines ++;
			}

			if(!pRec->GetFFCAccount().IsEmpty())
			{
				Text.Format("%45.45sF/C Account: %s\n", " ", (const char *) pRec->GetFFCAccount());
				PayFile.WriteString(Text);
				Lines ++;
			}
		}
	}

	GetPayTemplate().SetCurrentRow(CurrRow);
	return Lines;
}

int CPaymentData::ReptOurRef(CStdioFile &PayFile)
{
	CString Text;
	int i;

	PayFile.WriteString("Our Ref ID: " + GetPayRec().GetPayID());

	if(!GetPayRec().GetNote().IsEmpty())
		PayFile.WriteString("\t" + GetPayRec().GetNote());

	if(!GetPayRec().GetCPRef().IsEmpty())
	{
		PayFile.WriteString("\nCounterparty Ref: ");
		PayFile.WriteString("\t" + GetPayRec().GetCPRef());
	}

	Text.Empty();
	for(i = 1; i <= GetSRowCtrl().GetSS()->GetSheetRows(); i ++)
	{
		if(!Text.IsEmpty())
			Text += SLASH;
		Text += GetSRowCtrl().GetSS()->GetSheetText(2, i);
	}
	
	if(!Text.IsEmpty())
		PayFile.WriteString("\t" + Text);

	PayFile.WriteString("\n\n\n");
	return 3;
}

BOOL CPaymentData::ReptEnd(CStdioFile &PayFile, CString &Portfolio)
{
	if(Portfolio.IsEmpty())
		return FALSE;

	if(Portfolio == "EXPLORER")
		PayFile.WriteString("\n\nSigned Fax Confirmation Will Follow.  Please Avoid Duplication\n\n");
	PayFile.WriteString("\nThanks and Regards\n\f\n");
	return TRUE;
}

void CPaymentData::GeneratePayment()
{
	int Start, End, i;
	int Lines = 0;
	double Amount = 0;
	CStdioFile PayFile;
	CString Portfolio, Account, Currency, Dir, Text;
	CDBRec DataRec;
	CQData QData;

	if(GetSS()->GetIsBlockSelected())
	{
		if(GetSS()->GetSelBlockRow() <= GetSS()->GetSheetRows())
			Start = GetSS()->GetSelBlockRow();
		else
			Start = 1;

		if(GetSS()->GetSelBlockRow2() <= GetSS()->GetSheetRows())
			End = GetSS()->GetSelBlockRow2();
		else
			End = GetSS()->GetSheetRows();
	}
	else
	{
		Start = 1;
		End = GetSS()->GetSheetRows();
	}

	if(End < Start)
		return;

	if(!ReptOpenFile(PayFile))
		return;

	m_CustodianArray.RemoveAll();
	GetOraLoader().GetSql() = "SELECT A.PORTFOLIO, CUSTODIAN, CONTACT, PHONE, FAX, ACCOUNT, ACC_NUM "
								"FROM SEMAM.NW_PORTFOLIO_ACCOUNTS A, SEMAM.NW_PORTFOLIOS Z "
								"WHERE A.PORTFOLIO = Z.PORTFOLIO "
								"AND Z.STATUS IS NULL "
								"ORDER BY 1, 6 ";

	GetOraLoader().Open();
	GetOraLoader().MoveFirst();
	while(!GetOraLoader().IsEOF())
	{
		DataRec.RemoveAll();
		Portfolio = GetOraLoader().GetDBString(0);
		Account = GetOraLoader().GetDBString(5);
		DataRec.Add(Portfolio); // Portfolio
		DataRec.Add(GetOraLoader().GetDBString(1)); // Custodian
		DataRec.Add(GetOraLoader().GetDBString(2)); // Contact
		DataRec.Add(GetOraLoader().GetDBString(3)); // Phone
		DataRec.Add(GetOraLoader().GetDBString(4)); // Fax
		DataRec.Add(Account); // Account
		DataRec.Add(GetOraLoader().GetDBString(6)); // Account Number
		GetOraLoader().MoveNext();
		m_CustodianArray.Add(DataRec);
	}

	Portfolio.Empty();
	for(i = Start; i <= End; i ++)
	{
		SetCurrentRow(i);
		if(Portfolio != GetPayRec().GetPortfolio() || Account != GetPayRec().GetAccount() ||
			Currency != GetPayRec().GetCurrency())
		{
			if(!Portfolio.IsEmpty())
			{
				Text.Format("NET %s %s %s %s\n", Portfolio, Amount > 0 ? "PAY" : "RECEIVE", 
							Currency, QData.WriteNumber(fabs(Amount), TRUE, 2));
				PayFile.WriteString(Text);
				UpdateLines(PayFile, Lines, 1);
			}

			if(Portfolio != GetPayRec().GetPortfolio() || Account != GetPayRec().GetAccount())
			{
				if(ReptEnd(PayFile, Portfolio))
					Lines = 0;
				
				UpdateLines(PayFile, Lines, ReptWriteHeading(PayFile));
			}
		
			Portfolio = GetPayRec().GetPortfolio();
			Account = GetPayRec().GetAccount();
			Currency = GetPayRec().GetCurrency();
			Amount = 0;
			Dir.Empty();
		}

		if(GetPayRec().GetDir().Left(1) == "P")
		{
			if(Dir != GetPayRec().GetDir().Left(1))
			{								
				PayFile.WriteString("PAYMENTS\n\n");
				UpdateLines(PayFile, Lines, 2);
				PayFile.WriteString("Currency  Amount              Value Date     Pay To\n\n");
				
				UpdateLines(PayFile, Lines, 2);
				
				if(GetPayTemplate().GetSS()->GetSheetRows()  <= 0)
					LoadTemplates(TRUE);															
				Dir = GetPayRec().GetDir().Left(1);
			}

			Text.Format("%-10.10s%-20.20s%-15.15s", (const char *) GetPayRec().GetCurrency(), 
					QData.Abs(GetPayRec().GetAmount()), (const char *) GetPayRec().GetValueDate());
			PayFile.WriteString(Text);
			UpdateLines(PayFile, Lines, ReptPayInstruction(PayFile));
			UpdateLines(PayFile, Lines, ReptOurRef(PayFile));
			Amount += atof(QData.RemoveComma(GetPayRec().GetAmount()));
		}
		else
		{
			if(Dir != GetPayRec().GetDir().Left(1))
			{
				PayFile.WriteString("RECEIPTS\n\n");
				UpdateLines(PayFile, Lines, 2);
				PayFile.WriteString("Currency  Amount              Value Date     Receive From\n\n");
				UpdateLines(PayFile, Lines, 2);
				if(GetPayTemplate().GetSS()->GetSheetRows()  <= 0)
					LoadTemplates(TRUE);															
				Dir = GetPayRec().GetDir().Left(1);
			}

			Text.Format("%-10.10s%-20.20s%-15.15s%s\n", (const char *) GetPayRec().GetCurrency(), 
					QData.Abs(GetPayRec().GetAmount()), (const char *) GetPayRec().GetValueDate(), 
					(const char *) GetPayRec().GetCP());
			
			PayFile.WriteString(Text);
			UpdateLines(PayFile, Lines, 1);
			UpdateLines(PayFile, Lines, ReptOurRef(PayFile));
			Amount += atof(QData.RemoveComma(GetPayRec().GetAmount()));
		}
	}

	if(!Portfolio.IsEmpty())
	{
		Text.Format("NET %s %s %s %s\n", Portfolio, Amount > 0 ? "PAY" : "RECEIVE", 
					Currency, QData.WriteNumber(fabs(Amount), TRUE, 2));
		PayFile.WriteString(Text);
		UpdateLines(PayFile, Lines, 1);
		ReptEnd(PayFile, Portfolio);
	}
	PayFile.Close();
}