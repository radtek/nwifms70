#include "stdafx.h"
#include "NWIFMS70.h"
#include <math.h>
#include "autoallocdata.h"
#include "qdata.h"
#include "nwivar.h"


IMPLEMENT_DYNAMIC(CAutoAllocRec, CRecObj)
void CAutoAllocRec::Copy(CAutoAllocRec &AutoAllocRec)
{
	CRecObj::Copy(AutoAllocRec);

	SetAAMethod(AutoAllocRec.GetAAMethod());
	SetFromDate(AutoAllocRec.GetFromDate());
	SetToDate(AutoAllocRec.GetToDate());
	SetPortfolio(AutoAllocRec.GetPortfolio());
	SetAllocRate(AutoAllocRec.GetAllocRate());
}

void CAutoAllocRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();
	Rec.Add(GetAAMethod());
	Rec.Add(GetFromDate());
	Rec.Add(GetToDate());
	Rec.Add(GetPortfolio());
	Rec.Add(GetAllocRate());
}

BOOL CAutoAllocRec::AddRec(COraLoader &OraLoader)
{
	if(!OraLoader.Open("SELECT AA_METHOD, FROM_DATE, TO_DATE, PORTFOLIO, ALLOC "
						"FROM SEMAM.NW_AA_ALLOC ", ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);
	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CAutoAllocRec::UpdateRec(COraLoader &OraLoader, LPCTSTR ID)
{
	if(GetAAMethod().IsEmpty() || strlen(ID) <= 0)
		return FALSE;
	
	CQData QData;
	OraLoader.GetSql().Format("SELECT AA_METHOD, FROM_DATE, TO_DATE, PORTFOLIO, ALLOC "
								"FROM SEMAM.NW_AA_ALLOC WHERE ROWIDTOCHAR(ROWID) = %s ", 
								QData.GetQueryText(ID)); // Rowid 
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;
	
	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec);
}

BOOL CAutoAllocRec::DeleteRec(COraLoader &OraLoader)
{
	if(GetAAMethod().IsEmpty())
		return FALSE;

	CQData QData;
	OraLoader.GetSql().Format("DELETE FROM SEMAM.NW_AA_ALLOC WHERE ROWIDTOCHAR(ROWID) = %s ", 
								QData.GetQueryText(GetID()));
	return OraLoader.ExecuteSql(); 
}



IMPLEMENT_DYNAMIC(CAutoAllocData, CRowCtrl)
BOOL CAutoAllocData::IsReady()
{
	return CRowCtrl::IsReady() && GetOraLoader().GetDB() ? TRUE : FALSE;
}

BOOL CAutoAllocData::LoadDBData()
{
	if(!IsReady())
		return FALSE;

	if(!GetOraLoader().Open("SELECT ROWIDTOCHAR(A.ROWID), A.AA_METHOD, FROM_DATE, "
							"TO_DATE, PORTFOLIO, ALLOC "
							"FROM SEMAM.NW_AA_ALLOC A, SEMAM.NW_AA_METHOD_V B "
							"WHERE B.AA_METHOD = A.AA_METHOD ORDER BY 2 "))
		return FALSE;
	return GetOraLoader().LoadRowCtrl(*this, TRUE);
}

BOOL CAutoAllocData::AddRow()
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

BOOL CAutoAllocData::DeleteRow()
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

BOOL CAutoAllocData::UpdateRow()
{
	if(!IsReady())
		return FALSE;

	UpdateData();

	GetOraLoader().BeginTransaction();
	if(GetRec().UpdateRec(GetOraLoader(), GetID()))
	{
		GetOraLoader().Commit();
		UpdateSheetRow(FALSE);
		return TRUE;
	}

	GetOraLoader().Rollback();
	return FALSE;
}
