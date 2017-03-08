#ifndef __RUNREPORT_H__
#define __RUNREPORT_H__

extern const int SQL_REPORT_TYPE;
extern const int PRC_REPORT_TYPE;

class CRunReport
{
private:
	int m_nCmd;
	CString m_RunFile;
	CString m_PrintFile;
	CString m_PrintDef;
	CString m_Path;
	CStdioFile m_RunSqlFile;

public:
	CRunReport() { }
	CRunReport(int Cmd, LPCTSTR RunFile, LPCTSTR PrintFile, LPCTSTR PrintDef = "DEF") 
				{ Setup(Cmd, RunFile, PrintFile, PrintDef); }
	CRunReport(LPCTSTR Cmd, LPCTSTR RunFile, LPCTSTR PrintFile, LPCTSTR PrintDef = "DEF") 
			{ Setup(Cmd, RunFile, PrintFile, PrintDef); }
	~CRunReport() {};

protected:
	void PrintReport();

public:
	void Setup(int Cmd, LPCTSTR RunFile, LPCTSTR PrintFile, LPCTSTR PrintDef = "DEF");
	void Setup(LPCTSTR Cmd, LPCTSTR RunFile, LPCTSTR PrintFile, LPCTSTR PrintDef = "DEF")
			{ Setup(atoi(Cmd), RunFile, PrintFile, PrintDef); }

	void SetDate(LPCTSTR DateHeader, LPCTSTR Date);
	BOOL RunReport();
};
#endif
