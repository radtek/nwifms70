#ifndef __PROHIBITLIST_H__
#define __PROHIBITLIST_H__
#pragma once


#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

// CProhibitList dialog

class CProhibitList : public CDialog
{
	DECLARE_DYNAMIC(CProhibitList)

public:
	COraLoader m_OraLoader;
	CEdit m_ProhibitList;

public:
	CProhibitList(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProhibitList();

// Dialog Data
	enum { IDD = IDD_PROHIBITLIST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	// Generated message map functions
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
#endif