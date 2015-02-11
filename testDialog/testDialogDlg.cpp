
// testDialogDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "testDialog.h"
#include "testDialogDlg.h"
#include "afxdialogex.h"
#include "LoginDialog.h"
#include "UartDialog.h"
#include "ApSelDialog.h"
#include "ScanDialog.h"
#include "FinishDialog.h"

#include "Serial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestDialogDlg ��ȭ ����



CtestDialogDlg::CtestDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CtestDialogDlg::IDD, pParent)
	, m_strMac(_T(""))
	, m_strUsername(_T(""))
	, m_strPassword(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pLoginDlg = NULL;
	m_pUartDlg = NULL;
	m_pSerial = NULL;
	m_pApSelDlg = NULL;
	m_pScanDlg = NULL;
	m_pFinishDlg = NULL;
}

void CtestDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CtestDialogDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CtestDialogDlg �޽��� ó����

BOOL CtestDialogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	if (NULL == m_pLoginDlg) {
		m_pLoginDlg = new CLoginDialog(this, this);
		m_pLoginDlg->Create(CLoginDialog::IDD, this);
	}
	m_pLoginDlg->ShowWindow(SW_SHOWNORMAL);
	m_pLoginDlg->SetFocus();

	if (NULL == m_pUartDlg) {
		m_pUartDlg = new CUartDialog(this, this);
		m_pUartDlg->Create(CUartDialog::IDD, this);
	}

	if (NULL == m_pApSelDlg) {
		m_pApSelDlg = new CApSelDialog(this, this);
		m_pApSelDlg->Create(CApSelDialog::IDD, this);
	}

	if (NULL == m_pScanDlg) {
		m_pScanDlg = new CScanDialog(this, this);
		m_pScanDlg->Create(CScanDialog::IDD, this);
	}

	if (NULL == m_pFinishDlg) {
		m_pFinishDlg = new CFinishDialog(this, this);
		m_pFinishDlg->Create(CFinishDialog::IDD, this);
	}

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CtestDialogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CtestDialogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


CtestDialogDlg::~CtestDialogDlg()
{
	delete m_pLoginDlg;
	delete m_pUartDlg;
	delete m_pApSelDlg;
	delete m_pSerial;
	delete m_pScanDlg;
	delete m_pFinishDlg;
}

void CtestDialogDlg::NextProcess(CDialogEx * pChild)
{
	if (pChild == m_pLoginDlg) {
		SetWindowText(_T("���� ��Ʈ �˻�"));
		m_pLoginDlg->ShowWindow(SW_HIDE);
		m_pUartDlg->ShowWindow(SW_SHOWNORMAL);
		m_pUartDlg->SetFocus();
	}
	else if (pChild == m_pUartDlg) {
		SetWindowText(_T("AP ���� �Է�"));
		m_pUartDlg->ShowWindow(SW_HIDE);
		m_pApSelDlg->ShowWindow(SW_SHOWNORMAL);
		m_pApSelDlg->SetFocus();
	}
	else if (pChild == m_pApSelDlg) {
		SetWindowText(_T("������� ���� �Է�"));
		m_pApSelDlg->ShowWindow(SW_HIDE);
		m_pFinishDlg->ShowWindow(SW_SHOWNORMAL);
		m_pFinishDlg->SetFocus();
	}
	else if (pChild == m_pScanDlg) {
		SetWindowText(_T("������� ���� �Է�"));
		m_pScanDlg->ShowWindow(SW_HIDE);
		m_pFinishDlg->ShowWindow(SW_SHOWNORMAL);
		m_pFinishDlg->SetFocus();
	}
	else {
		EndDialog(0);
	}
}


void CtestDialogDlg::ToScanWindow()
{
	SetWindowText(_T("AP ��ĵ"));
	m_pApSelDlg->ShowWindow(SW_HIDE);
	m_pScanDlg->ShowWindow(SW_NORMAL);
	m_pScanDlg->SetFocus();
}
