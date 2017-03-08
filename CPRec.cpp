#include "stdafx.h"
#include "resource.h"
#include "cprec.h"
#include "nwivar.h"
#include "sqlstmt.h"
#include "qdata.h"


IMPLEMENT_DYNAMIC(CCTRec, CRecObj)
void CCTRec::Copy(CCTRec &CTRec)
{
	SetID(CTRec.GetID());
	SetCP(CTRec.GetCP());
	SetCTCode(CTRec.GetCTCode());
	SetName(CTRec.GetName());
	SetCT(CTRec.GetCT());
	SetAddress(CTRec.GetAddress());
	SetAddress2(CTRec.GetAddress2());
	SetPhone(CTRec.GetPhone());
	SetFax(CTRec.GetPhone());
	SetCedel(CTRec.GetCedel());
	SetEuroClear(CTRec.GetEuroClear());
	SetMSTC(CTRec.GetMSTC());
	SetFedID(CTRec.GetFedID());
	SetLimit(CTRec.GetLimit());
	SetIsin(CTRec.GetIsin());
	SetBic(CTRec.GetBic());
	SetDTC(CTRec.GetDTC());
	SetGroup(CTRec.GetGroup());
	SetSecID(CTRec.GetSecID());
	SetFSAID(CTRec.GetFSAID());
	SetCTIntl(CTRec.GetCTIntl());
	SetCTFx(CTRec.GetCTFx());
	SetActive(CTRec.GetActive());

	SetUser(CTRec.GetUser());
}

void CCTRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();

	Rec.Add(GetCP());
	Rec.Add(GetCTCode());
	Rec.Add(GetName());
	Rec.Add(GetCT());
	Rec.Add(GetAddress());
	Rec.Add(GetAddress2());
	Rec.Add(GetPhone());
	Rec.Add(GetFax());
	Rec.Add(GetCedel());
	Rec.Add(GetEuroClear());
	Rec.Add(GetMSTC());
	Rec.Add(GetFedID());
	Rec.Add(GetLimit());
	Rec.Add(GetIsin());
	Rec.Add(GetBic());
	Rec.Add(GetDTC());
	Rec.Add(GetGroup());
	Rec.Add(GetSecID());
	Rec.Add(GetFSAID());
	Rec.Add(GetCTIntl());
	Rec.Add(GetCTFx());
	Rec.Add(GetActive());
	Rec.Add(GetUser());
	Rec.Add(DBSYSDATE);
}

BOOL CCTRec::AddRec(COraLoader &OraLoader)
{
	if(GetCP().IsEmpty() || GetCTCode().IsEmpty())
		return FALSE;
	
	CQData QData;
	CString CtCode, Sql;

	CtCode = QData.GetQueryText(GetCTCode());
	Sql = "SELECT COUNT(*) FROM %s WHERE %s = %s AND %s = %s ";
	OraLoader.GetSql().Format((LPCTSTR ) Sql, "SEMAM.NW_COUNTER_CONTACTS", "CP_CODE", 
								QData.GetQueryText(GetCP()), "CT_CODE", (LPCTSTR) CtCode);
	if(OraLoader.IsExist() > 0)
	{
		MessageBox(NULL, "This Counterparty contact exist already", NULL, MB_OK);
		return FALSE;
	}

	if(!OraLoader.Open("SELECT CP_CODE, CT_CODE, CT_NAME, CT_CONTACT, CT_ADDRESS1, CT_ADDRESS2, "
						"CT_PHONE, CT_FAX, CT_CEDEL_NUM, CT_EURO_NUM, CT_MSTC_CODE, CT_FED_ID, "
						"CT_LIMIT, CT_ISIN_NUM, CT_BIC, CT_DTC_NUM, CT_GROUP, CT_SEC_ID, "
						"CT_FSA_ID, CT_INTL, CT_FX, ISVALID, USER_STAMP, DATE_STAMP "
						"FROM SEMAM.NW_COUNTER_CONTACTS ", ODYNASET_DEFAULT))
		return FALSE;
	
	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CCTRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(strlen(GetCP()) == 0 || strlen(GetCTCode()) == 0 || strlen(GetID()) == 0)
		return FALSE;
	
	CQData QData;

	OraLoader.GetSql().Format("SELECT CP_CODE, CT_CODE, CT_NAME, CT_CONTACT, CT_ADDRESS1, "
								"CT_ADDRESS2, CT_PHONE, CT_FAX, CT_CEDEL_NUM, CT_EURO_NUM, "
								"CT_MSTC_CODE, CT_FED_ID, CT_LIMIT, CT_ISIN_NUM, CT_BIC, "
								"CT_DTC_NUM, CT_GROUP, CT_SEC_ID, CT_FSA_ID, CT_INTL, "
								"CT_FX, ISVALID, USER_STAMP, DATE_STAMP "
								"FROM SEMAM.NW_COUNTER_CONTACTS WHERE ROWIDTOCHAR(ROWID) = %s", 
								QData.GetQueryText(GetID())); // Rowid 
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec);
}

BOOL CCTRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetCP().IsEmpty())
		return FALSE;

	CQData QData;

	if(bByID)
	{
		if(strlen(GetCTCode()) == 0 || strlen(GetID()) == 0)
			return FALSE;
		OraLoader.GetSql().Format("DELETE SEMAM.NW_COUNTER_CONTACTS WHERE ROWIDTOCHAR(ROWID) = %s", 
									QData.GetQueryText(GetID()));
	}
	else
		OraLoader.GetSql().Format("DELETE SEMAM.NW_COUNTER_CONTACTS WHERE CP_CODE = %s ", 
									QData.GetQueryText(GetCP()));

	return OraLoader.ExecuteSql();
}



IMPLEMENT_DYNAMIC(CCPRec, CRecObj)
void CCPRec::Copy(CCPRec &CPRec)
{
	SetCPCode(CPRec.GetCPCode());
    SetCP(CPRec.GetCP());
	SetImgCP(CPRec.GetImgCP());
	SetLimit(CPRec.GetLimit());
	SetSetLimit(CPRec.GetSetLimit());
	SetPru(CPRec.GetPru());
	SetActive(CPRec.GetActive());

	SetUser(CPRec.GetUser());
}

void CCPRec::ToDBRec(CDBRec &Rec)
{
	Rec.RemoveAll();

	Rec.Add(GetCPCode());
    Rec.Add(GetCP());
	Rec.Add(GetImgCP());
	Rec.Add(GetLimit());
	Rec.Add(GetSetLimit());
	Rec.Add(GetPru());
	Rec.Add(GetActive());
	Rec.Add(GetUser());
	Rec.Add(DBSYSDATE);
}

BOOL CCPRec::AddRec(COraLoader &OraLoader)
{
	CQData QData;

	OraLoader.GetSql().Format("SELECT COUNT(*) FROM %s WHERE %s = %s ", 
				"SEMAM.NW_COUNTER_CONTACTS", "CP_CODE", QData.GetQueryText(GetCP()));
	if(OraLoader.IsExist() > 0)
	{
		MessageBox(NULL, "This Counterparty contact exist already", NULL, MB_OK);
		return FALSE;
	}

	if(!OraLoader.Open("SELECT CP_CODE, CP_MAIN_NAME, IMG_CP_CODE, CP_LIMIT, "
						"CP_SET_LIMIT, PRU, ISVALID, USER_STAMP, DATE_STAMP "
						"FROM SEMAM.NW_COUNTER_PARTY ", ODYNASET_DEFAULT))
		return FALSE;
	
	CDBRec Rec;
	ToDBRec(Rec);

	return OraLoader.UpdateRecord(Rec, TRUE);
}

BOOL CCPRec::UpdateRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetCPCode().IsEmpty())
		return FALSE;
	
	CQData QData;
	CDBRec Rec;	

	OraLoader.GetSql().Format("SELECT CP_CODE, CP_MAIN_NAME, IMG_CP_CODE, "
								"CP_LIMIT, CP_SET_LIMIT, PRU, ISVALID, "
								"USER_STAMP, DATE_STAMP FROM SEMAM.NW_COUNTER_PARTY "
								"WHERE CP_CODE = %s", QData.GetQueryText(GetCPCode()));

	ToDBRec(Rec);
	if(!OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT))
		return FALSE;
	return OraLoader.UpdateRecord(Rec);
}

BOOL CCPRec::DeleteRec(COraLoader &OraLoader, BOOL bByID)
{
	if(GetCPCode().IsEmpty())
		return FALSE;

	CCTRec CTRec;
	CQData QData;

	CTRec.SetCP(GetCPCode());
	CTRec.DeleteRec(OraLoader);

	OraLoader.GetSql().Format("DELETE SEMAM.NW_COUNTER_PARTY WHERE CP_CODE = %s ", 
								QData.GetQueryText(GetCPCode()));
	return OraLoader.ExecuteSql();
}
