#ifndef __TICKETREC_H__
#define __TICKETREC_H__

#ifndef __RECOBJ_H__
#include "recobj.h"
#endif

#ifndef __ASSETREC_H__
#include "assetrec.h"
#endif

class CInvRec : public CRecObj
{
DECLARE_DYNAMIC(CInvRec)
protected:
	CString m_TransNum;
	CString m_Asset;
	CString m_NomAmount;
	CString m_CloseDate;
	CString m_ValueDate;
	CString m_Interest;
	CString m_Cash;
	CString m_Currency;
	CString m_TrDesc;
	CString m_Account;
	CString m_PostDate;
	CString m_ForexRef;
	CString m_CrossRate;
	CString m_AssignCP;
	CString m_AssignCT;
	
public:	
	CInvRec(CInvRec &InvRec) { Copy(InvRec); }
	CInvRec(LPCTSTR ID = NULL) { SetID(ID); }

	void SetTransNum(LPCTSTR TransNum) { m_TransNum = TransNum; }
	CString &GetTransNum() { return m_TransNum; }

	void SetAsset(LPCTSTR Asset) { m_Asset = Asset; }
	CString &GetAsset() { return m_Asset; }

	void SetNomAmount(LPCTSTR NomAmount) { m_NomAmount = NomAmount; }
	CString &GetNomAmount() { return m_NomAmount; }

	void SetCloseDate(LPCTSTR CloseDate) { m_CloseDate = CloseDate; }
	CString &GetCloseDate() { return m_CloseDate; }

	void SetValueDate(LPCTSTR ValueDate) { m_ValueDate = ValueDate; }
	CString &GetValueDate() { return m_ValueDate; }

	void SetInterest(LPCTSTR Interest) { m_Interest = Interest; }
	CString &GetInterest() { return m_Interest; }

	void SetCash(LPCTSTR Cash) { m_Cash = Cash; }
	CString &GetCash() { return m_Cash; }

	void SetCurrency(LPCTSTR Currency) { m_Currency = Currency; }
	CString &GetCurrency() { return m_Currency; }

	void SetTrDesc(LPCTSTR TrDesc) { m_TrDesc = TrDesc; }
	CString &GetTrDesc() { return m_TrDesc; }

	void SetAccount(LPCTSTR Account) { m_Account = Account; }
	CString &GetAccount() { return m_Account; }

	void SetPostDate(LPCTSTR PostDate) { m_PostDate = PostDate; }
	CString &GetPostDate() { return m_PostDate; }

	void SetForexRef(LPCTSTR ForexRef) { m_ForexRef = ForexRef; }
	CString &GetForexRef() { return m_ForexRef; }

	void SetCrossRate(LPCTSTR CrossRate) { m_CrossRate = CrossRate; }
	CString &GetCrossRate() { return m_CrossRate; }
	
	void SetAssignCP(LPCTSTR AssignCP) { m_AssignCP = AssignCP; }
	CString &GetAssignCP() { return m_AssignCP; }

	void SetAssignCT(LPCTSTR AssignCT) { m_AssignCT = AssignCT; }
	CString &GetAssignCT() { return m_AssignCT; }

	virtual void Copy(CInvRec &InvRec);
	const CInvRec &operator = (CInvRec &InvRec) { Copy(InvRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = FALSE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE);
	BOOL Closing(COraLoader &OraLoader, LPCTSTR CloseDate, LPCTSTR PostDate, BOOL bClose = TRUE);
};

class CTicketRec : public CRecObj
{
DECLARE_DYNAMIC(CTicketRec)
protected:
	CString m_TransNum;
	CString m_Portfolio;
	CString m_Ticket;
	CString m_CP;
	CString	m_Asset;
	CString m_Trader;
	CString	m_TradeDate;
	CString m_ExecTime;
	CString	m_ValueDate;
	CString m_TransType;
	CString m_Dir;
	CString	m_Amount;
	CString m_Price;
	CString m_Currency;
	CString	m_FxRate;
	CString m_CT;
	CString	m_TRS;
	CString m_Strike;
	CString m_ExerciseDate;
	CString m_OptExp;
	CString m_FXDate;
	CString m_OptTicker;
	CString m_OptSet;
	CString m_OptSet2;
	CString m_SetConvention;
	CString m_OptID;
	CString m_EuroOpt;
	CString m_OptAuto;
	CString m_Broker;
	CString	m_BrokerFee;
	CString m_SoftDollar;
	CString m_OtherFee;
	CString m_VAR;
	CString	m_Rate;
	CString	m_RateBasis;
	CString	m_Maturity;
	CString	m_Index;
	CString m_Formula;
	CString m_SWBooking;
	CString m_SWMaturity;
    CString	m_FxAcc;
    CString	m_USDAcc;
    CString	m_PFU;
	CString m_IPO;
	CString	m_Custodian;
	CString m_Link;
	CString m_TransSeq;
	CString	m_Note;
	CString m_Note2;
	CString m_CPID;
	CString m_DocReq;
	CString m_DocRecvd;
	CString m_Version;
	CString m_WI;
	CString m_Confirm;
	CString m_SwapTicket;
	CString m_DeliveryDate;
	CString m_Margin;
	CString m_MarginCurrency;
	CString m_MarginAmount;
	CString m_Listed;
	CString m_Funded;
	CString m_Binary;
	CString m_DV01;
	CString m_Booker;
	CString m_BookDate;
	CString m_BestExecution;
	CString m_ShortSale;
	CString m_CSPBShort;

	CString m_AssetDesc;
	CString m_AssetCurr;
	CString m_AssetClass;
	CString m_Account;
	CString m_AssignCP;
	CString m_AssignCT;
	CString m_CancelTicket;
	CString m_CorrectTicket;
	CString m_SecFee;
	CString m_OrFee;
	CString m_ETrade;

public:
	CTicketRec() { }
	CTicketRec(LPCTSTR ID) { SetID(ID); }
	CTicketRec(CTicketRec &TicketRec) { Copy(TicketRec); }
	~CTicketRec() {}

	CString &GetTransNum() { return m_TransNum; }
    CString &GetPortfolio() { return m_Portfolio; }
	CString &GetTicket() { return m_Ticket; }
	CString &GetCP() { return m_CP; }
	CString &GetAsset() { return m_Asset; }
	CString &GetTrader() { return m_Trader; }
	CString &GetTradeDate() { return m_TradeDate; }
	CString &GetExecTime() { return m_ExecTime; }
	CString &GetValueDate() { return m_ValueDate; }
	CString &GetTransType() { return m_TransType; }
	CString &GetDir() { return m_Dir; }
	CString &GetAmount() { return m_Amount; }
	CString &GetPrice() { return m_Price; }
	CString &GetCurrency() { return m_Currency; }
	CString &GetFxRate() { return m_FxRate; }
	CString &GetCT() { return m_CT; }
	CString &GetTRS() { return m_TRS; }
	CString &GetStrike() { return m_Strike; }
	CString &GetExerciseDate() { return m_ExerciseDate; }
	CString &GetOptExp() { return m_OptExp; }
	CString &GetFXDate() { return m_FXDate; }
	CString &GetOptTicker() { return m_OptTicker; }
	CString &GetOptSet() { return m_OptSet; }
	CString &GetOptSet2() { return m_OptSet2; }
	CString &GetSetConvention() { return m_SetConvention; }
	CString &GetOptID() { return m_OptID; }
	CString &GetEuroOpt() { return m_EuroOpt; }
	CString &GetOptAuto() { return m_OptAuto; }
	CString &GetBroker() { return m_Broker; }
	CString &GetBrokerFee() { return m_BrokerFee; }
	CString &GetSoftDollar() { return m_SoftDollar; }
	CString &GetOtherFee() { return m_OtherFee; }
	CString &GetVAR() { return m_VAR; }
	CString &GetRate() { return m_Rate; }
	CString &GetRateBasis() { return m_RateBasis; }
	CString &GetMaturity() { return m_Maturity; }
	CString &GetIndex() { return m_Index; }
	CString &GetFormula() { return m_Formula; }
	CString &GetSWBooking() { return m_SWBooking; }
	CString &GetSWMaturity() { return m_SWMaturity; }
    CString &GetFxAcc() { return m_FxAcc; }
    CString &GetUSDAcc() { return m_USDAcc; }
	CString &GetPFU() { return m_PFU; }
	CString &GetIPO() { return m_IPO; }
	CString &GetCustodian() { return m_Custodian; }
	CString &GetLink() { return m_Link; }
	CString &GetTransSeq() { return m_TransSeq; }
	CString &GetNote() { return m_Note; }
	CString &GetNote2() { return m_Note2; }
	CString &GetCPID() { return m_CPID; }
	CString &GetDocReq() { return m_DocReq; }
	CString &GetDocRecvd() { return m_DocRecvd; }
	CString &GetVersion() { return m_Version; }	
    CString &GetWI() { return m_WI; }
	CString &GetConfirm() { return m_Confirm; }
	CString &GetSwapTicket() { return m_SwapTicket; }
	CString &GetDeliveryDate() { return m_DeliveryDate; }
	CString &GetMargin() { return m_Margin; }
	CString &GetMarginCurrency() { return m_MarginCurrency; }
	CString &GetMarginAmount() { return m_MarginAmount; }
	CString &GetListed() { return m_Listed; }
	CString &GetFunded() { return m_Funded; }
	CString &GetBinary() { return m_Binary; }
	CString &GetDV01() { return m_DV01; }
	CString &GetBooker() { return m_Booker; }
	CString &GetBookDate() { return m_BookDate; }
	CString &GetBestExecution() { return m_BestExecution; }
	CString &GetShortSale() { return m_ShortSale; }
	CString &GetCSPBShort() { return m_CSPBShort; }
	CString &GetCancelTicket() { return m_CancelTicket; }
	CString &GetCorrectTicket() { return m_CorrectTicket; }
	CString &GetSecFee() { return m_SecFee; }
	CString &GetOrFee() { return m_OrFee; }
	CString &GetETrade() { return m_ETrade; }

    CString &GetAssetDesc() { return m_AssetDesc; }
    CString &GetAssetCurr() { return m_AssetCurr; }
	CString &GetAssetClass() { return m_AssetClass; }
	CString &GetAccount() { return m_Account; }
	CString &GetAssignCP() { return m_AssignCP; }
	CString &GetAssignCT() { return m_AssignCT; }

	void SetTransNum(LPCTSTR TransNum) { m_TransNum = TransNum; }
	void SetPortfolio(LPCTSTR Portfolio) { m_Portfolio = Portfolio; }
	void SetTicket(LPCTSTR Ticket) { m_Ticket = Ticket; }
	void SetCP(LPCTSTR CP) { m_CP = CP; }
	void SetAsset(LPCTSTR Asset) { m_Asset = Asset; }
	void SetTrader(LPCTSTR Trader) { m_Trader = Trader; }
	void SetTradeDate(LPCTSTR TradeDate) { m_TradeDate = TradeDate; }
	void SetExecTime(LPCTSTR ExecTime) { m_ExecTime = ExecTime; }
	void SetValueDate(LPCTSTR ValueDate) { m_ValueDate = ValueDate; }
	void SetTransType(LPCTSTR TransType) { m_TransType = TransType; }
	void SetDir(LPCTSTR Dir) { m_Dir = Dir; }
	void SetAmount(LPCTSTR Amount) { m_Amount = Amount; }
	void SetPrice(LPCTSTR Price) { m_Price = Price; }
	void SetCurrency(LPCTSTR Currency) { m_Currency = Currency; }
	void SetFxRate(LPCTSTR FxRate) { m_FxRate = FxRate; }
	void SetCT(LPCTSTR CT) { m_CT = CT; }
	void SetTRS(LPCTSTR TRS) { m_TRS = TRS; }
	void SetStrike(LPCTSTR Strike) { m_Strike = Strike; }
	void SetExerciseDate(LPCTSTR ExerciseDate) { m_ExerciseDate = ExerciseDate; }
	void SetOptExp(LPCTSTR OptExp) { m_OptExp = OptExp; }
	void SetFXDate(LPCTSTR FXDate) { m_FXDate = FXDate; }
	void SetOptTicker(LPCTSTR OptTicker) { m_OptTicker = OptTicker; }
	void SetOptSet(LPCTSTR OptSet) { m_OptSet = OptSet; }
	void SetOptSet2(LPCTSTR OptSet2) { m_OptSet2 = OptSet2; }
	void SetSetConvention(LPCTSTR SetConvention) { m_SetConvention = SetConvention; }
	void SetOptID(LPCTSTR OptID) { m_OptID = OptID; }
	void SetEuroOpt(LPCTSTR EuroOpt) { m_EuroOpt = EuroOpt; }
	void SetOptAuto(LPCTSTR OptAuto) { m_OptAuto = OptAuto; }
	void SetBroker(LPCTSTR Broker) { m_Broker = Broker; }
	void SetBrokerFee(LPCTSTR BrokerFee) { m_BrokerFee = BrokerFee; }
	void SetSoftDollar(LPCTSTR SoftDollar) { m_SoftDollar = SoftDollar; }
	void SetOtherFee(LPCTSTR OtherFee) { m_OtherFee = OtherFee; }
	void SetVAR(LPCTSTR VAR) { m_VAR = VAR; }
	void SetRate(LPCTSTR Rate) { m_Rate = Rate; }
	void SetRateBasis(LPCTSTR RateBasis) { m_RateBasis = RateBasis; }
	void SetMaturity(LPCTSTR Maturity) { m_Maturity = Maturity; }
	void SetIndex(LPCTSTR Index) { m_Index = Index; }
	void SetFormula(LPCTSTR Formula) { m_Formula = Formula; }
	void SetSWBooking(LPCTSTR SWBooking) { m_SWBooking = SWBooking; }
	void SetSWMaturity(LPCTSTR SWMaturity) { m_SWMaturity = SWMaturity; }
	void SetFxAcc(LPCTSTR FxAcc ) { m_FxAcc = FxAcc; }
	void SetUSDAcc(LPCTSTR USDAcc) { m_USDAcc = USDAcc; }
	void SetPFU(LPCTSTR PFU) { m_PFU = PFU; }
	void SetIPO(LPCTSTR IPO) { m_IPO = IPO; }
	void SetCustodian(LPCTSTR Custodian) { m_Custodian = Custodian; }
	void SetLink(LPCTSTR Link) { m_Link = Link; }
	void SetTransSeq(LPCTSTR TransSeq) { m_TransSeq = TransSeq; }
	void SetNote(LPCTSTR Note) { m_Note = Note; }
	void SetNote2(LPCTSTR Note2) { m_Note2 = Note2; }
	void SetCPID(LPCTSTR CPID) { m_CPID = CPID; }
	void SetDocReq(LPCTSTR DocReq) { m_DocReq = DocReq; }
	void SetDocRecvd(LPCTSTR DocRecvd) { m_DocRecvd = DocRecvd; }
	void SetWI(LPCTSTR WI) { m_WI = WI; }
	void SetConfirm(LPCTSTR Confirm) { m_Confirm = Confirm; }
	void SetSwapTicket(LPCTSTR SwapTicket) { m_SwapTicket = SwapTicket; }
	void SetDeliveryDate(LPCTSTR DeliveryDate) { m_DeliveryDate = DeliveryDate; }
	void SetMargin(LPCTSTR Margin) { m_Margin = Margin; }
	void SetMarginCurrency(LPCTSTR MarginCurrency) { m_MarginCurrency = MarginCurrency; }
	void SetMarginAmount(LPCTSTR MarginAmount) { m_MarginAmount = MarginAmount; }
	void SetListed(LPCTSTR Listed) { m_Listed = Listed; }
	void SetFunded(LPCTSTR Funded) { m_Funded = Funded; }
	void SetBinary(LPCTSTR Binary) { m_Binary = Binary; }
	void SetDV01(LPCTSTR DV01) { m_DV01 = DV01; }
	void SetBooker(LPCTSTR Booker) { m_Booker = Booker; }
	void SetBookDate(LPCTSTR BookDate) { m_BookDate = BookDate; }
	void SetBestExecution(LPCTSTR BestExecution) { m_BestExecution = BestExecution; }
	void SetShortSale(LPCTSTR ShortSale) { m_ShortSale = ShortSale; }
	void SetCSPBShort(LPCTSTR CSPBShort) { m_CSPBShort = CSPBShort; }
	void SetCancelTicket(LPCTSTR CancelTicket) { m_CancelTicket = CancelTicket; }
	void SetCorrectTicket(LPCTSTR CorrectTicket) { m_CorrectTicket = CorrectTicket; }
	void SetSecFee(LPCTSTR SecFee) { m_SecFee = SecFee; }
	void SetOrFee(LPCTSTR OrFee) { m_OrFee = OrFee; }
	void SetETrade(LPCTSTR ETrade) { m_ETrade = ETrade; }

	void SetVersion();
    void SetVersion(LPCTSTR Version) { m_Version = Version; }

	void SetAssetDesc(LPCTSTR AssetDesc) { m_AssetDesc = AssetDesc; }
	void SetAssetCurr(LPCTSTR AssetCurr) { m_AssetCurr = AssetCurr; }
	void SetAssetClass(LPCTSTR AssetClass) { m_AssetClass = AssetClass; }
	void SetAccount(LPCTSTR Account) { m_Account = Account; }
	void SetAssignCP(LPCTSTR AssignCP) { m_AssignCP = AssignCP; }
	void SetAssignCT(LPCTSTR AssignCT) { m_AssignCT = AssignCT; }

	void FromAssetRec(CAssetRec &AssetRec);
	void ToAssetRec(CAssetRec &AssetRec);
	void Copy(CTicketRec &TicketRec);
	const CTicketRec &operator = (CTicketRec &TicketRec) { Copy(TicketRec); return *this; }
	BOOL GenerateInv(COraLoader &OraLoader);

	BOOL UpdateFloatRepoRateRec(COraLoader &OraLoader, BOOL bDelete = FALSE);
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = FALSE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE);
	BOOL UpdateCurrPos(COraLoader &OraLoader, int nAction);
	BOOL UpdateCurrPosRecs(COraLoader &OraLoader, CString &Asset, 
							CString &TransType, CString &Ticket);
};
#endif
