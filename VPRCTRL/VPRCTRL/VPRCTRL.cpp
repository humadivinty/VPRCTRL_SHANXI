// VPRCTRL.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "VPRCTRL.h"
#include "utilityTool/ToolFunction.h"
#include "Camera/Camera6467_plate.h"
#include <memory>

#define TOOL_CHECK_ARG(arg)\
if (arg == NULL) \
    {\
    Tool_WriteFormatLog("%s is NULL", #arg); \
    return 0; \
    }

std::shared_ptr<Camera6467_plate> g_Camera_Plate;
VPR_GetVehicleInfo g_FuncGetVehicleInfo = NULL;


VPRCTRL_API int DLL_DEC VPR_Init()
{
    Tool_WriteFormatLog("VPR_Init, begin.");

    char chTemp[MAX_PATH] = { 0 };
    sprintf_s(chTemp, sizeof(chTemp), "1.1.1.1");
    Tool_ReadKeyValueFromConfigFile(INI_FILE_NAME, "PlateCamera", "ip", chTemp, sizeof(chTemp));
    Tool_WriteFormatLog("VPR_Init, get the plate Camera ip address  %s .", chTemp);

    std::string strPlateIp(chTemp);
    if (g_Camera_Plate != NULL
        && 0 == strcmp(strPlateIp.c_str(), g_Camera_Plate->GetCameraIP())
        )
    {
        Tool_WriteFormatLog("VPR_Init, the camera %s is already connecting, return true.", strPlateIp.c_str());
        return 0;
    }

    if (strlen(chTemp)> 0 
        && 0 != strcmp(chTemp, "1.1.1.1")
        )
    {
        Tool_WriteFormatLog("create plate camera.");
        g_Camera_Plate = std::make_shared<Camera6467_plate>();
        g_Camera_Plate->SetCameraIP(chTemp);
        int  iConnect = g_Camera_Plate->ConnectToCamera();
        if (g_Camera_Plate && NULL != g_FuncGetVehicleInfo)
        {
            g_Camera_Plate->SetUpLoaderCallback(g_FuncGetVehicleInfo);
        }
        Tool_WriteFormatLog("create plate camera finish, connect code = %d.", iConnect);
    }
    else
    {
        Tool_WriteFormatLog("please set camera ipAddress first.", chTemp);
    }

    Tool_WriteFormatLog("VPR_Init, finish.");
    return 0;
}

VPRCTRL_API int DLL_DEC VPR_RegCallBack(VPR_GetVehicleInfo fnGetVehicleInfo)
{
    Tool_WriteFormatLog("VPR_RegCallBack, begin ,VPR_GetVehicleInfo= %p .", fnGetVehicleInfo);
    g_FuncGetVehicleInfo = fnGetVehicleInfo;
    if (g_Camera_Plate)
    {
        g_Camera_Plate->SetUpLoaderCallback(fnGetVehicleInfo);
    }

    Tool_WriteFormatLog("VPR_RegCallBack, finish, return 0.");
    return 0;
}

VPRCTRL_API int DLL_DEC VPR_GetDeviceState(char *chTSIP, LPSTDEVSTATE pDevState)
{
    Tool_WriteFormatLog("VPR_GetDeviceState, begin ,chTSIP= %p .", chTSIP);
    int iRet = -1;
    if (g_Camera_Plate
        && 0 == strcmp(g_Camera_Plate->GetCameraIP(), chTSIP)
        )
    {
        //iRet = g_Camera_Plate->GetIttsStatus(pDevState);
        g_Camera_Plate->CopyCusStatus(pDevState);
        iRet = 0;
    }
    else
    {
        Tool_WriteFormatLog("VPR_GetDeviceState, camera not connect or ipAddress is no the same with it.");
    }

    Tool_WriteFormatLog("VPR_GetDeviceState, finish, return code = %d.", iRet);
    return iRet;
}

VPRCTRL_API int DLL_DEC VPR_AdjustTime(char *chTSIP)
{
    Tool_WriteFormatLog("VPR_AdjustTime, begin ,chTSIP= %p .", chTSIP);
    bool bRet = false;
    if (g_Camera_Plate 
        && 0 == strcmp(g_Camera_Plate->GetCameraIP() , chTSIP) 
        )
    {
        bRet = g_Camera_Plate->SynTime();
    }
    else
    {
        Tool_WriteFormatLog("VPR_AdjustTime, camera not connect or ipAddress is no the same with it.");
    }

    Tool_WriteFormatLog("VPR_AdjustTime, finish.");
    return (bRet ? 0 : -1);
}

VPRCTRL_API int DLL_DEC VPR_Quit()
{
    Tool_WriteFormatLog("VPR_Quit, begin .");
    if (g_Camera_Plate)
    {
        g_Camera_Plate = NULL;
    }

    Tool_WriteFormatLog("VPR_Quit, finish.");
    return 0;
}
