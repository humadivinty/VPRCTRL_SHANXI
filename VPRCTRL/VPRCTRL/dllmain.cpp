// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "utilityTool/log4z.h"
#include "utilityTool/ToolFunction.h"

static char chCurrenPath[MAX_PATH] = {0};

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
        memset(chCurrenPath, '\0', sizeof(chCurrenPath));
        sprintf_s(chCurrenPath, sizeof(chCurrenPath), "%s\\XLWLog\\", Tool_GetCurrentPath());
        zsummer::log4z::ILog4zManager::getRef().setLoggerPath(LOG4Z_MAIN_LOGGER_ID, chCurrenPath);
        zsummer::log4z::ILog4zManager::getRef().start();
        zsummer::log4z::ILog4zManager::getRef().setLoggerLevel(LOG4Z_MAIN_LOGGER_ID, LOG_LEVEL_TRACE);
        break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

