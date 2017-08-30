#ifndef __ASSETFINDDATA_H__
#define __ASSETFINDDATA_H__

#ifndef __ROWCTRL_H__
#include "rowctrl.h"
#endif

#ifndef __ORALOADER_H__
#include "oraloader.h"
#endif

#ifndef __ASSETREC_H__
#include "assetrec.h"
#endif

class CAssetFindData : public CRowCtrl
{
DECLARE_DYNAMIC(CAssetFindData)
protected:
	COraLoader m_OraLoader;
	CAssetRec m_Rec;
	CString m_TransType;
	CString m_Ticket;
	CString m_Strike;
	CString m_Nominal;
	CString m_Dir;
	CString m_OptID;
	CString m_OptTick;
	CString m_DeliveryDate;
	CString m_OptSetCode;
	CString m_Custodian;
	CString m_AssignCP;
	CString m_Trader;
	BOOL m_bWithNew;
	BOOL m_bTicketBooking;

public:
	CAssetFindData() { SetWithNew(); SetTicketBooking(FALSE); }
	CAssetFindData(COraLoader &OraLoader, CDBSheet *pSS, BOOL bWithNew = TRUE) 
		: CRowCtrl(pSS) { SetOraLoader(OraLoader); SetWithNew(bWithNew); SetTicketBooking(FALSE); }
	~CAssetFindData() {}
	
	BOOL IsRowOK();
	void LoadHoldings(LPCTSTR Asset, CDBSheet &HoldingSS);
	
	COraLoader &GetOraLoader() { return m_OraLoader; }
	void SetOraLoader(COraLoader &OraLoader) { m_OraLoader = OraLoader; }
	void Setup(COraLoader &OraLoader, CDBSheet *pSS, BOOL bWithNew = TRUE) 
			{ CRowCtrl::Setup(pSS), SetOraLoader(OraLoader); SetWithNew(bWithNew); }
	void SetWithNew(BOOL bWithNew = TRUE) { m_bWithNew = bWithNew; }
	BOOL GetWithNew() { return m_bWithNew; }
	void SetTicketBooking(BOOL bTicketBooking) { m_bTicketBooking = bTicketBooking; }
	BOOL GetTicketBooking() { return m_bTicketBooking; }
	
	CAssetRec &GetRec() { return m_Rec; }
	void SetRecArray(CStringArray &Rec);
	CString &GetStrike() { return m_Strike; }
	CString &GetNominal() { return m_Nominal; }
	CString &GetTransType() { return m_TransType; }
	CString &GetTicket() { return m_Ticket; }
	CString &GetDir() { return m_Dir; }
	CString &GetOptID() { return m_OptID; }
	CString &GetOptTick() { return m_OptTick; }
	CString &GetDeliveryDate() { return m_DeliveryDate; }
	CString &GetOptSetCode() { return m_OptSetCode; }
	CString &GetCustodian() { return m_Custodian; }
	CString &GetAssignCP() { return m_AssignCP; }
	CString &GetTrader() { return m_Trader; }
	int LoadDBData(BOOL bInitial = FALSE);
	int LoadCustodian(LPCTSTR Asset);
};
#endif // ASSETFINDDATA_H