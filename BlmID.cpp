// BlmID.cpp : implementation file
//

#include "stdafx.h"
#include "nwifms70.h"
#include "BlmID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBlmID dialog


CBlmID::CBlmID(CWnd* pParent /*=NULL*/)
	: CDialog(CBlmID::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBlmID)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBlmID::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBlmID)
	DDX_Control(pDX, IDC_BLM_LIST, m_SS);
	//}}AFX_DATA_MAP
}

void CBlmID::GetKeyInfo(CString Text, CString &TransType, CString &Asset, CString &Ticket, 
						CString &TransNum, CString &Strike, CString &OptExp)
{
	char Buf[80], *p, *q;
	int i = 0;

	strcpy_s(Buf, Text);
	p = strtok_s(Buf, "|", &q);
	while(p)
	{
		switch(i)
		{
			case 0:
				TransType = p;
				break;
			case 1:
				if(TransType == "IRS")
					Ticket = p;
				else
					Asset = p;
				break;
			case 2:
				if(TransType == "IRS")
					Asset = p;
				else
					if(TransType == "CALL" || TransType == "PUT")
						Strike = p;
				break;
			case 3:
				if(TransType == "CALL" || TransType == "PUT")
					OptExp = p;
				break;
			default:
				break;
		}

		p = strtok_s(NULL, "|", &q);
		i ++;
	}
}

void CBlmID::SetSheetSize()
{
	m_SS.SetSheetToFit(m_SS.GetVisibleCols(), m_SS.GetVisibleRows());
	m_SS.SetAutoSize(TRUE);
	m_SS.SetSheetColHidden(3, TRUE);
}

void CBlmID::UpdateRecord(CString &Text, LPCTSTR ID, int Status)
{
	CString TransType, Asset, Ticket, TransNum, Strike, OptExp;
	COraLoader OraLoader2;

	OraLoader2 = m_OraLoader;
	GetKeyInfo(Text, TransType, Asset, Ticket, TransNum, Strike, OptExp);

	if(TransType == "CDS")
	{
		if(Status == 1) // Add New
			OraLoader2.GetSql().Format("INSERT INTO SEMAM.NW_BLM_IDS (ASSET_CODE, BLM_ID) "
										"VALUES ('%s', '%s') ", (LPCTSTR) Asset, ID);
		else  // Update
			OraLoader2.GetSql().Format("UPDATE SEMAM.NW_BLM_IDS SET BLM_ID = '%s' "
										"WHERE ASSET_CODE = '%s' ", ID, (LPCTSTR) Asset);
		OraLoader2.ExecuteSql();
	}
	else
		if(TransType == "IRS")
		{
			if(!Ticket.IsEmpty())
			{
				OraLoader2.GetSql().Format("DELETE SEMAM.NW_BLM_IDS A "
											"WHERE  0 < (SELECT COUNT(*) FROM SEMAM.NW_TR_TICKETS B "
													"WHERE B.TICKET_NUM = '%s' AND B.TRANS_NUM = A.TRANS_NUM) ", 
													(LPCTSTR)Ticket);
				OraLoader2.ExecuteSql();
		
				if(ID && strlen(ID) > 0)
				{
					OraLoader2.GetSql().Format("INSERT INTO SEMAM.NW_BLM_IDS (TRANS_NUM, BLM_ID) "
												"SELECT TRANS_NUM, '%s' "
												"FROM SEMAM.NW_TR_TICKETS A, SEMAM.NW_ASSETS B "
												"WHERE B.ASS_CODE = A.ASSET_CODE "
												"AND B.ASS_INT_TYPE = 'FIXED' "
												"AND A.TICKET_NUM = '%s' ", ID, (LPCTSTR) Ticket);
					OraLoader2.ExecuteSql();
				}
			}
		}
		else
			if(TransType == "CALL" || TransType == "PUT")
			{
				OraLoader2.GetSql().Format("DELETE SEMAM.NW_BLM_IDS A "
									"WHERE  0 < (SELECT COUNT(*) FROM SEMAM.NW_TR_TICKETS B "
												"WHERE B.TRANS_NUM = A.TRANS_NUM "
												"AND B.TRANS_TYPE = '%s' "
												"AND B.ASSET_CODE = '%s' "
												"AND B.EXERCISE_PRICE = %s "
												"AND B.OPT_EXPIRATION = '%s') ", (LPCTSTR) TransType, 
												(LPCTSTR) Asset, (LPCTSTR) Strike, (LPCTSTR) OptExp);
				OraLoader2.ExecuteSql();
								
				if(ID && strlen(ID) > 0)
				{
					OraLoader2.GetSql().Format("INSERT INTO SEMAM.NW_BLM_IDS (TRANS_NUM, BLM_ID) "
										"SELECT TRANS_NUM, '%s' FROM SEMAM.NW_TR_TICKETS A "
										"WHERE A.TRANS_TYPE = '%s' "
										"AND A.ASSET_CODE = '%s' "
										"AND A.EXERCISE_PRICE = %s "
										"AND A.OPT_EXPIRATION = '%s' ", ID, (LPCTSTR) TransType, 
										(LPCTSTR) Asset, (LPCTSTR) Strike, (LPCTSTR) OptExp);
					OraLoader2.ExecuteSql();
				}
			}
}

BEGIN_MESSAGE_MAP(CBlmID, CDialog)
	//{{AFX_MSG_MAP(CBlmID)
	ON_BN_CLICKED(IDC_BLM_COPY_BUTTON, OnBlmCopyButton)
	ON_BN_CLICKED(IDC_BLM_PASTE_BUTTON, OnBlmPasteButton)
	ON_BN_CLICKED(IDC_BLM_RELOAD_BUTTON, OnBlmReloadButton)
	ON_BN_CLICKED(IDC_BLM_SAVE_BUTTON, OnBlmSaveButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlmID message handlers

BOOL CBlmID::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_SS.SetVisibleRows(16);
	m_SS.SetVisibleCols(3);
	m_SS.SetTypeEditCharCase(1);
	OnBlmReloadButton();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBlmID::OnBlmCopyButton() 
{
	m_SS.SetSelection(2, m_SS.GetActiveRow(), 2, m_SS.GetSelBlockRow2());
	m_SS.ClipboardCopy();
}

void CBlmID::OnBlmPasteButton() 
{
	m_SS.SetActiveCol(2);
	m_SS.ClipboardPaste();
	SetSheetSize();
}

void CBlmID::OnBlmReloadButton() 
{
	LPCTSTR p;
	CString Date, Sql;
	CDBRec DBRec;

	BeginWaitCursor();
	m_OraLoader.GetSql() = "SELECT MAX(INDATE) FROM SEMAM.NW_DAILY_POSITION ";
	if(!m_OraLoader.Open())
		return;
	
	m_OraLoader.LoadTextString(Date);
	p = (LPCTSTR ) Date;

	m_OraLoader.GetSql().Format("SELECT 'CDS'||'|'||A.ASSET_CODE \"ASSET\", BLM_ID "
				"FROM SEMAM.NW_DAILY_POSITION A "
				"LEFT OUTER JOIN SEMAM.NW_BLM_IDS B ON (A.ASSET_CODE = B.ASSET_CODE) "
				"WHERE INDATE = '%s' "
				"AND A.INDUSTRY IN ('EM CDS SWAPS', 'G7 CDS SWAPS') "
				"UNION "
				"SELECT DISTINCT 'IRS'||'|'||TICKET_NUM||'|'||A.ASSET_CODE||'|'||ASS_DESC, BLM_ID "
				"FROM SEMAM.NW_TR_TICKETS A "
				"LEFT OUTER JOIN SEMAM.NW_BLM_IDS B ON (A.TRANS_NUM = B.TRANS_NUM) "
				"JOIN SEMAM.NW_ASSETS C ON (A.ASSET_CODE = C.ASS_CODE) "
				"WHERE A.TRADE_DATE <= '%s' "
				"AND A.TRANS_TYPE = 'INT. SWAP' "
				"AND NVL(A.MATURITY_DATE, TO_DATE('%s') + 1) > '%s' "
				"AND C.ASS_INT_TYPE = 'FIXED' "
				"UNION "
				"SELECT DISTINCT A.TRANS_TYPE||'|'||A.ASSET_CODE||'|'||TO_NUMBER(A.EXERCISE_PRICE)"
				"||'|'||TO_CHAR(A.OPT_EXPIRATION), BLM_ID "
				"FROM SEMAM.NW_TR_TICKETS A "
				"LEFT OUTER JOIN SEMAM.NW_BLM_IDS B ON (A.TRANS_NUM = B.TRANS_NUM) "
				"LEFT OUTER JOIN SEMAM.NW_TR_TICKETS C ON (A.TRANS_NUM = C.OPTION_BACKING) "
				"WHERE C.TRADE_DATE <= '%s' "
				"AND A.TRANS_TYPE IN ('CALL', 'PUT') "
				"AND A.OPTION_BACKING IS NULL "
				"AND (A.TRADE_DATE <= '%s' OR A.VALUE_DATE <= '%s') "
				"AND NVL(A.EXERCISE_DATE, TO_DATE('%s') + 1) > TO_DATE('%s') "
				"AND NVL(A.OPT_EXPIRATION, TO_DATE('%s') + 1) > TO_DATE('%s') "
				"ORDER BY 1 ", p, p, p, p, p, p, p, p, p, p, p);
	if(!m_OraLoader.Open())
		return;

	m_OraLoader.LoadDBSheet(m_SS, TRUE);
	m_SS.LockSheetCol(2, FALSE); // Unlock Column 2
	SetSheetSize();

	m_DataArray.RemoveAll();
	for(int i = 1; i <= m_SS.GetSheetRows(); i ++)
	{
		DBRec.RemoveAll();
		DBRec.Add(m_SS.GetSheetText(1, i));
		DBRec.Add(m_SS.GetSheetText(2, i));
		m_DataArray.Add(DBRec);
	}
	EndWaitCursor();
}

void CBlmID::OnBlmSaveButton() 
{
	int i, Status;
	CString Text, ID, ID2;

	BeginWaitCursor();
	for (i = 1; i <= m_SS.GetSheetRows(); i ++)
	{
		Text = m_SS.GetSheetText(1, i);
		ID = m_SS.GetSheetText(2, i);
		ID2 = m_DataArray.GetAt(i - 1)->GetAt(1);

		Status = 0;
		if(ID2.IsEmpty())
		{
			if(!ID.IsEmpty())
				Status = 1;
		}
		else
			if(ID != ID2)
				Status = 2;
			else
				Status = 0;

		if(Status > 0)
			UpdateRecord(Text, ID, Status);
	}

	EndWaitCursor();
}

void CBlmID::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
