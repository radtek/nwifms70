// RefIndexDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "RefIndexDlg.h"
#include "qdata.h"

// CRefIndexDlg dialog

IMPLEMENT_DYNAMIC(CRefIndexDlg, CDialog)

CRefIndexDlg::CRefIndexDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRefIndexDlg::IDD, pParent)
{

}

CRefIndexDlg::~CRefIndexDlg()
{
}

void CRefIndexDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_REFIDX_LIST, m_SS);
}

BOOL CRefIndexDlg::DeleteRecord()
{
	if(MessageBox("You will delete selected record (Y/N) ? ") == IDYES)
	{
		CString RefIndex;

		m_RefIndex.GetWindowText(RefIndex);

		if(m_OraLoader.ExecuteSql("DELETE FROM SEMAM.NW_REF_IDX WHERE REF_IDX = '" + RefIndex + "'"))
			m_SS.DeleteSheetRow(m_SS.GetSheetCurRow());
	}

	return FALSE;
}

BOOL CRefIndexDlg::UpdateRecord()
{
	CString RefIndex, QRefIndex, Data;
	CDBRec Rec;
	CQData QData;

	m_RefIndex.GetWindowText(RefIndex);
	QRefIndex = QData.GetQueryText(RefIndex);

	m_OraLoader.GetSql() = "SELECT COUNT(*) FROM SEMAM.NW_REF_IDX WHERE REF_IDX = " + QRefIndex;

	Rec.Add(RefIndex);
	m_IndexDesc.GetWindowText(Data);
	Rec.Add(Data);
	m_IMIndex.GetWindowText(Data);
	Rec.Add(Data);
	m_CitcoCode.GetSelString(Data);
	Rec.Add(Data);
	m_ImagineYC.GetSelString(Data);
	Rec.Add(Data);

	if(m_OraLoader.GetCount(m_OraLoader.GetSql()) > 0)
	{
		m_OraLoader.Open("SELECT A.REF_IDX, A.REF_IDX_DESC, A.IM_IDX, A.CITCO_ID, A.YC_NAME "
						"FROM SEMAM.NW_REF_IDX A WHERE REF_IDX = " + QRefIndex, ODYNASET_DEFAULT);
		if(m_OraLoader.UpdateRecord(Rec))
		{
			m_SS.UpdateSheetRow(m_SS.GetSheetCurRow(), Rec);
			return TRUE;
		}
	}
	else
	{
		m_OraLoader.Open("SELECT A.REF_IDX, A.REF_IDX_DESC, A.IM_IDX, A.CITCO_ID, A.YC_NAME "
						"FROM SEMAM.NW_REF_IDX A ", ODYNASET_DEFAULT);
		if(m_OraLoader.UpdateRecord(Rec, TRUE))
		{
			m_SS.AddSheetRow(Rec);
			return TRUE;
		}
	}

	return FALSE;
}

BEGIN_MESSAGE_MAP(CRefIndexDlg, CDialog)
	ON_BN_CLICKED(IDC_REFIDX_ADD_BUTTON, &CRefIndexDlg::OnBnClickedRefidxAddButton)
	ON_BN_CLICKED(IDC_REFIDX_UPDATE_BUTTON, &CRefIndexDlg::OnBnClickedRefidxUpdateButton)
	ON_BN_CLICKED(IDC_REFIDX_DELETE_BUTTON, &CRefIndexDlg::OnBnClickedRefidxDeleteButton)
	ON_EN_CHANGE(IDC_REFIDX_IDX_EDIT, &CRefIndexDlg::OnEnChangeRefidxIdxEdit)
END_MESSAGE_MAP()


// CRefIndexDlg message handlers
BEGIN_EVENTSINK_MAP(CRefIndexDlg, CDialog)
	ON_EVENT(CRefIndexDlg, IDC_REFIDX_LIST, 11, CRefIndexDlg::OnDblClickRefidxList, VTS_I4 VTS_I4)
	ON_EVENT(CRefIndexDlg, IDC_REFIDX_LIST, 2, CRefIndexDlg::OnBlockSelectedRefidxList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

BOOL CRefIndexDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	BeginWaitCursor();
	m_SS.SetVisibleRows(14);
	m_SS.SetVisibleCols(5);

	m_RefIndex.Setup(this, IDC_REFIDX_IDX_EDIT, EMPTYSTRING, UPPERCASE);
	m_RefIndex.LimitText(15);
	m_IndexDesc.Setup(this, IDC_REFIDX_DESC_EDIT, EMPTYSTRING, UPPERCASE);
	m_IndexDesc.LimitText(30);
	m_IMIndex.Setup(this, IDC_REFIDX_IMIDX_EDIT, EMPTYSTRING, UPPERCASE);
	m_IMIndex.LimitText(15);
	m_CitcoCode.Setup(this, IDC_REFIDX_CITCO_COMBO, 2, TRUE);
	m_ImagineYC.Setup(this, IDC_REFIDX_IMAGINE_COMBO, 2, TRUE);

	m_OraLoader.SetDB(&theDB);

	m_OraLoader.Open("SELECT CITCO_ID, RATE_SOURCE FROM SEMAM.NW_CITCO_RATECODE ORDER BY 2");
	m_OraLoader.GetFieldArray().GetAt(0)->SetWithComma(FALSE);
	m_OraLoader.LoadMCComboBox(m_CitcoCode);

	m_OraLoader.Open("SELECT YC_NAME, YC_SYMBOL, YC_INDEX, YC_TERM, YC_SOURCE, YC_DEFAULT "
					"FROM SEMAM.NW_YIELD_CURVE ORDER BY 1");
	m_OraLoader.LoadMCComboBox(m_ImagineYC);

	m_OraLoader.Open("SELECT A.REF_IDX, A.REF_IDX_DESC, A.IM_IDX, A.CITCO_ID, A.YC_NAME "
					"FROM SEMAM.NW_REF_IDX A "
					"ORDER BY 1 ");
	m_OraLoader.GetFieldArray().GetAt(3)->SetWithComma(FALSE);
	m_OraLoader.LoadDBSheet(m_SS);
	
	EndWaitCursor();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRefIndexDlg::OnDblClickRefidxList(long Col, long Row)
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		CString Data;

		m_SS.SetSheetCurRow(Row);
		m_RefIndex.SetWindowText(m_SS.GetSheetText(1, Row));
		m_IndexDesc.SetWindowText(m_SS.GetSheetText(2, Row));
		Data = m_SS.GetSheetText(3, Row);
		if(Data.IsEmpty())
			m_IMIndex.SetWindowText(EMPTYSTRING);
		else
			m_IMIndex.SetWindowText(Data);
		Data = m_SS.GetSheetText(4, Row);
		if(Data.IsEmpty())
			m_CitcoCode.SetCurSel(-1);
		else
			m_CitcoCode.SelectString(0, Data);
		Data = m_SS.GetSheetText(5, Row);
		if(Data.IsEmpty())
			m_ImagineYC.SetCurSel(-1);
		else
			m_ImagineYC.SelectString(0, Data);
	}
	else
	{
		m_RefIndex.SetWindowText(EMPTYSTRING);
		m_IndexDesc.SetWindowText(EMPTYSTRING);
		m_IMIndex.SetWindowText(EMPTYSTRING);
		m_CitcoCode.SetCurSel(-1);
		m_ImagineYC.SetCurSel(-1);
	}

	GetDlgItem(IDC_REFIDX_ADD_BUTTON)->EnableWindow(m_RefIndex.GetWindowTextLength());
	GetDlgItem(IDC_REFIDX_UPDATE_BUTTON)->EnableWindow(m_RefIndex.GetWindowTextLength());
}

void CRefIndexDlg::OnBlockSelectedRefidxList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	OnDblClickRefidxList(BlockCol, BlockRow);
	GetDlgItem(IDC_REFIDX_DELETE_BUTTON)->EnableWindow(m_RefIndex.GetWindowTextLength());
}

void CRefIndexDlg::OnBnClickedRefidxAddButton()
{
	UpdateRecord();
}

void CRefIndexDlg::OnBnClickedRefidxUpdateButton()
{
	UpdateRecord();
}

void CRefIndexDlg::OnBnClickedRefidxDeleteButton()
{
	DeleteRecord();
}

void CRefIndexDlg::OnEnChangeRefidxIdxEdit()
{
	GetDlgItem(IDC_REFIDX_ADD_BUTTON)->EnableWindow(m_RefIndex.GetWindowTextLength());
	GetDlgItem(IDC_REFIDX_UPDATE_BUTTON)->EnableWindow(m_RefIndex.GetWindowTextLength());	
}
