#ifndef VPRCTRL_H
#define  VPRCTRL_H
#include "VPR_commondef.h"

// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� VPRCTRL_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// VPRCTRL_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifdef VPRCTRL_EXPORTS
#define VPRCTRL_API EXTERN_C __declspec(dllexport)
#else
#define VPRCTRL_API EXTERN_C __declspec(dllimport)
#endif



//fnGetVehicleInfo�� ��ͨ���ݲ��������ṹ����
//B.2.2  ��ʼ��
VPRCTRL_API int DLL_DEC VPR_Init();

//B.2  �ӿ�
//B.2.1  ע�����ݻص�����
VPRCTRL_API int DLL_DEC VPR_RegCallBack(VPR_GetVehicleInfo fnGetVehicleInfo);

//B.2.3  �豸״̬��ȡ
//chTSIP�� �ն��豸IP
//pDevState���豸״̬���������ṹ����
VPRCTRL_API int DLL_DEC VPR_GetDeviceState(char *chTSIP, LPSTDEVSTATE pDevState);


//B.2.4  �豸Уʱ
//chTSIP�� �ն��豸IP
VPRCTRL_API  int DLL_DEC VPR_AdjustTime(char *chTSIP);


//B.2.5  �ر�
VPRCTRL_API int DLL_DEC VPR_Quit();

#endif