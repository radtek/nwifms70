// ContractConvertor.cpp : implementation file
//

#include "stdafx.h"
#include "nwifms70.h"
#include "ContractConvertor.h"
#include "qdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CContractConvertor dialog


CContractConvertor::CContractConvertor(CWnd* pParent /*=NULL*/)
	: CDialog(CContractConvertor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CContractConvertor)
	m_AssetStr = "";
	m_dFactor = 0;
	//}}AFX_DATA_INIT
}


void CContractConvertor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CContractConvertor)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CContractConvertor, CDialog)
	//{{AFX_MSG_MAP(CContractConvertor)
	ON_EN_KILLFOCUS(IDC_CONTRACT_CONTRACT_EDIT, OnKillfocusContractContractEdit)
	ON_EN_KILLFOCUS(IDC_CONTRACT_FACTOR_EDIT, OnKillfocusContractFactorEdit)
	ON_EN_KILLFOCUS(IDC_CONTRACT_NOMINAL_EDIT, OnKillfocusContractNominalEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CContractConvertor message handlers

BOOL CContractConvertor::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CQData QData;
	
	m_Asset.Setup(this, IDC_CONTRACT_ASSET_EDIT, NULL, 0, FALSE);
	m_Desc.Setup(this, IDC_CONTRACT_DESC_EDIT, NULL, 0, FALSE);
	m_Factor.Setup(this, IDC_CONTRACT_FACTOR_EDIT);
	m_Contract.Setup(this, IDC_CONTRACT_CONTRACT_EDIT);
	m_Nominal.Setup(this, IDC_CONTRACT_NOMINAL_EDIT);

	if(!m_AssetStr.IsEmpty() && m_AssetStr != "NEWASSET")
	{
		m_OraLoader.GetSql().Format("SELECT ASS_CODE, ASS_DESC, NVL(ASS_PAR_VALUE, 1) "
								"FROM SEMAM.NW_ASSETS WHERE ASS_CODE = %s ", 
								QData.GetQueryText(m_AssetStr));

		if(m_OraLoader.Open())
		{
			m_Asset.SetWindowText(m_OraLoader.GetDBString(0));
			m_Desc.SetWindowText(m_OraLoader.GetDBString(1));
			m_Factor.SetWindowText(m_OraLoader.GetDBString(2));
			m_dFactor = atof(QData.RemoveComma(m_OraLoader.GetDBString(2)));
			if(m_dFactor > 0)
				m_Factor.SetReadOnly();
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CContractConvertor::OnOK() 
{
	if(!m_NominalStr.IsEmpty())
		CDialog::OnOK();
}

void CContractConvertor::OnKillfocusContractContractEdit() 
{
	CQData QData;
	CString Text;
	
	if(m_dFactor > 0)
	{
		m_Contract.GetWindowText(Text);
		m_dNominal = atof(QData.RemoveComma(Text))*m_dFactor;
		m_NominalStr = QData.WriteNumber(m_dNominal);
		m_Nominal.SetWindowText(m_NominalStr);
	}
}

void CContractConvertor::OnKillfocusContractFactorEdit() 
{
	CQData QData;
	CString Text;
	
	m_Factor.GetWindowText(Text);
	m_dFactor = atof(QData.RemoveComma(Text));
}

void CContractConvertor::OnKillfocusContractNominalEdit() 
{
	CQData QData;
	
	if(m_dFactor > 0)
	{
		m_Nominal.GetWindowText(m_NominalStr);
		m_dNominal = atof(QData.RemoveComma(m_NominalStr));
		m_Contract.SetWindowText(QData.WriteNumber(m_dNominal/m_dFactor));
	}	
}
