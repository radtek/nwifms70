// Country.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "giveupfeedlg.h"
#include "qdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGiveupFeeDlg dialog


CGiveupFeeDlg::CGiveupFeeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGiveupFeeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGiveupFeeDlg)
	//}}AFX_DATA_INIT
}


CGiveupFeeDlg::~CGiveupFeeDlg()
{
}

void CGiveupFeeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGiveupFeeDlg)
	DDX_Control(pDX, IDC_GIVEUP_LIST, m_SS);
	//}}AFX_DATA_MAP
}


void CGiveupFeeDlg::EnableCtrls()
{
	GetDlgItem(IDC_GIVEUP_UPDATE_BUTTON)->EnableWindow(m_SS.GetSheetCurRow() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_GIVEUP_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected());
}

BOOL CGiveupFeeDlg::IsOK()
{
	CString Text;

	if(m_PB.GetData().IsEmpty())
		Text = "Prime Broker is not selected";
	if(m_FromDate.GetData().IsEmpty())
		Text = "From Date did not enter";
	if(m_Cost.GetData().IsEmpty())
		Text = "Unit Cost did not enter";

	if(Text.IsEmpty())
		return TRUE;

	MessageBox(Text, "Giveup Fee");
	return FALSE;
}

BOOL CGiveupFeeDlg::UpdateData(BOOL bSaveAndValid)
{
	COraLoader OraLoader;
	CQData QData;
	CString PB, FromDate;

	OraLoader.SetDB(&theDB);

	if(bSaveAndValid)
	{
		CDBRec Rec;

		PB = QData.GetQueryText(m_PB.GetData());
		FromDate = QData.GetQueryText(m_FromDate.GetData());
		
		Rec.Add(m_PB.GetData());
		Rec.Add(m_FromDate.GetData());
		Rec.Add(m_ToDate.GetData());
		Rec.Add(m_Cost.GetData());
		Rec.Add(m_Cost2.GetData());
		Rec.Add(m_CP.GetData());
		Rec.Add(m_ExoticOptType.GetData());

		if(m_bAddNew)
		{
			if(OraLoader.Open("SELECT PB_NAME, FROM_DATE, TO_DATE, COST, COST2, COUNTERPARTY, EXOTIC_TYPE FROM SEMAM.NW_GIVEUP_UNIT_COST ", ODYNASET_DEFAULT))
			{
				OraLoader.UpdateRecord(Rec, TRUE);
				m_SS.AddSheetRow(Rec);
				return TRUE;
			}
		}
		else
		{
			if(OraLoader.Open("SELECT PB_NAME, FROM_DATE, TO_DATE, COST, COST2, COUNTERPARTY, EXOTIC_TYPE  FROM SEMAM.NW_GIVEUP_UNIT_COST "
								"WHERE PB_NAME = " + PB + " AND FROM_DATE = " + FromDate, ODYNASET_DEFAULT))
			{
				OraLoader.UpdateRecord(Rec);
				m_SS.UpdateSheetRow(m_SS.GetSheetCurRow(), Rec);
				return TRUE;
			}
		}

		return FALSE;
	}
	else
	{
		if(m_PB.GetCount() <= 0)
		{
			m_pData->LoadSupportData();

			m_pData->GetContactList().CopyKeyToComboBox(m_PB);
		}

		if(OraLoader.Open("SELECT PB_NAME, FROM_DATE, TO_DATE, COST, COST2, COUNTERPARTY, EXOTIC_TYPE FROM SEMAM.NW_GIVEUP_UNIT_COST ORDER BY 1, 2 "))
			return OraLoader.LoadDBSheet(m_SS);

		return FALSE;
	}
}

BEGIN_MESSAGE_MAP(CGiveupFeeDlg, CDialog)
	//{{AFX_MSG_MAP(CGiveupFeeDlg)
	ON_BN_CLICKED(IDC_GIVEUP_ADD_BUTTON, OnGiveupAddButton)
	ON_BN_CLICKED(IDC_GIVEUP_CLEAR_BUTTON, OnGiveupClearButton)
	ON_BN_CLICKED(IDC_GIVEUP_DELETE_BUTTON, OnGiveupDeleteButton)
	ON_BN_CLICKED(IDC_GIVEUP_FIND_BUTTON, OnGiveupFindButton)
	ON_BN_CLICKED(IDC_GIVEUP_UPDATE_BUTTON, OnGiveupUpdateButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGiveupFeeDlg message handlers

BEGIN_EVENTSINK_MAP(CGiveupFeeDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CGiveupFeeDlg)
	ON_EVENT(CGiveupFeeDlg, IDC_GIVEUP_LIST, 2 /* BlockSelected */, OnBlockSelectedGiveupList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CGiveupFeeDlg, IDC_GIVEUP_LIST, 11 /* DblClick */, OnDblClickGiveupList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CGiveupFeeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	BeginWaitCursor();

	m_SS.SetVisibleRows(14);
	m_SS.SetVisibleCols(8);

	m_PB.Setup(this, IDC_GIVEUP_PB_COMBO);
	m_FromDate.Setup(this, IDC_GIVEUP_FROM_EDIT);
	m_ToDate.Setup(this, IDC_GIVEUP_TO_EDIT);
	m_Cost.Setup(this, IDC_GIVEUP_COST_EDIT);
	m_Cost2.Setup(this, IDC_GIVEUP_COST2_EDIT);
	m_CP.Setup(this, IDC_GIVEUP_CP_EDIT);
	m_ExoticOptType.Setup(this, IDC_GIVEUP_EXOTIC_COMBO, TRUE);
	m_ExoticOptType.AddString(NONE);
	m_ExoticOptType.AddString("KI");
	m_ExoticOptType.AddString("KO");
	m_ExoticOptType.AddString("DIGITAL");

	this->SetWindowText("Giveup Fee");
	OnGiveupFindButton();
	EndWaitCursor();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGiveupFeeDlg::OnGiveupAddButton() 
{
	m_bAddNew = TRUE;
	UpdateData(TRUE);
}

void CGiveupFeeDlg::OnGiveupClearButton() 
{
	UpdateData(TRUE);
	EnableCtrls();
	GetDlgItem(IDC_GIVEUP_ADD_BUTTON)->EnableWindow(FALSE);
}

void CGiveupFeeDlg::OnGiveupDeleteButton() 
{
	COraLoader OraLoader;
	CString PB, FromDate;
	CQData QData;

	if(m_PB.GetData().IsEmpty() || m_FromDate.GetData().IsEmpty())
		return;

	OraLoader.SetDB(&theDB);

	PB = QData.GetQueryText(m_PB.GetData());
	FromDate = QData.GetQueryText(m_FromDate.GetData());
	if(OraLoader.ExecuteSql("DELETE FROM SEMAM.NW_GIVEUP_UNIT_COST WHERE PB_NAME = " + PB + " AND FROM_DATE = " + FromDate))
		m_SS.DeleteSheetRow(m_SS.GetSheetCurRow());

}

void CGiveupFeeDlg::OnGiveupFindButton() 
{
	m_bAddNew = FALSE;
	UpdateData(FALSE);
}

void CGiveupFeeDlg::OnGiveupUpdateButton() 
{
	UpdateData(TRUE);
}

void CGiveupFeeDlg::OnBlockSelectedGiveupList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickGiveupList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
}

void CGiveupFeeDlg::OnDblClickGiveupList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		m_SS.SetSheetCurRow(Row);

		m_PB.SetData(m_SS.GetSheetText(1, Row));
		m_FromDate.SetData(m_SS.GetSheetText(2, Row));
		m_ToDate.SetData(m_SS.GetSheetText(3, Row));
		m_Cost.SetData(m_SS.GetSheetText(4, Row));
		m_Cost2.SetData(m_SS.GetSheetText(5, Row));
		m_CP.SetData(m_SS.GetSheetText(6, Row));
		m_ExoticOptType.SetData(m_SS.GetSheetText(7, Row));
	}
	else
	{
		m_SS.SetSheetCurRow(-1);
		m_PB.SetData(EMPTYSTRING);
		m_FromDate.SetData(EMPTYSTRING);
		m_ToDate.SetData(EMPTYSTRING);
		m_Cost.SetData(EMPTYSTRING);
		m_Cost2.SetData(EMPTYSTRING);
		m_CP.SetData(EMPTYSTRING);
		m_ExoticOptType.SetData(EMPTYSTRING);
	}

	EnableCtrls();
}
