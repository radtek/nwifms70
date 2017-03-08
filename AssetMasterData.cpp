#include "stdafx.h"
#include "NWIFMS70.h"
#include <math.h>
#include "qdata.h"
#include "sqlstmt.h"
#include "assetmasterdata.h"


IMPLEMENT_DYNAMIC(CAssetMasterData, CMSRowCtrl)
int CAssetMasterData::AssetExist(LPCTSTR Asset, LPCTSTR AssetDesc)
{
	CQData QData;

	GetOraLoader().GetSql().Format("SELECT COUNT(*) FROM SEMAM.NW_ASSETS WHERE ASS_CODE = %s ", 
									QData.GetQueryText(Asset));
	if(AssetDesc && strlen(AssetDesc) > 0)
	{
		GetOraLoader().GetSql() += " OR ASS_DESC = ";
		GetOraLoader().GetSql() +=  QData.GetQueryText(AssetDesc);
	}

	return GetOraLoader().IsExist();
}

BOOL CAssetMasterData::GetSelIntRec(int Index, CAssetMasterIntRec &IntRec)
{
	if(Index < 1 || Index > GetSRowCtrl().GetSS()->GetSheetRows())
		return FALSE;
	
	CString Text;
	for(int i = 1; i <= GetSRowCtrl().GetSS()->GetSheetCols(); i++)
	{
		Text = GetSRowCtrl().GetSS()->GetSheetText(i, Index);
		switch(i)
		{
			case 1:
				IntRec.SetAssetFrom(Text);
				break;
			case 2:
				IntRec.SetAssetTo(Text);
				break;
			case 3:
				IntRec.SetFrom(Text);
				break;
			case 4:
				IntRec.SetSetDate(Text);
				break;
			case 5:
				IntRec.SetRate(Text);
				break;
			case 6:
				IntRec.SetAmort(Text);
				break;
			case 7:
				IntRec.SetCap(Text);
				break;
			case 8:
				IntRec.SetPlus(Text);
				break;
			case 9:
				IntRec.SetInclusive(Text);
				break;
			case 10:
				IntRec.SetTo(Text);
				break;
			default:
				break;
		}
	}
	
	return TRUE;
}

void CAssetMasterData::SetHeaders()
{
	if(!IsReady())
		return;

	if(GetOraLoader().Open("SELECT ASS_CODE, ASSET, PAR_VALUE, ASSET2, COMMON, CUSIP, SEDOL, "
				"ISIN, PID, ISSUE_DATE, CDS_FEE_TYPE, MATURITY, ROLL_DATE, FLOAT_INDEX, "
				"FLOAT_FORMULA, SOURCE, COUNTRY, CURRENCY, ISSUER, INDUSTRY, INT_TYPE, RATE_BASIS, "
				"FREQ, ADJUST_FACTOR, FACTOR, ACCRUABLE, PPAID_INT, FUTURE, T1256, BELOW_RATING, "
				"ID_144A, METHOD, UNLISTED, STATUS FROM SEMAM.NW_ASSET_MASTER A WHERE 1 = 2 "))
	{
		GetOraLoader().CopyFieldArray(GetSS()->GetFieldArray());
		GetSS()->SetSheetColHeaders();
	}

	if(GetOraLoader().Open("SELECT FROMDATE, TODATE, INT_SET_DATE, RATE, AMORT_FACT, CAPIT_RATE, PLUS_AMOUNT, "
							"INCLUSIVE, INT_FROM, INT_TO, ASS_CODE FROM SEMAM.NW_ASSET_RATE WHERE 1 = 2 "))
	{
		GetOraLoader().CopyFieldArray(GetSRowCtrl().GetSS()->GetFieldArray());
		GetSRowCtrl().GetSS()->SetSheetColHeaders();
	}

	if(GetOraLoader().Open("SELECT ASSET_CODE, ASSET_DESC, CATEGORY, BUCKET, CLASS FROM SEMAM.NW_ASSET_CLASS WHERE 1 = 2 "))
	{
		GetOraLoader().CopyFieldArray(GetAssetClass().GetSS()->GetFieldArray());
		GetAssetClass().GetSS()->SetSheetColHeaders();
	}
}

int CAssetMasterData::LoadDBData(LPCTSTR TicketNum)
{
	CDBRec Rec;
	CStringArray QRec;
	CString Sql;
	CQData QData;
	LPCTSTR Num;
	BOOL bOK;

	if(!IsReady())
		return FALSE;

	GetAssetClass().ClearSheet();
	GetSRowCtrl().GetSS()->ClearSheet();
	if(TicketNum && strlen(TicketNum) > 0)
	{
		Num = QData.GetQueryNumber(TicketNum);
		GetOraLoader().GetSql() = "SELECT ASS_CODE, ASSET, PAR_VALUE, ASSET2, COMMON, CUSIP, SEDOL, ISIN, "
				"PID, ISSUE_DATE, CDS_FEE_TYPE, MATURITY, ROLL_DATE, FLOAT_INDEX, FLOAT_FORMULA, SOURCE, "
				"COUNTRY, CURRENCY, ISSUER, INDUSTRY, INT_TYPE, RATE_BASIS, FREQ, ADJUST_FACTOR, FACTOR, "
				"ACCRUABLE, PPAID_INT, FUTURE, T1256, BELOW_RATING, ID_144A, METHOD, UNLISTED, STATUS "
				"FROM SEMAM.NW_ASSET_MASTER A, SEMAM.NW_ASSET_CLASS B, SEMAM.NW_RAW_TICKETS C "
				"WHERE B.ASS_CODE = A.ASS_CODE "
				"AND C.ASSET_CODE = B.ASSET_CODE "
				"AND C.TICKET_NUM = %s "
				"AND C.ASSET_CODE != 'NEW ASSET' "
				"UNION SELECT ASSET_CODE, ASSET_DESC, TO_NUMBER(NULL), NULL, ASSET_COMMON_CODE, "
				"ASSET_MSTC_CODE, ASSET_SEDOL_NUM, ASSET_ISIN_CODE, ASSET_PID, TO_DATE(NULL), NULL, "
				"ASSET_MATURITY, TO_DATE(NULL), NULL, NULL, SOURCE, ASSET_COUNTRY, ASSET_CURRENCY, NULL, "
				"ASSET_IND_AREA, NULL, NULL, TO_NUMBER(NULL), TO_NUMBER(NULL), TO_NUMBER(NULL), NULL, NULL, "
				"NULL, NULL, NULL, NULL, NULL, NULL, NULL "
				"FROM SEMAM.NW_RAW_TICKETS "
				"WHERE TICKET_NUM = %s AND ASSET_CODE = 'NEW ASSET' ";
		GetOraLoader().GetSql().Format((LPCTSTR) Sql, Num, Num);		
	}
	else
	{
		ToDBRec(Rec, TRUE);
		if(Rec.GetSize() <= 0)
			return -1;
		
		bOK = FALSE;
		for(int i = 0; i < Rec.GetSize(); i ++)
		{
			if(strlen(Rec.GetAt(i)) > 0)
				bOK = TRUE;
		}

		if(!bOK)
			return -1;
		
		GetOraLoader().RecToQueryRec(QRec, Rec, &GetSS()->GetFieldArray(), "%");

		GetOraLoader().GetSql() = "SELECT ASS_CODE, ASSET, PAR_VALUE, ASSET2, COMMON, CUSIP, SEDOL, "
				"ISIN, PID, ISSUE_DATE, CDS_FEE_TYPE, MATURITY, ROLL_DATE, FLOAT_INDEX, "
				"FLOAT_FORMULA, SOURCE, COUNTRY, CURRENCY, ISSUER, INDUSTRY, INT_TYPE, RATE_BASIS, "
				"FREQ, ADJUST_FACTOR, FACTOR, ACCRUABLE, PPAID_INT, FUTURE, T1256, BELOW_RATING, "
				"ID_144A, METHOD, UNLISTED, STATUS "
				"FROM SEMAM.NW_ASSET_MASTER A WHERE ";

		GetOraLoader().SetSqlSearchInfo(GetOraLoader().GetSql(), QRec, &GetSS()->GetFieldArray(), FALSE);
		GetOraLoader().GetSql() += " ORDER BY 1 ";
	}
	
	if(GetOraLoader().Open())
		return GetOraLoader().LoadRowCtrl(*this, TRUE, TRUE);
	return FALSE;
}

BOOL CAssetMasterData::AddRow()
{
	if(!IsReady() || strlen(GetKey()) > 0)
		return FALSE;

	UpdateData(&GetAssetRec(), &GetIntRec());
	GetClassRec().SetAsset(GetAssetRec().GetAsset());
	if(GetAssetRec().GetAsset().IsEmpty())
		return FALSE;
	
	if(AssetExist(GetAssetRec().GetAsset(), GetAssetRec().GetDesc()) > 0) // exist
	{
		MessageBox(NULL, "Either Identified Asset Code or Description already exist", NULL, MB_OK);
		return FALSE;
	}

	GetOraLoader().BeginTransaction();
	if(GetAssetRec().AddRec(GetOraLoader()))
	{
		GetOraLoader().Commit();
		AddSheetRow();
		return TRUE;
	}	

	GetOraLoader().Rollback();
	return FALSE;
}

BOOL CAssetMasterData::DeleteRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData(&GetAssetRec(), &GetIntRec());
	GetClassRec().SetAsset(GetAssetRec().GetAsset());
	if(strlen(GetKey()) == 0)
		return FALSE;
	
	CString Data;
	BOOL Exist = FALSE;
	CQData QData;

	// Check whether the asset code has been used
	GetOraLoader().GetSql().Format("SELECT COUNT(*) FROM SEMAM.NW_ASSET_CLASS WHERE ASS_CODE = %s ", 
									QData.GetQueryText(GetKey()));
	if(!GetOraLoader().Open(GetOraLoader().GetSql()))
		return FALSE;

	GetOraLoader().LoadTextString(Data);
	if(atoi(Data) > 0)
		Exist = TRUE;						

	if(Exist)
	{
		MessageBox(NULL, "Asset Code is in use", NULL, MB_OK);
		return FALSE;	
	}

	GetOraLoader().BeginTransaction();
	if(GetAssetRec().DeleteRec(GetOraLoader()))
	{
		GetOraLoader().Commit();
		DeleteSheetRow(GetCurrentRow());
		Refresh();
		return TRUE;
	}

	GetOraLoader().Rollback();
	return FALSE;
}

BOOL CAssetMasterData::UpdateRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData(&GetAssetRec(), &GetIntRec());
	GetClassRec().SetAsset(GetAssetRec().GetAsset());

	if(strlen(GetKey()) == 0)
		return FALSE;

	if(!GetModify(FALSE))
		return TRUE;

	if(AssetExist(GetAssetRec().GetAsset(), GetAssetRec().GetDesc()) <= 0) // Does not exist
	{
		MessageBox(NULL, "Asset do not exist", NULL, MB_OK);
		return FALSE;
	}

	GetOraLoader().BeginTransaction();
	if(GetAssetRec().UpdateRec(GetOraLoader()))
	{
		GetOraLoader().Commit();
		UpdateSheetRow(FALSE);
		return TRUE;
	}	

	GetOraLoader().Rollback();
	return FALSE;
}

BOOL CAssetMasterData::AddSRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData(&GetAssetRec(), &GetIntRec());
	if(strlen(GetKey()) == 0)
		return FALSE;

	GetIntRec().SetAsset(GetKey());
	GetOraLoader().BeginTransaction();
	if(GetIntRec().AddRec(GetOraLoader()))
	{
		GetOraLoader().Commit();
		LoadSheet();
		return TRUE;
	}

	GetOraLoader().Rollback();
	return FALSE;
}

BOOL CAssetMasterData::DeleteSRow(int CurrentRow)
{
	if(!IsReady())
		return FALSE;

	UpdateData(&GetAssetRec(), &GetIntRec());
	if(strlen(GetKey()) == 0 || strlen(GetIntRec().GetID()) == 0)
		return FALSE;

	GetIntRec().SetAsset(GetKey());
	GetOraLoader().BeginTransaction();
	if(GetIntRec().DeleteRec(GetOraLoader(), TRUE))
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

BOOL CAssetMasterData::UpdateSRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData(&GetAssetRec(), &GetIntRec());
	if(strlen(GetKey()) == 0 || strlen(GetIntRec().GetID()) == 0)
		return FALSE;

	if(!GetSRowCtrl().GetModify(FALSE))
		return TRUE;

	GetIntRec().SetAsset(GetKey());
	GetOraLoader().BeginTransaction();
	if(GetIntRec().UpdateRec(GetOraLoader()))
	{
		GetOraLoader().Commit();
		GetSRowCtrl().UpdateSheetRow(FALSE);
		return TRUE;
	}
	
	GetOraLoader().Rollback();
	return FALSE;
}

void CAssetMasterData::LoadSheet()
{
	if(!IsReady())
		return;

	CQData QData;

	SetKey();

	GetOraLoader().GetSql().Format("SELECT ROWIDTOCHAR(ROWID) \"ID\", FROMDATE, TODATE, INT_SET_DATE, "
									"RATE, AMORT_FACT, CAPIT_RATE, PLUS_AMOUNT, INCLUSIVE, INT_FROM, "
									"INT_TO, ASS_CODE "
									"FROM SEMAM.NW_ASSET_RATE WHERE ASS_CODE = %s "
									"ORDER BY ASS_CODE, INT_FROM, FROMDATE ", 
									QData.GetQueryText(GetKey()));
	if(GetOraLoader().Open())
		GetOraLoader().LoadRowCtrl(GetSRowCtrl(), TRUE);

	GetOraLoader().GetSql().Format("SELECT ROWIDTOCHAR(ROWID) \"ID\", ASSET_CODE, ASSET_DESC, ASS_CODE, "
									"CATEGORY, BUCKET, CLASS FROM SEMAM.NW_ASSET_CLASS WHERE ASS_CODE = %s ", 
									QData.GetQueryText(GetKey()));
	if(GetOraLoader().Open())
		GetOraLoader().LoadRowCtrl(GetAssetClass(), TRUE);
}

BOOL CAssetMasterData::GenerateFixedRates(CString &Asset, CString &AssetClass, CString &RateType, 
							CString &Freq, CString &Maturity, CString &IntToDate, CString &Rate)
{
	if(!IsReady())
		return FALSE;

	CString Text, FromDate, ToDate;
	int iFreq, mm, dd, yy, mm2, dd2, yy2, iMaturity, iToDate;
	CAssetMasterIntRec RateRec;

	if(Asset.IsEmpty() || (RateType != "FIXED" && AssetClass.Find("CDS SWAPS") < 0) || 
		Freq.IsEmpty() || Maturity.IsEmpty() || Rate.IsEmpty())
		return FALSE;

	iFreq = atoi(Freq);

	mm = atoi(IntToDate);
	dd = atoi(((LPCTSTR) IntToDate) + 3);
	yy = atoi(((LPCTSTR) IntToDate) + 6);
	
	mm2 = atoi(Maturity);
	dd2 = atoi(((LPCTSTR) Maturity) + 3);
	yy2 = atoi(((LPCTSTR) Maturity) + 6);
	iMaturity = yy2*10000 + mm2*100 + dd2;

	FromDate = GetIntRec().GetTo();
	iToDate = yy*10000 + mm*100 + dd;

	RateRec.SetAsset(Asset);
	RateRec.SetAmort("");
	RateRec.SetCap("");
	RateRec.SetPlus("");
	RateRec.SetInclusive("");

	while(iToDate < iMaturity)
	{
		mm += 12/iFreq;
		if(mm > 12)
		{
			yy += 1;
			mm -= 12;
		}

		ToDate.Format("%02d/%02d/%04d", mm, dd2, yy);
		if(ToDate != Maturity && AssetClass.Find("CDS SWAPS") >= 0)
		{
			ToDate = GetOraLoader().TPlusN(ToDate, (LPCTSTR) ToDate, 0, "USD");
			mm = atoi(ToDate);
			dd = atoi(((LPCTSTR) ToDate) + 3);
			yy = atoi(((LPCTSTR) ToDate) + 6);
		}

		RateRec.SetFrom(FromDate);
		RateRec.SetTo(ToDate);
		RateRec.SetSetDate(ToDate);
		RateRec.SetRate(Rate);

		RateRec.AddRec(GetOraLoader());

		FromDate = ToDate;
		iToDate = yy*10000 + mm*100+ dd;
	}
	
	LoadSheet();
	return TRUE;

}