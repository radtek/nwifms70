#ifndef __PAYMENTDATA_H__
#define __PAYMENTDATA_H__

#ifndef __MSROWCTRL_H__
#include "msrowctrl.h"
#endif

#ifndef __PAYMENTREC_H__
#include "paymentrec.h"
#endif


class CPayTemplate : public CRowCtrl
{
DECLARE_DYNAMIC(CPayTemplate)
protected:
	CPayTemplateRec m_Rec;
	COraLoader m_OraLoader;

public:
	CPayTemplate() { }
	CPayTemplate(COraLoader &OraLoader, CDBSheet *pSS) { Setup(OraLoader, pSS); }
	CPayTemplate(CPayTemplate &PayTemplate) : CRowCtrl(PayTemplate) 
				{ SetOraLoader(PayTemplate.GetOraLoader()); }
	~CPayTemplate() { }

	void Setup(COraLoader &OraLoader, CDBSheet *pSS) { SetOraLoader(OraLoader), SetSS(pSS); }
	CPayTemplateRec &GetRec() { return m_Rec; }

	COraLoader &GetOraLoader() { return m_OraLoader; }
	void SetOraLoader(COraLoader &OraLoader) { m_OraLoader = OraLoader; }

	int LoadBank(COptComboBox &Bank);
	BOOL LoadDBData();
	virtual BOOL IsReady();
	virtual BOOL AddRow();
	virtual BOOL DeleteRow();
	virtual BOOL UpdateRow();
	virtual BOOL AddSRows(const CString RefID, CStringArray &FundArray);
	virtual BOOL DeleteSRows(const CString RefID);
};

class CPaymentData : public CMSRowCtrl
{
DECLARE_DYNAMIC(CPaymentData)
protected:
	CPayTemplate m_PayTemplate;
	CPaymentRec m_PayRec;
	CPayInvRec m_PayInvRec;
	CPayInvCouponRec m_PayInvCouponRec;
	CDBRecArray m_CustodianArray;
	CStringArray m_PortArr;
	BOOL m_bCoupon;

protected:
	int FindAccount(const char *Portolio, const char *Account);
	BOOL ReptOpenFile(CStdioFile &PayFile);
	void UpdateLines(CStdioFile &PayFile, int &Lines, int AddedLines);
	int ReptWriteHeading(CStdioFile &PayFile);
	int ReptOurRef(CStdioFile &PayFile);
	int ReptPayInstruction(CStdioFile &PayFile);
	BOOL ReptEnd(CStdioFile &PayFile, CString &Portfolio);

public:
	CPaymentData() { SetbCoupon(FALSE); }
	CPaymentData(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS, int KeyIndex)
				{ CMSRowCtrl::Setup(OraLoader, pSS, pSSS, KeyIndex); SetbCoupon(FALSE); }
	~CPaymentData() { } 

	int LoadTemplates(const char *Portfolio, BOOL bLoad = FALSE);
	int LoadTemplates(BOOL bLoad = FALSE);
	int LoadTrades(CDBSheet &SS, BOOL bLoad = FALSE);

	void SetHeaders();
	void GeneratePayment();
	void SetbCoupon(BOOL bCoupon) { m_bCoupon = bCoupon; }
	BOOL GetbCoupon() { return m_bCoupon; }

	double ComputeSum(CDBSheet &SS, CString &Amount);
	BOOL LoadDBData();
	virtual void LoadSheet();
	virtual BOOL DeleteRow();
	virtual BOOL UpdateRow();
	BOOL IsOK();

	int LoadPortArr();
	CPaymentRec &GetPayRec() { return m_PayRec; }
	CPayInvRec &GetPayInvRec() { return m_PayInvRec; }
	CPayInvCouponRec &GetPayInvCouponRec() { return m_PayInvCouponRec; }
	CPayTemplate &GetPayTemplate() { return m_PayTemplate; }
};
#endif