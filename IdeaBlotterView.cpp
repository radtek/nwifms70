// IdeaBlotterView.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "IdeaBlotterView.h"
#include "qdata.h"

// CIdeaBlotterView

IMPLEMENT_DYNCREATE(CIdeaBlotterView, CFormView)

CIdeaBlotterView::CIdeaBlotterView()
	: CFormView(CIdeaBlotterView::IDD)
{
	m_Action = NULL;
	m_Aim = NULL;
	m_Dir = NULL;
	m_Currency = NULL;
	m_Counterparty = NULL;
	m_Trader = NULL;
	m_Risk = NULL;
	m_Reward = NULL;
	m_bFind = FALSE;
}

CIdeaBlotterView::~CIdeaBlotterView()
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

void CIdeaBlotterView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IDEAB_LIST, m_SS);
}

void CIdeaBlotterView::InitControls()
{
	m_SS.SetVisibleCols(11);
	m_SS.SetVisibleRows(10);
	m_SS.SetSheetCols(7);
	m_SS.SetSheetRows(m_SS.GetMaxRows());
	m_SS.LockSheet();
	m_SS.SetSheetRows(0);
	m_SS.SetSheetCurRow(-1);

	m_IdeaID.Setup(this, IDC_IDEAB_ID_EDIT);

	m_Date.Setup(this, IDC_IDEAB_DATE_EDIT);

	m_Action = new COptComboBox(this, IDC_IDEAB_ACTION_COMBO);
	m_Aim = new COptComboBox(this, IDC_IDEAB_AIM_COMBO);
    m_Dir = new COptComboBox(this, IDC_IDEAB_DIR_COMBO);
	m_Currency = new COptComboBox(this, IDC_IDEAB_CURRENCY_COMBO);
    m_Counterparty = new COptComboBox(this, IDC_IDEAB_CP_COMBO);
	m_Trader = new COptComboBox(this, IDC_IDEAB_TRADER_COMBO);
	m_Risk = new COptComboBox(this, IDC_IDEAB_RISK_COMBO);
	m_Reward = new COptComboBox(this, IDC_IDEAB_REWARD_COMBO);
	m_Decision = new COptComboBox(this, IDC_IDEAB_DECISION_COMBO);
	
	m_Capital.Setup(this, IDC_IDEAB_CAPITAL_EDIT);
	m_ApprovedCapital.Setup(this, IDC_IDEAB_APPROVEDCAPITAL_EDIT);
	m_Idea.Setup(this, IDC_IDEAB_IDEA_EDIT);
	m_Idea.LimitText(512);
	m_RiskNote.Setup(this, IDC_IDEAB_RISKNOTE_EDIT);
	m_RiskNote.LimitText(80);
	m_Note.Setup(this, IDC_IDEAB_NOTE_EDIT);
	m_Note.LimitText(80);

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

BOOL CIdeaBlotterView::UpdateData(BOOL bSaveandValid)
{
	CQData QData;
	COraLoader OraLoader;
	CString Date;

	BeginWaitCursor();
	OraLoader = m_Data.GetOraLoader();
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

	m_SS.SetSheetCurRow(-1);

	Invalidate();
	UpdateWindow();
	return TRUE;
}

BEGIN_MESSAGE_MAP(CIdeaBlotterView, CFormView)
	ON_COMMAND(ID_IDEABLOTTER_LOADIDEAS, &CIdeaBlotterView::OnIdeablotterLoadideas)
	ON_COMMAND(ID_IDEABLOTTER_CLEARSCREEN, &CIdeaBlotterView::OnIdeablotterClearscreen)
	ON_COMMAND(ID_IDEABLOTTER_FINDIDEAS, &CIdeaBlotterView::OnIdeablotterFindideas)
END_MESSAGE_MAP()


// CIdeaBlotterView diagnostics

#ifdef _DEBUG
void CIdeaBlotterView::AssertValid() const
{
	CFormView::AssertValid();
}

void CIdeaBlotterView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CNWIFMS70Doc* CIdeaBlotterView::GetDocument() // non-debug version is inline
{
	return STATIC_DOWNCAST(CNWIFMS70Doc, m_pDocument);
}
#endif //_DEBUG


// CIdeaBlotterView message handlers
BEGIN_EVENTSINK_MAP(CIdeaBlotterView, CFormView)
	ON_EVENT(CIdeaBlotterView, IDC_IDEAB_LIST, 11, CIdeaBlotterView::OnDblClickIdeabList, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

void CIdeaBlotterView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	BeginWaitCursor();

	InitControls();
	m_Data.Setup(GetData().GetOraLoader(), &m_SS);
	
	GetData().LoadDates();

	UpdateData(FALSE);
	EndWaitCursor();
}

void CIdeaBlotterView::OnDblClickIdeabList(long Col, long Row)
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
		m_Data.SetCurrentRow(Row);
}

void CIdeaBlotterView::OnIdeablotterLoadideas()
{
	m_Data.LoadIdeaRecords(3);
}

void CIdeaBlotterView::OnIdeablotterClearscreen()
{
	m_Data.Refresh();
	m_bFind = TRUE;
}

void CIdeaBlotterView::OnIdeablotterFindideas()
{
	if(m_bFind)
		m_Data.FindRecords();

	m_bFind = FALSE;
}
