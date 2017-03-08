// ChangeFundDlg.cpp : implementation file
//

#include "stdafx.h"
#include "nwifms70.h"
#include "qdata.h"
#include "ChangeFundDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChangeFundDlg dialog


CChangeFundDlg::CChangeFundDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChangeFundDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChangeFundDlg)
	m_Portfolio = NULL;
	m_NewPortfolio = NULL;
	//}}AFX_DATA_INIT
}

CChangeFundDlg::~CChangeFundDlg()
{
	if(m_Portfolio)
		delete m_Portfolio;
	if(m_NewPortfolio)
		delete m_NewPortfolio;
}

void CChangeFundDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChangeFundDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChangeFundDlg, CDialog)
	//{{AFX_MSG_MAP(CChangeFundDlg)
	ON_BN_CLICKED(IDC_CF_FIND, OnCfFind)
	ON_BN_CLICKED(IDC_CF_UPDATE, OnCfUpdate)
	ON_EN_CHANGE(IDC_CF_TRANSNUM_EDIT, OnChangeCfTransnumEdit)
	ON_CBN_SELCHANGE(IDC_CF_NEWPORTFOLIO_COMBO, OnSelchangeCfNewportfolioCombo)
	ON_BN_CLICKED(IDC_CF_CLEAR, OnCfClear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChangeFundDlg message handlers

BOOL CChangeFundDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CQData QData;
	
	m_TransNum.Setup(this, IDC_CF_TRANSNUM_EDIT);
	m_Portfolio = (COptComboBox *) new COptComboBox(this, IDC_CF_PORTFOLIO_COMBO);
	m_NewPortfolio = (COptComboBox *) new COptComboBox(this, IDC_CF_NEWPORTFOLIO_COMBO);

	GetDlgItem(IDC_CF_FIND)->EnableWindow(FALSE);
	GetDlgItem(IDC_CF_UPDATE)->EnableWindow(FALSE);
	
	m_OraLoader = m_pData->GetOraLoader();

	m_pData->LoadSupportData();
	QData.CopyDBRecArrayToComboBox(m_pData->GetPortfolioArr(), *m_Portfolio);
	QData.CopyDBRecArrayToComboBox(m_pData->GetPortfolioArr(), *m_NewPortfolio);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CChangeFundDlg::OnCfFind() 
{
	CString Text;
	CQData QData;

	Text = m_TransNum.GetData();
	
	m_OraLoader.GetSql().Format("SELECT TRANS_NUM, PORTFOLIO FROM SEMAM.NW_TR_TICKETS "
								"WHERE TRANS_NUM = %s ", QData.GetQueryNumber(Text));
	m_OraLoader.Open();
	m_OraLoader.MoveFirst();
	Text = m_OraLoader.GetDBString(1);
	m_Portfolio->SelectString(-1, Text);

	GetDlgItem(IDC_CF_FIND)->EnableWindow(FALSE);
}

void CChangeFundDlg::OnCfUpdate() 
{
	CString TransNum, Portfolio;
	CQData QData;

	TransNum = m_TransNum.GetData();
	m_NewPortfolio->GetSelString(Portfolio);
	if(TransNum.IsEmpty() || Portfolio.IsEmpty())
		return;

	m_OraLoader.GetSql().Format("UPDATE SEMAM.NW_TR_TICKETS SET PORTFOLIO = '%s' "
							"WHERE TRANS_NUM = %s ", (LPCTSTR) Portfolio, 
							QData.GetQueryNumber(TransNum));
	if(m_OraLoader.ExecuteSql())
		MessageBox("Portfolio has been changed successfully", MB_OK);
	GetDlgItem(IDC_CF_UPDATE)->EnableWindow(FALSE);
}

void CChangeFundDlg::OnChangeCfTransnumEdit() 
{
	GetDlgItem(IDC_CF_FIND)->EnableWindow(!m_TransNum.GetData().IsEmpty());
}

void CChangeFundDlg::OnSelchangeCfNewportfolioCombo() 
{
	GetDlgItem(IDC_CF_UPDATE)->EnableWindow(m_NewPortfolio->GetCurSel() >= 0);
}

void CChangeFundDlg::OnCfClear() 
{
	m_TransNum.SetData(EMPTYSTRING);
	m_Portfolio->SetCurSel(-1);
	m_NewPortfolio->SetCurSel(-1);
	GetDlgItem(IDC_CF_FIND)->EnableWindow(FALSE);
	GetDlgItem(IDC_CF_UPDATE)->EnableWindow(FALSE);
}
