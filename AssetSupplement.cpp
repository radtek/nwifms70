// AssetSupplement.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "AssetSupplement.h"
#include "assetdlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAssetSupplement dialog


CAssetSupplement::CAssetSupplement(CWnd* pParent /*=NULL*/)
	: CDialog(CAssetSupplement::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAssetSupplement)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAssetSupplement::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAssetSupplement)
	DDX_Control(pDX, IDC_ASSETSUPP_LIST, m_SS);
	//}}AFX_DATA_MAP
}

void CAssetSupplement::EnableCtrls()
{
	GetDlgItem(IDC_ASSETSUPP_UPDATE_BUTTON)->EnableWindow(m_Data.GetCurrentRow() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_ASSETSUPP_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected());
}

BEGIN_MESSAGE_MAP(CAssetSupplement, CDialog)
	//{{AFX_MSG_MAP(CAssetSupplement)
	ON_BN_CLICKED(IDC_ASSETSUPP_ADD_BUTTON, OnAssetsuppAddButton)
	ON_BN_CLICKED(IDC_ASSETSUPP_CLEAR_BUTTON, OnAssetsuppClearButton)
	ON_BN_CLICKED(IDC_ASSETSUPP_FIND_BUTTON, OnAssetsuppFindButton)
	ON_BN_CLICKED(IDC_ASSETSUPP_DELETE_BUTTON, OnAssetsuppDeleteButton)
	ON_BN_CLICKED(IDC_ASSETSUPP_FINDASSET_BUTTON, OnAssetsuppFindassetButton)
	ON_BN_CLICKED(IDC_ASSETSUPP_UPDATE_BUTTON, OnAssetsuppUpdateButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAssetSupplement message handlers

BOOL CAssetSupplement::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_SS.SetVisibleRows(14);
	m_SS.SetVisibleCols(8);

	m_Asset.Setup(this, IDC_ASSETSUPP_ASSET_EDIT);
	m_Asset.LimitText(10);
	m_Code.Setup(this, IDC_ASSETSUPP_SUPPCODE_EDIT);
	m_Code.LimitText(10);
	m_Desc.Setup(this, IDC_ASSETSUPP_DESC_EDIT);
	m_Desc.LimitText(30);
	m_Common.Setup(this, IDC_ASSETSUPP_COMMON_EDIT);
	m_Common.LimitText(13);
	m_Isin.Setup(this, IDC_ASSETSUPP_ISIN_EDIT);
	m_Isin.LimitText(12);
	m_AmountOf.Setup(this, IDC_ASSETSUPP_AMOUNTOF_EDIT);
	m_PerAmount.Setup(this, IDC_ASSETSUPP_PERAMOUNT_EDIT);
	m_RoundingFactor.Setup(this, IDC_ASSETSUPP_ROUNDINGFACTOR_EDIT);

	m_Data.Add(&m_Asset, &m_Data.GetRec().GetAsset());
	m_Data.Add(&m_Code, &m_Data.GetRec().GetCode());
	m_Data.Add(&m_Desc, &m_Data.GetRec().GetDesc());
	m_Data.Add(&m_Common, &m_Data.GetRec().GetCommon());
	m_Data.Add(&m_Isin, &m_Data.GetRec().GetIsin());
	m_Data.Add(&m_AmountOf, &m_Data.GetRec().GetAmountOf());
	m_Data.Add(&m_PerAmount, &m_Data.GetRec().GetPerAmount());
	m_Data.Add(&m_RoundingFactor, &m_Data.GetRec().GetRoundingFactor());

	m_Data.Setup(m_pData->GetOraLoader(), &m_SS, m_pData->GetUser());

	if(!m_Data.GetRec().GetAsset().IsEmpty())
		m_Asset.SetWindowText(m_Data.GetRec().GetAsset());

	this->SetWindowText("ASSET SUPPLEMENTS");
	OnAssetsuppFindButton();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_EVENTSINK_MAP(CAssetSupplement, CDialog)
    //{{AFX_EVENTSINK_MAP(CAssetSupplement)
	ON_EVENT(CAssetSupplement, IDC_ASSETSUPP_LIST, 2 /* BlockSelected */, OnBlockSelectedAssetsuppList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CAssetSupplement, IDC_ASSETSUPP_LIST, 11 /* DblClick */, OnDblClickAssetsuppList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CAssetSupplement::OnAssetsuppAddButton() 
{
	if(m_Data.AddRow())
		m_Data.Refresh();
}

void CAssetSupplement::OnAssetsuppClearButton() 
{
	m_Data.Refresh();
	EnableCtrls();
	GetDlgItem(IDC_ASSETSUPP_ADD_BUTTON)->EnableWindow(FALSE);
}

void CAssetSupplement::OnAssetsuppFindButton() 
{
	m_Data.LoadDBData();
}

void CAssetSupplement::OnAssetsuppDeleteButton() 
{
	m_Data.DeleteRow();
}

void CAssetSupplement::OnAssetsuppFindassetButton() 
{
	CAssetDlg Dlg;

	Dlg.m_pData = m_pData;
	m_Asset.GetWindowText(Dlg.m_FindData.GetRec().GetAsset());
	Dlg.m_bWithNew = FALSE;

	if(Dlg.DoModal() != IDOK)
		return;
	
	m_Asset.SetWindowText(Dlg.m_FindData.GetRec().GetAsset());
}

void CAssetSupplement::OnAssetsuppUpdateButton() 
{
	m_Data.UpdateRow();
}

void CAssetSupplement::OnBlockSelectedAssetsuppList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickAssetsuppList(m_SS.GetActiveCol(), m_SS.GetActiveRow());	
}

void CAssetSupplement::OnDblClickAssetsuppList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
		m_Data.SetCurrentRow(Row);
	else
		m_Data.SetCurrentRow(-1);

	EnableCtrls();
}
