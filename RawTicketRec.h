#ifndef __RAWTICKETREC_H__
#define __RAWTICKETREC_H__

#ifndef __RECOBJ_H__
#include "recobj.h"
#endif

#ifndef __ASSETREC_H__
#include "assetrec.h"
#endif

class CRawInvRec : public CRecObj
{
DECLARE_DYNAMIC(CRawInvRec)
protected:
	CString m_Ticket;
	CString m_Portfolio;
	CString m_NomAmount;
	CString m_Price;
	CString m_DownPymnt;
	CString m_Link;

	CString m_NetPrice;
	CString m_Custodian;
	CString m_Account;

public:
	CRawInvRec() {}
	CRawInvRec(CRawInvRec &RawInvRec) { Copy(RawInvRec); }
	CRawInvRec(LPCTSTR ID) : CRecObj(ID) { }
	~CRawInvRec() { }

	void SetTicket(LPCTSTR Ticket) { m_Ticket = Ticket; }
	CString &GetTicket() { return m_Ticket; }

	CString &GetPortfolio() { return m_Portfolio; }
	CString &GetNomAmount() { return m_NomAmount; }
	CString &GetPrice() { return m_Price; }
	CString &GetDownPymnt() { return m_DownPymnt; }
	CString &GetLink() { return m_Link; }

	CString &GetNetPrice() { return m_NetPrice; }
	CString &GetCustodian() { return m_Custodian; }
	CString &GetAccount() { return m_Account; }

	void SetPortfolio(LPCTSTR Portfolio) { m_Portfolio = Portfolio; }
	void SetNomAmount(LPCTSTR NomAmount) { m_NomAmount = NomAmount; }
	void SetPrice(LPCTSTR Price) { m_Price = Price; }
	void SetDownPymnt(LPCTSTR DownPymnt) { m_DownPymnt = DownPymnt; } 
	void SetLink(LPCTSTR Link) { m_Link = Link; }

	void SetNetPrice(LPCTSTR NetPrice) { m_NetPrice = NetPrice; }
	void SetCustodian(LPCTSTR Custodian) { m_Custodian = Custodian; }
	void SetAccount(LPCTSTR Account) { m_Account = Account; }

	virtual void Copy(CRawInvRec &RawInvRec);
	const CRawInvRec &operator = (CRawInvRec &RawInvRec) { Copy(RawInvRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = FALSE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE);
	BOOL UpdateCustodian(COraLoader &OraLoader);
};

class CRawTicketRec : public CRecObj
{
DECLARE_DYNAMIC(CRawTicketRec)
protected:
	double m_AvailAmount;

	CString m_Ticket;
    CString	m_Trader;
	CString m_DefTrader;
	CString m_ExecTime;
	CString	m_TradeDate;
	CString	m_ValueDate;
    CString	m_Dir;
    CString	m_TransType;
	CString m_NomAmount;
	CString m_OriNominal;
	CString m_Strike;
	CString m_OptExp;
	CString m_FXDate;
	CString m_OptTicker;
	CString m_OptID;
	CString m_Listed;
	CString m_EuroOpt;
	CString m_OptAuto;
    CString	m_Currency;
	CString m_FxRate;
    CString	m_CP;
    CString	m_Broker;
	CString	m_Asset;
	CString m_AssetDesc;
	CString m_AssetCommon;
	CString m_AssetCusip;
	CString m_AssetIsin;
	CString m_AssetSedol;
	CString m_AssetCountry;
	CString m_AssetCurr;
	CString m_AssetClass;
	CString m_AssetCategory;
	CString m_AssetBucket;
	CString m_AssetIndustry;
	CString m_AssetCoupon;
	CString m_AssetMaturity;
	CString m_AssetSource;
	CString m_AssetImagineCode;
	CString m_IPO;
	CString	m_TRS;
	CString m_RepoCP;
	CString m_RepoCT;
	CString m_RepoType;
	CString m_Rate;
	CString m_RateBasis;
	CString m_Maturity;
	CString	m_Note;
	CString	m_Note2;
	CString m_Processed;
	CString m_User;
	CString m_SWBooking;
	CString m_SWMaturity;
	CString m_SWFloatRate;
	CString m_SWFloatBasis;
	CString m_SWFloatMaturity;
	CString m_WI;
	CString m_OptSetCode;
	CString m_OptSetCode2;
	CString m_SetConvention;
	CString m_SwapTicket;
	CString m_DeliveryDate;
	CString m_Margin;
	CString m_MarginCurrency;
	CString m_MarginAmount;
	CString m_Funded;
	CString m_Binary;
	CString m_AAMethod;
	CString m_AAFReason;
	CString m_BestExecution;
	CString m_ShortSale;
	CString m_CSPBShort;
	CString m_Price;
	CString m_DownPymnt;
	CString m_BrokerFee;
	CString m_OtherFee;
	CString m_SoftDollar;
	CString m_AssignCP;
	CString m_AssignCT;
	CString m_DV01;
	CString m_VAR;
	CString m_UnWindTicket;
	CString m_CancelTicket;
	CString m_CorrectTicket;
	CString m_SecFee;
	CString m_OrFee;
	CString m_ETrade;
	CString m_ImagineExecID;
	CString m_CPTradeID;
	CString m_OrderID;

	CString m_Sign;
	CString m_NetPrice;

protected:
	void SetProcessed(LPCTSTR Processed) { m_Processed = Processed; }
	void SetUser(LPCTSTR User) { m_User = User; }
	void ToShortDBRec(CDBRec &Rec, BOOL bFloat = FALSE);

public:
	CRawTicketRec() { m_Processed.Empty(); }
	CRawTicketRec(LPCTSTR User, LPCTSTR Processed) { Setup(User, Processed); }
	CRawTicketRec(CRawTicketRec &RawTicketRec) { Copy(RawTicketRec); }
	~CRawTicketRec() {}

	void Setup(LPCTSTR User, LPCTSTR Processed = Y) 
				{ SetUser(User); SetProcessed(Processed); }
	double GetAvailAmount() { return m_AvailAmount; }

	CString &GetTicket() { return m_Ticket; }
	CString &GetTrader() { return m_Trader; }
	CString &GetDefTrader() { return m_DefTrader; }	
	CString &GetTradeDate() { return m_TradeDate; }
	CString &GetExecTime() { return m_ExecTime; }
	CString &GetValueDate() { return m_ValueDate; }
	CString &GetDir() { return m_Dir; }
	CString &GetTransType() { return m_TransType; }
	CString &GetNomAmount() { return m_NomAmount; }
	CString &GetOriNominal() { return m_OriNominal; }
	CString &GetStrike() { return m_Strike; }
	CString &GetOptExp() { return m_OptExp; }
	CString &GetFXDate() { return m_FXDate; }
	CString &GetOptTicker() { return m_OptTicker; }
	CString &GetOptID() { return m_OptID; }
	CString &GetListed() { return m_Listed; }
	CString &GetEuroOpt() { return m_EuroOpt; }
	CString &GetOptAuto() { return m_OptAuto; }
	CString &GetCurrency() { return m_Currency; }
	CString &GetFxRate() { return m_FxRate; }
	CString &GetCP() { return m_CP; }
	CString &GetBroker() { return m_Broker; }
	CString &GetAsset() { return m_Asset; }
	CString &GetAssetDesc() { return m_AssetDesc; }
	CString &GetAssetCommon() { return m_AssetCommon; }
	CString &GetAssetCusip() { return m_AssetCusip; }
	CString &GetAssetIsin() { return m_AssetIsin; }
	CString &GetAssetSedol() { return m_AssetSedol; }
	CString &GetAssetCountry() { return m_AssetCountry; }
	CString &GetAssetCurr() { return m_AssetCurr; }
	CString &GetAssetClass() { return m_AssetClass; }
	CString &GetAssetCategory() { return m_AssetCategory; }
	CString &GetAssetBucket() { return m_AssetBucket; }
	CString &GetAssetIndustry() { return m_AssetIndustry; }
	CString &GetAssetCoupon() { return m_AssetCoupon; }
	CString &GetAssetMaturity() { return m_AssetMaturity; }
	CString &GetAssetSource() { return m_AssetSource; }
	CString &GetAssetImagineCode() { return m_AssetImagineCode; }
	CString &GetIPO() { return m_IPO; }
	CString &GetTRS() { return m_TRS; }
	CString &GetRepoCP() { return m_RepoCP; }
	CString &GetRepoCT() { return m_RepoCT; }
	CString &GetRepoType() { return m_RepoType; }
	CString &GetRate() { return m_Rate; }
	CString &GetRateBasis() { return m_RateBasis; }
	CString &GetMaturity() { return m_Maturity; }
	CString &GetNote() { return m_Note; }
	CString &GetNote2() { return m_Note2; }
	CString &GetProcessed() { return m_Processed; }
	CString &GetUser() { return m_User; }
	CString &GetSWBooking() { return m_SWBooking; }
	CString &GetSWMaturity() { return m_SWMaturity; }
	CString &GetSWFloatRate() { return m_SWFloatRate; }
	CString &GetSWFloatBasis() { return m_SWFloatBasis; }
	CString &GetSWFloatMaturity() { return (CString&) m_SWFloatMaturity; }
	CString &GetWI() { return m_WI; }
	CString &GetOptSetCode() { return m_OptSetCode; }
	CString &GetOptSetCode2() { return m_OptSetCode2; }
	CString &GetSetConvention() { return m_SetConvention; }
	CString &GetSwapTicket() { return m_SwapTicket; }
	CString &GetDeliveryDate() { return m_DeliveryDate; }
	CString &GetMargin() { return m_Margin; }
	CString &GetMarginCurrency() { return m_MarginCurrency; }
	CString &GetMarginAmount() { return m_MarginAmount; }
	CString &GetFunded() { return m_Funded; }
	CString &GetBinary() { return m_Binary; }
	CString &GetAAMethod() { return m_AAMethod; }
	CString &GetAAFReason() { return m_AAFReason; }
	CString &GetBestExecution() { return m_BestExecution; }
	CString &GetShortSale() { return m_ShortSale; }
	CString &GetCSPBShort() { return m_CSPBShort; }
	CString &GetPrice() { return m_Price; }
	CString &GetDownPymnt() { return m_DownPymnt; }
	CString &GetBrokerFee() { return m_BrokerFee; }
	CString &GetOtherFee() { return m_OtherFee; }
	CString &GetSoftDollar() { return m_SoftDollar; }
	CString &GetAssignCP() { return m_AssignCP; }
	CString &GetAssignCT() { return m_AssignCT; }
	CString &GetDV01() { return m_DV01; }
	CString &GetVAR() { return m_VAR; }
	CString &GetUnWindTicket() { return m_UnWindTicket; }
	CString &GetCancelTicket() { return m_CancelTicket; }
	CString &GetCorrectTicket() { return m_CorrectTicket; }
	CString &GetSecFee() { return m_SecFee; }
	CString &GetOrFee() { return m_OrFee; }
	CString &GetETrade() {return m_ETrade; }
	CString &GetImagineExecID() { return m_ImagineExecID; }
	CString &GetCPTradeID() { return m_CPTradeID; }
	CString &GetOrderID() { return m_OrderID; }
	CString &GetSign() { return m_Sign; }

	void SetAvailAmount(double AvailAmount) { m_AvailAmount = AvailAmount; }
	void SetAvailAmount(LPCTSTR AvailAmount);

	void SetTicket(LPCTSTR Ticket) { m_Ticket = Ticket; }
	void SetTrader(LPCTSTR Trader) { m_Trader = Trader; }
	void SetDefTrader(LPCTSTR DefTrader) { m_DefTrader = DefTrader; }
	void SetExecTime(LPCTSTR ExecTime) { m_ExecTime = ExecTime; }
	void SetTradeDate(LPCTSTR TradeDate) { m_TradeDate = TradeDate; }
	void SetValueDate(LPCTSTR ValueDate) { m_ValueDate = ValueDate; }
	void SetDir(LPCTSTR Dir) { m_Dir = Dir; }
	void SetTransType(LPCTSTR TransType) { m_TransType = TransType; }
	void SetNomAmount(LPCTSTR NomAmount) { m_NomAmount = NomAmount; SetAvailAmount(NomAmount); }
	void SetOriNominal(LPCTSTR OriNominal) { m_OriNominal = OriNominal; }
	void SetStrike(LPCTSTR Strike) { m_Strike = Strike; }
	void SetOptExp(LPCTSTR OptExp) { m_OptExp = OptExp; }
	void SetFXDate(LPCTSTR FXDate) { m_FXDate = FXDate; }
	void SetOptTicker(LPCTSTR OptTicker) { m_OptTicker = OptTicker; }
	void SetOptID(LPCTSTR OptID) { m_OptID = OptID; }
	void SetListed(LPCTSTR Listed) { m_Listed = Listed; }
	void SetEuroOpt(LPCTSTR EuroOpt) { m_EuroOpt = EuroOpt; }
	void SetOptAuto(LPCTSTR OptAuto) { m_OptAuto = OptAuto; }
	void SetCurrency(LPCTSTR Currency) { m_Currency = Currency; }
	void SetFxRate(LPCTSTR FxRate) { m_FxRate = FxRate; }
	void SetCP(LPCTSTR CP) { m_CP = CP; }
	void SetBroker(LPCTSTR Broker) { m_Broker = Broker; }
	void SetAsset(LPCTSTR Asset) { m_Asset = Asset; }
	void SetAssetDesc(LPCTSTR AssetDesc) { m_AssetDesc = AssetDesc; }
	void SetAssetCommon(LPCTSTR AssetCommon) { m_AssetCommon = AssetCommon; }
	void SetAssetCusip(LPCTSTR AssetCusip) { m_AssetCusip = AssetCusip; }
	void SetAssetIsin(LPCTSTR AssetIsin) { m_AssetIsin = AssetIsin; }
	void SetAssetSedol(LPCTSTR AssetSedol) { m_AssetSedol = AssetSedol; }
	void SetAssetCountry(LPCTSTR AssetCountry) { m_AssetCountry = AssetCountry; }
	void SetAssetCurr(LPCTSTR AssetCurr) { m_AssetCurr = AssetCurr; }
	void SetAssetClass(LPCTSTR AssetClass) { m_AssetClass = AssetClass; }
	void SetAssetCategory(LPCTSTR AssetCategory) { m_AssetCategory = AssetCategory; }
	void SetAssetBucket(LPCTSTR AssetBucket) { m_AssetBucket = AssetBucket; }
	void SetAssetIndustry(LPCTSTR AssetIndustry) { m_AssetIndustry = AssetIndustry; }
	void SetAssetCoupon(LPCTSTR AssetCoupon) { m_AssetCoupon = AssetCoupon; }
	void SetAssetMaturity(LPCTSTR AssetMaturity) { m_AssetMaturity = AssetMaturity; }
	void SetAssetSource(LPCTSTR AssetSource) { m_AssetSource = AssetSource; }
	void SetAssetImagineCode(LPCTSTR AssetImagineCode) { m_AssetImagineCode = AssetImagineCode; }
	void SetIPO(LPCTSTR IPO) { m_IPO = IPO; }
	void SetTRS(LPCTSTR TRS) { m_TRS = TRS; }
	void SetRepoCP(LPCTSTR RepoCP) { m_RepoCP = RepoCP; }
	void SetRate(LPCTSTR Rate) { m_Rate = Rate; }
	void SetRateBasis(LPCTSTR RateBasis) { m_RateBasis = RateBasis; }
	void SetMaturity(LPCTSTR Maturity) { m_Maturity = Maturity; }
	void SetNote(LPCTSTR Note) { m_Note = Note; }
	void SetNote2(LPCTSTR Note2) { m_Note2 = Note2; }
	void SetSWBooking(LPCTSTR SWBooking) { m_SWBooking = SWBooking; }
	void SetSWMaturity(LPCTSTR SWMaturity) { m_SWMaturity = SWMaturity; }
	void SetSWFloatRate(LPCTSTR FloatRate) { m_SWFloatRate = FloatRate; }
	void SetSWFloatBasis(LPCTSTR FloatBasis) { m_SWFloatBasis = FloatBasis; }
	void SetSWFloatMaturity(LPCTSTR FloatMaturity) { m_SWFloatMaturity = FloatMaturity; }
	void SetWI(LPCTSTR WI) { m_WI = WI; }
	void SetOptSetCode(LPCTSTR OptSetCode) { m_OptSetCode = OptSetCode; }
	void SetOptSetCode2(LPCTSTR OptSetCode2) { m_OptSetCode2 = OptSetCode2; }
	void SetSetConvention(LPCTSTR SetConvention) { m_SetConvention = SetConvention; }
	void SetSwapTicket(LPCTSTR SwapTicket) { m_SwapTicket = SwapTicket; }
	void SetDeliveryDate(LPCTSTR DeliveryDate) { m_DeliveryDate = DeliveryDate; }
	void SetMargin(LPCTSTR Margin) { m_Margin = Margin; }
	void SetMarginCurrency(LPCTSTR MarginCurrency) { m_MarginCurrency = MarginCurrency; }
	void SetMarginAmount(LPCTSTR MarginAmount) { m_MarginAmount = MarginAmount; }
	void SetFunded(LPCTSTR Funded) { m_Funded = Funded; }
	void SetBinary(LPCTSTR Binary) { m_Binary = Binary; }
	void SetAAMethod(LPCTSTR AAMethod) { m_AAMethod = AAMethod; }
	void SetAAFReason(LPCTSTR AAFReason) { m_AAFReason = AAFReason; }
	void SetBestExecution(LPCTSTR BestExecution) { m_BestExecution = BestExecution; }
	void SetShortSale(LPCTSTR ShortSale) { m_ShortSale = ShortSale; }
	void SetCSPBShort(LPCTSTR CSPBShort) { m_CSPBShort = CSPBShort; }
	void SetPrice(LPCTSTR Price) { m_Price = Price; }
	void SetDownPymnt(LPCTSTR DownPymnt) { m_DownPymnt = DownPymnt; }
	void SetBrokerFee(LPCTSTR BrokerFee) { m_BrokerFee = BrokerFee; }
	void SetOtherFee(LPCTSTR OtherFee) { m_OtherFee = OtherFee; }
	void SetSoftDollar(LPCTSTR SoftDollar) { m_SoftDollar = SoftDollar; }
	void SetAssignCP(LPCTSTR AssignCP) { m_AssignCP = AssignCP; }
	void SetAssignCT(LPCTSTR AssignCT) { m_AssignCT = AssignCT; }
	void SetDV01(LPCTSTR DV01) { m_DV01 = DV01; }
	void SetVAR(LPCTSTR VAR) { m_VAR = VAR; }
	void SetUnWindTicket(LPCTSTR UnWindTicket) { m_UnWindTicket = UnWindTicket; }
	void SetCancelTicket(LPCTSTR CancelTicket) { m_CancelTicket = CancelTicket; }
	void SetCorrectTicket(LPCTSTR CorrectTicket) { m_CorrectTicket = CorrectTicket; }
	void SetSecFee(LPCTSTR SecFee) { m_SecFee = SecFee; }
	void SetOrFee(LPCTSTR OrFee) { m_OrFee = OrFee; }
	void SetETrade(LPCTSTR ETrade) { m_ETrade = ETrade; }
	void SetImagineExecID(LPCTSTR ImagineExecID) { m_ImagineExecID = ImagineExecID; }
	void SetCPTradeID(LPCTSTR CPTradeID) { m_CPTradeID = CPTradeID; }
	void SetOrderID(LPCTSTR OrderID) { m_OrderID = OrderID; }
	void SetSign(LPCTSTR Sign) { m_Sign = Sign; }

	void FromAssetRec(CAssetRec &AssetRec);
	void ToAssetRec(CAssetRec &AssetRec);
	virtual void Copy(CRawTicketRec &RawTicketRec);
	const CRawTicketRec &operator = (CRawTicketRec &RawTicketRec) { Copy(RawTicketRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = FALSE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE);
	virtual BOOL DeleteRawRecOnly(COraLoader &OraLoader, BOOL bByID = FALSE);
	
	CString &GetNetPrice() { return m_NetPrice; }
	void SetNetPrice(LPCTSTR NetPrice) { m_NetPrice = NetPrice; }

	BOOL SignTicket(COraLoader &OraLoader, BOOL bSign);
	BOOL UpdateShortRec(COraLoader &OraLoader, BOOL bFloat = FALSE);

	BOOL IsTicketProcessed(COraLoader &OraLoader);
	const CString &Alloc(CString &Text, const CString Amt, const CString AllocatedNom);
};
#endif
