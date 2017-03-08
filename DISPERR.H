// disperr.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDisplayErrorFile dialog

class CDisplayErrorFile : public CDialog
{
// Construction
public:
	CDisplayErrorFile(CWnd* pParent = NULL);	// standard constructor
	
// Dialog Data
	//{{AFX_DATA(CDisplayErrorFile)
	enum { IDD = IDD_ERRINFO };
	CStdioFile	m_ErrF;
	CString		m_Title;
	//}}AFX_DATA


// Overrides
public:
	virtual BOOL OnInitDialog();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDisplayErrorFile)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
