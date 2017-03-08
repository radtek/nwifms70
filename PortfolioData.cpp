#include "stdafx.h"
#include "resource.h"
#include "portfoliodata.h"
#include "qdata.h"


IMPLEMENT_DYNAMIC(CPortfolioRec, CRecObj)
void CPortfolioRec::Copy(CPortfolioRec &PortfolioRec)
{
	SetPortfolio(PortfolioRec.GetPortfolio());
	SetFullName(PortfolioRec.GetFullName());
	SetAmount(PortfolioRec.GetAmount());
	SetCtrlFactor(PortfolioRec.GetCtrlFactor());
	SetWtFactor(PortfolioRec.GetWtFactor());
	SetTableSpace(PortfolioRec.GetTableSpace());
	SetCustodian(PortfolioRec.GetCustodian());
	SetAccount(PortfolioRec.GetAccount());
	SetCurrency(PortfolioRec.GetCurrency());
	SetAccountant(PortfolioRec.GetAccountant());
	SetStatus(PortfolioRec.GetStatus());
}

void CPortfolioRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();

	Rec.Add(GetPortfolio());
	Rec.Add(GetFullName());
	Rec.Add(GetAmount());
	Rec.Add(GetCtrlFactor());
	Rec.Add(GetWtFactor());
	Rec.Add(GetTableSpace());
	Rec.Add(GetCustodian());
	Rec.Add(GetAccount());
	Rec.Add(GetCurrency());
	Rec.Add(GetAccountant());
	Rec.Add(GetStatus());
}

BOOL CPortfolioRec::AddRec(COraLoader &OraLoader)
{
	if(GetPortfolio().IsEmpty() || GetTableSpace().IsEmpty())
		return FALSE;
	
	OraLoader.GetSql() = "SELECT PORTFOLIO, FULL_NAME, INV_AMOUNT, EQUITY_CONTROL, EMERGING_MKT_WT_FACTOR, "
						"TABLESPACE_NAME, DEF_CUSTODIAN, DEF_ACCOUNT, BASE_CURRENCY, ACCOUNTANT, STATUS "
						"FROM SEMAM.NW_PORTFOLIOS ";
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CPortfolioRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetPortfolio().IsEmpty() || GetTableSpace().IsEmpty())
		return FALSE;
	
	CQData QData;
	
	OraLoader.GetSql().Format("SELECT PORTFOLIO, FULL_NAME, INV_AMOUNT, EQUITY_CONTROL, EMERGING_MKT_WT_FACTOR, "
								"TABLESPACE_NAME, DEF_CUSTODIAN, DEF_ACCOUNT, BASE_CURRENCY, ACCOUNTANT, STATUS "
								"FROM SEMAM.NW_PORTFOLIOS WHERE PORTFOLIO = %s ", QData.GetQueryText(GetPortfolio()));
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec);
}

BOOL CPortfolioRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetPortfolio().IsEmpty() || GetTableSpace().IsEmpty())
		return FALSE;
	
	CQData QData;
	
	OraLoader.GetSql().Format("DELETE SEMAM.NW_PORTFOLIOS WHERE PORTFOLIO = %s ", QData.GetQueryText(GetPortfolio()));
	return OraLoader.ExecuteSql();
}




IMPLEMENT_DYNAMIC(CPortfolioData, CRowCtrl)
BOOL CPortfolioData::IsReady()
{
	return CRowCtrl::IsReady() && GetOraLoader().GetDB() ? TRUE : FALSE;
}

BOOL CPortfolioData::LoadDBData()
{
	if(!IsReady())
		return FALSE;

	if(!GetOraLoader().Open("SELECT PORTFOLIO, FULL_NAME, TABLESPACE_NAME, DEF_CUSTODIAN, DEF_ACCOUNT, "
							"BASE_CURRENCY, ACCOUNTANT, INV_AMOUNT, EQUITY_CONTROL, EMERGING_MKT_WT_FACTOR, "
							"STATUS FROM SEMAM.ALL_PORTFOLIOS ORDER BY 1 "))
		return FALSE;
	return GetOraLoader().LoadRowCtrl(*this, TRUE, TRUE);
}

BOOL CPortfolioData::AddRow()
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

BOOL CPortfolioData::DeleteRow()
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

BOOL CPortfolioData::UpdateRow()
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

