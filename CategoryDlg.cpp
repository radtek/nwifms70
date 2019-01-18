// CategoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "CategoryDlg.h"
#include "qdata.h"
#include "afxdialogex.h"


// CCategoryDlg dialog

IMPLEMENT_DYNAMIC(CCategoryDlg, CDialog)

CCategoryDlg::CCategoryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCategoryDlg::IDD, pParent)
{

}


CCategoryDlg::~CCategoryDlg()
{
}


void CCategoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CAT_LIST, m_SS);
}


void CCategoryDlg::EnableButtons()
{
	GetDlgItem(IDC_CAT_UPDATE_BUTTON)->EnableWindow(m_SS.GetSheetCurRow() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_CAT_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected());
	GetDlgItem(IDC_CAT_ADD_BUTTON)->EnableWindow(IsOK());
}

BOOL CCategoryDlg::IsOK()
{
	if(m_Strategy.GetData().IsEmpty())
		return FALSE;

	if(m_Index.GetData().IsEmpty())
		return FALSE;
		
	if(m_CatCode.GetData().IsEmpty())
		return FALSE;
	
	return TRUE;
}

BEGIN_MESSAGE_MAP(CCategoryDlg, CDialog)
	ON_BN_CLICKED(IDC_CAT_CLEAR_BUTTON, &CCategoryDlg::OnBnClickedCatClearButton)
	ON_BN_CLICKED(IDC_CAT_ADD_BUTTON, &CCategoryDlg::OnBnClickedCatAddButton)
	ON_BN_CLICKED(IDC_CAT_UPDATE_BUTTON, &CCategoryDlg::OnBnClickedCatUpdateButton)
	ON_BN_CLICKED(IDC_CAT_DELETE_BUTTON, &CCategoryDlg::OnBnClickedCatDeleteButton)
END_MESSAGE_MAP()


// CCategoryDlg message handlers
BEGIN_EVENTSINK_MAP(CCategoryDlg, CDialog)
	ON_EVENT(CCategoryDlg, IDC_CAT_LIST, 11, CCategoryDlg::DblClickCatList, VTS_I4 VTS_I4)
	ON_EVENT(CCategoryDlg, IDC_CAT_LIST, 2, CCategoryDlg::BlockSelectedCatList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

BOOL CCategoryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_SS.SetVisibleRows(14);
	m_SS.SetVisibleCols(4);

	m_Strategy.Setup(this, IDC_CAT_STRATEGY_EDIT, EMPTYSTRING, UPPERCASE);
	m_Index.Setup(this, IDC_CAT_INDEX_EDIT);
	m_Trader.Setup(this, IDC_CAT_TRADER_COMBO);
	m_CatCode.Setup(this, IDC_CAT_CODE_EDIT, EMPTYSTRING, UPPERCASE);

	m_Strategy.SetLimitText(16);
	m_CatCode.SetLimitText(6);

	m_OraLoader = m_pData->GetOraLoader();
	
	m_pData->GetTraderArr().CopyToMCComboBox(m_Trader);

	m_OraLoader.Open("SELECT STRATEGY, CAT_INDEX, TRADER_INI, CAT_CODE FROM SEMAM.NW_INV_STRATEGIES "
					 "WHERE TRANS_BUCKET IS NULL ORDER BY CAT_INDEX ");
	
	m_OraLoader.LoadDBSheet(m_SS);
	m_bModified = FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CCategoryDlg::DblClickCatList(long Col, long Row)
{
	if(Row > 0 && Row <= m_SS.GetSheetRows())
	{
		m_SS.SetSheetCurRow(Row);
		m_Strategy.SetData(m_SS.GetSheetText(1, Row));
		m_Index.SetData(m_SS.GetSheetText(2, Row));
		m_Trader.SetData(m_SS.GetSheetText(3, Row));
		m_CatCode.SetData(m_SS.GetSheetText(4, Row));
	}
	else
		OnBnClickedCatClearButton();
	
	EnableButtons();
}


void CCategoryDlg::BlockSelectedCatList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	DblClickCatList(BlockCol, BlockRow);
}


void CCategoryDlg::OnBnClickedCatClearButton()
{
	m_SS.SetSheetCurRow(-1);
	m_Strategy.SetData(NULL);
	m_Index.SetData(NULL);
	m_Trader.SetData(NULL);
	m_CatCode.SetData(NULL);
}


void CCategoryDlg::OnBnClickedCatAddButton()
{
	CDBRec Rec;

	if(!IsOK())
		return;

	m_OraLoader.Open("SELECT STRATEGY, CAT_INDEX, TRADER_INI, CAT_CODE FROM SEMAM.NW_INV_STRATEGIES ", ODYNASET_DEFAULT);
	
	Rec.Add(m_Strategy.GetData());
	Rec.Add(m_Index.GetData());
	Rec.Add(m_Trader.GetData());
	Rec.Add(m_CatCode.GetData());

	if(m_OraLoader.UpdateRecord(Rec, TRUE))
		m_SS.AddSheetRow(Rec);
}


void CCategoryDlg::OnBnClickedCatUpdateButton()
{
	CDBRec Rec;
	CQData QData;
	CString Strategy;

	if(!IsOK())
		return;

	Strategy = QData.GetQueryText(m_Strategy.GetData());
	m_OraLoader.Open("SELECT STRATEGY, CAT_INDEX, TRADER_INI, CAT_CODE FROM SEMAM.NW_INV_STRATEGIES WHERE STRATEGY = " + Strategy, ODYNASET_DEFAULT);
	
	Rec.Add(m_Strategy.GetData());
	Rec.Add(m_Index.GetData());
	Rec.Add(m_Trader.GetData());
	Rec.Add(m_CatCode.GetData());

	if(m_OraLoader.UpdateRecord(Rec))
		m_SS.UpdateSheetRow(m_SS.GetSheetCurRow(), Rec);
}


void CCategoryDlg::OnBnClickedCatDeleteButton()
{
	if(!m_Strategy.GetData().IsEmpty())
	{
		CQData QData;
		CString Strategy;

		Strategy = QData.GetQueryText(m_Strategy.GetData());
		if(m_OraLoader.GetCount("SELECT COUNT(*) FROM SEMAM.NW_ASSETS WHERE ASS_CATEGORY = " + Strategy) > 0)
		{
			MessageBox("This strategy cannot been deleted because it has been used maintenance", "Category", MB_OK);
			return;
		}
		else
		{
			if(MessageBox("Are you sure you want to delete this record (Y/N) ?", "Category", MB_YESNO))
			{
				if(m_OraLoader.ExecuteSql("DELETE SEMAM.NW_INV_STRATEGIES WHERE STRATEGY = '" + m_Strategy.GetData() + "'"))
				{
					m_SS.DeleteSheetRow(m_SS.GetSheetCurRow());
					OnBnClickedCatClearButton();
				}
			}
		}
	}
}



