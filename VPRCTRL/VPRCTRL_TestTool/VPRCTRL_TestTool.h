
// VPRCTRL_TestTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CVPRCTRL_TestToolApp: 
// �йش����ʵ�֣������ VPRCTRL_TestTool.cpp
//

class CVPRCTRL_TestToolApp : public CWinApp
{
public:
	CVPRCTRL_TestToolApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CVPRCTRL_TestToolApp theApp;