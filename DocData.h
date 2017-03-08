#ifndef __DOCDATA_H__
#define __DOCDATA_H__

#ifndef __DATAOBJ_H__
#include "dataobj.h"
#endif

class CDocData : public CDataObj
{
DECLARE_DYNAMIC(CDocData)

protected:
	CDBRecArray m_TransTypeArr;
	CDBRecArray m_TransDirArr;
	CDBRecArray m_CurrencyArr;
	CDBRecArray m_CountryArr;
	CDBRecArray m_ProfileCountryArr;
	CDBRecArray m_IndustryArr;
	CDBRecArray m_AssetTypeArr;
	CDBRecArray m_AssetProfileTypeArr;
	CDBRecArray m_AccountArr;
	CDBRecArray m_BlmKeyArr;
	CDBRecArray m_BucketArr;
	CDBRecArray m_CategoryArr;
	CDBRecArray m_CDSFeeTypeArr;
	CDBRecArray m_RateBasisArr;
	CDBRecArray m_TraderArr;
	CDBRecArray m_PFUArr;
	CDBRecArray m_CashTypeArr;
	CDBRecArray m_PmntFrqArr;
	CDBRecArray m_IssuerArr;
	CDBRecArray m_ActionArr;
	CDBRecArray m_RateTypeArr;
	CDBRecArray m_MethodArr;
	CDBRecArray m_LevelArr;
	CDBRecArray m_IndexArr;
	CDBRecArray m_OptSetArr;
	CDBRecArray m_FxSetConvArr;
	CDBRecArray m_CTArr;
	CDBRecArray m_TrDescArr;
	CKeyDBList m_ContactList;
	CDBRecArray m_BestExecutionArr;
	CDBRecArray m_PFClassArr;
	CDBRecArray m_PFSubClassArr;
	CDBRecArray m_YieldCurveArr;

	CString m_TraderIni;
	CString m_TicketNumber;
	CString m_Host;
	BOOL m_bTrader;
	BOOL m_bOperation;
	BOOL m_bPowerUser;
	BOOL m_bSupervisor;
	BOOL m_bGuest;
	BOOL m_bBlotter;
	BOOL m_bBlock;
	BOOL m_bSign;
	BOOL m_bRisk;
	BOOL m_bPastDue;
	BOOL m_bRiskManager;
	double m_SecFee;
	double m_OptFee;

protected:
	int GetUserType(int UserType) { return GetUserStatus() & UserType; }
//	void PreUpdate(CStdioFile &ErrF, LPCTSTR Date, LPCTSTR FName, int PriceRange);
//	ParsePriceData(LPCTSTR Data, CString& Asset, CString& Price, int PriceValue);
//	void DisplayErrorFile(CStdioFile &ErrF);
//	void WriteError(CStdioFile &ErrF, LPCTSTR Text);
//	void ProcessPriceData(CStdioFile &ErrF, LPCTSTR ID, LPCTSTR Value);

public:
	CDocData() { m_Date.Empty(); };
	CDocData(COraLoader &OraLoader);
	~CDocData();

	CDBRecArray &GetTransTypeArr() { return m_TransTypeArr; }
	CDBRecArray &GetTransDirArr() { return m_TransDirArr; }
	CDBRecArray &GetCurrencyArr() { return m_CurrencyArr; }
	CDBRecArray &GetCountryArr() { return m_CountryArr; }
	CDBRecArray &GetProfileCountryArr() { return m_ProfileCountryArr; }
	CDBRecArray &GetIndustryArr() { return m_IndustryArr; }
	CDBRecArray &GetAssetTypeArr() { return m_AssetTypeArr; }
	CDBRecArray &GetAssetProfileTypeArr() { return m_AssetProfileTypeArr; }
	CDBRecArray &GetAccountArr() { return m_AccountArr; }
	CDBRecArray &GetBlmKeyArr() { return m_BlmKeyArr; }
	CDBRecArray &GetBucketArr() { return m_BucketArr; }
	CDBRecArray &GetCategoryArr() { return m_CategoryArr; }
	CDBRecArray &GetCDSFeeTypeArr() { return m_CDSFeeTypeArr; }
	CDBRecArray &GetRateBasisArr() { return m_RateBasisArr; }
	CDBRecArray &GetTraderArr() { return m_TraderArr; }
	CDBRecArray &GetPFUArr() { return m_PFUArr; }
	CDBRecArray &GetCashTypeArr() { return m_CashTypeArr; }
	CDBRecArray &GetPmntFrqArr() { return m_PmntFrqArr; }
	CDBRecArray &GetIssuerArr() { return m_IssuerArr; }
	CDBRecArray &GetActionArr() { return m_ActionArr; }
	CDBRecArray &GetRateTypeArr() { return m_RateTypeArr; }
	CDBRecArray &GetMethodArr() { return m_MethodArr; }
	CDBRecArray &GetLevelArr() { return m_LevelArr; }
	CDBRecArray &GetIndexArr() { return m_IndexArr; }
	CDBRecArray &GetBestExecutionArr() { return m_BestExecutionArr; }
	CDBRecArray &GetPFClassArr() { return m_PFClassArr; }
	CDBRecArray &GetPFSubClassArr() { return m_PFSubClassArr; }
	CDBRecArray &GetOptSetArr() { return m_OptSetArr; }
	CDBRecArray &GetFxSetConv() { return m_FxSetConvArr; }
	CDBRecArray &GetCTArr() { return m_CTArr; }
	CDBRecArray &GetTrDescArr() { return m_TrDescArr; }
	CDBRecArray &GetYieldCurveArr() { return m_YieldCurveArr; }
	CKeyDBList &GetContactList() { return m_ContactList; }

	double &GetSecFee() { return m_SecFee; }
	double &GetOptFee() { return m_OptFee; }

	void SetBlotter(BOOL bBlotter) { m_bBlotter = bBlotter; }
	BOOL GetBlotter() { return m_bBlotter; }

	void SetBlock(BOOL bBlock) { m_bBlock = bBlock; }
	BOOL GetBlock() { return m_bBlock; }
	void SetRisk(BOOL bRisk) { m_bRisk = bRisk; }
	BOOL GetRisk() { return m_bRisk; }
	void SetSign(BOOL bSign) { m_bSign = bSign; }
	BOOL GetSign() { return m_bSign; }

	void SetTrader(BOOL bTrader) { m_bTrader = bTrader; }
	BOOL IsTrader() { return m_bTrader; }
	void SetSupervisor(BOOL bSupervisor) { m_bSupervisor = bSupervisor; }
	BOOL IsSupervisor() { return m_bSupervisor; }
	void SetOperation(BOOL bOperation) { m_bOperation = bOperation; }
	BOOL IsOperation() { return m_bOperation; }
	void SetPowerUser(BOOL bPowerUser) { m_bPowerUser = bPowerUser; }
	BOOL IsGuest() { return m_bGuest; }
	void SetGuest(BOOL bGuest) { m_bGuest = bGuest; }
	BOOL IsPowerUser() { return m_bPowerUser; }
	BOOL IsPastDue() { return m_bPastDue; }
	BOOL IsRiskManager() { return m_bRiskManager; }
	void SetRiskManager(BOOL bRiskManager) { m_bRiskManager = bRiskManager; }

	LPCTSTR GetTraderIni() { return m_TraderIni; }
	void SetTraderIni(LPCTSTR TraderIni) { m_TraderIni = TraderIni; }
	void PastDue(BOOL bPastDue = TRUE) { m_bPastDue = bPastDue; }

	CString &GetTicketNumber() { return m_TicketNumber; }
	void SetTicketNumber(LPCTSTR TicketNumber) { m_TicketNumber = TicketNumber; }

	void LoadFees();
	void LoadDates();
	void LoadUser(BOOL Refresh = FALSE);
	void LoadAssetData(BOOL Refresh = FALSE);
	void LoadSupportData(BOOL Refresh = FALSE);
	void LoadCashData(BOOL Refresh = FALSE);

	void PasswordExpiryWarning();

	BOOL InputDate(CString &Date);
	LPCTSTR DefineTransType(CString &TransType, LPCTSTR Dir);

	int GetUnSignTicketCount();
};

#endif // __DOCDATA_H__