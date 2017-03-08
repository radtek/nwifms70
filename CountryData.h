#ifndef __COUNTRYDATA_H__
#define __COUNTRYDATA_H__

#ifndef __RECOBJ_H__
#include "recobj.h"
#endif

class CCountryRec : public CRecObj
{
DECLARE_DYNAMIC(CCountryRec)
protected:
	CString m_Country;
	CString m_Code;
	CString m_Code2;
	CString m_OldCountry;
	CString m_Limit;
	CString m_GeoArea;
	CString m_PFRegion;
	CString m_ProfileRegion;
	CString m_Pru;
	CString m_Currency;
	CString m_NewCurrency;
	CString	m_Conversion;
	CString m_ConvDate;
	CString m_SovFlag;
	CString m_SecFlag;
	CString m_DMFlag;

public:
	CCountryRec() { }
	CCountryRec(CCountryRec &CountryRec) { Copy(CountryRec); }
	~CCountryRec() {}

	CString &GetCountry() { return m_Country; }
	CString &GetCode() { return m_Code; }
	CString &GetCode2() { return m_Code2; }
    CString &GetOldCountry() { return m_OldCountry; }
	CString &GetLimit() { return m_Limit; }
	CString &GetGeoArea() { return m_GeoArea; }
	CString &GetPFRegion() { return m_PFRegion; }
	CString &GetProfileRegion() { return m_ProfileRegion; }
	CString &GetPru() { return m_Pru; }
	CString &GetCurrency() { return m_Currency; }
	CString &GetNewCurrency() { return m_NewCurrency; }
	CString &GetConversion() { return m_Conversion; }
	CString &GetConvDate() { return m_ConvDate; }
	CString &GetSovFlag() { return m_SovFlag; }
	CString &GetSecFlag() { return m_SecFlag; }
	CString &GetDMFlag() { return m_DMFlag; }

	void SetCountry(LPCTSTR Country) { m_Country = Country; }
	void SetCode(LPCTSTR Code) { m_Code = Code; }
	void SetCode2(LPCTSTR Code2) { m_Code2 = Code2; }
	void SetOldCountry(LPCTSTR OldCountry) { m_OldCountry = OldCountry; }
	void SetLimit(LPCTSTR Limit) { m_Limit = Limit; }
	void SetGeoArea(LPCTSTR GeoArea) { m_GeoArea = GeoArea; }
	void SetPFRegion(LPCTSTR PFRegion) { m_PFRegion = PFRegion; }
	void SetProfileRegion(LPCTSTR ProfileRegion) { m_ProfileRegion = ProfileRegion; }
	void SetPru(LPCTSTR Pru) { m_Pru = Pru; }
	void SetCurrency(LPCTSTR Currency) { m_Currency = Currency; }
	void SetNewCurrency(LPCTSTR NewCurrency) { m_NewCurrency = NewCurrency; }
	void SetConversion(LPCTSTR Conversion) { m_Conversion = Conversion; }
	void SetConvDate(LPCTSTR ConvDate) { m_ConvDate = ConvDate; }
	void SetSovFlag(LPCTSTR SovFlag) { m_SovFlag = SovFlag; }
	void SetSecFlag(LPCTSTR SecFlag) { m_SecFlag = SecFlag; }
	void SetDMFlag(LPCTSTR DMFlag) { m_DMFlag = DMFlag; }

	virtual void Copy(CCountryRec &CountryRec);
	const CCountryRec &operator = (CCountryRec &CountryRec) { Copy(CountryRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = FALSE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE);
};


class CCountryData : public CRowCtrl
{
DECLARE_DYNAMIC(CCountryData)
protected:
	CCountryRec m_Rec;
	COraLoader m_OraLoader;

public:
	CCountryData() { }
	CCountryData(COraLoader &OraLoader, CDBSheet *pSS) : CRowCtrl(pSS) { SetOraLoader(OraLoader); }
	CCountryData(CCountryData &Data) : CRowCtrl(Data) { SetOraLoader(Data.GetOraLoader()); }
	~CCountryData() { }

	CCountryRec &GetRec() { return m_Rec; }

	COraLoader &GetOraLoader() { return m_OraLoader; }
	void SetOraLoader(COraLoader &OraLoader) { m_OraLoader = OraLoader; }

	BOOL LoadDBData();
	virtual BOOL IsReady();
	virtual BOOL AddRow();
	virtual BOOL DeleteRow();
	virtual BOOL UpdateRow();
};

#endif