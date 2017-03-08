#include "stdafx.h"
#include "resource.h"
#include "portfolioAccountdata.h"
#include "qdata.h"


IMPLEMENT_DYNAMIC(CPortfolioAccountRec, CRecObj)
void CPortfolioAccountRec::Copy(CPortfolioAccountRec &PortfolioAccountRec)
{
	SetPortfolio(PortfolioAccountRec.GetPortfolio());
	SetCustodian(PortfolioAccountRec.GetCustodian());
	SetCustodianAccount(PortfolioAccountRec.GetCustodianAccount());
	SetAccountNum(PortfolioAccountRec.GetAccountNum());
	SetAccount(PortfolioAccountRec.GetAccount());
	SetAccount2(PortfolioAccountRec.GetAccount2());
	SetBank(PortfolioAccountRec.GetBank());
	SetEuroClear(PortfolioAccountRec.GetEuroClear());
	SetCustodianCode(PortfolioAccountRec.GetCustodianCode());
	SetMargin(PortfolioAccountRec.GetMargin());
	SetContact(PortfolioAccountRec.GetContact());
	SetPhone(PortfolioAccountRec.GetPhone());
	SetFax(PortfolioAccountRec.GetFax());
}

void CPortfolioAccountRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();

	Rec.Add(GetPortfolio());
	Rec.Add(GetCustodian());
	Rec.Add(GetCustodianAccount());
	Rec.Add(GetAccountNum());
	Rec.Add(GetAccount());
	Rec.Add(GetAccount2());
	Rec.Add(GetBank());
	Rec.Add(GetEuroClear());
	Rec.Add(GetCustodianCode());
	Rec.Add(GetMargin());
	Rec.Add(GetContact());
	Rec.Add(GetPhone());
	Rec.Add(GetFax());
}

BOOL CPortfolioAccountRec::AddRec(COraLoader &OraLoader)
{
	if(GetPortfolio().IsEmpty())
		return FALSE;
	
	OraLoader.GetSql() = "SELECT PORTFOLIO, CUSTODIAN, CUSTODIAN_ACCOUNT, ACC_NUM, "
						"ACCOUNT, ACCOUNT2, BANK, EURO_CLEAR, CUSTODIAN_CODE, MARGIN, "
						"CONTACT, PHONE, FAX FROM SEMAM.NW_PORTFOLIO_ACCOUNTS ";
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CPortfolioAccountRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetPortfolio().IsEmpty())
		return FALSE;
	
	CQData QData;
	OraLoader.GetSql().Format("SELECT PORTFOLIO, CUSTODIAN, CUSTODIAN_ACCOUNT, ACC_NUM, "
							"ACCOUNT, ACCOUNT2, BANK, EURO_CLEAR, CUSTODIAN_CODE, MARGIN, "
							"CONTACT, PHONE, FAX FROM SEMAM.NW_PORTFOLIO_ACCOUNTS "
							"WHERE PORTFOLIO = %s ", QData.GetQueryText(GetPortfolio()));
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec);
}

BOOL CPortfolioAccountRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetPortfolio().IsEmpty())
		return FALSE;
	
	CQData QData;
	
	OraLoader.GetSql().Format("DELETE SEMAM.NW_PORTFOLIO_ACCOUNTS WHERE PORTFOLIO = %s ", 
							QData.GetQueryText(GetPortfolio()));
	return OraLoader.ExecuteSql();
}




IMPLEMENT_DYNAMIC(CPortfolioAccountData, CRowCtrl)
BOOL CPortfolioAccountData::IsReady()
{
	return CRowCtrl::IsReady() && GetOraLoader().GetDB() ? TRUE : FALSE;
}

BOOL CPortfolioAccountData::LoadDBData()
{
	if(!IsReady())
		return FALSE;

	if(!GetOraLoader().Open("SELECT A.PORTFOLIO, CUSTODIAN, CUSTODIAN_ACCOUNT, ACC_NUM, "
							"ACCOUNT, ACCOUNT2, BANK, EURO_CLEAR, CUSTODIAN_CODE, MARGIN, "
							"CONTACT, PHONE, FAX "
							"FROM SEMAM.NW_PORTFOLIO_ACCOUNTS A, SEMAM.NW_PORTFOLIOS B "
							"WHERE A.PORTFOLIO = B.PORTFOLIO AND B.STATUS IS NULL ORDER BY 1 "))
		return FALSE;
	return GetOraLoader().LoadRowCtrl(*this, TRUE, TRUE);
}

BOOL CPortfolioAccountData::AddRow()
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

BOOL CPortfolioAccountData::DeleteRow()
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

BOOL CPortfolioAccountData::UpdateRow()
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

