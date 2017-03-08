// InvestorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "InvestorDialog.h"
#include "qdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInvestorDialog dialog


CInvestorDialog::CInvestorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CInvestorDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInvestorDialog)
	m_Country = NULL;
	m_Agent = NULL;
	m_InvClass = NULL;
	m_Status = NULL;

	m_Asset = NULL;
	m_OnShore = NULL;
	m_Erisa = NULL;
	m_Bhca = NULL;
	m_IPO = NULL;
	m_Side = NULL;
	//}}AFX_DATA_INIT
}

CInvestorDialog::~CInvestorDialog()
{
	if(m_Country)
		delete m_Country;
	if(m_Agent)
		delete m_Agent;
	if(m_InvClass)
		delete m_InvClass;
	if(m_Status)
		delete m_Status;
	if(m_Asset)
		delete m_Asset;
	if(m_OnShore)
		delete m_OnShore;
	if(m_Erisa)
		delete m_Erisa;
	if(m_Bhca)
		delete m_Bhca;
	if(m_IPO)
		delete m_IPO;
	if(m_Side)
		delete m_Side;
}

void CInvestorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInvestorDialog)
	DDX_Control(pDX, IDC_INVESTOR_LIST, m_SS);
	DDX_Control(pDX, IDC_INVESTOR_ALLOC_LIST, m_AllocSS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInvestorDialog, CDialog)
	//{{AFX_MSG_MAP(CInvestorDialog)
	ON_BN_CLICKED(IDC_INVESTOR_ADD_BUTTON, OnInvestorAddButton)
	ON_BN_CLICKED(IDC_INVESTOR_DELETE_BUTTON, OnInvestorDeleteButton)
	ON_BN_CLICKED(IDC_INVESTOR_FIND_BUTTON, OnInvestorFindButton)
	ON_BN_CLICKED(IDC_INVESTOR_UPDATE_BUTTON, OnInvestorUpdateButton)
	ON_BN_CLICKED(IDC_INVESTOR_CLEAR_BUTTON, OnInvestorClearButton)
	ON_EN_KILLFOCUS(IDC_INVESTOR_ID_EDIT, OnKillfocusIDEdit)
	ON_BN_CLICKED(IDC_INVESTOR_ADD_ASSET_BUTTON, OnInvestorAddAssetButton)
	ON_BN_CLICKED(IDC_INVESTOR_DELETE_ASSET_BUTTON, OnInvestorDeleteAssetButton)
	ON_BN_CLICKED(IDC_INVESTOR_UPDATE_ASSET_BUTTON, OnInvestorUpdateAssetButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CInvestorDialog::EnableCtrls()
{
	BOOL bEnable = FALSE;
	
	if(m_Data.GetCurrentRow() > 0)
		bEnable = TRUE;

	m_Asset->EnableWindow(bEnable);
	m_TaxID.EnableWindow(bEnable);
	m_OnShore->EnableWindow(bEnable);
	m_Erisa->EnableWindow(bEnable);
	m_Bhca->EnableWindow(bEnable);
	m_IPO->EnableWindow(bEnable);
	m_Side->EnableWindow(bEnable);

	GetDlgItem(IDC_INVESTOR_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected()|| m_AllocSS.GetIsBlockSelected());
	GetDlgItem(IDC_INVESTOR_UPDATE_BUTTON)->EnableWindow(m_Data.GetCurrentRow() > 0 ? TRUE : FALSE);

	if(m_Data.GetSRowCtrl().GetCurrentRow() > 0)
	{
		GetDlgItem(IDC_INVESTOR_DELETE_ASSET_BUTTON)->EnableWindow(m_AllocSS.GetIsBlockSelected());
		GetDlgItem(IDC_INVESTOR_UPDATE_ASSET_BUTTON)->EnableWindow(m_Data.GetSRowCtrl().GetCurrentRow() > 0 ? TRUE : FALSE);
	}
}
/////////////////////////////////////////////////////////////////////////////
// CInvestorDialog message handlers

BEGIN_EVENTSINK_MAP(CInvestorDialog, CDialog)
    //{{AFX_EVENTSINK_MAP(CInvestorDialog)
	ON_EVENT(CInvestorDialog, IDC_INVESTOR_ALLOC_LIST, 11 /* DblClick */, OnDblClickAllocList, VTS_I4 VTS_I4)
	ON_EVENT(CInvestorDialog, IDC_INVESTOR_ALLOC_LIST, 2 /* BlockSelected */, OnBlockSelectedAllocList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CInvestorDialog, IDC_INVESTOR_LIST, 11 /* DblClick */, OnDblClickInvestorList, VTS_I4 VTS_I4)
	ON_EVENT(CInvestorDialog, IDC_INVESTOR_LIST, 2 /* BlockSelected */, OnBlockSelectedInvestorList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CInvestorDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CQData QData;

	m_SS.SetVisibleCols(7);
	m_SS.SetVisibleRows(5);
	m_AllocSS.SetVisibleCols(7);
	m_AllocSS.SetVisibleRows(5);

	m_InvCode.Setup(this, IDC_INVESTOR_ID_EDIT);
	m_InvCode.LimitText(10);
	m_Investor.Setup(this, IDC_INVESTOR_EDIT);
	m_Investor.LimitText(40);
	m_Address.Setup(this, IDC_INVESTOR_ADDRESS_EDIT);
	m_Address.LimitText(80);
	m_City.Setup(this, IDC_INVESTOR_CITY_EDIT);
	m_City.LimitText(80);
	m_Country = (COptComboBox *) new COptComboBox(this, IDC_INVESTOR_COUNTRY_COMBO, TRUE);
	m_Agent = (COptComboBox *) new COptComboBox(this, IDC_INVESTOR_AGENT_COMBO, TRUE);
	m_InvClass = (COptComboBox *) new COptComboBox(this, IDC_INVESTOR_CLASS_COMBO, TRUE);
	m_MgmtFee.Setup(this, IDC_INVESTOR_MGMTFEE_EDIT);
	m_Performance.Setup(this, IDC_INVESTOR_PERFORMANCE_EDIT);
	m_Status =  (CCheckBox*) new CCheckBox(this, IDC_INVESTOR_STATUS_CHECK, "C", EMPTYSTRING);

	m_Asset = (COptComboBox *) new COptComboBox(this, IDC_INVESTOR_ASSET_COMBO);
	m_TaxID.Setup(this, IDC_INVESTOR_TAX_EDIT);
	m_TaxID.LimitText(20);
	m_OnShore = (CCheckBox*) new CCheckBox(this, IDC_INVESTOR_ONSHORE_CHECK, "Y", EMPTYSTRING);
	m_Erisa = (CCheckBox*) new CCheckBox(this, IDC_INVESTOR_ERISA_CHECK, "Y", EMPTYSTRING);
	m_Bhca = (CCheckBox*) new CCheckBox(this, IDC_INVESTOR_BHCA_CHECK, "Y", EMPTYSTRING);
	m_IPO = (CCheckBox*) new CCheckBox(this, IDC_INVESTOR_IPO_CHECK, "Y", EMPTYSTRING);
	m_Side = (CCheckBox*) new CCheckBox(this, IDC_INVESTOR_SIDE_CHECK, "Y", EMPTYSTRING);

	m_Data.Add(&m_InvCode, &m_Data.GetInvestorRec().GetInvCode());
	m_Data.Add(&m_Investor, &m_Data.GetInvestorRec().GetInvestor());
	m_Data.Add(&m_Address, &m_Data.GetInvestorRec().GetAddress());
	m_Data.Add(&m_City, &m_Data.GetInvestorRec().GetCity());
	m_Data.Add(m_Country, &m_Data.GetInvestorRec().GetCountry());
	m_Data.Add(m_Agent, &m_Data.GetInvestorRec().GetAgent());
	m_Data.Add(m_InvClass, &m_Data.GetInvestorRec().GetInvClass());
	m_Data.Add(&m_MgmtFee, &m_Data.GetInvestorRec().GetMgmtFee());
	m_Data.Add(&m_Performance, &m_Data.GetInvestorRec().GetPerformance());
	m_Data.Add(m_Status, &m_Data.GetInvestorRec().GetStatus());

	m_Data.GetSRowCtrl().Add(m_Asset, &m_Data.GetInvestorAssetRec().GetAsset());
	m_Data.GetSRowCtrl().Add(&m_TaxID, &m_Data.GetInvestorAssetRec().GetTaxID());
	m_Data.GetSRowCtrl().Add(m_OnShore, &m_Data.GetInvestorAssetRec().GetOnShore());
	m_Data.GetSRowCtrl().Add(m_Erisa, &m_Data.GetInvestorAssetRec().GetErisa());
	m_Data.GetSRowCtrl().Add(m_Bhca, &m_Data.GetInvestorAssetRec().GetBhca());
	m_Data.GetSRowCtrl().Add(m_IPO, &m_Data.GetInvestorAssetRec().GetIPO());
	m_Data.GetSRowCtrl().Add(m_Side, &m_Data.GetInvestorAssetRec().GetSide());

	m_Data.Setup(m_pData->GetOraLoader(), &m_SS, &m_AllocSS);

	QData.CopyDBRecArrayToComboBox(m_pData->GetCountryArr(), *m_Country, 0, FALSE);
	if(m_Data.GetOraLoader().Open("SELECT AGENT FROM SEMAM.NW_AGENT ORDER BY 1 "))
		m_Data.GetOraLoader().LoadComboBox(*m_Agent);

	if(m_Data.GetOraLoader().Open("SELECT INV_CLASS FROM SEMAM.NW_INVESTOR_CLASS ORDER BY 1 "))
		m_Data.GetOraLoader().LoadComboBox(*m_InvClass);

	if(m_Data.GetOraLoader().Open("SELECT ASS_CODE FROM SEMAM.NW_ASSETS "
								"WHERE ASS_ISSUER IN ('ALPHA-SSF FUND', 'EMCF FUND', 'EXPLORER FUND') "
								"AND STATUS IS NULL ORDER BY 1 "))
		m_Data.GetOraLoader().LoadComboBox(*m_Asset);
	m_Data.SetHeaders();

	m_Data.SetCurrentRow(-1);

	GetDlgItem(IDC_INVESTOR_UPDATE_ASSET_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_INVESTOR_DELETE_ASSET_BUTTON)->EnableWindow(FALSE);

	this->SetWindowText("INVESTORS");
	OnInvestorClearButton();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CInvestorDialog::OnInvestorAddButton() 
{
	if(m_Data.AddRow())
		m_Data.Refresh();
}

void CInvestorDialog::OnInvestorDeleteButton() 
{
	m_Data.DeleteRow();	
}

void CInvestorDialog::OnInvestorFindButton() 
{
	m_Data.LoadDBData();
}

void CInvestorDialog::OnInvestorUpdateButton() 
{
	m_Data.UpdateRow();
}

void CInvestorDialog::OnDblClickAllocList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_AllocSS.GetSheetRows())
		m_Data.GetSRowCtrl().SetCurrentRow(Row);
	else
		m_Data.GetSRowCtrl().SetCurrentRow(-1);

	GetDlgItem(IDC_INVESTOR_UPDATE_ASSET_BUTTON)->EnableWindow(m_AllocSS.GetSheetCurRow() > 0 ? TRUE: FALSE);
	EnableCtrls();
}

void CInvestorDialog::OnBlockSelectedAllocList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	OnDblClickAllocList(m_AllocSS.GetActiveCol(), m_AllocSS.GetActiveRow());
	GetDlgItem(IDC_INVESTOR_DELETE_ASSET_BUTTON)->EnableWindow(m_AllocSS.GetSheetCurRow() > 0 ? TRUE: FALSE);
}

void CInvestorDialog::OnDblClickInvestorList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
		m_Data.SetCurrentRow(Row);
	else
		m_Data.SetCurrentRow(-1);
	GetDlgItem(IDC_INVESTOR_UPDATE_BUTTON)->EnableWindow(m_SS.GetSheetCurRow() > 0 ? TRUE: FALSE);
	EnableCtrls();
}

void CInvestorDialog::OnBlockSelectedInvestorList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	OnDblClickInvestorList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
	GetDlgItem(IDC_INVESTOR_DELETE_BUTTON)->EnableWindow(m_SS.GetSheetCurRow() > 0 ? TRUE: FALSE);
}

void CInvestorDialog::OnInvestorClearButton() 
{
	m_Data.Refresh();
	m_Data.GetSRowCtrl().Refresh();
	
	EnableCtrls();
	GetDlgItem(IDC_INVESTOR_ADD_BUTTON)->EnableWindow(FALSE);
}

void CInvestorDialog::OnKillfocusIDEdit()
{
	GetDlgItem(IDC_INVESTOR_ADD_BUTTON)->EnableWindow(m_InvCode.GetWindowTextLength() > 0 ||
													m_Asset->GetCurSel() >= 0 ? TRUE : FALSE);
}

void CInvestorDialog::OnKillfocusAssetEdit()
{
	GetDlgItem(IDC_INVESTOR_ADD_BUTTON)->EnableWindow(m_InvCode.GetWindowTextLength() > 0||
														m_Asset->GetCurSel() >= 0 ? TRUE : FALSE);
	GetDlgItem(IDC_INVESTOR_ADD_ASSET_BUTTON)->EnableWindow(m_Asset->GetCurSel() >= 0 ? TRUE : FALSE);
}

void CInvestorDialog::OnInvestorAddAssetButton() 
{
	m_Data.AddSRow();	
}

void CInvestorDialog::OnInvestorDeleteAssetButton() 
{
	m_Data.DeleteSRow();
}

void CInvestorDialog::OnInvestorUpdateAssetButton() 
{
	m_Data.UpdateSRow();
}
