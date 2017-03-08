#include "stdafx.h"
#include "NWIFMS70.h"
#include <math.h>
#include "fsrec.h"
#include "qdata.h"

IMPLEMENT_DYNAMIC(CFSRec, CRecObj)
void CFSRec::Copy(CFSRec &FSRec)
{
	GetIndate() = FSRec.GetIndate();
	GetPortfolio() = FSRec.GetPortfolio();
	GetAsset() = FSRec.GetAsset();
	GetBucket() = FSRec.GetBucket();
	GetTransType() = FSRec.GetTransType();
	GetStrike() = FSRec.GetStrike();
	GetRate() = FSRec.GetRate();
	GetMaturity() = FSRec.GetMaturity();
	GetTransNum() = FSRec.GetTransNum();
	GetNominal()  = FSRec.GetNominal();
	GetAmount() = FSRec.GetAmount();
	GetDailyChange() = FSRec.GetDailyChange();
	GetReason() = FSRec.GetReason();
	GetNote() = FSRec.GetNote();
}

void CFSRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();
	Rec.Add(GetIndate());
	Rec.Add(GetPortfolio());
	Rec.Add(GetAsset());
	Rec.Add(GetBucket());
	Rec.Add(GetTransType());
	Rec.Add(GetStrike());
	Rec.Add(GetRate());
	Rec.Add(GetMaturity());
	Rec.Add(GetTransNum());
	Rec.Add(GetNominal());
	Rec.Add(GetAmount());
	Rec.Add(GetDailyChange());
	Rec.Add(GetReason());
	Rec.Add(GetNote());
}

BOOL CFSRec::AddRec(COraLoader &OraLoader)
{
	if(GetAsset().IsEmpty() && GetTransNum().IsEmpty())
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);

	if(!OraLoader.Open("SELECT INDATE, PORTFOLIO, ASSET_CODE, BUCKET, TRANS_TYPE, "
						"STRIKE, RATE, MATURITY, TRANS_NUM, NOMINAL, AMOUNT, "
						"DAILY_CHANGE, REASON, NOTE "
						"FROM SEMAM.NW_DAILY_FUND_SIZING ", ODYNASET_DEFAULT))
		return FALSE;

	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CFSRec::UpdateRec(COraLoader &OraLoader)
{
	if(GetAsset().IsEmpty() && GetTransNum().IsEmpty())
		return FALSE;

	CString Sql;
	CQData QData;

	Sql = "SELECT INDATE, PORTFOLIO, ASSET_CODE, BUCKET, TRANS_TYPE, "
			"STRIKE, RATE, MATURITY, TRANS_NUM, NOMINAL, AMOUNT, "
			"DAILY_CHANGE, REASON, NOTE "
			"FROM SEMAM.NW_DAILY_FUND_SIZING "
			"WHERE INDATE = ";
	Sql += QData.GetQueryDate(GetIndate());

	if(!GetTransNum().IsEmpty())
	{
		Sql += " AND TRANS_NUM = ";
		Sql += QData.GetQueryNumber(GetTransNum());
	}
	else
	{
		Sql += " AND ASSET_CODE = ";
		Sql += QData.GetQueryText(GetAsset());
	}

	if(!OraLoader.Open(Sql, ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec);
}

BOOL CFSRec::DeleteRec(COraLoader &OraLoader)
{
	if(GetAsset().IsEmpty() && GetTransNum().IsEmpty())
		return FALSE;

	CString Sql;
	CQData QData;

	Sql = "DELETE FROM SEMAM.NW_DAILY_FUND_SIZING WHERE INDATE = ";
	Sql += QData.GetQueryDate(GetIndate());

	if(!GetTransNum().IsEmpty())
	{
		Sql += " AND TRANS_NUM = ";
		Sql += QData.GetQueryNumber(GetTransNum());
	}
	else
	{
		Sql += " AND ASSET_CODE = ";
		Sql += QData.GetQueryText(GetAsset());
	}

	return OraLoader.ExecuteSql();
}

