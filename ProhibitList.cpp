// ProhibitList.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "ProhibitList.h"


// CProhibitList dialog

IMPLEMENT_DYNAMIC(CProhibitList, CDialog)

CProhibitList::CProhibitList(CWnd* pParent /*=NULL*/)
	: CDialog(CProhibitList::IDD, pParent)
{

}

CProhibitList::~CProhibitList()
{
}

void CProhibitList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CProhibitList, CDialog)
END_MESSAGE_MAP()


// CProhibitList message handlers

BOOL CProhibitList::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString Text, Data;
	int idx;

	m_OraLoader.Open("SELECT ITEM, ITEM_DESC FROM SEMAM.NW_PROHIBIT_LIST ORDER BY 1 ");
	m_OraLoader.MoveFirst();
	while(!m_OraLoader.IsEOF())
	{
		if(Text.IsEmpty())
			Text = m_OraLoader.GetDBString(0);
		else
			Text += m_OraLoader.GetDBString(0);

		Text += ".";
		
		idx = 0;
		
		Data = m_OraLoader.GetDBString(1);
		while(Data.GetLength() > 100)
		{
			if(idx > 0)
				Text += "     ";
			Text += Data.Left(100);
			Text += "\r\n";
			Data = Data.Right(Data.GetLength() - 100);
			idx ++;
		}

		if(Data.GetLength() > 0)
		{
			if(idx > 0)
				Text += "     ";
			Text += Data;
			Text += "\r\n";
		}
		
		m_OraLoader.MoveNext();
	}

	m_ProhibitList.SubclassDlgItem(IDC_PROHIBITLIST_EDIT, this);
	m_ProhibitList.SetWindowText(Text);

	return TRUE;
}