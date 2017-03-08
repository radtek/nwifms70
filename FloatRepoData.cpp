// repodata.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "resource.h"
#include "floatrepodata.h"
#include "sqlstmt.h"
#include "qdata.h"
#include "nwivar.h"
#include "oradate.h"


IMPLEMENT_DYNAMIC(CFloatRepoData, CMSRowCtrl)

void CFloatRepoData::Setup(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS, int KeyIndex)
{
	CMSRowCtrl::Setup(OraLoader, pSS, pSSS, KeyIndex);	
}

BOOL CFloatRepoData::IsReady()
{
	if(!CMSRowCtrl::IsReady())
		return FALSE;
	
	return TRUE;
}

void CFloatRepoData::LoadCP(COptComboBox &CP)
{
	if(!CMSRowCtrl::IsReady())
		return;

	if(GetOraLoader().Open("SELECT DISTINCT COUNTERPARTY FROM SEMAM.NW_TR_TICKETS "
							"WHERE TRANS_TYPE IN('REPO','LEVERAGE') "
							"AND FLOAT_RATE_FORMULA LIKE 'DAILY FLOAT%%' ORDER BY 1"))
		GetOraLoader().LoadComboBox(CP);
}

void CFloatRepoData::LoadDate(COptComboBox &Date)
{
	if(!CMSRowCtrl::IsReady())
		return;
	
	CString Text;

	if(!GetOraLoader().Open("SELECT DISTINCT INT_FROM, INT_TO FROM SEMAM.NW_REPO_RATES "
							"WHERE INT_FROM > ADD_MONTHS(SYSDATE, -3) "
							"ORDER BY 1 DESC, 2 DESC "))
		return;
	
	Date.ResetContent();
	GetOraLoader().MoveFirst();
	while(!GetOraLoader().IsEOF())
	{
		Text = GetOraLoader().GetDBString(0);
		Text += MINUS;
		Text += GetOraLoader().GetDBString(1);
		Date.AddString(Text);
		GetOraLoader().MoveNext();
	}
}

int CFloatRepoData::LoadTickets(int nOrder)
{
	if(!CMSRowCtrl::IsReady() || GetCP().IsEmpty())
		return -1;
	
	CSqlStmt Sql;
	CQData QData;
	CString Text, AssFrom, AssTo, AssetType, RateBasis, LevRateBasis, ValueDate, TransType, Formula;
	double NomAmount, Price, Rate, Amort, CapRate, Plus;
	CDBRec Rec;
	int i, AADays;

	Rec.SetInDatabase(TRUE);
	Rec.SetModify(FALSE);
	Text = QData.GetQueryText(GetCP());
	GetOraLoader().GetSql().Format("SELECT PORTFOLIO, ASS_DESC, TRANS_NUM, ASSET_CODE, NOM_AMOUNT, "
							"CURRENCY, DECODE(TRANS_TYPE, 'REPO', PRICE, 1) \"PRICE\", RATE_BASIS, "
							"VALUE_DATE, MATURITY_DATE, ASS_FROM, ASS_TO, ASS_RATE_BASIS, ASS_TYPE, "
							"TRANS_TYPE, C.RATE, NVL(AMORT_FACT, 1) \"AMORT_FACT\", "
							"NVL(CAPIT_RATE, 0) \"CAP_RATE\", NVL(PLUS_AMOUNT, 0) \"PLUS_AMOUNT\", "
							"ASS_TO - ADD_MONTHS(ASS_TO, ROUND(MONTHS_BETWEEN(ASS_FROM, ASS_TO), 0)) \"AADAYS\", "
							"FLOAT_RATE_FORMULA, TO_NUMBER(NULL) \"AMOUNT\" "
							"FROM SEMAM.NW_TR_TICKETS A "
							"JOIN SEMAM.NW_ASSETS B ON (A.ASSET_CODE = B.ASS_CODE) "
							"LEFT OUTER JOIN SEMAM.NW_ASS_PERIODS C ON (A.ASSET_CODE = C.ASS_CODE "
										"AND ASS_FROM < A.VALUE_DATE AND ASS_TO > A.VALUE_DATE) "
							"WHERE TRANS_TYPE IN ('REPO','LEVERAGE') AND FLOAT_RATE_FORMULA LIKE 'DAILY FLOAT%%' "
							"AND COUNTERPARTY = %s AND (MATURITY_DATE IS NULL OR MATURITY_DATE > TRUNC(SYSDATE)) ", 
							(LPCTSTR) Text);
	if(nOrder > 0)
		Sql.Format("ORDER BY %d ", nOrder);
	else
		Sql.Format("ORDER BY 1, 2, 3 ");	
	GetOraLoader().GetSql() += Sql;

	if(!GetOraLoader().Open())
		return -1;

	ClearSheet();
	GetOraLoader().GetFieldArray().GetAt(2)->SetPrecision(0);	// TransNum
	GetOraLoader().GetFieldArray().GetAt(2)->SetWithComma(FALSE);
	GetOraLoader().GetFieldArray().GetAt(4)->SetPrecision(-1); // Set price field
	GetOraLoader().CopyFieldArray(GetSS()->GetFieldArray());
	GetSS()->SetSheetColHeaders();
	
//	GetSS()->SetReDraw(FALSE);
	GetOraLoader().MoveFirst();
	while(!GetOraLoader().IsEOF())
	{				
		Rec.RemoveAll();
		for(i = 0; i < GetOraLoader().GetFieldCount() - 1; i ++)
		{
			Text = GetOraLoader().GetDBString(i);
			if(i == 2)
				Rec.SetID(Text);
			Rec.Add(Text);
			switch(i)
			{
				case 4: // NomAmount
					NomAmount = atof(QData.RemoveComma(Text));
					break;
				case 6:
					Price = atof(QData.RemoveComma(Text));
					break;
				case 7:
					LevRateBasis = Text;
					break;
				case 8:
					ValueDate = Text;
					break;
				case 10:
					AssFrom = Text;
					break;
				case 11:
					AssTo = Text;
					break;
				case 12:
					RateBasis = Text;
					break;
				case 13:
					AssetType = Text;
					break;
				case 14:
					TransType = Text;
					break;
				case 15:
					Rate = atof(QData.RemoveComma(Text));
					break;
				case 16:
					Amort = atof(QData.RemoveComma(Text));
					break;
				case 17:
					CapRate = atof(QData.RemoveComma(Text));
					break;
				case 18:
					Plus = atof(QData.RemoveComma(Text));
					break;
				case 19:
					AADays = atoi(QData.RemoveComma(Text));
					break;
				case 20:
					Formula = Text;
					break;
				default:
					break;
			}
		}

		m_Val.Setup(GetOraLoader(), TransType, NomAmount, Price, 1, AssetType, Amort, Plus, P);
		if(Rate != 0)
			m_Val.SetupIntInfo(AssFrom, ValueDate, ValueDate, AssTo, RateBasis, Rate, AADays, Y, Y);

		if(CapRate != 0)
			m_Val.SetupCapInfo(AssFrom, ValueDate, ValueDate, RateBasis, CapRate);
		m_Val.SetupLevInfo(ValueDate, ValueDate, LevRateBasis, 0, Formula);
		Rec.Add(QData.WriteNumber(m_Val.GetLevAmount(), TRUE, 2));
		AddSheetRow(Rec);// The 3rd field is id, first field is key.

		GetOraLoader().MoveNext();
	}
	
	if(GetSS()->GetSheetRows() >= 1)
		GetSS()->SetMaxRows(GetSS()->GetSheetRows());
	else
		GetSS()->SetMaxRows(500);

	GetSS()->LockSheet();
//	GetSS()->SetReDraw(TRUE);
//	GetSS()->SheetAutoFit();

	return GetSS()->GetSheetRows();
}

void CFloatRepoData::LoadSheet()
{
	if(!IsReady() || strlen(GetKey()) == 0)
		return;

	CString Text;
	double Rate, Interest, Amount;
	double Days = 0;
	CQData QData;

	GetRec().SetAmountVal(GetRec().GetAmount());
	UpdateData(&GetRec(), &GetInvRec());

	GetOraLoader().GetSql().Format("SELECT ROWIDTOCHAR(ROWID) \"ID\", INT_FROM, INT_TO, RATE, "
									"INT_TO - INT_FROM \"DAYS\", TO_NUMBER(NULL) \"INTEREST\" "
									"FROM SEMAM.NW_REPO_RATES WHERE TRANS_NUM = %s ORDER BY 2, 3", 
									(LPCTSTR) QData.GetQueryNumber(GetID()));
	if(!GetOraLoader().Open())
		return;

	GetSRowCtrl().ClearSheet();
	GetOraLoader().CopyFieldArray(GetSRowCtrl().GetSS()->GetFieldArray());
	GetSRowCtrl().GetSS()->GetFieldArray().RemoveAt(0); // Remove ID Field

	GetSRowCtrl().GetSS()->SetSheetColHeaders();
//	GetSRowCtrl().GetSS()->SetReDraw(FALSE);	
	GetOraLoader().MoveFirst();

	m_Val.SetupLevInfo(GetRec().GetValueDate(), GetRec().GetValueDate(), GetRec().GetRateBasis(),
						0, GetRec().GetFormula());

	Interest = 0;
	Amount = GetRec().GetAmountVal();
	while(!GetOraLoader().IsEOF())
	{				
		GetInvRec().SetFromDate(GetOraLoader().GetDBString(1));
		GetInvRec().SetToDate(GetOraLoader().GetDBString(2));
		GetInvRec().SetRate(GetOraLoader().GetDBString(3));
		Rate = atof(QData.RemoveComma(GetInvRec().GetRate()));

		if(GetRec().GetFormula() == DAILYREPOA)
		{
			Amount += Interest;
			m_Val.SetLevAmount(Amount);
		}
		else
			m_Val.SetLevAmount(GetRec().GetAmountVal());
		Interest = m_Val.GetDailyLevInterest(GetInvRec().GetFromDate(), GetInvRec().GetToDate(), Rate);
		Days = m_Val.GetLevDays() - Days;
		GetInvRec().SetDays(QData.WriteNumber(Days));
		
		Days = m_Val.GetLevDays();
		GetInvRec().SetInterest(QData.WriteNumber(Interest, TRUE, 2));
		Text = GetInvRec().GetFromDate() + TAB + GetInvRec().GetToDate() + TAB + 
				GetInvRec().GetRate() + TAB + GetInvRec().GetDays() + TAB + 
				GetInvRec().GetInterest();
		GetSRowCtrl().AddSheetRow(GetOraLoader().GetDBString(0), Text);// The first field is id
		GetOraLoader().MoveNext();
	}

	m_TotalInterest = m_Val.GetLevInterest();
	if(GetSRowCtrl().GetSS()->GetSheetRows() >= 1)
		GetSRowCtrl().GetSS()->SetMaxRows(GetSRowCtrl().GetSS()->GetSheetRows());
	else
		GetSRowCtrl().GetSS()->SetMaxRows(500);

	GetSRowCtrl().GetSS()->LockSheet();

//	GetSRowCtrl().GetSS()->SetReDraw(TRUE);
//	GetSRowCtrl().GetSS()->SheetAutoFit();
}

double CFloatRepoData::ComputeInterest(LPCTSTR IntFrom, LPCTSTR IntTo, LPCTSTR Rate)
{
	CQData QData;

	return ComputeInterest(IntFrom, IntTo, atof(QData.RemoveComma(Rate)));
}

double CFloatRepoData::ComputeInterest(LPCTSTR IntFrom, LPCTSTR IntTo, double Rate)
{
	int Days;
	int YearBase;

	COraDate ODate(IntFrom, m_Val.GetLevRateBasis(), 0);

	YearBase = ODate.GetYearBase();
	if(strlen(IntFrom) <= 0)
		return 0;

	Days = ODate.GetDays(IntTo);
	if(Days <= 0)
		Days = 0;	
	
	m_Interest = m_Val.GetOutput(GetRec().GetAmountVal()*Days*Rate/100/YearBase);
	return m_Interest;
}

BOOL CFloatRepoData::AddSRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData(&GetRec(), &GetInvRec());

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

BOOL CFloatRepoData::DeleteSRow(int CurrentRow)
{
	if(!IsReady() && GetSRowCtrl().GetSS()->GetSheetCurRow() < 1)
		return FALSE;

	UpdateData(&GetRec(), &GetInvRec());

	GetOraLoader().BeginTransaction();
	if(GetInvRec().DeleteRec(GetOraLoader()))
	{
		GetOraLoader().Commit();
		LoadSheet();
		GetSRowCtrl().Refresh();
		return TRUE;
	}
	
	GetOraLoader().Rollback();
	return FALSE;
}

BOOL CFloatRepoData::UpdateSRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData(&GetRec(), &GetInvRec());

	GetOraLoader().BeginTransaction();
	if(GetInvRec().UpdateRec(GetOraLoader()))
	{
		GetOraLoader().Commit();
		GetSRowCtrl().UpdateSheetRow(FALSE);
		
		double Interest;
		CQData QData;
		
		Interest = atof(QData.RemoveComma(GetInvRec().GetInterest()));
		m_TotalInterest += m_Interest - Interest;
		return TRUE;
	}
	
	GetOraLoader().Rollback();
	return FALSE;
}

double CFloatRepoData::ComputeTotalInterest()
{
	CQData QData;

	m_TotalInterest = 0;
	for(int i = 1; i <= GetSRowCtrl().GetSS()->GetSheetRows(); i ++)
		m_TotalInterest += atof(QData.RemoveComma(GetSRowCtrl().GetSS()->GetSheetText(5, i)));

	return m_TotalInterest;
}

void CFloatRepoData::UpdateData(CFloatRepoRec *pRec, CFloatRepoInvRec *pInvRec)
{
	CMSRowCtrl::UpdateData(pRec, pInvRec);
	pInvRec->SetTransNum(GetID());
	pInvRec->SetPortfolio(GetKey());
}