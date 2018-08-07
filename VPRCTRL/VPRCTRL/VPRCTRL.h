#ifndef VPRCTRL_H
#define  VPRCTRL_H
#include "VPR_commondef.h"

// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 VPRCTRL_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// VPRCTRL_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef VPRCTRL_EXPORTS
#define VPRCTRL_API EXTERN_C __declspec(dllexport)
#else
#define VPRCTRL_API EXTERN_C __declspec(dllimport)
#endif



//fnGetVehicleInfo： 交通数据参数，见结构定义
//B.2.2  初始化
VPRCTRL_API int DLL_DEC VPR_Init();

//B.2  接口
//B.2.1  注册数据回调函数
VPRCTRL_API int DLL_DEC VPR_RegCallBack(VPR_GetVehicleInfo fnGetVehicleInfo);

//B.2.3  设备状态获取
//chTSIP： 终端设备IP
//pDevState：设备状态参数，见结构定义
VPRCTRL_API int DLL_DEC VPR_GetDeviceState(char *chTSIP, LPSTDEVSTATE pDevState);


//B.2.4  设备校时
//chTSIP： 终端设备IP
VPRCTRL_API  int DLL_DEC VPR_AdjustTime(char *chTSIP);


//B.2.5  关闭
VPRCTRL_API int DLL_DEC VPR_Quit();

#endif