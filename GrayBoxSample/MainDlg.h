#pragma once


// MainDlg dialog

class MainDlg : public CDialog, public L_Observer
{
	DECLARE_DYNAMIC(MainDlg)

public:
	MainDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~MainDlg();

// Dialog Data
	enum { IDD = IDD_MAINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
	virtual void OnOK() {}
	virtual void OnCancel() {}
	virtual BOOL OnInitDialog();
	afx_msg void OnNcDestroy();
	afx_msg void OnClose();

// L_Observer
	virtual void HandleMessage(L_Message const *msg);
private:
	L_Account *account;
	L_Summary *summary;
private:
	CListBox m_MessageList;
	CEdit m_AtrEdit;
	CComboBox m_TimerCombo;
	CEdit m_SymbolEdit;
	CComboBox m_StopAtrCombo;
	CComboBox m_LooAtrCombo;

public:
	afx_msg void OnBnClickedStartbtn();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStopbtn();
	void SendMyMessage(CString const);
	
	void MyTrade();
	double getMax(double ,double ,double);
	double getAtr();
	CString getTime();
	
	void setCombos();
	bool getSet();
	int getOrder();
	tm *getTm();
	afx_msg void OnBnClickedSavebtn();
	afx_msg void OnBnClickedLoadbtn();
	CListBox m_SymbolList;
};
