// IssuerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "IssuerDlg.h"
#include "qdata.h"


// CIssuerDlg dialog

IMPLEMENT_DYNAMIC(CIssuerDlg, CDialog)

CIssuerDlg::CIssuerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIssuerDlg::IDD, pParent)
{
	m_Country = NULL;
	m_Currency = NULL;
	m_FwdCurrency = NULL;
	m_AssetClass = NULL;
	m_SovFlag = NULL;
}

CIssuerDlg::~CIssuerDlg()
{
	if(m_Country)
		delete m_Country;
	if(m_Currency)
		delete m_Currency;
	if(m_FwdCurrency)
		delete m_FwdCurrency;
	if(m_AssetClass)
		delete m_AssetClass;
	if(m_SovFlag)
		delete m_SovFlag;
}

void CIssuerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ISSUER_LIST, m_SS);
}

void CIssuerDlg::EnableCtrls()
{
	GetDlgItem(IDC_ISSUER_ADD_BUTTON)->EnableWindow(m_Issuer.GetWindowTextLength() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_ISSUER_UPDATE_BUTTON)->EnableWindow(m_SS.GetSheetCurRow() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_ISSUER_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected());
}

BOOL CIssuerDlg::IsOK()
{
	CString Data, Text;
	COraLoader OraLoader;

	OraLoader.SetDB(&theDB);

	if(m_Issuer.GetWindowTextLength() < 0)
		Text = "Issuer cannot be blank";

	m_FwdCurrency->GetSelString(Data);
	if(!Data.IsEmpty())
	{
		OraLoader.Open("SELECT COUNT(*) FROM SEMAM.NW_ISSUERS WHERE FWD_CURRENCY = '" + Data + "' ");
		if(OraLoader.GetCount() > 0)
			Text = "Forward Currency must be unique";
	}

	if(Text.IsEmpty())
		return TRUE;

	MessageBox(Text, "ISSUER");
	return FALSE;
}

void CIssuerDlg::LoadData(CDBRec &DBRec)
{
	CQData QData;
	CString Data;

	DBRec.RemoveAll();

	m_Issuer.GetWindowText(Data);
	DBRec.Add(Data);
	DBRec.Add(m_Country->GetSelString(Data));
	DBRec.Add(m_Currency->GetSelString(Data));
	DBRec.Add(m_AssetClass->GetSelString(Data));
	DBRec.Add(m_FwdCurrency->GetSelString(Data));
	m_OptLimit.GetWindowText(Data);
	DBRec.Add(QData.RemoveComma(Data));
	m_OptLimit2.GetWindowText(Data);
	DBRec.Add(QData.RemoveComma(Data));
	m_IMIssuer.GetWindowText(Data);
	DBRec.Add(Data);
	DBRec.Add(m_SovFlag->GetCheckString());
}

void CIssuerDlg::MapData(int nRow)
{
	CString Data;

	if(nRow >= 1 && nRow <= m_SS.GetSheetRows())
	{
		m_Issuer.SetWindowText(m_SS.GetSheetText(1, nRow));
		m_Country->SelectString(0, m_SS.GetSheetText(2, nRow));
		m_Currency->SelectString(0, m_SS.GetSheetText(3, nRow));
		m_AssetClass->SelectString(0, m_SS.GetSheetText(4, nRow));
		m_FwdCurrency->SelectString(0, m_SS.GetSheetText(5, nRow));
		m_OptLimit.SetWindowText(m_SS.GetSheetText(6, nRow));
		m_OptLimit2.SetWindowText(m_SS.GetSheetText(7, nRow));
		m_IMIssuer.SetWindowText(m_SS.GetSheetText(8, nRow));
		m_SovFlag->SetCheck(m_SS.GetSheetText(9, nRow));
	}
	else
	{
		m_Issuer.SetWindowText(EMPTYSTRING);
		m_Country->SetCurSel(-1);
		m_Currency->SetCurSel(-1);
		m_AssetClass->SetCurSel(-1);
		m_FwdCurrency->SetCurSel(-1);
		m_OptLimit.SetWindowText(EMPTYSTRING);
		m_OptLimit2.SetWindowText(EMPTYSTRING);
		m_IMIssuer.SetWindowText(EMPTYSTRING);
		m_SovFlag->SetCheck(0);
	}
}

BEGIN_MESSAGE_MAP(CIssuerDlg, CDialog)
	ON_BN_CLICKED(IDC_ISSUER_ADD_BUTTON, &CIssuerDlg::OnBnClickedIssuerAddButton)
	ON_BN_CLICKED(IDC_ISSUER_UPDATE_BUTTON, &CIssuerDlg::OnBnClickedIssuerUpdateButton)
	ON_BN_CLICKED(IDC_ISSUER_DELETE_BUTTON, &CIssuerDlg::OnBnClickedIssuerDeleteButton)
	ON_BN_CLICKED(IDC_ISSUER_CLEAR_BUTTON, &CIssuerDlg::OnBnClickedIssuerClearButton)
	ON_EN_CHANGE(IDC_ISSUER_ISSUER_EDIT, &CIssuerDlg::OnEnChangeIssuerIssuerEdit)
END_MESSAGE_MAP()


// CIssuerDlg message handlers
BOOL CIssuerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CQData QData;
	COraLoader OraLoader;

	OraLoader.SetDB(&theDB);

	m_SS.SetVisibleRows(14);
	m_SS.SetVisibleCols(8);

	m_Issuer.Setup(this, IDC_ISSUER_ISSUER_EDIT);
	m_Issuer.LimitText(15);
	m_IMIssuer.Setup(this, IDC_ISSUER_IM_EDIT);
	m_IMIssuer.LimitText(10);
	m_OptLimit.Setup(this, IDC_ISSUER_OPTLIMIT_EDIT);
	m_OptLimit2.Setup(this, IDC_ISSUER_OPTLIMIT2_EDIT);
	m_Country = (COptComboBox *) new COptComboBox(this, IDC_ISSUER_COUNTRY_COMBO, TRUE);
	m_Currency = (COptComboBox *) new COptComboBox(this, IDC_ISSUER_CURRENCY_COMBO, TRUE);
	m_FwdCurrency = (COptComboBox *) new COptComboBox(this, IDC_ISSUER_FWDCURRENCY_COMBO, TRUE);
	m_AssetClass = (COptComboBox *) new COptComboBox(this, IDC_ISSUER_ASSETCLASS_COMBO, TRUE);
	m_SovFlag = (CCheckBox*) new CCheckBox(this, IDC_ISSUER_SOV_CHECK, Y);

	BeginWaitCursor();

	OraLoader.Open("SELECT COUNTRY FROM SEMAM.NW_COUNTRIES WHERE SOV_FLAG = 'Y' ORDER BY 1");
	OraLoader.LoadComboBox(*m_Country);

	OraLoader.Open("SELECT CURRENCY FROM SEMAM.NW_CURRENCY ORDER BY 1");
	OraLoader.LoadComboBox(*m_Currency);

	OraLoader.Open("SELECT CURRENCY FROM SEMAM.NW_CURRENCY ORDER BY 1");
	OraLoader.LoadComboBox(*m_FwdCurrency);

	OraLoader.Open("SELECT IND_CODE FROM SEMAM.NW_INDUSTRIES WHERE TYPE_FLAG = 'Y' ORDER BY 1");
	OraLoader.LoadComboBox(*m_AssetClass);
	OraLoader.Open("SELECT ISSUER, ISSUER_COUNTRY, ISSUER_CURRENCY, ISSUER_ASSET_CLASS, FWD_CURRENCY, "
					"QUANTUM_OPT_LIMIT, NWI_OPT_LIMIT, IM_ISSUER, SOV_FLAG "
					"FROM SEMAM.NW_ISSUERS ORDER BY 1 ");
	OraLoader.LoadDBSheet(m_SS);
	this->SetWindowText("ISSUER");
	EnableCtrls();
	EndWaitCursor();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIssuerDlg::OnBnClickedIssuerAddButton()
{
	CDBRec DBRec;
	COraLoader OraLoader;

	if(!IsOK())
		return;

	OraLoader.SetDB(&theDB);
	LoadData(DBRec);

	OraLoader.Open("SELECT ISSUER, ISSUER_COUNTRY, ISSUER_CURRENCY, ISSUER_ASSET_CLASS, FWD_CURRENCY, "
					"QUANTUM_OPT_LIMIT, NWI_OPT_LIMIT, IM_ISSUER, SOV_FLAG "
					"FROM SEMAM.NW_ISSUERS ", ODYNASET_DEFAULT);
	OraLoader.UpdateRecord(DBRec, TRUE);

	m_SS.AddSheetRow(DBRec);
}

void CIssuerDlg::OnBnClickedIssuerUpdateButton()
{
	CDBRec DBRec;
	CQData QData;
	CString Text;
	COraLoader OraLoader;

	if(!IsOK())
		return;

	OraLoader.SetDB(&theDB);
	LoadData(DBRec);

	m_Issuer.GetWindowText(Text);
	OraLoader.GetSql().Format("SELECT ISSUER, ISSUER_COUNTRY, ISSUER_CURRENCY, ISSUER_ASSET_CLASS, FWD_CURRENCY, "
								"QUANTUM_OPT_LIMIT, NWI_OPT_LIMIT, IM_ISSUER, SOV_FLAG "
								"FROM SEMAM.NW_ISSUERS WHERE ISSUER = %s ", QData.GetQueryText(Text));
	OraLoader.Open(OraLoader.GetSql(), ODYNASET_DEFAULT);
	OraLoader.UpdateRecord(DBRec);
	m_SS.UpdateSheetRow(m_SS.GetSheetCurRow(), DBRec);
}

void CIssuerDlg::OnBnClickedIssuerDeleteButton()
{
	CQData QData;
	COraLoader OraLoader;
	CString Text;

	if(m_Issuer.GetWindowTextLength() <= 0)
		return;

	OraLoader.SetDB(&theDB);
	m_Issuer.GetWindowText(Text);

	OraLoader.GetSql().Format("DELETE FROM SEMAM.NW_ISSUERS WHERE ISSUER = %s ", QData.GetQueryText(Text));
	OraLoader.ExecuteSql();
	m_SS.DeleteSheetRow(m_SS.GetSheetCurRow());
}

BEGIN_EVENTSINK_MAP(CIssuerDlg, CDialog)
	ON_EVENT(CIssuerDlg, IDC_ISSUER_LIST, 11, CIssuerDlg::OnDblClickIssuerList, VTS_I4 VTS_I4)
	ON_EVENT(CIssuerDlg, IDC_ISSUER_LIST, 2, CIssuerDlg::OnBlockSelectedIssuerList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

void CIssuerDlg::OnDblClickIssuerList(long Col, long Row)
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
		m_SS.SetSheetCurRow(Row);
	else
		m_SS.SetSheetCurRow(-1);
	MapData(m_SS.GetSheetCurRow());

	EnableCtrls();
}

void CIssuerDlg::OnBlockSelectedIssuerList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	OnDblClickIssuerList(BlockCol, BlockRow);
	EnableCtrls();
}

void CIssuerDlg::OnBnClickedIssuerClearButton()
{
	MapData(0);
	EnableCtrls();
}

void CIssuerDlg::OnEnChangeIssuerIssuerEdit()
{
	EnableCtrls();
}
