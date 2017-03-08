// AgentDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nwifms70.h"
#include "AgentDlg.h"
#include "qdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "qdata.h"
#include "dbrec.h"

/////////////////////////////////////////////////////////////////////////////
// CAgentDlg dialog


CAgentDlg::CAgentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAgentDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAgentDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAgentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAgentDlg)
	DDX_Control(pDX, IDC_AGENT_LIST, m_SS);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAgentDlg, CDialog)
	//{{AFX_MSG_MAP(CAgentDlg)
	ON_EN_CHANGE(IDC_AGENT_ID_EDIT, OnChangeAgentIdEdit)
	ON_BN_CLICKED(IDC_AGENT_ADD_BUTTON, OnAgentAddButton)
	ON_BN_CLICKED(IDC_AGENT_CLEAR_BUTTON, OnAgentClearButton)
	ON_BN_CLICKED(IDC_AGENT_DELETE_BUTTON, OnAgentDeleteButton)
	ON_BN_CLICKED(IDC_AGENT_FIND_BUTTON, OnAgentFindButton)
	ON_BN_CLICKED(IDC_AGENT_UPDATE_BUTTON, OnAgentUpdateButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAgentDlg message handlers

BEGIN_EVENTSINK_MAP(CAgentDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CAgentDlg)
	ON_EVENT(CAgentDlg, IDC_AGENT_LIST, 2 /* BlockSelected */, OnBlockSelectedAgentList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CAgentDlg, IDC_AGENT_LIST, 11 /* DblClick */, OnDblClickAgentList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CAgentDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	BeginWaitCursor();
	m_SS.SetVisibleCols(3);
	m_SS.SetVisibleRows(10);

	m_IDEdit.Setup(this, IDC_AGENT_ID_EDIT);
	m_IDEdit.LimitText(10);
	m_AgentEdit.Setup(this, IDC_AGENT_AGENT_EDIT);
	m_AgentEdit.LimitText(40);
	m_RebateEdit.Setup(this, IDC_AGENT_REBATE_EDIT);

	OnAgentFindButton();
	EndWaitCursor();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAgentDlg::OnChangeAgentIdEdit() 
{
	BOOL bEnable;

	bEnable = m_IDEdit.GetWindowTextLength();
	GetDlgItem(IDC_AGENT_ADD_BUTTON)->EnableWindow(bEnable);
	GetDlgItem(IDC_AGENT_UPDATE_BUTTON)->EnableWindow(bEnable);
}

void CAgentDlg::OnAgentAddButton() 
{
	CString Text;
	CDBRec DBRec;
	CQData QData;

	m_IDEdit.GetWindowText(Text);
	if(Text.GetLength() == 0)
	{
		MessageBox("Agent ID does not exist");
		return;
	}
	DBRec.Add(Text);
	
	m_AgentEdit.GetWindowText(Text);
	if(Text.GetLength() == 0)
	{
		MessageBox("Agent does not exist");
		return;
	}

	DBRec.Add(Text);
	m_RebateEdit.GetWindowText(Text);
	if(Text.GetLength() == 0)
	{
		MessageBox("Rebate rate does not exist");
		return;
	}

	DBRec.Add(QData.GetQueryNumber(Text));

	m_OraLoader.GetSql() = "SELECT AGENT_ID, AGENT, REBATE FROM SEMAM.NW_AGENT ";
	if(m_OraLoader.Open(m_OraLoader.GetSql(), ODYNASET_DEFAULT))
	{
		if(m_OraLoader.UpdateRecord(DBRec, TRUE))
			m_SS.AddSheetRow(DBRec);
	}
}

void CAgentDlg::OnAgentClearButton() 
{
	m_IDEdit.SetWindowText("");
	m_AgentEdit.SetWindowText("");
	m_RebateEdit.SetWindowText("");
}

void CAgentDlg::OnAgentDeleteButton() 
{
	CString Text;
	CQData QData;

	m_IDEdit.GetWindowText(Text);
	if(Text.GetLength() <= 0)
	{
		MessageBox("No Agent selected");
		return;
	}

	m_OraLoader.GetSql().Format("DELETE SEMAM.NW_AGENT WHERE AGENT_ID = %s ", QData.GetQueryText(Text));
	if(m_OraLoader.ExecuteSql())
		m_SS.DeleteSheetRow(m_Row);
}

void CAgentDlg::OnAgentFindButton() 
{
	int Count = 0;
	CString Sql, Text;
	
	Sql = "SELECT AGENT_ID, AGENT, REBATE FROM SEMAM.NW_AGENT ";
	if(m_IDEdit.GetWindowTextLength() > 0 ||m_AgentEdit.GetWindowTextLength() > 0 || m_RebateEdit.GetWindowTextLength() > 0)
	{
		Sql += "WHERE ";
		if(m_IDEdit.GetWindowTextLength() > 0)
		{
			m_IDEdit.GetWindowText(Text);
			Sql += "AGENT_ID LIKE '%" + Text + "%'";
			Count = 1;
		}

		if(m_AgentEdit.GetWindowTextLength() > 0)
		{
			m_AgentEdit.GetWindowText(Text);
			if(Count > 0)
				Sql += "AND ";
			Sql += "AGENT LIKE '%" + Text + "%'";
			Count = 1;
		}
	
		if(m_RebateEdit.GetWindowTextLength() > 0)
		{
			m_AgentEdit.GetWindowText(Text);
			if(Count > 0)
				Sql += "AND ";
			Sql += "REBATE LIKE '%" + Text + "%'";
			Count = 1;
		}
	}

	if(m_OraLoader.Open(Sql))
		m_OraLoader.LoadDBSheet(m_SS, TRUE);	
}

void CAgentDlg::OnBlockSelectedAgentList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickAgentList(BlockCol, BlockRow);
}

void CAgentDlg::OnDblClickAgentList(long Col, long Row) 
{
	CString Text;

	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		m_Row = Row;
		for(int i = 1; i <= 3; i ++)
		{
			Text = m_SS.GetSheetText(i, m_Row);
			switch(i)
			{
				case 1:
					m_IDEdit.SetWindowText(Text);
					break;
				case 2:
					m_AgentEdit.SetWindowText(Text);
					break;
				case 3:
					m_RebateEdit.SetWindowText(Text);
					break;
				default:
					break;
			}
		}
	}
	else
	{
		m_IDEdit.SetWindowText("");
		m_AgentEdit.SetWindowText("");
		m_RebateEdit.SetWindowText("");
		m_Row = 0;
	}
}

void CAgentDlg::OnAgentUpdateButton() 
{
	CString Text, AgentID;
	CDBRec DBRec;
	CQData QData;

	m_IDEdit.GetWindowText(Text);
	if(Text.GetLength() == 0)
	{
		MessageBox("Agent ID does not exist");
		return;
	}
	AgentID = Text;
	DBRec.Add(Text);
	
	m_AgentEdit.GetWindowText(Text);
	if(Text.GetLength() == 0)
	{
		MessageBox("Agent does not exist");
		return;
	}
	DBRec.Add(Text);
	
	m_RebateEdit.GetWindowText(Text);
	if(Text.GetLength() == 0)
	{
		MessageBox("Rebate rate does not exist");
		return;
	}
	DBRec.Add(QData.GetQueryNumber(Text));

	m_OraLoader.GetSql().Format("SELECT AGENT_ID, AGENT, REBATE FROM SEMAM.NW_AGENT WHERE AGENT_ID = %s ", 
								QData.GetQueryText(AgentID));
	if(m_OraLoader.Open(m_OraLoader.GetSql(), ODYNASET_DEFAULT))
	{
		if(m_OraLoader.UpdateRecord(DBRec))
			m_SS.UpdateSheetRow(m_Row, DBRec);
	}
}

void CAgentDlg::OnCancel() 
{
	// TODO: Add extra cleanup here

	CDialog::OnCancel();
}

