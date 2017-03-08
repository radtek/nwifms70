// CPCounterpartyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "CPCounterpartyDlg.h"
#include "qdata.h"


// CCPCounterpartyDlg dialog

IMPLEMENT_DYNAMIC(CCPCounterpartyDlg, CDialog)

CCPCounterpartyDlg::CCPCounterpartyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCPCounterpartyDlg::IDD, pParent)
{
	m_CPList = NULL;
}

CCPCounterpartyDlg::~CCPCounterpartyDlg()
{
	if(m_CPList)
		delete m_CPList;
}

void CCPCounterpartyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CPCP_LIST, m_SS);
}

void CCPCounterpartyDlg::EnableCtrls()
{
	GetDlgItem(IDC_CPCP_UPDATE_BUTTON)->EnableWindow(m_SS.GetSheetCurRow() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_CPCP_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected());
}

// CCurrencyDlg message handlers
BOOL CCPCounterpartyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	COraLoader OraLoader;

	m_SS.SetVisibleRows(15);
	m_SS.SetVisibleCols(2);

	m_CPEdit.Setup(this, IDC_CPCP_CP_EDIT);
	m_CPEdit.LimitText(20);
	m_CPList = (COptComboBox*) new COptComboBox(this, IDC_CPCP_CP_COMBO);

	BeginWaitCursor();

	OraLoader.SetDB(&theDB);
	OraLoader.Open("SELECT CP_CODE FROM SEMAM.NW_COUNTER_PARTY WHERE ISVALID = 'Y' ORDER BY 1 ");
	OraLoader.LoadComboBox(*m_CPList);

	OnBnClickedCpcpLoadButton();
	EndWaitCursor();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CCPCounterpartyDlg, CDialog)
	ON_EVENT(CCPCounterpartyDlg, IDC_CPCP_LIST, 2, CCPCounterpartyDlg::OnBlockSelectedCpcpList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CCPCounterpartyDlg, IDC_CPCP_LIST, 11, CCPCounterpartyDlg::OnDblClickCpcpList, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

void CCPCounterpartyDlg::OnBlockSelectedCpcpList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	OnDblClickCpcpList(BlockCol, BlockRow);
}

void CCPCounterpartyDlg::OnDblClickCpcpList(long Col, long Row)
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		m_CPEdit.SetWindowText(m_SS.GetSheetText(1, Row));
		m_CPList->SelectString(0, m_SS.GetSheetText(2, Row));
	}
	else
		OnBnClickedCpcpClearButton();

	EnableCtrls();
}

BEGIN_MESSAGE_MAP(CCPCounterpartyDlg, CDialog)
	ON_BN_CLICKED(IDC_CPCP_CLEAR_BUTTON, &CCPCounterpartyDlg::OnBnClickedCpcpClearButton)
	ON_BN_CLICKED(IDC_CPCP_ADD_BUTTON, &CCPCounterpartyDlg::OnBnClickedCpcpAddButton)
	ON_BN_CLICKED(IDC_CPCP_UPDATE_BUTTON, &CCPCounterpartyDlg::OnBnClickedCpcpUpdateButton)
	ON_BN_CLICKED(IDC_CPCP_DELETE_BUTTON, &CCPCounterpartyDlg::OnBnClickedCpcpDeleteButton)
	ON_BN_CLICKED(IDC_CPCP_LOAD_BUTTON, &CCPCounterpartyDlg::OnBnClickedCpcpLoadButton)
	ON_BN_CLICKED(IDC_CPCP_LOADALL_BUTTON, &CCPCounterpartyDlg::OnBnClickedCpcpLoadallButton)
END_MESSAGE_MAP()


// CCPCounterpartyDlg message handlers

void CCPCounterpartyDlg::OnBnClickedCpcpClearButton()
{
	m_CPEdit.SetWindowText(EMPTYSTRING);
	m_CPList->SetCurSel(-1);
}

void CCPCounterpartyDlg::OnBnClickedCpcpAddButton()
{
	if(m_CPEdit.GetWindowTextLength() > 0 && m_CPList->GetCurSel() >= 0)
		return;

	COraLoader OraLoader;
	CQData QData;
	CString CP, qCP, MyCP;

	OraLoader.SetDB(&theDB);
	m_CPEdit.GetWindowText(CP);
	qCP = QData.GetQueryText(CP);
	m_CPList->GetSelString(MyCP);
	OraLoader.GetSql() = "SELECT COUNT(*) FROM SEMAM.NW_PRE_RAW_CP WHERE CP_CP = " + qCP;
	if(OraLoader.GetCount() < 1)
	{
		OraLoader.Open("SELECT CP_CP, COUNTERPARTY FROM SEMAM.NW_PRE_RAW_CP ", ODYNASET_DEFAULT);
		OraLoader.AddNewRecord();
		OraLoader.SetFieldValue(0, CP);
		OraLoader.SetFieldValue(1, MyCP);
		OraLoader.Update();
	}
	else
		OnBnClickedCpcpUpdateButton();
}

void CCPCounterpartyDlg::OnBnClickedCpcpUpdateButton()
{
	if(m_CPEdit.GetWindowTextLength() == 0 || m_CPList->GetCurSel() < 0)
		return;

	COraLoader OraLoader;
	CQData QData;
	CString CP, qCP, MyCP;

	OraLoader.SetDB(&theDB);
	m_CPEdit.GetWindowText(CP);
	qCP = QData.GetQueryText(CP);
	m_CPList->GetSelString(MyCP);

	if(OraLoader.Open("SELECT COUNTERPARTY FROM SEMAM.NW_PRE_RAW_CP "
						"WHERE CP_CP = " + qCP, ODYNASET_DEFAULT))
	{
		OraLoader.StartEdit();
		OraLoader.SetFieldValue(0, CP);
		OraLoader.Update();
	}

	m_SS.SetSheetCurRow(-1);
}

void CCPCounterpartyDlg::OnBnClickedCpcpDeleteButton()
{
	if(m_CPEdit.GetWindowTextLength() == 0 || m_CPList->GetCurSel() < 0)
		return;

	COraLoader OraLoader;
	CQData QData;
	CString CP, qCP;

	OraLoader.SetDB(&theDB);
	m_CPEdit.GetWindowText(CP);
	qCP = QData.GetQueryText(CP);

	OraLoader.ExecuteSql("DELETE FROM SEMAM.NW_PRE_RAW_CP WHERE CP_CP = " + qCP);
	GetDlgItem(IDC_CPCP_DELETE_BUTTON)->EnableWindow(FALSE);
}

void CCPCounterpartyDlg::OnBnClickedCpcpLoadButton()
{
	COraLoader OraLoader;

	OraLoader.SetDB(&theDB);
	m_SS.ClearSheet();

	OraLoader.Open("SELECT DISTINCT A.CP_CP, NULL \"COUNTERPARTY\" "
					"FROM SEMAM.NW_PRE_RAW_TICKETS A "
					"WHERE 0 = (SELECT COUNT(*) FROM SEMAM.NW_PRE_RAW_CP B "
								"WHERE B.CP_CP = A.CP_CP) ORDER BY 1 ");
	OraLoader.LoadDBSheet(m_SS);
}

void CCPCounterpartyDlg::OnBnClickedCpcpLoadallButton()
{
	COraLoader OraLoader;

	OraLoader.SetDB(&theDB);
	m_SS.ClearSheet();

	OraLoader.Open("SELECT DISTINCT A.CP_CP, COUNTERPARTY "
					"FROM SEMAM.NW_PRE_RAW_CP A ORDER BY 1 ");
	OraLoader.LoadDBSheet(m_SS);
}
