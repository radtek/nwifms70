// OPsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "OPsDlg.h"
#include "afxdialogex.h"


// COPsDlg dialog

IMPLEMENT_DYNAMIC(COPsDlg, CDialog)

COPsDlg::COPsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COPsDlg::IDD, pParent)
{

}


COPsDlg::~COPsDlg()
{
}


void COPsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OPS_LIST, m_SS);
}


void COPsDlg::GetData(CDBRec &Rec)
{
	Rec.RemoveAll();

	Rec.Add(m_OpsNameEdit.GetData());
	Rec.Add(m_FMSNameEdit.GetData());
	Rec.Add(m_Active.GetData());
	Rec.Add(m_PowerUser.GetData());
	Rec.Add(m_Guest.GetData());
}


BOOL COPsDlg::IsOK(BOOL bNew)
{
	CString Text;

	if(m_OpsNameEdit.GetData().IsEmpty())
		Text = "User Name cannot be black";
	else
		if(bNew)
		{
			if(GetOraLoader().GetCount("SELECT COUNT(*) FROM SEMAM.NW_TRADERS WHERE TRADER_INI = '" + m_OpsNameEdit.GetData() + "' ") > 0)
				Text = "User Name has been used";
		}
	
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


int COPsDlg::LoadOps()
{
	if(GetOraLoader().Open("SELECT OPS_NAME, USER_NAME, ACTIVE, POWERUSER, GUEST "
							"FROM SEMAM.NW_OPERATIONS "
							"ORDER BY 3 DESC, 1 "))
		GetOraLoader().LoadDBSheet(m_SS, TRUE);

	return m_SS.GetSheetRows();
}


BEGIN_EVENTSINK_MAP(COPsDlg, CDialog)
	ON_EVENT(COPsDlg, IDC_OPS_LIST, 11, COPsDlg::DblClickOpsList, VTS_I4 VTS_I4)
	ON_EVENT(COPsDlg, IDC_OPS_LIST, 2, COPsDlg::BlockSelectedOpsList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


BOOL COPsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	BeginWaitCursor();

	GetOraLoader().SetDB(&theDB);

	m_SS.SetVisibleCols(5);
	m_SS.SetVisibleRows(10);

	m_OpsNameEdit.Setup(this, IDC_OPS_NAME_EDIT);
	m_FMSNameEdit.Setup(this, IDC_OPS_USERNAME_EDIT);
	
	m_Active.Setup(this, IDC_OPS_ACTIVE_CHECK, Y, N);
	m_PowerUser.Setup(this, IDC_OPS_POWERUSER_CHECK, Y);
	m_Guest.Setup(this, IDC_OPS_GUEST_CHECK, Y);

	LoadOps();

	if(m_SS.GetSheetRows() > 0)
		DblClickOpsList(1, 1);

	EndWaitCursor();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}


BEGIN_MESSAGE_MAP(COPsDlg, CDialog)
	ON_BN_CLICKED(IDC_OPS_LOAD_BUTTON, &COPsDlg::OnBnClickedOpsLoadButton)
	ON_BN_CLICKED(IDC_OPS_ADD_BUTTON, &COPsDlg::OnBnClickedOpsAddButton)
	ON_BN_CLICKED(IDC_OPS_UPDATE_BUTTON, &COPsDlg::OnBnClickedOpsUpdateButton)
	ON_BN_CLICKED(IDC_OPS_DELETE_BUTTON, &COPsDlg::OnBnClickedOpsDeleteButton)
	ON_EN_CHANGE(IDC_OPS_NAME_EDIT, &COPsDlg::OnEnChangeOpsNameEdit)
END_MESSAGE_MAP()


// COPsDlg message handlers
void COPsDlg::DblClickOpsList(long Col, long Row)
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows()) // TODO: Add your message handler code here
	{
		m_OpsNameEdit.SetData(m_SS.GetSheetText(1, Row));
		m_FMSNameEdit.SetData(m_SS.GetSheetText(2, Row));
		m_Active.SetData(m_SS.GetSheetText(3, Row));
		m_PowerUser.SetData(m_SS.GetSheetText(4, Row));
		m_Guest.SetData(m_SS.GetSheetText(5, Row));
		m_SS.SetSheetCurRow(Row);
	}
	else
	{
		m_OpsNameEdit.SetData(EMPTYSTRING);
		m_FMSNameEdit.SetData(EMPTYSTRING);
		m_Active.SetData(N);
		m_PowerUser.SetData(EMPTYSTRING);
		m_Guest.SetData(EMPTYSTRING);
		m_SS.SetSheetCurRow(0);
	}

	GetDlgItem(IDC_OPS_UPDATE_BUTTON)->EnableWindow(m_SS.GetSheetCurRow() > 0);
}


void COPsDlg::BlockSelectedOpsList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	DblClickOpsList(BlockCol, BlockRow);
	
	GetDlgItem(IDC_OPS_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected() > 0);
}


void COPsDlg::OnBnClickedOpsLoadButton()
{
	LoadOps();
}


void COPsDlg::OnBnClickedOpsAddButton()
{
	if(IsOK(TRUE))
	{
		CDBRec Rec;

		GetData(Rec);

		if(GetOraLoader().Open("SELECT OPS_NAME, USER_NAME, ACTIVE, POWERUSER, GUEST FROM SEMAM.NW_OPERATIONS", ODYNASET_DEFAULT)) 
			GetOraLoader().UpdateRecord(Rec, TRUE);

		LoadOps();
	}
}


void COPsDlg::OnBnClickedOpsUpdateButton()
{
	if(IsOK(FALSE))
	{
		CDBRec Rec;

		GetData(Rec);

		if(GetOraLoader().Open("SELECT OPS_NAME, USER_NAME, ACTIVE, POWERUSER, GUEST FROM SEMAM.NW_OPERATIONS WHERE OPS_NAME = '" + m_OpsNameEdit.GetData() + "' ", ODYNASET_DEFAULT)) 
			GetOraLoader().UpdateRecord(Rec);

		LoadOps();
	}
}


void COPsDlg::OnBnClickedOpsDeleteButton()
{
	if(IsOK(FALSE))
	{
		if(GetOraLoader().GetCount("SELECT COUNT(*) FROM SEMAM.ALL_TICKETS_AR WHERE USER_STAMP = '" + m_FMSNameEdit.GetData() + "' ") == 0)
			GetOraLoader().ExecuteSql("DELETE SEMAM.NW_OPERATIONS WHERE OPS_NAME = '" + m_FMSNameEdit.GetData() + "' ");
		else
			MessageBox("This name cannot be deleted because has been used in trade"); 

		LoadOps();
	}
}


void COPsDlg::OnEnChangeOpsNameEdit()
{
	GetDlgItem(IDC_OPS_ADD_BUTTON)->EnableWindow(!m_OpsNameEdit.GetData().IsEmpty());
}
