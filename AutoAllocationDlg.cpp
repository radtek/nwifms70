// AutoAllocationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "AutoAllocationDlg.h"


// CAutoAllocationDlg dialog

IMPLEMENT_DYNAMIC(CAutoAllocationDlg, CDialog)

CAutoAllocationDlg::CAutoAllocationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoAllocationDlg::IDD, pParent)
{
	m_AAMethod = NULL;
	m_Portfolio = NULL;
}

CAutoAllocationDlg::~CAutoAllocationDlg()
{
	if(m_AAMethod)
		delete m_AAMethod;
	if(m_Portfolio)
		delete m_Portfolio;
}

void CAutoAllocationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_AA_LIST, m_SS);
}

void CAutoAllocationDlg::EnableCtrls()
{
	GetDlgItem(IDC_AA_UPDATE_BUTTON)->EnableWindow(m_SS.GetSheetCurRow() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_AA_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected());
}

BEGIN_MESSAGE_MAP(CAutoAllocationDlg, CDialog)
	ON_BN_CLICKED(IDC_AA_FIND_BUTTON, &CAutoAllocationDlg::OnBnClickedAaFindButton)
	ON_BN_CLICKED(IDC_AA_CLEAR_BUTTON, &CAutoAllocationDlg::OnBnClickedAaClearButton)
	ON_BN_CLICKED(IDC_AA_ADD_BUTTON, &CAutoAllocationDlg::OnBnClickedAaAddButton)
	ON_BN_CLICKED(IDC_AA_UPDATE_BUTTON, &CAutoAllocationDlg::OnBnClickedAaUpdateButton)
	ON_BN_CLICKED(IDC_AA_DELETE_BUTTON, &CAutoAllocationDlg::OnBnClickedAaDeleteButton)
END_MESSAGE_MAP()


// CAutoAllocationDlg message handlers
BEGIN_EVENTSINK_MAP(CAutoAllocationDlg, CDialog)
	ON_EVENT(CAutoAllocationDlg, IDC_AA_LIST, 11, CAutoAllocationDlg::OnDblClickAaList, VTS_I4 VTS_I4)
	ON_EVENT(CAutoAllocationDlg, IDC_AA_LIST, 2, CAutoAllocationDlg::OnBlockSelectedAaList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

BOOL CAutoAllocationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	COraLoader OraLoader;

	m_Data.Setup(m_pData->GetOraLoader(), &m_SS);
	m_SS.SetVisibleRows(14);
	m_SS.SetVisibleCols(5);

	m_AAMethod = new COptComboBox(this, IDC_AA_METHOD_COMBO);
	m_FromDate.Setup(this, IDC_AA_FROMDATE_EDIT);
	m_ToDate.Setup(this, IDC_AA_TODATE_EDIT);
	m_Portfolio = new COptComboBox(this, IDC_AA_PORTFOLIO_COMBO);
	m_Alloc.Setup(this, IDC_AA_ALLOC_EDIT);

	m_Data.Add(m_AAMethod, &m_Data.GetRec().GetAAMethod());
	m_Data.Add(&m_FromDate, &m_Data.GetRec().GetFromDate());
	m_Data.Add(&m_ToDate, &m_Data.GetRec().GetToDate());
	m_Data.Add(m_Portfolio, &m_Data.GetRec().GetPortfolio());
	m_Data.Add(&m_Alloc, &m_Data.GetRec().GetAllocRate());

	m_Data.Setup(m_pData->GetOraLoader(), &m_SS);
	OraLoader = m_pData->GetOraLoader();
	OraLoader.Open("SELECT AA_METHOD FROM SEMAM.NW_AA_METHOD ORDER BY 1 ");
	OraLoader.LoadComboBox(*m_AAMethod);
	m_pData->GetPortfolioArr().CopyToComboBox(*m_Portfolio);

	OnBnClickedAaFindButton();
	return TRUE;
}

void CAutoAllocationDlg::OnDblClickAaList(long Col, long Row)
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
		m_Data.SetCurrentRow(Row);
	else
		m_Data.SetCurrentRow(-1);

	EnableCtrls();
}

void CAutoAllocationDlg::OnBlockSelectedAaList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	OnDblClickAaList(m_SS.GetActiveCol(), m_SS.GetActiveRow());	
}

void CAutoAllocationDlg::OnBnClickedAaFindButton()
{
	m_Data.LoadDBData();
}

void CAutoAllocationDlg::OnBnClickedAaClearButton()
{
	m_Data.Refresh();
	EnableCtrls();
	GetDlgItem(IDC_AA_ADD_BUTTON)->EnableWindow(FALSE);
}

void CAutoAllocationDlg::OnBnClickedAaAddButton()
{
	if(m_Data.AddRow())
		m_Data.Refresh();
}

void CAutoAllocationDlg::OnBnClickedAaUpdateButton()
{
	m_Data.UpdateRow();
}

void CAutoAllocationDlg::OnBnClickedAaDeleteButton()
{
	m_Data.DeleteRow();
}
