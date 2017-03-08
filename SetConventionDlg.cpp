// SetConventionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "SetConventionDlg.h"


// CSetConventionDlg dialog

IMPLEMENT_DYNAMIC(CSetConventionDlg, CDialog)

CSetConventionDlg::CSetConventionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetConventionDlg::IDD, pParent)
{
	m_CP = NULL;
	m_Portfolio = NULL;
	m_Dir = NULL;
}

CSetConventionDlg::~CSetConventionDlg()
{
	if(m_CP)
		delete m_CP;
	if(m_Portfolio)
		delete m_Portfolio;
	if(m_Dir)
		delete m_Dir;
}

void CSetConventionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SETCONVENTION_LIST, m_SS);
}

void CSetConventionDlg::AssignData()
{
	CString Data;

	Data.Empty();

	for(int i = 1; i <= m_SS.GetSheetCols(); i ++)
	{
		if(m_SS.GetSheetCurRow() >= 1)
			Data = m_SS.GetSheetText(i, m_SS.GetSheetCurRow());

		switch(i)
		{
			case 1:
				m_SetConvention.SetWindowText(Data);
				break;
			case 2:
				m_CP->SelectString(0, Data);
				break;
			case 3:
				m_Portfolio->SelectString(0, Data);
				break;
			case 4:
				m_TransNum.SetData(Data);
				break;
			case 5:
				m_Dir->SelectString(0, Data);
				break;
			case 6:
				m_NomAmount.SetWindowText(Data);
				break;
			default:
				break;
		}
	}
}

void CSetConventionDlg::LoadSetConvention()
{
	m_pData->GetContactList().CopyKeyToComboBox(*m_CP);
	m_pData->GetTransDirArr().CopyToComboBox(*m_Dir);
	m_pData->GetPortfolioArr().CopyToComboBox(*m_Portfolio);

	if(m_sPortfolio.IsEmpty())
		m_pData->GetOraLoader().GetSql().Format("SELECT SET_CONVENTION, "
			"COUNTERPARTY, PORTFOLIO, TRANS_NUM, TRANS_DIRECTION, NOM_AMOUNT "
			"FROM SEMAM.NW_TR_TICKETS "
			"WHERE ASSET_CODE = '%s' "
			"AND TRANS_TYPE = 'SECURITIES' ", (LPCSTR) m_sAsset);
	else
		m_pData->GetOraLoader().GetSql().Format("SELECT SET_CONVENTION, "
			"COUNTERPARTY, PORTFOLIO, TRANS_NUM, TRANS_DIRECTION, NOM_AMOUNT "
			"FROM SEMAM.NW_TR_TICKETS "
			"WHERE PORTFOLIO = '%s' AND ASSET_CODE = '%s' "
			"AND TRANS_TYPE = 'SECURITIES' ", (LPCSTR) m_sPortfolio, (LPCSTR) m_sAsset);

	m_pData->GetOraLoader().Open();
	m_pData->GetOraLoader().GetFieldArray().GetAt(3)->SetWithComma(FALSE);
	m_pData->GetOraLoader().LoadDBSheet(m_SS);
}

BEGIN_MESSAGE_MAP(CSetConventionDlg, CDialog)
END_MESSAGE_MAP()


// CSetConventionDlg message handlers
BEGIN_EVENTSINK_MAP(CSetConventionDlg, CDialog)
	ON_EVENT(CSetConventionDlg, IDC_SETCONVENTION_LIST, 11, CSetConventionDlg::OnDblClickSetconventionList, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

BOOL CSetConventionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	BeginWaitCursor();
	m_SS.SetVisibleCols(6);
	m_SS.SetVisibleRows(10);

	m_SetConvention.Setup(this, IDC_SETCONVENTION_CONVENTION_EDIT);
	m_SetConvention.LimitText(15);
	m_CP = new COptComboBox(this, IDC_SETCONVENTION_CP_COMBO);
	m_Portfolio = new COptComboBox(this, IDC_SETCONVENTION_PORTFOLIO_COMBO);
	m_TransNum.Setup(this, IDC_SETCONVENTION_TRANSNUM_EDIT);
	m_Dir = new COptComboBox(this, IDC_SETCONVENTION_DIR_COMBO);
	m_NomAmount.Setup(this, IDC_SETCONVENTION_NOMINAL_EDIT);
	
	LoadSetConvention();
	EndWaitCursor();

	return TRUE;
}

void CSetConventionDlg::OnDblClickSetconventionList(long Col, long Row)
{
	if(Row >= 0 && Row <= m_SS.GetSheetRows())
		m_SS.SetSheetCurRow(Row);
	else
		m_SS.SetSheetCurRow(-1);
	AssignData();
}

void CSetConventionDlg::OnOK()
{
	m_SetConvention.GetWindowText(m_sSetConvention);
	CDialog::OnOK();
}

void CSetConventionDlg::OnCancel()
{
	m_sSetConvention.Empty();
	CDialog::OnCancel();
}
