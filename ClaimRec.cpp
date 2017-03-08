#include "stdafx.h"
#include "resource.h"
#include "claimrec.h"
#include "nwivar.h"
#include "sqlstmt.h"
#include "qdata.h"


IMPLEMENT_DYNAMIC(CClaimRec, CRecObj)
void CClaimRec::Copy(CClaimRec &ClaimRec)
{
	SetID(ClaimRec.GetID());
	SetCP(ClaimRec.GetCP());
	SetClaimCode(ClaimRec.GetClaimCode());
	SetName(ClaimRec.GetName());
	SetPhone(ClaimRec.GetPhone());
	SetFax(ClaimRec.GetPhone());
	SetEMail(ClaimRec.GetEMail());
}

void CClaimRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();

	Rec.Add(GetCP());
	Rec.Add(GetClaimCode());
	Rec.Add(GetName());
	Rec.Add(GetPhone());
	Rec.Add(GetFax());
	Rec.Add(GetEMail());
}

BOOL CClaimRec::AddRec(COraLoader &OraLoader)
{
	if(GetCP().IsEmpty())
		return FALSE;

	OraLoader.GetSql() = "SELECT CP_CODE, CLAIM_CODE, NAME, PHONE, FAX, EMAIL FROM SEMAM.NW_CLAIM_CP ";

	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CClaimRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(strlen(GetCP()) == 0)
		return FALSE;
	
	CQData QData;

	OraLoader.GetSql().Format("SELECT CP_CODE, CLAIM_CODE, NAME, PHONE, FAX, EMAIL FROM SEMAM.NW_CLAIM_CP "
							"WHERE ROWIDTOCHAR(ROWID) = %s ", QData.GetQueryText(GetID()));

	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec);
}

BOOL CClaimRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetCP().IsEmpty())
		return FALSE;

	CQData QData;

	OraLoader.GetSql().Format("DELETE SEMAM.NW_CLAIM_CP WHERE ROWIDTOCHAR(ROWID) = %s ", 
							QData.GetQueryText(GetID()));

	return OraLoader.ExecuteSql();
}
