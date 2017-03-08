// PaymentRestriction.cpp : implementation file
//

#include "stdafx.h"
#include "nwifms70.h"
#include "PaymentRestriction.h"
#include "qdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaymentRestriction dialog


CPaymentRestriction::CPaymentRestriction(CWnd* pParent /*=NULL*/)
	: CDialog(CPaymentRestriction::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPaymentRestriction)
	m_Portfolio = NULL;
	m_Ref = NULL;
	//}}AFX_DATA_INIT
}

CPaymentRestriction::~CPaymentRestriction()
{
	//{{AFX_DATA_INIT(CPaymentRestriction)
	if(m_Portfolio)
		delete m_Portfolio;
	if(m_Ref)
		delete m_Ref;
	//}}AFX_DATA_INIT
}

void CPaymentRestriction::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPaymentRestriction)
	DDX_Control(pDX, IDC_AR_LIST, m_SS);
	DDX_Control(pDX, IDC_AR_PAY_INSTRUCTION_LIST, m_Ref_SS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPaymentRestriction, CDialog)
	//{{AFX_MSG_MAP(CPaymentRestriction)
	ON_CBN_SELCHANGE(IDC_AR_PORTFOLIO_COMBO, OnSelchangeArPortfolioCombo)
	ON_CBN_SELCHANGE(IDC_AR_PAYMENT_COMBO, OnSelchangeArPaymentCombo)
	ON_BN_CLICKED(IDC_AR_ADD_BUTTON, OnArAddButton)
	ON_BN_CLICKED(IDC_AR_UPDATE_BUTTON, OnArUpdateButton)
	ON_BN_CLICKED(IDC_AR_DELETE_BUTTON, OnArDeleteButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPaymentRestriction message handlers

BEGIN_EVENTSINK_MAP(CPaymentRestriction, CDialog)
    //{{AFX_EVENTSINK_MAP(CPaymentRestriction)
	ON_EVENT(CPaymentRestriction, IDC_AR_PAY_INSTRUCTION_LIST, 11 /* DblClick */, OnDblClickArPayInstructionList, VTS_I4 VTS_I4)
	ON_EVENT(CPaymentRestriction, IDC_AR_LIST, 11 /* DblClick */, OnDblClickArList, VTS_I4 VTS_I4)
	ON_EVENT(CPaymentRestriction, IDC_AR_LIST, 2 /* BlockSelected */, OnBlockSelectedArList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CPaymentRestriction::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CQData QData;

	BeginWaitCursor();
	m_Ref_SS.SetVisibleRows(10);
	m_Ref_SS.SetVisibleCols(5);

	m_SS.SetVisibleRows(6);
	m_SS.SetVisibleCols(2);

	m_Data.Setup(GetOraLoader(), &m_SS);

    m_Portfolio = (COptComboBox*) new COptComboBox(this, IDC_AR_PORTFOLIO_COMBO);
	m_Ref = (COptComboBox*) new COptComboBox(this, IDC_AR_PAYMENT_COMBO);

	m_Data.Add(m_Portfolio, &m_Data.GetRec().GetPortfolio());
	m_Data.Add(m_Ref, &m_Data.GetRec().GetRefID());
	QData.CopyDBRecArrayToComboBox(GetData()->GetPortfolioArr(), *m_Portfolio);

	GetOraLoader().Open("SELECT REF_ID FROM SEMAM.NW_PAY_TEMPLATE "
						"ORDER BY 1 ");
	GetOraLoader().LoadComboBox(*m_Ref);
	m_Data.LoadRefData(m_Ref_SS);

	m_Data.LoadDBData();
	
	GetDlgItem(IDC_AR_ADD_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_AR_DELETE_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_AR_UPDATE_BUTTON)->EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPaymentRestriction::OnSelchangeArPortfolioCombo() 
{
	if(m_Portfolio->GetCurSel() >= 0 && m_Ref->GetCurSel() >= 0)
	{
		GetDlgItem(IDC_AR_ADD_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_AR_UPDATE_BUTTON)->EnableWindow(TRUE);
	}
}

void CPaymentRestriction::OnSelchangeArPaymentCombo() 
{
	OnSelchangeArPortfolioCombo();
}

void CPaymentRestriction::OnDblClickArPayInstructionList(long Col, long Row) 
{
	CString Text;

	if(Row > 0 && Row <= m_Ref_SS.GetSheetRows())
	{
		Text = m_Ref_SS.GetSheetText(1, Row);
		m_Ref->SelectString(0, Text);
		OnSelchangeArPortfolioCombo();
	}
}

void CPaymentRestriction::OnDblClickArList(long Col, long Row) 
{
	if(Row > 0 && Row <= m_Ref_SS.GetSheetRows())
		m_Data.SetCurrentRow(Row);
}

void CPaymentRestriction::OnArAddButton() 
{
	m_Data.AddRow();
	GetDlgItem(IDC_AR_ADD_BUTTON)->EnableWindow(FALSE);
}

void CPaymentRestriction::OnArUpdateButton() 
{
	m_Data.UpdateRow();
	GetDlgItem(IDC_AR_UPDATE_BUTTON)->EnableWindow(FALSE);
}

void CPaymentRestriction::OnArDeleteButton() 
{
	if(MessageBox("Delete selected entry ? ", NULL, MB_OKCANCEL) == IDOK)
		m_Data.DeleteRow();
	GetDlgItem(IDC_AR_DELETE_BUTTON)->EnableWindow(FALSE);
}

void CPaymentRestriction::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CPaymentRestriction::OnBlockSelectedArList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	if(BlockRow > 0 && BlockRow <= m_SS.GetSheetRows())
	{
		OnDblClickArList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
		GetDlgItem(IDC_AR_DELETE_BUTTON)->EnableWindow(TRUE);
	}
}