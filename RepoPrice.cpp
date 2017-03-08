// RepoPrice.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "RepoPrice.h"
#include "qdata.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRepoPrice dialog


CRepoPrice::CRepoPrice(CWnd* pParent /*=NULL*/)
	: CDialog(CRepoPrice::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRepoPrice)
	m_Asset = _T("");
	m_NetPrice = _T("");
	m_HairCut = _T("");
	//}}AFX_DATA_INIT
}


void CRepoPrice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRepoPrice)
	DDX_Text(pDX, IDC_REPOPRICE_ASSET_EDIT, m_Asset);
	DDX_Text(pDX, IDC_REPOPRICE_NETPRICE_EDIT, m_NetPrice);
	DDX_Text(pDX, IDC_REPOPRICE_HAIRCUT_EDIT, m_HairCut);
	//}}AFX_DATA_MAP
}

void CRepoPrice::DisplayPrice(int Index)
{
	double CleanPrice, DirtyPrice, NetPrice, NWINetPrice, HairCut;
	CString Text;
	CQData QData;

	m_CleanPriceEdit.GetWindowText(Text);
	CleanPrice = atof(QData.RemoveComma(Text));
	m_DirtyPriceEdit.GetWindowText(Text);
	DirtyPrice = atof(QData.RemoveComma(Text));
	m_HairCutEdit.GetWindowText(Text);
	HairCut = atof(QData.RemoveComma(Text))/100;
	m_NetPriceEdit.GetWindowText(Text);
	NetPrice = atof(QData.RemoveComma(Text));
	m_NWINetPriceEdit.GetWindowText(Text);
	NWINetPrice = atof(QData.RemoveComma(Text));

	switch(Index)
	{
		case 0: // We have Street all In Prices
			NWINetPrice = NetPrice/m_Val.GetAmortFact();
			DirtyPrice = NetPrice/(1 - HairCut)/m_Val.GetAmortFact();
			CleanPrice = DirtyPrice - (m_Val.GetPrePaidInt() + m_Val.GetPlusAmount())/m_Val.GetAmortFact();
			if(m_Val.GetPrePaidAccretion() > 0)
				CleanPrice = CleanPrice/(100 + m_Val.GetAccretion())*100;
			m_Val.SetPrice(CleanPrice);
			break;
		case 1: // We have Clean price
			m_Val.SetPrice(CleanPrice);
			DirtyPrice = CleanPrice + (m_Val.GetPrePaidInt() + m_Val.GetPlusAmount() +
						m_Val.GetPrePaidAccretionValue())/m_Val.GetAmortFact();
			NWINetPrice = DirtyPrice*(1 - HairCut);
			NetPrice = DirtyPrice*(1 - HairCut)*m_Val.GetAmortFact();
			break;
		case 2: // We have dirty price
			NWINetPrice = DirtyPrice*(1 - HairCut);
			NetPrice = DirtyPrice*(1 - HairCut)*m_Val.GetAmortFact();
			CleanPrice = DirtyPrice - (m_Val.GetPrePaidInt() + m_Val.GetPlusAmount())/m_Val.GetAmortFact();
			if(m_Val.GetPrePaidAccretion() > 0)
				CleanPrice = CleanPrice/(100 + m_Val.GetAccretion())*100;
			m_Val.SetPrice(CleanPrice);
			break;
		case 3: // We have a hair cut
			NWINetPrice = DirtyPrice*(1 - HairCut);
			NetPrice = DirtyPrice*(1 - HairCut)*m_Val.GetAmortFact();
			break;
		case 4: // We have a NWINetPrice
			NetPrice = NWINetPrice*m_Val.GetAmortFact();
			DirtyPrice = NetPrice/(1 - HairCut)/m_Val.GetAmortFact();
			CleanPrice = DirtyPrice - (m_Val.GetPrePaidInt() + m_Val.GetPlusAmount())/m_Val.GetAmortFact();
			if(m_Val.GetPrePaidAccretion() > 0)
				CleanPrice = CleanPrice/(100 + m_Val.GetAccretion())*100;
			m_Val.SetPrice(CleanPrice);
			break;
		default:
			return;			
	}

	switch(Index)
	{
		case 0: // We have Street all In Prices
			m_NWINetPriceEdit.SetWindowText(QData.WriteNumber(NWINetPrice));
			m_CleanPriceEdit.SetWindowText(QData.WriteNumber(CleanPrice));
			m_DirtyPriceEdit.SetWindowText(QData.WriteNumber(DirtyPrice));	
			m_HairCutEdit.SetWindowText(QData.WriteNumber(HairCut*100));
			break;
		case 1: // We have Clean price
			m_DirtyPriceEdit.SetWindowText(QData.WriteNumber(DirtyPrice));
			m_HairCutEdit.SetWindowText(QData.WriteNumber(HairCut*100));
			m_NWINetPriceEdit.SetWindowText(QData.WriteNumber(NWINetPrice));
			m_NetPriceEdit.SetWindowText(QData.WriteNumber(NetPrice));
			break;
		case 2: // We have dirty price
			m_CleanPriceEdit.SetWindowText(QData.WriteNumber(CleanPrice));			
			m_HairCutEdit.SetWindowText(QData.WriteNumber(HairCut*100));
			m_NWINetPriceEdit.SetWindowText(QData.WriteNumber(NWINetPrice));
			m_NetPriceEdit.SetWindowText(QData.WriteNumber(NetPrice));
			break;
		case 3: // We have a hair cut
			m_NWINetPriceEdit.SetWindowText(QData.WriteNumber(NWINetPrice));
			m_CleanPriceEdit.SetWindowText(QData.WriteNumber(CleanPrice));
			m_DirtyPriceEdit.SetWindowText(QData.WriteNumber(DirtyPrice));
			m_NetPriceEdit.SetWindowText(QData.WriteNumber(NetPrice));
			break;
		case 4: // We have a NWINetPrice
			m_CleanPriceEdit.SetWindowText(QData.WriteNumber(CleanPrice));
			m_DirtyPriceEdit.SetWindowText(QData.WriteNumber(DirtyPrice));
			m_HairCutEdit.SetWindowText(QData.WriteNumber(HairCut*100));
			m_NetPriceEdit.SetWindowText(QData.WriteNumber(NetPrice));
			break;
		default:
			break;
	}
}

BEGIN_MESSAGE_MAP(CRepoPrice, CDialog)
	//{{AFX_MSG_MAP(CRepoPrice)
	ON_EN_CHANGE(IDC_REPOPRICE_CLEANPRICE_EDIT, OnChangeRepoPriceCleanpriceEdit)
	ON_EN_CHANGE(IDC_REPOPRICE_DIRTYPRICE_EDIT, OnChangeRepoPriceDirtypriceEdit)
	ON_EN_CHANGE(IDC_REPOPRICE_HAIRCUT_EDIT, OnChangeRepoPriceHairCutEdit)
	ON_EN_CHANGE(IDC_REPOPRICE_NETPRICE_EDIT, OnChangeRepopriceNetpriceEdit)
	ON_EN_CHANGE(IDC_REPOPRICE_NWINETPRICE_EDIT, OnChangeRepopriceNwinetpriceEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRepoPrice message handlers

BOOL CRepoPrice::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_CleanPriceEdit.Setup(this, IDC_REPOPRICE_CLEANPRICE_EDIT);
	m_DirtyPriceEdit.Setup(this, IDC_REPOPRICE_DIRTYPRICE_EDIT);
	m_HairCutEdit.Setup(this, IDC_REPOPRICE_HAIRCUT_EDIT);
	m_NetPriceEdit.Setup(this, IDC_REPOPRICE_NETPRICE_EDIT);
	m_NWINetPriceEdit.Setup(this, IDC_REPOPRICE_NWINETPRICE_EDIT);
	
	if(!m_NWINetPrice.IsEmpty())
	{
		m_NWINetPriceEdit.SetWindowText(m_NWINetPrice);
		m_HairCutEdit.SetWindowText(m_HairCut);
		DisplayPrice(4);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRepoPrice::OnChangeRepoPriceCleanpriceEdit() 
{
	if(m_CleanPriceEdit.GetModify())
	{
		DisplayPrice(1);
		m_CleanPriceEdit.SetModify(FALSE);	
	}	
}

void CRepoPrice::OnChangeRepoPriceDirtypriceEdit() 
{
	if(m_DirtyPriceEdit.GetModify())
	{
		DisplayPrice(2);
		m_DirtyPriceEdit.SetModify(FALSE);
	}	
}

void CRepoPrice::OnChangeRepoPriceHairCutEdit() 
{
	if(m_HairCutEdit.GetModify())
	{
		DisplayPrice(3);
		m_HairCutEdit.SetModify(FALSE);
	}
}

void CRepoPrice::OnChangeRepopriceNetpriceEdit() 
{
	if(m_NetPriceEdit.GetModify())
	{
		DisplayPrice(0);
		m_NetPriceEdit.SetModify(FALSE);
	}
}

void CRepoPrice::OnChangeRepopriceNwinetpriceEdit() 
{
	if(m_NWINetPriceEdit.GetModify())
	{
		DisplayPrice(4);
		m_NWINetPriceEdit.SetModify(FALSE);
	}
}

void CRepoPrice::OnOK() 
{
	m_NetPriceEdit.GetWindowText(m_NetPrice);
	m_NWINetPriceEdit.GetWindowText(m_NWINetPrice);
	m_HairCutEdit.GetWindowText(m_HairCut);

	CDialog::OnOK();
}
