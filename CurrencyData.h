#ifndef __CURRENCYDATA_H__
#define __CURRENCYDATA_H__

#ifndef __RECOBJ_H__
#include "recobj.h"
#endif

class CCurrencyRec : public CRecObj
{
DECLARE_DYNAMIC(CCurrencyRec)
protected:
	CString m_Currency;
	CString m_NewCurrency;
	CString m_Conversion;
	CString m_ConvDate;
	CString m_DBPBTier;
	CString m_HSBCTier;
	CString m_Inverted;
	CString m_NDFFix;
	CString m_1256;

public:
	CCurrencyRec() { }
	CCurrencyRec(CCurrencyRec &CurrencyRec) { Copy(CurrencyRec); }
	~CCurrencyRec() {}

	CString &GetCurrency() { return m_Currency; }
	CString &GetNewCurrency() { return m_NewCurrency; }
    CString &GetConversion() { return m_Conversion; }
	CString &GetConvDate() { return m_ConvDate; }
	CString &GetDBPBTier() { return m_DBPBTier; }
	CString &GetHSBCTier() { return m_HSBCTier; }
	CString &GetInverted() { return m_Inverted; }
	CString &GetNDFFix() { return m_NDFFix; }
	CString &Get1256() { return m_1256; }

	void SetCurrency(LPCTSTR Currency) { m_Currency = Currency; }
	void SetNewCurrency(LPCTSTR NewCurrency) { m_NewCurrency = NewCurrency; }
	void SetConversion(LPCTSTR Conversion) { m_Conversion = Conversion; }
	void SetConvDate(LPCTSTR ConvDate) { m_ConvDate = ConvDate; }
	void SetDBPBTier(LPCTSTR DBPBTier) { m_DBPBTier = DBPBTier; }
	void SetHSBCTier(LPCTSTR HSBCTier) { m_HSBCTier = HSBCTier; }
	void SetInverted(LPCTSTR Inverted) { m_Inverted = Inverted; }
	void SetNDFFix(LPCTSTR NDFFix) { m_NDFFix = NDFFix; }
	void Set1256(LPCTSTR Sec1256) { m_1256 = Sec1256; }

	virtual void Copy(CCurrencyRec &CurrencyRec);
	const CCurrencyRec &operator = (CCurrencyRec &CurrencyRec) { Copy(CurrencyRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = FALSE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE);
};


class CCurrencyData : public CRowCtrl
{
DECLARE_DYNAMIC(CCurrencyData)
protected:
	CCurrencyRec m_Rec;
	COraLoader m_OraLoader;

public:
	CCurrencyData() { }
	CCurrencyData(COraLoader &OraLoader, CDBSheet *pSS) : CRowCtrl(pSS) { SetOraLoader(OraLoader); }
	CCurrencyData(CCurrencyData &Data) : CRowCtrl(Data) { SetOraLoader(Data.GetOraLoader()); }
	~CCurrencyData() { }

	CCurrencyRec &GetRec() { return m_Rec; }

	COraLoader &GetOraLoader() { return m_OraLoader; }
	void SetOraLoader(COraLoader &OraLoader) { m_OraLoader = OraLoader; }

	BOOL LoadDBData();
	virtual BOOL IsReady();
	virtual BOOL AddRow();
	virtual BOOL DeleteRow();
	virtual BOOL UpdateRow();
};

#endif