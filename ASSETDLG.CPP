// assetdlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "assetdlg.h"
#include "sqlstmt.h"
#include "qdata.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAssetDlg dialog

CAssetDlg::CAssetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAssetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAssetDlg)
	m_bWithNew = TRUE;
	m_bCash = FALSE;
	m_nActionID = FINDACTION_OK;
	m_bTicket = FALSE;
	//}}AFX_DATA_INIT
}

void CAssetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAssetDlg)
	DDX_Control(pDX, IDC_ASSETDLG_ITEM_LIST, m_SS);
	DDX_Control(pDX, IDC_ASSETDLG_HOLDING_LIST, m_HoldingSS);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAssetDlg, CDialog)
	//{{AFX_MSG_MAP(CAssetDlg)
	ON_BN_CLICKED(IDC_ASSETDLG_FIND_BUTTON, OnFindButton)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_ASSETDLG_REFRESH_BUTTON, OnRefreshButton)
	ON_CBN_SELCHANGE(IDC_ASSETDLG_BUCKET_COMBO, OnSelchangeAssetBucketCombo)
	ON_CBN_SELCHANGE(IDC_ASSETDLG_CATEGORY_COMBO, OnSelchangeAssetCategoryCombo)
	ON_CBN_SELCHANGE(IDC_ASSETDLG_CLASS_COMBO, OnSelchangeAssetClassCombo)
	ON_CBN_SELCHANGE(IDC_ASSETDLG_COUNTRY_COMBO, OnSelchangeAssetCountryCombo)
	ON_CBN_SELCHANGE(IDC_ASSETDLG_CURRENCY_COMBO, OnSelchangeAssetCurrencyCombo)
	ON_CBN_SELCHANGE(IDC_ASSETDLG_INDUSTRY_COMBO, OnSelchangeAssetIndustryCombo)
	ON_EN_CHANGE(IDC_ASSETDLG_DESC_EDIT, OnChangeAssetdescEdit)
	ON_EN_CHANGE(IDC_ASSETDLG_COMMON_EDIT, OnChangeAssetCommonEdit)
	ON_EN_CHANGE(IDC_ASSETDLG_ISIN_EDIT, OnChangeAssetIsinEdit)
	ON_EN_CHANGE(IDC_ASSETDLG_MATURITY_EDIT, OnChangeAssetMaturityEdit)
	ON_EN_CHANGE(IDC_ASSETDLG_MSTC_EDIT, OnChangeAssetMstcEdit)
	ON_EN_CHANGE(IDC_ASSETDLG_SEDOL_EDIT, OnChangeAssetSedolEdit)
	ON_EN_CHANGE(IDC_ASSETDLG_SOURCE_EDIT, OnChangeAssetSourceEdit)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ASSETDLG_UNWIND_BUTTON, &CAssetDlg::OnBnClickedAssetdlgUnwindButton)
	ON_BN_CLICKED(IDC_ASSETDLG_OPTEX_BUTTON, &CAssetDlg::OnBnClickedAssetdlgOptexButton)
	ON_BN_CLICKED(IDC_ASSETDLG_ADDPOS_BUTTON, &CAssetDlg::OnBnClickedAssetdlgAddposButton)
	ON_EN_CHANGE(IDC_ASSETDLG_ASSET_EDIT, &CAssetDlg::OnEnChangeAssetdlgAssetEdit)
END_MESSAGE_MAP()

void CAssetDlg::EnableButtons(BOOL bEnable)
{
	if(!bEnable)
	{
		
		GetDlgItem(IDOK)->EnableWindow(bEnable);
		GetDlgItem(IDC_ASSETDLG_ADDPOS_BUTTON)->EnableWindow(bEnable);
		GetDlgItem(IDC_ASSETDLG_UNWIND_BUTTON)->EnableWindow(bEnable);
		GetDlgItem(IDC_ASSETDLG_OPTEX_BUTTON)->EnableWindow(bEnable);
	}
	else
	{
		CString TransType;

		GetDlgItem(IDOK)->EnableWindow(bEnable);
		if(m_bTicket)
		{
			TransType = m_TransTypeCombo.GetData();
			if(TransType == CDS || TransType == SECURITIES || TransType == CALL || TransType == PUT)
			{
				GetDlgItem(IDC_ASSETDLG_ADDPOS_BUTTON)->EnableWindow(bEnable);
				if(m_PositionEdit.GetWindowTextLength() > 0)
				{
					GetDlgItem(IDC_ASSETDLG_UNWIND_BUTTON)->EnableWindow(bEnable);
					if(TransType == CALL || TransType == PUT)
						GetDlgItem(IDC_ASSETDLG_OPTEX_BUTTON)->EnableWindow(bEnable);
				}
			}
		}
	}
}

void CAssetDlg::EnableCtrls(BOOL bEnable)
{
	m_AssetDescEdit.EnableWindow(bEnable);
	m_CommonEdit.EnableWindow(bEnable);
	m_CusipEdit.EnableWindow(bEnable);
	m_IsinEdit.EnableWindow(bEnable);
	m_SedolEdit.EnableWindow(bEnable);
	m_StrikeEdit.EnableWindow(bEnable);

	m_CountryCombo.EnableWindow(bEnable);
	m_CurrencyCombo.EnableWindow(bEnable);
	m_ClassCombo.EnableWindow(bEnable);
	m_IndustryCombo.EnableWindow(bEnable);
	m_CategoryCombo.EnableWindow(bEnable);
	m_BucketCombo.EnableWindow(bEnable);
	m_TransTypeCombo.EnableWindow(bEnable);

	m_MaturityEdit.EnableWindow(bEnable);
	m_SourceEdit.EnableWindow(bEnable);
}

void CAssetDlg::EnableFind(CEdit& Edit, BOOL bAsset)
{
	if(Edit.GetModify() && Edit.GetWindowTextLength() > 0)
	{
		GetDlgItem(IDC_ASSETDLG_FIND_BUTTON)->EnableWindow(TRUE);
		SetControls(bAsset);
		Edit.SetModify(FALSE);
	}
}

void CAssetDlg::EnableFind(COptComboBox &Combo)
{
	if(m_AssetEdit.GetWindowTextLength() > 0)
		return;

	SetControls(FALSE);
	GetDlgItem(IDC_ASSETDLG_FIND_BUTTON)->EnableWindow(Combo.GetCurSel() >= 0 ? TRUE : FALSE);
}

void CAssetDlg::SetControls(BOOL bAsset)
{
	LPCTSTR p = EMPTYSTRING;
	
	if(m_AssetEdit.GetData() == NEWASSET)
		return;

	if(bAsset)
	{
		m_CommonEdit.SetData(p);
		m_CusipEdit.SetData(p);
		m_IsinEdit.SetData(p);
		m_SedolEdit.SetData(p);
		m_ClassCombo.SetCurSel(-1);
		m_CountryCombo.SetCurSel(-1);
		m_CurrencyCombo.SetCurSel(-1);
		m_IndustryCombo.SetCurSel(-1);
		m_BucketCombo.SetCurSel(-1);
		m_CategoryCombo.SetCurSel(-1);
		m_TransTypeCombo.SetCurSel(-1);
		m_DirCombo.SetCurSel(-1);
		m_MaturityEdit.SetData(p);
		m_SourceEdit.SetData(p);
		m_PositionEdit.SetData(p);
		m_StrikeEdit.SetData(p);
	}
	else
	{
		m_AssetEdit.SetData(p);
		m_AssetDescEdit.SetData(p);		
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAssetDlg message handlers
BEGIN_EVENTSINK_MAP(CAssetDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CAllocation)
	ON_EVENT(CAssetDlg, IDC_ASSETDLG_ITEM_LIST, 11 /* DblClick */, OnDblClickItemList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
	ON_EVENT(CAssetDlg, IDC_ASSETDLG_HOLDING_LIST, 11, CAssetDlg::OnDblClickAssetdlgHoldingList, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

BOOL CAssetDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	CQData QData;

	BeginWaitCursor();
	m_SS.SetVisibleCols(10);
	m_SS.SetVisibleRows(8);

	m_HoldingSS.SetVisibleCols(7);
	m_HoldingSS.SetVisibleRows(4);

	m_ClassCombo.Setup(this, IDC_ASSETDLG_CLASS_COMBO, TRUE);
	m_CountryCombo.Setup(this, IDC_ASSETDLG_COUNTRY_COMBO, TRUE);
	m_CurrencyCombo.Setup(this, IDC_ASSETDLG_CURRENCY_COMBO, TRUE);
	m_IndustryCombo.Setup(this, IDC_ASSETDLG_INDUSTRY_COMBO, TRUE);
	m_BucketCombo.Setup(this, IDC_ASSETDLG_BUCKET_COMBO, TRUE);
	m_CategoryCombo.Setup(this, IDC_ASSETDLG_CATEGORY_COMBO, TRUE);
	m_TransTypeCombo.Setup(this, IDC_ASSETDLG_TRANSTYPE_COMBO, TRUE);
	m_DirCombo.Setup(this, IDC_ASSETDLG_DIR_COMBO, TRUE);

	m_AssetEdit.Setup(this, IDC_ASSETDLG_ASSET_EDIT);
	m_AssetEdit.LimitText(10);
	m_AssetDescEdit.Setup(this, IDC_ASSETDLG_DESC_EDIT);
	m_AssetDescEdit.LimitText(40);
	m_AssetDesc2Edit.Setup(this, IDC_ASSETDLG_DESC2_EDIT);
	m_AssetDesc2Edit.LimitText(40);
	m_AssetDesc2Edit.SetReadOnly();
	m_CommonEdit.Setup(this, IDC_ASSETDLG_COMMON_EDIT);
	m_CommonEdit.LimitText(13);
	m_CusipEdit.Setup(this, IDC_ASSETDLG_MSTC_EDIT);
	m_CusipEdit.LimitText(10);
	m_IsinEdit.Setup(this, IDC_ASSETDLG_ISIN_EDIT);
	m_IsinEdit.LimitText(12);
	m_SedolEdit.Setup(this, IDC_ASSETDLG_SEDOL_EDIT);
	m_SedolEdit.LimitText(10);
	m_PID.Setup(this, IDC_ASSETDLG_PID_EDIT);
	m_PID.LimitText(25);
	m_MaturityEdit.Setup(this, IDC_ASSETDLG_MATURITY_EDIT);
	m_SourceEdit.Setup(this, IDC_ASSETDLG_SOURCE_EDIT);
	m_SourceEdit.LimitText(20);
	m_Underline.Setup(this, IDC_ASSETDLG_UNDERLINE_EDIT);
	m_UnderlineID.Setup(this, IDC_ASSETDLG_UNDERLINEID_EDIT);
	m_PositionEdit.Setup(this, IDC_ASSETDLG_POSITION_EDIT);
	m_ItemsEdit.Setup(this, IDC_ASSETDLG_ITEM_EDIT);
	m_ParEdit.Setup(this, IDC_ASSETDLG_PAR_EDIT);
	m_StrikeEdit.Setup(this, IDC_ASSETDLG_STRIKE_EDIT);
	m_TicketEdit.Setup(this, IDC_ASSETDLG_TICKET_EDIT);

	m_FindData.Setup(GetData().GetOraLoader(), &m_SS, m_bWithNew);
	m_FindData.Add(&m_AssetEdit, &m_FindData.GetRec().GetAsset());
	m_FindData.Add(&m_AssetDescEdit, &m_FindData.GetRec().GetDesc());
	m_FindData.Add(&m_AssetDesc2Edit, &m_FindData.GetRec().GetDesc2());
	m_FindData.Add(&m_CountryCombo, &m_FindData.GetRec().GetCountry());
	m_FindData.Add(&m_CurrencyCombo, &m_FindData.GetRec().GetCurrency());
	m_FindData.Add(&m_TransTypeCombo, &m_FindData.GetTransType());
	m_FindData.Add(&m_StrikeEdit, &m_FindData.GetStrike());
	m_FindData.Add(&m_MaturityEdit, &m_FindData.GetRec().GetMaturity());
	m_FindData.Add(&m_CategoryCombo, &m_FindData.GetRec().GetCategory());
	m_FindData.Add(&m_ClassCombo, &m_FindData.GetRec().GetClass());
	m_FindData.Add(&m_BucketCombo, &m_FindData.GetRec().GetBucket());
	m_FindData.Add(&m_IndustryCombo, &m_FindData.GetRec().GetIndustry());
	m_FindData.Add(&m_CommonEdit, &m_FindData.GetRec().GetCommon());
	m_FindData.Add(&m_CusipEdit, &m_FindData.GetRec().GetCusip());
	m_FindData.Add(&m_IsinEdit, &m_FindData.GetRec().GetIsin());
	m_FindData.Add(&m_SedolEdit, &m_FindData.GetRec().GetSedol());
	m_FindData.Add(&m_PID, &m_FindData.GetRec().GetPID());
	m_FindData.Add(&m_SourceEdit, &m_FindData.GetRec().GetSource());
	m_FindData.Add(&m_ParEdit, &m_FindData.GetRec().GetPar());
	m_FindData.Add(&m_TicketEdit, &m_FindData.GetTicket());
	m_FindData.Add(&m_Underline);
	m_FindData.Add(&m_UnderlineID);
	m_FindData.Add(&m_DirCombo, &m_FindData.GetDir());
	m_FindData.Add(&m_PositionEdit, &m_FindData.GetNominal());
	m_FindData.Add(&m_Future);
	m_FindData.Add(&m_EuropeBond);
	m_FindData.Add(&m_FindData.GetOptID());
	m_FindData.Add(&m_FindData.GetOptTick());
	m_FindData.Add(&m_FindData.GetDeliveryDate());
	m_FindData.Add(&m_FindData.GetOptSetCode());
	m_FindData.Add(&m_FindData.GetAssignCP());
	m_FindData.Add(&m_FindData.GetTrader());
	m_FindData.Add(&m_OptListed);
	m_FindData.Add(&m_Rev);

	GetData().LoadAssetData();
	GetData().GetTransTypeArr().CopyToComboBox(m_TransTypeCombo);
	GetData().GetTransDirArr().CopyToComboBox(m_DirCombo);
	GetData().GetAssetTypeArr().CopyToComboBox(m_ClassCombo);
	GetData().GetCountryArr().CopyToComboBox(m_CountryCombo);
	GetData().GetCurrencyArr().CopyToComboBox(m_CurrencyCombo);
	GetData().GetIndustryArr().CopyToComboBox(m_IndustryCombo);
	GetData().GetBucketArr().CopyToComboBox(m_BucketCombo, 1);
	GetData().GetCategoryArr().CopyToComboBox(m_CategoryCombo);

	if(m_FindData.GetRec().GetAsset().IsEmpty())
		m_FindData.GetRec().SetAsset(NEWASSET);

	if(m_FindData.GetRec().GetAsset() == NEWASSET)
	{
		m_AssetEdit.SetData(EMPTYSTRING);
		m_AssetDescEdit.SetData(EMPTYSTRING);
	}
	else
	{
		m_AssetEdit.SetData(m_FindData.GetRec().GetAsset());
		m_AssetDescEdit.SetData(m_FindData.GetRec().GetDesc());
	}
 
	m_FindData.LoadDBData(TRUE);
	
	m_ItemsEdit.SetData(QData.WriteNumber(m_SS.GetSheetRows()));

	GetDlgItem(IDC_ASSETDLG_FIND_BUTTON)->EnableWindow(FALSE);
	
	EnableButtons(FALSE);

	EnableCtrls(m_SS.GetSheetRows() <= 1 ? TRUE : FALSE);	
	EndWaitCursor();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAssetDlg::OnFindButton()
{
	BeginWaitCursor();
	m_FindData.UpdateData();
	m_FindData.LoadDBData();
	m_ItemsEdit.SetData(m_SS.GetSheetRows());
	
	if(m_SS.GetSheetRows() <= 0)
	{
		MessageBox("No records found");
		return;
	}

	if(m_SS.GetSheetRows() == 1)
		OnDblClickItemList(1, 1);		

	EndWaitCursor();
}

void CAssetDlg::OnOK()
{
	CString Asset, Text;
	CDBRec Rec;

	m_FindData.UpdateData();
	if(!m_FindData.IsRowOK())
		return;

	Asset = m_FindData.GetRec().GetAsset();

	if(!m_bCash)
	{
		if(!(Asset == "O/N INTRST" || Asset == "ERRORENTRY"))
		{
			Text = m_FindData.GetRec().GetBucket();

			if(Text.IsEmpty())
				return;

			int i;
			i = GetData().GetBucketArr().Find(Text, 1);
			if(i >= 0)
				Text = GetData().GetBucketArr().GetAt(i)->GetAt(0);
			else
				Text.Empty();
		
			if(Text.IsEmpty())
				return;
		
			m_FindData.GetRec().SetBucket(Text);
			m_FindData.LoadCustodian(Asset);
		}
	}
	
	CDialog::OnOK();
}

void CAssetDlg::OnDblClickItemList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		m_FindData.SetCurrentRow(Row);
		
		m_FindData.LoadHoldings(m_AssetEdit.GetData(), m_HoldingSS);
	}
	else
		m_FindData.SetCurrentRow(-1);
	
	EnableButtons(m_SS.GetSheetCurRow() > 0);
}

void CAssetDlg::OnDblClickAssetdlgHoldingList(long Col, long Row)
{
	if(Row > 0 && Row <= m_HoldingSS.GetSheetRows())
		m_TicketEdit.SetData(m_HoldingSS.GetSheetText(4, Row));
	else
		m_TicketEdit.SetData(EMPTYSTRING);
}

void CAssetDlg::OnRefreshButton()
{
	m_FindData.Refresh();
	GetDlgItem(IDOK)->EnableWindow(FALSE);

	EnableCtrls(TRUE);
}

void CAssetDlg::OnClose()
{
	OnOK();
}

void CAssetDlg::OnSelchangeAssetBucketCombo() 
{
	EnableFind(m_BucketCombo);
}

void CAssetDlg::OnSelchangeAssetCategoryCombo() 
{
	EnableFind(m_CategoryCombo);
}

void CAssetDlg::OnSelchangeAssetIndustryCombo() 
{
	EnableFind(m_IndustryCombo);
}

void CAssetDlg::OnSelchangeAssetClassCombo() 
{
	EnableFind(m_ClassCombo);
}

void CAssetDlg::OnSelchangeAssetCountryCombo() 
{
	EnableFind(m_CountryCombo);
}

void CAssetDlg::OnSelchangeAssetCurrencyCombo() 
{
	EnableFind(m_CurrencyCombo);
}

void CAssetDlg::OnChangeAssetdescEdit()
{
	EnableFind(m_AssetDescEdit, TRUE);
}

void CAssetDlg::OnChangeAssetCommonEdit() 
{
	EnableFind(m_CommonEdit);
}

void CAssetDlg::OnChangeAssetIsinEdit() 
{
	EnableFind(m_IsinEdit);
}

void CAssetDlg::OnChangeAssetMstcEdit() 
{
	EnableFind(m_CusipEdit);
}

void CAssetDlg::OnChangeAssetSedolEdit() 
{
	EnableFind(m_SedolEdit);
}

void CAssetDlg::OnChangeAssetMaturityEdit() 
{
	EnableFind(m_MaturityEdit);
}

void CAssetDlg::OnChangeAssetSourceEdit() 
{
	EnableFind(m_SourceEdit);
}

void CAssetDlg::OnBnClickedAssetdlgAddposButton()
{
	m_nActionID = FINDACTION_ADD;
	OnOK();
}

void CAssetDlg::OnBnClickedAssetdlgUnwindButton()
{
	m_nActionID = FINDACTION_UNWIND;
	OnOK();
}

void CAssetDlg::OnBnClickedAssetdlgOptexButton()
{
	m_nActionID = FINDACTION_OPTEX;
	OnOK();
}


void CAssetDlg::OnEnChangeAssetdlgAssetEdit()
{
	if(m_AssetEdit.GetModify())
		EnableButtons(FALSE);

	EnableFind(m_AssetEdit, TRUE);
}
