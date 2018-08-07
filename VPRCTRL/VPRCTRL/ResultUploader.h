#ifndef RESULT_UPLOAD_H
#define RESULT_UPLOAD_H
#include "Camera/ThreadSafeList.h"
#include "Camera/CameraResult.h"
#include <memory>

class ResultUploader
{
public:
    ResultUploader();
    ~ResultUploader();

    void startUpLoad();

    void SetUploadPath(const char* uploadPath);
    void SetUploadCallBackFunc(void* callbackFunc, void* userData);
    void UpLoadData();
    bool UpLoadResultByCallBack(std::shared_ptr<CameraResult> pResult);

    std::shared_ptr<CameraResult> DeSerializationResult(const char* fileName);
    bool DeserializationCIMG(const char* AppendInfo, CameraIMG& IMG, const char* imgType, std::string& filePath);
    void DealWithPlateNo(std::shared_ptr<CameraResult> pResult);
    void DealWithCarFace(std::shared_ptr<CameraResult> pResult);
    void DealWithCarFaceEx(std::shared_ptr<CameraResult> pResult);

    bool CheckIfArmyPlate(const char* plateNo, size_t iLength);

    void SearchFile();

    void SetCheckThreadExit(bool  bExit);
    bool GetCheckThreadExit();
    
    int AnalysisVelchType(const char* vehType);

public:
    bool LoadCarfaceXml(const char* fileName, std::map<std::string, int>& myMap);
private:
    void* m_pCallBackFunc;
    void* m_pUserData;

    HANDLE m_hSearchThread;
    HANDLE m_hUploadThread;

    bool m_bCheckExit;

    char m_chUploadPath[MAX_PATH];

    TemplateThreadSafeList<std::string> m_lsFileNameLsit;
    std::map<std::string, int> m_CameraIPMap;
    std::map<std::string, int> m_CarFaceMap;
    CRITICAL_SECTION m_cs;

    friend class MySafeLocker;
};

#endif // RESULT_UPLOAD_H


