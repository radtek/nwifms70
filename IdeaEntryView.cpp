// IdeaEntryView.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "IdeaEntryView.h"
#include "qdata.h"

// CIdeaEntryView

IMPLEMENT_DYNCREATE(CIdeaEntryView, CFormView)

CIdeaEntryView::CIdeaEntryView()
	: CFormView(CIdeaEntryView::IDD)
{
	m_Action = NULL;
	m_Aim = NULL;
	m_Dir = NULL;
	m_Currency = NULL;
	m_Counterparty = NULL;
	m_Trader = NULL;
	m_Risk = NULL;
	m_Reward = NULL;

	m_bLoaded = FALSE;
}

CIdeaEntryView::~CIdeaEntryView()
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
}

void CIdeaEntryView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IDEAE_LIST, m_SS);
}

void CIdeaEntryView::InitControls()
{
	m_SS.SetVisibleCols(11);
	m_SS.SetVisibleRows(10);
	m_SS.SetSheetCols(7);
	m_SS.SetSheetRows(m_SS.GetMaxRows());
	m_SS.LockSheet();
	m_SS.SetSheetRows(0);
	m_SS.SetSheetCurRow(-1);

	m_IdeaID.Setup(this, IDC_IDEAE_ID_EDIT);

	m_Date.Setup(this, IDC_IDEAE_DATE_EDIT);

	m_Action = new COptComboBox(this, IDC_IDEAE_ACTION_COMBO);
	m_Aim = new COptComboBox(this, IDC_IDEAE_AIM_COMBO);
    m_Dir = new COptComboBox(this, IDC_IDEAE_DIR_COMBO);
	m_Currency = new COptComboBox(this, IDC_IDEAE_CURRENCY_COMBO);
    m_Counterparty = new COptComboBox(this, IDC_IDEAE_CP_COMBO, TRUE);
	m_Trader = new COptComboBox(this, IDC_IDEAE_TRADER_COMBO);
	m_Risk = new COptComboBox(this, IDC_IDEAE_RISK_COMBO);
	m_Reward = new COptComboBox(this, IDC_IDEAE_REWARD_COMBO);
	
	m_Capital.Setup(this, IDC_IDEAE_CAPITAL_EDIT);
	m_Idea.Setup(this, IDC_IDEAE_IDEA_EDIT);
	m_Idea.LimitText(512);

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
}

BOOL CIdeaEntryView::IsOK()
{
	CString TransType, Text, Data;
	CQData QData;
	
	if(m_Action->GetCurSel() < 0)
		Text = "Invalid Time To Act, must enter a selection";
	
	if(m_Dir->GetCurSel() < 0)
		Text = "Invalid Dir, must enter a selection ";

	if(m_Currency->GetCurSel() < 0)
		Text = "Invalid Currency, must enter a currency.";

	if(m_Aim->GetCurSel() < 0)
		Text = "Invalid Aim, must enter a aim";

	if(m_Risk->GetCurSel() < 0)
		Text = "Invalid Risk, must enter a selection.";

	if(m_Reward->GetCurSel() < 0)
		Text = "Invalid Reward, must enter a reward.";

	if(m_Capital.GetWindowTextLength() == 0)
		Text = "Invalid Capital. Must enter capital amount.";
	if(Text.IsEmpty())
		return TRUE;

	MessageBox(Text);
	return FALSE;
}


BOOL CIdeaEntryView::UpdateData(BOOL bSaveandValid)
{
	if(!bSaveandValid)
	{
		BeginWaitCursor();
		CQData QData;
		COraLoader OraLoader;

		OraLoader = m_Data.GetOraLoader();
		m_Data.LoadIdeaRecords(TRUE);

		if(!m_bLoaded)
		{
			CString Date;

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
			OraLoader.Today(Date);
			m_Date.SetData(Date);
			m_bLoaded = TRUE;
		}
		EndWaitCursor();
	}

	m_SS.SetSheetCurRow(-1);

	Invalidate();
	UpdateWindow();
	return TRUE;
}

BEGIN_MESSAGE_MAP(CIdeaEntryView, CFormView)
	ON_COMMAND(ID_IDEAS_ADD, &CIdeaEntryView::OnIdeasAdd)
	ON_COMMAND(ID_IDEAS_UPDATE, &CIdeaEntryView::OnIdeasUpdate)
	ON_COMMAND(ID_IDEAS_DELETE, &CIdeaEntryView::OnIdeasDelete)
	ON_UPDATE_COMMAND_UI(ID_IDEAS_ADD, &CIdeaEntryView::OnUpdateIdeasAdd)
	ON_UPDATE_COMMAND_UI(ID_IDEAS_UPDATE, &CIdeaEntryView::OnUpdateIdeasUpdate)
	ON_UPDATE_COMMAND_UI(ID_IDEAS_DELETE, &CIdeaEntryView::OnUpdateIdeasDelete)
END_MESSAGE_MAP()


// CIdeaEntryView diagnostics

#ifdef _DEBUG
void CIdeaEntryView::AssertValid() const
{
	CFormView::AssertValid();
}

void CIdeaEntryView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CNWIFMS70Doc* CIdeaEntryView::GetDocument() // non-debug version is inline
{
	return STATIC_DOWNCAST(CNWIFMS70Doc, m_pDocument);
}
#endif //_DEBUG


// CIdeaEntryView message handlers
BEGIN_EVENTSINK_MAP(CIdeaEntryView, CFormView)
	ON_EVENT(CIdeaEntryView, IDC_IDEAE_LIST, 11, CIdeaEntryView::OnDblClickIdeaeList, VTS_I4 VTS_I4)
	ON_EVENT(CIdeaEntryView, IDC_IDEAE_LIST, 2, CIdeaEntryView::OnBlockSelectedIdeaeList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()
void CIdeaEntryView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	BeginWaitCursor();

	InitControls();
	m_Data.Setup(GetData().GetOraLoader(), &m_SS);
	
	GetData().LoadDates();

	UpdateData(FALSE);
	EndWaitCursor();
}

void CIdeaEntryView::OnDblClickIdeaeList(long Col, long Row)
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
		m_Data.SetCurrentRow(Row);
}

void CIdeaEntryView::OnBlockSelectedIdeaeList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	OnDblClickIdeaeList(BlockCol, BlockRow);
}

void CIdeaEntryView::OnIdeasAdd()
{
   	if(!IsOK())
		return;

	m_Data.AddRow();
}

void CIdeaEntryView::OnIdeasUpdate()
{
   	if(!IsOK())
		return;

	m_Data.UpdateRow();
}

void CIdeaEntryView::OnIdeasDelete()
{
	if(MessageBox("Are you sure you want to delete the record ?",
	  			  "Delete", MB_OKCANCEL) == IDCANCEL)
		return;
	
	m_Data.DeleteRow();
}

void CIdeaEntryView::OnUpdateIdeasAdd(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Capital.GetWindowTextLength() > 0);
}

void CIdeaEntryView::OnUpdateIdeasUpdate(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_SS.GetSheetCurRow() > 0);
}

void CIdeaEntryView::OnUpdateIdeasDelete(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_SS.GetSheetCurRow() > 0 && m_SS.GetIsBlockSelected());
}
