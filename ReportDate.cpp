// ReportDate.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "nwivar.h"
#include "ReportDate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int DATE_TODAY = 1;
const int DATE_MONTH_START = 2;
const int DATE_PREV_MONTH_END = 3;
const int DATE_FUND_START = 4;
const int DATE_PREV_YEAR_END = 5;


IMPLEMENT_DYNAMIC(CReportDateItem, CObject)
CReportDateItem::CReportDateItem(CReportDateItem &Item)
{
	SetName(Item.GetName());
	SetLongName(Item.GetLongName());
	SetDate(Item.GetDate());
	SetDateMethod(Item.GetDateMethod());
}

CReportDateItem::CReportDateItem(LPCTSTR Name, LPCTSTR LongName, LPCTSTR Date, int DateMethod)
{
	SetName(Name);
	SetLongName(LongName);
	SetDate(Date);
	SetDateMethod(DateMethod);
}

IMPLEMENT_DYNAMIC(CReportDateItemArray, CObArray)
void CReportDateItemArray::SetAt(int Index, CReportDateItem *pItem)
{
	CReportDateItem *pOriItem;
	
	pOriItem = GetAt(Index);
	if(pOriItem)
	{
		delete pOriItem;
		CObArray::SetAt(Index, pItem);
	}
}

void CReportDateItemArray::SetAt(int Index, CReportDateItem &Item)
{
	CReportDateItem *pOriItem;
	
	pOriItem = GetAt(Index);
	if(pOriItem)
	{
		delete pOriItem;
		CObArray::SetAt(Index, new CReportDateItem(Item));
	}
}

void CReportDateItemArray::RemoveAt(int Index)
{
	CReportDateItem *pItem = GetAt(Index);
	if(pItem)
	{
		CObArray::RemoveAt(Index);
		delete pItem;		
	}
}

void CReportDateItemArray::RemoveAll()
{
	CReportDateItem *pItem;
	
	for(int i = 0; i < GetSize(); i++)
	{
		pItem = GetAt(i);
		if(pItem)
			delete pItem;
    }
    
    CObArray::RemoveAll();
}

CReportDateItem *CReportDateItemArray::GetAt(int Index)
{
	if(Index < 0 || Index > GetSize())
		return NULL;
	return (CReportDateItem*) CObArray::GetAt(Index);
}


/////////////////////////////////////////////////////////////////////////////
// CReportDate dialog

CReportDate::CReportDate(CWnd* pParent /*=NULL*/)
	: CDialog(CReportDate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportDate)
	//}}AFX_DATA_INIT
}

void CReportDate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportDate)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CReportDate, CDialog)
	//{{AFX_MSG_MAP(CReportDate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CReportDate::AddDateItem(CString Flag, LPCTSTR Name, LPCTSTR LongName, int DateMethod)
{
	if(Flag != Y || !Name || strlen(Name) == 0 || !LongName || strlen(LongName) == 0)
		return;

	CReportDateItem Item;

	Item.SetName(Name);
	Item.SetLongName(LongName);
	Item.SetDateMethod(DateMethod);
	m_DateArray.Add(Item);
}

int CReportDate::GetLastDay(int Month, int Year)
{
	int Days;
	switch(Month)
	{
		case 4:
		case 6:
		case 9:
		case 11:
			Days = 30;
			break;
		case 2:
			Days = 28;
			Days += (Year % 400 == 0 || (Year % 4 == 0 && Year % 100 != 0)) ? 1 : 0;
			break;
		default:
			Days = 31;
			break;
	}

	return Days;
}

void CReportDate::ConvertDate(CString &Date)
{
	char Months[13][4] = {"", "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL", "AUG", "SEP",
							"OCT", "NOV", "DEC"};
	CString SrcDate;
	int Day, Month, Year;

	SrcDate = Date;
	Month = atoi(SrcDate);
	Day = atoi(((LPCTSTR) SrcDate) + 3);
	Year = atoi(((LPCTSTR) SrcDate) + 6);

	Date.Format("'%02d-%s-%02d'", Day, Months[Month], Year%100);
}

void CReportDate::GetDateInfo(CReportDateItem *pItem)
{
	pItem->SetDate(EMPTYSTRING);
	if(pItem->GetName().IsEmpty())
		return;

	int Day, Month, Year;

	Day = m_nDay;
	Month = m_nMonth;
	Year = m_nYear;
	switch(pItem->GetDateMethod())
	{
		case DATE_MONTH_START:
			Day = 1;
			break;
		case DATE_PREV_MONTH_END: 
			Month -= 1;
			if(Month <= 0)
			{
				Month = 12;
				Year -= 1;
			}

			if(Year < 0)
				Year = 99;
			Day = GetLastDay(Month, Year);
			break;
		case DATE_FUND_START:
			Day = 1;
			Month = 10;
			Year = 1993;
			break;
		case DATE_PREV_YEAR_END:
			Day = 31;
			Month = 12;
			Year -= 1;
			break;
		default: // DATE_TODAY
			break;
	}
	
	pItem->GetDate().Format("%02d/%02d/%02d", Month, Day, Year);
}
/////////////////////////////////////////////////////////////////////////////
// CReportDate message handlers

BOOL CReportDate::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Convert Today into interger for easy 
	m_nMonth = atoi(m_Today);
	m_nDay = atoi(((LPCTSTR) m_Today) + 3);
	m_nYear = atoi(((LPCTSTR) m_Today) + 6);

	GetDlgItem(IDC_REPORTDATE_REPORT_STATIC)->SetWindowText(m_Report);
	
	CReportDateItem *pItem;
	int i;

	if(m_DateArray.GetSize() == 0)
		return FALSE;
	
	for(i = 0; i < m_DateArray.GetSize(); i++)
	{
		pItem = m_DateArray.GetAt(i);
		GetDateInfo(pItem);
		if(i < 3)
		{
			GetDlgItem(IDC_REPORTDATE_DATE1_STATIC + i)->SetWindowText(pItem->GetLongName());
			GetDlgItem(IDC_REPORTDATE_DATE1_EDIT + i)->SetWindowText(pItem->GetDate());
		}
	}
	
	for(i = m_DateArray.GetSize(); i < 3; i++)
		GetDlgItem(IDC_REPORTDATE_DATE1_EDIT + i)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReportDate::OnOK() 
{
	CString Txt;
	CReportDateItem *pItem;

	for(int i = 0; i < m_DateArray.GetSize(); i++)
	{
		pItem = m_DateArray.GetAt(i);
		if(i < 3)
		{
			GetDlgItem(IDC_REPORTDATE_DATE1_EDIT + i)->GetWindowText(pItem->GetDate());
			ConvertDate(pItem->GetDate());
			if(!pItem->GetLongName().IsEmpty() && pItem->GetDate().IsEmpty())
				Txt = pItem->GetLongName() + " Is Not Available";
		}
	}

	if(Txt.IsEmpty())
		CDialog::OnOK();
	else
		MessageBox(Txt);	
}
