// OtherFeeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "OtherFeeDlg.h"
#include "equity.h"


// COtherFeeDlg dialog

IMPLEMENT_DYNAMIC(COtherFeeDlg, CDialog)

COtherFeeDlg::COtherFeeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COtherFeeDlg::IDD, pParent)
{

}

COtherFeeDlg::~COtherFeeDlg()
{
}

void COtherFeeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void COtherFeeDlg::ComputeFees()
{
	COraLoader OraLoader;
	OValue Value;
	CEquity EqVal;
	CString TransType, Dir;
	double NomAmount, Price, Fxrate, BrokerFee, OtherFee, Par, Amount;
	CQData QData;

	m_dSecFee = 0;
	m_dOrFee = 0;
	OraLoader.SetDB(&theDB);
	OraLoader.Open("SELECT TRANS_TYPE, TRANS_DIRECTION, NOM_AMOUNT, PRICE, NVL(FXRATE, 1), "
					"NVL(BR_FEES, 0), NVL(LEV_RATE, 0), ASS_PAR_VALUE "
					"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_ASSETS B "
					"WHERE A.ASSET_CODE = B.ASS_CODE "
					"AND A.TRANS_NUM = " + m_TransNum);

	OraLoader.MoveFirst();
	while(!OraLoader.IsEOF())
	{
		TransType = OraLoader.GetDBString(0);
		Dir = OraLoader.GetDBString(1);
		OraLoader.GetFieldValue(2, &Value);
		NomAmount = (double) Value;
		OraLoader.GetFieldValue(3, &Value);
		Price = (double) Value;
		OraLoader.GetFieldValue(4, &Value);
		Fxrate = (double) Value;
		OraLoader.GetFieldValue(5, &Value);
		BrokerFee = (double) Value;
		OraLoader.GetFieldValue(6, &Value);
		OtherFee = (double) Value;
		OraLoader.GetFieldValue(7, &Value);
		Par = (double) Value;
		OraLoader.MoveNext();
	}

	EqVal.Setup(TransType, NomAmount, Price, Fxrate, Dir);
	if(m_bSecFee)
	{
		Amount = EqVal.GetValue();
		Amount += BrokerFee + OtherFee;
		Price = Amount/EqVal.GetNomAmount();
		m_dSecFee = EqVal.GetSecFees(Price, m_bSecFee);
	}

	if(m_bOrFee)
		m_dOrFee = EqVal.GetOrFees(Par, m_bOrFee);

	m_SecFee.SetWindowText(QData.WriteNumber(m_dSecFee, TRUE));
	m_OrFee.SetWindowText(QData.WriteNumber(m_dOrFee, TRUE));
	m_TotalFee.SetWindowText(QData.WriteNumber(m_dSecFee + m_dOrFee, TRUE));
}

BOOL COtherFeeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_SecFee.Setup(this, IDC_OTHERFEE_SECFEE_EDIT);
	m_OrFee.Setup(this, IDC_OTHERFEE_ORFEE_EDIT);
	m_TotalFee.Setup(this, IDC_OTHERFEE_TOTAL_EDIT);
	ComputeFees();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BEGIN_MESSAGE_MAP(COtherFeeDlg, CDialog)
END_MESSAGE_MAP()

// COtherFeeDlg message handlers
