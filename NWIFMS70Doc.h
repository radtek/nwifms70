
// NWIFMS70Doc.h : interface of the CNWIFMS70Doc class
//
#ifndef __NWIFMS70DOC_H__
#define __NWIFMS70DOC_H__

#ifndef __DOCDATA_H__
#include "docdata.h"
#endif

#pragma once


class CNWIFMS70Doc : public CDocument
{
protected: // create from serialization only
	CNWIFMS70Doc();
	DECLARE_DYNCREATE(CNWIFMS70Doc)

// Attributes
public:
	CDocData m_Data;
	CString m_Title;

// Operations
public:
	CDocData& GetData() { return m_Data; }
	void SetViewTitle(LPCTSTR Title);

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CNWIFMS70Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};
#endif

