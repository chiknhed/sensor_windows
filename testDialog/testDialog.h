
// testDialog.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.

#define SERVER_ADDRESS "galvanic-cirrus-841.appspot.com"

// CtestDialogApp:
// �� Ŭ������ ������ ���ؼ��� testDialog.cpp�� �����Ͻʽÿ�.
//

class CtestDialogApp : public CWinApp
{
public:
	CtestDialogApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CtestDialogApp theApp;