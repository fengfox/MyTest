// MainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GrayBoxSample.h"
#include "MainDlg.h"
#include "afxdialogex.h"
#include <vector>
#include <time.h>
#include "tinyxml.h"
#pragma comment(lib,"tinyxml.lib")
using namespace std;
//需要初始化的数据
BOOL isWatch,isOrder;
int nTimer,nLooAtr,nStopAtr;
float fAtr;

CString sSymbol;

// MainDlg dialog

IMPLEMENT_DYNAMIC(MainDlg, CDialog)

MainDlg::MainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(MainDlg::IDD, pParent)
	, account(0)
	, summary(0)
{


}

MainDlg::~MainDlg()
{
	if (summary)
	{
		summary->L_Detach(this);
		L_DestroySummary(summary);
	}
	if (account)
	{
		
		account->L_Detach(this);
	}
}

void MainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MESSAGELIST, m_MessageList);
	DDX_Control(pDX, IDC_SYMBOLEDIT, m_SymbolEdit);
	DDX_Control(pDX, IDC_ATREDIT, m_AtrEdit);
	DDX_Control(pDX, IDC_TIMERCOMBO, m_TimerCombo);

	DDX_Control(pDX, IDC_STOPATRCOMBO, m_StopAtrCombo);
	DDX_Control(pDX, IDC_LOOATRCOMBO, m_LooAtrCombo);
}


BEGIN_MESSAGE_MAP(MainDlg, CDialog)

	ON_WM_NCDESTROY()
	ON_WM_CLOSE()

ON_BN_CLICKED(IDC_STARTBTN, &MainDlg::OnBnClickedStartbtn)
ON_WM_TIMER()
ON_BN_CLICKED(IDC_STOPBTN, &MainDlg::OnBnClickedStopbtn)

ON_BN_CLICKED(IDC_SAVEBTN, &MainDlg::OnBnClickedSavebtn)
ON_BN_CLICKED(IDC_LOADBTN, &MainDlg::OnBnClickedLoadbtn)
END_MESSAGE_MAP()

void MainDlg::OnNcDestroy()
{
	delete this;
}

void MainDlg::OnClose()
{
	L_ExitLightspeedExtension(2);
}

BOOL MainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	account = L_GetAccount();
	account->L_Attach(this);
	//初始化combo控件
	setCombos();

	//初始化数据
	isWatch=false;
	isOrder=getOrder();
	

	m_MessageList.InsertString(-1,"Init Success");
	
	return TRUE;
}

void MainDlg::HandleMessage(L_Message const *msg)
{
	switch (msg->L_Type())
	{
	case L_MsgOrderChange::id:
		SetDlgItemInt(IDC_PENDINGORDERS, account->L_PendingOrdersCount());
		break;
	case L_MsgL1::id:
	case L_MsgL1Update::id:
		{
			double bid = summary->L_Bid();
			double ask = summary->L_Ask();
			CString buf;
			buf.Format("%.2f", bid);
			SetDlgItemText(IDC_BID, buf);
			buf.Format("%.2f", ask);
			SetDlgItemText(IDC_ASK, buf);
		}
		break;
	}
}

//void MainDlg::OnBnClickedSendOrder()
//{
//	account->L_SendOrder(
//				summary,
//				L_OrderType::LIMIT,
//				L_Side::BUY,
//				100,
//				summary->L_Bid() - 0.01,
//				"NSDQ",
//				L_TIF::DAY
//				);
//	L_AddMessageToExtensionWnd("OnBnClickedSendOrder");
//}




void MainDlg::OnBnClickedStartbtn()
{
	// TODO: 在此添加控件通知处理程序代码
	if(getSet()==false)
	{
		return;
	}
	if(isWatch==false)
	{

		if(summary = L_CreateSummary(sSymbol))
		{
			summary->L_Attach(this);
			this->SendMyMessage("summary create success");
			SetTimer(1,1000*nTimer,NULL);
			CString sTmp;
			sTmp="start watching ";
			sTmp.Append(sSymbol);

			this->SendMyMessage(sTmp);
			isWatch=true;
		
		}
		else
		{
			this->SendMyMessage("summary create fail");
		
		}
		
		
	}
	else
	{
		this->SendMyMessage("is watching");
	}
	
}


void MainDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	__super::OnTimer(nIDEvent);
	
		MyTrade();
}


void MainDlg::OnBnClickedStopbtn()
{
	// TODO: 在此添加控件通知处理程序代码
	if(isWatch==false)
	{
		this->SendMyMessage("isn't watching");
	}
	else
	{
		
		summary->L_Detach(this);
		L_DestroySummary(summary);
		this->SendMyMessage("summary destroy success");
		KillTimer(1);
		CString sTmp;
		sTmp="stop watching ";
		sTmp.Append(sSymbol);
		this->SendMyMessage(sTmp);
		isWatch=false;
		
		
	}
}

void MainDlg::SendMyMessage(CString const str)
{
	m_MessageList.SetTopIndex(m_MessageList.InsertString(-1,str));
	L_AddMessageToExtensionWnd(str);
	
}
void MainDlg::MyTrade()
{
	/*
	double open=summary->L_Open();
	double close=summary->L_Close();
	//获取时间
	this->SendMyMessage("-----------------------------------------------");
	this->SendMyMessage(this->getTime());

	if(close>open)
	{
		SendMyMessage("Watching...close>open");
		double primary_open=summary->L_PrimaryOpen();
		double high=summary->L_High();
		double low=summary->L_Low();
		double previous_close=summary->L_PrevClose();
		double max=getMax(high-low,high-previous_close,previous_close-low);
		double atr=getAtr();
		//dmax.push_back(max);
		CString retString="";
		retString.Format("%.2f",max);
		SendMyMessage(retString);
		if(primary_open>(atr/10+close))
		{
			account->L_SendOrder(
			summary,
			L_OrderType::LOO,
			L_Side::SELL,
			100,
			summary->L_PrimaryOpen(),
			"NSDQ",
			L_TIF::GTC
			);
			SendMyMessage("SendOrder LOO!");
		}	
	}
	else
	{
		SendMyMessage("Watching...close<=open");
	}
	*/
	this->SendMyMessage("-----------------------------------------------");
	this->SendMyMessage(this->getTime());
	switch(L_MarketStatus())
	{
	case L_Market::Preopening://如果是准备开盘的状态
		{
			SendMyMessage("Status:Preopening!");
			if(summary->L_Open()<summary->L_PrevClose())
			{
				
				if(getOrder()==0)//如果没有单,就下单
				{
					account->L_SendOrder(
					summary,
					L_OrderType::LOO,
					L_Side::SELL,
					100,
					(summary->L_PrimaryOpen()+(int)(nLooAtr*fAtr)/100.0),
					"NYSE",
					L_TIF::GTC
					);
					SendMyMessage("SendOrder LOO!");
					break;
				}
				else
				{
					SendMyMessage("you have already placed order");
					break;
				}
			}
			else
			{
				SendMyMessage("yesterday open>close");
				OnBnClickedStopbtn();
				break;
			}
		}
	case L_Market::Premarket:
		{
			SendMyMessage("Status:PreMarket!");
			break;
		}
	case L_Market::RegSessionOpen:
		{
			SendMyMessage("Status:RegSessionOpen!");
			switch(getOrder())
			{
			case 0://开盘的时候没有单子，直接退出
				{
					SendMyMessage("No Order");
					OnBnClickedStopbtn();
					break;
				}
			case 1://开盘的时候有一个单子
				{
					order_iterator it,itend;
					it=account->orders_begin();
					
					if((*it)->L_ExecutedShares()!=0)//单子已执行
					{
						account->L_SendOrder(
						summary,
						L_OrderType::STOP,
						L_Side::BUY,
						100,
						(summary->L_PrimaryOpen()+(int)(nStopAtr*fAtr)/100.0),
						"STOP",
						L_TIF::DAY
						);
						SendMyMessage("SendOrder STOP!");
						break;
					}
					break;
				}
			case 2://开盘的时候有2个单子，直接退出.
				{
					SendMyMessage("orders have been obeyed!");
					OnBnClickedStopbtn();
					break;
				}
			}
			
		}
	default:
		{
			SendMyMessage("Status:Others!");
		}
	}
	/*
	if(this->getTm()->tm_hour<9||(this->getTm()->tm_hour==9&&this->getTm()->tm_min<30))//在9点30以前
	{
		SendMyMessage("market isn't open");
		if(summary->L_Open()<summary->L_Close())//如果前一日开盘<收盘
		{
			bool hasOrder=getOrder();
			if(hasOrder==false)//如果没有单,就下单
			{
				account->L_SendOrder(
				summary,
				L_OrderType::LOO,
				L_Side::SELL,
				100,
				(summary->L_PrimaryOpen()+(int)(nLooAtr*fAtr)/100.0),
				"NSDQ",
				L_TIF::GTC
				);
				SendMyMessage("SendOrder LOO!");
			}
		}
		else 
		{
			SendMyMessage("Open>=Close");
			OnBnClickedStopbtn();
		}
		
	}
	else
	{
		SendMyMessage("market is open");
	}
	*/
}
double MainDlg::getMax(double value1,double value2,double value3)
{
	double max;
	if(value1>value2)
	{
		max=value1;
	}
	else
	{
		max=value2;
	}
	if(max>value3)
	{
	}
	else
	{
		max=value3;
	}
	return max;
}

double MainDlg::getAtr()
{
	double atr=0.1;
	return atr;
}


CString MainDlg::getTime()
{
	struct tm *ptr,p;
	time_t it;
	it=time(NULL);
	ptr=localtime(&it);
	L_GetUSEasternTm(*ptr);
	CString buf="";
	CString str="";
	
	buf.Format("%d/",ptr->tm_year+1900);
	str.Append(buf);
	buf.Format("%d/",ptr->tm_mon+1);
	str.Append(buf);
	buf.Format("%d ",ptr->tm_mday);
	str.Append(buf);
	buf.Format("%d:",ptr->tm_hour);
	str.Append(buf);
	buf.Format("%d:",ptr->tm_min);
	str.Append(buf);
	buf.Format("%d",ptr->tm_sec);
	str.Append(buf);
	return str;
}
tm *MainDlg::getTm()
{
	struct tm *ptr,p;
	time_t it;
	it=time(NULL);
	ptr=localtime(&it);
	L_GetUSEasternTm(*ptr);
	return ptr;
}
void MainDlg::setCombos()
{
	//设置timer
	m_TimerCombo.InsertString(-1,"1");
	m_TimerCombo.InsertString(-1,"2");
	m_TimerCombo.InsertString(-1,"5");
	m_TimerCombo.InsertString(-1,"10");
	m_TimerCombo.SetCurSel(0);
	//设置looatr
	m_LooAtrCombo.InsertString(-1,"10");
	m_LooAtrCombo.InsertString(-1,"15");
	m_LooAtrCombo.InsertString(-1,"20");
	m_LooAtrCombo.InsertString(-1,"25");
	m_LooAtrCombo.SetCurSel(0);
	//设置stopatr
	m_StopAtrCombo.InsertString(-1,"20");
	m_StopAtrCombo.InsertString(-1,"25");
	m_StopAtrCombo.InsertString(-1,"30");
	m_StopAtrCombo.InsertString(-1,"35");
	m_StopAtrCombo.SetCurSel(0);
}
bool MainDlg::getSet()
{
	CString sAtr;
	m_AtrEdit.GetWindowTextA(sAtr);
	m_SymbolEdit.GetWindowTextA(sSymbol);//获得Symbol的值
	CString sTimer,sLooAtr,sStopAtr;
	m_TimerCombo.GetWindowTextA(sTimer);
	m_LooAtrCombo.GetWindowTextA(sLooAtr);
	m_StopAtrCombo.GetWindowTextA(sStopAtr);
	nTimer=atoi(sTimer.GetBuffer(sTimer.GetLength()));//获得nTimer的值
	nLooAtr=atoi(sLooAtr.GetBuffer(sLooAtr.GetLength()));//获得looAtr的值
	nStopAtr=atoi(sStopAtr.GetBuffer(sStopAtr.GetLength()));//获得stopAtr的值
	if(sSymbol.IsEmpty())
	{
		this->SendMyMessage("Set Symbol");
		return false;
	}
	if(sAtr.IsEmpty())
	{
		this->SendMyMessage("Set ATR");
		return false;
	}
	else
	{
		fAtr=(float)atof(sAtr.GetBuffer(sAtr.GetLength()));//获得Atr的值
	}
	return true;
}
//获取是否有order,如果当前account没有order返回false,有的话返回true,不判断order是否为activity.
int MainDlg::getOrder()
{
	
	order_iterator it,itend;
	
	it=account->orders_begin();
	itend=account->orders_end();
	int count=0;
	for(;it!=itend;++it)
	{
		count++;
	}
	return count;
}



void MainDlg::OnBnClickedSavebtn()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog hFileDlg(false,NULL,"a.xml",OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_READONLY,
       TEXT("xml文件(*.xml)|*.xml|所有文件(*.*)|*.*||"),NULL);
	hFileDlg.m_ofn.nFilterIndex=1;
	hFileDlg.m_ofn.hwndOwner=m_hWnd;
	hFileDlg.m_ofn.lStructSize=sizeof(OPENFILENAME);
	hFileDlg.m_ofn.lpstrTitle=TEXT("打开xml文件...\0");
	hFileDlg.m_ofn.nMaxFile=MAX_PATH;
	
	if(hFileDlg.DoModal() == IDOK)
	{
		CString m_path = hFileDlg.GetPathName();
		
		UpdateData(FALSE);
		SendMyMessage(m_path);
		//char szFile[64];
		HANDLE handle;
		//unsigned long lWritten,lRead;
		handle=CreateFile(m_path,GENERIC_READ|GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);
		CloseHandle(handle);
		//写入xml
		
	
	
	}
}
	






void MainDlg::OnBnClickedLoadbtn()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog hFileDlg(TRUE,NULL,NULL,OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_READONLY,TEXT("xml文件(*.xml)|*.xml|所有文件(*.*)|*.*||"),NULL);
	hFileDlg.m_ofn.nFilterIndex=1;
	hFileDlg.m_ofn.hwndOwner=m_hWnd;
	hFileDlg.m_ofn.lStructSize=sizeof(OPENFILENAME);
	hFileDlg.m_ofn.lpstrTitle=TEXT("打开xml文件...\0");
	hFileDlg.m_ofn.nMaxFile=MAX_PATH;
	if(hFileDlg.DoModal() == IDOK)
	{
		CString m_path = hFileDlg.GetPathName();
		UpdateData(FALSE);
		SendMyMessage(m_path);
	}
}
