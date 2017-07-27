
// NWIFMS70Doc.cpp : implementation of the CNWIFMS70Doc class
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "equity.h"
#include "NWIFMS70Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNWIFMS70Doc

IMPLEMENT_DYNCREATE(CNWIFMS70Doc, CDocument)

BEGIN_MESSAGE_MAP(CNWIFMS70Doc, CDocument)
END_MESSAGE_MAP()


// CNWIFMS70Doc construction/destruction

CNWIFMS70Doc::CNWIFMS70Doc()
{

}

CNWIFMS70Doc::~CNWIFMS70Doc()
{
}

void CNWIFMS70Doc::SetViewTitle(LPCTSTR Title)
{
	CString T;

	T = m_Title;
	if(Title && strlen(Title) > 0)
	{
		T += " - ";
		T += Title;
	}
	
	SetTitle(T);
}

BOOL CNWIFMS70Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	CEquity Eq;

	GetData().GetOraLoader().SetDB(&theDB);
	GetData().GetOraLoader().SetCase(CASESENSITIVE);
	GetData().GetOraLoader().SetSql("ALTER SESSION SET NLS_DATE_FORMAT = 'MM/DD/YYYY'");
	GetData().GetOraLoader().ExecuteSql();

	GetData().LoadFees();
	Eq.m_SecFeeRate = GetData().GetSecFee();
	GetData().LoadOrFeeArray(m_OrFeeDataArray);
	GetData().PasswordExpiryWarning();

	GetData().LoadUser();

	if(strlen(m_Data.GetUser()) > 0)
	{
		SetTitle(m_Data.GetUser());
		m_Title = GetTitle();
		m_Title += " : ";
		m_Title += ((CNWIFMS70App *) AfxGetApp())->GetHost();
		m_Title.MakeUpper();
		SetViewTitle("");
	}

	return TRUE;
}




// CNWIFMS70Doc serialization

void CNWIFMS70Doc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CNWIFMS70Doc diagnostics

#ifdef _DEBUG
void CNWIFMS70Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CNWIFMS70Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CNWIFMS70Doc commands
