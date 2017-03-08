// CPDialog.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "CPDialog.h"
#include "qdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCPDialog dialog


CCPDialog::CCPDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CCPDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCPDialog)
	m_Pru = NULL;
	m_Active = NULL;
	m_CTIntl = NULL;
	m_CTFx = NULL;
	m_CTActive = NULL;
	//}}AFX_DATA_INIT
}

CCPDialog::~CCPDialog()
{
	if(m_Pru)
		delete m_Pru;
	if(m_Active)
		delete m_Active;
	if(m_CTIntl)
		delete m_CTIntl;
	if(m_CTFx)
		delete m_CTFx;
	if(m_CTActive)
		delete m_CTActive;
}

void CCPDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCPDialog)
	DDX_Control(pDX, IDC_CP_CP_LIST, m_SS);
	DDX_Control(pDX, IDC_CP_CT_LIST, m_CTSS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCPDialog, CDialog)
	//{{AFX_MSG_MAP(CCPDialog)
	ON_BN_CLICKED(IDC_CP_ADD_BUTTON, OnCpAddButton)
	ON_BN_CLICKED(IDC_CP_DELETE_BUTTON, OnCpDeleteButton)
	ON_BN_CLICKED(IDC_CP_FIND_BUTTON, OnCpFindButton)
	ON_BN_CLICKED(IDC_CP_UPDATE_BUTTON, OnCpUpdateButton)
	ON_BN_CLICKED(IDC_CP_CLEAR_BUTTON, OnCpClearButton)
	ON_EN_KILLFOCUS(IDC_CP_CPCODE_EDIT, OnKillfocusCPCPCodeEdit)
	ON_EN_KILLFOCUS(IDC_CP_CP_EDIT, OnKillfocusCPEdit)
	ON_BN_CLICKED(IDC_CP_ADD_CT_BUTTON, OnCpAddCtButton)
	ON_BN_CLICKED(IDC_CP_DELETE_CT_BUTTON, OnCpDeleteCtButton)
	ON_BN_CLICKED(IDC_CP_UPDATE_CT_BUTTON, OnCpUpdateCtButton)
	ON_EN_KILLFOCUS(IDC_CP_CTCODE_EDIT, OnKillfocusCpCtcodeEdit)
	ON_BN_CLICKED(IDC_CP_CTFX_CHECK, &CCPDialog::OnBnClickedCpCtfxCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCPDialog::EnableCtrls()
{
	BOOL bEnable = FALSE;
	
	if(m_Data.GetCurrentRow() > 0)
		bEnable = TRUE;

	m_CTCode.EnableWindow(bEnable);
	m_Name.EnableWindow(bEnable);
	m_CT.EnableWindow(bEnable);
	m_Address.EnableWindow(bEnable);
	m_Address2.EnableWindow(bEnable);
	m_Phone.EnableWindow(bEnable);
	m_Fax.EnableWindow(bEnable);
	m_Cedel.EnableWindow(bEnable);
	m_EuroClear.EnableWindow(bEnable);
	m_MSTC.EnableWindow(bEnable);
	m_FedID.EnableWindow(bEnable);
	m_CTLimit.EnableWindow(bEnable);
	m_Isin.EnableWindow(bEnable);
	m_Bic.EnableWindow(bEnable);
	m_DTC.EnableWindow(bEnable);
	m_Group.EnableWindow(bEnable);
	m_SecID.EnableWindow(bEnable);
	m_FSAID.EnableWindow(bEnable);
	m_CTIntl->EnableWindow(bEnable);
	m_CTFx->EnableWindow(bEnable);
	m_CTActive->EnableWindow(bEnable);
	GetDlgItem(IDC_CP_DELETE_BUTTON)->EnableWindow(m_SS.GetIsBlockSelected()|| m_CTSS.GetIsBlockSelected());
	GetDlgItem(IDC_CP_UPDATE_BUTTON)->EnableWindow(m_Data.GetCurrentRow() > 0 ? TRUE : FALSE);

	if(m_Data.GetSRowCtrl().GetCurrentRow() > 0)
	{
		GetDlgItem(IDC_CP_DELETE_CT_BUTTON)->EnableWindow(m_CTSS.GetIsBlockSelected());
		GetDlgItem(IDC_CP_UPDATE_CT_BUTTON)->EnableWindow(m_Data.GetSRowCtrl().GetCurrentRow() > 0 ? TRUE : FALSE);
	}
	
	OnKillfocusCpCtcodeEdit();
}
/////////////////////////////////////////////////////////////////////////////
// CCPDialog message handlers

BEGIN_EVENTSINK_MAP(CCPDialog, CDialog)
    //{{AFX_EVENTSINK_MAP(CCPDialog)
	ON_EVENT(CCPDialog, IDC_CP_CT_LIST, 11 /* DblClick */, OnDblClickCpCtList, VTS_I4 VTS_I4)
	ON_EVENT(CCPDialog, IDC_CP_CT_LIST, 2 /* BlockSelected */, OnBlockSelectedCpCtList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CCPDialog, IDC_CP_CP_LIST, 11 /* DblClick */, OnDblClickCpCpList, VTS_I4 VTS_I4)
	ON_EVENT(CCPDialog, IDC_CP_CP_LIST, 2 /* BlockSelected */, OnBlockSelectedCpCpList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL CCPDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_SS.SetVisibleCols(6);
	m_SS.SetVisibleRows(4);
	m_CTSS.SetVisibleCols(6);
	m_CTSS.SetVisibleRows(4);

	m_CPCode.Setup(this, IDC_CP_CPCODE_EDIT);
	m_CPCode.LimitText(20);
	m_CP.Setup(this, IDC_CP_CP_EDIT);
	m_CP.LimitText(40);
	m_ImgCP.Setup(this, IDC_CP_IMGCODE_EDIT);
	m_ImgCP.LimitText(4);
	m_Limit.Setup(this, IDC_CP_LIMIT_EDIT);
	m_SetLimit.Setup(this, IDC_CP_SETLIMIT_EDIT);
	m_Pru =  (CCheckBox*) new CCheckBox(this, IDC_CP_PRU_CHECK, Y, EMPTYSTRING);
	m_Active = (CCheckBox*) new CCheckBox(this, IDC_CP_ACTIVE_CHECK, Y, N);

	m_CTCode.Setup(this, IDC_CP_CTCODE_EDIT);
	m_CTCode.LimitText(3);
	m_Name.Setup(this, IDC_CP_CTNAME_EDIT);
	m_Name.LimitText(40);
	m_CT.Setup(this, IDC_CP_CT_EDIT);
	m_CT.LimitText(30);
	m_Address.Setup(this, IDC_CP_ADDRESS_EDIT);
	m_Address.LimitText(40);
	m_Address2.Setup(this, IDC_CP_ADDRESS2_EDIT);
	m_Address2.LimitText(40);
	m_Phone.Setup(this, IDC_CP_PHONE_EDIT);
	m_Phone.LimitText(16);
	m_Fax.Setup(this, IDC_CP_FAX_EDIT);
	m_Fax.LimitText(16);
	m_Cedel.Setup(this, IDC_CP_CEDEL_EDIT);
	m_Cedel.LimitText(5);
	m_EuroClear.Setup(this, IDC_CP_EUROCLEAR_EDIT);
	m_EuroClear.LimitText(5);
	m_MSTC.Setup(this, IDC_CP_MSTC_EDIT);
	m_MSTC.LimitText(4);
	m_FedID.Setup(this, IDC_CP_FEDID_EDIT);
	m_FedID.LimitText(20);
	m_CTLimit.Setup(this, IDC_CP_CTLIMIT_EDIT);
	m_Isin.Setup(this, IDC_CP_ISIN_EDIT);
	m_Isin.LimitText(20);
	m_Bic.Setup(this, IDC_CP_BIC_EDIT);
	m_Bic.LimitText(8);
	m_DTC.Setup(this, IDC_CP_DTC_EDIT);
	m_DTC.LimitText(5);
	m_Group.Setup(this, IDC_CP_GROUP_EDIT);
	m_Group.LimitText(3);
	m_SecID.Setup(this, IDC_CP_CTSECID_EDIT);
	m_SecID.LimitText(10);
	m_FSAID.Setup(this, IDC_CP_CTFSAID_EDIT);
	m_FSAID.LimitText(10);
	m_CTIntl = (CCheckBox *) new CCheckBox(this, IDC_CP_CTINTL_CHECK, Y);
	m_CTFx = (CCheckBox *) new CCheckBox(this, IDC_CP_CTFX_CHECK, Y);
	m_CTActive = (CCheckBox *) new CCheckBox(this, IDC_CP_CTACTIVE_CHECK, Y, N);

	m_Data.Add(&m_CPCode, &m_Data.GetCPRec().GetCPCode());
	m_Data.Add(&m_CP, &m_Data.GetCPRec().GetCP());
	m_Data.Add(&m_ImgCP, &m_Data.GetCPRec().GetImgCP());
	m_Data.Add(&m_Limit, &m_Data.GetCPRec().GetLimit());
	m_Data.Add(&m_SetLimit, &m_Data.GetCPRec().GetSetLimit());
	m_Data.Add(m_Pru, &m_Data.GetCPRec().GetPru());
	m_Data.Add(m_Active, &m_Data.GetCPRec().GetActive());

	m_Data.GetSRowCtrl().Add(&m_CTCode, &m_Data.GetCTRec().GetCTCode());
	m_Data.GetSRowCtrl().Add(&m_Name, &m_Data.GetCTRec().GetName());
	m_Data.GetSRowCtrl().Add(&m_CT, &m_Data.GetCTRec().GetCT());
	m_Data.GetSRowCtrl().Add(&m_Address, &m_Data.GetCTRec().GetAddress());
	m_Data.GetSRowCtrl().Add(&m_Address2, &m_Data.GetCTRec().GetAddress2());
	m_Data.GetSRowCtrl().Add(&m_Phone, &m_Data.GetCTRec().GetPhone());
	m_Data.GetSRowCtrl().Add(&m_Fax, &m_Data.GetCTRec().GetFax());
	m_Data.GetSRowCtrl().Add(&m_Cedel, &m_Data.GetCTRec().GetCedel());
	m_Data.GetSRowCtrl().Add(&m_EuroClear, &m_Data.GetCTRec().GetEuroClear());
	m_Data.GetSRowCtrl().Add(&m_MSTC, &m_Data.GetCTRec().GetMSTC());
	m_Data.GetSRowCtrl().Add(&m_FedID, &m_Data.GetCTRec().GetFedID());
	m_Data.GetSRowCtrl().Add(&m_CTLimit, &m_Data.GetCTRec().GetLimit());
	m_Data.GetSRowCtrl().Add(&m_Isin, &m_Data.GetCTRec().GetIsin());
	m_Data.GetSRowCtrl().Add(&m_Bic, &m_Data.GetCTRec().GetBic());
	m_Data.GetSRowCtrl().Add(&m_DTC, &m_Data.GetCTRec().GetDTC());
	m_Data.GetSRowCtrl().Add(&m_Group, &m_Data.GetCTRec().GetGroup());
	m_Data.GetSRowCtrl().Add(&m_SecID, &m_Data.GetCTRec().GetSecID());
	m_Data.GetSRowCtrl().Add(&m_FSAID, &m_Data.GetCTRec().GetFSAID());
	m_Data.GetSRowCtrl().Add(m_CTIntl, &m_Data.GetCTRec().GetCTIntl());
	m_Data.GetSRowCtrl().Add(m_CTFx, &m_Data.GetCTRec().GetCTFx());
	m_Data.GetSRowCtrl().Add(m_CTActive, &m_Data.GetCTRec().GetActive());

	m_Data.Setup(m_pData->GetOraLoader(), &m_SS, &m_CTSS, m_pData->GetTraderIni());

	m_Data.SetHeaders();

	m_Data.SetCurrentRow(-1);

	this->SetWindowText("COUNTERPARTY");
	OnCpClearButton();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCPDialog::OnCpAddButton() 
{
	if(m_Data.AddRow())
		m_Data.Refresh();
}

void CCPDialog::OnCpDeleteButton() 
{
	m_Data.DeleteRow();	
}

void CCPDialog::OnCpFindButton() 
{
	m_Data.LoadDBData();
}

void CCPDialog::OnCpUpdateButton() 
{
	m_Data.UpdateRow();
}

void CCPDialog::OnDblClickCpCtList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_CTSS.GetSheetRows())
		m_Data.GetSRowCtrl().SetCurrentRow(Row);
	else
		m_Data.GetSRowCtrl().SetCurrentRow(-1);
	EnableCtrls();
}

void CCPDialog::OnBlockSelectedCpCtList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	OnDblClickCpCtList(m_CTSS.GetActiveCol(), m_CTSS.GetActiveRow());
}

void CCPDialog::OnDblClickCpCpList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
		m_Data.SetCurrentRow(Row);
	else
		m_Data.SetCurrentRow(-1);
	EnableCtrls();
}

void CCPDialog::OnBlockSelectedCpCpList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2)
{
	OnDblClickCpCpList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
}

void CCPDialog::OnCpClearButton() 
{
	m_Data.Refresh();
	m_Data.GetSRowCtrl().Refresh();
	
	EnableCtrls();
	GetDlgItem(IDC_CP_ADD_BUTTON)->EnableWindow(FALSE);
}

void CCPDialog::OnKillfocusCPCPCodeEdit()
{
	OnKillfocusCPEdit();
}

void CCPDialog::OnKillfocusCPEdit()
{
	GetDlgItem(IDC_CP_ADD_BUTTON)->EnableWindow((m_CPCode.GetWindowTextLength() > 0 && 
												 m_CP.GetWindowTextLength() > 0 && 
												 m_ImgCP.GetWindowTextLength() > 0) ||
		                                         m_CTCode.GetWindowTextLength() > 0 ? TRUE : FALSE);
}

void CCPDialog::OnCpAddCtButton() 
{
	m_Data.AddSRow();	
}

void CCPDialog::OnCpDeleteCtButton() 
{
	m_Data.DeleteSRow();
}

void CCPDialog::OnCpUpdateCtButton() 
{
	m_Data.UpdateSRow();
}

void CCPDialog::OnKillfocusCpCtcodeEdit() 
{
	GetDlgItem(IDC_CP_ADD_BUTTON)->EnableWindow((m_CPCode.GetWindowTextLength() > 0 && 
												m_CP.GetWindowTextLength() > 0 && 
												m_ImgCP.GetWindowTextLength() > 0)||
		                                        m_CTCode.GetWindowTextLength() > 0 ? TRUE : FALSE);
	GetDlgItem(IDC_CP_ADD_CT_BUTTON)->EnableWindow(m_CTCode.GetWindowTextLength() > 0 ? TRUE : FALSE);
}


void CCPDialog::OnBnClickedCpCtfxCheck()
{
	if(m_CPCode.GetWindowTextLength() > 0 && m_CTCode.GetWindowTextLength() > 0 && m_CTFx->GetCheck() > 0)
	{
		COraLoader OraLoader;
		CQData QData;
		CString CP, CT;

		OraLoader.SetDB(&theDB);
		m_CPCode.GetWindowText(CP);
		m_CTCode.GetWindowText(CT);
		CP = QData.GetQueryText(CP);

		if(OraLoader.GetCount("SELECT COUNT(*) FROM SEMAM.NW_COUNTER_CONTACTS WHERE CT_FX = 'Y' AND CP_CODE = " + CP + " AND CT_CODE != " + QData.GetQueryText(CT)) > 0)
		{
			MessageBox("Only one contact for fx trade per counterparty allowed", "Counterparty");
			m_CTFx->SetCheck(0);
		}
	}
}
