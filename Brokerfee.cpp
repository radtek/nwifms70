// Brokerfee.cpp : implementation file
//

#include "stdafx.h"
#include "nwifms70.h"
#include "Brokerfee.h"
#include "qdata.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBrokerFee dialog


CBrokerFee::CBrokerFee(CWnd* pParent /*=NULL*/)
	: CDialog(CBrokerFee::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBrokerFee)
	m_Total = _T("");
	//}}AFX_DATA_INIT
}


void CBrokerFee::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBrokerFee)
	DDX_Text(pDX, IDC_BROKER_TOTAL_EDIT, m_Total);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBrokerFee, CDialog)
	//{{AFX_MSG_MAP(CBrokerFee)
	ON_EN_CHANGE(IDC_BROKER_BROKERFEE_EDIT, OnChangeBrokerfeeEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBrokerFee message handlers

BOOL CBrokerFee::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CQData QData;

	m_BrokerFee.SubclassDlgItem(IDC_BROKER_BROKERFEE_EDIT, this);
	SetDlgItemText(IDC_BROKER_AMOUNT_EDIT, m_sAmount);
	m_Amount = atof(QData.RemoveComma(m_sAmount));
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBrokerFee::OnChangeBrokerfeeEdit() 
{
	if(m_BrokerFee.GetModify())
	{
		double BrFee;
		CString Text;
		CQData QData;

		m_BrokerFee.GetWindowText(Text);
		BrFee = atof(QData.RemoveComma(Text));		
	
		SetDlgItemText(IDC_BROKER_TOTAL_EDIT, QData.WriteNumber(BrFee*m_Amount, TRUE, 2));
		m_BrokerFee.SetModify(FALSE);
	}
}
