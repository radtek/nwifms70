#include "stdafx.h"
#include <math.h>
#include "resource.h"
#include "assetratedata.h"
#include "nwifms70.h"
#include "disperr.h"
#include "qdata.h"
#include "sqlstmt.h"
#include "nwivar.h"


CAssetRateData::CAssetRateData()
{
	m_OraLoader.SetDB(&theDB);
}

CAssetRateData::CAssetRateData(LPCTSTR FName, int DataRange)
{
	m_OraLoader.SetDB(&theDB);
	Setup(FName, DataRange);
}

BOOL CAssetRateData::Setup(LPCTSTR FName, int DataRange)
{
	char Text[1024];
	CFileException e;

	SetDataRange(DataRange);
	if(!GetInFile().Open(FName, CFile::modeRead, &e))
	{
		e.GetErrorMessage(Text, sizeof(Text) - 1);
		MessageBox(NULL, Text, "File could not be opened ", MB_OK);
		return FALSE;
	}

	return TRUE;
}

void CAssetRateData::DisplayErrorFile()
{
	CDisplayErrorFile ErrDlg;

	ErrDlg.m_ErrF.Open(GetErrorFile().GetFileName(), CFile::modeRead);
	if(ErrDlg.m_ErrF.GetLength() == 0)
		ErrDlg.m_ErrF.Close();
    else
    {
		ErrDlg.m_Title = "Display Error";	
		ErrDlg.DoModal();		
	}
}

void CAssetRateData::WriteError(LPCTSTR Text)
{
	CString Err;

	Err = Text;
	Err += " does not exist\n";
	GetErrorFile().WriteString(Err);
}

void CAssetRateData::ProcessData()
{
    char Buffer[1024];
	CString	Data, ErrFName, Price, Asset;
	CFileException e;
	int Pos;

	ErrFName = GetInFile().GetFileName();
	Pos = ErrFName.Find(PERIOD);
	if(Pos >= 0)
		ErrFName = ErrFName.Left(Pos);
	
	ErrFName += ".ERR";
	if(!GetErrorFile().Open(ErrFName, CFile::modeCreate | CFile::modeWrite, &e))
	{
		e.GetErrorMessage(Buffer, sizeof(Buffer) - 1);
		MessageBox(NULL, Buffer, "File could not be opened ", MB_OK);
		return;
	}
	
	GetInFile().ReadString(Buffer, sizeof(Buffer));
	GetInFile().ReadString(Buffer, sizeof(Buffer));

	while(GetInFile().ReadString(Data))
	{
		ParseAssetRateData(Data);
		SaveAssetRateData();
	}

	GetErrorFile().Close();
	GetInFile().Close();

	DisplayErrorFile();
}

void CAssetRateData::ParseAssetRateData(const CString Data)
{
	int Pos, i = 0;
	CString Buf, Text;

	m_Asset.Empty();
	m_FromDate.Empty();
	m_ToDate.Empty();
	m_Rate.Empty();
	m_Amort.Empty();
	m_CapRate.Empty();
	m_SetDate.Empty();

	Buf = Data;
	Pos = Buf.Find(",");
	while(Pos >= 0)
	{
		Text = Buf.Left(Pos);
		Buf = Buf.Right(Buf.GetLength() - Pos - 1);
		switch(i)
		{
			case 0: // Asset
				m_Asset = Text;
				m_Asset.Trim();
				break;
			case 1: // FromDate
				m_FromDate = Text;
				break;
			case 2: // ToDate
				m_ToDate = Text;
				break;
			case 3: // Rate
				m_Rate = Text;
				break;
			case 4:
				m_Amort = Text;
				break;
			case 5:
				m_CapRate = Text;
				m_SetDate = Buf;
				return;
			default:
				break;
		}

		i++;
		Pos = Buf.Find(",");
	}
}

BOOL CAssetRateData::IsValid()
{
	if(m_Asset.IsEmpty() || m_FromDate.IsEmpty() || m_ToDate.IsEmpty() || 
		m_SetDate.IsEmpty() || m_Rate.IsEmpty())
		return FALSE;

	COraLoader OraLoader;
	CQData QData;
	CString Asset;

	if(atof(QData.RemoveComma(m_Rate)) < 0)
		return FALSE;

	OraLoader = GetOraLoader();
	
	// Whether asset exist
	Asset = QData.GetQueryText(m_Asset);
	OraLoader.Open("SELECT COUNT(*) FROM SEMAM.NW_ASSETS WHERE ASS_CODE = " + Asset);
	if(OraLoader.GetCount() <= 0)
	{
		WriteError(m_Asset);
		return FALSE;
	}

	OraLoader.Open("SELECT COUNT(*) FROM SEMAM.NW_ASS_PERIODS WHERE ASS_CODE = " + Asset + 
					" AND ASS_FROM = " + QData.GetQueryDate(m_FromDate));

	m_bNew = OraLoader.GetCount() <= 0 ? TRUE : FALSE;
	return TRUE;
}

void CAssetRateData::ToDBRec(CDBRec &Rec)
{
	CQData QData;

	Rec.RemoveAll();
	Rec.Add(m_Asset);
	Rec.Add(m_FromDate);
	Rec.Add(m_ToDate);
	Rec.Add(QData.RemoveComma(m_Rate));
	Rec.Add(QData.RemoveComma(m_Amort));
	Rec.Add(QData.RemoveComma(m_CapRate));
	Rec.Add(m_SetDate);
}

BOOL CAssetRateData::SaveAssetRateData()
{
	if(!IsValid())
		return FALSE;

	CDBRec Rec;
	CQData QData;
	CString Asset, FromDate;

	ToDBRec(Rec);
	if(IsNew())
	{
		GetOraLoader().Open("SELECT ASS_CODE, ASS_FROM, ASS_TO, RATE, AMORT_FACT, CAPIT_RATE, "
							"INT_SET_DATE FROM SEMAM.NW_ASS_PERIODS ", ODYNASET_DEFAULT);
		return GetOraLoader().UpdateRecord(Rec, TRUE);
	}
	else
	{
		Asset = QData.GetQueryText(m_Asset);
		FromDate = QData.GetQueryDate(m_FromDate);

		GetOraLoader().Open("SELECT ASS_CODE, ASS_FROM, ASS_TO, RATE, AMORT_FACT, CAPIT_RATE, "
							"INT_SET_DATE FROM SEMAM.NW_ASS_PERIODS WHERE ASS_CODE = " + Asset + 
							" AND ASS_FROM = " + FromDate, ODYNASET_DEFAULT);
		return GetOraLoader().UpdateRecord(Rec);
	}
}

