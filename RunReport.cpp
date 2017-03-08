#include "stdafx.h"
#include <process.h>
#include "NWIFMS70.h"
#include "NWIVAR.h"
#include "runreport.h"

const int SQL_REPORT_TYPE = 1;
const int PRC_REPORT_TYPE = 2;

void CRunReport::Setup(int Cmd, LPCTSTR RunFile, LPCTSTR PrintFile, LPCTSTR PrintDef)
{
	CString DataBuf;
	char RunSqlFile[500], Path[500];
	CNWIFMS70App *pTheApp;
	CFileException e;
	
	m_nCmd = Cmd;	
	m_RunFile = RunFile;
	m_PrintFile = PrintFile;
	m_PrintDef = PrintDef;

	if(!m_PrintFile.IsEmpty())
	{
		m_RunSqlFile.Open(m_PrintFile, CFile::modeCreate | CFile::modeWrite);
		m_RunSqlFile.Close();
		m_RunSqlFile.Remove(m_PrintFile);
	}

	RunSqlFile[0] = 0;

	GetEnvironmentVariable("FMS_DATA", Path, sizeof(Path));
	DataBuf.Format("%d", SetCurrentDirectory(Path));
	sprintf_s(RunSqlFile, "%s\\RUNSQL.SQL", Path);

	if(!m_RunSqlFile.Open(RunSqlFile, CFile::modeCreate | CFile::modeWrite, &e))
	{
		TCHAR szError[1024];
		e.GetErrorMessage(szError, 1024);
		MessageBox(NULL, szError, NULL, MB_OK);
		return;
	}

	m_Path = "%FMS_HOME%\\";

	pTheApp = (CNWIFMS70App *) AfxGetApp();
	DataBuf.Format("DEFINE UNAME = %s\n", pTheApp->GetUser());
	m_RunSqlFile.WriteString(DataBuf);
	DataBuf.Format("DEFINE PWORD = %s\n", pTheApp->GetPassword());
	m_RunSqlFile.WriteString(DataBuf);
	DataBuf.Format("DEFINE HOST = %s\n", pTheApp->GetHost());
	m_RunSqlFile.WriteString(DataBuf);

	if(m_nCmd == PRC_REPORT_TYPE)
	{
		DataBuf.Format("HO %s%s &&UNAME &&PWORD &&HOST\n", m_Path, m_RunFile);
		m_RunSqlFile.WriteString(DataBuf);
	}
}

void CRunReport::SetDate(LPCTSTR DateHeader, LPCTSTR Date)
{
	if(m_nCmd == SQL_REPORT_TYPE)
	{
		CString Buf;
		Buf.Format("DEFINE %s = %s\n", DateHeader, Date);
		m_RunSqlFile.WriteString(Buf);
	}
}

void CRunReport::PrintReport()
{
	if(!m_PrintFile.IsEmpty())
	{
		CString Cmd;

		Cmd.Format("HO %%FMS_HOME%%\\NPRINT.EXE %%FMS_DATA%%\\%s %s\n", m_PrintFile, m_PrintDef);
		m_RunSqlFile.WriteString(Cmd);
	}
}

BOOL CRunReport::RunReport()
{
	if(m_RunFile.IsEmpty() ||!m_RunSqlFile.m_pStream)
		return FALSE;

	char Path[500];
	CNWIFMS70App *pTheApp;
	CString Cmd, Login;
	if(m_nCmd == SQL_REPORT_TYPE)
	{
		Cmd.Format("@%s%s\n", m_Path, m_RunFile);
		m_RunSqlFile.WriteString(Cmd);
	}

	PrintReport();
	m_RunSqlFile.WriteString("EXIT\n");
	m_RunSqlFile.Close();

	int Status;
	char PlusCmd[200];

	Cmd = "@" + m_RunSqlFile.GetFilePath();
	pTheApp = (CNWIFMS70App *) AfxGetApp();
	Login.Format("%s/%s@%s", pTheApp->GetUser(), pTheApp->GetPassword(), pTheApp->GetHost(), Cmd);
	GetEnvironmentVariable("ORACLE_HOME", Path, sizeof(Path));
	sprintf_s(PlusCmd, "%s\\bin\\sqlplus.exe", Path);
	Status = _spawnl(_P_WAIT, PlusCmd, PlusCmd, Login, Cmd, NULL);

	if(Status == 0)
		m_RunSqlFile.Remove(m_RunSqlFile.GetFilePath());
	else		
	{
		sprintf_s(PlusCmd, "Error When Run Report: %d %d", Status, errno);
		MessageBox(NULL, PlusCmd, m_RunFile, MB_OK);
		return FALSE;
	}

	return TRUE;
}
