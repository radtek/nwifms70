
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "NWIFMS70.h"

#include "MainFrm.h"
#include "nwifms70doc.h"
#include "password.h"
#include "ticketapproval.h"
#include "changefunddlg.h"
#include "pastduetrades.h"
#include "ticketentry.h"
#include "ticketfxentry.h"
// #include "ticketblock.h"
#include "ticketprocess.h"
#include "fmsticket.h"
#include "assetmaturityview.h"
#include "maturityticket.h"
#include "fmsticket.h"
#include "confirmview.h"
#include "cashform.h"
#include "maintenance.h"
#include "payment.h"
#include "assetmaster.h"
#include "assetidview.h"
#include "dpview.h"
#include "dailyinput.h"
#include "report.h"
#include "ideaentryview.h"
#include "ideaapprovalview.h"
#include "ideablotterview.h"
#include "ticketclass.h"
#include "fundsizing.h"
#include "OrderEntry.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_UPDATEINFO, &CMainFrame::OnUpdateinfo)
	ON_COMMAND(ID_PASSWORD, &CMainFrame::OnPassword)
	ON_COMMAND(ID_CHANGE_PORTFOLIO, &CMainFrame::OnChangePortfolio)
	ON_UPDATE_COMMAND_UI(ID_CHANGE_PORTFOLIO, &CMainFrame::OnUpdateChangePortfolio)
	ON_COMMAND_RANGE(ID_TICKET_BLOTTER, ID_ASSETPRIVATEID, OnNWIFMSApplication)
	ON_UPDATE_COMMAND_UI(ID_TICKET_BLOTTER, &CMainFrame::OnUpdateTicketBlotter)
	ON_UPDATE_COMMAND_UI(ID_TICKET_ENTRY, &CMainFrame::OnUpdateTicketEntry)
	ON_UPDATE_COMMAND_UI(ID_TICKET_PROCESS, &CMainFrame::OnUpdateTicketProcess)
	ON_UPDATE_COMMAND_UI(ID_TICKET_SIGN, &CMainFrame::OnUpdateTicketSign)
	ON_UPDATE_COMMAND_UI(ID_TICKET_APPROVAL, &CMainFrame::OnUpdateTicketApproval)
	ON_UPDATE_COMMAND_UI(ID_TRANSACTION, &CMainFrame::OnUpdateTransaction)
	ON_UPDATE_COMMAND_UI(ID_CONFIRM, &CMainFrame::OnUpdateConfirm)
	ON_UPDATE_COMMAND_UI(ID_CASH, &CMainFrame::OnUpdateCash)
	ON_UPDATE_COMMAND_UI(ID_PAYMENT, &CMainFrame::OnUpdatePayment)
	ON_UPDATE_COMMAND_UI(ID_MAINTENANCE, &CMainFrame::OnUpdateMaintenance)
	ON_UPDATE_COMMAND_UI(ID_ASSET, &CMainFrame::OnUpdateAsset)
	ON_UPDATE_COMMAND_UI(ID_DAILY, &CMainFrame::OnUpdateDaily)
	ON_UPDATE_COMMAND_UI(ID_DATA_PROCESSING, &CMainFrame::OnUpdateDataProcessing)
	ON_UPDATE_COMMAND_UI(ID_TICKET_MATURITY, &CMainFrame::OnUpdateTicketMaturity)
	ON_UPDATE_COMMAND_UI(ID_TICKET_PROCESS_MATURITY, &CMainFrame::OnUpdateTicketProcessMaturity)
	ON_UPDATE_COMMAND_UI(ID_IDEA_ENTRY, &CMainFrame::OnUpdateIdeaEntry)
	ON_UPDATE_COMMAND_UI(ID_IDEA_RISK, &CMainFrame::OnUpdateIdeaRisk)
	ON_UPDATE_COMMAND_UI(ID_IDEA_APPROVAL, &CMainFrame::OnUpdateIdeaApproval)
	ON_UPDATE_COMMAND_UI(ID_TICKET_FXENTRY, &CMainFrame::OnUpdateTicketFxentry)
	ON_UPDATE_COMMAND_UI(ID_FUNDSIZING, &CMainFrame::OnUpdateFundSizing)
	ON_UPDATE_COMMAND_UI(ID_TRADE_ORDER, &CMainFrame::OnUpdateTradeOrder)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2005);
	m_nCurrent = 0;
}

CMainFrame::~CMainFrame()
{
	m_nCurrent = 0;
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;
	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// Allow user-defined toolbars operations:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Load menu item image (not placed on any standard toolbars):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// create docking windows
	if (!CreateDockingWindows())
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}

	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);
	CDockablePane* pTabbedBar = NULL;
	m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);
	m_wndProperties.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndProperties);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// load user-defined toolbar images
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			m_UserImages.SetImageSize(CSize(16, 16), FALSE);
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// enable menu personalization (most-recently used commands)
	// TODO: define your own basic commands, ensuring that each pulldown menu has at least one basic command.
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2003);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_VS_2005);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLUE);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_SILVER);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_BLACK);
	lstBasicCommands.AddTail(ID_VIEW_APPLOOK_OFF_2007_AQUA);
	lstBasicCommands.AddTail(ID_SORTING_SORTALPHABETIC);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYTYPE);
	lstBasicCommands.AddTail(ID_SORTING_SORTBYACCESS);
	lstBasicCommands.AddTail(ID_SORTING_GROUPBYTYPE);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	CMenu Menu;

	Menu.Attach(m_wndMenuBar.GetHMenu()); // Create a Menu from MenuBar
	AfxGetMainWnd()->SetMenu(&Menu);      // Set MainWnd menu to this menu. 
	Menu.Detach();						  // Detach Menu
	Menu.DestroyMenu();					  // Destory Menu

	m_wndMenuBar.ShowWindow(FALSE);		  // Hide m_wndMenuBar.
	m_wndOutput.ShowWindow(FALSE);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// Create class view
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Class View window\n");
		return FALSE; // failed to create
	}

	// Create file view
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create File View window\n");
		return FALSE; // failed to create
	}

	// Create output window
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Output window\n");
		return FALSE; // failed to create
	}

	// Create properties window
	CString strPropertiesWnd;
	bNameValid = strPropertiesWnd.LoadString(IDS_PROPERTIES_WND);
	ASSERT(bNameValid);
	if (!m_wndProperties.Create(strPropertiesWnd, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_PROPERTIESWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Properties window\n");
		return FALSE; // failed to create
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndClassView.SetIcon(hClassViewIcon, FALSE);

	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	HICON hPropertiesBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_PROPERTIES_WND_HC : IDI_PROPERTIES_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndProperties.SetIcon(hPropertiesBarIcon, FALSE);
}

void CMainFrame::LoadAppMenu(UINT MenuID, LPCTSTR PopupMenuTitle)
{
	CMenu AppMenu, *pMenu;

	AppMenu.LoadMenu(MenuID);
	ASSERT(AppMenu);

	pMenu = AfxGetMainWnd()->GetMenu();
	ASSERT(*pMenu);
	
	pMenu->InsertMenuA(1, MF_BYPOSITION|MF_POPUP, (UINT) AppMenu.GetSubMenu(0)->GetSafeHmenu(), PopupMenuTitle);

	AfxGetMainWnd()->DrawMenuBar();

	AppMenu.Detach();
	AppMenu.DestroyMenu();
}

void CMainFrame::RemoveAppMenu()
{
	CMenu *pMenu;

	pMenu = AfxGetMainWnd()->GetMenu(); 
	ASSERT(*pMenu);	
	
	pMenu->RemoveMenu(1, MF_BYPOSITION);
	AfxGetMainWnd()->DrawMenuBar();
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// base class does the real work

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

void CMainFrame::OnUpdateinfo()
{
	BeginWaitCursor();
	((CNWIFMS70Doc*) GetActiveDocument())->GetData().LoadUser(TRUE);
	((CNWIFMS70Doc*) GetActiveDocument())->GetData().LoadSupportData(TRUE);
	((CNWIFMS70Doc*) GetActiveDocument())->GetData().LoadAssetData(TRUE);
	EndWaitCursor();
}

void CMainFrame::OnPassword()
{
	CPassword Dlg;
	
	Dlg.m_OraLoader = ((CNWIFMS70Doc*) GetActiveDocument())->GetData().GetOraLoader();
	Dlg.DoModal();
}

void CMainFrame::OnChangePortfolio() 
{
	CChangeFundDlg Dlg;

	Dlg.m_pData = &((CNWIFMS70Doc*) GetActiveDocument())->GetData();
	Dlg.DoModal();
}

void CMainFrame::OnUpdateChangePortfolio(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsPowerUser());
}

void CMainFrame::OnNWIFMSApplication(UINT nCmdID) 
{
	if(nCmdID == m_nCurrent)
		return;

	CDocData *pDocData;
	CString Text;
	CRuntimeClass* pNewViewClass;

	CView* pOldActiveView = GetActiveView();
	::SetWindowLong(pOldActiveView->m_hWnd, GWL_ID, m_nCurrent);

	if(m_nCurrent != 0)
		RemoveAppMenu();

	pDocData = &((CNWIFMS70Doc*) GetActiveDocument())->GetData();

	switch(nCmdID)
	{	
		case ID_TICKET_APPROVAL:
			pDocData->SetBlotter(FALSE);
			pDocData->SetSign(FALSE);
			pNewViewClass = RUNTIME_CLASS(CTicketApproval);
			LoadAppMenu(IDR_APPROVAL, "&Approval");
			break;
		case ID_TICKET_ENTRY:
			if(pDocData->IsPastDue()) // Display Post report
			{
				CPastDueTrades Dlg;

				Dlg.m_pData = pDocData;
				Dlg.DoModal();
				pDocData->PastDue(FALSE);
			}

			pDocData->SetBlotter(FALSE);
			pDocData->SetBlock(FALSE);
			if(pDocData->IsTrader() && pDocData->GetUnSignTicketCount() > 0)
			{
				pDocData->SetSign(TRUE);
				pNewViewClass = RUNTIME_CLASS(CTicketApproval);
				LoadAppMenu(IDR_APPROVAL, "&Sign");
			} 
			else
			{
				pNewViewClass = RUNTIME_CLASS(CTicketEntry);
				LoadAppMenu(IDR_ENTRY, "&Entry");
			}
			break;
		case ID_TICKET_FXENTRY:
			if(pDocData->IsPastDue()) // Display Post report
			{
				CPastDueTrades Dlg;

				Dlg.m_pData = pDocData;
				Dlg.DoModal();
				pDocData->PastDue(FALSE);
			}

			pDocData->SetBlotter(FALSE);
			pDocData->SetBlock(FALSE);
			if(pDocData->IsTrader() && pDocData->GetUnSignTicketCount() > 0)
			{
				pDocData->SetSign(TRUE);
				pNewViewClass = RUNTIME_CLASS(CTicketApproval);
				LoadAppMenu(IDR_APPROVAL, "&Sign");
			} 
			else
			{
				pNewViewClass = RUNTIME_CLASS(CTicketFXEntry);
				LoadAppMenu(IDR_ENTRY, "&Entry");
			}
			break;
/*		case ID_TICKET_BLOCK:
			pDocData->SetBlotter(FALSE);
			pDocData->SetSign(FALSE);
			pNewViewClass = RUNTIME_CLASS(CTicketBlock);
			LoadAppMenu(IDR_BLOCK, "&Block Trade");
			break; */
/*		case ID_TICKET_BLOCKALLOC:
			pDocData->SetBlotter(FALSE);
			pDocData->SetBlock(TRUE);
			pNewViewClass = RUNTIME_CLASS(CTicketEntry);
			LoadAppMenu(IDR_ENTRY, "&Block Trade Alloc");
			break; */
		case ID_TICKET_PROCESS:
			pNewViewClass = RUNTIME_CLASS(CTicketProcess);
			LoadAppMenu(IDR_PROCESS, "&Process");
			break;
		case ID_TICKET_BLOTTER:
			pDocData->SetBlotter(TRUE);
			pDocData->SetSign(FALSE);
			pNewViewClass = RUNTIME_CLASS(CTicketApproval);
			LoadAppMenu(IDR_APPROVAL, "&Blotter");
			break;
		case ID_TICKET_SIGN:
			pDocData->SetBlotter(FALSE);
			pDocData->SetSign(TRUE);
			pNewViewClass = RUNTIME_CLASS(CTicketApproval);
			LoadAppMenu(IDR_APPROVAL, "&Sign");
			break;
		case ID_TICKET_MATURITY:
			pDocData->SetBlotter(FALSE);
			pDocData->SetSign(FALSE);
			pNewViewClass = RUNTIME_CLASS(CAssetMaturityView);
			LoadAppMenu(IDR_ENTRY, "&Entry");
			break;
		case ID_TICKET_PROCESS_MATURITY:
			pDocData->SetBlotter(FALSE);
			pDocData->SetSign(FALSE);
			pNewViewClass = RUNTIME_CLASS(CMaturityTicket);
			LoadAppMenu(IDR_MATURITY, "&Maturity Asset");
			break;
		case ID_TRANSACTION:
			pNewViewClass = RUNTIME_CLASS(CFmsTicket);
			LoadAppMenu(IDR_TRANSACTION, "&Transaction");
			break;
		case ID_CONFIRM:
			pNewViewClass = RUNTIME_CLASS(CConfirmView);
			LoadAppMenu(IDR_CONFIRM, "&Confirmation");
			break;
		case ID_CASH:
			pNewViewClass = RUNTIME_CLASS(CCashForm);
			LoadAppMenu(IDR_CASH, "&Cash");
			break;
		case ID_PAYMENT:
			pNewViewClass = RUNTIME_CLASS(CPayment);
			LoadAppMenu(IDR_PAYMENT, "&Payment");
			break;
		case ID_MAINTENANCE:
			pNewViewClass = RUNTIME_CLASS(CMaintenance);
			LoadAppMenu(IDR_MAINTENANCE, "&Maintenance");
			break;
		case ID_ASSET:
			pNewViewClass = RUNTIME_CLASS(CAssetMaster);
			LoadAppMenu(IDR_ASSET, "&Maintenance");
			break;
		case ID_DAILY:
			pNewViewClass = RUNTIME_CLASS(CDailyInput);
			LoadAppMenu(IDR_DAILY, "&Daily");
			break;
		case ID_DATA_PROCESSING:
			pNewViewClass = RUNTIME_CLASS(CDPView);
			LoadAppMenu(IDR_DATA_PROCESSING, "&Data Processing");
			break;
		case ID_REPORT:
			pNewViewClass = RUNTIME_CLASS(CReport);
			LoadAppMenu(IDR_REPORT, "&Report");
			break;
		case ID_IDEA_ENTRY:
			pNewViewClass = RUNTIME_CLASS(CIdeaEntryView);
			LoadAppMenu(IDR_IDEA_ENTRY_MENU, "Idea &Entry");
			break;
		case ID_IDEA_RISK:
			pDocData->SetRisk(TRUE);
			pNewViewClass = RUNTIME_CLASS(CIdeaApprovalView);
			LoadAppMenu(IDR_IDEA_RISK_MENU, "Idea &Risk");
			break;
		case ID_IDEA_APPROVAL:
			pDocData->SetRisk(FALSE);
			pNewViewClass = RUNTIME_CLASS(CIdeaApprovalView);
			LoadAppMenu(IDR_IDEA_APPROVAL_MENU, "Idea &Approval");
			break;
		case ID_IDEA_BLOTTER:
			pNewViewClass = RUNTIME_CLASS(CIdeaBlotterView);
			LoadAppMenu(IDR_IDEA_BLOTTER_MENU, "Idea &Blotter");
			break;
		case ID_ASSETPRIVATEID:
			pNewViewClass = RUNTIME_CLASS(CAssetIDView);
			break;
		case ID_TRADECLASS:
			pNewViewClass = RUNTIME_CLASS(CTicketClass);
			break;
		case ID_FUNDSIZING:
			pNewViewClass = RUNTIME_CLASS(CFundSizing);
			break;
		case ID_TRADE_ORDER:
			pNewViewClass = RUNTIME_CLASS(COrderEntry);
			LoadAppMenu(IDR_TRADE_ORDER, "Trade &Order Entry");
			break;
		default:
			ASSERT(0);
			return;
	}

	CCreateContext Context;
	Context.m_pNewViewClass = pNewViewClass;
	Context.m_pCurrentDoc = GetActiveDocument();
	Context.m_pNewDocTemplate = NULL;
	Context.m_pLastView = pOldActiveView;
 
	CView* pNewView = STATIC_DOWNCAST(CView, CreateView(&Context));

	if(pNewView != NULL)
	{
		pNewView->OnInitialUpdate();
		SetActiveView(pNewView);
		pNewView->ShowWindow(SW_SHOWMAXIMIZED);
		RecalcLayout();
		m_nCurrent = nCmdID;
		pOldActiveView->DestroyWindow();
	}
}

void CMainFrame::OnUpdateTicketBlotter(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsGuest());
}

void CMainFrame::OnUpdateTicketEntry(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsTrader() || 
					((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsOperation());
}

void CMainFrame::OnUpdateTicketProcess(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsOperation());
}

void CMainFrame::OnUpdateTicketSign(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsTrader());
}


void CMainFrame::OnUpdateTicketApproval(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsSupervisor());
}

void CMainFrame::OnUpdateTransaction(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsOperation() ||
					((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsTrader() ||
					((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsGuest());
}

void CMainFrame::OnUpdateConfirm(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsOperation());
}

void CMainFrame::OnUpdateCash(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsOperation()||
					((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsTrader() || 
					((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsGuest());
}

void CMainFrame::OnUpdatePayment(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsOperation());
}

void CMainFrame::OnUpdateMaintenance(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsOperation() || 
					((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsTrader());
}

void CMainFrame::OnUpdateAsset(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(strcmp(((CNWIFMS70Doc*) GetActiveDocument())->GetData().GetUser(), "LIN") == 0 ? TRUE : FALSE);
}

void CMainFrame::OnUpdateDaily(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsOperation() ||
					((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsGuest());
}

void CMainFrame::OnUpdateDataProcessing(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsOperation() ||
					((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsGuest());
}

void CMainFrame::OnUpdateTicketMaturity(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsOperation() ||
					((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsGuest());
}

void CMainFrame::OnUpdateTicketProcessMaturity(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsOperation() ||
					((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsGuest());
}

void CMainFrame::OnUpdateIdeaEntry(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsTrader());
}

void CMainFrame::OnUpdateIdeaRisk(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsRiskManager());
}

void CMainFrame::OnUpdateIdeaApproval(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsSupervisor());
}

void CMainFrame::OnUpdateTicketFxentry(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(FALSE);
//	pCmdUI->Enable(((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsOperation() ||
//					((CNWIFMS70Doc*) GetActiveDocument())->GetData().IsTrader());
}

void CMainFrame::OnUpdateFundSizing(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CMainFrame::OnUpdateTradeOrder(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}



