#include "stdafx.h"
#include <math.h>
#include "resource.h"
#include "reportdata.h"
#include "qdata.h"


IMPLEMENT_DYNAMIC(CReportBaseData, CObArray)

int	CReportBaseData::Add(CDBRec *Rec)
{
	return CObArray::Add(Rec);
}

int CReportBaseData::Add(CDBRec &Rec)
{
	return CObArray::Add(new CDBRec(Rec));
}

void CReportBaseData::SetAt(int Index, CDBRec *Rec)
{
	CDBRec *pRec;

	pRec = GetAt(Index);
	if(pRec)
	{
		delete pRec;
		CObArray::SetAt(Index, Rec);
	}
}

void CReportBaseData::SetAt(int Index, CDBRec &Rec)
{
	CDBRec *pRec;

	pRec = GetAt(Index);
	if(pRec)
	{
		delete pRec;
		CObArray::SetAt(Index, new CDBRec(Rec));
	}
}

void CReportBaseData::RemoveAt(int Index)
{
	CDBRec *pRec;
	
	pRec = GetAt(Index);
	if(pRec)
	{
		CObArray::RemoveAt(Index);
		delete pRec;		
	}
}

void CReportBaseData::RemoveAt(CDBRec &Rec)
{
	int i;
	
	if(Rec.GetSize() <= 0)
		return;

	i = Find(Rec.GetAt(0), 0);
	if(i < 0)
		return;

	if(Rec.GetSize() == 1)
	{
		while(GetSize() > 0 && strcmp(GetAt(i)->GetAt(0), Rec.GetAt(0)) == 0)
			RemoveAt(i);			
	}
	else
	{
		while(GetSize() > 0 && GetSize() > i && strcmp(GetAt(i)->GetAt(0), Rec.GetAt(0)) == 0)
		{
			if(strcmp(GetAt(i)->GetAt(1), Rec.GetAt(1)) == 0)
				RemoveAt(i);
			else
				i++;
		}
	}
}

void CReportBaseData::RemoveAll()
{
	int	i;
	CDBRec *pRec;
	
	for(i = 0; i < GetSize(); i++)
	{
		pRec = GetAt(i);
		if(pRec)
			delete pRec;				
    }
    
    CObArray::RemoveAll();
}

CDBRec *CReportBaseData::GetAt(int Index)
{
	if(Index < 0 || Index > GetSize())
		return NULL;
	return (CDBRec*) CObArray::GetAt(Index);
}

void CReportBaseData::InsertAt(int Index, CDBRec *Rec)
{
	CObArray::InsertAt(Index, Rec);	
}

void CReportBaseData::InsertAt(int Index, CDBRec &Rec)
{
	CObArray::InsertAt(Index, new CDBRec(Rec));
}

void CReportBaseData::InsertAt(CDBRec &Rec)
{
	int i, Ret;
	
	if(Rec.GetSize() < 2)
		return;

	i = Find(Rec.GetAt(0), 0);
	if(i < 0)
	{
		Add(Rec);
		return;
	}

	while(i < GetSize() && strcmp(GetAt(i)->GetAt(0), Rec.GetAt(0)) == 0)
	{
		Ret = strcmp(GetAt(i)->GetAt(1), Rec.GetAt(1));
		if(Ret < 0)
			i++;
		else
		{
			if(Ret > 0)
			{
				if(i == 0)
					InsertAt(0, Rec);
				else
					InsertAt(i - 1, Rec);
			}
			return;
		}
	}

	if(i == GetSize())
		Add(Rec);
}

int CReportBaseData::FindListed(int Index)
{
	int	i;
	CDBRec*	Data;

	for(i = Index; i < GetSize(); i ++)
	{
		Data = GetAt(i);
		if(Data && Data->GetListed())
			return i;
	}

	return -1;
}

CDBRec *CReportBaseData::GetListedAt(int Index)
{
	int	i;
	CDBRec*	Data;
	
	for(i = Index; i < GetSize(); i ++)
	{
		Data = GetAt(i);
		if(Data && Data->GetListed())
			return Data;
	}
	
	return NULL;
}

int CReportBaseData::Find(LPCTSTR Key, int FieldID)
{
	// If FieldID == -1, Use RecID.  Otherwise, use the field in m_Rec accordingly
	int i;
	CDBRec* Data;

	if(!Key || strlen(Key) == 0)
		return -1;
	
	if(GetSize() <= 0)
		return -1;

	if(GetAt(0)->GetSize() <= FieldID) // if Col is out of range, set default to zero
		FieldID = 0;

	for(i = 0; i < GetSize(); i++)
	{
		Data = GetAt(i);
		if(Data)
		{
			if(FieldID < 0)
			{
				if(strcmp(Data->GetRecID(), Key) == 0)
					return i;
			}
			else
			{
				if(strcmp(Data->GetAt(FieldID), Key) == 0)
					return i;
			}
		}
	}

	return -1;
}

int CReportBaseData::CopyField(CHLBox *FieldBox)
{
	CString Text;
	
	FieldBox->ResetContent();
	for(int i = 0; i < GetSize(); i ++)
	{		
		if(Text != GetAt(i)->GetAt(0))
		{
			Text = GetAt(i)->GetAt(0);
			FieldBox->AddString(Text);			
		}
	}

	return FieldBox->GetCount();
}

int CReportBaseData::CopyFieldValue(CHLBox *ValueBox, LPCTSTR FieldName)
{
	ValueBox->ResetContent();

	for(int i = 0; i < GetSize(); i ++)
		if(strcmp(GetAt(i)->GetAt(0), FieldName) == 0)
			ValueBox->AddString(GetAt(i)->GetAt(1));

	return ValueBox->GetCount();
}



IMPLEMENT_DYNAMIC(CReportData, CObject)
int CReportData::LoadDate(COptComboBox *ComboBox)
{
	ComboBox->ResetContent();
	GetOraLoader().GetSql().Format("SELECT DISTINCT INDATE FROM SEMAM.%s ORDER BY 1 DESC", 
									GetTableName());
	if(GetOraLoader().Open())
		return GetOraLoader().LoadComboBox(*ComboBox);
	return -1;
}

int CReportData::LoadReportArray()
{
	GetAvail().GetReportArray().RemoveAll();
	if(GetOraLoader().Open("SELECT REPORT, TABLE_NAME, REPORT_ID, WITH_UNION "
							"FROM SEMAM.NW_FMS_REPORTS ORDER BY REPORT_ID "))
		return GetOraLoader().LoadDBRecArray(GetAvail().GetReportArray());
	return -1;
}
	
int CReportData::LoadFieldArray()
{
	int i;
	CDBRec Rec;
	CString FldSql;
	CStringArray VArray;

	GetAvail().RemoveAll();
	GetOraLoader().GetSql().Format("SELECT * FROM SEMAM.%s ", GetTableName());
	
	if(!GetOraLoader().Open())
		return -1;
	
	for(i = 0; i < GetOraLoader().GetFieldArray().GetSize(); i++)
		if(GetOraLoader().GetFieldArray().GetAt(i)->GetType() == OTYPE_VARCHAR2)
			VArray.Add(GetOraLoader().GetFieldArray().GetAt(i)->GetName());

	Rec.Add(EMPTYSTRING);
	Rec.Add(EMPTYSTRING);
	if(GetWithUnion())
		FldSql = "SELECT DISTINCT %s FROM SEMAM.%s WHERE %s IS NOT NULL "
				"UNION SELECT DISTINCT %s FROM MNGACC.%s WHERE %s IS NOT NULL "
				"ORDER BY 1";
	else
		FldSql = "SELECT DISTINCT %s FROM SEMAM.%s WHERE %s IS NOT NULL";
	for(i = 0; i < VArray.GetSize(); i ++)
	{		
		Rec.SetAt(0, VArray.GetAt(i));
		if(GetWithUnion())
			GetOraLoader().GetSql().Format((LPCTSTR) FldSql, Rec.GetAt(0), GetTableName(),	
									Rec.GetAt(0), Rec.GetAt(0), GetTableName(), Rec.GetAt(0));
		else
			GetOraLoader().GetSql().Format((LPCTSTR) FldSql, Rec.GetAt(0), GetTableName(), Rec.GetAt(0));

		if(GetOraLoader().Open())
		{
			GetOraLoader().MoveFirst();
			while(!GetOraLoader().IsEOF())
			{
				Rec.SetAt(1, GetOraLoader().GetDBString(0));
				GetAvail().Add(Rec);
				GetOraLoader().MoveNext();
			}
		}
	}

	return GetAvail().GetSize();
}
	
int CReportData::LoadReportDefArray()
{
	GetSel().GetReportArray().RemoveAll();
	GetOraLoader().GetSql().Format("SELECT DEF_NAME, DEF_ID FROM SEMAM.NW_FMS_REPORT_DEF "
									"WHERE REPORT_ID = %s AND USER_INI = '%s' ORDER BY DEF_ID", 
									GetReportID(), GetUserIni());

	if(GetOraLoader().Open())
		return GetOraLoader().LoadDBRecArray(GetSel().GetReportArray());
	return -1;
}

int CReportData::LoadSelFieldArray()
{
	CDBRec Rec;

	GetSel().RemoveAll();
	GetOraLoader().GetSql().Format("SELECT DISTINCT FIELD_NAME, FIELD_VALUE "
									"FROM SEMAM.NW_FMS_REPORT_DEF_FIELDS "
									"WHERE DEF_ID = %s", GetDefID());
	if(GetOraLoader().Open())
	{
		GetOraLoader().MoveFirst();
		while(!GetOraLoader().IsEOF())
		{
			Rec.RemoveAll();
			Rec.Add(GetOraLoader().GetDBString(0));
			Rec.Add(GetOraLoader().GetDBString(1));
			GetSel().Add(Rec);
			GetOraLoader().MoveNext();
		}
	}

	return GetSel().GetSize();
}

BOOL CReportData::AddFields()
{
	CDBRec Rec;

	GetOraLoader().Open("SELECT DEF_ID, FIELD_NAME, FIELD_VALUE "
						"FROM SEMAM.NW_FMS_REPORT_DEF_FIELDS ", ODYNASET_DEFAULT);
	GetOraLoader().BeginTransaction();
	for(int i = 0; i < GetSel().GetSize(); i ++)
	{
		Rec.RemoveAll();
		Rec.Add(GetDefID());
		Rec.Add(GetSel().GetAt(i)->GetAt(0));
		Rec.Add(GetSel().GetAt(i)->GetAt(1));
		GetOraLoader().UpdateRecord(Rec, TRUE);
	}
	GetOraLoader().Commit();

	return TRUE;
}

BOOL CReportData::DelFields()
{
	GetOraLoader().GetSql().Format("DELETE SEMAM.NW_FMS_REPORT_DEF_FIELDS "
									"WHERE DEF_ID = %s", GetDefID());
	return GetOraLoader().ExecuteSql();
}

BOOL CReportData::AddReportDef(LPCTSTR ReportDef)
{
	if(!ReportDef || strlen(ReportDef) == 0)
		return FALSE;

	if(!m_DefID.IsEmpty())
		return UpdateReportDef(ReportDef);

	CDBRec Rec;

	Rec.Add(ReportDef);
	Rec.Add(GetUserIni());
	Rec.Add(GetReportID());
	GetOraLoader().GenerateUniqueID("DEF_ID", m_DefID);
	Rec.Add(GetDefID());
	GetOraLoader().Open("SELECT DEF_NAME, USER_INI, REPORT_ID, DEF_ID "
						"FROM SEMAM.NW_FMS_REPORT_DEF", ODYNASET_DEFAULT);
	if(GetOraLoader().UpdateRecord(Rec, TRUE))
	{
		if(AddFields())
		{
			Rec.RemoveAll();
			Rec.Add(ReportDef);
			Rec.Add(GetDefID());
			GetSel().GetReportArray().Add(Rec);
			return TRUE;
		}
	}

	return FALSE;
}

int CReportData::DelReportDef(LPCTSTR ReportDef)
{
	if(!ReportDef || strlen(ReportDef) == 0)
		return FALSE;

	if(m_DefID.IsEmpty())
		return FALSE;

	DelFields();
	GetOraLoader().GetSql().Format("DELETE SEMAM.NW_FMS_REPORT_DEF WHERE DEF_ID = %s", GetDefID());
	if(GetOraLoader().ExecuteSql())
	{
		int Index;

		Index = GetSel().GetReportArray().Find(GetDefID(), 1);
		if(Index >= 0)
		{
			GetSel().GetReportArray().RemoveAt(Index);
			return TRUE;
		}
	}

	return FALSE;
}

int CReportData::UpdateReportDef(LPCTSTR ReportDef)
{
	if(!ReportDef || strlen(ReportDef) == 0)
		return FALSE;
	
	if(m_DefID.IsEmpty())
		return AddReportDef(ReportDef);
	
	CDBRec Rec;

	GetOraLoader().GetSql().Format("SELECT DEF_NAME, USER_INI, REPORT_ID "
							"FROM SEMAM.NW_FMS_REPORT_DEF WHERE DEF_ID = %s", GetDefID());
	Rec.Add(ReportDef);
	Rec.Add(GetUserIni());
	Rec.Add(GetReportID());

	if(GetOraLoader().Open(GetOraLoader().GetSql(), ODYNASET_DEFAULT))
		return FALSE;

	if(GetOraLoader().UpdateRecord(Rec))
	{
		DelFields();
		if(AddFields())
		{
			int Index;

			Index = GetSel().GetReportArray().Find(GetDefID(), 1);
			if(Index >= 0)
			{
				Rec.RemoveAll();
				Rec.Add(ReportDef);
				Rec.Add(GetDefID());
				GetSel().GetReportArray().SetAt(Index, Rec);
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CReportData::LoadReportSql(CString &Sql, CString &OrderBy, int ReportID, LPCTSTR Date, LPCTSTR PrevDate)
{
	CString MySql, GroupBy;

	switch(ReportID)
	{
		case 1: // NAV_SUMMARY 
			MySql.Format("SELECT PORTFOLIO, ID, CATEGORY, SUM(REALIZED) \"REALIZED\", SUM(UNREALIZED) \"UNREALIZED\", "
						 "SUM(ACCRUAL) \"ACCRUAL\", SUM(LEVERAGE_EXP) \"LEVERAGE_EXP\", SUM(TOTAL) \"TOTAL\" "
						"FROM SEMAM.NW_NAV_SUMMARY WHERE INDATE = %s", Date);
			GroupBy = "GROUP BY PORTFOLIO, ID, CATEGORY ";
			OrderBy = "ORDER BY 1, 2, 3";
			break;
		case 2: // NAV_SUMMARY
			MySql.Format("SELECT PORTFOLIO, ID, ID2, CATEGORY, BUCKET, REALIZED, UNREALIZED, ACCRUAL, LEVERAGE_EXP, "
						 "TOTAL, NAV FROM SEMAM.NW_NAV_SUMMARY "
						 "WHERE INDATE = %s ", Date);
			OrderBy = "ORDER BY 1, 2, 3, 4, 5";
			break;
		case 3: // DAILY_NAV
			MySql.Format("SELECT PORTFOLIO, B.CAT_INDEX \"ID\", C.BUCKET_INDEX \"ID2\", A.CATEGORY, A.BUCKET, COUNTRY, "
						  "ASS_CODE, ASSET, SOURCE, CURRENCY, ORI_AMOUNT, ORI_FCOST, ORI_VALUE, ACCRUAL, LEVERAGE_EXP, "
						  "REAL_PNL, NOM_AMOUNT, FCOST, PRICE, EXRATE, MARKET, MARKET_VALUE, UNREAL_PNL, DAILY_CHANGE "
						  "FROM SEMAM.NW_DAILY_NAV A, SEMAM.NW_INV_STRATEGIES B, SEMAM.NW_BUCKETS C "
						  "WHERE B.STRATEGY(+) = A.CATEGORY "
						  "AND C.BUCKET_DESC(+) = A.BUCKET "
						  "AND INDATE = %s ", Date);
			OrderBy = "ORDER BY 1, 2, 3, 6, 7";
			break;
		case 4: // DAILY_POSITION
			MySql.Format("SELECT PORTFOLIO, COUNTRY, INDUSTRY, ASSET, CURRENCY, COST, FXRATE, "
						"MARKET, EXRATE, NOM_AMOUNT, AMOUNT, PROFIT, LEVERAGE "
						"FROM SEMAM.NW_DAILY_POSITION WHERE INDATE = %s", Date);
			OrderBy = "ORDER BY 1, 2, 3, 4";
			break;
		case 5: // POS_CHANGE
			MySql.Format("SELECT A.PORTFOLIO, A.COUNTRY, SUM(NVL(A.MARKET_VALUE, 0)) \"TODAY_MM\", "
						"SUM(NVL(B.MARKET_VALUE, 0)) \"PRV_DAY_MM\", "
						"SUM(NVL(A.MARKET_VALUE, 0) - NVL(B.MARKET_VALUE, 0)) \"MKT_DIFF\", "
						"SUM(NVL(A.GAIN_LOSS, 0)) \"TODAY_P/L\", SUM(NVL(B.GAIN_LOSS, 0)) \"PRV_DAY_P/L\", "
						"SUM(NVL(A.GAIN_LOSS, 0) - NVL(B.GAIN_LOSS, 0)) \"P/L_DIFF\" "
						"FROM SEMAM.NW_POS_CHANGE A "
						"LEFT OUTER JOIN SEMAM.NW_POS_CHANGE B ON (A.PORTFOLIO = B.PORTFOLIO "
										"AND A.COUNTRY = B.COUNTRY AND B.INDATE = %s) "
						"WHERE A.INDATE = %s GROUP BY A.PORTFOLIO, A.COUNTRY "
						"UNION "
						"SELECT A.PORTFOLIO, A.COUNTRY, 0, SUM(NVL(A.MARKET_VALUE, 0)), "
						"SUM(0 - NVL(A.MARKET_VALUE, 0)), 0, SUM(NVL(A.GAIN_LOSS, 0)), "
						"SUM(0 - NVL(A.GAIN_LOSS, 0)) FROM SEMAM.NW_POS_CHANGE A "
						"WHERE 0 = (SELECT COUNT(*) FROM SEMAM.NW_POS_CHANGE B "
									"WHERE B.COUNTRY = A.COUNTRY AND B.INDATE = %s) "
									"AND A.INDATE = %s GROUP BY A.PORTFOLIO, A.COUNTRY ", 
						PrevDate, Date, Date, PrevDate);
			OrderBy = "ORDER BY 1, 2";
			break;
		case 6: // Asset_alloc
			MySql.Format("SELECT COUNTRY, ASSET_CLASS, ASSET_DESC, SUM(ALPHA_SSF) \"ALPHA-SSF\", "
						"SUM(EXPLORER) \"EXPLORER\", SUM(EMCF) \"EMCF\", SUM(PRU) \"NWI-LYXOR\", "
						"SUM(QEG_NWI) \"LYXOR-EXPG\", SUM(ALPHA_SSF+EXPLORER+EMCF+PRU+QEG_NWI) \"TOTAL\", "
						"ASS_CODE FROM SEMAM.NW_ASSET_ALLOC A, SEMAM.NW_ASSETS B "
						"WHERE B.ASS_DESC = A.ASSET_DESC "
						"WHERE A.INDATE = %s ", Date);
			GroupBy = "GROUP BY COUNTRY, ASSET_CLASS, ASSET_DESC, ASS_CODE HAVING (SUM(ALPHA_SSF) != 0 OR SUM(EXPLORER) != 0 OR SUM(EMCF) != 0 OR SUM(PRU) != 0) ";
			OrderBy = "ORDER BY 1, 2, 3 ";
			break;
		case 7:	// Option Allocation
			MySql.Format("SELECT COUNTRY, ASS_DESC, OPT_TICK, TRANS_TYPE, OPT_EXPIRATION, "
						"EXERCISE_PRICE \"STRIKE\", TRANS_DIRECTION \"DIR\", TRADE_DATE, "
						"COUNTERPARTY, TRANS_NUM, IDX, CLASSA_EXP \"ALPHA-SSF\", EXPLORER, "
						"EMCF, QEG_NWI \"NWI-LYXOR\", QIH_NWI \"LYXOR-EXPG\", TOTAL "
						"FROM SEMAM.NW_OPT_ALLOC WHERE INDATE = %s ", Date);
			OrderBy = "ORDER BY 1, 2, 3, 4, 5, 6 ";
			break;
		case 8:	// Leverage Outstanding
			MySql.Format("SELECT PORTFOLIO, ASSET_CODE, D, DEAL_TYPE, TRANS_NUM, TRANS_TYPE, DIR, "
						"COUNTERPARTY, TRADE_DATE, VALUE_DATE, MATURITY_DATE, TR_RATE, AV_AMOUNT, "
						"NOM_AMOUNT, CURRENCY, AMOUNT, POS FROM SEMAM.NW_DAILY_LEV_OUT "
						"WHERE INDATE = %s ", Date);
			OrderBy = "ORDER BY 1, 3, 17, 6, 5";
			break;
		default:
			break;
	}

	if(MySql.IsEmpty())
		return FALSE;

	Sql += MySql;
	LoadReportSqlConstraint(Sql);
	
	if(!GroupBy.IsEmpty())
		Sql += GroupBy;

	return TRUE;
}

void CReportData::LoadReportSqlConstraint(CString &Sql)
{
	CString MySql, Text;
	
	for(int i = 0; i < GetSel().GetSize() && !m_DefID.IsEmpty(); i++)
	{
		if(Text != GetSel().GetAt(i)->GetAt(0))
		{
			Text = GetSel().GetAt(i)->GetAt(0);
			if(!MySql.IsEmpty())
				MySql += ") ";
			MySql += " AND " + Text + " IN ('";
			MySql += GetSel().GetAt(i)->GetAt(1);
			MySql += QUOTE;
		}
		else
		{
			MySql += COMMA;
			MySql += QUOTE;
			MySql += GetSel().GetAt(i)->GetAt(1);
			MySql += QUOTE;
		}
	}

	if(!MySql.IsEmpty())
	{
		MySql += ") ";
		Sql += MySql;		
	}
}

BOOL CReportData::RunReport(CDBSheet &SS, LPCTSTR Date)
{
	CString Sql, QDate, OrderBy, PrevDate;
	int ReportID;	
	CQData QData;

	QDate = QData.GetQueryDate(Date);
	ReportID = atoi(GetReportID());
	
	GetOraLoader().TMinus1(PrevDate, Date);
	PrevDate = QData.GetQueryDate(PrevDate);
	LoadReportSql(Sql, OrderBy, ReportID, QDate, PrevDate);	

	if(!OrderBy.IsEmpty())
		Sql += OrderBy;

	if(!GetOraLoader().Open(Sql))
		return FALSE;

	COraField *pField;
	for(int i = 0; i < GetOraLoader().GetFieldArray().GetSize(); i ++)
	{
		pField = GetOraLoader().GetFieldArray().GetAt(i);
		if(pField && pField->GetType() == OTYPE_NUMBER)
			pField->SetPrecision(2);
	}

	switch(ReportID)
	{
		case 1: // NAV_SUMMARY
			GetOraLoader().GetFieldArray().GetAt(1)->SetVisible(FALSE);
			break;
		case 2: // NAV_SUMMARY
		case 3: // DAILY_NAV
			GetOraLoader().GetFieldArray().GetAt(1)->SetVisible(FALSE);
			GetOraLoader().GetFieldArray().GetAt(2)->SetVisible(FALSE);
			break;
		case 4: // DAILY_POSITION
		case 5: // POS_CHANGE
		case 6: // Asset_alloc
			break;
		case 7: // OptDistribution
			GetOraLoader().GetFieldArray().GetAt(9)->SetVisible(FALSE);
			break;
		case 8: // Leverage outstanding
			GetOraLoader().GetFieldArray().GetAt(2)->SetVisible(FALSE);
			break;
		default:
			break;
	}

	return GetOraLoader().LoadDBSheet(SS, TRUE) > 0 ? TRUE : FALSE;	
}
