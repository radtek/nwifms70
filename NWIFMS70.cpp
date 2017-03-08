
// NWIFMS70.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "NWIFMS70.h"
#include "MainFrm.h"
#include "oracl.h"
#include "connect.h"

#include "NWIFMS70Doc.h"
#include "NWIFMS70View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNWIFMS70App

BEGIN_MESSAGE_MAP(CNWIFMS70App, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CNWIFMS70App::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
END_MESSAGE_MAP()


// CNWIFMS70App construction

CNWIFMS70App::CNWIFMS70App()
{
	m_User.Empty();
	m_Password.Empty();
	m_Host.Empty();	

	m_bHiColorIcons = TRUE;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CNWIFMS70App::~CNWIFMS70App()
{
	OShutdown();
}

// The one and only CNWIFMS70App object

CNWIFMS70App theApp;
ODatabase theDB;

// CNWIFMS70App initialization

BOOL CNWIFMS70App::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	OStartup();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CNWIFMS70Doc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CNWIFMS70View));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	if(!Login())
		return FALSE;

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
public:
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


// App command to run the dialog
void CNWIFMS70App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CNWIFMS70App customization load/save methods

void CNWIFMS70App::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CNWIFMS70App::LoadCustomState()
{
}

void CNWIFMS70App::SaveCustomState()
{
}

// CNWIFMS70App message handlers

BOOL CNWIFMS70App::Login()
{
	LPCTSTR FileSection = "Login";
	LPCTSTR FileUserEntry = "User";
	LPCTSTR FileHostEntry = "Host";
	CConnect   ConDlg;

	// Get the user name and host from the private profile	
	m_User = GetProfileString((LPCTSTR) FileSection, (LPCTSTR) FileUserEntry);
	m_Host = GetProfileString((LPCTSTR) FileSection, (LPCTSTR) FileHostEntry);
	ConDlg.m_nTrials = 3;
	
    if (ConDlg.DoModal() == IDOK)
	{
		// Save the user name and host
		WriteProfileString((LPCTSTR) FileSection, (LPCTSTR) FileUserEntry, (LPCTSTR) m_User);
		WriteProfileString((LPCTSTR) FileSection, (LPCTSTR) FileHostEntry, (LPCTSTR) m_Host);
		return TRUE;
	}

	return FALSE;
}
