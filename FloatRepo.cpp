// inrepo.cpp : implementation file
//
#include "stdafx.h"
#include "nwifms70.h"
#include "floatrepodata.h"
#include "floatrepo.h"
#include "qdata.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFloatRepo dialog
CFloatRepo::CFloatRepo(CWnd *pParent /*=NULL*/)
	: CDialog(CFloatRepo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFloatRepo)
	m_CP = NULL;
	m_Date = NULL;
	//}}AFX_DATA_INIT
}

CFloatRepo::~CFloatRepo()
{
	if(m_CP)
		delete m_CP;
	if(m_Date)
		delete m_Date;
}

void CFloatRepo::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFloatRepo)
	DDX_Control(pDX, IDC_REPO_LIST, m_SS);
	DDX_Control(pDX, IDC_REPO_DETAIL_LIST, m_DetailSS);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFloatRepo, CDialog)
	//{{AFX_MSG_MAP(CFloatRepo)
	ON_BN_CLICKED(IDC_REPO_ADD, OnAddButton)
	ON_BN_CLICKED(IDC_REPO_UPDATE, OnUpdateButton)
	ON_BN_CLICKED(IDC_REPO_DELETE, OnDeleteButton)
	ON_CBN_SELCHANGE(IDC_REPO_DATE_COMBO, OnSelchangeDateCombo)
	ON_CBN_SELCHANGE(IDC_REPO_COUNTERPARTY_COMBO, OnSelchangeCounterpartyCombo)
	ON_BN_CLICKED(IDC_REPO_DEF, OnRepoDef)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CFloatRepo::ComputeInterest()
{
	if(!IsOK(FALSE))
		return;

	if(m_ToDate.GetData().IsEmpty())
	{
		m_Interest.SetWindowText("0.00");
		m_Days.SetWindowText("0");
	}
	else
	{
		CString IntFrom, IntTo, Rate;
		CQData QData;

		IntFrom = m_FromDate.GetData();
		IntTo = m_ToDate.GetData();
		m_Rate.GetWindowText(Rate);
	
		m_Data.ComputeInterest(IntFrom, IntTo, Rate);
		m_Interest.SetWindowText(QData.WriteNumber(m_Data.GetInterest(), TRUE, 2));
		m_Days.SetWindowText(QData.WriteNumber(m_Data.GetVal().GetLevDays()));
	}
}

BOOL CFloatRepo::IsOK(BOOL bShow)
{
	if(m_FromDate.GetData().IsEmpty())
	{
		if(bShow)
			MessageBox("No FromDate Available");
		return FALSE;
	}

	if(m_Rate.GetWindowTextLength() <= 0)
	{
		if(bShow)
			MessageBox("No Rate Available");
		return FALSE;
	}
	
	return TRUE;
}

void CFloatRepo::AddDates()
{
	SetDates();
	if(m_Date->GetCurSel() < 0)
	{
		m_Date->InsertString(0, m_Dates);
		m_Date->SelectString(-1, m_Dates);
	}
}

void CFloatRepo::SetDates()
{
	CString ToDate;

	m_Dates = m_FromDate.GetData();
	ToDate = m_ToDate.GetData();
	m_Dates += MINUS + ToDate;
	m_Date->SelectString(-1, m_Dates);
}

void CFloatRepo::SetTotal()
{
	CQData QData;
	
	m_Data.ComputeTotalInterest();
	m_TotalInterest.SetWindowText(QData.WriteNumber(m_Data.GetTotalInterest(), TRUE, 2));
	m_Total.SetWindowText(QData.WriteNumber(m_Data.GetTotalInterest() + m_Data.GetRec().GetAmountVal(), TRUE, 2));
}

void CFloatRepo::UpdateDates()
{
	CString Text, FromDate;

	m_Date->GetSelString(Text);
	FromDate = Text.Left(10);
	Text = Text.Right(10);

	m_FromDate.SetData(FromDate);
	m_ToDate.SetData(Text);
}
/////////////////////////////////////////////////////////////////////////////
// CFloatRepo message handlers
BOOL CFloatRepo::OnInitDialog()
{
	CDialog::OnInitDialog();
	    
	BeginWaitCursor();

	m_SS.SetVisibleRows(6);
	m_DetailSS.SetVisibleRows(6);

    m_CP = (COptComboBox*) new COptComboBox(this, IDC_REPO_COUNTERPARTY_COMBO);
    m_Date = (COptComboBox*) new COptComboBox(this, IDC_REPO_DATE_COMBO);

	m_FromDate.Setup(this, IDC_REPO_FROMDATE_EDIT);
	m_ToDate.Setup(this, IDC_REPO_TODATE_EDIT);
	m_Days.Setup(this, IDC_REPO_DAYS_EDIT);

	m_TransNum.Setup(this, IDC_REPO_TRANS_NUM_EDIT);
	m_Portfolio.Setup(this, IDC_REPO_PORTFOLIO_EDIT);
	m_NomAmount.Setup(this, IDC_REPO_NOMINAL_EDIT);
	m_Currency.Setup(this, IDC_REPO_CURRENCY_EDIT);
	m_Price.Setup(this, IDC_REPO_PRICE_EDIT);
	m_RateBasis.Setup(this, IDC_REPO_RATE_BASIS_EDIT);
	m_ValueDate.Setup(this, IDC_REPO_VALUE_DATE_EDIT);
	m_Maturity.Setup(this, IDC_REPO_MATURITY_EDIT);
	m_AssetCode.Setup(this, IDC_REPO_ASSETCODE_EDIT);
	m_Asset.Setup(this, IDC_REPO_ASSET_EDIT);
	m_IntRate.Setup(this, IDC_REPO_INT_RATE_EDIT);
	m_Amort.Setup(this, IDC_REPO_AMORT_FACT_EDIT);
	m_CapRate.Setup(this, IDC_REPO_CAP_RATE_EDIT);
	m_Plus.Setup(this, IDC_REPO_PLUS_AMOUNT_EDIT);
	m_Amount.Setup(this, IDC_REPO_AMOUNT_EDIT, "Invalid Number", 2);

	m_Rate.Setup(this, IDC_REPO_RATE_EDIT);
	m_Interest.Setup(this, IDC_REPO_INTEREST_EDIT, "Invalid Number", 2);
	m_TotalInterest.Setup(this, IDC_REPO_TOTAL_INTEREST_EDIT, "Invalid Number", 2);
	m_Total.Setup(this, IDC_REPO_TOTAL_EDIT, "Invalid Number", 2);

	m_Data.Add(&m_Portfolio, &m_Data.GetRec().GetPortfolio());
	m_Data.Add(&m_Asset, &m_Data.GetRec().GetAsset());
	m_Data.Add(&m_TransNum, &m_Data.GetRec().GetTransNum());
	m_Data.Add(&m_AssetCode, &m_Data.GetRec().GetAssetCode());
	m_Data.Add(&m_NomAmount, &m_Data.GetRec().GetNomAmount());
	m_Data.Add(&m_Currency, &m_Data.GetRec().GetCurrency());
	m_Data.Add(&m_Price, &m_Data.GetRec().GetPrice());
	m_Data.Add(&m_RateBasis, &m_Data.GetRec().GetRateBasis());
	m_Data.Add(&m_ValueDate, &m_Data.GetRec().GetValueDate());
	m_Data.Add(&m_Maturity, &m_Data.GetRec().GetMaturity());
	m_Data.Add(&m_Data.GetRec().GetAssFrom());
	m_Data.Add(&m_Data.GetRec().GetAssRateBasis());
	m_Data.Add(&m_Data.GetRec().GetAssetType());
	m_Data.Add(&m_Data.GetRec().GetTransType());
	m_Data.Add(&m_IntRate, &m_Data.GetRec().GetIntRate());
	m_Data.Add(&m_Amort, &m_Data.GetRec().GetAmort());
	m_Data.Add(&m_CapRate, &m_Data.GetRec().GetCapRate());
	m_Data.Add(&m_Plus, &m_Data.GetRec().GetPlus());
	m_Data.Add(&m_Data.GetRec().GetAADays());
	m_Data.Add(&m_Data.GetRec().GetFormula());
	m_Data.Add(&m_Amount, &m_Data.GetRec().GetAmount());
	
	m_Data.GetSRowCtrl().Add(&m_FromDate, &m_Data.GetInvRec().GetFromDate());
	m_Data.GetSRowCtrl().Add(&m_ToDate, &m_Data.GetInvRec().GetToDate());
	m_Data.GetSRowCtrl().Add(&m_Rate, &m_Data.GetInvRec().GetRate());
	m_Data.GetSRowCtrl().Add(&m_Days, &m_Data.GetInvRec().GetDays());
	m_Data.GetSRowCtrl().Add(&m_Interest, &m_Data.GetInvRec().GetInterest());

	m_Data.Setup(GetData()->GetOraLoader(), &m_SS, &m_DetailSS, 0);
	m_Data.LoadCP(*m_CP);
	m_Data.LoadDate(*m_Date);
	GetDlgItem(IDC_REPO_ADD)->EnableWindow(FALSE);
	GetDlgItem(IDC_REPO_DELETE)->EnableWindow(FALSE);
	GetDlgItem(IDC_REPO_UPDATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_REPO_DEF)->EnableWindow(FALSE);
	EndWaitCursor();
	return TRUE;
}

void CFloatRepo::OnSelchangeCounterpartyCombo()
{
	CString CP;

	m_CP->GetSelString(CP);
	if(CP != m_Data.GetCP())
	{
		BeginWaitCursor();
		m_Data.SetCP(CP);
		m_Data.SetCurrentRow(-1);
		m_Data.LoadTickets();
		GetDlgItem(IDC_REPO_ADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_REPO_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_REPO_UPDATE)->EnableWindow(FALSE);
		EndWaitCursor();
	}

	GetDlgItem(IDC_REPO_DEF)->EnableWindow(!CP.IsEmpty());
}

void CFloatRepo::OnSelchangeDateCombo()
{
	UpdateDates();
	ComputeInterest();
}

void CFloatRepo::OnAddButton()
{
	if(IsOK() && m_Data.AddSRow())
	{		
		AddDates();
		SetTotal();
	}
}

void CFloatRepo::OnUpdateButton()
{
	if(IsOK() && m_Data.UpdateSRow())
	{
		AddDates();
		OnDblClickRepoList(0, m_Data.GetCurrentRow());
	}
}

void CFloatRepo::OnDeleteButton()
{
	if(MessageBox("Are you sure you want to delete this entry ?", NULL, MB_YESNO) == IDYES)
		if(m_Data.DeleteSRow())
			SetTotal();
}

BEGIN_EVENTSINK_MAP(CFloatRepo, CDialog)
    //{{AFX_EVENTSINK_MAP(CFloatRepo)
	ON_EVENT(CFloatRepo, IDC_REPO_LIST, 11 /* DblClick */, OnDblClickRepoList, VTS_I4 VTS_I4)
	ON_EVENT(CFloatRepo, IDC_REPO_DETAIL_LIST, 11 /* DblClick */, OnDblClickRepoDetailList, VTS_I4 VTS_I4)
	ON_EVENT(CFloatRepo, IDC_REPO_DETAIL_LIST, 2 /* BlockSelected */, OnBlockSelectedRepoDetailList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CFloatRepo, IDC_REPO_LIST, 2 /* BlockSelected */, OnBlockSelectedRepoList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CFloatRepo::OnDblClickRepoList(long Col, long Row)
{
	m_Data.SetCurrentRow(-1);
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		m_Data.SetCurrentRow(Row);
		SetTotal();
	}

	GetDlgItem(IDC_REPO_ADD)->EnableWindow(m_SS.GetSheetCurRow() > 0 ? TRUE : FALSE);
}

void CFloatRepo::OnDblClickRepoDetailList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_DetailSS.GetSheetRows())
	{
		m_Data.GetSRowCtrl().SetCurrentRow(Row);
		SetDates();
	}
	else
	{
		m_Data.GetSRowCtrl().SetCurrentRow(-1);
		m_Data.GetSRowCtrl().Refresh();
		m_Date->SetCurSel(-1);
	}
	
	GetDlgItem(IDC_REPO_UPDATE)->EnableWindow(m_DetailSS.GetSheetCurRow() > 0 ? TRUE : FALSE);
}

void CFloatRepo::OnBlockSelectedRepoDetailList(long BlockCol1, long BlockRow1, long BlockCol2, long BlockRow2) 
{
	OnDblClickRepoDetailList(m_DetailSS.GetActiveCol(), m_DetailSS.GetActiveRow());
	GetDlgItem(IDC_REPO_DELETE)->EnableWindow(m_DetailSS.GetSheetCurRow() > 0 ? TRUE : FALSE);
}

void CFloatRepo::OnBlockSelectedRepoList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	if(BlockRow == 0 && BlockCol > 0)
	{
		BeginWaitCursor();
		m_Data.LoadTickets(BlockCol);
		EndWaitCursor();
	}
}

void CFloatRepo::OnRepoDef() 
{
	if(m_CP->GetCurSel() >= 0)
	{
		BeginWaitCursor();
		if(m_SS.GetOperationMode() == 0)
			m_SS.ClearSelection();
//			m_SS.SetAction(SS_ACTION_DESELECT_BLOCK);
		else 
			m_SS.SetSelModeSelected(FALSE);
//			m_SS.SetAction(SS_ACTION_SEL_MODE_CLEAR);

		m_SS.SetBlockMode(FALSE);
		m_Data.SetCurrentRow(-1);
		m_Data.LoadTickets();
		GetDlgItem(IDC_REPO_ADD)->EnableWindow(FALSE);
		GetDlgItem(IDC_REPO_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_REPO_UPDATE)->EnableWindow(FALSE);
		EndWaitCursor();
	}
}
