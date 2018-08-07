#include "stdafx.h"
#include "ResultUploader.h"
#include "utilityTool/ToolFunction.h"
#include"utilityTool/log4z.h"
#include "VPR_commondef.h"

unsigned int __stdcall Result_FileSearchThread(LPVOID lpParam);
unsigned int __stdcall Result_UploadResultThread(LPVOID lpParam);

ResultUploader::ResultUploader() :
m_pCallBackFunc(NULL),
m_pUserData(NULL),
m_hSearchThread(NULL),
m_hUploadThread(NULL),
m_bCheckExit(false)
{
    Tool_LoadCamerXml(CAMERA_XML_NAME, m_CameraIPMap);
    LoadCarfaceXml(CARFACE_XML_NAME, m_CarFaceMap);
    memset(m_chUploadPath, '\0', sizeof(m_chUploadPath));
    InitializeCriticalSection(&m_cs);
}

ResultUploader::~ResultUploader()
{
    SetCheckThreadExit(true);
    Tool_SafeCloseThread(m_hSearchThread);
    Tool_SafeCloseThread(m_hUploadThread);

    DeleteCriticalSection(&m_cs);
}

void ResultUploader::startUpLoad()
{
    if (NULL == m_hSearchThread)
    {
        m_hSearchThread = (HANDLE)_beginthreadex(NULL, 0, Result_FileSearchThread, this, 0, NULL);
    }

    if (NULL == m_hUploadThread)
    {
        m_hUploadThread = (HANDLE)_beginthreadex(NULL, 0, Result_UploadResultThread, this, 0, NULL);
    }
}

void ResultUploader::SetUploadPath(const char* uploadPath)
{
    MySafeLocker locker(&m_cs);
    if (NULL != uploadPath
        && strlen(uploadPath) < sizeof(m_chUploadPath))
    {
        strcpy_s(m_chUploadPath, sizeof(m_chUploadPath), uploadPath);
    }
}

void ResultUploader::SetUploadCallBackFunc(void* callbackFunc, void* userData)
{
    MySafeLocker locker(&m_cs);
    m_pCallBackFunc = callbackFunc;
    m_pUserData = userData;
}

void ResultUploader::UpLoadData()
{
    std::string strFileName;
    int iTryTime = 0;
    while (!GetCheckThreadExit())
    {
        if (m_lsFileNameLsit.empty())
        {
            Sleep(50);
            continue;
        }

        //if (m_lsFileNameLsit.size() < 2 && iTryTime++ < 5)
        //{
        //    Sleep(1000);
        //    continue;
        //}
        //else
        //{
        //    iTryTime = 0;
        //}

        strFileName = m_lsFileNameLsit.front();
        m_lsFileNameLsit.pop_front();
        {
            MySafeLocker locker(&m_cs);
            if (m_pCallBackFunc == NULL
                || !Tool_IsFileExist(strFileName.c_str())
                )
            {
                Sleep(50);
                continue;
            }

            std::shared_ptr<CameraResult> pResult = DeSerializationResult(strFileName.c_str());
            if (pResult)
            {
                LOGFMTD("UpLoadResultByCallBack��begin.");
                UpLoadResultByCallBack(pResult);
                LOGFMTD("UpLoadResultByCallBack��finish.");
            }
        }
        Sleep(50);
        continue;
    }
}

bool ResultUploader::UpLoadResultByCallBack(std::shared_ptr<CameraResult> pResult)
{
    if (NULL == pResult)
    {
        return false;
    }

    if (m_pCallBackFunc)
    {
        TRAFFICDATA trafficData;

        ////todo �������
        trafficData.dwVersion = 1;
        sprintf_s(trafficData.chTSIP, sizeof(trafficData.chTSIP), "%s", pResult->chServerIP);
        sprintf_s(trafficData.chPlate, sizeof(trafficData.chPlate), "%s", pResult->chPlateNO);
        trafficData.chPlateColor = pResult->iPlateColor;                //������ɫ
        if (pResult->CIMG_BinImage.dwImgSize > 0 && NULL != pResult->CIMG_BinImage.pbImgData)
        {
            memcpy(trafficData.byBinImg, pResult->CIMG_BinImage.pbImgData, BINARY_IMG_SIZE);        //��ֵͼ
        }
        else
        {
            memset(trafficData.byBinImg, 0, BINARY_IMG_SIZE);       //��ֵͼ
        }
        trafficData.dwLaneNum = pResult->iRoadNo;                   //�������
        trafficData.dwSpeed = pResult->iSpeed;                          //�г��ٶ�
        trafficData.dwPassTime = pResult->dw64TimeMS;        //����ʱ��
        trafficData.dwDir = pResult->iDirection;                    //����(0)�����У�1�����ɶ�����(2)��������(3)��������(4)���ɱ�����(5)
        trafficData.dwDataType = 0;     //��������
        trafficData.dwDefense = 0;      //Υ�´���
        if (m_CameraIPMap.size() > 0 
            && m_CameraIPMap.find(pResult->chDeviceIp) != m_CameraIPMap.end())
        {
            trafficData.dwCameraID = m_CameraIPMap.find(pResult->chDeviceIp)->second;    //������
        }
        else
        {
            trafficData.dwCameraID = 0;         //������
        }
        
        trafficData.dwPlateType = pResult->iPlateTypeNo;         //��������        
        trafficData.dwVehicleColor = 0;                                     //������ɫ

        trafficData.dwPlateImgLen = pResult->CIMG_PlateImage.dwImgSize;     //����ͼ����
        if (pResult->CIMG_PlateImage.dwImgSize > 0)
        {
            trafficData.pPlateJpegImg = pResult->CIMG_PlateImage.pbImgData;
        }
        else
        {
            trafficData.pPlateJpegImg = NULL;
        }

        trafficData.dwCarJpegLen1 = pResult->CIMG_BestSnapshot.dwImgSize;     //����JPEGͼ1����
        if (pResult->CIMG_BestSnapshot.dwImgSize > 0)
        {
            trafficData.pCarJpegImg1 = pResult->CIMG_BestSnapshot.pbImgData;
        }
        else
        {
            trafficData.pCarJpegImg1 = NULL;
        }

        trafficData.dwCarJpegLen2 = pResult->CIMG_LastSnapshot.dwImgSize;     //����JPEGͼ2����
        if (pResult->CIMG_LastSnapshot.dwImgSize > 0)
        {
            trafficData.pCarJpegImg2 = pResult->CIMG_LastSnapshot.pbImgData;
        }
        else
        {
            trafficData.pCarJpegImg2 = NULL;
        }

        trafficData.dwCarJpegLen3 = pResult->CIMG_BestCapture.dwImgSize;      //����JPEGͼ3����
        if (pResult->CIMG_BestCapture.dwImgSize > 0)
        {
            trafficData.pCarJpegImg3 = pResult->CIMG_BestCapture.pbImgData;
        }
        else
        {
            trafficData.pCarJpegImg3 = NULL;
        }

        trafficData.byVehicleType = pResult->iVehSizeType;        //��������
        trafficData.byVehicleKind = pResult->iVehTypeNo;       //�������� ��0-δ֪��1�ͳ���2����
        trafficData.byColorDepth = 0;        //������ɫ��ǳ��0-��ɫ��1-ǳɫ 
        trafficData.byColor = 0xff;                 //������ɫ 0������ɫ��1����ɫ��2����(��)��3����(��)��4����ɫ��5����ɫ��6��������7����ɫ��8����ɫ��9����ɫ��10����ɫ��11.:��ɫ12: ��ɫ
        trafficData.byRes1 = 0;                     //����
        trafficData.wLength = 0;                     //������
        trafficData.byVehicleLogoRecog = pResult->iVehLogoType;                     //����Ʒ��
        memcpy(trafficData.byVehicleSubLogoRecog, pResult->chChileLogo, 31);
        trafficData.byVehicleSubLogoRecog[31] = '\0';              //������Ʒ��
        trafficData.byVehicleModel = 0;                                 //������Ʒ�����
        trafficData.byExhaust[0] = 0;                                     //�����ŷ���Ϣ  
        trafficData.byCustomInfo[0] = 0;                                //�Զ�����Ϣ
        trafficData.byRes3[0] = 0;                                              //����

        ((VPR_GetVehicleInfo)m_pCallBackFunc)(&trafficData);

        memset(&trafficData, 0, sizeof(trafficData));
    }
    return true;
}

std::shared_ptr<CameraResult> ResultUploader::DeSerializationResult(const char* fileName)
{
    LOGFMTD("DeSerializationResult begin, fileName = %p", fileName);

    std::shared_ptr<CameraResult> pResult;
    if (NULL == fileName)
    {
        LOGFMTW("DeSerializationResult finish, fileName = NULL");
        return pResult;
    }
    LOGFMTD("DeSerializationResult fileName = %s.", fileName);
       
    if (strlen(fileName) <= 0)
    {
        LOGFMTE("DeSerializationResult finish, fileName length is invalid.");
        return pResult;
    }

    ///// �����л�����
    size_t iFileSize = Tool_GetFileSize(fileName);
    unsigned char* pFileBuffer = new unsigned char[iFileSize + 1];
    if (!Tool_LoadFile(fileName, (void*)pFileBuffer, iFileSize))
    {
        LOGFMTE("DeSerializationResult Tool_LoadFile  failed.");

        SAFE_DELETE_ARRAY(pFileBuffer);
        return pResult;
    }

    if (pFileBuffer == NULL || iFileSize <= 0)
    {
        LOGFMTE("DeSerializationResult Tool_LoadFile  pFileBuffer == NULL || iFileSize <= 0.");

        SAFE_DELETE_ARRAY(pFileBuffer);
        return pResult;
    }
    
    pResult = std::make_shared<CameraResult>();
    if (NULL == pResult)
    {
        LOGFMTE("DeSerializationResult Tool_LoadFile,  NULL == pResult");
        SAFE_DELETE_ARRAY(pFileBuffer);
        return pResult;
    }

    char chTemp[MAX_PATH] = { 0 };
    int iLenth = MAX_PATH;
    pFileBuffer[iFileSize] = '\0';
    memset(chTemp, '\0', sizeof(chTemp));
    if (Tool_GetDataFromAppenedInfo((char*)pFileBuffer, "VehicleType", chTemp, &iLenth))
    {
        int iValue = AnalysisVelchType(chTemp);
        if (iValue >= 10)
        {
            pResult->iVehTypeNo = 2;
        }
        else if (iValue>0)
        {
            pResult->iVehTypeNo = 1;
        }
        else
        {
            pResult->iVehTypeNo = 0;
        }        
    }

    memset(chTemp, '\0', sizeof(chTemp));
    iLenth = MAX_PATH;
    if (Tool_GetDataFromAppenedInfo((char*)pFileBuffer, "AxleCnt", chTemp, &iLenth))
    {
        int iAxleCount = 0;
        sscanf_s(chTemp, "%d", &iAxleCount);
        pResult->iAxletreeCount = iAxleCount;
        //printf("the Axletree count is %d.\n", iAxleCount);
    }

    memset(chTemp, '\0', sizeof(chTemp));
    iLenth = MAX_PATH;
    if (Tool_GetDataFromAppenedInfo((char*)pFileBuffer, "Confidence", chTemp, &iLenth))        //������
    {
        float fConfidence = 0.0;
        sscanf_s(chTemp, "%f", &fConfidence);
        pResult->fConfidenceLevel = fConfidence;
        //printf("the Axletree count is %d.\n", iAxleCount);
    }

    memset(chTemp, '\0', sizeof(chTemp));
    iLenth = MAX_PATH;
    if (Tool_GetDataFromAppenedInfo((char*)pFileBuffer, "CarType", chTemp, &iLenth))
    {
        if (strstr(chTemp, "��"))
        {
            pResult->iVehSizeType = CAR_TYPE_SIZE_BIG;
        }
        else if (strstr(chTemp, "��"))
        {
            pResult->iVehSizeType = CAR_TYPE_SIZE_MIDDLE;
        }
        else if (strstr(chTemp, "С"))
        {
            pResult->iVehSizeType = CAR_TYPE_SIZE_SMALL;
        }
        else
        {
            pResult->iVehSizeType = CAR_TYPE_SIZE_UNKNOWN;
        }
    }

    memset(chTemp, '\0', sizeof(chTemp));
    iLenth = MAX_PATH;
    if (Tool_GetDataFromAppenedInfo((char*)pFileBuffer, "VideoScaleSpeed", chTemp, &iLenth)) 
    {
        int iVideoScaleSpeed = 0;
        sscanf_s(chTemp, "%d KM/h", &iVideoScaleSpeed);
        pResult->iSpeed = iVideoScaleSpeed;
        //printf("the Axletree count is %d.\n", iAxleCount);
    }

    memset(chTemp, '\0', sizeof(chTemp));
    iLenth = MAX_PATH;
    if (Tool_GetDataFromAppenedInfo((char*)pFileBuffer, "RoadNumber", chTemp, &iLenth))        //������
    {
        int iRoadNumber = 0;
        sscanf_s(chTemp, "%d", &iRoadNumber);
        pResult->iRoadNo = iRoadNumber;
        //printf("the Axletree count is %d.\n", iAxleCount);
    }

    memset(chTemp, '\0', sizeof(chTemp));
    iLenth = MAX_PATH;
    if (Tool_GetDataFromAppenedInfo((char*)pFileBuffer, "StreetName", chTemp, &iLenth)) 
    {
        sprintf_s(pResult->chSignStationName, sizeof(pResult->chSignStationName), "%s", chTemp);
    }

    memset(chTemp, '\0', sizeof(chTemp));
    iLenth = MAX_PATH;
    if (Tool_GetDataFromAppenedInfo((char*)pFileBuffer, "StreetDirection", chTemp, &iLenth)) 
    {
        sprintf_s(pResult->chSignDirection, sizeof(pResult->chSignDirection), "%s", chTemp);

        if (strlen(chTemp)> 0 )
        {
            if (NULL != strstr(chTemp, "����" ))
            {
                pResult->iDirection = DIRECTION_UP_TO_DOWN;
            }
            else if (NULL != strstr(chTemp, "����"))
            {
                pResult->iDirection = DIRECTION_DOWN_TO_UP;
            }
            else if (NULL != strstr(chTemp, "�ɶ�����"))
            {
                pResult->iDirection = DIRECTION_EAST_TO_WEST;
            }
            else if (NULL != strstr(chTemp, "������"))
            {
                pResult->iDirection = DIRECTION_WEST_TO_EAST;
            }
            else  if (NULL != strstr(chTemp, "������"))
            {
                pResult->iDirection = DIRECTION_SOUTH_TO_NORTH;
            }
            else if (NULL != strstr(chTemp, "�ɱ�����"))
            {
                pResult->iDirection = DIRECTION_NORTH_TO_SOUTH;
            }
            else
            {
                pResult->iDirection = DIRECTION_UP_TO_DOWN;
            }
        }

    }
    
    memset(chTemp, '\0', sizeof(chTemp));
    iLenth = MAX_PATH;
    if (Tool_GetDataFromAppenedInfo((char*)pFileBuffer, "CarFace", chTemp, &iLenth))
    {
        sprintf_s(pResult->chCarFace, sizeof(pResult->chCarFace), "%s", chTemp);
        //DealWithCarFace(pResult);
        DealWithCarFaceEx(pResult);
    }

    memset(chTemp, '\0', sizeof(chTemp));
    iLenth = MAX_PATH;
    if (Tool_GetDataFromAppenedInfo((char*)pFileBuffer, "CarID", chTemp, &iLenth))
    {
        DWORD dwCarID = 0;
        sscanf_s(chTemp, "%d", &dwCarID);
        pResult->dwCarID = dwCarID;
    }

    memset(chTemp, '\0', sizeof(chTemp));
    iLenth = MAX_PATH;
    if (Tool_GetDataFromAppenedInfo((char*)pFileBuffer, "DeviceIP", chTemp, &iLenth))
    {
        sprintf_s(pResult->chDeviceIp, sizeof(pResult->chDeviceIp), "%s", chTemp);
    }

    memset(chTemp, '\0', sizeof(chTemp));
    iLenth = MAX_PATH;
    if (Tool_GetDataFromAppenedInfo((char*)pFileBuffer, "CarArriveTime", chTemp, &iLenth))
    {
        sprintf_s(pResult->chPlateTime, sizeof(pResult->chPlateTime), "%s", chTemp);

        if (strlen(chTemp) > 0)
        {
            tm tm_;
            int year, month, day, hour, minute, second;
            sscanf_s(chTemp, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
            tm_.tm_year = year - 1900;
            tm_.tm_mon = month - 1;
            tm_.tm_mday = day;
            tm_.tm_hour = hour;
            tm_.tm_min = minute;
            tm_.tm_sec = second;
            tm_.tm_isdst = 0;

            time_t time64Value= mktime(&tm_); //�Ѿ�����8��ʱ��  
            pResult->dw64TimeMS = time64Value;
        }
    }
        
    memset(chTemp, '\0', sizeof(chTemp));
    size_t iBufLength = MAX_PATH;
    if (Tool_GetElementTextByName((char*)pFileBuffer, "PlateName", 2, chTemp, iBufLength))
    {
        sprintf_s(pResult->chPlateNO, sizeof(pResult->chPlateNO), "%s", chTemp);
        DealWithPlateNo(pResult);
    }

    memset(chTemp, '\0', sizeof(chTemp));
    iBufLength = MAX_PATH;
    if (Tool_GetElementTextByName((char*)pFileBuffer, "ITTS_IP", 2, chTemp, iBufLength))
    {
        sprintf_s(pResult->chServerIP, sizeof(pResult->chServerIP), "%s", chTemp);
    }

    pResult->iVehBodyColorNo = 0xff;
    
    int iTryTime = 5;
    int iWaitTime = 1000;
    bool bTrySuccess = false;
    std::string filePath;
    for (int i = 0; i < iTryTime; i++)
    {
        filePath = "";
        if (DeserializationCIMG((const char*)pFileBuffer, pResult->CIMG_BinImage, "BinImgPath", filePath))
        {
            bTrySuccess = true;
            break;
        }
        if (filePath.length()> 0)
        {
            Sleep(iWaitTime);
        }
    }
    if (!bTrySuccess)
    {
        LOGFMTE("DeSerializationResult, DeserializationCIMG  failed,  delete file %s", filePath.c_str());
        Tool_DeleteFileByCMD(filePath.c_str());
    }

    bTrySuccess = false;
    for (int i = 0; i < iTryTime; i++)
    {
        filePath = "";
        if (DeserializationCIMG((const char*)pFileBuffer, pResult->CIMG_PlateImage, "SmallImgPath", filePath))
        {
            bTrySuccess = true;
            break;
        }
        if (filePath.length()> 0)
        {
            Sleep(iWaitTime);
        }
    }
    if (!bTrySuccess)
    {
        LOGFMTE("DeSerializationResult, DeserializationCIMG  failed,  delete file %s", filePath.c_str());
        Tool_DeleteFileByCMD(filePath.c_str());
    }

    bTrySuccess = false;
    for (int i = 0; i < iTryTime; i++)
    {
        filePath = "";
        if (DeserializationCIMG((const char*)pFileBuffer, pResult->CIMG_BestSnapshot, "BestSnapshotPath", filePath))
        {
            bTrySuccess = true;
            break;
        }
        if (filePath.length()> 0)
        {
            Sleep(iWaitTime);
        }        
    }
    if (!bTrySuccess)
    {
        LOGFMTE("DeSerializationResult, DeserializationCIMG  failed,  delete file %s", filePath.c_str());
        Tool_DeleteFileByCMD(filePath.c_str());
    }

    bTrySuccess = false;
    for (int i = 0; i < iTryTime; i++)
    {
        filePath = "";
        if (DeserializationCIMG((const char*)pFileBuffer, pResult->CIMG_LastSnapshot, "LastSnapshotPath", filePath))
        {
            bTrySuccess = true;
            break;
        }
        if (filePath.length()> 0)
        {
            Sleep(iWaitTime);
        }
    }
    if (!bTrySuccess)
    {
        LOGFMTE("DeSerializationResult, DeserializationCIMG  failed,  delete file %s", filePath.c_str());
        Tool_DeleteFileByCMD(filePath.c_str());        
    }

    Tool_DeleteFileByCMD(fileName);
    SAFE_DELETE_ARRAY(pFileBuffer);
    LOGFMTD("DeSerializationResult finish");

    return pResult;
}

bool ResultUploader::DeserializationCIMG(const char* AppendInfo, CameraIMG& IMG, const char* imgType, std::string& filePath)
{
    LOGFMTD("DeserializationCIMG %s ", imgType);
    bool bRet = false;
    char chTemp[MAX_PATH] = {0};
    memset(chTemp, '\0', sizeof(chTemp));
    size_t iBufLength = MAX_PATH;
    if (Tool_GetElementTextByName(AppendInfo, imgType, 2, chTemp, iBufLength))
    {
        filePath = chTemp;
        size_t iFileSize = Tool_GetFileSize(chTemp);
        if (iBufLength > 0 && iFileSize > 0)
        {
            unsigned char* pbImgData = new unsigned char[iFileSize + 1];
            
            if (pbImgData)
            {
                memset(pbImgData, '\0', iFileSize+1);
                if (Tool_LoadFile(chTemp, (void*)pbImgData, iFileSize))
                {
                    IMG.pbImgData = pbImgData;
                    IMG.dwImgSize = iFileSize;

                    pbImgData = NULL;
                    
                    bRet = true;
                }
                else
                {
                    IMG.pbImgData = NULL;
                    IMG.dwImgSize = NULL;

                    LOGFMTE("%s path= %s, Tool_LoadFile data failed", imgType, chTemp);
                }
            }
            else
            {
                LOGFMTE("DeserializationCIMG, failed, %s path = %s, malloc data failed", imgType, chTemp);
            }
            SAFE_DELETE_ARRAY(pbImgData);
        }
        else
        {
            LOGFMTE("DeserializationCIMG, failed, iBufLength <= 0 or iFileSize <= 0, %s Path = %s, file size = %d, ", imgType, chTemp, iFileSize);
        }
        Tool_DeleteFileByCMD(chTemp);
        if (Tool_IsFileExist(chTemp))
        {
            LOGFMTE("DeserializationCIMG, DeleteFile %s failed", chTemp);
            Tool_DeleteFileByCMD(chTemp);
        }
    }
    else
    {
        LOGFMTE("DeserializationCIMG, failed, Tool_GetElementTextByName failed, imgType = %s.", imgType);
        Tool_DeleteFileByCMD(chTemp);
    }
    LOGFMTD("DeserializationCIMG %s  finish.", imgType);
    return bRet;
}

void ResultUploader::DealWithPlateNo(std::shared_ptr<CameraResult> pResult)
{
    if (NULL == pResult)
    {
        return;
    }

    char chTemp[64] = {0};
    strcpy_s(chTemp, sizeof(chTemp), pResult->chPlateNO);

    size_t  iLenth = strlen(chTemp);
    printf("find the plate number = %s, plate length = %d\n", chTemp, iLenth);

    bool bIsArmyPlate = CheckIfArmyPlate(chTemp, iLenth);

    if (strlen(chTemp) > 0 && NULL == strstr(chTemp, "��"))
    {
        if (bIsArmyPlate)
        {
            pResult->iPlateTypeNo = PLATE_TYPE_LEFT_RIGHT_ARMY;
        }
        else
        {
            if (NULL != strstr(chTemp, "WJ"))
            {
                pResult->iPlateTypeNo = PLATE_TYPE_ONE_LINE_ARMED_POLICE;
            }
            else if (NULL != strstr(chTemp, "��"))
            {
                pResult->iPlateTypeNo = PLATE_TYPE_POLICE;
            }
            else if (NULL != strstr(chTemp, "ʹ") || NULL != strstr(chTemp, "��"))
            {
                pResult->iPlateTypeNo = PLATE_TYPE_EMBASSY;
            }
            else if (NULL != strstr(chTemp, "��"))
            {
                pResult->iPlateTypeNo = PLATE_TYPE_NEW_ENERGY;
            }
            else
            {
                pResult->iPlateTypeNo = PLATE_TYPE_NORMAL;
            }
        }
    }
    else
    {
        pResult->iPlateTypeNo = PLATE_TYPE_NORMAL;
    }

    if (strlen(chTemp) > 0 && NULL == strstr(chTemp, "��"))
    {
        if (NULL != strstr(chTemp, "��") && NULL == strstr(chTemp, "��"))
        {
            pResult->iPlateColor = COLOR_BLUE;
            printf("find plate color COLOR_BLUE.\n");
        }
        else if (NULL != strstr(chTemp, "��"))
        {
            pResult->iPlateColor = COLOR_YELLOW;
            printf("find plate color COLOR_YELLOW.\n");
        }
        else if (NULL != strstr(chTemp, "��"))
        {
            pResult->iPlateColor = COLOR_BLACK;
            printf("find plate color COLOR_BLACK.\n");
        }
        else if (NULL != strstr(chTemp, "��") && NULL == strstr(chTemp, "��"))
        {
            pResult->iPlateColor = COLOR_WHITE;
            printf("find plate color COLOR_WHITE.\n");
        }
        else if (NULL != strstr(chTemp, "��"))
        {
            size_t ilen = strlen(chTemp);
            if (chTemp[ilen - 1] == 'D' || chTemp[ilen - 1] == 'F')
            {
                pResult->iPlateColor = COLOR_YELLOW_GREEN;
                printf("find plate color COLOR_YELLOW_GREEN.\n");
            }
            else
            {
                pResult->iPlateColor = COLOR_GRADIENT_CREEN;
                printf("find plate color COLOR_GRADIENT_CREEN.\n");
            }
        }
        else if (NULL != strstr(chTemp, "��") && NULL != strstr(chTemp, "��"))
        {
            pResult->iPlateColor = COLOR_BLUE_WHIETE;
            printf("find plate color COLOR_BLUE_WHIETE.\n");
        }
        else
        {
            pResult->iPlateColor = COLOR_BLUE;
            printf("find plate color COLOR_UNKNOW.\n");
        }

        memset(pResult->chPlateNO, '\0', sizeof(pResult->chPlateNO));
        sprintf_s(pResult->chPlateNO, sizeof(pResult->chPlateNO), "%s", chTemp + 2);
    }
    else
    {
        sprintf_s(pResult->chPlateNO, sizeof(pResult->chPlateNO), "�޳���");
        pResult->iPlateColor = COLOR_BLUE;
    }
}

void ResultUploader::DealWithCarFace(std::shared_ptr<CameraResult> pResult)
{
    if (NULL == pResult)
    {
        return;
    }
    char chTemp[64] = { 0 };
    strcpy_s(chTemp, sizeof(chTemp), pResult->chCarFace);

    if (strstr(chTemp, "_"))
    {
        std::string strCarFace(chTemp);
        std::string strChileLog = strCarFace.substr(strCarFace.find("_")+1);
        sprintf_s(pResult->chChileLogo, sizeof(pResult->chChileLogo), "%s", strChileLog.c_str());
    }

    if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 1;
    }
    else if (strstr(chTemp, "���"))
    {
        pResult->iVehLogoType = 2;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 3;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 4;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 5;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 6;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 7;
    }
    else if (strstr(chTemp, "�ղ�"))
    {
        pResult->iVehLogoType = 8;
    }
    else if (strstr(chTemp, "�µ�"))
    {
        pResult->iVehLogoType = 9;
    }
    else if (strstr(chTemp, "���Դ�"))
    {
        pResult->iVehLogoType = 10;
    }
    else if (strstr(chTemp, "ѩ����"))
    {
        pResult->iVehLogoType = 11;
    }
    else if (strstr(chTemp, "ѩ����"))
    {
        pResult->iVehLogoType = 12;
    }
    else if (strstr(chTemp, "�ִ�"))
    {
        pResult->iVehLogoType = 13;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 14;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 15;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 16;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 17;
    }
    else if (strstr(chTemp, "˹�´�"))
    {
        pResult->iVehLogoType = 18;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 19;
    }
    else if (strstr(chTemp, "�л�"))
    {
        pResult->iVehLogoType = 20;
    }
    else if (strstr(chTemp, "�ֶ���"))
    {
        pResult->iVehLogoType = 21;
    }
    else if (strstr(chTemp, "�׿���˹"))
    {
        pResult->iVehLogoType = 22;
    }
    else if (strstr(chTemp, "������"))
    {
        pResult->iVehLogoType = 23;
    }
    else if (strstr(chTemp, "�ۺ�"))
    {
        pResult->iVehLogoType = 24;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 25;
    }
    else if (strstr(chTemp, "���ǵ�"))
    {
        pResult->iVehLogoType = 26;
    }
    else if (strstr(chTemp, "��ľ"))
    {
        pResult->iVehLogoType = 27;
    }
    else if (strstr(chTemp, "��"))
    {
        pResult->iVehLogoType = 28;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 29;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 30;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 31;
    }
    else if (strstr(chTemp, "˹��³"))
    {
        pResult->iVehLogoType = 32;
    }
    else if (strstr(chTemp, "Ӣ��"))
    {
        pResult->iVehLogoType = 33;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 34;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 35;
    }
    else if (strstr(chTemp, "��ʮ��"))
    {
        pResult->iVehLogoType = 36;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 37;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 38;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 39;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 40;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 41;
    }
    else if (strstr(chTemp, "һ��"))
    {
        pResult->iVehLogoType = 42;
    }
    else if (strstr(chTemp, "��ά��"))
    {
        pResult->iVehLogoType = 43;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 44;
    }
    else if (strstr(chTemp, "һ������"))
    {
        pResult->iVehLogoType = 45;
    }
    else if (strstr(chTemp, "�ʹ�"))
    {
        pResult->iVehLogoType = 46;
    }
    else if (strstr(chTemp, "��ŵ"))
    {
        pResult->iVehLogoType = 47;
    }
    else if (strstr(chTemp, "JMC") || strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 48;
    }
    else if (strstr(chTemp, "MG"))
    {
        pResult->iVehLogoType = 49;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 50;
    }
    else if (strstr(chTemp, "��̩"))
    {
        pResult->iVehLogoType = 51;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 52;
    }
    else if (strstr(chTemp, "���Ž���"))
    {
        pResult->iVehLogoType = 53;
    }
    else if (strstr(chTemp, "�Ϻ�����"))
    {
        pResult->iVehLogoType = 54;
    }
    else if (strstr(chTemp, "���ݽ���"))
    {
        pResult->iVehLogoType = 55;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 56;
    }
    else if (strstr(chTemp, "��ͨ"))
    {
        pResult->iVehLogoType = 57;
    }
    else if (strstr(chTemp, "�й�����"))
    {
        pResult->iVehLogoType = 58;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 59;
    }
    else if (strstr(chTemp, "����"))
    {
        pResult->iVehLogoType = 60;
    }
    else if (strstr(chTemp, "Ծ��"))
    {
        pResult->iVehLogoType = 61;
    }
    else if (strstr(chTemp, "�ƺ�"))
    {
        pResult->iVehLogoType = 62;
    }
    else
    {
        pResult->iVehLogoType = 0;
    }

}

void ResultUploader::DealWithCarFaceEx(std::shared_ptr<CameraResult> pResult)
{
    if (NULL == pResult)
    {
        return;
    }
    char chTemp[256] = { 0 };
    strcpy_s(chTemp, sizeof(chTemp), pResult->chCarFace);

    char chSeparator[10] = {0};
    memset(chSeparator, '\0', sizeof(chSeparator));
    if (strstr(chTemp, "_"))
    {
        chSeparator[0] = '_';
    }
    else if (strstr(chTemp, "-"))
    {
        chSeparator[0] = '-';
    }

    std::string strMainLogo, strSubLogo;
    if (strlen(chSeparator) > 0)
    {
        std::string strCarFace(chTemp);
        strMainLogo = strCarFace.substr(0, strCarFace.find("_"));
        strSubLogo = strCarFace.substr(strCarFace.find("_") + 1);
        sprintf_s(pResult->chChileLogo, sizeof(pResult->chChileLogo), "%s", strSubLogo.c_str());
    }
    else
    {
        strMainLogo = chTemp;
    }
    int iLogoType = 0;
    if (m_CarFaceMap.size() > 0)
    {
        if (strMainLogo.length() > 0)
        {
            if (Tool_FindMapAndGetValue(m_CarFaceMap, strMainLogo, iLogoType))
            {
                pResult->iVehLogoType = iLogoType;
            }
            else if (Tool_FindMapAndGetValue(m_CarFaceMap, strSubLogo, iLogoType))
            {
                pResult->iVehLogoType = iLogoType;
            }
            else
            {
                pResult->iVehLogoType = 0;
            }
        }
        else
        {
            pResult->iVehLogoType = 0;
        }
    }
    else
    {
        pResult->iVehLogoType = 0;
    }
}

bool ResultUploader::CheckIfArmyPlate(const char* plateNo, size_t iLength)
{
    if (plateNo == NULL || iLength <= 0)
    {
        return false;
    }
    //��������λ�����ǷǺ��֣���������жϼ���
    //���ȣ�������˫�ֽڵı����ʽ��
    //    ���ֵĵ�һ�ֽڣ��Ǵ�0xB0   ��ʼ����
    //    ���ֵĵڶ��ֽڣ��Ǵ�0xA1   ��ʼ����
    //    �������ϵ��˽⣬�Ͳ���ʶ��������ģ����������ı��룡
    //    ������ֻ��Ҫ�жϣ�   ��һ�ֽ� >= 0xB0 && �ڶ��ֽ� >= 0xA1   ���ͺã�

    char chTemp[256] = {0};
    memset(chTemp, '\0', sizeof(chTemp));
    memcpy(chTemp, plateNo, iLength);
    for (int i = 0; i < sizeof(chTemp); i++)
    {
        if (chTemp[i] > 0)
        {
            return false;
        }
    }
    return true;
}

void ResultUploader::SearchFile()
{
    if (m_lsFileNameLsit.empty())
    {
        const std::string strDir(m_chUploadPath);

        std::list<std::string> strFileList = getFilesPath(strDir, "\\*.xml");
        for (std::list<std::string>::iterator it = strFileList.begin(); it != strFileList.end(); it++)
        {
            m_lsFileNameLsit.push_back(*it);
        }
    }
}

void ResultUploader::SetCheckThreadExit(bool bExit)
{
    MySafeLocker locker(&m_cs);
    m_bCheckExit = bExit;
}

bool ResultUploader::GetCheckThreadExit()
{
    MySafeLocker locker(&m_cs);
    return m_bCheckExit;
}

int ResultUploader::AnalysisVelchType(const char* vehType)
{
    if (vehType == NULL)
    {
        return UNKOWN_TYPE;
    }
    if (strstr(vehType, "��1"))
    {
        return BUS_TYPE_1;
    }
    else if (strstr(vehType, "��2"))
    {
        return BUS_TYPE_2;
        //printf("the Vehicle type code is 2.\n");
    }
    else if (strstr(vehType, "��3"))
    {
        return BUS_TYPE_3;
    }
    else if (strstr(vehType, "��4"))
    {
        return BUS_TYPE_4;
    }
    else if (strstr(vehType, "��5"))
    {
        return BUS_TYPE_5;
    }
    else if (strstr(vehType, "��1"))
    {
        return TRUCK_TYPE_1;
    }
    else if (strstr(vehType, "��2"))
    {
        return TRUCK_TYPE_2;
    }
    else if (strstr(vehType, "��3"))
    {
        return TRUCK_TYPE_3;
    }
    else if (strstr(vehType, "��4"))
    {
        return TRUCK_TYPE_4;
    }
    else if (strstr(vehType, "��5"))
    {
        return TRUCK_TYPE_5;
    }
    else
    {
        return UNKOWN_TYPE;
    }
}

bool ResultUploader::LoadCarfaceXml(const char* fileName, std::map<std::string, int>& myMap)
{
    //���ص�XML��ʽ����
    //< ? xml version = "1.0" encoding = "GB2312" standalone = "yes" ? >
    //    <CarFace>
    //    <LogoName>����< / LogoName>
    //    <LogoType>40 < / LogoType >
    //    < /CarFace>
    //    <CarFace>
    //    <LogoName>һ������< / LogoName>
    //    <LogoType>45 < / LogoType >
    //    < /CarFace>
    //    < / Camera>

    TiXmlDocument cXmlDoc;
    TiXmlElement* pRootElement = NULL;

    if (!cXmlDoc.LoadFile(fileName))
    {
        LOGFMTW("parse XML file failed \n");
        return false;
    }
    pRootElement = cXmlDoc.RootElement();
    if (NULL == pRootElement)
    {
        LOGFMTW("no have root Element\n");
        return false;
    }
    LOGFMTW("root element text = %s.\n", pRootElement->Value());
    const TiXmlNode* pNodeCamera = pRootElement;
    do
    {
        if (0 == strcmp(pNodeCamera->Value(), "CarFace"))
        {
            const TiXmlNode* pNode = pNodeCamera->FirstChild();
            std::string strKey;
            while (pNode)
            {
                LOGFMTW("root element value = %s.\n", pNode->Value());
                if (0 == strcmp(pNode->Value(), "LogoName"))
                {
                    LOGFMTW("root element FirstChild()->Value() = %s.\n", pNode->FirstChild()->Value());
                    strKey = pNode->FirstChild()->Value();
                    myMap.insert(std::make_pair(strKey, 0));
                }
                if (0 == strcmp(pNode->Value(), "LogoType"))
                {
                    myMap[strKey] = atoi(pNode->FirstChild()->Value());
                }
                pNode = pNode->NextSibling();
            }
        }
        pNodeCamera = pNodeCamera->NextSibling();
    } while (pNodeCamera);

    return true;
}

unsigned int __stdcall Result_FileSearchThread(LPVOID lpParam)
{
    if (NULL == lpParam)
    {
        return 0;
    }
    ResultUploader* pUploader = (ResultUploader*)lpParam;
    while (!pUploader->GetCheckThreadExit())
    {
        Sleep(50);
        pUploader->SearchFile();
    }

    return 0;
}
unsigned int __stdcall Result_UploadResultThread(LPVOID lpParam)
{
    if (NULL == lpParam)
    {
        return 0;
    }
    ResultUploader* pUploader = (ResultUploader*)lpParam;
    pUploader->UpLoadData();

    return 0;
}