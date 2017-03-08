// AssetClassDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "AssetClassDlg.h"
#include "qdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CAssetClassDlg dialog

CAssetClassDlg::CAssetClassDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAssetClassDlg::IDD, pParent)
{
	m_Liquidity = NULL;
	m_PGroupCheck = NULL;
}

CAssetClassDlg::~CAssetClassDlg()
{
	if(m_Liquidity)
		delete m_Liquidity;
	
	if(m_PGroupCheck)
		delete m_PGroupCheck;
}

void CAssetClassDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAssetClassDlg)
	DDX_Control(pDX, IDC_ASSETCLASS_LIST, m_SS);
	//}}AFX_DATA_MAP}
}

void CAssetClassDlg::LoadData()
{
	if(m_OraLoader.Open("SELECT IND_CODE, PROFILE_GROUP, EXPOSURE_GROUP, "
						"IND_LIQUIDITY, TYPE_FLAG FROM SEMAM.NW_INDUSTRIES ORDER BY 1"))
		m_OraLoader.LoadDBSheet(m_SS, TRUE);
}

BOOL CAssetClassDlg::UpdateRecord(BOOL bAddNew)
{
	CString Industry, Text;
	CDBRec DBRec;
	CQData QData;

	if(m_Class.GetWindowTextLength() == 0)
	{
		MessageBox("Class does not exist");
		return FALSE;
	}
	else
	{
		m_Class.GetWindowText(Industry);
		DBRec.Add(Industry);
	}
	
	if(m_PGroup.GetWindowTextLength() == 0)
	{
		MessageBox("Profile Group does not exist");
		return FALSE;;
	}
	else
	{
		m_PGroup.GetWindowText(Text);
		DBRec.Add(Text);
	}

	if(m_EGroup.GetWindowTextLength() == 0)
	{
		MessageBox("Exposure Group does not exist");
		return FALSE;
	}
	else
	{
		m_EGroup.GetWindowText(Text);
		DBRec.Add(Text);
	}

	m_Liquidity->GetSelString(Text);
	DBRec.Add(QData.RemoveComma(Text));

	DBRec.Add(m_PGroupCheck->GetCheckString());

	if(bAddNew)
	{
		if(m_OraLoader.Open("SELECT IND_CODE, PROFILE_GROUP, EXPOSURE_GROUP, IND_LIQUIDITY, TYPE_FLAG "
							"FROM SEMAM.NW_INDUSTRIES ", ODYNASET_DEFAULT))
			if(m_OraLoader.UpdateRecord(DBRec, bAddNew))
				m_SS.AddSheetRow(DBRec);
	}
	else
	{
		m_OraLoader.GetSql() = "SELECT IND_CODE, PROFILE_GROUP, EXPOSURE_GROUP, IND_LIQUIDITY, "
							"TYPE_FLAG FROM SEMAM.NW_INDUSTRIES WHERE IND_CODE = ";
		m_OraLoader.GetSql() += QData.GetQueryText(Industry);
		
		if(m_OraLoader.Open(m_OraLoader.GetSql(), ODYNASET_DEFAULT))
		{
			if(m_OraLoader.UpdateRecord(DBRec))
				m_SS.UpdateSheetRow(m_Row, DBRec);
		}
	}

	LoadData();
	return TRUE;
}

// CAssetClassDlg message handlers
BOOL CAssetClassDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	BeginWaitCursor();
	m_SS.SetVisibleCols(5);
	m_SS.SetVisibleRows(14);
	m_SS.LockSheet();

	m_Class.Setup(this, IDC_ASSETCLASS_CLASS_EDIT);
	m_Class.LimitText(15);
	m_PGroup.Setup(this, IDC_ASSETCLASS_PGROUP_EDIT);
	m_PGroup.LimitText(15);
	m_EGroup.Setup(this, IDC_ASSETCLASS_EGROUP_EDIT);
	m_EGroup.LimitText(25);
	m_Liquidity = new COptComboBox(this, IDC_ASSETCLASS_LIQUIDITY_COMBO);
	m_PGroupCheck = (CCheckBox*) new CCheckBox(this, IDC_ASSETCLASS_PGROUP_CHECK, Y);

	m_OraLoader.Open("SELECT AMOUNT FROM SEMAM.NW_IND_LIQUIDITY ORDER BY 1 ");
	m_OraLoader.LoadComboBox(*m_Liquidity);

	GetDlgItem(IDC_ASSETCLASS_ADD_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_ASSETCLASS_UPDATE_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_ASSETCLASS_DELETE_BUTTON)->EnableWindow(FALSE);
	LoadData();
	EndWaitCursor();

	return TRUE;
}

BEGIN_MESSAGE_MAP(CAssetClassDlg, CDialog)
	ON_BN_CLICKED(IDC_ASSETCLASS_CLEAR_BUTTON, &CAssetClassDlg::OnBnClickedAssetclassClearButton)
	ON_BN_CLICKED(IDC_ASSETCLASS_ADD_BUTTON, &CAssetClassDlg::OnBnClickedAssetclassAddButton)
	ON_BN_CLICKED(IDC_ASSETCLASS_UPDATE_BUTTON, &CAssetClassDlg::OnBnClickedAssetclassUpdateButton)
	ON_BN_CLICKED(IDC_ASSETCLASS_DELETE_BUTTON, &CAssetClassDlg::OnBnClickedAssetclassDeleteButton)
	ON_EN_CHANGE(IDC_ASSETCLASS_CLASS_EDIT, &CAssetClassDlg::OnEnChangeAssetclassClassEdit)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CAssetClassDlg, CDialog)
	ON_EVENT(CAssetClassDlg, IDC_ASSETCLASS_LIST, 11, CAssetClassDlg::OnDblClickAssetclassList, VTS_I4 VTS_I4)
	ON_EVENT(CAssetClassDlg, IDC_ASSETCLASS_LIST, 2, CAssetClassDlg::OnBlockSelectedAssetclassList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

void CAssetClassDlg::OnDblClickAssetclassList(long Col, long Row)
{
	CString Text;

	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		m_Row = Row;
		for(int i = 1; i <= 5; i ++)
		{
			Text = m_SS.GetSheetText(i, m_Row);
			switch(i)
			{
				case 1:
					m_Class.SetWindowText(Text);
					break;
				case 2:
					m_PGroup.SetWindowText(Text);
					break;
				case 3:
					m_EGroup.SetWindowText(Text);
					break;
				case 4:
					m_Liquidity->SelectString(0, Text);
					break;
				case 5:
					m_PGroupCheck->SetCheck(Text);
					break;
				default:
					break;
			}
		}
	}
	else
	{
		m_Class.SetWindowText("");
		m_PGroup.SetWindowText("");
		m_EGroup.SetWindowText("");
		m_Liquidity->SetCurSel(-1);
		m_PGroupCheck->SetCheck(0);
		m_Row = 0;
	}

	GetDlgItem(IDC_ASSETCLASS_UPDATE_BUTTON)->EnableWindow((m_Row > 0 ? TRUE : FALSE) && m_bPowerUser);
}

void CAssetClassDlg::OnBlockSelectedAssetclassList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	OnDblClickAssetclassList(BlockCol, BlockRow);
	GetDlgItem(IDC_ASSETCLASS_DELETE_BUTTON)->EnableWindow((m_Row > 0 ? TRUE : FALSE) && m_bPowerUser);
}

void CAssetClassDlg::OnBnClickedAssetclassClearButton()
{
	m_Class.SetWindowText(EMPTYSTRING);
	m_PGroup.SetWindowText(EMPTYSTRING);
	m_EGroup.SetWindowText(EMPTYSTRING);
	m_Liquidity->SetCurSel(-1);
	m_PGroupCheck->SetCheck(0);
}

void CAssetClassDlg::OnBnClickedAssetclassAddButton()
{
	UpdateRecord(TRUE);
	GetDlgItem(IDC_ASSETCLASS_UPDATE_BUTTON)->EnableWindow(FALSE);
}

void CAssetClassDlg::OnBnClickedAssetclassUpdateButton()
{
	UpdateRecord(FALSE);
	GetDlgItem(IDC_ASSETCLASS_ADD_BUTTON)->EnableWindow(FALSE);
}

void CAssetClassDlg::OnBnClickedAssetclassDeleteButton()
{
	CString Text;
	CQData QData;

	m_Class.GetWindowText(Text);
	if(Text.GetLength() <= 0)
	{
		MessageBox("No Class selected");
		return;
	}

	m_OraLoader.GetSql().Format("DELETE SEMAM.NW_INDUSTRIES WHERE IND_CODE = %s ", 
								QData.GetQueryText(Text));
	if(MessageBox("Are you sure you want to delete this record ?", "AssetClass", MB_YESNO) == IDYES)
	{
		if(m_OraLoader.ExecuteSql())
		{
			m_SS.DeleteSheetRow(m_Row);
			OnBnClickedAssetclassClearButton();
		}
	
		LoadData();
	}

	GetDlgItem(IDC_ASSETCLASS_DELETE_BUTTON)->EnableWindow(FALSE);
}


void CAssetClassDlg::OnEnChangeAssetclassClassEdit()
{
	GetDlgItem(IDC_ASSETCLASS_ADD_BUTTON)->EnableWindow((m_Class.GetWindowTextLength() > 0) && m_bPowerUser);
}
