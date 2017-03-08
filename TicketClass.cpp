// TicketClass.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "TicketClass.h"
#include "qdata.h"

// CTicketClass

IMPLEMENT_DYNCREATE(CTicketClass, CFormView)

CTicketClass::CTicketClass()
	: CFormView(CTicketClass::IDD)
{
	m_TransType = NULL;
	m_CitiClass = NULL;
}

CTicketClass::~CTicketClass()
{
	if(m_TransType)
		delete m_TransType;
	if(m_CitiClass)
		delete m_CitiClass;
}

void CTicketClass::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRADECLASS_LIST, m_SS);
}

BEGIN_MESSAGE_MAP(CTicketClass, CFormView)
	ON_BN_CLICKED(IDC_TRADECLASS_UPDATE_BUTTON, &CTicketClass::OnBnClickedTradeclassUpdateButton)
	ON_BN_CLICKED(IDC_TRADECLASS_DELETE_BUTTON, &CTicketClass::OnBnClickedTradeclassDeleteButton)
	ON_EN_CHANGE(IDC_TRADECLASS_DATE_EDIT, &CTicketClass::OnEnChangeTradeclassDateEdit)
	ON_BN_CLICKED(IDC_TRADECLASS_LOAD_BUTTON, &CTicketClass::OnBnClickedTradeclassLoadButton)
END_MESSAGE_MAP()


// CTicketClass diagnostics

#ifdef _DEBUG
void CTicketClass::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTicketClass::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif

CNWIFMS70Doc* CTicketClass::GetDocument() // non-debug version is inline
{
	return STATIC_DOWNCAST(CNWIFMS70Doc, m_pDocument);
}

#endif //_DEBUG

void CTicketClass::AddRecord(const CString Ticket, const CString CitiClass)
{
	m_OraLoader.ExecuteSql("INSERT INTO SEMAM.NW_TICKET_CLASS VALUES "
							"(" + Ticket + ", " + CitiClass + ")");
}

void CTicketClass::DeleteRecord(const CString Ticket)
{
	m_OraLoader.ExecuteSql("DELETE FROM SEMAM.NW_TICKET_CLASS WHERE TICKET_NUM = " + Ticket);
}

BOOL CTicketClass::IsExist(const CString Ticket)
{
	m_OraLoader.Open("SELECT COUNT(*) FROM SEMAM.NW_TICKET_CLASS WHERE TICKET_NUM = " + Ticket);
	return m_OraLoader.GetCount();
}

void CTicketClass::LoadRecord()
{
	if(m_SS.GetSheetCurRow() <= 0)
		return;
	CString Data;

	for(int i = 1; i <= m_SS.GetSheetCols(); i++)
	{
		Data = m_SS.GetSheetText(i, m_SS.GetSheetCurRow());
		switch(i)
		{
			case 1:
				m_Ticket.SetData(Data);
				break;
			case 2:
				m_Asset.SetWindowText(Data);
				break;
			case 3:
				m_AssetDesc.SetWindowText(Data);
				break;
			case 4:
				m_TransType->SelectString(0, Data);
				break;
			case 5:
				m_Strike.SetWindowText(Data);
				break;
			case 6:
				m_Rate.SetWindowText(Data);
				break;
			case 7:
				m_Maturity.SetWindowText(Data);
				break;
			case 8: 
				m_CitiClass->SelectString(0, Data);
				break;
			default:
				break;
		}
	}
}

void CTicketClass::UpdateRecord(const CString Ticket, const CString CitiClass)
{
	m_OraLoader.ExecuteSql("UPDATE SEMAM.NW_TICKET_CLASS SET CITI_CLASS = " 
							+ CitiClass + " WHERE TICKET_NUM = " + Ticket);
}

void CTicketClass::InitControls()
{
	m_SS.SetVisibleCols(8);
	m_SS.SetVisibleRows(10);

	m_OraLoader.SetDB(&theDB);

	m_Ticket.Setup(this, IDC_TRADECLASS_TICKET_EDIT);
	m_Asset.Setup(this, IDC_TRADECLASS_ASSET_EDIT);
	m_AssetDesc.Setup(this, IDC_TRADECLASS_DESC_EDIT);
	m_Strike.Setup(this, IDC_TRADECLASS_STRIKE_EDIT);
	m_Rate.Setup(this, IDC_TRADECLASS_RATE_EDIT);
	m_Date.Setup(this, IDC_TRADECLASS_DATE_EDIT);
	m_Maturity.Setup(this, IDC_TRADECLASS_MATURITY_EDIT);
	m_TransType = (COptComboBox*) new COptComboBox(this, IDC_TRADECLASS_TYPE_COMBO);
	m_CitiClass = (COptComboBox*) new COptComboBox(this, IDC_TRADECLASS_CITICLASS_COMBO);
}

BOOL CTicketClass::IsOK()
{
	CString Text;

	if(m_Ticket.GetData().IsEmpty())
		Text = "Must have a ticket number";

	if(m_CitiClass->GetCurSel() < 0)
		Text = "Must select a Citi Class";

	if(Text.IsEmpty())
		return TRUE;

	MessageBox(Text);
	return FALSE;
}

BOOL CTicketClass::UpdateData(BOOL bSaveandValid)
{
	CQData QData;
	CString Date;

	if(bSaveandValid)
	{
		CString Ticket, CitiClass, qCitiClass;
		
		BeginWaitCursor();
		Ticket = m_Ticket.GetData();
		m_CitiClass->GetSelString(CitiClass);
		m_SS.GetSheetCurRow();

		Ticket = QData.GetQueryText(Ticket);
		qCitiClass = QData.GetQueryText(CitiClass);
		if(IsExist(Ticket))
			UpdateRecord(Ticket, qCitiClass);
		else
			AddRecord(Ticket, qCitiClass);
		m_SS.SetSheetText(8, m_SS.GetSheetCurRow(), CitiClass);
		EndWaitCursor();
	}
	else
	{	
		BeginWaitCursor();

		if(!m_bLoaded)
		{
			GetData().LoadSupportData(TRUE);
			QData.CopyDBRecArrayToComboBox(GetData().GetTransTypeArr(), *m_TransType);
			GetData().GetOraLoader().Open("SELECT CITI_CLASS FROM SEMAM.NW_CITI_CLASS ORDER BY 1 ");
			GetData().GetOraLoader().LoadComboBox(*m_CitiClass);
			GetData().GetOraLoader().Today(Date);
			m_Date.SetData(Date);
			m_bLoaded = TRUE;
		}
		
		Date = m_Date.GetData();
		Date = QData.GetQueryDate(Date);
		m_OraLoader.Open("SELECT DISTINCT A.TICKET_NUM, A.ASSET_CODE, B.ASS_DESC, A.TRANS_TYPE, "
						"A.EXERCISE_PRICE, A.TR_RATE, "
						"DECODE(A.TRANS_TYPE, 'REPO', A.MATURITY_DATE, A.OPT_EXPIRATION), "
						"C.CITI_CLASS "
						"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_ASSETS B, SEMAM.NW_TICKET_CLASS C "
						"WHERE B.ASS_CODE = A.ASSET_CODE "
						"AND C.TICKET_NUM(+) = A.TICKET_NUM "
						"AND A.TRANS_TYPE IN ('CALL', 'PUT', 'REPO') "
						"AND A.TRADE_DATE = " + Date + 
						"ORDER BY 1, 2 ");
		m_OraLoader.LoadDBSheet(m_SS);

		if(m_SS.GetSheetRows() > 0)
			OnDblClickTradeclassList(1, 1);
		EndWaitCursor();
	}

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
// CTicketClass message handlers
void CTicketClass::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	BeginWaitCursor();
	InitControls();
	m_bLoaded = FALSE;
	UpdateData(FALSE);
	GetDlgItem(IDC_TRADECLASS_DELETE_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_TRADECLASS_UPDATE_BUTTON)->EnableWindow(FALSE);
	EndWaitCursor();
}

// CTicketClass message handlers
BEGIN_EVENTSINK_MAP(CTicketClass, CFormView)
	ON_EVENT(CTicketClass, IDC_TRADECLASS_LIST, 11, CTicketClass::OnDblClickTradeclassList, VTS_I4 VTS_I4)
	ON_EVENT(CTicketClass, IDC_TRADECLASS_LIST, 2, CTicketClass::OnBlockSelectedTradeclassList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

void CTicketClass::OnDblClickTradeclassList(long Col, long Row)
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		m_SS.SetSheetCurRow(Row);
		LoadRecord();
	}

	GetDlgItem(IDC_TRADECLASS_UPDATE_BUTTON)->EnableWindow(m_SS.GetSheetCurRow() > 0);
}

void CTicketClass::OnBlockSelectedTradeclassList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	OnDblClickTradeclassList(BlockCol, BlockRow);
	GetDlgItem(IDC_TRADECLASS_DELETE_BUTTON)->EnableWindow(m_SS.GetSheetCurRow() > 0);
}

void CTicketClass::OnBnClickedTradeclassUpdateButton()
{
	if(IsOK())
	{
		UpdateData(TRUE);
		GetDlgItem(IDC_TRADECLASS_UPDATE_BUTTON)->EnableWindow(FALSE);
	}
}

void CTicketClass::OnBnClickedTradeclassDeleteButton()
{
	CQData QData;

	DeleteRecord(QData.GetQueryNumber(m_Ticket.GetData()));
	m_SS.DeleteSheetRow(m_SS.GetSheetCurRow());
	GetDlgItem(IDC_TRADECLASS_DELETE_BUTTON)->EnableWindow(FALSE);
}

void CTicketClass::OnEnChangeTradeclassDateEdit()
{
	GetDlgItem(IDC_TRADECLASS_LOAD_BUTTON)->EnableWindow(!m_Date.GetData().IsEmpty());
}

void CTicketClass::OnBnClickedTradeclassLoadButton()
{
	UpdateData(FALSE);
}
