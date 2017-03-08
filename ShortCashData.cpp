#include "stdafx.h"
#include "NWIFMS70.h"
#include <math.h>
#include "qdata.h"
#include "sqlstmt.h"
#include "shortcashdata.h"
#include "qdata.h"

IMPLEMENT_DYNAMIC(CShortCashData, CRowCtrl)
int CShortCashData::LoadDBData()
{
	CString Sql;
	CStringArray FieldVals, QRec;

	if(!IsReady())
		return -1;

    Sql = "SELECT ROWIDTOCHAR(ROWID), INDATE, PORTFOLIO, ACCOUNT, CURRENCY, AMOUNT "
			"FROM SEMAM.NW_SHORT_CASH WHERE ";

	ToStringArray(FieldVals, TRUE);
	if(GetOraLoader().GetFieldArray().GetSize() == 0)
		GetOraLoader().Open("SELECT INDATE, PORTFOLIO, ACCOUNT, CURRENCY, AMOUNT "
							"FROM SEMAM.NW_SHORT_CASH ");

	GetOraLoader().RecToQueryRec(QRec, FieldVals, &GetOraLoader().GetFieldArray(), "%");
	GetOraLoader().SetSqlSearchInfo(Sql, QRec);

	if(GetOraLoader().Open(Sql))
		return GetOraLoader().LoadRowCtrl(*this, TRUE, FALSE);

	return -1;
}

BOOL CShortCashData::IsRowOK()
{
	if(GetDate().IsEmpty())
	{
		MessageBox(NULL, "Must Enter a Date", NULL, MB_OK);
		return FALSE;
	}

	if(GetPortfolio().IsEmpty())
	{
		MessageBox(NULL, "Must Select a Portfolio", NULL, MB_OK);
		return FALSE;
	}

	if(GetAccount().IsEmpty())
	{
		MessageBox(NULL, "Must Select an Account", NULL, MB_OK);
		return FALSE;
	}

	if(GetCurrency().IsEmpty())
	{
		MessageBox(NULL, "Must Select a Currency", NULL, MB_OK);
		return FALSE;
	}

	if(GetAmount().IsEmpty())
	{
		MessageBox(NULL, "Must Enter Amount", NULL, MB_OK);
		return FALSE;
	}

	return TRUE;
}

BOOL CShortCashData::SaveData(BOOL bAdd)
{
	UpdateData();

	if(bAdd)
		GetOraLoader().GetSql() = "SELECT INDATE, PORTFOLIO, ACCOUNT, "
								"CURRENCY, AMOUNT FROM SEMAM.NW_SHORT_CASH ";
	else
		GetOraLoader().GetSql().Format("SELECT INDATE, PORTFOLIO, ACCOUNT, "
				"CURRENCY, AMOUNT FROM SEMAM.NW_SHORT_CASH "
				"WHERE ROWIDTOCHAR(ROWID) = '%s' ", GetID());

	if(!GetOraLoader().Open(GetOraLoader().GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	
	GetOraLoader().BeginTransaction();
	if(GetOraLoader().UpdateRecord(Rec, bAdd))
	{
		GetOraLoader().Commit();
		if(bAdd)
			AddSheetRow();
		else
			UpdateSheetRow(FALSE);
		Refresh();
		return TRUE;
	}

	GetOraLoader().Rollback();
	return FALSE;
}

int CShortCashData::DeleteData()
{
	UpdateData();

	GetOraLoader().GetSql().Format("DELETE SEMAM.NW_SHORT_CASH "
				"WHERE ROWIDTOCHAR(ROWID) = '%s' ", GetID());
	GetOraLoader().BeginTransaction();
	if(GetOraLoader().ExecuteSql())
	{
		GetOraLoader().Commit();
		DeleteSheetRow(GetCurrentRow());
		Refresh();
		return TRUE;
	}

	GetOraLoader().Rollback();
	return FALSE;
}
