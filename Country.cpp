// Country.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "Country.h"
#include "qdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCountry dialog


CCountry::CCountry(CWnd* pParent /*=NULL*/)
	: CDialog(CCountry::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCountry)
	m_OldCountry = NULL;
	m_GeoArea = NULL;
	m_PFRegion = NULL;
	m_ProfileRegion = NULL;
	m_Pru = NULL;
	m_NewCurrency = NULL;
	m_SovFlag = NULL;
	m_SecFlag = NULL;
	m_DMFlag = NULL;
	//}}AFX_DATA_INIT
}


CCountry::~CCountry()
{
	if(m_OldCountry)
		delete m_OldCountry;
	if(m_GeoArea)
		delete m_GeoArea;
	if(m_PFRegion)
		delete m_PFRegion;
	if(m_ProfileRegion)
		delete m_ProfileRegion;
	if(m_Pru)
		delete m_Pru;
	if(m_NewCurrency)
		delete m_NewCurrency;
	if(m_SovFlag)
		delete m_SovFlag;
	if(m_SecFlag)
		delete m_SecFlag;
	if(m_DMFlag)
		delete m_DMFlag;
}

void CCountry::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCountry)
	DDX_Control(pDX, IDC_COUNTRY_LIST, m_SS);
	//}}AFX_DATA_MAP
}


void CCountry::EnableCtrls()
{
	GetDlgItem(IDC_COUNTRY_UPDATE_BUTTON)->EnableWindow(m_Data.GetCurrentRow() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_COUNTRY_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected());
}


BEGIN_MESSAGE_MAP(CCountry, CDialog)
	//{{AFX_MSG_MAP(CCountry)
	ON_BN_CLICKED(IDC_COUNTRY_ADD_BUTTON, OnCountryAddButton)
	ON_BN_CLICKED(IDC_COUNTRY_CLEAR_BUTTON, OnCountryClearButton)
	ON_BN_CLICKED(IDC_COUNTRY_DELETE_BUTTON, OnCountryDeleteButton)
	ON_BN_CLICKED(IDC_COUNTRY_FIND_BUTTON, OnCountryFindButton)
	ON_BN_CLICKED(IDC_COUNTRY_UPDATE_BUTTON, OnCountryUpdateButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCountry message handlers

BEGIN_EVENTSINK_MAP(CCountry, CDialog)
    //{{AFX_EVENTSINK_MAP(CCountry)
	ON_EVENT(CCountry, IDC_COUNTRY_LIST, 2 /* BlockSelected */, OnBlockSelectedCountryList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CCountry, IDC_COUNTRY_LIST, 11 /* DblClick */, OnDblClickCountryList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CCountry::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CQData QData;
	
	m_SS.SetVisibleRows(14);
	m_SS.SetVisibleCols(8);

	m_Country.Setup(this, IDC_COUNTRY_COUNTRY_EDIT);
	m_Country.LimitText(20);
	m_Code.Setup(this, IDC_COUNTRY_CODE_EDIT);
	m_Code.LimitText(3);
	m_Code2.Setup(this, IDC_COUNTRY_CODE2_COMBO, 2, TRUE);
	m_OldCountry = (COptComboBox*) new COptComboBox(this, IDC_COUNTRY_OLDCOUNTRY_COMBO, TRUE);
	m_GeoArea = (COptComboBox*) new COptComboBox(this, IDC_COUNTRY_GEOAREA_COMBO);
	m_PFRegion = (COptComboBox*) new COptComboBox(this, IDC_COUNTRY_PFGEOAREA_COMBO);
	m_ProfileRegion = (COptComboBox*) new COptComboBox(this, IDC_COUNTRY_PROFILEGEOAREA_COMBO);
	m_Pru = (COptComboBox*) new COptComboBox(this, IDC_COUNTRY_PRU_COMBO, TRUE);
	m_Currency.Setup(this, IDC_COUNTRY_CURRENCY_EDIT);
	m_Currency.LimitText(3);
	m_NewCurrency = (COptComboBox*) new COptComboBox(this, IDC_COUNTRY_NEWCURRENCY_COMBO, TRUE);
	m_Conversion.Setup(this, IDC_COUNTRY_CONVERSION_EDIT);
	m_ConvDate.Setup(this, IDC_COUNTRY_CONVDATE_EDIT);
	m_SovFlag = (CCheckBox*) new CCheckBox(this, IDC_COUNTRY_SOV_CHECK, Y);
	m_SecFlag = (CCheckBox*) new CCheckBox(this, IDC_COUNTRY_SEC_CHECK, Y);
	m_DMFlag = (CCheckBox*) new CCheckBox(this, IDC_COUNTRY_DM_CHECK, Y);

	m_Data.Add(&m_Country, &m_Data.GetRec().GetCountry());
	m_Data.Add(&m_Code, &m_Data.GetRec().GetCode());
	m_Data.Add(&m_Code2, &m_Data.GetRec().GetCode2());
	m_Data.Add(m_OldCountry, &m_Data.GetRec().GetOldCountry());
	m_Data.Add(&m_Data.GetRec().GetLimit());
	m_Data.Add(m_GeoArea, &m_Data.GetRec().GetGeoArea());
	m_Data.Add(m_PFRegion, &m_Data.GetRec().GetPFRegion());
	m_Data.Add(m_ProfileRegion, &m_Data.GetRec().GetProfileRegion());
	m_Data.Add(m_SovFlag, &m_Data.GetRec().GetSovFlag());
	m_Data.Add(m_SecFlag, &m_Data.GetRec().GetSecFlag());
	m_Data.Add(m_DMFlag, &m_Data.GetRec().GetDMFlag());
	m_Data.Add(m_Pru, &m_Data.GetRec().GetPru());
	m_Data.Add(&m_Currency, &m_Data.GetRec().GetCurrency());
	m_Data.Add(m_NewCurrency, &m_Data.GetRec().GetNewCurrency());
	m_Data.Add(&m_Conversion, &m_Data.GetRec().GetConversion());
	m_Data.Add(&m_ConvDate, &m_Data.GetRec().GetConvDate());

	m_Data.Setup(&m_SS);
	m_Data.SetOraLoader(m_pData->GetOraLoader());

	BeginWaitCursor();

	QData.CopyDBRecArrayToComboBox(m_pData->GetCountryArr(), *m_OldCountry, 0, FALSE);
	QData.CopyDBRecArrayToComboBox(m_pData->GetCurrencyArr(), *m_NewCurrency, 0, FALSE);
	if(m_Data.GetOraLoader().Open("SELECT C_CODE2, COUNTRY FROM SEMAM.NW_COUNTRY_CODE2 ORDER BY 1 "))
		m_Data.GetOraLoader().LoadMCComboBox(m_Code2);
	if(m_Data.GetOraLoader().Open("SELECT GEO_AREA FROM SEMAM.NW_GEO_AREAS ORDER BY 1"))
		m_Data.GetOraLoader().LoadComboBox(*m_GeoArea);
	if(m_Data.GetOraLoader().Open("SELECT PF_REGION FROM SEMAM.NW_PF_REGION ORDER BY 1"))
		m_Data.GetOraLoader().LoadComboBox(*m_PFRegion);
	if(m_Data.GetOraLoader().Open("SELECT PROFILE_REGION FROM SEMAM.NW_PROFILE_REGION ORDER BY 1"))
		m_Data.GetOraLoader().LoadComboBox(*m_ProfileRegion);
	if(m_Data.GetOraLoader().Open("SELECT PRU_TIER FROM SEMAM.NW_PRU_TIER ORDER BY 1 "))
		m_Data.GetOraLoader().LoadComboBox(*m_Pru);

	this->SetWindowText("COUNTRY");
	OnCountryFindButton();
	EndWaitCursor();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCountry::OnCountryAddButton() 
{
	if(m_Data.AddRow())
	{
		m_Data.Refresh();
		m_pData->GetCountryArr().Modified();
		m_pData->GetCurrencyArr().Modified();
	}
}

void CCountry::OnCountryClearButton() 
{
	m_Data.Refresh();
	EnableCtrls();
	GetDlgItem(IDC_COUNTRY_ADD_BUTTON)->EnableWindow(FALSE);
}

void CCountry::OnCountryDeleteButton() 
{
	m_Data.DeleteRow();
	m_pData->GetCountryArr().Modified();
	m_pData->GetCurrencyArr().Modified();
}

void CCountry::OnCountryFindButton() 
{
	m_Data.LoadDBData();
}

void CCountry::OnCountryUpdateButton() 
{
	m_Data.UpdateRow();
	m_pData->GetCountryArr().Modified();
	m_pData->GetCurrencyArr().Modified();
}

void CCountry::OnBlockSelectedCountryList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickCountryList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
}

void CCountry::OnDblClickCountryList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
		m_Data.SetCurrentRow(Row);
	else
		m_Data.SetCurrentRow(-1);

	EnableCtrls();
}
