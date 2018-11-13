#include "stdafx.h"
#include "NWIFMS70.h"
#include <math.h>
#include "allocdata.h"
#include "qdata.h"
#include "nwivar.h"


IMPLEMENT_DYNAMIC(CAllocData, CRowCtrl)
void CAllocData::Setup(COraLoader &OraLoader, CRawTicketRec &RawTicket, LPCTSTR Formula)
{
	SetOraLoader(OraLoader);

	GetTicket() = RawTicket;
	SetAutoRepo(!GetTicket().GetRepoCP().IsEmpty() && !GetTicket().GetRepoType().IsEmpty());
	
	m_bLocal = TRUE;
	if(!GetTicket().GetFxRate().IsEmpty() && GetTicket().GetFxRate() != "1")
		m_bLocal = FALSE;

	m_Formula = Formula;
}

void CAllocData::SetupAssetInfo()
{
	double Fxrate, LevRate;
	CString Date;
	CQData QData;
	
	Fxrate = atof(QData.RemoveComma(GetTicket().GetFxRate()));
	LevRate = atof(QData.RemoveComma(GetTicket().GetRate()));
	SetPrice(atof(QData.RemoveComma(GetTicket().GetPrice())));

	Date = GetTicket().GetMaturity().IsEmpty() ? GetTicket().GetValueDate() : GetTicket().GetMaturity();
	if(LevRate <= 0 && GetAutoRepo())
		LevRate = 1;

	m_Val.Setup(GetOraLoader(), GetTicket().GetTransType(), GetTicket().GetDir(), GetTicket().GetAsset(), 
				GetTicket().GetValueDate(), Date, GetTicket().GetAvailAmount(), GetPrice(), Fxrate, 
				GetTicket().GetRateBasis(), LevRate, GetFormula());
}

BOOL CAllocData::LoadIDs(CString &Common, CString &Cusip, CString &Sedol, CString &Isin)
{
	if(GetTicket().GetAsset().IsEmpty())
		return FALSE;
	else
	{
		CQData QData;
		OValue Value;

		GetOraLoader().GetSql().Format("SELECT ASS_COMMON_CODE, ASS_MSTC_CODE, ASS_SEDOL_NUM, "
										"ASS_ISIN_CODE, NVL(ASS_PAR_VALUE, 1) FROM SEMAM.NW_ASSETS WHERE ASS_CODE = %s", 
										QData.GetQueryText(GetTicket().GetAsset()));
		GetOraLoader().Open();
		while(!GetOraLoader().IsEOF())
		{
			Common = GetOraLoader().GetDBString(0);
			Cusip = GetOraLoader().GetDBString(1);
			Sedol = GetOraLoader().GetDBString(2);
			Isin = GetOraLoader().GetDBString(3);
			GetOraLoader().GetFieldValue(4, &Value);
			m_Par = (double) Value;
			GetOraLoader().MoveNext();
		}
		return TRUE;
	}
}

BOOL CAllocData::GetSelRawInv(int Index, CRawInvRec &RecInvRec)
{
	if(Index < 1 || Index > GetSS()->GetSheetRows())
		return FALSE;

	CString Text;
	for(int i = 1; i <= GetSS()->GetSheetCols(); i++)
	{
		Text = GetSS()->GetSheetText(i, Index);
		switch(i)
		{
			case 2:
				RecInvRec.SetPortfolio(Text);
				break;
			case 3:
				RecInvRec.SetNomAmount(Text);
				break;
			case 4:
				RecInvRec.SetCustodian(Text);
				break;
			case 5:
				RecInvRec.SetAccount(Text);
				break;
			case 6:
				RecInvRec.SetPrice(Text);
				break;
			case 7:
				RecInvRec.SetLink(Text);
				break;			
			case 8:
				if(Text.IsEmpty())
					RecInvRec.SetDownPymnt(GetTicket().GetDownPymnt());
				else
					RecInvRec.SetDownPymnt(Text);
				break;
			case 9:
				RecInvRec.SetOtherFee(Text);
				break;
/*			case 4:
				RecInvRec.SetPrice(Text);
				break;
			case 5:
				RecInvRec.SetDownPymnt(Text);
				break;
			case 6:
				RecInvRec.SetBrokerFee(Text);
				break;
			case 7:
				RecInvRec.SetSoftDollar(Text);
				break;
			case 8:
				RecInvRec.SetOtherFee(Text);
				break;
			case 9:
				RecInvRec.SetVAR(Text);
				break;
			case 10:
				RecInvRec.SetDV01(Text);
				break;
			case 11:
				RecInvRec.SetLink(Text);
				break;
			case 12:
				RecInvRec.SetNetPrice(Text);
				break; */
			default:
				break;
		}
	}
	
	return TRUE;
}

double CAllocData::RepoPrice(double DownPay, double Price)
{
	double Fxrate;

	Fxrate = GetLocal() ? 1 : m_Val.GetFxRate();
	if(DownPay >= 0)
	{
		m_Val.SetNomAmount(1);
		if(GetAutoRepo())
		{
			if(DownPay == 0)
				DownPay = 100;
			DownPay *= 0.01;
			if(strcmp(GetTicket().GetRepoType(), "REPO") == 0)
			{
				m_Val.SetPrice(Price);
				Price = DownPay*(m_Val.GetValue() + m_Val.GetPrePaidInt() + 
								m_Val.GetPrePaidAccretionValue())/Fxrate/
								m_Val.GetBondFact()/m_Val.GetAmortFact();
			}
			else
				Price = DownPay*(m_Val.GetValue() + m_Val.GetPrePaidInt() + 
								m_Val.GetPrePaidAccretionValue()/Fxrate)/1000000;
		}
	}

	return Price;
}

void CAllocData::ComputeValue(double OtherFee)
{
	double Fxrate, DownPay, Contracts, Amount;
	CString Buf, PB;
	CQData QData;

	SetBrFee(atof(QData.RemoveComma(GetTicket().Alloc(Buf, GetTicket().GetBrokerFee(), GetInv().GetNomAmount()))));
	SetSoftDollar(atof(QData.RemoveComma(GetTicket().Alloc(Buf, GetTicket().GetSoftDollar(), GetInv().GetNomAmount()))));
	SetVAR(atof(QData.RemoveComma(GetTicket().Alloc(Buf, GetTicket().GetVAR(), GetInv().GetNomAmount()))));
	DownPay = atof(QData.RemoveComma(GetTicket().GetDownPymnt()));

	m_Val.SetNomAmount(atof(QData.RemoveComma(GetInv().GetNomAmount())));
	m_Val.SetPrice(GetPrice());
	Amount = m_Val.ComputeLevAmount(TRUE);

	OtherFee += m_Val.GetSecFees(GetPrice(), GetTicket().GetSecFee() == Y);
	if(GetTicket().GetOrFee() == Y)
	{
		if(m_Par > 0)
			Contracts = m_Val.GetNomAmount()/m_Par;
		else
			Contracts = m_Val.GetNomAmount();
		OtherFee += m_Val.GetOrFees(m_PB, GetTicket().GetCP(), Contracts);
	}
	
	SetOtherFee(OtherFee);

	if(GetOtherFee() > 0 || GetBrFee() > 0)
	{
		if(m_Val.GetDir() == "P")
			Amount += (GetOtherFee() + GetBrFee());
		else
			Amount -= (GetOtherFee() + GetBrFee());
		SetNetPrice(Amount/m_Val.GetNomAmount()/m_Val.GetAmortFact()/m_Val.GetBondFact());
	}
	else
		SetNetPrice(GetPrice());

	Fxrate = GetLocal() ? 1 : m_Val.GetFxRate();
	if(strcmp(m_Val.GetType(), REPO) == 0 || strcmp(m_Val.GetType(), LEVERAGE) == 0 || 
		(GetAutoRepo() && DownPay == 100))
	{
		SetNetPrice(GetNetPrice());
		m_Val.SetPrice(GetNetPrice());
		m_Val.SetNomAmount(atof(QData.RemoveComma(GetInv().GetNomAmount())));
		m_Val.ComputeLevAmount(TRUE);
		if(strcmp(m_Val.GetType(), LEVERAGE) == 0 || strcmp(GetTicket().GetRepoType(), LEVERAGE) == 0)
			SetCash(m_Val.GetNomAmount()/Fxrate);
		else
			SetCash(m_Val.GetLevAmount()/Fxrate);

		SetAccretion(0);
		if(strlen(GetTicket().GetMaturity()) == 0)
			SetPrePaid(0);
		else
		{
			if(strcmp(GetTicket().GetRepoType(), LEVERAGE) == 0)
				SetPrePaid(0);
			else
				SetPrePaid(m_Val.GetLevInterest()/Fxrate);
		}
	}
	else
	{
		m_Val.SetPrice(GetNetPrice());
		SetCash(m_Val.GetValue()/Fxrate);
		SetPrePaid(m_Val.GetPrePaidInt()/Fxrate);
		SetAccretion(m_Val.GetPrePaidAccretionValue()/Fxrate);
	}
}

void CAllocData::GenerateRepo()
{
	int Size;
	CDBRec Rec;
	double DownPay, LevPrice, NomAmount, Fxrate;
	CQData QData;
	CRawInvRec Inv;

	SetAutoRepo(TRUE);
	Fxrate = atof(QData.RemoveComma(GetTicket().GetFxRate()));
	DownPay = atof(QData.RemoveComma(GetTicket().GetDownPymnt()));
	m_NetPrice = atof(QData.RemoveComma(GetInv().GetPrice()));
	Size = GetSS()->GetSheetRows();
	for(int i = 1; i <= Size; i ++)
	{
		GetSelRawInv(i, Inv);
		if(DownPay == 100)
		{
			MessageBox(NULL, "In order to do auto repo, Down payment cannot be 100%", NULL, MB_OK);
			return;
		}

		if(DownPay >= 0 && DownPay < 100)
		{
			if(strcmp(GetTicket().GetRepoType(), REPO) == 0)
			{
				m_Val.SetNomAmount(1);
				LevPrice = RepoPrice(DownPay, m_NetPrice);
			}
			else
			{
				m_Val.SetNomAmount(atof(QData.RemoveComma(Inv.GetNomAmount())));
				LevPrice = m_Val.GetNomAmount()/1000000;
				NomAmount = 0.01*(100 - DownPay)*(m_Val.GetValue() + m_Val.GetPrePaidInt() +
													m_Val.GetPrePaidAccretionValue());
				if(Fxrate > 0)
					NomAmount /= Fxrate;
			}
		}

		Rec.RemoveAll();
		Rec.Add(GetTicket().GetTicket()); // TicketNum
		Rec.Add(Inv.GetPortfolio()); // Portfolio
		if(GetTicket().GetRepoType() == REPO)
			Rec.Add(Inv.GetNomAmount());
		else
			Rec.Add(QData.WriteNumber(NomAmount, TRUE, 2));

		Rec.Add(Inv.GetCustodian()); // Custodian
		Rec.Add(Inv.GetAccount()); // Account;
		Rec.Add(QData.WriteNumber(LevPrice));// Price
		Rec.Add(EMPTYSTRING); // OptLink
		Rec.Add("100"); // DownPay
		AddSheetRow(Rec);
	}
}

int CAllocData::AllocCheckSum(LPCTSTR Portfolio, double CurrentAmount)
{
	double NomAmount;
	double Multiple;
	CQData QData;

	Multiple = GetAutoRepo() ? 2 : 1;
	NomAmount = ComputeAllocAmount(Portfolio, CurrentAmount);

	if(fabs(NomAmount - GetTicket().GetAvailAmount()*Multiple) <= 0.00001)
		return 0;

	if(NomAmount - GetTicket().GetAvailAmount()*Multiple > -0.00001)
		return -1;

	return 1;
}

double CAllocData::ComputeAllocAmount(LPCTSTR Portfolio, double CurrentAmount)
{
	CString Port;
	CQData QData;
	double AllocAmount;
	CRawInvRec Inv;

	if(Portfolio && strlen(Portfolio) > 0)
		Port = Portfolio;
	else
		Port.Empty();
	AllocAmount = 0;
	for (int i = 1; i <= GetSS()->GetSheetRows(); i++)
	{				
		GetSelRawInv(i, Inv);
		if(Port.IsEmpty())
		{
			if(GetAutoRepo() && Inv.GetDownPymnt() == "100" && GetTicket().GetRepoType() == LEVERAGE)
				AllocAmount = AllocAmount + atof(QData.RemoveComma(GetSS()->GetSheetText(6, i)))*1000000;
			else
				AllocAmount = AllocAmount + atof(QData.RemoveComma(Inv.GetNomAmount()));
		}
		else
			if(Port != Inv.GetPortfolio()) // Add everything except identical portfolio
			{
				if(GetAutoRepo() && Inv.GetDownPymnt() == "100" && GetTicket().GetRepoType() == LEVERAGE)
					AllocAmount = AllocAmount + atof(QData.RemoveComma(GetSS()->GetSheetText(6, i)))*1000000;
				else
					AllocAmount = AllocAmount + atof(QData.RemoveComma(Inv.GetNomAmount()));
			}
	}

	if(!Port.IsEmpty())
		AllocAmount += CurrentAmount; // Add passed nom_amount

	return AllocAmount;
}

void CAllocData::LoadAlloc(CRowCtrl &m_Data, const CString DownPayment)
{
	CString Price, Table = "SEMAM.RAW_TICKET_V A ";
	CQData QData;

	Price = QData.RemoveComma(GetTicket().GetPrice());
	if(!GetTicket().GetUnWindTicket().IsEmpty())
	{
		GetOraLoader().GetSql() = "SELECT A.TICKET_NUM, A.PORTFOLIO, A.NOM_AMOUNT, A.CUSTODIAN, A.ACCOUNT, " 
									+ Price + " \"PRICE\", B.TRANS_NUM \"OPT_BACK\", "	+ DownPayment + 
									"\"DOWN_PYMNT\", DECODE(SIGN(OTHER_FEES), 1, OTHER_FEES, OTHER_FEE*NVL(ALLOC, 100)/100) \"OTHER_FEES\" "
									"FROM " + Table + ", SEMAM.NW_TR_TICKETS B, SEMAM.NW_AA_ALLOC C "
									"WHERE B.PORTFOLIO(+) = A.PORTFOLIO "
									"AND B.TICKET_NUM(+) = '" + GetTicket().GetUnWindTicket() + "' "
									"AND C.AA_METHOD(+) = A.AA_METHOD "
									"AND C.PORTFOLIO(+) = A.PORTFOLIO "
									"AND C.FROM_DATE(+) <= A.TRADE_DATE "
									"AND NVL(C.TO_DATE(+), SYSDATE + 100) > A.TRADE_DATE "
									"AND A.PROCESSED IS NULL "
									"AND A.TICKET_NUM = '" + GetTicket().GetTicket() + "' ";
	}
	else
	{
		if(GetTicket().GetTransType() == "INT. SWAP")
		{
			GetOraLoader().GetSql() = "SELECT A.TICKET_NUM, A.PORTFOLIO, A.NOM_AMOUNT, A.CUSTODIAN, "
										"A.ACCOUNT, " + Price + " \"PRICE\", TO_NUMBER(NULL) \"OPT_BACK\", " 
										+ DownPayment + "\"DOWN_PYMNT\", "
										"DECODE(SIGN(OTHER_FEES), 1, OTHER_FEES, OTHER_FEE*NVL(ALLOC, 100)/100) \"OTHER_FEES\" "
										"FROM " + Table + ", SEMAM.NW_AA_ALLOC B "
										"WHERE B.AA_METHOD(+) = A.AA_METHOD "
										"AND B.PORTFOLIO(+) = A.PORTFOLIO "
										"AND B.FROM_DATE(+) <= A.TRADE_DATE "
										"AND NVL(B.TO_DATE(+), SYSDATE + 100) > A.TRADE_DATE "
										"AND A.TICKET_NUM = '" + GetTicket().GetTicket() + "' ";
		}
		else
		{
			GetOraLoader().GetSql() = "SELECT A.TICKET_NUM, A.PORTFOLIO, A.NOM_AMOUNT, A.CUSTODIAN, "
										"A.ACCOUNT, " + Price + " \"PRICE\", TO_NUMBER(NULL) \"OPT_BACK\", " 
										+ DownPayment + "\"DOWN_PYMNT\", "
										"DECODE(SIGN(OTHER_FEES), 1, OTHER_FEES, OTHER_FEE*NVL(ALLOC, 100)/100) \"OTHER_FEES\" "
										"FROM " + Table + ", SEMAM.NW_AA_ALLOC B "
										"WHERE B.AA_METHOD(+) = A.AA_METHOD "
										"AND B.PORTFOLIO(+) = A.PORTFOLIO "
										"AND B.FROM_DATE(+) <= A.TRADE_DATE "
										"AND NVL(B.TO_DATE(+), SYSDATE + 100) > A.TRADE_DATE "
										"AND A.PROCESSED IS NULL "
										"AND A.TICKET_NUM = '" + GetTicket().GetTicket() + "' ";
		}
	}

	GetOraLoader().GetSql() += "AND A.SIGN IS NOT NULL ";
	GetOraLoader().GetSql() += "ORDER BY 1, 2 ";
	GetOraLoader().Open();
	GetOraLoader().GetFieldArray().GetAt(0)->SetWithComma(FALSE); // TicketNum
	GetOraLoader().GetFieldArray().GetAt(6)->SetWithComma(FALSE); // TransNum
	GetOraLoader().LoadRowCtrl(m_Data, TRUE, TRUE);
}