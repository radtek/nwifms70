// TwoField.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "optmap.h"
#include "qdata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptMap dialog


COptMap::COptMap(CWnd* pParent /*=NULL*/)
	: CDialog(COptMap::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptMap)
	//}}AFX_DATA_INIT
}


void COptMap::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptMap)
	DDX_Control(pDX, IDC_OPTMAP_LIST, m_SS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptMap, CDialog)
	//{{AFX_MSG_MAP(COptMap)
	ON_BN_CLICKED(IDC_OPTMAP_UPDATE_BUTTON, OnOptMapUpdateButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptMap message handlers

BEGIN_EVENTSINK_MAP(COptMap, CDialog)
    //{{AFX_EVENTSINK_MAP(COptMap)
	ON_EVENT(COptMap, IDC_OPTMAP_LIST, 11 /* DblClick */, OnDblClickOptMapList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

BOOL COptMap::OnInitDialog() 
{
	CDialog::OnInitDialog();

	BeginWaitCursor();
	m_SS.SetVisibleRows(14);
	m_SS.SetVisibleCols(2);

	m_OptTicker.Setup(this, IDC_OPTMAP_TICKER_EDIT, EMPTYSTRING, UPPERCASE);
	m_OptTicker.SetReadOnly();
	m_ImagineCode.Setup(this, IDC_OPTMAP_IMGCODE_EDIT, EMPTYSTRING);
	m_ImagineCode.LimitText(25);
	m_ImagineBBTick.Setup(this, IDC_OPTMAP_IMGBBTICK_EDIT, EMPTYSTRING, UPPERCASE);
	m_ImagineBBTick.LimitText(25);

	m_OraLoader.SetDB(&theDB);

	m_OraLoader.Open("SELECT DISTINCT A.OPT_TICK, B.IMAGINE_CODE, B.IMAGINE_BBTICK "
					"FROM SEMAM.NW_OPTION_VIEW A, SEMAM.NW_OPT_TICK_MAPS B "
					"WHERE B.OPT_TICK(+) = A.OPT_TICK "
					"AND A.LISTED = 'Y' "
					"ORDER BY 1 ");

	m_OraLoader.LoadDBSheet(m_SS);
	GetDlgItem(IDC_OPTMAP_UPDATE_BUTTON)->EnableWindow(FALSE);
	EndWaitCursor();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COptMap::OnOptMapUpdateButton() 
{
	CString OptTick, BBTick, IMGCode, QTick;
	CQData QData;
	CDBRec Rec;

	m_OptTicker.GetWindowText(OptTick);
	QTick = QData.GetQueryText(OptTick);

	Rec.Add(OptTick);
	m_ImagineCode.GetWindowText(IMGCode);
	Rec.Add(IMGCode);
	m_ImagineBBTick.GetWindowText(BBTick);
	Rec.Add(BBTick);
	
	m_OraLoader.GetSql() = "SELECT COUNT(*) FROM SEMAM.NW_OPT_TICK_MAPS WHERE OPT_TICK = ";
	m_OraLoader.GetSql() += QTick;
	if(m_OraLoader.GetCount() <= 0)
	{
		m_OraLoader.Open("SELECT OPT_TICK, IMAGINE_CODE, IMAGINE_BBTICK "
						"FROM SEMAM.NW_OPT_TICK_MAPS ", ODYNASET_DEFAULT);
		m_OraLoader.UpdateRecord(Rec, TRUE);
	}
	else
	{
		m_OraLoader.GetSql() = "SELECT OPT_TICK, IMAGINE_CODE, IMAGINE_BBTICK "
								"FROM SEMAM.NW_OPT_TICK_MAPS WHERE OPT_TICK = ";
		m_OraLoader.GetSql() += QTick;
		m_OraLoader.Open(m_OraLoader.GetSql(), ODYNASET_DEFAULT);
		m_OraLoader.UpdateRecord(Rec);
	}

	m_SS.SetSheetText(2, m_SS.GetSheetCurRow(), IMGCode);
	m_SS.SetSheetText(3, m_SS.GetSheetCurRow(), BBTick);
}

void COptMap::OnDblClickOptMapList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		m_SS.SetSheetCurRow(Row);
		m_OptTicker.SetWindowText(m_SS.GetSheetText(1, Row));
		m_ImagineCode.SetWindowText(m_SS.GetSheetText(2, Row));
		m_ImagineBBTick.SetWindowText(m_SS.GetSheetText(3, Row));
	}
	else
	{
		m_OptTicker.SetWindowText(EMPTYSTRING);
		m_ImagineCode.SetWindowText(EMPTYSTRING);
		m_ImagineBBTick.SetWindowText(EMPTYSTRING);
	}
	
	GetDlgItem(IDC_OPTMAP_UPDATE_BUTTON)->EnableWindow(m_OptTicker.GetWindowTextLength() > 0 ? TRUE : FALSE);
}

