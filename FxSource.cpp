// FxSource.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "FxSource.h"
#include "qdata.h"


// CFxSource dialog

IMPLEMENT_DYNAMIC(CFxSource, CDialog)

CFxSource::CFxSource(CWnd* pParent /*=NULL*/)
	: CDialog(CFxSource::IDD, pParent)
{
	m_pData = NULL;
}

CFxSource::~CFxSource()
{
}

void CFxSource::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FX_LIST, m_SS);
	DDX_Control(pDX, IDC_FX_SEL_LIST, m_SelSS);
}


BEGIN_MESSAGE_MAP(CFxSource, CDialog)
END_MESSAGE_MAP()

void CFxSource::UpdateData(BOOL bAdd)
{
	double Amount;
	int i;
	CStringArray Data;
	CString InvNum;
	CQData QData;

	if(bAdd)
	{
		if(m_SS.GetSheetCurRow() > 0)
		{
			InvNum = m_SS.GetSheetText(m_SS.GetSheetCols(), m_SS.GetSheetCurRow());
			Data.RemoveAll();
			for(i = 1; i <= m_SS.GetSheetCols(); i++)
				Data.Add(m_SS.GetSheetText(i, m_SS.GetSheetCurRow()));
			
			m_SelSS.SetSheetCols(m_SS.GetSheetCols());
			for(i = 1; i <= m_SelSS.GetSheetRows(); i++)
				if(InvNum == m_SelSS.GetSheetText(m_SS.GetSheetCols(), i))
					return;

			m_SelSS.AddSheetRow(Data);
		}
	}

	Amount = 0;
	for(i = 1; i <= m_SelSS.GetSheetRows(); i++)
		Amount += atof(QData.RemoveComma(m_SelSS.GetSheetText(m_SelSS.GetSheetCols() - 1, i)));

	m_Amount.SetWindowText(QData.WriteNumber(Amount, TRUE, 2));
	m_Remain.SetWindowText(QData.WriteNumber(atof(m_sNomAmount) + Amount, TRUE, 2));
}

// CFxSource message handlers
BOOL CFxSource::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	BeginWaitCursor();
	m_SS.SetVisibleCols(5);
	m_SS.SetVisibleRows(10);

	m_SelSS.SetVisibleCols(5);
	m_SelSS.SetVisibleRows(10);

	m_Ticket.Setup(this, IDC_FX_TICKET_EDIT);
	m_Currency.Setup(this, IDC_FX_CURRENCY_EDIT);
	m_ValueDate.Setup(this, IDC_FX_VALUEDATE_EDIT);
	m_Fxrate.Setup(this, IDC_FX_FXRATE_EDIT);
	m_NomAmount.Setup(this, IDC_FX_NOMAMOUNT_EDIT);
	m_Amount.Setup(this, IDC_FX_AMOUNT_EDIT);
	m_Remain.Setup(this, IDC_FX_REMAIN_EDIT);

	m_Ticket.SetData(m_sTicket);
	m_Currency.SetWindowText(m_sCurrency);
	m_ValueDate.SetWindowText(m_sValueDate);
	m_Fxrate.SetWindowText(m_sFxrate);
	m_NomAmount.SetWindowText(m_sNomAmount);

	if(m_pData)
	{
		COraLoader OraLoader = m_pData->GetOraLoader();
		
		OraLoader.GetSql().Format("SELECT PAY_CUR, VALUE_DATE, PORTFOLIO, COUNTERPARTY, "
								"A.ASS_CODE, A.TRANS_NUM, ASS_DESC, "
								"DECODE(DIR, 'P', -1, 'S', 1)*DECODE(TRANS_TYPE, 'LEVERAGE', -1, 'INT. SWAP', -1, 1)*"
									"DECODE(TR_DESC, 'MATURITY', -1, 'FX-BKING', -1, 1)*"
									"(NVL(AMOUNT, 0) + NVL(INTEREST, 0)), INV_NUM "
								"FROM SEMAM.NW_OPEN_TRADE_2 A, SEMAM.NW_ASSETS B "
								"WHERE A.ASS_CODE = B.ASS_CODE "
								"AND A.PAY_CUR = '%s' "
								"AND (A.TRANS_TYPE IN ('CALL', 'CASH', 'CDS', 'INT. SWAP', "
								"'LEVERAGE', 'PUT', 'REPO', 'SPR.SW.CLL', 'SPR.SW.PUT') OR "
								"(A.TRANS_TYPE = 'SECURITIES' AND A.ASSET_CLASS != 'CURRENCY FWDS') "
									"AND B.ASS_CONV_TYPE IS NULL) "
								"AND 0 = (SELECT COUNT(*) FROM SEMAM.NW_TR_INV C "
										"WHERE A.INV_NUM = C.INV_NUM AND C.FOREX_REF_NUM IS NOT NULL) "
								"ORDER BY 1, 2, 3, 4 ", (LPCTSTR) m_sCurrency);
		OraLoader.Open();
		OraLoader.GetFieldArray().GetAt(5)->SetWithComma(FALSE);
		OraLoader.GetFieldArray().GetAt(OraLoader.GetFieldArray().GetSize() - 1)->SetWithComma(FALSE);
		OraLoader.LoadDBSheet(m_SS);
	}
	EndWaitCursor();
	
	return TRUE;
}

BEGIN_EVENTSINK_MAP(CFxSource, CDialog)
	ON_EVENT(CFxSource, IDC_FX_SEL_LIST, 11, CFxSource::OnDblClickFxSelList, VTS_I4 VTS_I4)
	ON_EVENT(CFxSource, IDC_FX_LIST, 11, CFxSource::OnDblClickFxList, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

void CFxSource::OnDblClickFxSelList(long Col, long Row)
{
	if(Row >= 1 && Row <= m_SelSS.GetSheetRows())
	{
		m_SelSS.DeleteSheetRow(Row);
		UpdateData(FALSE);
	}
}

void CFxSource::OnDblClickFxList(long Col, long Row)
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		m_SS.SetSheetCurRow(Row);
		UpdateData(TRUE);
	}
}

void CFxSource::OnOK()
{
	m_RecArray.RemoveAll();

	CDBRec Rec;
	
	for(int i = 1; i <= m_SelSS.GetSheetRows(); i++)
	{
		Rec.RemoveAll();
		Rec.Add(m_SelSS.GetSheetText(m_SelSS.GetSheetCols(), i)); // InvNum
		Rec.Add(m_SelSS.GetSheetText(3, i)); // Portfolio
		Rec.Add(m_SelSS.GetSheetText(m_SelSS.GetSheetCols() - 1, i)); // Amount
		m_RecArray.Add(Rec);
	}

	CDialog::OnOK();
}


