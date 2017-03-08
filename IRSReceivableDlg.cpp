// IRSReceivableDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "IRSReceivableDlg.h"
#include "qdata.h"


// CIRSReceivableDlg dialog

IMPLEMENT_DYNAMIC(CIRSReceivableDlg, CDialog)

CIRSReceivableDlg::CIRSReceivableDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIRSReceivableDlg::IDD, pParent)
{
	m_Portfolio = NULL;
	m_Currency = NULL;
}

CIRSReceivableDlg::~CIRSReceivableDlg()
{
	if(m_Portfolio)
		delete m_Portfolio;
	if(m_Currency)
		delete m_Currency;
}

void CIRSReceivableDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IRS_LIST, m_SS);
	DDX_Control(pDX, IDC_IRS_SEL_LIST, m_SelSS);
}


BEGIN_MESSAGE_MAP(CIRSReceivableDlg, CDialog)
	ON_BN_CLICKED(IDC_IRS_UPDATE_BUTTON, &CIRSReceivableDlg::OnBnClickedIrsUpdateButton)
	ON_BN_CLICKED(IDC_IRS_DELETE_BUTTON, &CIRSReceivableDlg::OnBnClickedIrsDeleteButton)
	ON_BN_CLICKED(IDC_IRS_LOAD_BUTTON, &CIRSReceivableDlg::OnBnClickedIrsLoadButton)
END_MESSAGE_MAP()


// CIRSReceivableDlg message handlers
BEGIN_EVENTSINK_MAP(CIRSReceivableDlg, CDialog)
	ON_EVENT(CIRSReceivableDlg, IDC_IRS_SEL_LIST, 2, CIRSReceivableDlg::OnBlockSelectedIrsSelList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CIRSReceivableDlg, IDC_IRS_SEL_LIST, 11, CIRSReceivableDlg::OnDblClickIrsSelList, VTS_I4 VTS_I4)
	ON_EVENT(CIRSReceivableDlg, IDC_IRS_LIST, 11, CIRSReceivableDlg::OnDblClickIrsList, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

BOOL CIRSReceivableDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	BeginWaitCursor();
	m_SS.SetVisibleCols(5);
	m_SS.SetVisibleRows(10);

	m_SelSS.SetVisibleCols(4);
	m_SelSS.SetVisibleRows(10);

	m_FromDate.Setup(this, IDC_IRS_FROMDATE_EDIT);
	m_ToDate.Setup(this, IDC_IRS_TODATE_EDIT);
	m_Portfolio = new COptComboBox(this, IDC_IRS_PORTFOLIO_COMBO);
	m_Currency = new COptComboBox(this, IDC_IRS_CURRENCY_COMBO);
	m_TransNum.Setup(this, IDC_IRS_TRANSNUM_EDIT);
	m_Interest.Setup(this, IDC_IRS_INTEREST_EDIT);

	if(m_pData)
	{
		m_pData->GetPortfolioArr().CopyToComboBox(*m_Portfolio);
		m_pData->GetCurrencyArr().CopyToComboBox(*m_Currency);
	
		COraLoader OraLoader = m_pData->GetOraLoader();
		CString Date;

		OraLoader.Open("SELECT LAST_DAY(SYSDATE) FROM DUAL");
		OraLoader.LoadTextString(Date);

		m_FromDate.SetData("01/" + Date.Right(5));
		m_ToDate.SetData(Date);

		OraLoader.Open("SELECT FROM_DATE, TO_DATE, PORTFOLIO, CURRENCY, TRANS_NUM, ACCRUAL "
						"FROM SEMAM.NW_IRS_RECEIVABLE A "
						"WHERE FROM_DATE >= TRUNC(SYSDATE) AND TO_DATE <= TRUNC(SYSDATE) ORDER BY 1, 2, 3 ");
		OraLoader.LoadDBSheet(m_SelSS);

		OraLoader.GetSql().Format("SELECT PORTFOLIO, CURRENCY, TRANS_NUM, L_ACCRUAL, ASS_TO "
						"FROM SEMAM.NW_IRS_RECEIVABLE_ACCRUAL "
						"WHERE ASS_TO >= '%s' AND ASS_TO <= '%s' ORDER BY 1, 2, 3 ",
						(LPCTSTR) m_FromDate.GetData(), (LPCTSTR) m_ToDate.GetData());
		OraLoader.Open();
		OraLoader.LoadDBSheet(m_SS);
	}
	EndWaitCursor();
	
	return TRUE;
}

void CIRSReceivableDlg::OnBlockSelectedIrsSelList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	if(BlockRow >= 1 && BlockRow <= m_SelSS.GetSheetRows())
	{
		m_SelSS.SetSheetCurRow(BlockRow);
		GetDlgItem(IDC_IRS_DELETE_BUTTON)->EnableWindow(TRUE);
	}
	else
		GetDlgItem(IDC_IRS_DELETE_BUTTON)->EnableWindow(FALSE);
}

void CIRSReceivableDlg::OnDblClickIrsSelList(long Col, long Row)
{
	if(Row >= 1 && Row <= m_SelSS.GetSheetRows())
	{
		m_SelSS.SetSheetCurRow(Row);
		OnBnClickedIrsDeleteButton();
	}
}

void CIRSReceivableDlg::OnDblClickIrsList(long Col, long Row)
{
	GetDlgItem(IDC_IRS_LOAD_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_IRS_ADD_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_IRS_UPDATE_BUTTON)->EnableWindow(FALSE);

	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		m_Portfolio->SelectString(0, m_SS.GetSheetText(1, Row)); // Portfolio
		m_Currency->SelectString(0, m_SS.GetSheetText(2, Row));  // Currency
		m_TransNum.SetData(m_SS.GetSheetText(3, Row));  // TransNum
		m_Interest.SetWindowText(m_SS.GetSheetText(4, Row));  // Interest

		if(m_Portfolio->GetCurSel() >= 0 || m_Currency->GetCurSel() >= 0)
		{
			GetDlgItem(IDC_IRS_LOAD_BUTTON)->EnableWindow(TRUE);
			if(!m_TransNum.GetData().IsEmpty() && m_Interest.GetWindowTextLength() > 0)
			{
				GetDlgItem(IDC_IRS_ADD_BUTTON)->EnableWindow(TRUE);
				GetDlgItem(IDC_IRS_UPDATE_BUTTON)->EnableWindow(TRUE);
			}
		}
	}
	else
	{
		m_Portfolio->SetCurSel(-1);
		m_Currency->SetCurSel(-1);
		m_TransNum.SetData(EMPTYSTRING);
		m_Interest.SetWindowText(EMPTYSTRING);
	}
}

void CIRSReceivableDlg::OnBnClickedIrsUpdateButton()
{
	if(m_SelSS.GetSheetCurRow() >= 1)
	{
		COraLoader OraLoader;
		CString Portfolio, Currency, Interest;
		CQData QData;

		m_Portfolio->GetSelString(Portfolio);
		m_Currency->GetSelString(Currency);
		m_Interest.GetWindowText(Interest);

		OraLoader = m_pData->GetOraLoader();
		OraLoader.GetSql().Format("UPDATE SEMAM.NW_IRS_RECEIVABLE "
									"SET PORTFOLIO = %s, CURRENCY = '%s' AND ACCRUAL = %s ",
									(LPCTSTR) Portfolio, (LPCTSTR) Currency, 
									QData.RemoveComma(Interest));
		OraLoader.ExecuteSql();
	}
}

void CIRSReceivableDlg::OnBnClickedIrsDeleteButton()
{
	if(m_SelSS.GetSheetCurRow() >= 1)
	{
		COraLoader OraLoader;

		OraLoader = m_pData->GetOraLoader();
		OraLoader.GetSql().Format("DELETE SEMAM.NW_IRS_RECEIVABLE "
									"SET TRANS_NUM = %s ", (LPCTSTR) m_TransNum.GetData());
		OraLoader.ExecuteSql();
	}
}

void CIRSReceivableDlg::OnBnClickedIrsLoadButton()
{
	CString Portfolio, Currency;

	m_Portfolio->GetSelString(Portfolio);
	m_Currency->GetSelString(Currency);

	COraLoader OraLoader;
	OraLoader = m_pData->GetOraLoader();
	
	if(Currency == "USD")
		OraLoader.GetSql().Format("SELECT PORTFOLIO, CURRENCY, TRANS_NUM, L_ACCRUAL, "
						"ASS_TO FROM SEMAM.NW_IRS_RECEIVABLE_ACCRUAL "
						"WHERE ASS_TO >= '%s' AND ASS_TO <= '%s' "
						"AND PORTFOLIO = '%s' AND Currency = '%s' "
						"UNION "
						"SELECT PORTFOLIO, 'USD', TRANS_NUM, ACCRUAL, ASS_TO "
						"FROM SEMAM.NW_IRS_RECEIVABLE_ACCRUAL "
						"WHERE ASS_TO >= '%s' AND ASS_TO <= '%s' "
						"AND PORTFOLIO = '%s' AND Currency != '%s' "
						"ORDER BY 1, 2, 3 ", (LPCTSTR) m_FromDate.GetData(), 
						(LPCTSTR) m_ToDate.GetData(), (LPCTSTR) Portfolio, 
						(LPCTSTR) Currency, (LPCTSTR) m_FromDate.GetData(), 
						(LPCTSTR) m_ToDate.GetData(), (LPCTSTR) Portfolio, 
						(LPCTSTR) Currency);
	else
		OraLoader.GetSql().Format("SELECT PORTFOLIO, CURRENCY, TRANS_NUM, L_ACCRUAL, "
						"ASS_TO FROM SEMAM.NW_IRS_RECEIVABLE_ACCRUAL "
						"WHERE ASS_TO >= '%s' AND ASS_TO <= '%s' "
						"AND PORTFOLIO = '%s' AND Currency = '%s' ORDER BY 1, 2, 3 ",
						(LPCTSTR) m_FromDate.GetData(), (LPCTSTR) m_ToDate.GetData(), 
						(LPCTSTR) Portfolio, (LPCTSTR) Currency);

	OraLoader.Open();
	m_SS.ClearSheet();
	OraLoader.LoadDBSheet(m_SS);
}

