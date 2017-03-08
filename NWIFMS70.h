
// NWIFMS70.h : main header file for the NWIFMS70 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#ifndef __ORADSET_H__
#include <oradset.h>
#endif
// CNWIFMS70App:
// See NWIFMS70.cpp for the implementation of this class
//

class CNWIFMS70App : public CWinAppEx
{
public:
	CNWIFMS70App();
	~CNWIFMS70App();

	CString m_User;
	CString m_Password;
	CString m_Host;

	BOOL Login();
	CString &GetUser() { return m_User; }
	CString &GetPassword() { return m_Password; }
	CString &GetHost() { return m_Host; }

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern ODatabase theDB;
extern CNWIFMS70App theApp;
