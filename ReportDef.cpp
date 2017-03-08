// ReportDef.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "ReportDef.h"
#include "qdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CReportDef dialog


CReportDef::CReportDef(CWnd* pParent /*=NULL*/)
	: CDialog(CReportDef::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReportDef)
	m_Report = NULL;
	m_ReportDef = NULL;
	m_AvailField = NULL;
	m_SelField = NULL;
	m_AvailValue = NULL;
	m_SelValue = NULL;
	//}}AFX_DATA_INIT
}

CReportDef::~CReportDef()
{
	if(m_Report)
		delete m_Report;
	if(m_ReportDef)
		delete m_ReportDef;
	if(m_AvailField)
		delete m_AvailField;
	if(m_SelField)
		delete m_SelField;
	if(m_AvailValue)
		delete m_AvailValue;
	if(m_SelValue)
		delete m_SelValue;
}

void CReportDef::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReportDef)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

void CReportDef::AddSelItem(CHLBox *Avail, CHLBox *Sel)
{
	CString Text;
	
	Avail->GetSelString(Text);
	if(Text.IsEmpty())
		return;
	if(Sel->FindStringExact(-1, Text) < 0)
		Sel->AddString(Text);
}

void CReportDef::DelSelItem(CHLBox *Sel)
{
	int Index;

	Index = Sel->GetCurSel();
	if(Index >= 0)
		Sel->DeleteString(Index);
}

void CReportDef::EnableButtons()
{
	if(m_ReportDef->GetCurSel() >= 0)	
	{
		GetDlgItem(IDC_REPORTDEF_ADD_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_REPORTDEF_UPDATE_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_REPORTDEF_COPY_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_REPORTDEF_DELETE_BUTTON)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_REPORTDEF_ADD_BUTTON)->EnableWindow(TRUE);
		GetDlgItem(IDC_REPORTDEF_UPDATE_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_REPORTDEF_COPY_BUTTON)->EnableWindow(FALSE);
		GetDlgItem(IDC_REPORTDEF_DELETE_BUTTON)->EnableWindow(FALSE);
	}	
}

void CReportDef::ProcessData()
{
	int Index;
	CString Text;
	CDBRec Rec;

	Index = m_SelField->GetCurSel();
	if(Index < 0)
		return;
	
	m_SelField->GetText(Index, Text);
	Rec.Add(Text);
	Rec.Add(Text);
	for(int i = 0; i < m_SelValue->GetCount(); i ++)
	{
		m_SelValue->GetText(i, Text);
		Rec.SetAt(1, Text);
	}
}

BEGIN_MESSAGE_MAP(CReportDef, CDialog)
	//{{AFX_MSG_MAP(CReportDef)
	ON_CBN_SELCHANGE(IDC_REPORTDEF_REPORT_COMBO, OnSelchangeReportdefReportCombo)
	ON_CBN_SELCHANGE(IDC_REPORTDEF_REPORTDEF_COMBO, OnSelchangeReportdefReportdefCombo)
	ON_EN_KILLFOCUS(IDC_REPORTDEF_NAME_EDIT, OnKillfocusReportdefNameEdit)
	ON_BN_CLICKED(IDC_REPORTDEF_ADDFIELD_BUTTON, OnReportdefAddfieldButton)
	ON_BN_CLICKED(IDC_REPORTDEF_DELFIELD_BUTTON, OnReportdefDelfieldButton)
	ON_LBN_DBLCLK(IDC_REPORTDEF_AVAILFIELD_LIST, OnDblclkReportdefAvailfieldList)
	ON_LBN_DBLCLK(IDC_REPORTDEF_SELFIELD_LIST, OnDblclkReportdefSelfieldList)
	ON_LBN_SELCHANGE(IDC_REPORTDEF_SELFIELD_LIST, OnSelchangeReportdefSelfieldList)
	ON_BN_CLICKED(IDC_REPORTDEF_ADDVALUE_BUTTON, OnReportdefAddvalueButton)
	ON_LBN_DBLCLK(IDC_REPORTDEF_AVAILVALUE_LIST, OnDblclkReportdefAvailvalueList)
	ON_BN_CLICKED(IDC_REPORTDEF_DELVALUE_BUTTON, OnReportdefDelvalueButton)
	ON_LBN_DBLCLK(IDC_REPORTDEF_SELVALUE_LIST, OnDblclkReportdefSelvalueList)
	ON_BN_CLICKED(IDC_REPORTDEF_ADD_BUTTON, OnReportdefAddButton)
	ON_BN_CLICKED(IDC_REPORTDEF_UPDATE_BUTTON, OnReportdefUpdateButton)
	ON_BN_CLICKED(IDC_REPORTDEF_COPY_BUTTON, OnReportdefCopyButton)
	ON_BN_CLICKED(IDC_REPORTDEF_DELETE_BUTTON, OnReportdefDeleteButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReportDef message handlers

BOOL CReportDef::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	BeginWaitCursor();
	m_Report = (COptComboBox*) new COptComboBox(this, IDC_REPORTDEF_REPORT_COMBO);
	m_ReportDef = (COptComboBox*) new COptComboBox(this, IDC_REPORTDEF_REPORTDEF_COMBO);
	m_DefName.SubclassDlgItem(IDC_REPORTDEF_NAME_EDIT, this);
	m_DefName.LimitText(40);
	m_AvailField = (CHLBox*) new CHLBox(this, IDC_REPORTDEF_AVAILFIELD_LIST);
	m_SelField = (CHLBox*) new CHLBox(this, IDC_REPORTDEF_SELFIELD_LIST);
	m_AvailValue = (CHLBox*) new CHLBox(this, IDC_REPORTDEF_AVAILVALUE_LIST);
	m_SelValue = (CHLBox*) new CHLBox(this, IDC_REPORTDEF_SELVALUE_LIST);

	for(int i = 0; i < GetReportData()->GetAvail().GetReportArray().GetSize(); i ++)
		m_Report->AddString(GetReportData()->GetAvail().GetReportArray().GetAt(i)->GetAt(0));

	EndWaitCursor();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CReportDef::OnSelchangeReportdefReportCombo() 
{
	int Index;
	CQData QData;
	
	Index = m_Report->GetCurSel();
	if(Index < 0)
		return;
	
	BeginWaitCursor();
	m_ReportData->SetTableName(m_ReportData->GetAvail().GetReportArray().GetAt(Index)->GetAt(1));
	m_ReportData->SetReportID(m_ReportData->GetAvail().GetReportArray().GetAt(Index)->GetAt(2));
	m_ReportData->SetWithUnion(m_ReportData->GetAvail().GetReportArray().GetAt(Index)->GetAt(3));	
	m_ReportData->LoadReportDefArray();
	m_ReportData->LoadFieldArray();
	
	m_ReportDef->ResetContent();
	m_AvailValue->ResetContent();
	m_SelValue->ResetContent();
	m_SelField->ResetContent();

	QData.CopyDBRecArrayToComboBox(m_ReportData->GetSel().GetReportArray(), *m_ReportDef, 0, FALSE);
	m_ReportData->GetAvail().CopyField(m_AvailField);
	EndWaitCursor();
}

void CReportDef::OnSelchangeReportdefReportdefCombo() 
{
	int Index;
	CString Text;
	
	Index = m_ReportDef->GetCurSel();
	if(Index < 0)
		return;
	
	BeginWaitCursor();
	m_ReportDef->GetSelString(Text);
	m_DefName.SetWindowText(Text);
	m_ReportData->SetDefID(m_ReportData->GetSel().GetReportArray().GetAt(Index)->GetAt(1));
	m_ReportData->LoadSelFieldArray();
	m_AvailValue->ResetContent();
	m_SelValue->ResetContent();
	m_ReportData->GetSel().CopyField(m_SelField);
	EnableButtons();
	EndWaitCursor();
}

void CReportDef::OnKillfocusReportdefNameEdit() 
{
	CString Text;
	int Index;

	m_DefName.GetWindowText(Text);
	Index = m_ReportDef->FindStringExact(-1, Text);
	if(Index >= 0)
	{
		m_ReportDef->SetCurSel(Index);
		OnSelchangeReportdefReportdefCombo();
	}

	EnableButtons();
}

void CReportDef::OnReportdefAddfieldButton() 
{
	AddSelItem(m_AvailField, m_SelField);
}

void CReportDef::OnReportdefDelfieldButton() 
{
	if(m_SelField->GetCurSel() >= 0)
		OnDblclkReportdefSelfieldList();
}

void CReportDef::OnDblclkReportdefAvailfieldList() 
{
	AddSelItem(m_AvailField, m_SelField);	
}

void CReportDef::OnDblclkReportdefSelfieldList() 
{
	CString Text;
	CDBRec Rec;
	
	m_SelField->GetSelString(Text);	
	if(Text.IsEmpty())
		return;

	m_AvailValue->ResetContent();
	m_SelValue->ResetContent();
	Rec.Add(Text);
	m_ReportData->GetSel().RemoveAt(Rec);
	DelSelItem(m_SelField);
}

void CReportDef::OnSelchangeReportdefSelfieldList() 
{
	int Index;

	Index = m_SelField->GetCurSel();
	if(Index < 0)
		return;
	
	CString Text;
	m_SelField->GetText(Index, Text);
	
	m_ReportData->GetAvail().CopyFieldValue(m_AvailValue, Text);
	m_ReportData->GetSel().CopyFieldValue(m_SelValue, Text);
}

void CReportDef::OnReportdefAddvalueButton() 
{
	AddSelItem(m_AvailValue, m_SelValue);

	CString Text;
	CDBRec Rec;

	m_AvailField->GetSelString(Text);
	Rec.Add(Text);
	m_AvailValue->GetSelString(Text);
	Rec.Add(Text);
	m_ReportData->GetSel().InsertAt(Rec);
}

void CReportDef::OnDblclkReportdefAvailvalueList() 
{
	OnReportdefAddvalueButton();
}

void CReportDef::OnReportdefDelvalueButton() 
{
	if(m_SelValue->GetCurSel() >= 0)
		OnDblclkReportdefSelvalueList();
}

void CReportDef::OnDblclkReportdefSelvalueList() 
{
	CString Text;
	CDBRec Rec;

	m_SelField->GetSelString(Text);
	Rec.Add(Text);
	m_SelValue->GetSelString(Text);
	Rec.Add(Text);

	m_ReportData->GetSel().RemoveAt(Rec);
	DelSelItem(m_SelValue);
}

void CReportDef::OnReportdefAddButton() 
{
	CString Text;

	m_DefName.GetWindowText(Text);
	if(m_ReportDef->FindStringExact(-1, Text) < 0)
	{
		m_ReportData->AddReportDef(Text);
		m_ReportDef->AddString(Text);
		m_ReportDef->SelectString(-1, Text);
	}
	else
		MessageBox("Duplicated Report Name");
}

void CReportDef::OnReportdefUpdateButton() 
{
	CString Text;

	m_ReportDef->GetSelString(Text);
	m_ReportData->UpdateReportDef(Text);
}

void CReportDef::OnReportdefCopyButton() 
{
	CString Text;
	
	m_DefName.GetWindowText(Text);
	m_ReportData->SetDefID(EMPTYSTRING);
	m_ReportData->AddReportDef(Text);
}

void CReportDef::OnReportdefDeleteButton() 
{
	CString Text;
	int Index;

	Index = m_ReportDef->GetCurSel();
	if(Index >= 0)
	{
		m_ReportDef->GetSelString(Text);
		m_ReportData->DelReportDef(Text);
		m_ReportDef->DeleteString(Index);
		m_ReportDef->SetCurSel(-1);
		m_DefName.SetWindowText(EMPTYSTRING);
		m_AvailField->ResetContent();
		m_SelField->ResetContent();
		m_AvailValue->ResetContent();
		m_SelValue->ResetContent();
	}
}

void CReportDef::OnCancel() 
{
	CDialog::OnCancel();
}

