#ifndef __PROCESSDATA_H__
#define __PROCESSDATA_H__

#ifndef __ENTRYDATA_H__
#include "entrydata.h"
#endif

#ifndef __TICKETREC_H__
#include "ticketrec.h"
#endif

#ifndef __ASSETVAL_H__
#include "assetval.h"
#endif

class CProcessData : public CEntryData
{
DECLARE_DYNAMIC(CProcessData)
protected:
	CTicketRec m_Ticket;
	CInvRec m_Inv;
	CAssetVal m_Val;
	CString m_Booker;
	CString m_BookDate;
	double m_NetPrice;
	double m_OtherFee;
	double m_Cash;
	double m_PrePaid;
	double m_Accretion;
	CDBRecArray m_FxRecArray;

protected:
	virtual BOOL DeleteDataRow();
	virtual BOOL UpdateDataRow();
	BOOL GenerateTicketRec(CRawInvRec &RawInvRec);
	BOOL GetSelInv(int Index, CInvRec &Inv);
	double GetParValue();

public:
	CProcessData() { }
	CProcessData(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS, LPCTSTR User)
				{ Setup(OraLoader, pSS, pSSS, User); }
	~CProcessData() {}

	void Setup(COraLoader &OraLoader, CDBSheet *pSS, CDBSheet *pSSS, LPCTSTR User, int KeyIndex = -1);
	
	CTicketRec &GetTicket() { return m_Ticket; }
	CInvRec &GetInv() { return m_Inv; }
	virtual BOOL GetSelRawInv(int Index, CRawInvRec &RecInvRec);

	CString &GetBooker() { return m_Booker; }
	CString &GetBookDate() { return m_BookDate; }

	CDBRecArray &GetFxRecArray() { return m_FxRecArray; }

	BOOL IsTicketProcessed();
	virtual int LoadAllocList();
	int ReLoadAllocList(const CString Ticket, const CString OptTicket);
	virtual int LoadTickets();
	virtual void UpdateSheetRow(BOOL bFromSheet = TRUE);
	BOOL UpdateFxRef(CString TransNum, CString Portfolio);
	void SetupAssetInfo();
	void ComputeValue(BOOL bSec, BOOL bOrFee);

	double ComputeOtherFee(double Price, BOOL bSec, BOOL bOrFee);
	double GetNetPrice() { return m_NetPrice; }
	double GetOtherFee() { return m_OtherFee; }
	double GetCash() { return m_Cash; }
	double GetPrePaid() { return m_PrePaid; }
	double GetAccretion() { return m_Accretion; }
	CAssetVal GetAssetVal() { return m_Val; }

	void SetNetPrice(double NetPrice) { m_NetPrice = NetPrice; }
	void SetOtherFee(double OtherFee) { m_OtherFee = OtherFee; }
	void SetCash(double Cash) { m_Cash = Cash; }
	void SetPrePaid(double PrePaid) { m_PrePaid = PrePaid; }
	void SetAccretion(double Accretion) { m_Accretion = Accretion; }
	void LoadAllocTicket(CStringArray &AllocTickets);
};
#endif