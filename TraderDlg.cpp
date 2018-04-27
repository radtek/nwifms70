// TraderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "TraderDlg.h"
#include "afxdialogex.h"


// CTraderDlg dialog

IMPLEMENT_DYNAMIC(CTraderDlg, CDialog)

CTraderDlg::CTraderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTraderDlg::IDD, pParent)
{

}

CTraderDlg::~CTraderDlg()
{
}

void CTraderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRADER_LIST, m_SS);
}

void CTraderDlg::GetData(CDBRec &Rec)
{
	Rec.RemoveAll();

	Rec.Add(m_TraderIniEdit.GetData());
	Rec.Add(m_TraderNameEdit.GetData());
	Rec.Add(m_FMSNameEdit.GetData());
	Rec.Add(m_Active.GetData());
	Rec.Add(m_PowerUser.GetData());
	Rec.Add(m_RiskManager.GetData());
}


BOOL CTraderDlg::IsOK(BOOL bNew)
{
	CString Text;

	if(m_TraderIniEdit.GetData().IsEmpty())
		Text = "Trader Initial cannot be black";
	else
		if(bNew)
		{
			if(GetOraLoader().GetCount("SELECT COUNT(*) FROM SEMAM.NW_TRADERS WHERE TRADER_INI = '" + m_TraderIniEdit.GetData() + "' ") > 0)
				Text = "Trader Initial has been used";
		}
	
	if(m_TraderNameEdit.GetData().IsEmpty())
		Text = "Trader Name cannot be black";
	
	if(m_FMSNameEdit.GetData().IsEmpty())
		Text = "FMS UserName cannot be black";

	if(Text.IsEmpty())
		return TRUE;
	else
	{
		MessageBox(Text, "Traders");
		return FALSE;
	}
}


int CTraderDlg::LoadTraders()
{
	if(GetOraLoader().Open("SELECT TRADER_INI, TRADER, USER_NAME, ACTIVE, SUPERVISOR, RISK_MAN "
							"FROM SEMAM.NW_TRADERS "
							"ORDER BY 4 DESC, 2 "))
		GetOraLoader().LoadDBSheet(m_SS, TRUE);

	return m_SS.GetSheetRows();
}


BEGIN_MESSAGE_MAP(CTraderDlg, CDialog)
	ON_BN_CLICKED(IDC_TRADER_LOAD_BUTTON, &CTraderDlg::OnBnClickedTraderLoadButton)
	ON_BN_CLICKED(IDC_TRADER_ADD_BUTTON, &CTraderDlg::OnBnClickedTraderAddButton)
	ON_BN_CLICKED(IDC_TRADER_UPDATE_BUTTON, &CTraderDlg::OnBnClickedTraderUpdateButton)
	ON_BN_CLICKED(IDC_TRADER_DELETE_BUTTON, &CTraderDlg::OnBnClickedTraderDeleteButton)
	ON_EN_CHANGE(IDC_TRADER_INI_EDIT, &CTraderDlg::OnEnChangeTraderIniEdit)
END_MESSAGE_MAP()


// CTraderDlg message handlers
BEGIN_EVENTSINK_MAP(CTraderDlg, CDialog)
	ON_EVENT(CTraderDlg, IDC_TRADER_LIST, 11, CTraderDlg::DblClickTraderList, VTS_I4 VTS_I4)
	ON_EVENT(CTraderDlg, IDC_TRADER_LIST, 2, CTraderDlg::BlockSelectedTraderList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

BOOL CTraderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	BeginWaitCursor();

	GetOraLoader().SetDB(&theDB);

	m_SS.SetVisibleCols(6);
	m_SS.SetVisibleRows(10);

	m_TraderIniEdit.Setup(this, IDC_TRADER_INI_EDIT);
	m_TraderNameEdit.Setup(this, IDC_TRADER_NAME_EDIT);
	m_FMSNameEdit.Setup(this, IDC_TRADER_FMSNAME_EDIT);
	
	m_Active.Setup(this, IDC_TRADER_ACTIVE_CHECK, Y, N);
	m_PowerUser.Setup(this, IDC_TRADER_POWERUSER_CHECK, Y);
	m_RiskManager.Setup(this, IDC_TRADER_RISKMAN_CHECK, Y);

	LoadTraders();

	if(m_SS.GetSheetRows() > 0)
		DblClickTraderList(1, 1);

	EndWaitCursor();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTraderDlg::DblClickTraderList(long Col, long Row)
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows()) // TODO: Add your message handler code here
	{
		m_TraderIniEdit.SetData(m_SS.GetSheetText(1, Row));
		m_TraderNameEdit.SetData(m_SS.GetSheetText(2, Row));
		m_FMSNameEdit.SetData(m_SS.GetSheetText(3, Row));
		m_Active.SetData(m_SS.GetSheetText(4, Row));
		m_PowerUser.SetData(m_SS.GetSheetText(5, Row));
		m_RiskManager.SetData(m_SS.GetSheetText(6, Row));
		m_SS.SetSheetCurRow(Row);
	}
	else
	{
		m_TraderIniEdit.SetData(EMPTYSTRING);
		m_TraderNameEdit.SetData(EMPTYSTRING);
		m_FMSNameEdit.SetData(EMPTYSTRING);
		m_Active.SetData(N);
		m_PowerUser.SetData(EMPTYSTRING);
		m_RiskManager.SetData(EMPTYSTRING);
		m_SS.SetSheetCurRow(0);
	}

	GetDlgItem(IDC_TRADER_UPDATE_BUTTON)->EnableWindow(m_SS.GetSheetCurRow() > 0);
}


void CTraderDlg::BlockSelectedTraderList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	DblClickTraderList(BlockCol, BlockRow);
	
	GetDlgItem(IDC_TRADER_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected() > 0);
}


void CTraderDlg::OnBnClickedTraderLoadButton()
{
	LoadTraders();
}


void CTraderDlg::OnBnClickedTraderAddButton()
{
	if(IsOK(TRUE))
	{
		CDBRec Rec;

		GetData(Rec);

		if(GetOraLoader().Open("SELECT TRADER_INI, TRADER, USER_NAME, ACTIVE, SUPERVISOR, RISK_MAN FROM SEMAM.NW_TRADERS", ODYNASET_DEFAULT)) 
			GetOraLoader().UpdateRecord(Rec, TRUE);

		LoadTraders();
	}
}


void CTraderDlg::OnBnClickedTraderUpdateButton()
{
	if(IsOK(FALSE))
	{
		CDBRec Rec;

		GetData(Rec);

		if(GetOraLoader().Open("SELECT TRADER_INI, TRADER, USER_NAME, ACTIVE, SUPERVISOR, RISK_MAN FROM SEMAM.NW_TRADERS WHERE TRADER_INI = '" + m_TraderIniEdit.GetData() + "' ", ODYNASET_DEFAULT)) 
			GetOraLoader().UpdateRecord(Rec);

		LoadTraders();
	}
}


void CTraderDlg::OnBnClickedTraderDeleteButton()
{
	if(IsOK(FALSE))
	{
		if(GetOraLoader().GetCount("SELECT COUNT(*) FROM SEMAM.ALL_TICKETS_AR WHERE TRADER_INI = '" + m_TraderIniEdit.GetData() + "' ") == 0)
			GetOraLoader().ExecuteSql("DELETE SEMAM.NW_TRADERS WHERE TRADER_INI = '" + m_TraderIniEdit.GetData() + "' ");
		else
			MessageBox("This name cannot be deleted because has been used in trade"); 

		LoadTraders();
	}
}


void CTraderDlg::OnEnChangeTraderIniEdit()
{
	GetDlgItem(IDC_TRADER_ADD_BUTTON)->EnableWindow(!m_TraderIniEdit.GetData().IsEmpty());
}
