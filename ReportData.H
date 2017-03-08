#ifndef __REPORTDATA_H__
#define __REPORTDATA_H__

#ifndef __DBREC_H__
#include "dbrec.h"
#endif

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

class CReportBaseData : public CObArray
{
DECLARE_DYNAMIC(CReportBaseData);

protected:
	CDBRecArray m_ReportArray;

public:
	CReportBaseData() {}
	~CReportBaseData() { RemoveAll(); }

	CDBRecArray &GetReportArray() { return m_ReportArray; }

	int	Add(CDBRec *Rec);
	void SetAt(int Index, CDBRec *Rec);
	void InsertAt(int Index, CDBRec *Rec);
	int Add(CDBRec &Rec);
	void SetAt(int Index, CDBRec &Rec);
	void InsertAt(int Index, CDBRec &Rec);
	void InsertAt(CDBRec &Rec);
	void RemoveAt(int Index);
	void RemoveAt(CDBRec &Rec);
	void RemoveAll();
	CDBRec *GetAt(int Index);
	int FindListed(int Index);
	CDBRec *GetListedAt(int Index);
    
	int Find(LPCTSTR Key, int FieldID = -1);
	
	int CopyField(CHLBox *FieldBox);
	int CopyFieldValue(CHLBox *ValueBox, LPCTSTR FieldName);
};

class CReportData : public CObject
{
DECLARE_DYNAMIC(CReportData);
// Construction
protected:
	COraLoader m_OraLoader;
	CReportBaseData m_Avail;
	CReportBaseData m_Sel;
	CString m_DefID;
	CString m_ReportID;
	CString m_UserIni;
	CString m_TableName;
	BOOL m_bUnion;

protected:
	BOOL AddFields();
	BOOL DelFields();
	BOOL LoadReportSql(CString &Sql, CString &OrderBy, int ReportID, LPCTSTR Date, LPCTSTR PrevDate);
	void LoadReportSqlConstraint(CString &Sql);

public:
	CReportData() {};
	CReportData(COraLoader &OraLoader, LPCTSTR UserIni) { SetOraLoader(OraLoader); SetUserIni(UserIni); }

	CReportBaseData &GetAvail() { return m_Avail; }
	CReportBaseData &GetSel() { return m_Sel; }

	COraLoader &GetOraLoader() { return m_OraLoader; }
	void SetOraLoader(COraLoader &OraLoader) { m_OraLoader = OraLoader; }

	LPCTSTR GetUserIni() { return (LPCTSTR) m_UserIni; }
	void SetUserIni(LPCTSTR UserIni) { m_UserIni = UserIni; }

	LPCTSTR GetDefID() { return (LPCTSTR) m_DefID; }
	void SetDefID(LPCTSTR DefID) { m_DefID = DefID; }
	LPCTSTR GetReportID() { return (LPCTSTR) m_ReportID; }
	void SetReportID(LPCTSTR ReportID) { m_ReportID = ReportID; }
	LPCTSTR GetTableName() { return (LPCTSTR) m_TableName; }
	void SetTableName(LPCTSTR TableName) { m_TableName = TableName; }
	BOOL GetWithUnion() { return m_bUnion; }
	void SetWithUnion(LPCTSTR WithUnion) { m_bUnion = strcmp(WithUnion, "Y") == 0 ? TRUE : FALSE; }
	void SetWithUnion(BOOL bWithUnion) { m_bUnion = bWithUnion; }

	int LoadDate(COptComboBox *ComboBox);
	int LoadReportArray();
	int LoadFieldArray();

	int LoadReportDefArray();
	int LoadSelFieldArray();

	BOOL AddReportDef(LPCTSTR ReportDef);
	BOOL UpdateReportDef(LPCTSTR ReportDef);
	BOOL DelReportDef(LPCTSTR ReportDef);
	BOOL RunReport(CDBSheet &SS, LPCTSTR Date);
};
#endif // __REPORTDATA_H__
