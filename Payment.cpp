// Payment.cpp : implementation file
//

#include "stdafx.h"
#include "NWIFMS70.h"
#include "Payment.h"
#include "qdata.h"
#include "assetdlg.h"
#include "CouponListDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPayment

IMPLEMENT_DYNCREATE(CPayment, CFormView)

CPayment::CPayment()
	: CFormView(CPayment::IDD)
{
	//{{AFX_DATA_INIT(CPayment)
	//}}AFX_DATA_INIT
}

void CPayment::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPayment)
	DDX_Control(pDX, IDC_PAY_LIST, m_SS);
	DDX_Control(pDX, IDC_PAY_INSTRUCTION_LIST, m_InstSS);
	DDX_Control(pDX, IDC_PAY_OPENTRADE_LIST, m_OT_SS);
	DDX_Control(pDX, IDC_PAY_SEL_TRADE_LIST, m_Sel_SS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPayment, CFormView)
	//{{AFX_MSG_MAP(CPayment)
	ON_EN_CHANGE(IDC_PAY_AMOUNT_EDIT, OnChangePayAmountEdit)
	ON_COMMAND(ID_PAY_CLEARSCREEN, OnPayClearscreen)
	ON_COMMAND(ID_PAY_FIND, OnPayFind)
	ON_COMMAND(ID_PAY_LOAD_OPENTRADE, OnPayLoadOpentrade)
	ON_COMMAND(ID_PAY_ADD, OnPayAdd)
	ON_COMMAND(ID_PAY_UPDATE, OnPayUpdate)
	ON_UPDATE_COMMAND_UI(ID_PAY_UPDATE, OnUpdatePayUpdate)
	ON_COMMAND(ID_PAY_DELETE, OnPayDelete)
	ON_UPDATE_COMMAND_UI(ID_PAY_DELETE, OnUpdatePayDelete)
	ON_CBN_SELCHANGE(IDC_PAY_TYPE_COMBO, OnSelchangePayTypeCombo)
	ON_UPDATE_COMMAND_UI(ID_PAY_LOAD_OPENTRADE, OnUpdatePayLoadOpentrade)
	ON_COMMAND(ID_PAY_REPORT, OnPayReport)
	ON_UPDATE_COMMAND_UI(ID_PAY_REPORT, OnUpdatePayReport)
	ON_COMMAND(ID_PAY_CLEARALL, OnPayClearall)
	ON_COMMAND(ID_PAY_FIND_ASSET, OnPayFindAsset)
	ON_UPDATE_COMMAND_UI(ID_PAY_FIND_ASSET, OnUpdatePayFindAsset)
	ON_EN_SETFOCUS(IDC_PAY_ASSET_EDIT, OnSetfocusPayAssetEdit)
	ON_EN_KILLFOCUS(IDC_PAY_ASSET_EDIT, OnKillfocusPayAssetEdit)
	ON_CBN_SELCHANGE(IDC_PAY_PORTFOLIO_COMBO, OnSelchangePayPortfolioCombo)
	ON_UPDATE_COMMAND_UI(ID_PAY_LOAD_COUPON, OnUpdatePayLoadCoupon)
	ON_COMMAND(ID_PAY_LOAD_COUPON, OnPayLoadCoupon)
	ON_COMMAND(ID_PAY_LOAD_CDS_COUPON, OnPayLoadCdsCoupon)
	ON_UPDATE_COMMAND_UI(ID_PAY_LOAD_CDS_COUPON, OnUpdatePayLoadCdsCoupon)
	ON_CBN_SELCHANGE(IDC_PAY_CASHTYPE_COMBO, OnSelchangePayCashtypeCombo)
	ON_COMMAND(ID_PAY_LOAD_IRS_COUPON, &CPayment::OnPayLoadIrsCoupon)
	ON_UPDATE_COMMAND_UI(ID_PAY_LOAD_IRS_COUPON, &CPayment::OnUpdatePayLoadIrsCoupon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPayment diagnostics

#ifdef _DEBUG
void CPayment::AssertValid() const
{
	CFormView::AssertValid();
}

void CPayment::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CNWIFMS70Doc* CPayment::GetDocument() // non-debug version is inline
{
	return STATIC_DOWNCAST(CNWIFMS70Doc, m_pDocument);
}
#endif //_DEBUG

BOOL CPayment::FindInvNum(const char *InvNum)
{
	for(int i = 1; i <= m_Sel_SS.GetSheetRows(); i ++)
		if(strcmp(InvNum, m_Sel_SS.GetSheetText(2, i)) == 0)
			return TRUE;
	
	return FALSE;
}

BOOL CPayment::IsOK()
{
	CString Buf, Text;
	double Amt, Sum;

	Amt = atof(m_Amount.GetData());
	Sum = atof(m_Sum.GetData());

	Buf = m_Dir.GetData();
	if(Buf.IsEmpty())
		Text = "The instruction type must be filled";
	else
		if(Buf == "PAYMENT" && Amt < 0)
			Text = "The Amount indicated that RECEIPT should be used";
		else
			if(Buf == "RECEIPT" && Amt > 0)
				Text = "The Amount indicated that PAYMENT should be used";
			else
				if(Amt > 0 && Sum < 0 || Amt < 0 && Sum > 0)
					Text = "There is a conflict between Suggested amount and actual amount";
	
	if(Text.IsEmpty())
		return TRUE;

	MessageBox(Text);
	return FALSE;
}

void CPayment::Init()
{
	m_SS.SetVisibleRows(4);
	m_SS.SetVisibleCols(10);
	
	m_OT_SS.SetVisibleRows(4);
	m_OT_SS.SetVisibleCols(6);
	
	m_Sel_SS.SetVisibleRows(3);
	m_Sel_SS.SetVisibleCols(3);

	m_InstSS.SetVisibleRows(4);
	m_InstSS.SetVisibleCols(10);
	
	m_PayID.Setup(this, IDC_PAY_PAYID_EDIT);
	m_RefID.Setup(this, IDC_PAY_REF_EDIT);
	m_Dir.Setup(this, IDC_PAY_TYPE_COMBO);
	m_Portfolio.Setup(this, IDC_PAY_PORTFOLIO_COMBO);
	m_PMAccount.Setup(this, IDC_PAY_ACCOUNT_COMBO);
	m_CP.Setup(this, IDC_PAY_CP_COMBO);
	m_Date.Setup(this, IDC_PAY_DATE_EDIT);
	m_Sum.Setup(this, IDC_PAY_SUM_EDIT, NULL, 2);
	m_Amount.Setup(this, IDC_PAY_AMOUNT_EDIT, NULL, 2);
	m_Adjust.Setup(this, IDC_PAY_ADJUST_EDIT, NULL, 2);
	m_Note.Setup(this, IDC_PAY_NOTE_EDIT);
	m_Note.LimitText(40);
	m_ValueDate.Setup(this, IDC_PAY_VALUEDATE_EDIT);
	m_Currency.Setup(this, IDC_PAY_CURRENCY_COMBO);
	m_CashType.Setup(this, IDC_PAY_CASHTYPE_COMBO, TRUE);
	m_ToAccount.Setup(this, IDC_PAY_TOACCOUNT_COMBO, TRUE);
	m_Bank.Setup(this, IDC_PAY_BANK_EDIT);
	m_BankID.Setup(this, IDC_PAY_BANK_ID_EDIT);
	m_BankBIC.Setup(this, IDC_PAY_BANK_BIC_EDIT);
	m_Account.Setup(this, IDC_PAY_ACCOUNT_EDIT);
	m_Beneficiary.Setup(this, IDC_PAY_BENEFICIARY_EDIT);
	m_BeneficiaryID.Setup(this, IDC_PAY_BENEFICIARY_ID_EDIT);
	m_BeneficiaryBIC.Setup(this, IDC_PAY_BENEFICIARY_BIC_EDIT);
	m_BeneficiaryAccount.Setup(this, IDC_PAY_BENEFICIARY_ACCOUNT_EDIT);
	m_FFC.Setup(this, IDC_PAY_FFC_EDIT);
	m_FFCID.Setup(this, IDC_PAY_FFC_ID_EDIT);
	m_FFCBIC.Setup(this, IDC_PAY_FFC_BIC_EDIT);
	m_FFCAccount.Setup(this, IDC_PAY_FFC_ACCOUNT_EDIT);
	m_CPRef.Setup(this, IDC_PAY_CP_REF_EDIT);
	m_AssetCode.Setup(this, IDC_PAY_ASSET_EDIT);
	m_AssetCode.LimitText(10);
	m_Paid.Setup(this, IDC_PAY_PAID_CHECK);
	m_Fxrate.Setup(this, IDC_PAY_FXRATE_EDIT);

	CPaymentRec *pRec;
	pRec = &m_Data.GetPayRec();
	m_Data.Add(&m_PayID, &pRec->GetPayID());
	m_Data.Add(&m_RefID, &pRec->GetRefID());
	m_Data.Add(&m_Portfolio, &pRec->GetPortfolio());
	m_Data.Add(&m_PMAccount, &pRec->GetAccount());
	m_Data.Add(&m_CP, &pRec->GetCP());
	m_Data.Add(&m_Dir, &pRec->GetDir());
	m_Data.Add(&m_Date, &pRec->GetDate());
	m_Data.Add(&m_Currency, &pRec->GetCurrency());
	m_Data.Add(&m_Amount, &pRec->GetAmount());
	m_Data.Add(&m_Adjust, &pRec->GetAdjust());
	m_Data.Add(&m_Note, &pRec->GetNote());
	m_Data.Add(&m_ValueDate, &pRec->GetValueDate());
	m_Data.Add(&m_CashType, &pRec->GetCashType());
	m_Data.Add(&m_AssetCode, &pRec->GetAssetCode());
	m_Data.Add(&m_CPRef, &pRec->GetCPRef());
	m_Data.Add(&m_Paid, &pRec->GetCloseFlag());
	m_Data.Add(&m_ToAccount, &pRec->GetToAccount());
	m_Data.Add(&m_Fxrate, &pRec->GetFxrate());
	m_Data.Add(&m_Deletable);

	m_Data.GetSRowCtrl().Add(&m_Data.GetPayInvRec().GetID());
	m_Data.GetSRowCtrl().Add(&m_Data.GetPayInvRec().GetTransNum());
	m_Data.GetSRowCtrl().Add(&m_Data.GetPayInvRec().GetAmount());

	m_Data.GetPayTemplate().Add(&m_Data.GetPayTemplate().GetRec().GetRefID());
	m_Data.GetPayTemplate().Add(&m_Data.GetPayTemplate().GetRec().GetCP());
	m_Data.GetPayTemplate().Add(&m_Data.GetPayTemplate().GetRec().GetCurrency());
	m_Data.GetPayTemplate().Add(&m_Bank, &m_Data.GetPayTemplate().GetRec().GetBank());
	m_Data.GetPayTemplate().Add(&m_BankID, &m_Data.GetPayTemplate().GetRec().GetBankID());
	m_Data.GetPayTemplate().Add(&m_BankBIC, &m_Data.GetPayTemplate().GetRec().GetBankBIC());
	m_Data.GetPayTemplate().Add(&m_Account, &m_Data.GetPayTemplate().GetRec().GetAccount());
	m_Data.GetPayTemplate().Add(&m_Beneficiary, &m_Data.GetPayTemplate().GetRec().GetBeneficiary());
	m_Data.GetPayTemplate().Add(&m_BeneficiaryID, &m_Data.GetPayTemplate().GetRec().GetBeneficiaryID());
	m_Data.GetPayTemplate().Add(&m_BeneficiaryBIC, &m_Data.GetPayTemplate().GetRec().GetBeneficiaryBIC());
	m_Data.GetPayTemplate().Add(&m_BeneficiaryAccount, &m_Data.GetPayTemplate().GetRec().GetBeneficiaryAccount());
	m_Data.GetPayTemplate().Add(&m_FFC, &m_Data.GetPayTemplate().GetRec().GetFFC());
	m_Data.GetPayTemplate().Add(&m_FFCID, &m_Data.GetPayTemplate().GetRec().GetFFCID());
	m_Data.GetPayTemplate().Add(&m_FFCBIC, &m_Data.GetPayTemplate().GetRec().GetFFCBIC());
	m_Data.GetPayTemplate().Add(&m_FFCAccount, &m_Data.GetPayTemplate().GetRec().GetFFCAccount());
	m_Data.GetPayTemplate().Add(&m_CPRef, &m_Data.GetPayTemplate().GetRec().GetCPRef());

/*	m_Data.GetPayInstruct().Add(&m_Data.GetPayInstruct().GetRec().GetRefID());
	m_Data.GetPayInstruct().Add(&m_Data.GetPayInstruct().GetRec().GetCP());
	m_Data.GetPayInstruct().Add(&m_Data.GetPayInstruct().GetRec().GetCurrency());
	m_Data.GetPayInstruct().Add(&m_Bank, &m_Data.GetPayInstruct().GetRec().GetBank());
	m_Data.GetPayInstruct().Add(&m_BankID, &m_Data.GetPayInstruct().GetRec().GetBankID());
	m_Data.GetPayInstruct().Add(&m_Beneficiary, &m_Data.GetPayInstruct().GetRec().GetBeneficiary());
	m_Data.GetPayInstruct().Add(&m_BeneficiaryID, &m_Data.GetPayInstruct().GetRec().GetBeneficiaryID());
	m_Data.GetPayInstruct().Add(&m_Account, &m_Data.GetPayInstruct().GetRec().GetAccount());
	m_Data.GetPayInstruct().Add(&m_FFC, &m_Data.GetPayInstruct().GetRec().GetFFC());
	m_Data.GetPayInstruct().Add(&m_FFCAccount, &m_Data.GetPayInstruct().GetRec().GetFFCAccount()); */
}

void CPayment::LoadCoupon(BOOL nType) 
{
	CCouponListDlg CouponDlg;
	int i, j;
	CString Text;
	CDBRec Rec;

	m_Data.GetOraLoader().GetSql().Format("SELECT ASSET_CODE, TO_DATE, AMOUNT, TRANS_NUM, "
											"INV_NUM, FXRATE, REMAINING, COUPON_PAID "
											"FROM SEMAM.NW_PAYMENT_COUPON WHERE 1 = 2 ");
	m_Data.GetOraLoader().Open();
	m_Sel_SS.GetFieldArray() = m_Data.GetOraLoader().GetFieldArray();
	m_Sel_SS.SetSheetColHeaders();
	m_Sel_SS.SetVisibleCols(3);

	CouponDlg.m_OraLoader = m_Data.GetOraLoader();
	CouponDlg.m_Portfolio = m_Portfolio.GetData();
	CouponDlg.m_Currency = m_Currency.GetData();
	CouponDlg.m_CouponType = nType;
	CouponDlg.m_bCash = FALSE;
	switch(nType)
	{
		case 1: // CDS Only
			CouponDlg.m_Counterparty = m_CP.GetData();
			break;
		case 2: // IRS Only
			CouponDlg.m_Counterparty = m_CP.GetData();
			break;
		case 3: // Other Coupon
			CouponDlg.m_Counterparty.Empty();
			break;
		default:
			return;
	}

	CouponDlg.m_CouponArray.RemoveAll();
	for (i = 1; i <= m_Sel_SS.GetSheetRows(); i ++)
	{
		Rec.RemoveAll();
		Text = m_Sel_SS.GetSheetText(4, i);
		if(Text.IsEmpty())
			Text = "SECURITIES";
		else
			Text = "REPO";

		for(j = 1; j <= m_Sel_SS.GetSheetCols(); j++)
		{
			if(j == 3)
				Rec.Add(Text);
			Rec.Add(m_Sel_SS.GetSheetText(j, i));
		}

		CouponDlg.m_CouponArray.Add(Rec);
	}

	if(CouponDlg.DoModal() == IDOK)
	{
		m_Sel_SS.ClearSheet();
		for(i = 0; i < CouponDlg.m_CouponArray.GetSize(); i ++)
			m_Sel_SS.AddSheetRow(*CouponDlg.m_CouponArray.GetAt(i));

		m_Amount.SetData(CouponDlg.m_Amount);
		m_Adjust.SetData(CouponDlg.m_Remaining);
		m_Sum.SetData(CouponDlg.m_Amount);
	}
}


BOOL CPayment::UpdateData(BOOL bSaveAndValidate)
{
	CQData QData;

	if(bSaveAndValidate)
	{
		CString CashType;

		CashType = m_CashType.GetData();
		m_Data.SetbCoupon(CashType == "COUPON PAY" || CashType == "SWAP INT");

		if(IsOK())
			m_Data.UpdateRow();
	}
	else
	{
		m_Data.LoadDBData();

		GetData().LoadSupportData();
		GetData().LoadCashData();
		GetData().GetCurrencyArr().CopyToComboBox(m_Currency);
		GetData().GetPortfolioArr().CopyToComboBox(m_Portfolio);
		GetData().GetAccountArr().CopyToComboBox(m_PMAccount);
		GetData().GetAccountArr().CopyToComboBox(m_ToAccount);
		GetData().GetCashTypeArr().CopyToComboBox(m_CashType);
		if(m_CP.GetCount() == 0)
			GetData().GetContactList().CopyKeyToComboBox(m_CP);
	}
	return TRUE;
}

void CPayment::UpdateSum(BOOL bAdjustment)
{
	CString Text;

	if(bAdjustment)
	{
		double Amt, Sum, Adjust;
		CQData QData;

		Sum = atof(QData.RemoveComma(m_Sum.GetData()));		
		Amt = atof(QData.RemoveComma(m_Amount.GetData()));
		Adjust = Amt - Sum;
		m_Adjust.SetData(QData.WriteNumber(Adjust, TRUE, 2));
	}
	else
	{
		m_Data.ComputeSum(m_Sel_SS, Text);
		m_Amount.SetData(Text);
		m_Adjust.SetData("0.00");
		m_Sum.SetData(Text);
	}
}
/////////////////////////////////////////////////////////////////////////////
// CPayment message handlers

BEGIN_EVENTSINK_MAP(CPayment, CFormView)
    //{{AFX_EVENTSINK_MAP(CPayment)
	ON_EVENT(CPayment, IDC_PAY_OPENTRADE_LIST, 11 /* DblClick */, OnDblClickPayOpentradeList, VTS_I4 VTS_I4)
	ON_EVENT(CPayment, IDC_PAY_SEL_TRADE_LIST, 11 /* DblClick */, OnDblClickPaySelTradeList, VTS_I4 VTS_I4)
	ON_EVENT(CPayment, IDC_PAY_INSTRUCTION_LIST, 11 /* DblClick */, OnDblClickPayInstructionList, VTS_I4 VTS_I4)
	ON_EVENT(CPayment, IDC_PAY_LIST, 2 /* BlockSelected */, OnBlockSelectedPayList, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	ON_EVENT(CPayment, IDC_PAY_LIST, 11 /* DblClick */, OnDblClickPayList, VTS_I4 VTS_I4)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CPayment::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	Init();
	
	GetData().LoadDates();
	m_Dir.AddString("PAYMENT");
	m_Dir.AddString("RECEIPT");
	
	GetData().GetOraLoader().Today(m_StrDate);
	m_Date.SetData(m_StrDate);
	
	m_Data.Setup(GetData().GetOraLoader(), &m_SS, &m_Sel_SS, 0);
	m_Data.LoadPortArr();
	m_Data.GetPayTemplate().Setup(GetData().GetOraLoader(), &m_InstSS);
	m_Data.LoadTrades(m_OT_SS);
	m_Data.SetHeaders();
	
	UpdateData(FALSE);
	if(m_SS.GetSheetRows() == 0)
		m_ValueDate.SetData(m_StrDate);

	OnSelchangePayCashtypeCombo();
	GetDocument()->SetTitle(GetData().GetUser());
	GetDocument()->SetViewTitle("PAYMENT/RECEIPT INSTRUCTION");
}

void CPayment::OnDblClickPayOpentradeList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_OT_SS.GetSheetRows())
	{
		CString Text, Data;
		CQData QData;

		Text = m_OT_SS.GetSheetText(1, Row);
		Data = m_Portfolio.GetData();
		if(Data.IsEmpty())
			m_Portfolio.SetData(Text);
		else
			if(Text != Data)
			{
				MessageBox("There is conflict in portfolio");
				return;
			}
	
		if(!FindInvNum(m_OT_SS.GetSheetText(5, Row)))
		{
			CDBRec Rec;
			Rec.SetInDatabase(FALSE);
			Rec.SetModify();

			Rec.Add(m_OT_SS.GetSheetText(5, Row)); // InvNum
			Rec.Add(m_OT_SS.GetSheetText(4, Row)); // TransNum
			Text = m_OT_SS.GetSheetText(7, Row);   // Amount

			Rec.Add(QData.WriteNumber(Text, TRUE, 2));
			m_Data.GetSRowCtrl().AddSheetRow(Rec);
			UpdateSum();
		}
	}
}

void CPayment::OnDblClickPaySelTradeList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_Sel_SS.GetSheetRows())
	{
		m_Sel_SS.DeleteSheetRow(Row);
		UpdateSum();
	}
}

void CPayment::OnDblClickPayInstructionList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_InstSS.GetSheetRows())
	{
		if(m_Data.GetPayTemplate().GetRec().GetRefID().IsEmpty() ||
			m_Data.GetPayRec().GetRefID() != m_Data.GetPayTemplate().GetRec().GetRefID())
		{
			m_Data.GetPayTemplate().SetCurrentRow(Row);
			m_RefID.SetData(m_Data.GetPayTemplate().GetRec().GetRefID());
			m_CP.SetData(m_Data.GetPayTemplate().GetRec().GetCP());
			m_Currency.SetData(m_Data.GetPayTemplate().GetRec().GetCurrency());
			m_OT_SS.ClearSheet();
		}
	}
}

void CPayment::OnChangePayAmountEdit() 
{
	if(m_Amount.GetModify())
	{
		if(m_Sel_SS.GetSheetRows() > 0)
			UpdateSum(TRUE);
		else
			m_Adjust.SetData("0.00");
	}
}

void CPayment::OnPayClearscreen() 
{
	m_Data.GetPayTemplate().SetCurrentRow(-1);
	m_Data.GetPayTemplate().Refresh();
	m_Data.SetCurrentRow(-1);
	m_Data.Refresh();
	m_Data.GetSRowCtrl().ClearSheet();
	m_Data.GetSRowCtrl().Refresh();
	m_Data.SetbCoupon(FALSE);
	m_Sum.SetData(EMPTYSTRING);

	m_Date.SetData(m_StrDate);
	m_ValueDate.SetData(m_StrDate);
}

void CPayment::OnPayFind() 
{
	UpdateData(FALSE);
}

void CPayment::OnUpdatePayLoadOpentrade(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_CP.GetCurSel() >= 0 && m_Currency.GetCurSel() > 0 && !m_ValueDate.GetData().IsEmpty());
}

void CPayment::OnPayLoadOpentrade() 
{
	m_Data.LoadTrades(m_OT_SS, TRUE);
	m_Sel_SS.SetVisibleCols(3);
	m_Data.GetOraLoader().GetSql().Format("SELECT INV_NUM, TRANS_NUM, AMOUNT "
											"FROM SEMAM.NW_PAYMENT_DETAILS WHERE 1 = 2 ");
	m_Data.GetOraLoader().Open();
	m_Sel_SS.SetSheetColHeaders();

}

void CPayment::OnPayAdd() 
{
	m_Data.SetKey(EMPTYSTRING);

	CString ID;
	m_Data.GetOraLoader().GenerateUniqueID("PAY_ID", ID);
	m_PayID.SetData(ID);
	
	UpdateData(TRUE);
}

void CPayment::OnPayUpdate() 
{
	UpdateData(TRUE);
}

void CPayment::OnUpdatePayUpdate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_Data.GetCurrentRow() > 0 && !m_PayID.GetData().IsEmpty());
}

void CPayment::OnPayDelete() 
{
	if(MessageBox("Delete selected entry ? ", NULL, MB_OKCANCEL) == IDOK)
		m_Data.DeleteRow();
}

void CPayment::OnUpdatePayDelete(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable((GetData().IsPowerUser() || GetData().IsOperation() && m_Deletable == "1") && 
					m_Data.GetCurrentRow() > 0 && m_SS.GetIsBlockSelected() && !m_PayID.GetData().IsEmpty());
}

void CPayment::OnSelchangePayTypeCombo() 
{
	CString Text;
	
	Text = m_Dir.GetData();
	if(Text.IsEmpty())
		return;

	if(m_Data.GetPayRec().GetDir() != Text)
	{
//		if(Text == "PAYMENT")
//			m_Data.LoadTemplates(TRUE);
//		else
//			m_InstSS.ClearSheet();
		if(Text != "PAYMENT")
			m_InstSS.ClearSheet();

		CString Date;

		Date = m_Date.GetData();
		m_OT_SS.ClearSheet();
		m_Data.GetPayTemplate().SetCurrentRow(-1);
		m_Data.GetPayTemplate().Refresh();
		m_Data.SetCurrentRow(-1);
		m_Data.Refresh();
		m_Data.GetSRowCtrl().ClearSheet();
		m_Data.GetSRowCtrl().Refresh();

		m_Dir.SetData(Text);
		m_Date.SetData(Date);
		m_ValueDate.SetData(Date);
		m_Data.SetbCoupon(FALSE);
		m_Sum.SetData(EMPTYSTRING);
	}
}

void CPayment::OnBlockSelectedPayList(long BlockCol, long BlockRow, long BlockCol2, long BlockRow2) 
{
	OnDblClickPayList(m_SS.GetActiveCol(), m_SS.GetActiveRow());
}

void CPayment::OnDblClickPayList(long Col, long Row) 
{
	if(Row >= 1 && Row <= m_SS.GetSheetRows())
	{
		CString Text;
		double Sum = 0;
		CQData QData;

		m_Data.SetbCoupon((strcmp(m_SS.GetSheetText(13, Row), "COUPON PAY") == 0 ||
						strcmp(m_SS.GetSheetText(13, Row), "SWAP INT") == 0) ? TRUE : FALSE);
		m_Data.SetCurrentRow(Row);
		if(m_Data.ComputeSum(m_Sel_SS, Text) != 0)
			m_Sum.SetData(Text);
		else
		{
			Sum += atof(QData.RemoveComma(m_Amount.GetData()));
			Sum += atof(QData.RemoveComma(m_Adjust.GetData()));
			m_Sum.SetData(QData.WriteNumber(Sum, TRUE, 2));
		}
	}
}

void CPayment::OnPayReport() 
{
	m_Data.GeneratePayment();
}

void CPayment::OnUpdatePayReport(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_SS.GetSheetRows() > 0);
}

void CPayment::OnPayClearall() 
{
	OnPayClearscreen();

	m_Date.SetData(EMPTYSTRING);
	m_ValueDate.SetData(EMPTYSTRING);
}

void CPayment::OnSelchangePayPortfolioCombo() 
{
	CString Portfolio, Dir;

	Portfolio = m_Portfolio.GetData();
	if(Portfolio.IsEmpty())
		return;

	Dir = m_Dir.GetData();

	if(Dir == "PAYMENT")
		m_Data.LoadTemplates(Portfolio, TRUE);

	GetData().GetPortfolioArr().SetPortfolio(Portfolio);
	m_PMAccount.SetData(GetData().GetPortfolioArr().GetAccount());
}

void CPayment::OnPayFindAsset()
{
	CAssetDlg Dlg;

	Dlg.m_pData = &GetData();
	Dlg.m_FindData.GetRec().GetAsset() = m_AssetCode.GetData();

	if(Dlg.DoModal() != IDOK)
		return;

	m_AssetCode.SetData(Dlg.m_FindData.GetRec().GetAsset());
}

void CPayment::OnUpdatePayFindAsset(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_nCurrID == IDC_PAY_ASSET_EDIT ? TRUE : FALSE);
}

void CPayment::OnSetfocusPayAssetEdit()
{
	m_nCurrID = IDC_PAY_ASSET_EDIT;
}

void CPayment::OnKillfocusPayAssetEdit()
{
	m_nCurrID = 0;
}

void CPayment::OnUpdatePayLoadCoupon(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_Portfolio.GetCurSel() >= 0 && (m_CashType.GetData() == "COUPON PAY") ? TRUE : FALSE);
}

void CPayment::OnPayLoadCoupon() 
{
	LoadCoupon(3);
}

void CPayment::OnPayLoadCdsCoupon() 
{
	LoadCoupon(1);
}

void CPayment::OnUpdatePayLoadCdsCoupon(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_Portfolio.GetCurSel() >= 0 && m_CP.GetCurSel() >= 0 && m_CashType.GetData() == "COUPON PAY" ? TRUE : FALSE);	
}

void CPayment::OnSelchangePayCashtypeCombo() 
{
	CString Type;
	BOOL bEnable;

	Type = m_CashType.GetData();
	if(Type == "COUPON PAY" || Type == "SWAP INT")
	{
		m_AssetCode.EnableWindow(FALSE);
		m_Data.SetbCoupon(TRUE);
	}
	else
		m_Data.SetbCoupon(FALSE);

	bEnable = !Type.IsEmpty();
	if(Type != "COUPON PAY" && Type != "SWAP INT")
		m_AssetCode.EnableWindow(bEnable);
	m_Fxrate.EnableWindow(bEnable);
	m_ToAccount.EnableWindow(bEnable);
}

void CPayment::OnPayLoadIrsCoupon()
{
	LoadCoupon(2);
}

void CPayment::OnUpdatePayLoadIrsCoupon(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_Portfolio.GetCurSel() >= 0 && m_CP.GetCurSel() >= 0 && m_CashType.GetData() == "SWAP INT" ? TRUE : FALSE);	
}
