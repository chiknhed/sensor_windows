// ApSelDialog.cpp : implementation file
//

	#include "stdafx.h"
#include "testDialog.h"
#include "ApSelDialog.h"
#include "afxdialogex.h"

#include "testDialogDlg.h"
#include "Serial.h"

// CApSelDialog dialog

#define SET_OR_SCAN_MESSAGE "Press 'a' to set AP settings, Press 's' to scan APs : \r\n"
#define SSID_MESSAGE "SSID : "
#define SECURITY_MESSAGE "Security : "
#define PASSWORD_MESSAGE "Password : "
#define CONNECTING_MESSAGE "connecting to AP.. \r\n"
#define CONNECTED_MESSAGE "AP connected 1\r\n"
#define CONNECTION_FAIL_MESSAGE "AP connected 1\r\n"
#define WRITE_EEPROM_MESSAGE "Writing EEPROM\r\n"
#define READ_EEPROM_MESSAGE "reading EEPROM..\r\n"
#define MAC_MESSAGE "mac: 08-00-00-59-2d-26\r\n"
#define MAX_INIT_STRING_LEN	300

IMPLEMENT_DYNAMIC(CApSelDialog, CDialogEx)

CApSelDialog::CApSelDialog(CWnd* pParent /*=NULL*/, CtestDialogDlg * p)
	: CDialogEx(CApSelDialog::IDD, pParent)
{
	_parent = p;

}

CApSelDialog::~CApSelDialog()
{
}

void CApSelDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_APNAME_EDIT, m_ApNameEdit);
	DDX_Control(pDX, IDC_APPASSWORD_EDIT, m_ApPasswordEdit);
	DDX_Control(pDX, IDC_SECURITY_COMBO, m_SecurityCB);
	DDX_Control(pDX, IDC_SETAP_BTN, m_SetApBtn);
	DDX_Control(pDX, IDC_SCANAP_BTN, m_ScanApBtn);
	DDX_Control(pDX, IDC_PROGRESS1, m_ProgressCtrl);
}


BEGIN_MESSAGE_MAP(CApSelDialog, CDialogEx)
	ON_COMMAND(IDCANCEL, &CApSelDialog::OnIdcancel)
	ON_COMMAND(IDOK, &CApSelDialog::OnIdok)
	ON_BN_CLICKED(IDC_SETAP_BTN, &CApSelDialog::OnBnClickedSetapBtn)
	ON_BN_CLICKED(IDC_SCANAP_BTN, &CApSelDialog::OnBnClickedScanapBtn)
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_SECURITY_COMBO, &CApSelDialog::OnCbnSelchangeSecurityCombo)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CApSelDialog message handlers


void CApSelDialog::OnIdcancel()
{
	// TODO: Add your command handler code here
}


void CApSelDialog::OnIdok()
{
	// TODO: Add your command handler code here
}

static UINT ConnectMonitor(LPVOID pParam)
{
	CApSelDialog* pDlg = (CApSelDialog*)pParam;
	char buffer[MAX_INIT_STRING_LEN];
	bool bConnected = true;
	bool bMacReceived = false;
	int readBytes;
	CSerial * pSerial = pDlg->_parent->m_pSerial;
	unsigned int mac[6];
	
	readBytes = pSerial->ReadBytesOrWait(buffer, strlen(CONNECTED_MESSAGE), 1000000);
	if (readBytes != strlen(CONNECTED_MESSAGE)) {
		bConnected = false;
	}
	else if (strncmp(CONNECTED_MESSAGE, buffer, strlen(CONNECTED_MESSAGE))) {
		bConnected = false;
	}

	if (bConnected) {
		readBytes = pSerial->ReadBytesOrWait(buffer, strlen(WRITE_EEPROM_MESSAGE), 1000000);
		if (readBytes != strlen(WRITE_EEPROM_MESSAGE)) {
			bConnected = false;
		}
		else if (strncmp(WRITE_EEPROM_MESSAGE, buffer, strlen(WRITE_EEPROM_MESSAGE))) {
			bConnected = false;
		}

		readBytes = pSerial->ReadBytesOrWait(buffer, strlen(READ_EEPROM_MESSAGE), 1000000);
		if (readBytes != strlen(READ_EEPROM_MESSAGE)) {
			bConnected = false;
		}
		else if (strncmp(READ_EEPROM_MESSAGE, buffer, strlen(READ_EEPROM_MESSAGE))) {
			bConnected = false;
		}
	}

	readBytes = pSerial->ReadBytesOrWait(buffer, strlen(MAC_MESSAGE), 1000000);
	if (readBytes != strlen(MAC_MESSAGE)) {
		bConnected = false;
		bMacReceived = false;
	}
	else {
		bMacReceived = true;
	}

	if (!bConnected) {
		pSerial->SendData("b", 1);
	}

	if (bMacReceived) {
		buffer[readBytes] = '\0';
		sscanf_s(buffer, "mac: %x-%x-%x-%x-%x-%x\r\n",
			&mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5]);

		pDlg->_parent->m_strMac.Format(_T("%2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x"),
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		pDlg->OnApConnect();
	}
	else {
		pDlg->OnApConnectFail();
	}

	return 0;
}


void CApSelDialog::OnBnClickedSetapBtn()
{
	// TODO: Add your control notification handler code here
	CSerial* pSerial = _parent->m_pSerial;
	char buffer[MAX_INIT_STRING_LEN];
	CString apNameStr, passwordStr;
	char apNameBuffer[100], passwordBuffer[100];
	bool bFound = false;

	m_ApNameEdit.GetWindowText(apNameStr);
	if (apNameStr.GetLength() == 0) {
		MessageBox(_T("SSID�� �Է����ּ���"), _T("����"), MB_ICONERROR | MB_OK);
		return;
	}
	m_ApPasswordEdit.GetWindowText(passwordStr);
	if (m_SecurityCB.GetCurSel() != 0  && passwordStr.GetLength() == 0) {
		MessageBox(_T("��ȣ�� �Է����ּ���"), _T("����"), MB_ICONERROR | MB_OK);
		return;
	}

	WideCharToMultiByte(CP_UTF8, 0, apNameStr, -1, apNameBuffer, 100, NULL, NULL);
	WideCharToMultiByte(CP_UTF8, 0, passwordStr, -1, passwordBuffer, 100, NULL, NULL);

	bFound = true;
	do {
		int readBytes = pSerial->ReadBytesOrWait(buffer, strlen(SET_OR_SCAN_MESSAGE), 1000);
		if (strlen(SET_OR_SCAN_MESSAGE) != readBytes) {
			bFound = false;
			break;
		}
		else if (strncmp(SET_OR_SCAN_MESSAGE, buffer, strlen(SET_OR_SCAN_MESSAGE))) {
			bFound = false;
			break;
		}

		pSerial->SendData("a", 1);

		readBytes = pSerial->ReadBytesOrWait(buffer, strlen(SSID_MESSAGE), 1000);
		if (strlen(SSID_MESSAGE) != readBytes) {
			bFound = false;
			break;
		}
		else if (strncmp(SSID_MESSAGE, buffer, strlen(SSID_MESSAGE))) {
			bFound = false;
			break;
		}

		pSerial->SendData(apNameBuffer, strlen(apNameBuffer));
		pSerial->SendData("\r\n", 2);

		readBytes = pSerial->ReadBytesOrWait(buffer, strlen(SECURITY_MESSAGE), 1000);
		if (strlen(SECURITY_MESSAGE) != readBytes) {
			bFound = false;
			break;
		}
		else if (strncmp(SECURITY_MESSAGE, buffer, strlen(SECURITY_MESSAGE))) {
			bFound = false;
			break;
		}

		char temp = '0' + m_SecurityCB.GetCurSel();
		pSerial->SendData(&temp, 1);
		pSerial->SendData("\r\n", 2);

		if (m_SecurityCB.GetCurSel() != 0) {
			readBytes = pSerial->ReadBytesOrWait(buffer, strlen(PASSWORD_MESSAGE), 1000);
			if (strlen(PASSWORD_MESSAGE) != readBytes) {
				bFound = false;
				break;
			}
			else if (strncmp(PASSWORD_MESSAGE, buffer, strlen(PASSWORD_MESSAGE))) {
				bFound = false;
				break;
			}

			pSerial->SendData(passwordBuffer, strlen(passwordBuffer));
			pSerial->SendData("\r\n", 2);
		}

		readBytes = pSerial->ReadBytesOrWait(buffer, strlen(CONNECTING_MESSAGE), 1000);
		if (strlen(CONNECTING_MESSAGE) != readBytes) {
			bFound = false;
			break;
		}
		else if (strncmp(CONNECTING_MESSAGE, buffer, strlen(CONNECTING_MESSAGE))) {
			bFound = false;
			break;
		}
	} while (false);

	if (!bFound) {
		MessageBox(_T("���������� ��ſ� �����߽��ϴ�."), _T("����"), MB_ICONERROR | MB_OK);
		return;
	}

	//disable controls
	m_ApNameEdit.EnableWindow(false);
	m_ApPasswordEdit.EnableWindow(false);
	m_ScanApBtn.EnableWindow(false);
	m_SecurityCB.EnableWindow(false);
	m_SetApBtn.EnableWindow(false);

	m_ProgressCtrl.SetMarquee(true, 50);
	m_ProgressCtrl.ShowWindow(SW_SHOWNORMAL);

	//Start connection check thread
	AfxBeginThread(ConnectMonitor, (LPVOID)this);
}


void CApSelDialog::OnBnClickedScanapBtn()
{
	// TODO: Add your control notification handler code here
	_parent->ToScanWindow();
}


int CApSelDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	

	return 0;
}


void CApSelDialog::OnCbnSelchangeSecurityCombo()
{
	// TODO: Add your control notification handler code here
	int selection = m_SecurityCB.GetCurSel();

	if (selection != 0)
		m_ApPasswordEdit.ShowWindow(SW_SHOWNORMAL);
	else 
		m_ApPasswordEdit.ShowWindow(SW_HIDE);
}


BOOL CApSelDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	m_ApPasswordEdit.ShowWindow(SW_HIDE);

	COMBOBOXEXITEM  item;
	ZeroMemory(&item, sizeof(item));
	item.mask = CBEIF_TEXT;
	item.iItem = 0;
	item.pszText = _T("WPA2");
	m_SecurityCB.InsertItem(&item);
	ZeroMemory(&item, sizeof(item));
	item.mask = CBEIF_TEXT;
	item.iItem = 0;
	item.pszText = _T("WPA1");
	m_SecurityCB.InsertItem(&item);
	ZeroMemory(&item, sizeof(item));
	item.mask = CBEIF_TEXT;
	item.iItem = 0;
	item.pszText = _T("WEP");
	m_SecurityCB.InsertItem(&item);
	ZeroMemory(&item, sizeof(item));
	item.mask = CBEIF_TEXT;
	item.iItem = 0;
	item.pszText = _T("Open");
	m_SecurityCB.InsertItem(&item);

	m_SecurityCB.SetCurSel(0);

	m_ProgressCtrl.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CApSelDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);
	
	
}


void CApSelDialog::OnApConnect()
{
	_parent->NextProcess(this);
}


void CApSelDialog::OnApConnectFail()
{
	m_ApNameEdit.EnableWindow(true);
	m_ApPasswordEdit.EnableWindow(true);
	m_ScanApBtn.EnableWindow(true);
	m_SecurityCB.EnableWindow(true);
	m_SetApBtn.EnableWindow(true);

	m_ProgressCtrl.SetMarquee(false, 50);
	m_ProgressCtrl.ShowWindow(SW_HIDE);

	MessageBox(_T("AP ���ӿ� �����߽��ϴ�."), _T("����"), MB_ICONERROR | MB_OK);
}
