// Allocation.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include <math.h>
#include <stdlib.h>
#include "Allocation.h"
#include "finddlg.h"
#include "qdata.h"
#include "nwivar.h"
#include "invert.h"
#include "repoprice.h"
#include "contractconvertor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAllocation dialog
CAllocation::CAllocation(CWnd *pParent /*=NULL*/)
	: CDialog(CAllocation::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAllocation)
	//}}AFX_DATA_INIT
}

void CAllocation::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAllocation)
	DDX_Control(pDX, IDC_ALLOC_LIST, m_SS);
	DDX_Control(pDX, IDC_ALLOC_REPOINFO_LIST, m_RepoInfoSS);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAllocation, CDialog)
	//{{AFX_MSG_MAP(CAllocation)
	ON_BN_CLICKED(IDC_ALLOC_ADD_BUTTON, OnAddButton)
	ON_BN_CLICKED(IDC_ALLOC_DELETE_BUTTON, OnDeleteButton)
	ON_BN_CLICKED(IDC_ALLOC_REPO_BUTTON, OnRepoButton)
	ON_CBN_SELCHANGE(IDC_ALLOC_PORTFOLIO_COMBO, OnSelchangePortfolioCombo)
	ON_CBN_SELCHANGE(IDC_ALLOC_ACCOUNT_COMBO, OnSelchangeAccountCombo)
	ON_BN_CLICKED(IDC_ALLOC_UPDATE_BUTTON, OnUpdateButton)
	ON_EN_KILLFOCUS(IDC_ALLOC_NOMAMOUNT_EDIT, OnKillfocusNomamountEdit)
	ON_EN_KILLFOCUS(IDC_ALLOC_GROSS_PRICE_EDIT, OnKillfocusPriceEdit)
	ON_BN_CLICKED(IDC_ALLOC_FINDITEM_BUTTON, OnAllocFinditemButton)
	ON_BN_CLICKED(IDC_ALLOC_INVERTPRICE_BUTTON, OnAllocInvertPriceButton)
	ON_BN_CLICKED(IDC_ALLOC_FINDITEM2_BUTTON, OnAllocFinditem2Button)
	ON_BN_CLICKED(IDC_ALLOC_CONTRACT_CONVERTOR_BUTTON, OnAllocContractConvertorButton)
	ON_EN_SETFOCUS(IDC_ALLOC_NOMAMOUNT_EDIT, OnSetfocusAllocNomamountEdit)
	ON_BN_CLICKED(IDC_ALLOC_REPOPRICE_BUTTON, OnAllocRepopriceButton)
	//}}AFX_MSG_MAP
	ON_CBN_KILLFOCUS(IDC_ALLOC_PORTFOLIO_COMBO, &CAllocation::OnCbnKillfocusAllocPortfolioCombo)
END_MESSAGE_MAP()

void CAllocation::EnableCtrls()
{
	BOOL bEnable;

	bEnable = m_Portfolio.GetCurSel() >= 0 ? TRUE : FALSE;

	m_Amount.EnableWindow(bEnable);
	if(!bEnable)
		return;

	CString TransType, Dir;

	TransType = m_Data.GetTicket().GetTransType();
	Dir = m_Data.GetTicket().GetDir();

	m_Price.EnableWindow((TransType == INTSWAP || TransType == FOREX) ? FALSE : TRUE);	
}

BOOL CAllocation::IsOK()
{
	double Amount = 0;
	COraLoader OraLoader;
	CQData QData;
	CString Text, TransType, Portfolio, Asset;
	CRawInvRec RecInvRec;

	if(m_Portfolio.GetCurSel() < 0)
		Text = "Must select a portfolio";

	TransType = m_Data.GetTicket().GetTransType();
	if(TransType != INTSWAP && TransType != INTSWAP && TransType != FOREX)
	{
		if(m_Price.GetWindowTextLength() <= 0)
			Text = "Price must be entered";
	}

	if(!(TransType == INTSWAP || TransType == CASH || TransType == INTSWAP || TransType == FOREX))
	{
		if(m_Custodian.GetCurSel() < 0)
			Text = "Must Select a Custodian";
	}

	if(m_Account.GetCurSel() < 0)
		Text = "Must select an account";

	if(!Text.IsEmpty())
	{
		MessageBox(Text, "Allocation");
		return FALSE;
	}
	
	// Risk Control
	Text.Empty();
	for(int i = 1; i <= m_SS.GetSheetRows(); i ++)
	{
		Portfolio = m_SS.GetSheetText(1, i);
		if(Portfolio == "BNAMTR")
		{
			switch(VerifyRisk(Text, Portfolio))
			{
				case 1:
					MessageBox(Text, "Allocation");
					return TRUE;
				case 3:
					MessageBox(Text, "Allocation");
					return FALSE;
				default:
					return TRUE;
			}
		}
	}

	return TRUE;
}

void CAllocation::LoadRepoInfo()
{
	m_RepoInfoSS.ClearSheet();

	if(m_TransType == REPO)
	{
		CQData QData;
		CString Portfolio, Asset;

		BeginWaitCursor();
		Portfolio = QData.GetQueryText(m_Portfolio.GetData());
		Asset = QData.GetQueryText(m_Data.GetTicket().GetAsset());
		
		GetData().GetOraLoader().GetSql().Format("SELECT PORTFOLIO, ASSET_CODE, 'HOLDING' \"STATUS\", "
			"SUM(DECODE(TRANS_TYPE, 'SECURITIES', DECODE(DIR, 'P', 1, 'S', -1)*NOM_AMOUNT, 0)) \"SEC_HOLDING\", "
			"SUM(DECODE(TRANS_TYPE, 'REPO', DECODE(TR_DESC, 'MATURITY', 1, 0)*DECODE(DIR, 'P', 1, 'S', -1)*NOM_AMOUNT, 0)) \"REPO_HOLDING\", "
			"0 \"REPO_BOOKING\" "
			"FROM SEMAM.ALL_TICKET_INV_V A "
			"WHERE ASSET_CODE = %s "
			"AND PORTFOLIO = %s "
			"AND A.TRANS_TYPE IN ('SECURITIES', 'REPO') "
			"AND DECODE(TRANS_TYPE, 'SECURITIES', TRUNC(SYSDATE) + 1, 'REPO', "
				"NVL(ACTUAL_VDATE, NVL(MATURITY_DATE, TRUNC(SYSDATE) + 1))) > TRUNC(SYSDATE) "
			"GROUP BY PORTFOLIO, ASSET_CODE "
			"UNION "
			"SELECT PORTFOLIO, ASSET_CODE, 'OPEN TRADE', "
			"SUM(DECODE(TRANS_TYPE, 'SECURITIES', DECODE(DIR, 'P', 1, 'S', -1)*NOM_AMOUNT, 0)), "
			"SUM(DECODE(TRANS_TYPE, 'REPO', DECODE(TR_DESC, 'MATURITY', 1, 0)*DECODE(DIR, 'P', 1, 'S', -1)*NOM_AMOUNT, 0)), "
			"SUM(DECODE(TRANS_TYPE, 'REPO', DECODE(TR_DESC, 'BOOKING', 1, 0)*DECODE(DIR, 'P', 1, 'S', -1)*NOM_AMOUNT, 0)) "
			"FROM SEMAM.ALL_TICKET_INV_V A "
			"WHERE ASSET_CODE = %s "
			"AND PORTFOLIO = %s "
			"AND A.TRANS_TYPE IN ('SECURITIES', 'REPO') "
			"AND NVL(ACTUAL_CLOSING, TRUNC(SYSDATE) + 1) > TRUNC(SYSDATE) "
			"GROUP BY PORTFOLIO, ASSET_CODE "
			"ORDER BY 1, 2, 3 ", (LPCTSTR) Asset, (LPCTSTR) Portfolio, (LPCTSTR) Asset, (LPCTSTR) Portfolio);

		if(GetData().GetOraLoader().Open())
			GetData().GetOraLoader().LoadDBSheet(m_RepoInfoSS);
		EndWaitCursor();
	}
}

void CAllocation::ProcessVerifyRisk()
{
	CString Text;

	m_nRiskLevel = VerifyRisk(Text, m_Portfolio.GetData());
	if(!Text.IsEmpty() && !m_bWarned)
	{
		MessageBox(Text, "Allocation");
		m_bWarned = TRUE;
	}
}

void CAllocation::Refresh()
{
	m_Portfolio.SetCurSel(-1);
	OnSelchangePortfolioCombo();
}

void CAllocation::SetDefInfo(LPCTSTR Portfolio)
{
	BOOL bCP = TRUE;
	CString AssignCP;

	if(m_PFU == "P")
		bCP = FALSE;
	else
		if(m_TransType == SECURITIES)
		{
			if(m_AssetClass == "EURO BONDS" || m_AssetClass == "BRADY BONDS" || 
				m_AssetClass == "DOM EQUITIES" || m_AssetClass == "ADR EQUITIES" ||
				m_AssetClass == "DOM EQTY-NEW" || m_AssetClass == "WARRENTS" ||
				m_AssetClass == "CONVERTIBLE" || m_AssetClass == "FIXED INCOME" ||
				m_AssetClass == "US FIXED INCOME" || m_AssetClass == "SOVEREIGN" ||
				m_AssetClass == "CORPORATE" || m_AssetClass == "EM BONDS" ||
				m_AssetClass == "G7 BONDS" || m_AssetClass == "EXCH TRADE FUND")
				bCP = FALSE;
		}
		else
			if(m_TransType == CALL || m_TransType == PUT)
			{
				if(m_Data.GetTicket().GetListed() == "Y" || !m_Data.GetTicket().GetOptTicker().IsEmpty())
					bCP = FALSE;
			}

	if(!m_Data.GetTicket().GetAssignCP().IsEmpty())
	{
		m_Custodian.SetData(m_Data.GetTicket().GetAssignCP());
		m_Account.SetData(m_Data.GetTicket().GetAssignCP());
	}
	else
	{
		if(bCP)
			m_Custodian.SetData(m_Data.GetTicket().GetCP());
		else
		{	
			GetData().GetPortfolioArr().SetPortfolio(Portfolio);
			m_Custodian.SetData(GetData().GetPortfolioArr().GetCustodian());		
			m_Account.SetData(GetData().GetPortfolioArr().GetAccount());
		}
	}

	OnSelchangeAccountCombo();
}

void CAllocation::UpdateButtons()
{
	GetDlgItem(IDC_ALLOC_ADD_BUTTON)->EnableWindow(m_Amount.GetWindowTextLength() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_ALLOC_UPDATE_BUTTON)->EnableWindow(m_SS.GetSheetCurRow() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_ALLOC_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected() && m_SS.GetSheetCurRow() >= 1 ? TRUE : FALSE);
	GetDlgItem(IDC_ALLOC_FINDITEM_BUTTON)->EnableWindow(m_Portfolio.GetCurSel() > 0);
	GetDlgItem(IDOK)->EnableWindow(m_SS.GetSheetRows() > 0 ? TRUE : FALSE);
}

BOOL CAllocation::UpdateData(BOOL bAdd)
{
	if(!IsOK())
		return FALSE;

	CDBRec Rec;
	m_Data.UpdateData();
	m_Data.ToDBRec(Rec);

	if(bAdd)
		m_Data.AddSheetRow(Rec);
	else
		m_Data.UpdateSheetRow(FALSE);

	m_SS.LockSheet();
	GetDlgItem(IDOK)->EnableWindow(m_SS.GetSheetRows() > 0 ? TRUE : FALSE);
	
	return TRUE;
}

int CAllocation::VerifyRisk(CString &Text, const CString Portfolio)
{
	CString Nominal, Price;

	if(Portfolio != "BNAMTR"||m_Data.GetTicket().GetTRS() == "TRS")
		return 0;

	Nominal = m_Amount.GetData();
	Price = m_Price.GetData();

	if(m_Data.GetTicket().GetDir() == S)
		Nominal = "-" + Nominal;

	m_bWarned = FALSE;
	return m_Risk.VerifyRisk(Text, Portfolio, m_Data.GetTicket().GetTransType(), m_Data.GetTicket().GetValueDate(), 
							m_Data.GetTicket().GetCurrency(), Nominal, Price, m_Data.GetTicket().GetFxRate());
}
/////////////////////////////////////////////////////////////////////////////
// CAllocation message handlers

BEGIN_EVENTSINK_MAP(CAllocation, CDialog)
    //{{AFX_EVENTSINK_MAP(CAllocation)
	ON_EVENT(CAllocation, IDC_ALLOC_LIST, 11 /* DblClick */, OnDblClickAllocationList, VTS_I4 VTS_I4)
	ON_EVENT(CAllocation, IDC_ALLOC_LIST, 2 /* BlockSelected */, OnBlockSelectedAllocationList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


BOOL CAllocation::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CQData QData;
	CString Common, Cusip, Sedol, Isin;

	m_CSPBShort.Setup(this, IDC_ALLOC_CSPBSHORT_CHECK, Y);

	m_Portfolio.Setup(this, IDC_ALLOC_PORTFOLIO_COMBO);
	m_Custodian.Setup(this, IDC_ALLOC_CUSTODIAN_COMBO, TRUE);
	m_Account.Setup(this, IDC_ALLOC_ACCOUNT_COMBO, TRUE);

	m_Amount.Setup(this, IDC_ALLOC_NOMAMOUNT_EDIT);
	m_Price.Setup(this, IDC_ALLOC_GROSS_PRICE_EDIT);
	m_OtherFee.Setup(this, IDC_ALLOC_OTHER_FEE, "Invalid Number", 2);
	
	m_SS.SetVisibleCols(8);
	m_SS.SetVisibleRows(5);
	m_SS.SetSheetToFit(m_SS.GetVisibleCols(), m_SS.GetVisibleRows());

	m_RepoInfoSS.SetVisibleCols(6);
	m_RepoInfoSS.SetVisibleRows(5);

	m_Data.Add(&m_TransNum);
	m_Data.Add(&m_Portfolio, &m_Data.GetInv().GetPortfolio());
	m_Data.Add(&m_Amount, &m_Data.GetInv().GetNomAmount());
	m_Data.Add(&m_Custodian, &m_Data.GetInv().GetCustodian());
	m_Data.Add(&m_Account, &m_Data.GetInv().GetAccount());
	m_Data.Add(&m_Price, &m_Data.GetInv().GetPrice());
	m_Data.Add(&m_Data.GetInv().GetLink());
	m_Data.Add(&m_Data.GetInv().GetDownPymnt());
	m_Data.Add(&m_OtherFee, &m_Data.GetInv().GetOtherFee());
	m_Data.SetSS(&m_SS);
	m_Data.SetupAssetInfo();

	QData.CopyDBRecArrayToComboBox(GetData().GetPortfolioArr(), m_Portfolio, 0, FALSE);
	QData.CopyKeyDBListKeyToComboBox(GetData().GetContactList(), m_Custodian, FALSE);
	QData.CopyDBRecArrayToComboBox(GetData().GetAccountArr(), m_Account, 0, FALSE);

	GetDlgItem(IDC_ALLOC_REPO_BUTTON)->EnableWindow(m_Data.GetAutoRepo());
	SetDlgItemText(IDC_ALLOC_AVAIL_EDIT, m_Data.GetTicket().GetNomAmount());
	SetDlgItemText(IDC_ALLOC_ASSET_EDIT, m_Data.GetTicket().GetAsset());
	SetDlgItemText(IDC_ALLOC_ASSETDESC_EDIT, m_Data.GetTicket().GetAssetDesc());
	SetDlgItemText(IDC_ALLOC_TRANSTYPE_EDIT, m_Data.GetTicket().GetTransType());
	SetDlgItemText(IDC_ALLOC_DIR_EDIT, m_Data.GetTicket().GetDir());
	SetDlgItemText(IDC_ALLOC_COUNTERPARTY_EDIT, m_Data.GetTicket().GetCP());
	SetDlgItemText(IDC_ALLOC_FXRATE_EDIT, m_Data.GetTicket().GetFxRate());
	SetDlgItemText(IDC_ALLOC_RATE_EDIT, m_Data.GetTicket().GetRate());
	SetDlgItemText(IDC_ALLOC_MATURITY_EDIT, m_Data.GetTicket().GetMaturity());
	SetDlgItemText(IDC_ALLOC_CURRENCY_EDIT, m_Data.GetLocal() ? m_Data.GetTicket().GetCurrency() : USD);
	m_Price.SetData(m_Data.GetTicket().GetPrice());
	SetDlgItemText(IDC_ALLOC_DOWNPAY_EDIT, m_Data.GetTicket().GetDownPymnt());

	if(m_Data.LoadIDs(Common, Cusip, Sedol, Isin))
	{
		SetDlgItemText(IDC_ALLOC_COMMON_EDIT, Common);
		SetDlgItemText(IDC_ALLOC_CUSIP_EDIT, Cusip);
		SetDlgItemText(IDC_ALLOC_SEDOL_EDIT, Sedol);
		SetDlgItemText(IDC_ALLOC_ISIN_EDIT, Isin);
	}

	m_Data.GetPB() = m_Data.GetTicket().GetAssignCP();
	if(m_Data.GetPB().IsEmpty())
		m_Data.GetPB() = m_sCustodian;

	m_Data.LoadAlloc(m_Data, m_DownPay);
	
	UpdateButtons();
	EnableCtrls();
	Refresh();

	GetDlgItem(IDC_ALLOC_ALLOCATED_EDIT)->SetWindowText(QData.WriteNumber(m_Data.ComputeAllocAmount()));

	m_AssetClass = m_Data.GetTicket().GetAssetClass();

	GetDlgItem(IDC_ALLOC_CONTRACT_CONVERTOR_BUTTON)->EnableWindow(m_TransType == CALL || 
				m_TransType == PUT || m_TransType == SECURITIES ? TRUE : FALSE);

	m_Risk.Setup(GetData().GetOraLoader());

	m_Risk.LoadRisk(m_Data.GetTicket().GetAsset(), m_Data.GetTicket().GetTicket());
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAllocation::OnDblClickAllocationList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		m_Data.SetCurrentRow(Row);
		if(m_Data.GetAutoRepo())
		{
			CString CP;
			
			CP = m_Custodian.GetData();
			if(CP.IsEmpty())
			{
				if(!m_sCustodian.IsEmpty())
					m_Custodian.SetData(m_sCustodian);
				else
					m_Custodian.SetData(m_Data.GetTicket().GetCP());
			}
		}

		if(m_Custodian.GetCurSel() < 0 && m_Account.GetCurSel() < 0)
		{
			CString Portfolio;

			m_Portfolio.GetSelString(Portfolio);
			SetDefInfo(Portfolio);
		}

		EnableCtrls();
		UpdateButtons();
		Value();
	}
	else
	{
		m_Data.SetCurrentRow(-1);
		m_Data.Refresh();
	}

	LoadRepoInfo();
}

void CAllocation::OnBlockSelectedAllocationList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	m_SS.SetSheetCurRow(m_SS.GetActiveRow());
	UpdateButtons();
}

void CAllocation::OnKillfocusNomamountEdit() 
{
	if(m_Amount.GetWindowTextLength() > 0)
	{
		CString Portfolio;
		double NomAmount, Amount;
		CQData QData;

		Portfolio = m_Portfolio.GetData();
		NomAmount = atof(QData.RemoveComma(m_Amount.GetData()));

		Amount = m_Data.ComputeAllocAmount(Portfolio, NomAmount);
		if(m_Data.AllocCheckSum(Portfolio, NomAmount) < 0)		
		{
			MessageBox("Sum of the shares in each portfolios exceeds the NomAmount");
			m_Amount.SetFocus();
		}
		else
		{
			GetDlgItem(IDC_ALLOC_ALLOCATED_EDIT)->SetWindowText(QData.WriteNumber(Amount));
			Value();
		}

		ProcessVerifyRisk();
	}

	UpdateButtons();
}

void CAllocation::OnKillfocusPriceEdit() 
{
	Value();
}

void CAllocation::OnAddButton() 
{
	UpdateData();
}

void CAllocation::OnDeleteButton() 
{
	long Row;
	CQData QData;

	if(!m_SS.GetIsBlockSelected() || m_SS.GetSheetCurRow() < 1)
		return;

	if(MessageBox("Are you sure you want to delete these allocations ?",
	  			  "Delete", MB_OKCANCEL) == IDCANCEL)
		return;
	
	Row = m_SS.GetSheetCurRow();
	m_Data.DeleteSheetRow(Row);

	if(m_SS.GetSheetRows() < 0)
		m_SS.SetSheetRows(0);

	GetDlgItem(IDC_ALLOC_ALLOCATED_EDIT)->SetWindowText(QData.WriteNumber(m_Data.ComputeAllocAmount()));
	GetDlgItem(IDOK)->EnableWindow(m_SS.GetSheetRows() > 0 ? TRUE : FALSE);
	Refresh();
}

void CAllocation::OnUpdateButton() 
{
	if(m_SS.GetSheetCurRow() <= 0)
		return;

	UpdateData(FALSE);
}

void CAllocation::OnSelchangePortfolioCombo() 
{
	int i;
	CString Portfolio, Text;
	BOOL Valid = TRUE;
	CRawInvRec RecInvRec;

	Portfolio = m_Portfolio.GetData();
	if(Portfolio.IsEmpty())
		return;

	for (i = 1; i <= m_SS.GetSheetRows(); i ++)
	{
		m_Data.GetSelRawInv(i, RecInvRec);
		if(Portfolio == RecInvRec.GetPortfolio())
			Valid = FALSE;
	}

	ProcessVerifyRisk();

	if(!Valid)
	{
		MessageBox("Duplicated portfolio selection");
		m_Portfolio.SetCurSel(-1);
	}

	SetDefInfo(Portfolio);
	EnableCtrls();
	UpdateButtons();
}

void CAllocation::OnSelchangeAccountCombo()
{
/*	CString Account, Custodian;
	CRawInvRec RecInvRec;

	m_Account->GetSelString(Account);
	m_Custodian->GetSelString(Custodian);
	if(Account.IsEmpty())
		return;

	m_Data.GetSelRawInv(m_SS.GetSheetCurRow(), RecInvRec);
	if(m_Account->GetCurSel() < 0)
	{
		RecInvRec.GetAccount() = Account;
		m_SS.SetSheetText(5, m_SS.GetSheetCurRow(), Account);
	}
	
	if(!Custodian.IsEmpty() && RecInvRec.GetCustodian().IsEmpty())
	{
		RecInvRec.GetCustodian() = Custodian;
		m_SS.SetSheetText(4, m_SS.GetSheetCurRow(), Custodian);
	} */
}

void CAllocation::OnOK() 
{
	if(m_SS.GetSheetRows() <= 0)
		return;

	if(m_Data.AllocCheckSum() != 0) // Compute everything in the list
	{
		MessageBox("Sum of the shares in each portfolio must be equal to Available Shares");
		return;
	}

	CDBRec Rec;
	CString Price, DownPymnt;

	Price = m_Price.GetData();
	m_RecArray.RemoveAll();
	for(int i = 1; i <= m_SS.GetSheetRows(); i ++)
	{
		Rec.RemoveAll();
		DownPymnt = m_SS.GetSheetText(i, 8);
		for(int j = 1; j <= m_SS.GetSheetCols(); j ++)
		{
			switch(j)
			{
				case 1: // TransNum
					Rec.Add(EMPTYSTRING);
					break;
				case 6:
					if(DownPymnt == "0")
						Rec.Add(Price);
					else
						Rec.Add(m_SS.GetSheetText(j, i)); // TRS 
					break;
				default:
					Rec.Add(m_SS.GetSheetText(j, i));
					break;
			}
		}
		m_RecArray.Add(Rec);
	}

	m_sCustodian = m_Custodian.GetData();
	CDialog::OnOK();
}

void CAllocation::Value()
{
	CQData QData;

	m_Data.UpdateData();
	m_Data.ComputeValue();
	
	m_Price.SetData(m_Data.GetNetPrice());
	m_OtherFee.SetData(m_Data.GetOtherFee());
	GetDlgItem(IDC_ALLOC_PRINCIPLE_EDIT)->SetWindowText(QData.WriteNumber(m_Data.GetCash(), TRUE, 2));
	GetDlgItem(IDC_ALLOC_ACCRETION_EDIT)->SetWindowText(QData.WriteNumber(m_Data.GetAccretion(), TRUE, 2));
	GetDlgItem(IDC_ALLOC_INTEREST_EDIT)->SetWindowText(QData.WriteNumber(m_Data.GetPrePaid(), TRUE, 2));
	GetDlgItem(IDC_ALLOC_TOTAL_AMT_EDIT)->SetWindowText(QData.WriteNumber(m_Data.GetCash() + m_Data.GetAccretion() + m_Data.GetPrePaid(), TRUE, 2));
}

void CAllocation::OnRepoButton()
{
	m_Data.GenerateRepo();

	m_SS.LockSheet();
	UpdateButtons();
}

void CAllocation::OnAllocFinditemButton() 
{
/*	CFindDlg FindDlg;
	CString Portfolio, Asset, ValueDate, OptExp, Strike, TransType, Dir;
	CQData QData;
	
	TransType = m_Data.GetTicket().GetTransType();
	Dir = m_Data.GetTicket().GetDir();
	if((TransType == SECURITIES && (Dir == P || Dir == S)) || TransType == CDS ||
		TransType == CALL || TransType == PUT || TransType == SPRSWCLL || TransType == SPRSWPUT)
	{
		FindDlg.m_OraLoader = GetData().GetOraLoader();
		
		m_Portfolio->GetSelString(Portfolio);
		GetData().GetPortfolioArr().SetPortfolio(Portfolio);
		Portfolio = QData.GetQueryText(Portfolio);
		Asset = QData.GetQueryText(m_Data.GetTicket().GetAsset());
		ValueDate = QData.GetQueryDate(m_Data.GetTicket().GetValueDate());
		if(TransType == SECURITIES || TransType == CDS)
		{
			if(Portfolio.IsEmpty() || Asset.IsEmpty() || ValueDate.IsEmpty())
			{
				MessageBox("Insufficient search condition", "Error", MB_OK);
				return;
			}

			FindDlg.m_OraLoader.GetSql().Format("SELECT TRANS_NUM, ASSET_CODE, TRANS_DIRECTION \"DIR\", "
						"TRANS_TYPE, EXERCISE_PRICE \"STRIKE\", OPT_EXPIRATION \"OPT_EXP\", NOM_AMOUNT "
						"FROM SEMAM.NW_OUT_OPTIONS WHERE PORTFOLIO = %s AND ASSET_CODE = %s "
						"AND TRANS_TYPE IN ('PUT', 'CALL', 'SPR.SW.CLL', 'SPR.SW.PUT') "
						"AND (OPT_EXPIRATION IS NULL OR OPT_EXPIRATION >=TO_DATE(%s) - 5) "
						"AND EXERCISE_DATE IS NULL AND NOM_AMOUNT > 0 ORDER BY TRANS_NUM ", 
						(LPCTSTR) Portfolio, (LPCTSTR) Asset,	(LPCTSTR) ValueDate);
		}
		else
		{
			Strike = QData.GetQueryNumber(m_Data.GetTicket().GetStrike());
			OptExp = QData.GetQueryDate(m_Data.GetTicket().GetOptExp());
			TransType = QData.GetQueryText(TransType);
			Dir = QData.GetQueryText(Dir);
			if(Portfolio.IsEmpty() || Asset.IsEmpty() || ValueDate.IsEmpty() || Strike.IsEmpty() ||
				OptExp.IsEmpty() || Dir.IsEmpty())
			{
				MessageBox("Insufficient search condition", "Error", MB_OK);
				return;
			}

			FindDlg.m_OraLoader.GetSql().Format("SELECT TRANS_NUM, ASSET_CODE, TRANS_DIRECTION \"DIR\", "
						"TRANS_TYPE, EXERCISE_PRICE \"STRIKE\", OPT_EXPIRATION \"OPT_EXP\", NOM_AMOUNT "
						"FROM SEMAM.NW_OUT_OPTIONS WHERE PORTFOLIO = %s AND ASSET_CODE = %s "
						"AND TRANS_TYPE IN ('PUT', 'CALL', 'SPR.SW.CLL', 'SPR.SW.PUT') "
						"AND (OPT_EXPIRATION IS NULL OR OPT_EXPIRATION >=TO_DATE(%s) - 5) "
						"AND EXERCISE_DATE IS NULL AND NOM_AMOUNT > 0 AND EXERCISE_PRICE = %s "
						"AND OPT_EXPIRATION = %s AND TRANS_TYPE = %s AND TRANS_DIRECTION != %s "
						"ORDER BY TRANS_NUM ", (LPCTSTR) Portfolio, (LPCTSTR) Asset, (LPCTSTR) ValueDate, 
						(LPCTSTR) Strike, (LPCTSTR) OptExp, (LPCTSTR) TransType, (LPCTSTR) Dir);
		}

		if(FindDlg.DoModal() == IDOK)
			m_Link.SetWindowText(QData.RemoveComma(FindDlg.m_Key));
	} */
}

void CAllocation::OnAllocInvertPriceButton()
{
	CInvert Dlg;

	Dlg.m_Origin = m_Price.GetData();
	if(Dlg.DoModal() == IDOK)
		m_Price.SetData(Dlg.m_New);
}

void CAllocation::OnAllocFinditem2Button() 
{
/*	CFindDlg FindDlg;
	CString CP;
	CQData QData;

	m_AssignCP->GetSelString(CP);
	if(CP.IsEmpty())
	{
		MessageBox("Select an Assign Counterparty First", "Error", MB_OK);
		return;
	}

	FindDlg.m_OraLoader = GetData().GetOraLoader();
	FindDlg.m_OraLoader.GetSql().Format("SELECT CT_CODE \"CT\", CT_NAME, CT_EURO_NUM \"EUROCLEAR\", "
					"CT_CEDEL_NUM \"CEDEL\", CT_ADDRESS1 \"CP_CT\" , TO_NUMBER(CT_CODE) \"IDX\" "
					"FROM SEMAM.NW_COUNTER_CONTACTS WHERE ISVALID = 'Y' AND CP_CODE = %s ORDER BY 6 ", 
					QData.GetQueryText(CP));
	if(FindDlg.DoModal() == IDOK)
		m_AssignCT.SetWindowText(FindDlg.m_Key); */
}

void CAllocation::OnAllocContractConvertorButton() 
{
	CContractConvertor Dlg;
	CQData QData;

	m_Portfolio.SetFocus();

	Dlg.m_AssetStr = m_Data.GetTicket().GetAsset();
	Dlg.m_OraLoader = GetData().GetOraLoader();
	if(Dlg.DoModal() == IDOK)
		m_Amount.SetData(QData.WriteNumber(Dlg.m_dNominal));	
}

void CAllocation::OnSetfocusAllocNomamountEdit() 
{
	if(m_Amount.GetWindowTextLength() == 0)
	{
		OnAllocContractConvertorButton();
		if(m_Amount.GetWindowTextLength() > 0)
			m_Amount.SetFocus();
	}
}

void CAllocation::OnAllocRepopriceButton() 
{
	CRepoPrice Dlg;
	CString	ValueDate, RateBasis;
	
	Dlg.m_Asset = m_Data.GetTicket().GetAsset();
	if(Dlg.m_Asset.IsEmpty() || Dlg.m_Asset == "NEW ASSET")
		return;

	if(m_Margin.IsEmpty())
	{
		if(m_Data.GetTicket().GetTRS().IsEmpty())
			Dlg.m_HairCut = "10";
		else
			Dlg.m_HairCut = "0";
	}
	else
		Dlg.m_HairCut = m_Margin;

	Dlg.m_NWINetPrice = m_Price.GetData();
	ValueDate = m_Data.GetTicket().GetValueDate();
	RateBasis = m_Data.GetTicket().GetRateBasis(); // Lev Rate Basis
	Dlg.m_Val.Setup(GetData().GetOraLoader(), REPO, P, Dlg.m_Asset, ValueDate, ValueDate, "0", "0", "1", 
					RateBasis, "1", m_Formula);

	if(Dlg.DoModal() == IDOK)
	{
		m_Price.SetData(Dlg.m_NWINetPrice);
		m_Margin = Dlg.m_HairCut;
	}
}

void CAllocation::OnCbnKillfocusAllocPortfolioCombo()
{
	if(m_nRiskLevel == 3) // Exceeds Limit
		m_Portfolio.SetFocus();
}
