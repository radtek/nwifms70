#ifndef __REPORTDATE_H__
#define __REPORTDATE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ReportDate.h : header file
//

extern const int DATE_TODAY;
extern const int DATE_MONTH_START;
extern const int DATE_PREV_MONTH_END;
extern const int DATE_FUND_START;
extern const int DATE_PREV_YEAR_END;

/////////////////////////////////////////////////////////////////////////////
// CReportDate dialog
class CReportDateItem : public CObject
{
DECLARE_DYNAMIC(CReportDateItem)
public:
	CReportDateItem() {};
	CReportDateItem(CReportDateItem &Item);
	CReportDateItem(LPCTSTR Name, LPCTSTR LongName, LPCTSTR Date, int DateMethod);
	~CReportDateItem() {}

protected:
	CString m_Name;
	CString m_LongName;
	CString m_Date;
	int m_nDateMethod;

public:
	void SetName(LPCTSTR Name) { m_Name = Name; }
	void SetLongName(LPCTSTR LongName) { m_LongName = LongName; }
	void SetDate(LPCTSTR Date) { m_Date = Date; }
	void SetDateMethod(int DateMethod) { m_nDateMethod = DateMethod; }

	CString &GetName() { return m_Name; }
	CString &GetLongName() { return m_LongName; }
	CString &GetDate() { return m_Date; }
	int GetDateMethod() { return m_nDateMethod; }
};

class CReportDateItemArray : public CObArray
{
DECLARE_DYNAMIC(CReportDateItemArray)
public:
	CReportDateItemArray() { };
	~CReportDateItemArray() { RemoveAll(); }

	int	Add(CReportDateItem *pItem) { return CObArray::Add(pItem); }
	void SetAt(int Index, CReportDateItem *pItem);
	void InsertAt(int Index, CReportDateItem *pItem) { CObArray::InsertAt(Index, pItem); }
	int Add(CReportDateItem &Item) { return CObArray::Add(new CReportDateItem(Item)); }
	void SetAt(int Index, CReportDateItem &Item);
	void InsertAt(int Index, CReportDateItem &Item) { CObArray::InsertAt(Index, new CReportDateItem(Item)); }
	void RemoveAt(int Index);
	void RemoveAll();
	CReportDateItem *GetAt(int Index);
};

class CReportDate : public CDialog
{
// Construction
public:
	CReportDate(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CReportDate)
	enum { IDD = IDD_REPORTDATE_DIALOG };
	CString m_Report;
	CString m_Today;
	int m_nDay;
	int m_nMonth;
	int m_nYear;
	CReportDateItemArray m_DateArray;	
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReportDate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void AddDateItem(CString Flag, LPCTSTR Name, LPCTSTR LongName, int DateMethod);

protected:
	void ConvertDate(CString &Date);
	int GetLastDay(int Month, int Year);
	void GetDateInfo(CReportDateItem *pItem);
	// Generated message map functions
	//{{AFX_MSG(CReportDate)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
