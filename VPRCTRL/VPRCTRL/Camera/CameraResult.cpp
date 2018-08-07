#include "stdafx.h"
#include "CameraResult.h"

CameraIMG& CameraIMG::operator=( const CameraIMG& CaIMG )
{
	if (this != &CaIMG)
	{
		wImgWidth = CaIMG.wImgWidth;
		wImgHeight = CaIMG.wImgHeight;
		dwImgSize = CaIMG.dwImgSize;
		wImgType = CaIMG.wImgType;
		for (unsigned int i = 0; i< sizeof(chSavePath); i++)
		{
			chSavePath[i] = CaIMG.chSavePath[i];
		}
		if (NULL != CaIMG.pbImgData)
		{
			pbImgData = new BYTE[CaIMG.dwImgSize];
			for (DWORD j = 0; j< dwImgSize; j++)
			{
				pbImgData[j] = CaIMG.pbImgData[j];
			}
		}
		else
		{
			pbImgData = NULL;
		}
	}
	return *this;
}

CameraIMG::CameraIMG()
{
	wImgWidth = 0;
	wImgHeight = 0;
	pbImgData = NULL;
	dwImgSize = 0;
	for (unsigned int i = 0; i< sizeof(chSavePath); i++)
	{
		chSavePath[i] = 0;
	}
}

CameraIMG::CameraIMG( const CameraIMG& CaIMG )
{
    if (this == &CaIMG)
    {
        return;
    }
	wImgWidth = CaIMG.wImgWidth;
	wImgHeight = CaIMG.wImgHeight;
	dwImgSize = CaIMG.dwImgSize;
	wImgType = CaIMG.wImgType;
	for (unsigned int i = 0; i< sizeof(chSavePath); i++)
	{
		chSavePath[i] = CaIMG.chSavePath[i];
	}
	if (NULL != CaIMG.pbImgData)
	{
		pbImgData = new BYTE[CaIMG.dwImgSize];
		for (DWORD j = 0; j< dwImgSize; j++)
		{
			pbImgData[j] = CaIMG.pbImgData[j];
		}
	}
	else
	{
		pbImgData = NULL;
	}
}

CameraIMG::~CameraIMG()
{
	wImgWidth = 0;
	wImgHeight = 0;
	if (NULL != pbImgData)
	{
		delete[] pbImgData;
		pbImgData = NULL;
	}
	dwImgSize = 0;
	memset(chSavePath, 0, sizeof(chSavePath));
}
//-------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------

CameraResult::CameraResult()
{
	dwCarID = 0;
	iDeviceID = 0;
	iPlateColor = 0;
	iPlateTypeNo = 0;
	dw64TimeMS = 0;
	iSpeed = 0;
	iResultNo = 0;
	iVehTypeNo = 0;		//车型代码: 客1--1 。。。客4--4， 货1--5  。。。货4--8
    iVehSizeType = 0;
	iVehBodyColorNo = 0;	
	iVehBodyDeepNo = 0;	
    iVehLogoType = 0;
	iAreaNo = 0;
	iRoadNo = 0;
	iLaneNo = 0;
	iDirection = 0;
	iWheelCount = 0;		//轮数
	iAxletreeCount = 0;		//轴数
	iAxletreeType = 0;		//轴型
	iAxletreeGroupCount = 2;
    iReliability = 0;
	fVehLenth = 0;			//车长
	fDistanceBetweenAxles = 0.0;		//轴距
	fVehHeight = 0.0;		//车高
    fConfidenceLevel = 0.0;
	bBackUpVeh = false;		//是否倒车

	//pcAppendInfo = NULL;
	//for (unsigned int i = 0; i < sizeof(pcAppendInfo); i++)
	//{
	//	pcAppendInfo[i] = 0;
	//}
    
    memset(chDeviceIp, '\0', sizeof(chDeviceIp));
    memset(chServerIP, '\0', sizeof(chServerIP));
    memset(chPlateNO, '\0', sizeof(chPlateNO));
    memset(chPlateColor, '\0', sizeof(chPlateColor));
    memset(chListNo, '\0', sizeof(chListNo));
    memset(chPlateTime, '\0', sizeof(chPlateTime));
    memset(chSignStationID, '\0', sizeof(chSignStationID));
    memset(chSignStationName, '\0', sizeof(chSignStationName));
    memset(chSignDirection, '\0', sizeof(chSignDirection));
    memset(chDeviceID, '\0', sizeof(chDeviceID));
    memset(chLaneID, '\0', sizeof(chLaneID)); 
    memset(chCarFace, '\0', sizeof(chCarFace));
    memset(chChileLogo, '\0', sizeof(chChileLogo));
}

CameraResult::CameraResult( const CameraResult& CaRESULT )
{
    if (this == &CaRESULT)
    {
        return;
    }
	dwCarID = CaRESULT.dwCarID;
	dw64TimeMS = CaRESULT.dw64TimeMS;
	iLaneNo = CaRESULT.iLaneNo;
	iPlateColor = CaRESULT.iPlateColor;
	iVehTypeNo = CaRESULT.iVehTypeNo;
    iVehSizeType = CaRESULT.iVehSizeType;
	iDirection = CaRESULT.iDirection;
	iAreaNo = CaRESULT.iAreaNo;
	iVehBodyColorNo = CaRESULT.iVehBodyColorNo;
	iPlateTypeNo = CaRESULT.iPlateTypeNo;
	iVehBodyDeepNo = CaRESULT.iVehBodyDeepNo;
    iVehLogoType = CaRESULT.iVehLogoType;
	iDeviceID = CaRESULT.iDeviceID;
	iRoadNo = CaRESULT.iRoadNo;
	iSpeed = CaRESULT.iSpeed;
	iAxletreeType = CaRESULT.iAxletreeType;
	iAxletreeCount = CaRESULT.iAxletreeCount;
	iAxletreeGroupCount = CaRESULT.iAxletreeGroupCount;
	iResultNo = CaRESULT.iResultNo;
	iWheelCount = CaRESULT.iWheelCount;
    iReliability = CaRESULT.iReliability;
	fVehHeight = CaRESULT.fVehHeight;
	fVehLenth = CaRESULT.fVehLenth;
	fDistanceBetweenAxles = CaRESULT.fDistanceBetweenAxles;
    fConfidenceLevel = CaRESULT.fConfidenceLevel;
	bBackUpVeh = CaRESULT.bBackUpVeh;

	//pcAppendInfo = NULL;
    strAppendInfo = CaRESULT.strAppendInfo;

    memcpy(chDeviceIp, CaRESULT.chDeviceIp, sizeof(chDeviceIp));
    memcpy(chServerIP, CaRESULT.chServerIP, sizeof(chServerIP));
    memcpy(chPlateNO, CaRESULT.chPlateNO, sizeof(chPlateNO));
    memcpy(chPlateColor, CaRESULT.chPlateColor, sizeof(chPlateColor));
    memcpy(chListNo, CaRESULT.chListNo, sizeof(chListNo));
    memcpy(chPlateTime, CaRESULT.chPlateTime, sizeof(chPlateTime));
    memcpy(chSignStationID, CaRESULT.chSignStationID, sizeof(chSignStationID));
    memcpy(chSignStationName, CaRESULT.chSignStationName, sizeof(chSignStationName));
    memcpy(chSignDirection, CaRESULT.chSignDirection, sizeof(chSignDirection));
    memcpy(chDeviceID, CaRESULT.chDeviceID, sizeof(chDeviceID));
    memcpy(chLaneID, CaRESULT.chLaneID, sizeof(chLaneID));
    memcpy(chCarFace, CaRESULT.chCarFace, sizeof(chCarFace));
    memcpy(chChileLogo, CaRESULT.chChileLogo, sizeof(chChileLogo));
    //memcpy(pcAppendInfo, CaRESULT.pcAppendInfo, sizeof(pcAppendInfo));

	CIMG_BestCapture = CaRESULT.CIMG_BestCapture;
	CIMG_BeginCapture = CaRESULT.CIMG_BeginCapture;
	CIMG_LastCapture = CaRESULT.CIMG_LastCapture;
	CIMG_BestSnapshot = CaRESULT.CIMG_BestSnapshot;
	CIMG_LastSnapshot = CaRESULT.CIMG_LastSnapshot;
	CIMG_PlateImage = CaRESULT.CIMG_PlateImage;
	CIMG_BinImage = CaRESULT.CIMG_BinImage;
}

CameraResult::~CameraResult()
{
	dwCarID = 0;
	iDeviceID = 0;
	iPlateColor = 0;
	iPlateTypeNo = 0;
	dw64TimeMS = 0;
	iSpeed = 0;
	iResultNo = 0;
	iVehTypeNo = 0;		//车型代码: 客1--1 。。。客4--4， 货1--5  。。。货4--8
	iVehBodyColorNo = 0;	
	iVehBodyDeepNo = 0;	
    iVehLogoType = 0;
	iAreaNo = 0;
	iRoadNo = 0;
	iLaneNo = 0;
	iDirection = 0;
	iWheelCount = 0;		//轮数
	iAxletreeCount = 0;		//轴数
	iAxletreeGroupCount = 0;
	iAxletreeType = 0;		//轴型
    iReliability = 0;
	fVehLenth = 0;			//车长
	fDistanceBetweenAxles = 0.0;		//轴距
	fVehHeight = 0.0;		//车高
	bBackUpVeh = false;		//是否倒车

	memset(chDeviceIp, 0, sizeof(chDeviceIp));
	memset(chPlateNO, 0, sizeof(chPlateNO));
	memset(chPlateColor, 0, sizeof(chPlateColor));
	memset(chListNo, 0, sizeof(chListNo));
	memset(chPlateTime, 0, sizeof(chPlateTime));
	memset(chSignStationID, 0, sizeof(chSignStationID));
	memset(chSignStationName, 0, sizeof(chSignStationName));
	//memset(pcAppendInfo, 0, sizeof(pcAppendInfo));
	memset(chLaneID, 0, sizeof(chLaneID));
	memset(chDeviceID, 0, sizeof(chDeviceID));
}

CameraResult& CameraResult::operator=( const CameraResult& CaRESULT )
{
    if (this == &CaRESULT)
    {
        return *this;
    }
	dwCarID = CaRESULT.dwCarID;
	dw64TimeMS = CaRESULT.dw64TimeMS;
	iLaneNo = CaRESULT.iLaneNo;
	iPlateColor = CaRESULT.iPlateColor;
	iVehTypeNo = CaRESULT.iVehTypeNo;
    iVehSizeType = CaRESULT.iVehSizeType;
	iDirection = CaRESULT.iDirection;
	iAreaNo = CaRESULT.iAreaNo;
	iVehBodyColorNo = CaRESULT.iVehBodyColorNo;
	iPlateTypeNo = CaRESULT.iPlateTypeNo;
	iVehBodyDeepNo = CaRESULT.iVehBodyDeepNo;
    iVehLogoType = CaRESULT.iVehLogoType;
	iDeviceID = CaRESULT.iDeviceID;
	iRoadNo = CaRESULT.iRoadNo;
	iSpeed = CaRESULT.iSpeed;
	iAxletreeType = CaRESULT.iAxletreeType;
	iAxletreeCount = CaRESULT.iAxletreeCount;
	iAxletreeGroupCount = CaRESULT.iAxletreeGroupCount;
	iResultNo = CaRESULT.iResultNo;
	iWheelCount = CaRESULT.iWheelCount;
    iReliability = CaRESULT.iReliability;
	fVehHeight = CaRESULT.fVehHeight;
	fVehLenth = CaRESULT.fVehLenth;
	fDistanceBetweenAxles = CaRESULT.fDistanceBetweenAxles;
    fConfidenceLevel = CaRESULT.fConfidenceLevel;
	bBackUpVeh = CaRESULT.bBackUpVeh;

	//pcAppendInfo = NULL;
    strAppendInfo = CaRESULT.strAppendInfo;

    memcpy(chDeviceIp, CaRESULT.chDeviceIp, sizeof(chDeviceIp));
    memcpy(chServerIP, CaRESULT.chServerIP, sizeof(chServerIP));
    memcpy(chPlateNO, CaRESULT.chPlateNO, sizeof(chPlateNO));
    memcpy(chPlateColor, CaRESULT.chPlateColor, sizeof(chPlateColor));
    memcpy(chListNo, CaRESULT.chListNo, sizeof(chListNo));
    memcpy(chPlateTime, CaRESULT.chPlateTime, sizeof(chPlateTime));
    memcpy(chSignStationID, CaRESULT.chSignStationID, sizeof(chSignStationID));
    memcpy(chSignStationName, CaRESULT.chSignStationName, sizeof(chSignStationName));
    memcpy(chSignDirection, CaRESULT.chSignDirection, sizeof(chSignDirection));
    memcpy(chDeviceID, CaRESULT.chDeviceID, sizeof(chDeviceID));
    memcpy(chLaneID, CaRESULT.chLaneID, sizeof(chLaneID));
    memcpy(chCarFace, CaRESULT.chCarFace, sizeof(chCarFace));
    memcpy(chChileLogo, CaRESULT.chChileLogo, sizeof(chChileLogo));
    //memcpy(pcAppendInfo, CaRESULT.pcAppendInfo, sizeof(pcAppendInfo));

	CIMG_BestCapture = CaRESULT.CIMG_BestCapture;
	CIMG_BeginCapture = CaRESULT.CIMG_BeginCapture;
	CIMG_LastCapture = CaRESULT.CIMG_LastCapture;
	CIMG_BestSnapshot = CaRESULT.CIMG_BestSnapshot;
	CIMG_LastSnapshot = CaRESULT.CIMG_LastSnapshot;
	CIMG_PlateImage = CaRESULT.CIMG_PlateImage;
	CIMG_BinImage = CaRESULT.CIMG_BinImage;

	return *this;
}
