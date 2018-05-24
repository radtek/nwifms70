 #include "stdafx.h"
#include "NWIFMS70.h"
#include <math.h>
#include "qdata.h"
#include "sqlstmt.h"
#include "assetdata.h"


IMPLEMENT_DYNAMIC(CAssetData, CMSRowCtrl)
int CAssetData::AssetExist(LPCTSTR Asset, LPCTSTR AssetDesc)
{
	CQData QData;

	GetOraLoader().GetSql().Format("SELECT COUNT(*) FROM SEMAM.NW_ASSETS "
								"WHERE ASS_CODE = %s ", QData.GetQueryText(Asset));
	if(AssetDesc && strlen(AssetDesc) > 0)
	{
		GetOraLoader().GetSql() += " OR ASS_DESC = ";
		GetOraLoader().GetSql() +=  QData.GetQueryText(AssetDesc);
	}

	return GetOraLoader().IsExist();
}

BOOL CAssetData::GetSelIntRec(int Index, CAssetIntRec &IntRec)
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
				IntRec.SetFrom(Text);
				break;
			case 2:
				IntRec.SetTo(Text);
				break;
			case 3:
				IntRec.SetSetDate(Text);
				break;
			case 4:
				IntRec.SetEndDate(Text);
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
				IntRec.SetAction(Text);
				break;
//			case 10:
//				IntRec.SetInvType(Text);
//				break;
			default:
				break;
		}
	}
	
	return TRUE;
}

void CAssetData::SetHeaders()
{
	if(!IsReady())
		return;

	if(GetOraLoader().Open("SELECT ASS_CODE, ASSET_NUM, ASS_DESC, ASS_COMMON_CODE, ASS_MSTC_CODE, ASS_ISIN_CODE, "
							"ASS_SEDOL_NUM, RED_CODE, IMAGINE_CODE, ASS_PID, ASS_ISSUE_DATE, ASS_COUPON_START_DATE, "
							"ASS_BEGIN_ROLL_DATE, ASS_MATURITY_DATE, ASS_FX_DATE, ASS_FLOAT_INDEX, ASS_FLOAT_FORMULA, "
							"SOURCE, ASS_IND_AREA, ASS_COUNTRY, ASS_PROFILE_COUNTRY, ASS_CURRENCY, ASS_CURRENCY2, "
							"ASS_INDUSTRY, ASS_PCLASS2, ASS_PROFILE_CLASS, ASS_RATE_BASIS, ASS_INT_TYPE, "
							"ASS_PAYMENT_FREQUENCY, ASS_CMPD_FREQ, ASS_CATEGORY, STATUS, BUCKET, BUCKET2, BUCKET3, "
							"ASS_ISSUER, ASS_METHOD, ASS_SWAP_FACTOR, ASS_INT_RATE, ASS_TYPE, ASS_ACCRUABLE, "
							"ASS_PPAID_INT, UNLISTED_SECURITY, ASS_OTC_CLEAR, SUBSTR(ASS_CONV_TYPE, 1, 1) \"ASS_CONV_TYPE\", "
							"ASS_1256, ASS_PAR_VALUE, ASS_DF_FEE_TYPE, UNDERLINE_ASSET, BELOW_RATING, ASS_144A, "
							"ID_144A, ASS_ADMIN_ID, ASS_MARGIN, ASS_FRA, ASS_LIQUIDITY, ASS_PF_CLASS, ASS_PF_SUBCLASS, "
							"ASS_BDATE, ASS_CDS_RESTRUCT, ASS_BDC, ASS_CITI_CLASS, BLM_KEY, ASS_YC_NAME, ASS_IRS_TMPLATE, "
							"ASS_EXCHANGE, ASS_ACCOUNT, ASS_FUTURE_TYPE, UNDERLINE_ID, NOM_AMOUNT, INV_TYPE "
							"FROM SEMAM.NW_ASSETS_V A, SEMAM.NW_POSITION_V B "
							"WHERE B.ASSET_CODE(+) = A.ASS_CODE AND 1 = 2 "))
	{
		GetOraLoader().CopyFieldArray(GetSS()->GetFieldArray());
		GetSS()->SetSheetColHeaders();
	}

	if(GetOraLoader().Open("SELECT ASS_FROM, ASS_TO, INT_SET_DATE, START_DATE, END_DATE, RATE, AMORT_FACT, CAPIT_RATE, PLUS_AMOUNT, ACTION "
							"FROM SEMAM.NW_ASS_PERIODS "))
	{
		GetOraLoader().CopyFieldArray(GetSRowCtrl().GetSS()->GetFieldArray());
		GetSRowCtrl().GetSS()->SetSheetColHeaders();
	}
}

int CAssetData::LoadDBData(LPCTSTR TicketNum)
{
	CDBRec Rec;
	CStringArray QRec;
	CString Sql;
	CQData QData;
	CString Num;
	BOOL bOK;
	int i;

	if(!IsReady())
		return FALSE;

	if(TicketNum && strlen(TicketNum) > 0)
	{
		Num = QData.GetQueryNumber(TicketNum);
		Sql = "SELECT ASSET_CODE, TO_NUMBER(NULL) \"ASSET_NUM\", ASSET_DESC, ASSET_COMMON_CODE, ASSET_MSTC_CODE, "
				"ASSET_ISIN_CODE, ASSET_SEDOL_NUM, NULL \"RED_CODE\", ASSET_IMAGINE_CODE, ASSET_PID, ASS_ISSUE_DATE, "
				"ASS_COUPON_START_DATE, ASS_BEGIN_ROLL_DATE, ASSET_MATURITY, FX_DATE, ASS_FLOAT_INDEX, ASS_FLOAT_FORMULA, "
				"A.SOURCE, ASSET_IND_AREA, ASSET_COUNTRY, ASS_PROFILE_COUNTRY, ASSET_CURRENCY, ASS_CURRENCY2, ASSET_CLASS, "
				"NULL \"ASS_PCLASS2\", ASS_PROFILE_CLASS, ASS_RATE_BASIS, ASS_INT_TYPE, ASS_FREQ, ASS_CMPD_FREQ, "
				"ASSET_CATEGORY, STATUS, ASSET_BUCKET, NULL \"BUCKET2\", NULL \"BUCKET3\", ASS_ISSUER, "
				"DECODE(ASS_CONV_TYPE, 'F', 'FIFO') \"ASS_METHOD\", ASS_SWAP_FACTOR, ASS_INT_RATE, NULL \"ASS_TYPE\", "
				"ASS_ACCRUABLE, ASS_PPAID_INT, UNLISTED_SECURITY, ASS_OTC_CLEAR, ASS_CONV_TYPE, ASS_1256, ASS_PAR_VALUE, "
				"ASS_DF_FEE_TYPE, UNDERLINE_ASSET, BELOW_RATING, ASS_144A, ID_144A, ASS_ADMIN_ID, ASS_MARGIN, ASS_FRA, "
				"ASS_LIQUIDITY, ASS_PF_CLASS, ASS_PF_SUBCLASS, ASS_BDATE, ASS_CDS_RESTRUCT, ASS_BDC, ASS_CITI_CLASS, "
				"BLM_KEY, ASS_YC_NAME, ASS_EXCHANGE, ASS_IRS_TMPLATE, NULL \"ASS_ACCOUNT\", NULL \"ASS_FUTURE_TYPE\", "
				"NULL \"ASSET_ID2\", TO_NUMBER(NULL) \"POSITION\", "
				"DECODE(ASSET_CLASS, 'ADR EQUITIES', 'D', 'DOM EQUITIES', 'D', 'DOM EQTY-NEW', 'D', 'EXCH TRADE FUND', 'D', 'R') \"INV_TYPE\" "
				"FROM SEMAM.NW_RAW_TICKETS A, SEMAM.NW_IMG_ASSETS D "
				"WHERE D.IMAGINE_CODE(+) = A.ASSET_IMAGINE_CODE "
				"AND TICKET_NUM = " + Num +
				"AND ASSET_CODE = 'NEW ASSET' "
				"UNION "
				"SELECT ASS_CODE, ASSET_NUM, ASS_DESC, ASS_COMMON_CODE, ASS_MSTC_CODE, ASS_ISIN_CODE, ASS_SEDOL_NUM, "
				"RED_CODE, IMAGINE_CODE, ASS_PID, ASS_ISSUE_DATE, ASS_COUPON_START_DATE, ASS_BEGIN_ROLL_DATE, "
				"ASS_MATURITY_DATE, ASS_FX_DATE, ASS_FLOAT_INDEX, ASS_FLOAT_FORMULA, A.SOURCE, ASS_IND_AREA, ASS_COUNTRY, "
				"ASS_PROFILE_COUNTRY, ASS_CURRENCY, ASS_CURRENCY2, ASS_INDUSTRY, ASS_PCLASS2, ASS_PROFILE_CLASS, "
				"ASS_RATE_BASIS, ASS_INT_TYPE, ASS_PAYMENT_FREQUENCY, ASS_CMPD_FREQ, ASS_CATEGORY, STATUS, BUCKET, BUCKET2, "
				"BUCKET3, ASS_ISSUER, ASS_METHOD, ASS_SWAP_FACTOR, ASS_INT_RATE, ASS_TYPE, ASS_ACCRUABLE, ASS_PPAID_INT, "
				"UNLISTED_SECURITY, ASS_OTC_CLEAR, SUBSTR(ASS_CONV_TYPE, 1, 1) \"ASS_CONV_TYPE\", ASS_1256, ASS_PAR_VALUE, "
				"ASS_DF_FEE_TYPE, UNDERLINE_ASSET, BELOW_RATING, ASS_144A, ID_144A, ASS_ADMIN_ID, ASS_MARGIN, ASS_FRA, "
				"ASS_LIQUIDITY, ASS_PF_CLASS, ASS_PF_SUBCLASS, ASS_BDATE, ASS_CDS_RESTRUCT, ASS_BDC, ASS_CITI_CLASS, "
				"BLM_KEY, ASS_YC_NAME, ASS_IRS_TMPLATE, ASS_EXCHANGE, ASS_ACCOUNT, ASS_FUTURE_TYPE, UNDERLINE_ID, "
				"SUM(C.NOM_AMOUNT) \"POSITION\", INV_TYPE "
				"FROM SEMAM.NW_ASSETS_V A, SEMAM.NW_RAW_TICKETS B, SEMAM.NW_POSITION_V C "
				"WHERE (B.ASSET_CODE = A.ASS_CODE OR B.ASSET_IMAGINE_CODE = A.IMAGINE_CODE) "
				"AND C.ASSET_CODE(+) = B.ASSET_CODE "
				"AND B.TICKET_NUM = " + Num + 
				"AND B.ASSET_CODE !='NEW ASSET' ";
	}
	else
	{
		Rec.RemoveAll();

		ToDBRec(Rec, TRUE);
		if(Rec.GetSize() <= 0)
			return -1;

		bOK = FALSE;
		for(i = 0; i < Rec.GetSize(); i ++)
		{
			if(strlen(Rec.GetAt(i)) > 2)
				bOK = TRUE;
		}

		if(!bOK)
			return -1;

		GetOraLoader().RecToQueryRec(QRec, Rec, &GetSS()->GetFieldArray(), "%");
		Sql = "SELECT ASS_CODE, ASSET_NUM, ASS_DESC, ASS_COMMON_CODE, ASS_MSTC_CODE, ASS_ISIN_CODE, ASS_SEDOL_NUM, "
				"RED_CODE, IMAGINE_CODE, ASS_PID, ASS_ISSUE_DATE, ASS_COUPON_START_DATE, ASS_BEGIN_ROLL_DATE, "
				"ASS_MATURITY_DATE, ASS_FX_DATE, ASS_FLOAT_INDEX, ASS_FLOAT_FORMULA, A.SOURCE, ASS_IND_AREA, "
				"ASS_COUNTRY, ASS_PROFILE_COUNTRY, ASS_CURRENCY, ASS_CURRENCY2, ASS_INDUSTRY, ASS_PCLASS2, "
				"ASS_PROFILE_CLASS, ASS_RATE_BASIS, ASS_INT_TYPE, ASS_PAYMENT_FREQUENCY, ASS_CMPD_FREQ, ASS_CATEGORY, "
				"STATUS, BUCKET, BUCKET2, BUCKET3, ASS_ISSUER, ASS_METHOD, ASS_SWAP_FACTOR, ASS_INT_RATE, ASS_TYPE, "
				"ASS_ACCRUABLE, ASS_PPAID_INT, UNLISTED_SECURITY, ASS_OTC_CLEAR, "
				"SUBSTR(ASS_CONV_TYPE, 1, 1) \"ASS_CONV_TYPE\", ASS_1256, ASS_PAR_VALUE, ASS_DF_FEE_TYPE, "
				"UNDERLINE_ASSET, BELOW_RATING, ASS_144A, ID_144A, ASS_ADMIN_ID, ASS_MARGIN, ASS_FRA, ASS_LIQUIDITY, "
				"ASS_PF_CLASS, ASS_PF_SUBCLASS, ASS_BDATE, ASS_CDS_RESTRUCT, ASS_BDC, ASS_CITI_CLASS, BLM_KEY, "
				"ASS_YC_NAME, ASS_IRS_TMPLATE, ASS_EXCHANGE, ASS_ACCOUNT, ASS_FUTURE_TYPE, UNDERLINE_ID, "
				"SUM(B.NOM_AMOUNT) \"POSITION\", INV_TYPE "
				"FROM SEMAM.NW_ASSETS_V A "
				"LEFT OUTER JOIN SEMAM.NW_POSITION_V B ON (B.ASSET_CODE = A.ASS_CODE AND B.TRANS_TYPE IN ('CDS', 'SECURITIES')) "
				"WHERE ";
		if(!GetOraLoader().SetSqlSearchInfo(Sql, QRec, &GetSS()->GetFieldArray(), FALSE))
			Sql += "1 = 2 ";
	}
	
	Sql += "GROUP BY ASS_CODE, ASSET_NUM, ASS_DESC, ASS_COMMON_CODE, ASS_MSTC_CODE, ASS_ISIN_CODE, ASS_SEDOL_NUM, "
			"RED_CODE, IMAGINE_CODE, ASS_PID, ASS_ISSUE_DATE, ASS_COUPON_START_DATE, ASS_BEGIN_ROLL_DATE, "
			"ASS_MATURITY_DATE, ASS_FX_DATE, ASS_FLOAT_INDEX, ASS_FLOAT_FORMULA, A.SOURCE, ASS_IND_AREA, ASS_COUNTRY, "
			"ASS_PROFILE_COUNTRY, ASS_CURRENCY, ASS_CURRENCY2, ASS_INDUSTRY, ASS_PCLASS2, ASS_PROFILE_CLASS, "
			"ASS_RATE_BASIS, ASS_INT_TYPE, ASS_PAYMENT_FREQUENCY, ASS_CMPD_FREQ, ASS_CATEGORY, STATUS, BUCKET, BUCKET2, "
			"BUCKET3, ASS_ISSUER, ASS_METHOD, ASS_SWAP_FACTOR, ASS_INT_RATE, ASS_TYPE, ASS_ACCRUABLE, ASS_PPAID_INT, "
			"UNLISTED_SECURITY, ASS_OTC_CLEAR, SUBSTR(ASS_CONV_TYPE, 1, 1), ASS_1256, ASS_PAR_VALUE, ASS_DF_FEE_TYPE, "
			"UNDERLINE_ASSET, BELOW_RATING, ASS_144A, ID_144A, ASS_ADMIN_ID, ASS_MARGIN, ASS_FRA, ASS_LIQUIDITY, "
			"ASS_PF_CLASS, ASS_PF_SUBCLASS, ASS_BDATE, ASS_CDS_RESTRUCT, ASS_BDC, ASS_CITI_CLASS, BLM_KEY, ASS_YC_NAME, "
			"ASS_IRS_TMPLATE, ASS_EXCHANGE, ASS_ACCOUNT, ASS_FUTURE_TYPE, UNDERLINE_ID, INV_TYPE ";
	Sql += " ORDER BY 1 ";	
	
	if(GetOraLoader().Open(Sql))
		return GetOraLoader().LoadRowCtrl(*this, TRUE, TRUE);
	return FALSE;
}

BOOL CAssetData::AddRow()
{
	if(!IsReady() || strlen(GetKey()) > 0)
		return FALSE;

	UpdateData(&GetAssetRec(), &GetIntRec());
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

BOOL CAssetData::DeleteRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData(&GetAssetRec(), &GetIntRec());
	if(strlen(GetKey()) == 0)
		return FALSE;
	
	CStringArray Data;
	BOOL Exist = FALSE;
	CString Asset;
	CQData QData;

	Asset = QData.GetQueryText(GetKey()); // Check whether the asset code has been used
	GetOraLoader().GetSql().Format("SELECT COUNT(*) FROM SEMAM.NW_TR_TICKETS "
									"WHERE ASSET_CODE = %s ", (LPCTSTR) Asset);
	if(!GetOraLoader().Open())
		return FALSE;

	GetOraLoader().LoadStringArray(Data);
	for(int i = 0; i < Data.GetSize(); i++)
	{
		if(atoi(Data.GetAt(i)) > 0)
			Exist = TRUE;						
	}

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

BOOL CAssetData::UpdateRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData(&GetAssetRec(), &GetIntRec());

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

BOOL CAssetData::AddSRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData(&GetAssetRec(), &GetIntRec());
	GetIntRec().SetInvType(GetInvType());
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

BOOL CAssetData::DeleteSRow(int CurrentRow)
{
	if(!IsReady())
		return FALSE;

	UpdateData(&GetAssetRec(), &GetIntRec());
	GetIntRec().SetInvType(GetInvType());
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

BOOL CAssetData::UpdateSRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData(&GetAssetRec(), &GetIntRec());
	GetIntRec().SetInvType(GetInvType());
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

void CAssetData::LoadSheet()
{
	if(!IsReady())
		return;

	LPCTSTR P;
	CQData QData;

	SetKey();
	P = QData.GetQueryText(GetKey());

	GetOraLoader().GetSql().Format("SELECT ROWIDTOCHAR(ROWID) \"ID\", ASS_FROM, ASS_TO, INT_SET_DATE, START_DATE, END_DATE, "
									"RATE, AMORT_FACT, CAPIT_RATE, PLUS_AMOUNT, ACTION "
									"FROM SEMAM.NW_ASS_PERIODS "
									"WHERE ASS_CODE = %s "
									"UNION "
									"SELECT ROWIDTOCHAR(ROWID) \"ID\", EXD_DATE, TO_DATE(NULL), TO_DATE(NULL), TO_DATE(NULL), "
									"PAY_DATE, DIVIDEND, TO_NUMBER(NULL), TO_NUMBER(NULL), TO_NUMBER(NULL), CURRENCY "
									"FROM SEMAM.NW_DIVIDEND_SCHEDULE "
									"WHERE ASSET_CODE = %s "
									"ORDER BY 2 DESC ", P, P);

	if(GetOraLoader().Open())
		GetOraLoader().LoadRowCtrl(GetSRowCtrl(), TRUE);
}

int CAssetData::YearAdj(int yy)
{
	if(yy < 100)
	{
		if(yy < 80)
			yy += 2000;
		else
			yy += 1900;
	}

	return yy;
}

BOOL CAssetData::GenerateFixedRates(CString &Asset, CString &AssetClass, CString &RateType, 
							CString &Freq, CString &Maturity, CString &IntToDate, CString &Rate)
{
	if(!IsReady())
		return FALSE;

	CString Text, FromDate, ToDate;
	int iFreq, mm, dd, yy, mm2, dd2, yy2, iMaturity, iToDate;
	BOOL bLastday = FALSE;
	CAssetIntRec RateRec;

	if(Asset.IsEmpty() || (RateType != "FIXED" && AssetClass.Find("CDS SWAPS") < 0) || 
		Freq.IsEmpty() || Maturity.IsEmpty() || Rate.IsEmpty())
		return FALSE;

	RateRec.GetInvType() = "R";
	GetOraLoader().SetSql("ALTER SESSION SET NLS_DATE_FORMAT = 'MM/DD/YYYY'");
	GetOraLoader().ExecuteSql();
	iFreq = atoi(Freq);

	mm = atoi(IntToDate);
	dd = atoi(((LPCTSTR) IntToDate) + 3);
	yy = atoi(((LPCTSTR) IntToDate) + 6);
	yy = YearAdj(yy);
	
	mm2 = atoi(Maturity);
	dd2 = atoi(((LPCTSTR) Maturity) + 3);
	if(dd2 == 31 || dd2 == 29)
		bLastday = TRUE;
	yy2 = atoi(((LPCTSTR) Maturity) + 6);
	yy2 = YearAdj(yy2);

	iMaturity = yy2*10000 + mm2*100 + dd2;

	FromDate = GetIntRec().GetTo();
	iToDate = yy*10000 + mm*100 + dd;

	RateRec.SetAsset(Asset);
	RateRec.SetAmort("");
	RateRec.SetCap("");
	RateRec.SetPlus("");
	RateRec.SetAction("");

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
			yy = YearAdj(yy);
		}


		RateRec.SetFrom(FromDate);
		RateRec.SetTo(ToDate);
		RateRec.SetSetDate(ToDate);
		RateRec.SetRate(Rate);

		RateRec.AddRec(GetOraLoader());

		FromDate = ToDate;
		iToDate = yy*10000 + mm*100+ dd;
	}

	GetOraLoader().SetSql("ALTER SESSION SET NLS_DATE_FORMAT = 'MM/DD/RR'");
	GetOraLoader().ExecuteSql();
	LoadSheet();
	return TRUE;

}

void CAssetData::LoadHoldings(LPCTSTR Asset, CDBSheet &HoldingSS)
{
	CQData QData;

	GetOraLoader().GetSql().Format("SELECT CUSTODIAN, PORTFOLIO, TRANS_TYPE, TRANS_NUM, RATE, MATURITY, "
									"SUM(NOM_AMOUNT) \"NOM_AMOUNT\" "
									"FROM SEMAM.NW_POSITION_V WHERE ASSET_CODE = %s "
									"GROUP BY CUSTODIAN, PORTFOLIO, TRANS_TYPE, TRANS_NUM, RATE, MATURITY "
									"HAVING SUM(NOM_AMOUNT) != 0 ORDER BY 1, 2, 3 ", 
									QData.GetQueryText(Asset));
	if(GetOraLoader().Open())
		GetOraLoader().LoadDBSheet(HoldingSS, TRUE);
}

BOOL CAssetData::ImportImagineAsset()
{
	this->ClearSheet();
	if(GetOraLoader().Open("SELECT NULL \"ASS_CODE\", TO_NUMBER(NULL) \"ASSET_NUM\", ASS_DESC, NULL \"ASS_COMMON_CODE\", "
						"ASS_MSTC_CODE, ASS_ISIN_CODE, ASS_SEDOL_NUM, RED_CODE, IMAGINE_CODE, ASS_PID, ASS_ISSUE_DATE, "
						"ASS_COUPON_START_DATE, ASS_BEGIN_ROLL_DATE, ASS_MATURITY_DATE, ASS_FX_DATE, ASS_FLOAT_INDEX, "
						"ASS_FLOAT_FORMULA, SOURCE, ASS_IND_AREA, ASS_COUNTRY, ASS_PROFILE_COUNTRY, ASS_CURRENCY, "
						"ASS_CURRENCY2, ASS_INDUSTRY, NULL \"ASS_PCLASS2\", ASS_PROFILE_CLASS, ASS_RATE_BASIS, ASS_INT_TYPE, "
						"ASS_FREQ, ASS_CMPD_FREQ, ASS_CATEGORY, STATUS, BUCKET, NULL \"BUCKET2\", NULL \"BUCKET3\", "
						"ASS_ISSUER, NULL \"ASS_METHOD\", ASS_SWAP_FACTOR, ASS_INT_RATE, NULL \"ASS_TYPE\", ASS_ACCRUABLE, "
						"ASS_PPAID_INT, UNLISTED_SECURITY, ASS_OTC_CLEAR, ASS_CONV_TYPE, ASS_1256, ASS_PAR_VALUE, ASS_DF_FEE_TYPE, "
						"UNDERLINE_ASSET, BELOW_RATING, ASS_144A, ID_144A, ASS_ADMIN_ID, ASS_MARGIN, ASS_FRA, ASS_LIQUIDITY, "
						"ASS_PF_CLASS, ASS_PF_SUBCLASS, ASS_BDATE, ASS_CDS_RESTRUCT, ASS_BDC, ASS_CITI_CLASS, BLM_KEY, "
						"ASS_YC_NAME, ASS_IRS_TMPLATE, NULL \"UNDERLINE_ID\", TO_NUMBER(NULL) \"NOMINAL\" "
						"FROM SEMAM.NW_IMG_ASSETS "
						"ORDER BY ASS_DESC "))
		return GetOraLoader().LoadRowCtrl(*this, TRUE, TRUE);
	return FALSE;
}