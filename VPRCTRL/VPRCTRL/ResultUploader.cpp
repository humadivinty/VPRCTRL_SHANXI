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
                LOGFMTD("UpLoadResultByCallBack，begin.");
                UpLoadResultByCallBack(pResult);
                LOGFMTD("UpLoadResultByCallBack，finish.");
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

        ////todo 填充数据
        trafficData.dwVersion = 1;
        sprintf_s(trafficData.chTSIP, sizeof(trafficData.chTSIP), "%s", pResult->chServerIP);
        sprintf_s(trafficData.chPlate, sizeof(trafficData.chPlate), "%s", pResult->chPlateNO);
        trafficData.chPlateColor = pResult->iPlateColor;                //车牌颜色
        if (pResult->CIMG_BinImage.dwImgSize > 0 && NULL != pResult->CIMG_BinImage.pbImgData)
        {
            memcpy(trafficData.byBinImg, pResult->CIMG_BinImage.pbImgData, BINARY_IMG_SIZE);        //二值图
        }
        else
        {
            memset(trafficData.byBinImg, 0, BINARY_IMG_SIZE);       //二值图
        }
        trafficData.dwLaneNum = pResult->iRoadNo;                   //车道编号
        trafficData.dwSpeed = pResult->iSpeed;                          //行车速度
        trafficData.dwPassTime = pResult->dw64TimeMS;        //过车时间
        trafficData.dwDir = pResult->iDirection;                    //下行(0)，上行（1），由东向西(2)，由西向东(3)，由南向北(4)，由北向南(5)
        trafficData.dwDataType = 0;     //数据类型
        trafficData.dwDefense = 0;      //违章代码
        if (m_CameraIPMap.size() > 0 
            && m_CameraIPMap.find(pResult->chDeviceIp) != m_CameraIPMap.end())
        {
            trafficData.dwCameraID = m_CameraIPMap.find(pResult->chDeviceIp)->second;    //相机编号
        }
        else
        {
            trafficData.dwCameraID = 0;         //相机编号
        }
        
        trafficData.dwPlateType = pResult->iPlateTypeNo;         //车牌类型        
        trafficData.dwVehicleColor = 0;                                     //车辆颜色

        trafficData.dwPlateImgLen = pResult->CIMG_PlateImage.dwImgSize;     //车牌图长度
        if (pResult->CIMG_PlateImage.dwImgSize > 0)
        {
            trafficData.pPlateJpegImg = pResult->CIMG_PlateImage.pbImgData;
        }
        else
        {
            trafficData.pPlateJpegImg = NULL;
        }

        trafficData.dwCarJpegLen1 = pResult->CIMG_BestSnapshot.dwImgSize;     //车辆JPEG图1长度
        if (pResult->CIMG_BestSnapshot.dwImgSize > 0)
        {
            trafficData.pCarJpegImg1 = pResult->CIMG_BestSnapshot.pbImgData;
        }
        else
        {
            trafficData.pCarJpegImg1 = NULL;
        }

        trafficData.dwCarJpegLen2 = pResult->CIMG_LastSnapshot.dwImgSize;     //车辆JPEG图2长度
        if (pResult->CIMG_LastSnapshot.dwImgSize > 0)
        {
            trafficData.pCarJpegImg2 = pResult->CIMG_LastSnapshot.pbImgData;
        }
        else
        {
            trafficData.pCarJpegImg2 = NULL;
        }

        trafficData.dwCarJpegLen3 = pResult->CIMG_BestCapture.dwImgSize;      //车辆JPEG图3长度
        if (pResult->CIMG_BestCapture.dwImgSize > 0)
        {
            trafficData.pCarJpegImg3 = pResult->CIMG_BestCapture.pbImgData;
        }
        else
        {
            trafficData.pCarJpegImg3 = NULL;
        }

        trafficData.byVehicleType = pResult->iVehSizeType;        //车辆类型
        trafficData.byVehicleKind = pResult->iVehTypeNo;       //车辆种类 ，0-未知，1客车，2货车
        trafficData.byColorDepth = 0;        //车身颜色深浅，0-深色，1-浅色 
        trafficData.byColor = 0xff;                 //车身颜色 0：其他色；1：白色；2：灰(银)；3：灰(银)；4：黑色；5：红色；6：深蓝；7：蓝色；8：黄色；9：绿色；10：棕色；11.:粉色12: 紫色
        trafficData.byRes1 = 0;                     //保留
        trafficData.wLength = 0;                     //车身长度
        trafficData.byVehicleLogoRecog = pResult->iVehLogoType;                     //汽车品牌
        memcpy(trafficData.byVehicleSubLogoRecog, pResult->chChileLogo, 31);
        trafficData.byVehicleSubLogoRecog[31] = '\0';              //车辆子品牌
        trafficData.byVehicleModel = 0;                                 //车辆子品牌年款
        trafficData.byExhaust[0] = 0;                                     //车辆排放信息  
        trafficData.byCustomInfo[0] = 0;                                //自定义信息
        trafficData.byRes3[0] = 0;                                              //保留

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

    ///// 反序列化数据
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
    if (Tool_GetDataFromAppenedInfo((char*)pFileBuffer, "Confidence", chTemp, &iLenth))        //车道号
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
        if (strstr(chTemp, "大"))
        {
            pResult->iVehSizeType = CAR_TYPE_SIZE_BIG;
        }
        else if (strstr(chTemp, "中"))
        {
            pResult->iVehSizeType = CAR_TYPE_SIZE_MIDDLE;
        }
        else if (strstr(chTemp, "小"))
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
    if (Tool_GetDataFromAppenedInfo((char*)pFileBuffer, "RoadNumber", chTemp, &iLenth))        //车道号
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
            if (NULL != strstr(chTemp, "下行" ))
            {
                pResult->iDirection = DIRECTION_UP_TO_DOWN;
            }
            else if (NULL != strstr(chTemp, "上行"))
            {
                pResult->iDirection = DIRECTION_DOWN_TO_UP;
            }
            else if (NULL != strstr(chTemp, "由东向西"))
            {
                pResult->iDirection = DIRECTION_EAST_TO_WEST;
            }
            else if (NULL != strstr(chTemp, "由西向东"))
            {
                pResult->iDirection = DIRECTION_WEST_TO_EAST;
            }
            else  if (NULL != strstr(chTemp, "由南向北"))
            {
                pResult->iDirection = DIRECTION_SOUTH_TO_NORTH;
            }
            else if (NULL != strstr(chTemp, "由北向南"))
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

            time_t time64Value= mktime(&tm_); //已经减了8个时区  
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

    if (strlen(chTemp) > 0 && NULL == strstr(chTemp, "无"))
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
            else if (NULL != strstr(chTemp, "警"))
            {
                pResult->iPlateTypeNo = PLATE_TYPE_POLICE;
            }
            else if (NULL != strstr(chTemp, "使") || NULL != strstr(chTemp, "领"))
            {
                pResult->iPlateTypeNo = PLATE_TYPE_EMBASSY;
            }
            else if (NULL != strstr(chTemp, "绿"))
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

    if (strlen(chTemp) > 0 && NULL == strstr(chTemp, "无"))
    {
        if (NULL != strstr(chTemp, "蓝") && NULL == strstr(chTemp, "白"))
        {
            pResult->iPlateColor = COLOR_BLUE;
            printf("find plate color COLOR_BLUE.\n");
        }
        else if (NULL != strstr(chTemp, "黄"))
        {
            pResult->iPlateColor = COLOR_YELLOW;
            printf("find plate color COLOR_YELLOW.\n");
        }
        else if (NULL != strstr(chTemp, "黑"))
        {
            pResult->iPlateColor = COLOR_BLACK;
            printf("find plate color COLOR_BLACK.\n");
        }
        else if (NULL != strstr(chTemp, "白") && NULL == strstr(chTemp, "蓝"))
        {
            pResult->iPlateColor = COLOR_WHITE;
            printf("find plate color COLOR_WHITE.\n");
        }
        else if (NULL != strstr(chTemp, "绿"))
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
        else if (NULL != strstr(chTemp, "蓝") && NULL != strstr(chTemp, "白"))
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
        sprintf_s(pResult->chPlateNO, sizeof(pResult->chPlateNO), "无车牌");
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

    if (strstr(chTemp, "大众"))
    {
        pResult->iVehLogoType = 1;
    }
    else if (strstr(chTemp, "别克"))
    {
        pResult->iVehLogoType = 2;
    }
    else if (strstr(chTemp, "宝马"))
    {
        pResult->iVehLogoType = 3;
    }
    else if (strstr(chTemp, "本田"))
    {
        pResult->iVehLogoType = 4;
    }
    else if (strstr(chTemp, "标致"))
    {
        pResult->iVehLogoType = 5;
    }
    else if (strstr(chTemp, "丰田"))
    {
        pResult->iVehLogoType = 6;
    }
    else if (strstr(chTemp, "福特"))
    {
        pResult->iVehLogoType = 7;
    }
    else if (strstr(chTemp, "日产"))
    {
        pResult->iVehLogoType = 8;
    }
    else if (strstr(chTemp, "奥迪"))
    {
        pResult->iVehLogoType = 9;
    }
    else if (strstr(chTemp, "马自达"))
    {
        pResult->iVehLogoType = 10;
    }
    else if (strstr(chTemp, "雪佛兰"))
    {
        pResult->iVehLogoType = 11;
    }
    else if (strstr(chTemp, "雪铁龙"))
    {
        pResult->iVehLogoType = 12;
    }
    else if (strstr(chTemp, "现代"))
    {
        pResult->iVehLogoType = 13;
    }
    else if (strstr(chTemp, "奇瑞"))
    {
        pResult->iVehLogoType = 14;
    }
    else if (strstr(chTemp, "起亚"))
    {
        pResult->iVehLogoType = 15;
    }
    else if (strstr(chTemp, "荣威"))
    {
        pResult->iVehLogoType = 16;
    }
    else if (strstr(chTemp, "三菱"))
    {
        pResult->iVehLogoType = 17;
    }
    else if (strstr(chTemp, "斯柯达"))
    {
        pResult->iVehLogoType = 18;
    }
    else if (strstr(chTemp, "吉利"))
    {
        pResult->iVehLogoType = 19;
    }
    else if (strstr(chTemp, "中华"))
    {
        pResult->iVehLogoType = 20;
    }
    else if (strstr(chTemp, "沃尔沃"))
    {
        pResult->iVehLogoType = 21;
    }
    else if (strstr(chTemp, "雷克萨斯"))
    {
        pResult->iVehLogoType = 22;
    }
    else if (strstr(chTemp, "菲亚特"))
    {
        pResult->iVehLogoType = 23;
    }
    else if (strstr(chTemp, "帝豪"))
    {
        pResult->iVehLogoType = 24;
    }
    else if (strstr(chTemp, "东风"))
    {
        pResult->iVehLogoType = 25;
    }
    else if (strstr(chTemp, "比亚迪"))
    {
        pResult->iVehLogoType = 26;
    }
    else if (strstr(chTemp, "铃木"))
    {
        pResult->iVehLogoType = 27;
    }
    else if (strstr(chTemp, "金杯"))
    {
        pResult->iVehLogoType = 28;
    }
    else if (strstr(chTemp, "海马"))
    {
        pResult->iVehLogoType = 29;
    }
    else if (strstr(chTemp, "五菱"))
    {
        pResult->iVehLogoType = 30;
    }
    else if (strstr(chTemp, "江淮"))
    {
        pResult->iVehLogoType = 31;
    }
    else if (strstr(chTemp, "斯巴鲁"))
    {
        pResult->iVehLogoType = 32;
    }
    else if (strstr(chTemp, "英伦"))
    {
        pResult->iVehLogoType = 33;
    }
    else if (strstr(chTemp, "长城"))
    {
        pResult->iVehLogoType = 34;
    }
    else if (strstr(chTemp, "哈飞"))
    {
        pResult->iVehLogoType = 35;
    }
    else if (strstr(chTemp, "五十铃"))
    {
        pResult->iVehLogoType = 36;
    }
    else if (strstr(chTemp, "东南"))
    {
        pResult->iVehLogoType = 37;
    }
    else if (strstr(chTemp, "长安"))
    {
        pResult->iVehLogoType = 38;
    }
    else if (strstr(chTemp, "福田"))
    {
        pResult->iVehLogoType = 39;
    }
    else if (strstr(chTemp, "夏利"))
    {
        pResult->iVehLogoType = 40;
    }
    else if (strstr(chTemp, "奔驰"))
    {
        pResult->iVehLogoType = 41;
    }
    else if (strstr(chTemp, "一汽"))
    {
        pResult->iVehLogoType = 42;
    }
    else if (strstr(chTemp, "依维柯"))
    {
        pResult->iVehLogoType = 43;
    }
    else if (strstr(chTemp, "力帆"))
    {
        pResult->iVehLogoType = 44;
    }
    else if (strstr(chTemp, "一汽奔腾"))
    {
        pResult->iVehLogoType = 45;
    }
    else if (strstr(chTemp, "皇冠"))
    {
        pResult->iVehLogoType = 46;
    }
    else if (strstr(chTemp, "雷诺"))
    {
        pResult->iVehLogoType = 47;
    }
    else if (strstr(chTemp, "JMC") || strstr(chTemp, "江铃"))
    {
        pResult->iVehLogoType = 48;
    }
    else if (strstr(chTemp, "MG"))
    {
        pResult->iVehLogoType = 49;
    }
    else if (strstr(chTemp, "凯马"))
    {
        pResult->iVehLogoType = 50;
    }
    else if (strstr(chTemp, "众泰"))
    {
        pResult->iVehLogoType = 51;
    }
    else if (strstr(chTemp, "昌河"))
    {
        pResult->iVehLogoType = 52;
    }
    else if (strstr(chTemp, "厦门金龙"))
    {
        pResult->iVehLogoType = 53;
    }
    else if (strstr(chTemp, "上海汇众"))
    {
        pResult->iVehLogoType = 54;
    }
    else if (strstr(chTemp, "苏州金龙"))
    {
        pResult->iVehLogoType = 55;
    }
    else if (strstr(chTemp, "海格"))
    {
        pResult->iVehLogoType = 56;
    }
    else if (strstr(chTemp, "宇通"))
    {
        pResult->iVehLogoType = 57;
    }
    else if (strstr(chTemp, "中国重汽"))
    {
        pResult->iVehLogoType = 58;
    }
    else if (strstr(chTemp, "北奔"))
    {
        pResult->iVehLogoType = 59;
    }
    else if (strstr(chTemp, "华菱"))
    {
        pResult->iVehLogoType = 60;
    }
    else if (strstr(chTemp, "跃进"))
    {
        pResult->iVehLogoType = 61;
    }
    else if (strstr(chTemp, "黄海"))
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
    //军牌所有位数都是非汉字，根据这个判断即可
    //首先，汉字是双字节的编码格式！
    //    汉字的第一字节：是从0xB0   开始编码
    //    汉字的第二字节：是从0xA1   开始编码
    //    有了以上的了解，就不难识别出是中文，还是其他的编码！
    //    程序中只需要判断（   第一字节 >= 0xB0 && 第二字节 >= 0xA1   ）就好！

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
    if (strstr(vehType, "客1"))
    {
        return BUS_TYPE_1;
    }
    else if (strstr(vehType, "客2"))
    {
        return BUS_TYPE_2;
        //printf("the Vehicle type code is 2.\n");
    }
    else if (strstr(vehType, "客3"))
    {
        return BUS_TYPE_3;
    }
    else if (strstr(vehType, "客4"))
    {
        return BUS_TYPE_4;
    }
    else if (strstr(vehType, "客5"))
    {
        return BUS_TYPE_5;
    }
    else if (strstr(vehType, "货1"))
    {
        return TRUCK_TYPE_1;
    }
    else if (strstr(vehType, "货2"))
    {
        return TRUCK_TYPE_2;
    }
    else if (strstr(vehType, "货3"))
    {
        return TRUCK_TYPE_3;
    }
    else if (strstr(vehType, "货4"))
    {
        return TRUCK_TYPE_4;
    }
    else if (strstr(vehType, "货5"))
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
    //加载的XML格式如下
    //< ? xml version = "1.0" encoding = "GB2312" standalone = "yes" ? >
    //    <CarFace>
    //    <LogoName>夏利< / LogoName>
    //    <LogoType>40 < / LogoType >
    //    < /CarFace>
    //    <CarFace>
    //    <LogoName>一汽奔腾< / LogoName>
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