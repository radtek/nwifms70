#ifndef __ASSETSUPPLEMENTDATA_H__
#define __ASSETSUPPLEMENTDATA_H__

#ifndef __RECOBJ_H__
#include "recobj.h"
#endif

class CAssetSupplementRec : public CRecObj
{
DECLARE_DYNAMIC(CAssetSupplementRec)
protected:
	CString m_Asset;
	CString m_Code;
	CString m_Desc;
	CString m_Common;
	CString m_Isin;
	CString m_AmountOf;
	CString m_PerAmount;
	CString	m_RoundingFactor;

	CString m_User;

public:
	CAssetSupplementRec() { }
	CAssetSupplementRec(LPCTSTR User) { SetUser(User); }
	CAssetSupplementRec(CAssetSupplementRec &AssetSupplementRec) { Copy(AssetSupplementRec); }
	~CAssetSupplementRec() {}

	CString &GetAsset() { return m_Asset; }
	CString &GetCode() { return m_Code; }
    CString &GetDesc() { return m_Desc; }
	CString &GetCommon() { return m_Common; }
	CString &GetIsin() { return m_Isin; }
	CString &GetAmountOf() { return m_AmountOf; }
	CString &GetPerAmount() { return m_PerAmount; }
	CString &GetRoundingFactor() { return m_RoundingFactor; }

	CString &GetUser() { return m_User; }

	void SetAsset(LPCTSTR Asset) { m_Asset = Asset; }
	void SetCode(LPCTSTR Code) { m_Code = Code; }
	void SetDesc(LPCTSTR Desc) { m_Desc = Desc; }
	void SetCommon(LPCTSTR Common) { m_Common = Common; }
	void SetIsin(LPCTSTR Isin) { m_Isin = Isin; }
	void SetAmountOf(LPCTSTR AmountOf) { m_AmountOf = AmountOf; }
	void SetPerAmount(LPCTSTR PerAmount) { m_PerAmount = PerAmount; }
	void SetRoundingFactor(LPCTSTR RoundingFactor) { m_RoundingFactor = RoundingFactor; }

	void SetUser(LPCTSTR User) { m_User = User; }

	virtual void Copy(CAssetSupplementRec &AssetSupplementRec);
	const CAssetSupplementRec &operator = (CAssetSupplementRec &AssetSupplementRec) 
			{ Copy(AssetSupplementRec); return *this; }
	virtual void ToDBRec(CDBRec &Rec);
	virtual BOOL AddRec(COraLoader &OraLoader);
	virtual BOOL UpdateRec(COraLoader &OraLoader, BOOL bByID = FALSE);
	virtual BOOL DeleteRec(COraLoader &OraLoader, BOOL bByID = FALSE);
};


class CAssetSupplementData : public CRowCtrl
{
DECLARE_DYNAMIC(CAssetSupplementData)
protected:
	CAssetSupplementRec m_Rec;
	COraLoader m_OraLoader;

public:
	CAssetSupplementData() { }
	CAssetSupplementData(COraLoader &OraLoader, CDBSheet *pSS, LPCTSTR User) { Setup(OraLoader, pSS, User); }
	CAssetSupplementData(CAssetSupplementData &Data) : CRowCtrl(Data) 
				{ Setup(Data.GetOraLoader(), Data.GetSS(), Data.GetRec().GetUser()); }
	~CAssetSupplementData() { }

	void Setup(COraLoader &OraLoader, CDBSheet *pSS, LPCTSTR User) { SetOraLoader(OraLoader), SetSS(pSS); m_Rec.SetUser(User); }
	CAssetSupplementRec &GetRec() { return m_Rec; }

	COraLoader &GetOraLoader() { return m_OraLoader; }
	void SetOraLoader(COraLoader &OraLoader) { m_OraLoader = OraLoader; }

	BOOL LoadDBData();
	virtual BOOL IsReady();
	virtual BOOL AddRow();
	virtual BOOL DeleteRow();
	virtual BOOL UpdateRow();
};

#endif