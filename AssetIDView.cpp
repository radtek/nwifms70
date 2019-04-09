// AssetIDView.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "AssetIDView.h"
#include "qdata.h"


// CAssetIDView

IMPLEMENT_DYNCREATE(CAssetIDView, CFormView)

CAssetIDView::CAssetIDView()
	: CFormView(CAssetIDView::IDD)
{
	m_Row = 0;
}

void CAssetIDView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ASSETID_LIST, m_SS);
}

BOOL CAssetIDView::UpdateData(BOOL bSaveandValid)
{
	COraLoader OraLoader;

	BeginWaitCursor();

	OraLoader = GetData().GetOraLoader();

	if(bSaveandValid)
	{
		CString Fund, AssetCode, pID;
		CQData QData;

		Fund = m_Portfolio.GetData();
		AssetCode = m_AssetCode.GetData();
		pID = m_PID.GetData();

		if(Fund.IsEmpty() || AssetCode.IsEmpty())
			return TRUE;

		Fund = QData.GetQueryText(Fund);
		AssetCode = QData.GetQueryText(AssetCode);

		if(!pID.IsEmpty())
		{
			pID = QData.GetQueryText(pID);

			OraLoader.Open("SELECT COUNT(*) FROM SEMAM.NW_ASSET_ID WHERE PORTFOLIO = " + 
							Fund + " AND ASSET_CODE = " + AssetCode);

			if(OraLoader.GetCount() > 0)
				OraLoader.ExecuteSql("UPDATE SEMAM.NW_ASSET_ID SET PID = " + pID + 
									" WHERE PORTFOLIO " + Fund + " AND ASSET_CODE = " + AssetCode);
			else
				OraLoader.ExecuteSql("INSERT INTO SEMAM.NW_ASSET_ID VALUES (" + Fund + ", " + 
									AssetCode + ", " + pID + ")");
		}
		else
			OraLoader.ExecuteSql("DELETE FROM SEMAM.NW_ASSET_ID WHERE PORTFOLIO = " + Fund + 
									" AND ASSET_CODE = " + AssetCode);
		UpdateData(FALSE);
	}
	else
	{
		GetData().LoadSupportData();
		GetData().GetTransTypeArr().CopyToComboBox(m_TransType);
		GetData().GetPortfolioArr().CopyToComboBox(m_Portfolio);

		OraLoader.Open("SELECT A.PORTFOLIO, TRANS_TYPE, A.ASSET_CODE, ASS_DESC, RATE, ASS_MATURITY_DATE, PID, "
						"SUM(DECODE(DIR, 'P', 1, 'S', -1)*NOM_AMOUNT) \"NOM_AMOUNT\" "
						"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B, SEMAM.NW_ASS_PERIODS C, "
						"SEMAM.NW_ASSET_ID D, SEMAM.NW_PORTFOLIOS Z "
						"WHERE B.ASS_CODE = A.ASSET_CODE "
						"AND C.ASS_CODE(+) = A.ASSET_CODE "
						"AND C.ASS_FROM(+) <= TRUNC(SYSDATE) "
						"AND C.ASS_TO(+) + DECODE(NVL(C.ACTION(+), 'A'), 'INCLUSIVE', 1, 0) > TRUNC(SYSDATE) "
						"AND D.PORTFOLIO(+) = A.PORTFOLIO "
						"AND D.ASSET_CODE(+) = A.ASSET_CODE "
						"AND Z.PORTFOLIO = A.PORTFOLIO "
						"AND Z.STATUS IS NULL "
						"AND A.TRANS_TYPE IN ('CDS') "
						"GROUP BY A.PORTFOLIO, TRANS_TYPE, A.ASSET_CODE, ASS_DESC, RATE, ASS_MATURITY_DATE, PID "
						"HAVING ABS(SUM(DECODE(DIR, 'P', 1, 'S', -1)*NOM_AMOUNT)) > 0 "
						"UNION "
						"SELECT A.PORTFOLIO, TRANS_TYPE, A.ASSET_CODE, ASS_DESC, RATE, "
						"NVL(ACTUAL_VDATE, NVL(MATURITY_DATE, ASS_MATURITY_DATE)) \"MATURITY\", "
						"PID, DECODE(DIR, 'P', 1, 'S', -1)*NOM_AMOUNT \"NOM_AMOUNT\" "
						"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B, SEMAM.NW_ASS_PERIODS C, "
						"SEMAM.NW_ASSET_ID D, SEMAM.NW_PORTFOLIOS Z "
						"WHERE B.ASS_CODE = A.ASSET_CODE "
						"AND B.ASS_INT_TYPE = 'FIXED' "
						"AND C.ASS_CODE(+) = A.ASSET_CODE "
						"AND C.ASS_FROM(+) <= TRUNC(SYSDATE) "
						"AND C.ASS_TO(+) + DECODE(NVL(C.ACTION(+), 'A'), 'INCLUSIVE', 1, 0) > TRUNC(SYSDATE) "
						"AND D.PORTFOLIO(+) = A.PORTFOLIO "
						"AND D.ASSET_CODE(+) = A.ASSET_CODE "
						"AND Z.PORTFOLIO = A.PORTFOLIO "
						"AND Z.STATUS IS NULL "
						"AND A.TRANS_TYPE IN ('INT. SWAP') "
						"AND NVL(ACTUAL_VDATE, NVL(MATURITY_DATE, NVL(ASS_MATURITY_DATE, TRUNC(SYSDATE) + 1))) > TRUNC(SYSDATE) "
						"UNION "
						"SELECT A.PORTFOLIO, TRANS_TYPE, A.ASSET_CODE, ASS_DESC, RATE, "
						"DECODE(ASS_INDUSTRY, 'CURRENCY FWDS', "
						"DECODE(SIGN(INSTR(ASS_DESC, 'FWD')), 1, VALUE_DATE, ASS_MATURITY_DATE)) \"MATURITY\", "
						"PID, SUM(DECODE(DIR, 'P', 1, 'S', -1)*NOM_AMOUNT) "
						"FROM SEMAM.ALL_TICKET_INV_V A, SEMAM.NW_ASSETS B, SEMAM.NW_ASS_PERIODS C, "
						"SEMAM.NW_ASSET_ID D, SEMAM.NW_PORTFOLIOS Z "
						"WHERE B.ASS_CODE = A.ASSET_CODE "
						"AND C.ASS_CODE(+) = A.ASSET_CODE "
						"AND C.ASS_FROM(+) = TRUNC(SYSDATE) "
						"AND C.ASS_TO(+) + DECODE(NVL(C.ACTION(+), 'A'), 'INCLUSIVE', 1, 0) = TRUNC(SYSDATE) "
						"AND D.PORTFOLIO(+) = A.PORTFOLIO "
						"AND D.ASSET_CODE(+) = A.ASSET_CODE "
						"AND Z.PORTFOLIO = A.PORTFOLIO "
						"AND Z.STATUS IS NULL "
						"AND A.TRANS_TYPE IN ('SECURITIES') "
						"GROUP BY A.PORTFOLIO, A.TRANS_TYPE, A.ASSET_CODE, ASS_DESC, RATE, "
						"DECODE(ASS_INDUSTRY, 'CURRENCY FWDS', "
							"DECODE(SIGN(INSTR(ASS_DESC, 'FWD')), 1, VALUE_DATE, ASS_MATURITY_DATE)), PID "
						"HAVING ABS(SUM(DECODE(DIR, 'P', 1, 'S', -1)*NOM_AMOUNT)) > 0 "
						"ORDER BY 1, 2, 3 ");
		 OraLoader.LoadDBSheet(m_SS);
	}
	EndWaitCursor();

	return TRUE;
}

BEGIN_MESSAGE_MAP(CAssetIDView, CFormView)
	ON_EN_CHANGE(IDC_ASSETID_PID_EDIT, &CAssetIDView::OnEnChangeAssetidPidEdit)
	ON_BN_CLICKED(IDC_ASSETID_UPDATE_BUTTON, &CAssetIDView::OnBnClickedAssetidUpdateButton)
	ON_BN_CLICKED(IDC_ASSETID_DELETE_BUTTON, &CAssetIDView::OnBnClickedAssetidDeleteButton)
END_MESSAGE_MAP()


// CAssetIDView diagnostics

#ifdef _DEBUG
void CAssetIDView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAssetIDView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif

CNWIFMS70Doc* CAssetIDView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNWIFMS70Doc)));
	return (CNWIFMS70Doc*)m_pDocument;
}
#endif //_DEBUG


// CAssetIDView message handlers

void CAssetIDView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	BeginWaitCursor();

	m_SS.SetVisibleCols(8);
	m_SS.SetVisibleRows(20);
	m_SS.LockSheet();

	m_Portfolio.Setup(this, IDC_ASSETID_PORTFOLIO_COMBO);
	m_TransType.Setup(this, IDC_ASSETID_TRANSTYPE_COMBO);

	m_AssetCode.Setup(this, IDC_ASSETID_ASSETCODE_EDIT);
	m_AssetCode.SetReadOnly(TRUE);
	m_Asset.Setup(this, IDC_ASSETID_ASSET_EDIT);
	m_Asset.SetReadOnly(TRUE);
	m_Nominal.Setup(this, IDC_ASSETID_NOMINAL_EDIT);
	m_Nominal.SetReadOnly(TRUE);
	m_Rate.Setup(this, IDC_ASSETID_RATE_EDIT);
	m_Rate.SetReadOnly(TRUE);
	m_Maturity.Setup(this, IDC_ASSETID_MATURITY_EDIT);
	m_Maturity.SetReadOnly(TRUE);
	m_PID.Setup(this, IDC_ASSETID_PID_EDIT);
	m_PID.LimitText(25);

	UpdateData(FALSE);
	GetDlgItem(IDC_ASSETID_UPDATE_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_ASSETID_DELETE_BUTTON)->EnableWindow(FALSE);
	EndWaitCursor();
}

BEGIN_EVENTSINK_MAP(CAssetIDView, CFormView)
ON_EVENT(CAssetIDView, IDC_ASSETID_LIST, 11, CAssetIDView::OnDblClickAssetidList, VTS_I4 VTS_I4)
ON_EVENT(CAssetIDView, IDC_ASSETID_LIST, 2, CAssetIDView::OnBlockSelectedAssetidList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

void CAssetIDView::OnDblClickAssetidList(long Col, long Row)
{
	if(Row > 0 && Row <= m_SS.GetSheetRows())
	{
		CString Text;

		m_Row = Row;

		for(int i = 1; i <= 8; i ++)
		{
			Text = m_SS.GetSheetText(i, m_Row);
			switch(i)
			{
				case 1:
					m_Portfolio.SetData(Text);
					break;
				case 2:
					m_TransType.SetData(Text);
					break;
				case 3:
					m_AssetCode.SetData(Text);
					break;
				case 4:
					m_Asset.SetData(Text);
					break;
				case 5:
					m_Rate.SetData(Text);
					break;
				case 6:
					m_Maturity.SetData(Text);
					break;
				case 7:
					m_PID.SetData(Text);
					break;
				case 8:
					m_Nominal.SetData(Text);
					break;
				default:
					break;
			}
		}
	}
	else
	{
		m_Portfolio.SetCurSel(-1);
		m_TransType.SetCurSel(-1);
		m_AssetCode.SetData(EMPTYSTRING);
		m_Asset.SetData(EMPTYSTRING);
		m_Nominal.SetData(EMPTYSTRING);
		m_Rate.SetData(EMPTYSTRING);
		m_Maturity.SetData(EMPTYSTRING);
		m_PID.SetData(EMPTYSTRING);
		m_Row = 0;
	}
}

void CAssetIDView::OnBlockSelectedAssetidList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	OnDblClickAssetidList(BlockCol, BlockRow);
	GetDlgItem(IDC_ASSETID_DELETE_BUTTON)->EnableWindow(TRUE);
}

void CAssetIDView::OnEnChangeAssetidPidEdit()
{
	GetDlgItem(IDC_ASSETID_UPDATE_BUTTON)->EnableWindow(m_PID.GetWindowTextLength() > 0);
}

void CAssetIDView::OnBnClickedAssetidUpdateButton()
{
	UpdateData();
	GetDlgItem(IDC_ASSETID_UPDATE_BUTTON)->EnableWindow(FALSE);
}

void CAssetIDView::OnBnClickedAssetidDeleteButton()
{
	if(MessageBox("Do you want to delete this record?", "Asset ID", MB_YESNO) == IDYES)
	{
		m_PID.SetData(EMPTYSTRING);
		UpdateData();
	}

	GetDlgItem(IDC_ASSETID_DELETE_BUTTON)->EnableWindow(FALSE);
}
