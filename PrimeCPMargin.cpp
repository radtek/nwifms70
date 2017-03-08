// PrimeCPMargin.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "PrimeCPMargin.h"
#include "afxdialogex.h"
#include "qdata.h"


// CPrimeCPMargin dialog

IMPLEMENT_DYNAMIC(CPrimeCPMargin, CDialog)

CPrimeCPMargin::CPrimeCPMargin(CWnd* pParent /*=NULL*/)
	: CDialog(CPrimeCPMargin::IDD, pParent)
{
	m_Fund = NULL;
	m_AssetClass = NULL;
	m_TransType = NULL;
	m_MarginCheck = NULL;
}

CPrimeCPMargin::~CPrimeCPMargin()
{
	if(m_Fund)
		delete m_Fund;
	if(m_AssetClass)
		delete m_AssetClass;
	if(m_TransType)
		delete m_TransType;
	if(m_MarginCheck)
		delete m_MarginCheck;
}

void CPrimeCPMargin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PM_LIST, m_SS);
}

void CPrimeCPMargin::LoadData()
{
	if(m_OraLoader.Open("SELECT PORTFOLIO, ASSET_CLASS, TRANS_TYPE, MARGIN FROM SEMAM.NW_CP_FUND_MARGIN ORDER BY 1, 2"))
	{
		m_SS.ClearSheet();
		m_OraLoader.LoadDBSheet(m_SS, TRUE);
	}
}

BOOL CPrimeCPMargin::UpdateRecord(BOOL bAddNew)
{
	CString Text;
	CDBRec DBRec;
	CQData QData;

	m_Fund->GetSelString(Text);
	if(Text.IsEmpty())
	{
		MessageBox("Porfolio does not exist");
		return FALSE;
	}
	DBRec.Add(Text);

	m_AssetClass->GetSelString(Text);
	if(Text.IsEmpty())
	{
		MessageBox("Asset Class does not exist");
		return FALSE;;
	}

	DBRec.Add(Text);

	m_TransType->GetSelString(Text);
	if(Text.IsEmpty())
	{
		MessageBox("Trans Type does not exist");
		return FALSE;;
	}
	
	DBRec.Add(Text);

	DBRec.Add(m_MarginCheck->GetCheckString());

	if(bAddNew)
	{
		if(m_OraLoader.Open("SELECT PORTFOLIO, ASSET_CLASS, TRANS_TYPE, MARGIN "
							"FROM SEMAM.NW_CP_FUND_MARGIN ORDER BY 1, 2, 3 ", ODYNASET_DEFAULT))
			if(m_OraLoader.UpdateRecord(DBRec, bAddNew))
				m_SS.AddSheetRow(DBRec);
	}
	else
	{
		m_OraLoader.GetSql() = "SELECT PORTFOLIO, ASSET_CLASS, TRANS_TYPE, MARGIN "
								"FROM SEMAM.NW_CP_FUND_MARGIN WHERE PORTFOLIO = ";
		m_OraLoader.GetSql() += QData.GetQueryText(DBRec.GetAt(0));
		m_OraLoader.GetSql() += " AND ASSET_CLASS = ";
		m_OraLoader.GetSql() += QData.GetQueryText(DBRec.GetAt(1));
		m_OraLoader.GetSql() += " AND TRANS_TYPE = ";
		m_OraLoader.GetSql() += QData.GetQueryText(DBRec.GetAt(2));
		
		if(m_OraLoader.Open(m_OraLoader.GetSql(), ODYNASET_DEFAULT))
		{
			if(m_OraLoader.UpdateRecord(DBRec))
				m_SS.UpdateSheetRow(m_nRow, DBRec);
		}
	}

	LoadData();
	return TRUE;
}

BOOL CPrimeCPMargin::OnInitDialog()
{
	CDialog::OnInitDialog();

	BeginWaitCursor();
	m_SS.SetVisibleCols(3);
	m_SS.SetVisibleRows(14);
	m_SS.LockSheet();

	m_Fund = new COptComboBox(this, IDC_PM_FUND_COMBO);
	m_AssetClass = new COptComboBox(this, IDC_PM_ASSETCLASS_COMBO);
	m_TransType = new COptComboBox(this, IDC_PM_TRANSTYPE_COMBO);
	m_MarginCheck = (CCheckBox*) new CCheckBox(this, IDC_PM_MARGIN_CHECK, Y);

	if(m_OraLoader.Open("SELECT PORTFOLIO FROM SEMAM.NW_PORTFOLIOS WHERE STATUS IS NULL "))
		m_OraLoader.LoadComboBox(*m_Fund);
	if(m_OraLoader.Open("SELECT IND_CODE FROM SEMAM.NW_INDUSTRIES WHERE TYPE_FLAG = 'Y' "))
		m_OraLoader.LoadComboBox(*m_AssetClass);
	if(m_OraLoader.Open("SELECT TRANS_TYPE FROM SEMAM.NW_TRANS_TYPES WHERE TRANS_TYPE IN ('CDS', 'SECURITIES', 'CALL', 'PUT') "))
		m_OraLoader.LoadComboBox(*m_TransType);

	LoadData();

	OnCbnSelchangePmFundCombo();
	GetDlgItem(IDC_PM_DELETE_BUTTON)->EnableWindow(FALSE);
	
	EndWaitCursor();

	return TRUE;
}

BEGIN_MESSAGE_MAP(CPrimeCPMargin, CDialog)
	ON_CBN_SELCHANGE(IDC_PM_FUND_COMBO, &CPrimeCPMargin::OnCbnSelchangePmFundCombo)
	ON_CBN_SELCHANGE(IDC_PM_ASSETCLASS_COMBO, &CPrimeCPMargin::OnCbnSelchangePmAssetclassCombo)
	ON_CBN_SELCHANGE(IDC_PM_TRANSTYPE_COMBO, &CPrimeCPMargin::OnCbnSelchangePmTranstypeCombo)
	ON_BN_CLICKED(IDC_PM_ADD_BUTTON, &CPrimeCPMargin::OnBnClickedPmAddButton)
	ON_BN_CLICKED(IDC_PM_UPDATE_BUTTON, &CPrimeCPMargin::OnBnClickedPmUpdateButton)
	ON_BN_CLICKED(IDC_PM_DELETE_BUTTON, &CPrimeCPMargin::OnBnClickedPmDeleteButton)
END_MESSAGE_MAP()


// CPrimeCPMargin message handlers
BEGIN_EVENTSINK_MAP(CPrimeCPMargin, CDialog)
	ON_EVENT(CPrimeCPMargin, IDC_PM_LIST, 11, CPrimeCPMargin::OnDblClickPmList, VTS_I4 VTS_I4)
	ON_EVENT(CPrimeCPMargin, IDC_PM_LIST, 2, CPrimeCPMargin::OnBlockSelectedPmList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


void CPrimeCPMargin::OnDblClickPmList(long Col, long Row)
{
	CString Text;

	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		m_nRow = Row;
		for(int i = 1; i <= 3; i ++)
		{
			Text = m_SS.GetSheetText(i, m_nRow);
			switch(i)
			{
				case 1:
					m_Fund->SelectString(0, Text);
					break;
				case 2:
					m_AssetClass->SelectString(0, Text);
					break;
				case 3:
					m_TransType->SelectString(0, Text);
					break;
				case 4:
					m_MarginCheck->SetCheck(Text);
					break;
				default:
					break;
			}
		}
	}
	else
	{
		m_Fund->SetCurSel(-1);
		m_AssetClass->SetCurSel(-1);
		m_MarginCheck->SetCheck(0);
		m_nRow = 0;
	}

	GetDlgItem(IDC_PM_UPDATE_BUTTON)->EnableWindow((m_nRow > 0 ? TRUE : FALSE) && m_bPowerUser);
}


void CPrimeCPMargin::OnBlockSelectedPmList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	OnDblClickPmList(BlockCol, BlockRow);
	GetDlgItem(IDC_PM_DELETE_BUTTON)->EnableWindow((m_nRow > 0 ? TRUE : FALSE) && m_bPowerUser);
}


void CPrimeCPMargin::OnCbnSelchangePmFundCombo()
{
	GetDlgItem(IDC_PM_ADD_BUTTON)->EnableWindow(m_Fund->GetCurSel() >= 0 && m_AssetClass->GetCurSel() >= 0 && m_TransType->GetCurSel() >= 0);
	GetDlgItem(IDC_PM_UPDATE_BUTTON)->EnableWindow(m_Fund->GetCurSel() >= 0 && m_AssetClass->GetCurSel() >= 0 && m_TransType->GetCurSel() >= 0);
}


void CPrimeCPMargin::OnCbnSelchangePmAssetclassCombo()
{
	OnCbnSelchangePmFundCombo();
}

void CPrimeCPMargin::OnCbnSelchangePmTranstypeCombo()
{
	OnCbnSelchangePmFundCombo();
}

void CPrimeCPMargin::OnBnClickedPmAddButton()
{
	UpdateRecord(TRUE);
	GetDlgItem(IDC_PM_ADD_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_PM_UPDATE_BUTTON)->EnableWindow(FALSE);
}


void CPrimeCPMargin::OnBnClickedPmUpdateButton()
{
	UpdateRecord();
	GetDlgItem(IDC_PM_ADD_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_PM_UPDATE_BUTTON)->EnableWindow(FALSE);
}


void CPrimeCPMargin::OnBnClickedPmDeleteButton()
{
	CString Fund, AssetClass, TransType;
	CQData QData;

	if(m_nRow <= 0)
	{
		MessageBox("No Record selected");
		return;
	}

	m_Fund->GetSelString(Fund);
	if(Fund.IsEmpty())
	{
		MessageBox("No Fund selected");
		return;
	}

	m_AssetClass->GetSelString(AssetClass);
	if(AssetClass.IsEmpty())
	{
		MessageBox("No Asset Class selected");
		return;
	}

	m_TransType->GetSelString(TransType);

	Fund = QData.GetQueryText(Fund);
	AssetClass = QData.GetQueryText(AssetClass);
	TransType = QData.GetQueryText(TransType);

	m_OraLoader.GetSql().Format("DELETE SEMAM.NW_CP_FUND_MARGIN WHERE PORTFOLIO = %s AND ASSET_CLASS = %s ", 
								"AND TRANS_TYPE = %s ", (LPCTSTR) Fund, (LPCTSTR) AssetClass, (LPCTSTR) TransType);
	
	if(MessageBox("Are you sure you want to delete this record ?", "Prime Broker Asset Margin", MB_YESNO) == IDYES)
	{
		if(m_OraLoader.ExecuteSql())
		{
			m_SS.DeleteSheetRow(m_nRow);
			m_Fund->SetCurSel(-1);
			m_AssetClass->SetCurSel(-1);
			m_MarginCheck->SetCheck(0);
		}
	
		LoadData();
	}

	GetDlgItem(IDC_PM_DELETE_BUTTON)->EnableWindow(FALSE);
}



