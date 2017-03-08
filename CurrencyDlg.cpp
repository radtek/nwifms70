// CurrencyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "CurrencyDlg.h"
#include "qdata.h"


// CCurrencyDlg dialog

IMPLEMENT_DYNAMIC(CCurrencyDlg, CDialog)

CCurrencyDlg::CCurrencyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCurrencyDlg::IDD, pParent)
{
	m_DBPBTier = NULL;
	m_HSBCTier = NULL;
	m_Inverted = NULL;
	m_NDF_Fix = NULL;
	m_1256 = NULL;
}

CCurrencyDlg::~CCurrencyDlg()
{
	if(m_DBPBTier)
		delete m_DBPBTier;
	if(m_HSBCTier)
		delete m_HSBCTier;
	if(m_Inverted)
		delete m_Inverted;
	if(m_NDF_Fix)
		delete m_NDF_Fix;
	if(m_1256)
		delete m_1256;
}

void CCurrencyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CURRENCY_LIST, m_SS);
}

void CCurrencyDlg::EnableCtrls()
{
	GetDlgItem(IDC_CURRENCY_UPDATE_BUTTON)->EnableWindow(m_Data.GetCurrentRow() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_CURRENCY_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected());
}

BEGIN_MESSAGE_MAP(CCurrencyDlg, CDialog)
	ON_BN_CLICKED(IDC_CURRENCY_FIND_BUTTON, &CCurrencyDlg::OnBnClickedCurrencyFindButton)
	ON_BN_CLICKED(IDC_CURRENCY_CLEAR_BUTTON, &CCurrencyDlg::OnBnClickedCurrencyClearButton)
	ON_BN_CLICKED(IDC_CURRENCY_ADD_BUTTON, &CCurrencyDlg::OnBnClickedCurrencyAddButton)
	ON_BN_CLICKED(IDC_CURRENCY_UPDATE_BUTTON, &CCurrencyDlg::OnBnClickedCurrencyUpdateButton)
	ON_BN_CLICKED(IDC_CURRENCY_DELETE_BUTTON, &CCurrencyDlg::OnBnClickedCurrencyDeleteButton)
END_MESSAGE_MAP()


// CCurrencyDlg message handlers
BOOL CCurrencyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CQData QData;
	
	m_SS.SetVisibleRows(14);
	m_SS.SetVisibleCols(8);

	m_Currency.Setup(this, IDC_CURRENCY_CURRENCY_EDIT);
	m_Currency.LimitText(3);
	m_NewCurrency.Setup(this, IDC_CURRENCY_NEWCURRENCY_EDIT);
	m_NewCurrency.LimitText(3);
	m_Conversion.Setup(this, IDC_CURRENCY_CONVERSION_EDIT);
	m_ConvDate.Setup(this, IDC_CURRENCY_CONVDATE_EDIT);
	m_DBPBTier = (COptComboBox*) new COptComboBox(this, IDC_CURRENCY_DBPBTIER_COMBO, TRUE);
	m_HSBCTier = (COptComboBox*) new COptComboBox(this, IDC_CURRENCY_HSBCTIER_COMBO, TRUE);
	m_Inverted = (CCheckBox*) new CCheckBox(this, IDC_CURRENCY_INVERTED_CHECK, Y);
	m_NDF_Fix = (CCheckBox*) new CCheckBox(this, IDC_CURRENCY_NDFFIX_CHECK, Y);
	m_1256 = (CCheckBox *) new CCheckBox(this, IDC_CURRENCY_1256_CHECK, Y);

	m_Data.Add(&m_Currency, &m_Data.GetRec().GetCurrency());
	m_Data.Add(&m_NewCurrency, &m_Data.GetRec().GetNewCurrency());
	m_Data.Add(&m_Conversion, &m_Data.GetRec().GetConversion());
	m_Data.Add(&m_ConvDate, &m_Data.GetRec().GetConvDate());
	m_Data.Add(m_DBPBTier, &m_Data.GetRec().GetDBPBTier());
	m_Data.Add(m_HSBCTier, &m_Data.GetRec().GetHSBCTier());
	m_Data.Add(m_Inverted, &m_Data.GetRec().GetInverted());
	m_Data.Add(m_NDF_Fix, &m_Data.GetRec().GetNDFFix());
	m_Data.Add(m_1256, &m_Data.GetRec().Get1256());

	m_Data.Setup(&m_SS);
	m_Data.SetOraLoader(m_pData->GetOraLoader());

	BeginWaitCursor();

	if(m_Data.GetOraLoader().Open("SELECT DISTINCT DB_TIER FROM SEMAM.NW_CURRENCY ORDER BY 1"))
		m_Data.GetOraLoader().LoadComboBox(*m_DBPBTier);
	if(m_Data.GetOraLoader().Open("SELECT DISTINCT HSBC_TIER FROM SEMAM.NW_CURRENCY ORDER BY 1 "))
		m_Data.GetOraLoader().LoadComboBox(*m_HSBCTier);

	this->SetWindowText("CURRENCY");

	EndWaitCursor();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CCurrencyDlg, CDialog)
ON_EVENT(CCurrencyDlg, IDC_CURRENCY_LIST, 11, CCurrencyDlg::OnDblClickCurrencyList, VTS_I4 VTS_I4)
ON_EVENT(CCurrencyDlg, IDC_CURRENCY_LIST, 2, CCurrencyDlg::OnBlockSelectedCurrencyList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

void CCurrencyDlg::OnDblClickCurrencyList(long Col, long Row)
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
		m_Data.SetCurrentRow(Row);
	else
		m_Data.SetCurrentRow(-1);

	EnableCtrls();
}

void CCurrencyDlg::OnBlockSelectedCurrencyList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	OnDblClickCurrencyList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
}

void CCurrencyDlg::OnBnClickedCurrencyFindButton()
{
	m_Data.LoadDBData();
}

void CCurrencyDlg::OnBnClickedCurrencyClearButton()
{
	m_Data.Refresh();
	EnableCtrls();
	GetDlgItem(IDC_CURRENCY_ADD_BUTTON)->EnableWindow(FALSE);
}

void CCurrencyDlg::OnBnClickedCurrencyAddButton()
{
	if(m_Data.AddRow())
	{
		m_Data.Refresh();
		m_pData->GetCurrencyArr().Modified();
	}
}

void CCurrencyDlg::OnBnClickedCurrencyUpdateButton()
{
	m_Data.UpdateRow();
	m_pData->GetCurrencyArr().Modified();
}

void CCurrencyDlg::OnBnClickedCurrencyDeleteButton()
{
	m_Data.DeleteRow();
	m_pData->GetCurrencyArr().Modified();
}
