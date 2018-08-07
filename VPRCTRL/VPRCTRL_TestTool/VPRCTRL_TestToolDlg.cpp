
// VPRCTRL_TestToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "VPRCTRL_TestTool.h"
#include "VPRCTRL_TestToolDlg.h"
#include "afxdialogex.h"
#include "ToolFunction.h"

#include "../VPRCTRL/VPRCTRL.H"
#ifdef DEBUG
#pragma comment(lib, "../debug/VPRCTRL.lib")
#else
#pragma comment(lib, "../release/VPRCTRL.lib")
#endif


#ifdef _DEBUG
#define new DEBUG_NEW
#endif



static void CALLBACK CallbackFunc_GetVehicleInfo(LPTRAFFICDATA pTrafficData)
{
    time_t timeResult = pTrafficData->dwPassTime;
    tm pTime;
    localtime_s(&pTime, &timeResult);
    char chTime[256] = { 0 };
    sprintf_s(chTime, sizeof(chTime), "%04d-%02d-%02d_%02d_%02d_%02d",
        pTime.tm_year + 1900,
        pTime.tm_mon + 1,
        pTime.tm_mday,
        pTime.tm_hour,
        pTime.tm_min,
        pTime.tm_sec);


    char szFilePath[MAX_PATH] = {0};    
    sprintf_s(szFilePath, sizeof(szFilePath), "%s\\CallbackResult\\%04d-%02d-%02d-%02d\\", Tool_GetCurrentPath(),
        pTime.tm_year + 1900, 
        pTime.tm_mon + 1, 
        pTime.tm_mday, 
        pTime.tm_hour);
    MakeSureDirectoryPathExists(szFilePath);
    
    DWORD dwTick = GetTickCount();

    char szTextFileName[MAX_PATH * 2] = { 0 };
    sprintf_s(szTextFileName, sizeof(szTextFileName), "%s\\%s-%s-%lu-trafficData.txt", szFilePath, chTime, pTrafficData->chPlate, dwTick);

    char szBinImgName[MAX_PATH * 2] = { 0 };
    sprintf_s(szBinImgName, sizeof(szBinImgName), "%s\\%s-%s-%lu-trafficData.bin", szFilePath, chTime, pTrafficData->chPlate, dwTick);

    char szPlateImgName[MAX_PATH * 2] = { 0 };
    sprintf_s(szPlateImgName, sizeof(szPlateImgName), "%s\\%s-%s-%lu-trafficData-plate.jpg", szFilePath, chTime, pTrafficData->chPlate, dwTick);

    char szjpg1ImgName[MAX_PATH * 2] = { 0 };
    sprintf_s(szjpg1ImgName, sizeof(szjpg1ImgName), "%s\\%s-%s-%lu-trafficData-jpeg1.jpg", szFilePath, chTime, pTrafficData->chPlate, dwTick);

    char szjpg2ImgName[MAX_PATH * 2] = { 0 };
    sprintf_s(szjpg2ImgName, sizeof(szjpg2ImgName), "%s\\%s-%s-%lu-trafficData-jpeg2.jpg", szFilePath, chTime, pTrafficData->chPlate, dwTick);

    char szjpg3ImgName[MAX_PATH * 2] = { 0 };
    sprintf_s(szjpg3ImgName, sizeof(szjpg3ImgName), "%s\\%s-%s-%lu-trafficData-jpeg3.jpg", szFilePath, chTime, pTrafficData->chPlate, dwTick);

    static char chBuffer[512] = { 0 };
    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "dwVersion = %lu.", pTrafficData->dwVersion);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "chTSIP = %s.", pTrafficData->chTSIP);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "chPlate = %s.", pTrafficData->chPlate);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "chPlateColor = %d.", pTrafficData->chPlateColor);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "dwLaneNum = %d.", pTrafficData->dwLaneNum);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "dwSpeed = %d.", pTrafficData->dwSpeed);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "dwPassTime = %lu.", pTrafficData->dwPassTime);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "dwDir = %lu.", pTrafficData->dwDir);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "dwDataType = %lu.", pTrafficData->dwDataType);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "dwDefense = %lu.", pTrafficData->dwDefense);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "dwCameraID = %lu.", pTrafficData->dwCameraID);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "dwPlateType = %lu.", pTrafficData->dwPlateType);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "dwVehicleColor = %lu.", pTrafficData->dwVehicleColor);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "dwPlateImgLen = %lu.", pTrafficData->dwPlateImgLen);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "dwCarJpegLen1 = %lu.", pTrafficData->dwCarJpegLen1);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "dwCarJpegLen2 = %lu.", pTrafficData->dwCarJpegLen2);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "dwCarJpegLen3 = %lu.", pTrafficData->dwCarJpegLen3);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "byVehicleType = %d.", pTrafficData->byVehicleType);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "byVehicleKind = %d.", pTrafficData->byVehicleKind);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "byColorDepth = %d.", pTrafficData->byColorDepth);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "byColor = %d.", pTrafficData->byColor);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "byRes1 = %d.", pTrafficData->byRes1);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "wLength = %d.", pTrafficData->wLength);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "byVehicleLogoRecog = %d.", pTrafficData->byVehicleLogoRecog);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "byVehicleSubLogoRecog = %s.", pTrafficData->byVehicleSubLogoRecog);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "byVehicleModel = %s.", pTrafficData->byVehicleModel);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer,sizeof(chBuffer), "byExhaust = %s.", pTrafficData->byExhaust);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer, sizeof(chBuffer), "byCustomInfo = %s.", pTrafficData->byCustomInfo);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    memset(chBuffer, '\0', sizeof(chBuffer));
    sprintf_s(chBuffer, sizeof(chBuffer), "byRes3 = %s.", pTrafficData->byRes3);
    Tool_AppenTextToFile(szTextFileName, chBuffer);

    Tool_SaveFileToPath(szBinImgName, pTrafficData->byBinImg, BINARY_IMG_SIZE);

    if (pTrafficData->dwPlateImgLen > 0)
    {
        Tool_SaveFileToPath(szPlateImgName, pTrafficData->pPlateJpegImg, pTrafficData->dwPlateImgLen);
    }

    if (pTrafficData->dwCarJpegLen1 > 0)
    {
        Tool_SaveFileToPath(szjpg1ImgName, pTrafficData->pCarJpegImg1, pTrafficData->dwCarJpegLen1);
    }

    if (pTrafficData->dwCarJpegLen2 > 0)
    {
        Tool_SaveFileToPath(szjpg2ImgName, pTrafficData->pCarJpegImg2, pTrafficData->dwCarJpegLen2);
    }

    if (pTrafficData->dwCarJpegLen3 > 0)
    {
        Tool_SaveFileToPath(szjpg3ImgName, pTrafficData->pCarJpegImg3, pTrafficData->dwCarJpegLen3);
    }
}

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CVPRCTRL_TestToolDlg 对话框



CVPRCTRL_TestToolDlg::CVPRCTRL_TestToolDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVPRCTRL_TestToolDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CVPRCTRL_TestToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CVPRCTRL_TestToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_BUTTON_SetIP, &CVPRCTRL_TestToolDlg::OnBnClickedButtonSetip)
    ON_BN_CLICKED(IDC_BUTTON_init, &CVPRCTRL_TestToolDlg::OnBnClickedButtoninit)
    ON_BN_CLICKED(IDC_BUTTON_RegCallBack, &CVPRCTRL_TestToolDlg::OnBnClickedButtonRegcallback)
    ON_BN_CLICKED(IDC_BUTTON_GetDeviceState, &CVPRCTRL_TestToolDlg::OnBnClickedButtonGetdevicestate)
    ON_BN_CLICKED(IDC_BUTTON_AdjustTime, &CVPRCTRL_TestToolDlg::OnBnClickedButtonAdjusttime)
    ON_BN_CLICKED(IDC_BUTTON_QUIT, &CVPRCTRL_TestToolDlg::OnBnClickedButtonQuit)
END_MESSAGE_MAP()


// CVPRCTRL_TestToolDlg 消息处理程序

BOOL CVPRCTRL_TestToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CVPRCTRL_TestToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CVPRCTRL_TestToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CVPRCTRL_TestToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CVPRCTRL_TestToolDlg::OnBnClickedButtonSetip()
{
    // TODO:  在此添加控件通知处理程序代码
    CString cstrIP;
    GetDlgItem(IDC_IPADDRESS1)->GetWindowText(cstrIP);

    char iniFileName[MAX_PATH] = { 0 };
    sprintf_s(iniFileName, sizeof(iniFileName), "%s\\XLW_VPRCTRL.ini", Tool_GetCurrentPath());

    WritePrivateProfileStringA("PlateCamera", "ip", cstrIP.GetBuffer(), iniFileName);
    cstrIP.ReleaseBuffer();

    char szLog[MAX_PATH] = { 0 };
    sprintf_s(szLog, sizeof(szLog), "set ip %s to config file .", cstrIP.GetBuffer());
    cstrIP.ReleaseBuffer();
    MessageBox(szLog);    
}


void CVPRCTRL_TestToolDlg::OnBnClickedButtoninit()
{
    // TODO:  在此添加控件通知处理程序代码
    int iRet = VPR_Init();
    char szLog[MAX_PATH] = {0};
    sprintf_s(szLog, sizeof(szLog), "VPR_Init , return code = %d", iRet);
    MessageBox(szLog);
}


void CVPRCTRL_TestToolDlg::OnBnClickedButtonRegcallback()
{
    // TODO:  在此添加控件通知处理程序代码
    int iRet = VPR_RegCallBack(CallbackFunc_GetVehicleInfo);
    char szLog[MAX_PATH] = { 0 };
    sprintf_s(szLog, sizeof(szLog), "VPR_RegCallBack ,CallbackFunc_GetVehicleInfo = %p,  return code = %d",
        CallbackFunc_GetVehicleInfo, iRet);
    MessageBox(szLog);
}


void CVPRCTRL_TestToolDlg::OnBnClickedButtonGetdevicestate()
{
    // TODO:  在此添加控件通知处理程序代码
    CString cstrIP;
    GetDlgItem(IDC_IPADDRESS1)->GetWindowText(cstrIP);

    STDEVSTATE DevState;
    int iRet = VPR_GetDeviceState(cstrIP.GetBuffer(), &DevState);
    cstrIP.ReleaseBuffer();
    
    char szTextFileName[MAX_PATH] = { 0 };
    sprintf_s(szTextFileName, sizeof(szTextFileName), "%s\\%lu-status.txt", Tool_GetCurrentPath(), GetTickCount());

    static char szLog[2018] = { 0 };

    FILE* pFile = NULL;
    fopen_s(&pFile, szTextFileName, "wb");
    if (pFile)
    {
        memset(szLog, '\0', sizeof(szLog));
        memcpy(szLog, DevState.chIP, 20);
        fprintf(pFile, "chIP: %s\n", szLog);

        fprintf(pFile, "dwServerStatus: %d\n", DevState.dwServerStatus);
        fprintf(pFile, "dwCameraNum: %d\n", DevState.dwCameraNum);

        memset(szLog, '\0', sizeof(szLog));
        memcpy(szLog, DevState.chCamIP, 2000);
        fprintf(pFile, "chCamIP: %s\n", szLog);

        fprintf(pFile, "byCamStatus: \n");
        for (int i = 0; i < 100; i++)
        {
            fprintf(pFile, "%d", DevState.byCamStatus[i]);
        }
        fprintf(pFile, "\nbyVehicleSensorStatus: \n");
        for (int i = 0; i < 100; i++)
        {
            fprintf(pFile, "%d", DevState.byVehicleSensorStatus[i]);
        }
        fprintf(pFile, "\nbyLoopStatus: \n");
        for (int i = 0; i < 100; i++)
        {
            fprintf(pFile, "%d", DevState.byLoopStatus[i]);
        }

        fclose(pFile);
        pFile = NULL;
    }
    
    sprintf_s(szLog, sizeof(szLog), "VPR_GetDeviceState , return code = %d", iRet);
    MessageBox(szLog);
}


void CVPRCTRL_TestToolDlg::OnBnClickedButtonAdjusttime()
{
    // TODO:  在此添加控件通知处理程序代码
    CString cstrIP;
    GetDlgItem(IDC_IPADDRESS1)->GetWindowText(cstrIP);

    int iRet = VPR_AdjustTime(cstrIP.GetBuffer());
    cstrIP.ReleaseBuffer();
    char szLog[MAX_PATH] = { 0 };
    sprintf_s(szLog, sizeof(szLog), "VPR_AdjustTime(%s) , return code = %d", cstrIP.GetBuffer(), iRet);
    cstrIP.ReleaseBuffer();
    MessageBox(szLog);
}


void CVPRCTRL_TestToolDlg::OnBnClickedButtonQuit()
{
    // TODO:  在此添加控件通知处理程序代码
    int iRet = VPR_Quit();
    char szLog[MAX_PATH] = { 0 };
    sprintf_s(szLog, sizeof(szLog), "VPR_Quit , return code = %d", iRet);
    MessageBox(szLog);
}
