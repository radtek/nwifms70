#ifndef __RISKREC_H__
#define __RISKREC_H__

#ifndef __DBRECARRARY_H__
#include "dbrecarray.h"
#endif

#ifndef __RECOBJ_H__
#include "recobj.h"
#endif

class CRiskRec : public CRecObj
{
DECLARE_DYNAMIC(CRiskRec)
protected:
	CString m_Portfolio;
	CString m_RiskType;
	CString m_AssetClass;
	double m_WLimit;
	double m_Limit;
	double m_Amount;

public:
	CRiskRec() {}
	CRiskRec(CRiskRec &RiskRec) { Copy(RiskRec); }

	CString &GetPortfolio() { return m_Portfolio; }
	CString &GetRiskType() { return m_RiskType; }
	CString &GetAssetClass() { return m_AssetClass; }
	double &GetWLimit() { return m_WLimit; }
	double &GetLimit() { return m_Limit; }
	double &GetAmount() { return m_Amount; }
	
	virtual void Copy(CRiskRec &RiskRec);
	const CRiskRec &operator = (CRiskRec &RiskRec) { Copy(RiskRec); return *this; }
};


class CRiskRecArray : public CObArray
{
DECLARE_DYNAMIC(CRiskRecArray)
protected:
	CString m_Asset;

public:
	CRiskRecArray() { m_Asset.Empty(); }
	CRiskRecArray(CRiskRecArray &RiskRecArray) { Copy(RiskRecArray); }
	~CRiskRecArray() { RemoveAll(); }

	void Copy(CRiskRecArray &RiskRecArray);

	int	Add(CRiskRec *Rec) { return CObArray::Add(Rec); }
	void SetAt(int Index, CRiskRec *Rec);
	void InsertAt(int Index, CRiskRec *Rec) { CObArray::InsertAt(Index, Rec); }
	int Add(CRiskRec &Rec) { return CObArray::Add(new CRiskRec(Rec)); }
	void SetAt(int Index, CRiskRec &Rec);
	void InsertAt(int Index, CRiskRec &Rec) { CObArray::InsertAt(Index, new CRiskRec(Rec)); }
	void RemoveAt(int Index);
	void RemoveAll();
	
	CRiskRec *GetAt(int Index);

	void SetAsset(const CString Asset);
	CString &GetAsset() { return m_Asset; }

	const CRiskRecArray &operator = (CRiskRecArray &RiskRecArray) { Copy(RiskRecArray); return *this; }
};

class CRiskControl
{
protected:
	COraLoader m_OraLoader;
	CString m_PrevBDate;
	CString m_QPrevBDate;
	CString m_RiskType;
	CString m_ValueDate;
	CString m_Currency;
	CString m_Currency2;
	double m_NAV;
	double m_Exrate;
	double m_Exrate2;
	CRiskRecArray m_RiskArray;

protected:
	double GetFxrate(const CString Currency, double Fxrate = 0);
	void SetupCurrency(CString &Currency, double &Exrate, const CString SrcCurr, double Fxrate = 0.0);
	void AddIDPhrase(CString &Sql, const CString ID, const CString sID, BOOL bByTicket = TRUE);
	BOOL AddRisk(const CString sID, double Amount, BOOL bByTicket = TRUE, BOOL bFirstLeg = TRUE);

public:
	CRiskControl() { }
	CRiskControl(COraLoader &OraLoader) { Setup(OraLoader); }
	~CRiskControl() {}

	void Setup(COraLoader &OraLoader);

	COraLoader &GetOraLoader() { return m_OraLoader; }
	CString &GetAsset() { return m_RiskArray.GetAsset(); }
	CString &GetPrevBDate() { return m_PrevBDate; }
	CString &GetQPrevBDate() { return m_QPrevBDate; }
	CString &GetRiskType() { return m_RiskType; }
	CString &GetValueDate() { return m_ValueDate; }
	CString &GetCurrency() { return m_Currency; }
	void SetCurrency(const CString Currency, double Fxrate = 0.0) { SetupCurrency(m_Currency, m_Exrate, Currency, Fxrate); }
	CString &GetCurrency2() { return m_Currency2; }
	void SetCurrency2(const CString Currency) { SetupCurrency(m_Currency2, m_Exrate2, Currency); }

	double GetExrate() { return m_Exrate; }
	double GetExrate2() { return m_Exrate2; }

	double &GetNAV() { return m_NAV; }

	CRiskRecArray &GetRiskArray() { return m_RiskArray; }
	double ComputeAmount(double &Amount2, const CString TransType, const CString ValueDate, const CString Currency, double Nominal, double Price, double Fxrate = 0.0);
	BOOL UpdateRisk(const CString ID, BOOL bByTicket = TRUE);
	BOOL LoadRisk(const CString Asset, const CString ID, BOOL bByTicket = TRUE);
	BOOL DeleteRisk(const CString ID, BOOL bByTicket = TRUE);
	int VerifyRisk(CString &Text, const CString Fund, const CString Currency, double Nominal, double Amount = 0.0);
	int VerifyRisk(CString &Text, const CString Fund, const CString TransType, const CString ValueDate, const CString Currency, double Nominal, double Price, double Fxrate = 0.0);
	int VerifyRisk(CString &Text, const CString Fund, const CString TransType, const CString ValueDate, const CString Currency, 
					const CString Nominal, const CString Price, const CString Fxrate);
};
#endif 