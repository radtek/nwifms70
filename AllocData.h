#ifndef __ALLOCDATA_H__
#define __ALLOCDATA_H__

#ifndef __ROWCTRL_H__
#include "rowctrl.h"
#endif

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __ASSETVAL_H__
#include "assetval.h"
#endif

#ifndef __RAWTICKETREC_H__
#include "rawticketrec.h"
#endif


class CAllocData : public CRowCtrl
{
DECLARE_DYNAMIC(CAllocData)
protected:
	COraLoader m_OraLoader;
	CAssetVal m_Val;
	BOOL m_bAutoRepo;
	BOOL m_bLocal;
	CString m_PB;
	CString m_Formula;
	double m_Price;
	double m_BrFee;
	double m_SoftDollar;
	double m_OtherFee;
	double m_VAR;
	double m_Par;
	double m_PrePaid;
	double m_Accretion;
	double m_NetPrice;
	double m_Cash;
	CRawTicketRec m_Ticket;
	CRawInvRec m_Inv;

public:
	CAllocData() { m_bAutoRepo = FALSE; }
	CAllocData(COraLoader &OraLoader, CDBSheet *pSS) { Setup(OraLoader, pSS); }
	~CAllocData() {}

	void Setup(COraLoader &OraLoader, CDBSheet *pSS) { CRowCtrl::Setup(pSS); SetOraLoader(OraLoader); }
	void Setup(COraLoader &OraLoader, CRawTicketRec &RawTicket, LPCTSTR Daily);

	COraLoader &GetOraLoader() { return m_OraLoader; }
	void SetOraLoader(COraLoader &OraLoader) { m_OraLoader = OraLoader; }

	void SetAutoRepo(BOOL bAutoRepo = TRUE) { m_bAutoRepo = bAutoRepo; }
	BOOL GetAutoRepo() { return m_bAutoRepo; }
	BOOL GetLocal() { return m_bLocal; }

	double GetPrePaid() { return m_PrePaid; }
	double GetAccretion() { return m_Accretion; }
	double GetBrFee() { return m_BrFee; }
	double GetSoftDollar() { return m_SoftDollar; }
	double GetOtherFee() { return m_OtherFee; }
	double GetVAR() { return m_VAR; }
	double GetNetPrice() { return m_NetPrice; }
	double GetPrice() { return m_Price; }
	double GetCash() { return m_Cash; }

	void SetPrePaid(double PrePaid) { m_PrePaid = PrePaid; }
	void SetAccretion(double Accretion) { m_Accretion = Accretion; }
	void SetBrFee(double BrFee) { m_BrFee = BrFee; }
	void SetSoftDollar(double SoftDollar) { m_SoftDollar = SoftDollar; }
	void SetOtherFee(double OtherFee) { m_OtherFee = OtherFee; }
	void SetVAR(double VAR) { m_VAR = VAR; }
	void SetNetPrice(double NetPrice) { m_NetPrice = NetPrice; }
	void SetPrice(double Price) { m_Price = Price; }
	void SetCash(double Cash) { m_Cash = Cash; }

	CString &GetPB() { return m_PB; }
	LPCTSTR GetFormula() { return (LPCTSTR) m_Formula; }

	CRawTicketRec &GetTicket() { return m_Ticket; }
	CRawInvRec &GetInv() { return m_Inv; }

	void SetupAssetInfo();
	double RepoPrice(double DownPay, double Price);
	void GenerateRepo();
	void ComputeValue(double OtherFee);
	int AllocCheckSum(LPCTSTR Portfolio = NULL, double CurrentAmount = 0);
	double ComputeAllocAmount(LPCTSTR Portfolio = NULL, double CurrentAmount = 0);

	BOOL GetSelRawInv(int Index, CRawInvRec &RecInvRec);
	BOOL LoadIDs(CString &Common, CString &Cusip, CString &Sedol, CString &Isin);
	void LoadAlloc(CRowCtrl &m_Data, const CString DownPayment);
};
#endif 