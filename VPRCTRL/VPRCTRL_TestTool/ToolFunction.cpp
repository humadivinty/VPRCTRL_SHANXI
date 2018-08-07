#include "stdafx.h"
#include "ToolFunction.h"
#include <string>

bool Tool_AppenTextToFile(const char* fileName, const char* szText)
{
    FILE *file = NULL;
    //file = fopen(fileName, "a+");
    fopen_s(&file, fileName, "a+");
    if (file)
    {
        //SYSTEMTIME systime;
        //GetLocalTime(&systime);//本地时间

        //fprintf(file, "%04d-%02d-%02d %02d:%02d:%02d:%03d : %s\n",
        //    systime.wYear,
        //    systime.wMonth,
        //    systime.wDay,
        //    systime.wHour,
        //    systime.wMinute,
        //    systime.wSecond,
        //    systime.wMilliseconds,
        //    szText);

        fprintf(file, "%s\n", szText);

        fclose(file);
        file = NULL;
        return true;
    }
    return false;
}

const TCHAR* Tool_GetCurrentPath()
{
    static TCHAR szPath[256] = { 0 };
    if (strlen(szPath) <= 0)
    {
        GetModuleFileName(NULL, szPath, MAX_PATH - 1);
        PathRemoveFileSpec(szPath);
    }
    return szPath;
}

bool Tool_SaveFileToPath(const char* szPath, void* fileData, size_t fileSize)
{
    printf("begin SaveImgToDisk");
    if (NULL == fileData)
    {
        printf("end1 SaveImgToDisk");
        return false;
    }
    bool bRet = false;
    size_t iWritedSpecialSize = 0;
    std::string tempFile(szPath);
    size_t iPosition = tempFile.rfind("\\");
    std::string tempDir = tempFile.substr(0, iPosition + 1);
    if (MakeSureDirectoryPathExists(tempDir.c_str()))
    {
        FILE* fp = NULL;
        //fp = fopen(chImgPath, "wb+");
        fopen_s(&fp, szPath, "wb+");
        if (fp)
        {
            //iWritedSpecialSize = fwrite(pImgData, dwImgSize , 1, fp);
            iWritedSpecialSize = fwrite(fileData, 1, fileSize, fp);
            fclose(fp);
            fp = NULL;
            bRet = true;
        }
        if (iWritedSpecialSize == fileSize)
        {
            char chLogBuff[MAX_PATH] = { 0 };
            //sprintf_s(chLogBuff, "%s save success", chImgPath);
            sprintf_s(chLogBuff, sizeof(chLogBuff), "%s save success", szPath);
            printf(chLogBuff);
        }
    }
    else
    {
        char chLogBuff[MAX_PATH] = { 0 };
        //sprintf_s(chLogBuff, "%s save failed", chImgPath);
        sprintf_s(chLogBuff, sizeof(chLogBuff), "%s save failed", szPath);
        printf(chLogBuff);
        bRet = false;
    }
    printf("end SaveImgToDisk");
    return bRet;
}