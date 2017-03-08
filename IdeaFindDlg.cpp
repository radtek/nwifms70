// IdeaFindDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "IdeaFindDlg.h"
#include "qdata.h"

// CIdeaFindDlg dialog

IMPLEMENT_DYNAMIC(CIdeaFindDlg, CDialog)

CIdeaFindDlg::CIdeaFindDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIdeaFindDlg::IDD, pParent)
{
	m_Action = NULL;
	m_Aim = NULL;
	m_Dir = NULL;
	m_Currency = NULL;
	m_Counterparty = NULL;
	m_Trader = NULL;
	m_Risk = NULL;
	m_Reward = NULL;
}

CIdeaFindDlg::~CIdeaFindDlg()
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

void CIdeaFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IDEAF_LIST, m_SS);
}


BEGIN_MESSAGE_MAP(CIdeaFindDlg, CDialog)
	ON_BN_CLICKED(IDC_IDEAF_CLEAR_BUTTON, &CIdeaFindDlg::OnBnClickedIdeafClearButton)
	ON_BN_CLICKED(IDC_IDEAF_FIND_BUTTON, &CIdeaFindDlg::OnBnClickedIdeafFindButton)
	ON_EN_CHANGE(IDC_IDEAF_DATE_EDIT, &CIdeaFindDlg::OnEnChangeIdeafDateEdit)
	ON_EN_CHANGE(IDC_IDEAF_IDEA_EDIT, &CIdeaFindDlg::OnEnChangeIdeafIdeaEdit)
	ON_EN_CHANGE(IDC_IDEAF_RISKNOTE_EDIT, &CIdeaFindDlg::OnEnChangeIdeafRisknoteEdit)
	ON_EN_CHANGE(IDC_IDEAF_NOTE_EDIT, &CIdeaFindDlg::OnEnChangeIdeafNoteEdit)
	ON_CBN_SELCHANGE(IDC_IDEAF_TRADER_COMBO, &CIdeaFindDlg::OnCbnSelchangeIdeafTraderCombo)
	ON_CBN_SELCHANGE(IDC_IDEAF_ACTION_COMBO, &CIdeaFindDlg::OnCbnSelchangeIdeafActionCombo)
	ON_CBN_SELCHANGE(IDC_IDEAF_DIR_COMBO, &CIdeaFindDlg::OnCbnSelchangeIdeafDirCombo)
	ON_CBN_SELCHANGE(IDC_IDEAF_CURRENCY_COMBO, &CIdeaFindDlg::OnCbnSelchangeIdeafCurrencyCombo)
	ON_CBN_SELCHANGE(IDC_IDEAF_CP_COMBO, &CIdeaFindDlg::OnCbnSelchangeIdeafCpCombo)
	ON_CBN_SELCHANGE(IDC_IDEAF_DECISION_COMBO, &CIdeaFindDlg::OnCbnSelchangeIdeafDecisionCombo)
	ON_CBN_SELCHANGE(IDC_IDEAF_AIM_COMBO, &CIdeaFindDlg::OnCbnSelchangeIdeafAimCombo)
	ON_CBN_SELCHANGE(IDC_IDEAF_RISK_COMBO, &CIdeaFindDlg::OnCbnSelchangeIdeafRiskCombo)
	ON_CBN_SELCHANGE(IDC_IDEAF_REWARD_COMBO, &CIdeaFindDlg::OnCbnSelchangeIdeafRewardCombo)
END_MESSAGE_MAP()

void CIdeaFindDlg::EnableFind(CEdit& Edit, BOOL bAssetDesc)
{
	if(Edit.GetModify() && Edit.GetWindowTextLength() > 0)
	{
		GetDlgItem(IDC_IDEAF_FIND_BUTTON)->EnableWindow(TRUE);
		Edit.SetModify(FALSE);
	}
}

void CIdeaFindDlg::EnableFind(COptComboBox* Combo)
{
	Combo->GetCurSel();
	GetDlgItem(IDC_IDEAF_FIND_BUTTON)->EnableWindow(Combo->GetCurSel() >= 0 ? TRUE : FALSE);
}

// CIdeaFindDlg message handlers
BEGIN_EVENTSINK_MAP(CIdeaFindDlg, CDialog)
	ON_EVENT(CIdeaFindDlg, IDC_IDEAF_LIST, 11, CIdeaFindDlg::OnDblClickIdeafList, VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

BOOL CIdeaFindDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CQData QData;
	
	BeginWaitCursor();
	m_SS.SetVisibleCols(10);
	m_SS.SetVisibleRows(10);

	m_IdeaID.Setup(this, IDC_IDEAF_ID_EDIT);

	m_Date.Setup(this, IDC_IDEAF_DATE_EDIT);

	m_Action = new COptComboBox(this, IDC_IDEAF_ACTION_COMBO);
	m_Aim = new COptComboBox(this, IDC_IDEAF_AIM_COMBO);
    m_Dir = new COptComboBox(this, IDC_IDEAF_DIR_COMBO);
	m_Currency = new COptComboBox(this, IDC_IDEAF_CURRENCY_COMBO);
    m_Counterparty = new COptComboBox(this, IDC_IDEAF_CP_COMBO);
	m_Trader = new COptComboBox(this, IDC_IDEAF_TRADER_COMBO);
	m_Risk = new COptComboBox(this, IDC_IDEAF_RISK_COMBO);
	m_Reward = new COptComboBox(this, IDC_IDEAF_REWARD_COMBO);
	m_Decision = new COptComboBox(this, IDC_IDEAF_DECISION_COMBO);
	
	m_Capital.Setup(this, IDC_IDEAF_CAPITAL_EDIT);
	m_ApprovedCapital.Setup(this, IDC_IDEAF_APPROVEDCAPITAL_EDIT);
	m_Idea.Setup(this, IDC_IDEAF_IDEA_EDIT);
	m_Idea.LimitText(512);
	m_RiskNote.Setup(this, IDC_IDEAF_RISKNOTE_EDIT);
	m_RiskNote.LimitText(80);
	m_Note.Setup(this, IDC_IDEAF_NOTE_EDIT);
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

	GetDlgItem(IDC_IDEAF_FIND_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	
	EndWaitCursor();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIdeaFindDlg::OnDblClickIdeafList(long Col, long Row)
{
	if(Row > 0 && Row <= m_SS.GetSheetRows())
		m_Data.SetCurrentRow(Row);

	GetDlgItem(IDOK)->EnableWindow(!m_IdeaID.GetData().IsEmpty());
}

void CIdeaFindDlg::OnBnClickedIdeafClearButton()
{
	m_Data.Refresh();
	GetDlgItem(IDOK)->EnableWindow(!m_IdeaID.GetData().IsEmpty());
}

void CIdeaFindDlg::OnBnClickedIdeafFindButton()
{
	m_Data.FindRecords();
}

void CIdeaFindDlg::OnEnChangeIdeafDateEdit()
{
	EnableFind(m_Date);
}

void CIdeaFindDlg::OnEnChangeIdeafIdeaEdit()
{
	EnableFind(m_Idea);
}

void CIdeaFindDlg::OnEnChangeIdeafRisknoteEdit()
{
	EnableFind(m_RiskNote);
}

void CIdeaFindDlg::OnEnChangeIdeafNoteEdit()
{
	EnableFind(m_Note);
}

void CIdeaFindDlg::OnCbnSelchangeIdeafTraderCombo()
{
	EnableFind(m_Trader);
}

void CIdeaFindDlg::OnCbnSelchangeIdeafActionCombo()
{
	EnableFind(m_Action);
}

void CIdeaFindDlg::OnCbnSelchangeIdeafDirCombo()
{
	EnableFind(m_Dir);
}

void CIdeaFindDlg::OnCbnSelchangeIdeafCurrencyCombo()
{
	EnableFind(m_Currency);
}

void CIdeaFindDlg::OnCbnSelchangeIdeafCpCombo()
{
	EnableFind(m_Counterparty);
}

void CIdeaFindDlg::OnCbnSelchangeIdeafDecisionCombo()
{
	EnableFind(m_Decision);
}

void CIdeaFindDlg::OnCbnSelchangeIdeafAimCombo()
{
	EnableFind(m_Aim);
}

void CIdeaFindDlg::OnCbnSelchangeIdeafRiskCombo()
{
	EnableFind(m_Risk);
}

void CIdeaFindDlg::OnCbnSelchangeIdeafRewardCombo()
{
	EnableFind(m_Reward);
}

void CIdeaFindDlg::OnOK()
{
	m_strIdeaID = m_IdeaID.GetData();
	if(!m_strIdeaID.IsEmpty())
		CDialog::OnOK();
}
