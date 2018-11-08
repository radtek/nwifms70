// RefIndexDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "YieldCurveDlg.h"
#include "qdata.h"

// CYieldCurveDlg dialog

IMPLEMENT_DYNAMIC(CYieldCurveDlg, CDialog)

CYieldCurveDlg::CYieldCurveDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CYieldCurveDlg::IDD, pParent)
{

}

CYieldCurveDlg::~CYieldCurveDlg()
{
}

void CYieldCurveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_YC_LIST, m_SS);
}

BOOL CYieldCurveDlg::DeleteRecord()
{
	if(MessageBox("You will delete selected record (Y/N) ? ") == IDYES)
	{
		if(m_OraLoader.ExecuteSql("DELETE FROM SEMAM.NW_YIELD_CURVE WHERE YC_NAME = '" + m_Name.GetData() + "'"))
			m_SS.DeleteSheetRow(m_SS.GetSheetCurRow());
	}

	return FALSE;
}

BOOL CYieldCurveDlg::UpdateRecord()
{
	CString Name, QName;
	CDBRec Rec;
	CQData QData;

	Name = m_Name.GetData();
	QName = QData.GetQueryText(Name);

	m_OraLoader.GetSql() = "SELECT COUNT(*) FROM SEMAM.NW_YIELD_CURVE WHERE YC_NAME = " + QName;

	Rec.Add(Name);
	Rec.Add(m_Symbol.GetData());
	Rec.Add(m_Index.GetData());
	Rec.Add(m_Term.GetData());
	Rec.Add(m_Source.GetData());
	Rec.Add(m_Default.GetData());
	Rec.Add(m_BBTick.GetData());

	if(m_OraLoader.GetCount(m_OraLoader.GetSql()) > 0)
	{
		m_OraLoader.Open("SELECT YC_NAME, YC_SYMBOL, YC_INDEX, YC_TERM, YC_SOURCE, YC_DEFAULT, BB_TICK "
						"FROM SEMAM.NW_YIELD_CURVE WHERE YC_NAME = " + QName, ODYNASET_DEFAULT);
		if(m_OraLoader.UpdateRecord(Rec))
		{
			m_SS.UpdateSheetRow(m_SS.GetSheetCurRow(), Rec);
			return TRUE;
		}
	}
	else
	{
		m_OraLoader.Open("SELECT YC_NAME, YC_SYMBOL, YC_INDEX, YC_TERM, YC_SOURCE, YC_DEFAULT, BB_TICK "
						"FROM SEMAM.NW_YIELD_CURVE ", ODYNASET_DEFAULT);
		if(m_OraLoader.UpdateRecord(Rec, TRUE))
		{
			m_SS.AddSheetRow(Rec);
			return TRUE;
		}
	}

	return FALSE;
}

BEGIN_MESSAGE_MAP(CYieldCurveDlg, CDialog)
	ON_BN_CLICKED(IDC_YC_ADD_BUTTON, &CYieldCurveDlg::OnBnClickedYieldcurveAddButton)
	ON_BN_CLICKED(IDC_YC_UPDATE_BUTTON, &CYieldCurveDlg::OnBnClickedYieldcurveUpdateButton)
	ON_BN_CLICKED(IDC_YC_DELETE_BUTTON, &CYieldCurveDlg::OnBnClickedYieldcurveDeleteButton)
	ON_EN_CHANGE(IDC_YC_NAME_EDIT, &CYieldCurveDlg::OnEnChangeYieldcurveNameEdit)
END_MESSAGE_MAP()


// CRefIndexDlg message handlers
BEGIN_EVENTSINK_MAP(CYieldCurveDlg, CDialog)
	ON_EVENT(CYieldCurveDlg, IDC_YC_LIST, 11, CYieldCurveDlg::OnDblClickYieldcurveList, VTS_I4 VTS_I4)
	ON_EVENT(CYieldCurveDlg, IDC_YC_LIST, 2, CYieldCurveDlg::OnBlockSelectedYieldcurveList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

BOOL CYieldCurveDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	BeginWaitCursor();
	m_SS.SetVisibleRows(14);
	m_SS.SetVisibleCols(5);

	m_Name.Setup(this, IDC_YC_NAME_EDIT, EMPTYSTRING, UPPERCASE);
	m_Name.LimitText(22);
	m_Symbol.Setup(this, IDC_YC_SYMBOL_EDIT, EMPTYSTRING, UPPERCASE);
	m_Symbol.LimitText(20);
	m_Index.Setup(this, IDC_YC_INDEX_COMBO);
	m_Term.Setup(this, IDC_YC_TERM_COMBO);
	m_Source.Setup(this, IDC_YC_SOURCE_COMBO);
	m_Default.Setup(this, IDC_YC_DEFAULT_COMBO, 2, TRUE);
	m_BBTick.Setup(this, IDC_YC_TICK_COMBO);

	m_OraLoader.SetDB(&theDB);

	m_OraLoader.Open("SELECT YC_INDEX FROM SEMAM.NW_YIELDCURVE_INDICES ORDER BY 1");
	m_OraLoader.LoadComboBox(m_Index);

	m_OraLoader.Open("SELECT YC_TERM, YC_DESC FROM SEMAM.NW_YIELDCURVE_TERM ORDER BY 1");
	m_OraLoader.LoadMCComboBox(m_Term);

	m_OraLoader.Open("SELECT YC_SOURCE FROM SEMAM.NW_YIELDCURVE_SOURCE ORDER BY 1 ");
	m_OraLoader.LoadComboBox(m_Source);

	m_OraLoader.Open("SELECT YC_DEFAULT FROM SEMAM.NW_YIELDCURVE_DEFAULT ORDER BY 1 ");
	m_OraLoader.LoadComboBox(m_Default);

	m_OraLoader.Open("SELECT BB_TICK FROM SEMAM.NW_YIELDCURVE_TICK ORDER BY 1 ");
	m_OraLoader.LoadComboBox(m_BBTick);

	m_OraLoader.Open("SELECT YC_NAME, YC_SYMBOL, YC_INDEX, YC_TERM, YC_SOURCE, YC_DEFAULT, BB_TICK FROM SEMAM.NW_YIELD_CURVE ORDER BY 1 ");
	m_OraLoader.LoadDBSheet(m_SS);
	
	EndWaitCursor();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CYieldCurveDlg::OnDblClickYieldcurveList(long Col, long Row)
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		CString Data;

		m_SS.SetSheetCurRow(Row);
		m_Name.SetData(m_SS.GetSheetText(1, Row));
		m_Symbol.SetData(m_SS.GetSheetText(2, Row));
		Data = m_SS.GetSheetText(3, Row);
		if(Data.IsEmpty())
			m_Index.SetData(EMPTYSTRING);
		else
			m_Index.SetData(Data);
		Data = m_SS.GetSheetText(4, Row);
		if(Data.IsEmpty())
			m_Term.SetCurSel(-1);
		else
			m_Term.SelectString(0, Data);
		Data = m_SS.GetSheetText(5, Row);
		if(Data.IsEmpty())
			m_Source.SetCurSel(-1);
		else
			m_Source.SelectString(0, Data);
		Data = m_SS.GetSheetText(6, Row);
		if(Data.IsEmpty())
			m_Default.SetCurSel(-1);
		else
			m_Default.SelectString(0, Data);
		Data = m_SS.GetSheetText(7, Row);
		if(Data.IsEmpty())
			m_BBTick.SetCurSel(-1);
		else
			m_BBTick.SelectString(0, Data);
	}
	else
	{
		m_Name.SetData(EMPTYSTRING);
		m_Symbol.SetData(EMPTYSTRING);
		m_Index.SetData(EMPTYSTRING);
		m_Term.SetCurSel(-1);
		m_Source.SetCurSel(-1);
		m_Default.SetCurSel(-1);
		m_BBTick.SetCurSel(-1);
	}

	GetDlgItem(IDC_YC_ADD_BUTTON)->EnableWindow(m_Name.GetWindowTextLength());
	GetDlgItem(IDC_YC_UPDATE_BUTTON)->EnableWindow(m_Name.GetWindowTextLength());
}

void CYieldCurveDlg::OnBlockSelectedYieldcurveList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	OnDblClickYieldcurveList(BlockCol, BlockRow);
	GetDlgItem(IDC_YC_DELETE_BUTTON)->EnableWindow(m_Name.GetWindowTextLength());
}

void CYieldCurveDlg::OnBnClickedYieldcurveAddButton()
{
	UpdateRecord();
}

void CYieldCurveDlg::OnBnClickedYieldcurveUpdateButton()
{
	UpdateRecord();
}

void CYieldCurveDlg::OnBnClickedYieldcurveDeleteButton()
{
	DeleteRecord();
}

void CYieldCurveDlg::OnEnChangeYieldcurveNameEdit()
{
	GetDlgItem(IDC_YC_ADD_BUTTON)->EnableWindow(m_Name.GetWindowTextLength());
	GetDlgItem(IDC_YC_UPDATE_BUTTON)->EnableWindow(m_Name.GetWindowTextLength());	
}
