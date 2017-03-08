#include "stdafx.h"
#include "resource.h"
#include "portfolioinfodata.h"
#include "qdata.h"


IMPLEMENT_DYNAMIC(CPortfolioInfoRec, CRecObj)
void CPortfolioInfoRec::Copy(CPortfolioInfoRec &PortfolioInfoRec)
{
	SetPortfolio(PortfolioInfoRec.GetPortfolio());
	SetCustodian(PortfolioInfoRec.GetCustodian());
	SetCustodianAccount(PortfolioInfoRec.GetCustodianAccount());
	SetAccount(PortfolioInfoRec.GetAccount());
	SetAccount2(PortfolioInfoRec.GetAccount2());
	SetBank(PortfolioInfoRec.GetBank());
	SetEuroClear(PortfolioInfoRec.GetEuroClear());
	SetMargin(PortfolioInfoRec.GetMargin());
	SetContact(PortfolioInfoRec.GetContact());
	SetPhone(PortfolioInfoRec.GetPhone());
	SetFax(PortfolioInfoRec.GetFax());
}

void CPortfolioInfoRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();

	Rec.Add(GetPortfolio());
	Rec.Add(GetCustodian());
	Rec.Add(GetCustodianAccount());
	Rec.Add(GetAccount());
	Rec.Add(GetAccount2());
	Rec.Add(GetBank());
	Rec.Add(GetEuroClear());
	Rec.Add(GetMargin());
	Rec.Add(GetContact());
	Rec.Add(GetPhone());
	Rec.Add(GetFax());
}

BOOL CPortfolioInfoRec::AddRec(COraLoader &OraLoader)
{
	if(GetPortfolio().IsEmpty())
		return FALSE;
	
	OraLoader.GetSql() = "SELECT PORTFOLIO, CUSTODIAN, CUSTODIAN_ACCOUNT, ACCOUNT, "
						"ACCOUNT2, BANK, EURO_CLEAR, MARGIN, CONTACT, PHONE, FAX "
						"FROM SEMAM.NW_PORTFOLIO_INFO ";
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CPortfolioInfoRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetPortfolio().IsEmpty())
		return FALSE;
	
	CQData QData;
	OraLoader.GetSql().Format("SELECT PORTFOLIO, CUSTODIAN, CUSTODIAN_ACCOUNT, ACCOUNT, "
							"ACCOUNT2, BANK, EURO_CLEAR, MARGIN, CONTACT, PHONE, FAX "
							"FROM SEMAM.NW_PORTFOLIO_INFO WHERE PORTFOLIO = %s ", 
							QData.GetQueryText(GetPortfolio()));
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec);
}

BOOL CPortfolioInfoRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetPortfolio().IsEmpty())
		return FALSE;
	
	CQData QData;
	
	OraLoader.GetSql().Format("DELETE SEMAM.NW_PORTFOLIO_INFO WHERE PORTFOLIO = %s ", 
							QData.GetQueryText(GetPortfolio()));
	return OraLoader.ExecuteSql();
}




IMPLEMENT_DYNAMIC(CPortfolioInfoData, CRowCtrl)
BOOL CPortfolioInfoData::IsReady()
{
	return CRowCtrl::IsReady() && GetOraLoader().GetDB() ? TRUE : FALSE;
}

BOOL CPortfolioInfoData::LoadDBData()
{
	if(!IsReady())
		return FALSE;

	if(!GetOraLoader().Open("SELECT A.PORTFOLIO, CUSTODIAN, CUSTODIAN_ACCOUNT, ACCOUNT, "
						"ACCOUNT2, BANK, EURO_CLEAR, MARGIN, CONTACT, PHONE, FAX "
						"FROM SEMAM.NW_PORTFOLIO_INFO A,SEMAM.NW_PORTFOLIOS B "
						"WHERE A.PORTFOLIO = B.PORTFOLIO AND B.STATUS IS NULL ORDER BY 1 "))
		return FALSE;
	return GetOraLoader().LoadRowCtrl(*this, TRUE, TRUE);
}

BOOL CPortfolioInfoData::AddRow()
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

BOOL CPortfolioInfoData::DeleteRow()
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

BOOL CPortfolioInfoData::UpdateRow()
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

