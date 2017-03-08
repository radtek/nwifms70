// IdeaApprovalView.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "IdeaApprovalView.h"
#include "qdata.h"

// CIdeaApprovalView

IMPLEMENT_DYNCREATE(CIdeaApprovalView, CFormView)

CIdeaApprovalView::CIdeaApprovalView()
	: CFormView(CIdeaApprovalView::IDD)
{
	m_Action = NULL;
	m_Aim = NULL;
	m_Dir = NULL;
	m_Currency = NULL;
	m_Counterparty = NULL;
	m_Trader = NULL;
	m_Risk = NULL;
	m_Reward = NULL;
	m_Decision = NULL;
}

CIdeaApprovalView::~CIdeaApprovalView()
{
	if(m_Action)
		delete m_Action;
	if(m_Aim)
		delete m_Aim;
	if(m_Dir)
		delete m_Dir;
	if(m_Currency)
		delete m_Currency;
	if(m_Counterparty)
		delete m_Counterparty;
	if(m_Trader)
		delete m_Trader;
	if(m_Risk)
		delete m_Risk;
	if(m_Reward)
		delete m_Reward;
	if(m_Decision)
		delete m_Decision;
}

void CIdeaApprovalView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IDEAA_LIST, m_SS);
}

void CIdeaApprovalView::InitControls()
{
	m_SS.SetVisibleCols(11);
	m_SS.SetVisibleRows(10);
	m_SS.SetSheetCols(7);
	m_SS.SetSheetRows(m_SS.GetMaxRows());
	m_SS.LockSheet();
	m_SS.SetSheetRows(0);
	m_SS.SetSheetCurRow(-1);

	m_IdeaID.Setup(this, IDC_IDEAA_ID_EDIT);
	m_IdeaID.SetReadOnly(TRUE);

	m_Date.Setup(this, IDC_IDEAA_DATE_EDIT);
	m_Date.SetReadOnly(TRUE);
	m_Action = new COptComboBox(this, IDC_IDEAA_ACTION_COMBO);
	m_Action->SetReadOnly(TRUE);
	m_Aim = new COptComboBox(this, IDC_IDEAA_AIM_COMBO);
	m_Aim->SetReadOnly(TRUE);
    m_Dir = new COptComboBox(this, IDC_IDEAA_DIR_COMBO);
	m_Dir->SetReadOnly(TRUE);
	m_Currency = new COptComboBox(this, IDC_IDEAA_CURRENCY_COMBO);
	m_Currency->SetReadOnly(TRUE);
    m_Counterparty = new COptComboBox(this, IDC_IDEAA_CP_COMBO);
	m_Counterparty->SetReadOnly(TRUE);
	m_Trader = new COptComboBox(this, IDC_IDEAA_TRADER_COMBO);
	m_Trader->SetReadOnly(TRUE);
	m_Risk = new COptComboBox(this, IDC_IDEAA_RISK_COMBO, TRUE);
	m_Risk->SetReadOnly(TRUE);
	m_Reward = new COptComboBox(this, IDC_IDEAA_REWARD_COMBO, TRUE);
	m_Reward->SetReadOnly(TRUE);
	m_Decision = new COptComboBox(this, IDC_IDEAA_DECISION_COMBO, TRUE);
	
	m_Capital.Setup(this, IDC_IDEAA_CAPITAL_EDIT);
	m_Capital.SetReadOnly(TRUE);
	m_ApprovedCapital.Setup(this, IDC_IDEAA_APPROVEDCAPITAL_EDIT);
	m_Idea.Setup(this, IDC_IDEAA_IDEA_EDIT);
	m_Idea.LimitText(512);
	m_Idea.SetReadOnly(TRUE);
	m_RiskNote.Setup(this, IDC_IDEAA_RISKNOTE_EDIT);
	m_RiskNote.LimitText(80);
	m_Note.Setup(this, IDC_IDEAA_NOTE_EDIT);
	m_Note.LimitText(80);

	if(m_bRiskManager)
	{
		m_Decision->SetReadOnly(TRUE);
		m_ApprovedCapital.SetReadOnly(TRUE);
		m_Note.SetReadOnly(TRUE);
	}
	else
		m_RiskNote.SetReadOnly(TRUE);

	CIdeaRec *pRec;
	pRec = &m_Data.GetIdeaRec();
	m_Data.Add(&m_IdeaID, &pRec->GetIdeaID());
	m_Data.Add(&m_Date, &pRec->GetDate());
	m_Data.Add(m_Action, &pRec->GetAction());
	m_Data.Add(m_Dir, &pRec->GetDir());
	m_Data.Add(m_Currency, &pRec->GetCurrency());
	m_Data.Add(m_Aim, &pRec->GetAim());
	m_Data.Add(m_Counterparty, &pRec->GetCounterparty());
	m_Data.Add(m_Trader, &pRec->GetTrader());
	m_Data.Add(&m_Capital, &pRec->GetCapital());
	m_Data.Add(m_Risk, &pRec->GetRisk());
	m_Data.Add(m_Reward, &pRec->GetReward());
	m_Data.Add(&m_Idea, &pRec->GetIdea());
	m_Data.Add(&m_RiskNote, &pRec->GetRiskNote());
	m_Data.Add(&m_ApprovedCapital, &pRec->GetApprovedAmt());
	m_Data.Add(&m_Note, &pRec->GetManagerNote());
	m_Data.Add(m_Decision, &pRec->GetDecision());

	m_Date.SetData("");
}

BOOL CIdeaApprovalView::UpdateData(BOOL bSaveandValid)
{
	if(!bSaveandValid)
	{
		CString Date;
		CQData QData;
		COraLoader OraLoader;

		BeginWaitCursor();
		OraLoader = m_Data.GetOraLoader();
		if(m_bRiskManager)
			m_Data.LoadIdeaRecords(2);
		else
			m_Data.LoadIdeaRecords(3);

		GetData().LoadSupportData();
		GetData().GetTransDirArr().CopyToComboBox(*m_Dir);	/* TransDir */
		GetData().GetCurrencyArr().CopyToComboBox(*m_Currency); /* Margin Currency */
		QData.CopyKeyDBListKeyToComboBox(GetData().GetContactList(), *m_Counterparty, FALSE); /* Counterparty */
		GetData().GetTraderArr().CopyToComboBox(*m_Trader); /* Trader */

		OraLoader.Open("SELECT TIMETOACT FROM SEMAM.NW_TRADE_IDEA_ACTION ORDER BY 1 ");
		OraLoader.LoadComboBox(*m_Action);
		OraLoader.Open("SELECT AIM FROM SEMAM.NW_TRADE_IDEA_AIM ORDER BY 1 ");
		OraLoader.LoadComboBox(*m_Aim);
		OraLoader.Open("SELECT RISK FROM SEMAM.NW_TRADE_IDEA_RISK ORDER BY 1 ");
		OraLoader.LoadComboBox(*m_Risk);
		OraLoader.Open("SELECT REWARD FROM SEMAM.NW_TRADE_IDEA_REWARD ORDER BY 1 ");
		OraLoader.LoadComboBox(*m_Reward);
		m_Decision->AddString("ACCEPT");
		m_Decision->AddString("HOLD");
		m_Decision->AddString("DENY");
		OraLoader.Today(Date);
		m_Date.SetData(Date);
		EndWaitCursor();
	}

	m_SS.SetSheetCurRow(-1);

	Invalidate();
	UpdateWindow();
	return TRUE;
}

BEGIN_MESSAGE_MAP(CIdeaApprovalView, CFormView)
	ON_COMMAND(ID_IDEASAPPROVAL_LOADIDEAS, &CIdeaApprovalView::OnIdeasapprovalLoadideas)
	ON_COMMAND(ID_IDEASAPPROVAL_LOADNEWIDEAS, &CIdeaApprovalView::OnIdeasapprovalLoadnewideas)
	ON_CBN_SELCHANGE(IDC_IDEAA_DECISION_COMBO, &CIdeaApprovalView::OnCbnSelchangeIdeaaDecisionCombo)
	ON_BN_CLICKED(IDC_IDEAA_UPDATE_BUTTON, &CIdeaApprovalView::OnBnClickedIdeaaUpdateButton)
	ON_EN_CHANGE(IDC_IDEAA_RISKNOTE_EDIT, &CIdeaApprovalView::OnEnChangeIdeaaRisknoteEdit)
END_MESSAGE_MAP()

void CIdeaApprovalView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	BeginWaitCursor();
	
	m_bRiskManager = GetData().GetRisk();
	InitControls();
	m_Data.Setup(GetData().GetOraLoader(), &m_SS);
	
	GetData().LoadDates();

	UpdateData(FALSE);
	EndWaitCursor();

	GetDlgItem(IDC_IDEAA_UPDATE_BUTTON)->EnableWindow(FALSE);
}

// CIdeaApprovalView diagnostics

#ifdef _DEBUG
void CIdeaApprovalView::AssertValid() const
{
	CFormView::AssertValid();
}

void CIdeaApprovalView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CNWIFMS70Doc* CIdeaApprovalView::GetDocument() // non-debug version is inline
{
	return STATIC_DOWNCAST(CNWIFMS70Doc, m_pDocument);
}
#endif //_DEBUG

BEGIN_EVENTSINK_MAP(CIdeaApprovalView, CFormView)
	ON_EVENT(CIdeaApprovalView, IDC_IDEAA_LIST, 11, CIdeaApprovalView::OnDblClickIdeaaList, VTS_I4 VTS_I4)
	ON_EVENT(CIdeaApprovalView, IDC_IDEAA_LIST, 2, CIdeaApprovalView::OnBlockSelectedIdeaaList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

// CIdeaApprovalView message handlers

void CIdeaApprovalView::OnIdeasapprovalLoadideas()
{
	if(m_bRiskManager)
		m_Data.LoadIdeaRecords(2);
	else
		m_Data.LoadIdeaRecords(3);
}

void CIdeaApprovalView::OnIdeasapprovalLoadnewideas()
{
	if(m_bRiskManager)
		m_Data.LoadIdeaRecords(2, TRUE);
	else
		m_Data.LoadIdeaRecords(3, TRUE);
}

void CIdeaApprovalView::OnDblClickIdeaaList(long Col, long Row)
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		m_Data.SetCurrentRow(Row);
		
		if(m_ApprovedCapital.GetWindowTextLength() == 0)
		{
			CString Text;
			
			m_Capital.GetWindowText(Text);
			m_ApprovedCapital.SetWindowText(Text);
		}
	}
}

void CIdeaApprovalView::OnBlockSelectedIdeaaList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	OnDblClickIdeaaList(BlockCol, BlockRow);
}

void CIdeaApprovalView::OnCbnSelchangeIdeaaDecisionCombo()
{
	GetDlgItem(IDC_IDEAA_UPDATE_BUTTON)->EnableWindow(!m_bRiskManager && 
														m_Decision->GetCurSel() >= 0 && 
														!m_IdeaID.GetData().IsEmpty());
}

void CIdeaApprovalView::OnBnClickedIdeaaUpdateButton()
{
	CString Decision, Capital, Note;

	if(m_bRiskManager)
	{
		m_RiskNote.GetWindowText(Note);
		m_Data.RiskApprove(m_IdeaID.GetData(), Note);
	}
	else
	{
		if(m_Decision->GetCurSel() > 0)
		{
			m_Decision->GetSelString(Decision);
			m_Note.GetWindowText(Note);

			if(Decision == "ACCEPT")
				m_Capital.GetWindowText(Capital);
			else
				Capital = "0";

			m_Data.ApproveIdea(m_IdeaID.GetData(), Decision, Capital, Note);
		}
	}
}

void CIdeaApprovalView::OnEnChangeIdeaaRisknoteEdit()
{
	GetDlgItem(IDC_IDEAA_UPDATE_BUTTON)->EnableWindow(m_bRiskManager && 
														m_RiskNote.GetWindowTextLength() > 0 && 
														!m_IdeaID.GetData().IsEmpty());
}
