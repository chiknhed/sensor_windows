
// testDialogDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"


class CLoginDialog;
class CUartDialog;
class CApSelDialog;
class CScanDialog;
class CFinishDialog;

class CSerial;

// CtestDialogDlg ��ȭ ����
class CtestDialogDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CtestDialogDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_TESTDIALOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;
	
	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel2();
	CTabCtrl m_tabCtrl;
	~CtestDialogDlg();
	void NextProcess(CDialogEx* pChild);
	CSerial* m_pSerial;
	CLoginDialog *m_pLoginDlg;
	CUartDialog * m_pUartDlg;
	CApSelDialog *m_pApSelDlg;
	CScanDialog * m_pScanDlg;
	CFinishDialog * m_pFinishDlg;
	void ToScanWindow();
	CString m_strMac;
	CString m_strUsername;
	CString m_strPassword;
};
