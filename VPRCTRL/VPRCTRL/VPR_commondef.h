#ifndef VPR_COMMON_H
#define  VPR_COMMON_H


#define DLL_DEC PASCAL

#define BINARY_IMG_SIZE		280

/*
B.4  数据字典
B.4.1  车牌号字符串意义：
chPlate 为有效的车牌数据，比如“沪A12345”，若无识别结果，输出“无车牌”。
输出结果示例：“沪A12345\0”、“无车牌\0”。
注：第一二个字节的汉字表示车牌归属地，如“沪”。

chPlateColor表示车牌颜色：1个字节，定义如下：
0 - 蓝色，1 - 黄色，2 - 黑色，3 - 白色，4 - 绿白渐变色，5 - 黄绿渐变色，6 - 蓝白渐变色。

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
B.4.2  车牌类型定义:
0 - 无类型
1 - 92式民用车牌类型
2 - 警用车牌类型
3 - 上下军车（现在没有这种车牌类型）
4 - 92式武警车牌类型(现在没有这种车牌类型)
5 - 左右军车车牌类型(一行结构)
7 - 02式个性化车牌类型
8 - 黄色双行尾牌（货车或公交车尾牌）
9 - 04式新军车类型(两行结构)
10 - 使馆车牌类型
11 - 一行结构的新WJ车牌类型
12 - 两行结构的新WJ车牌类型
13 - 黄色1225农用车
14 - 绿色1325农用车
15 - 黄色1325农用车
16 - 摩托车
17 - 新能源
B.4.3  车身颜色定义：
0：其他色；1：白色；2：灰(银)；3：灰(银)；4：黑色；5：红色；6：深蓝；7：蓝色；8：黄色；9：绿色；10：棕色；11.:粉色12 : 紫色

4.违章代码定义
0：正常
1018 : 机动车不在机动车道内行驶的(车道禁行)
1019： 机动车违反规定使用专用车道的(预留)
1208：不按导向标志行驶（违章左转、违章直行、违章右转等）
1211 : 通过路口遇停止信号时，停在停止线以内或路口内的(压停止线，预留暂时不提供)
1229：违反禁令标志（预留，目前没有提供此类违章代码）
1230：机动车禁止标线指示（压车道线等）
1301：逆行
1302：违反信号灯（闯红灯）
1042：机动车不按规定车道行驶
1303：机动车行驶超过规定时速50%以下
1603：机动车行驶超过规定时速50%
5.方向编号
下行(0)，上行（1），由东向西(2)，由西向东(3)，由南向北(4)，由北向南(5)
数据类型
0：卡口；1：电警（闯红灯），这是老版本区分违章类型的，新版本终端程序都以违章代码（国标定义）输出，开发时可用违章类型字段。
7、车型字段
车辆类型：0 - 其他车辆，1 - 小型车，2 - 大型车，3 - 行人触发，4 - 二轮车触发，5 - 三轮车触发， 6 - 机动车触发
车身颜色深浅，0 - 深色，1 - 浅色
车身颜色：0 - 其他色，1 - 白色，2 - 银色，3 - 灰色，4 - 黑色，5 - 红色，6 - 深蓝，7 - 蓝色，8 - 黄色，9 - 绿色， 10 - 棕色，11 - 粉色，12 - 紫色，0xff - 未进行车身颜色识别

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

byVehicleLogoRecog汽车品牌，如下列表

VLR_OTHER = 0,    //其它
VLR_VOLKSWAGEN = 1,    //大众
VLR_BUICK = 2,    //别克
VLR_BMW = 3,    //宝马
VLR_HONDA = 4,    //本田
VLR_PEUGEOT = 5,    //标致
VLR_TOYOTA = 6,    //丰田
VLR_FORD = 7,    //福特
VLR_NISSAN = 8,    //日产
VLR_AUDI = 9,    //奥迪
VLR_MAZDA = 10,   //马自达
VLR_CHEVROLET = 11,   //雪佛兰
VLR_CITROEN = 12,   //雪铁龙
VLR_HYUNDAI = 13,   //现代
VLR_CHERY = 14,   //奇瑞
VLR_KIA = 15,   //起亚
VLR_ROEWE = 16,   //荣威
VLR_MITSUBISHI = 17,   //三菱
VLR_SKODA = 18,   //斯柯达
VLR_GEELY = 19,   //吉利
VLR_ZHONGHUA = 20,   //中华
VLR_VOLVO = 21,   //沃尔沃
VLR_LEXUS = 22,   //雷克萨斯
VLR_FIAT = 23,   //菲亚特
VLR_EMGRAND = 24,   //帝豪
VLR_DONGFENG = 25,   //东风
VLR_BYD = 26,   //比亚迪
VLR_SUZUKI = 27,   //铃木
VLR_JINBEI = 28,   //金杯
VLR_HAIMA = 29,   //海马
VLR_SGMW = 30,   //五菱
VLR_JAC = 31,   //江淮
VLR_SUBARU = 32,   //斯巴鲁
VLR_ENGLON = 33,   //英伦
VLR_GREATWALL = 34,   //长城
VLR_HAFEI = 35,   //哈飞
VLR_ISUZU = 36,   //五十铃
VLR_SOUEAST = 37,   //东南
VLR_CHANA = 38,   //长安
VLR_FOTON = 39,   //福田
VLR_XIALI = 40,   //夏利
VLR_BENZ = 41,   //奔驰
VLR_FAW = 42,   //一汽
VLR_NAVECO = 43,   //依维柯
VLR_LIFAN = 44,   //力帆
VLR_BESTURN = 45,   //一汽奔腾
VLR_CROWN = 46,   //皇冠
VLR_RENAULT = 47,   //雷诺
VLR_JMC = 48,   //JMC
VLR_MG = 49,   //MG 名爵
VLR_KAMA = 50,   //凯马
VLR_ZOTYE = 51,   //众泰
VLR_CHANGHE = 52,   //昌河
VLR_XMKINGLONG = 53,   //厦门金龙
VLR_HUIZHONG = 54,   //上海汇众
VLR_SZKINGLONG = 55,   //苏州金龙
VLR_HIGER = 56,   //海格
VLR_YUTONG = 57,   //宇通
VLR_CNHTC = 58,   //中国重汽
VLR_BEIBEN = 59,   //北奔重卡
VLR_XINGMA = 60,   //华菱星马
VLR_YUEJIN = 61,   //跃进
VLR_HUANGHAI = 62   //黄海
*/



typedef struct _tagTrafficData
{
    DWORD		dwVersion;					//协议版本号，版本号为1
    char		chTSIP[20];					//终端服务器IP
    char		chPlate[20];				//车牌号码，长度为20的字符型数组
    BYTE		chPlateColor;				//车牌颜色
    BYTE		byBinImg[BINARY_IMG_SIZE];  //二值图
    DWORD		dwLaneNum;					//车道编号
    DWORD		dwSpeed;					//行车速度
    DWORD		dwPassTime;					//过车时间
    DWORD		dwDir;						//行车方向
    DWORD		dwDataType;					//数据类型
    DWORD		dwDefense;					//违章代码
    DWORD		dwCameraID;					//相机编号
    DWORD		dwPlateType;                //车牌类型              
    DWORD       dwVehicleColor;             //车辆颜色				
    DWORD	    dwPlateImgLen;				//车牌图长度
    BYTE		*pPlateJpegImg;				//车牌图
    DWORD		dwCarJpegLen1;				//车辆JPEG图1长度
    BYTE		*pCarJpegImg1;			    //车辆JPEG图1
    DWORD		dwCarJpegLen2;				//车辆JPEG图2长度
    BYTE		*pCarJpegImg2;				//车辆JPEG图2
    DWORD		dwCarJpegLen3;				//车辆JPEG图3长度
    BYTE		*pCarJpegImg3;			    //车辆JPEG图3	

    //车型数据
    BYTE       byVehicleType; 				//车辆类型
    BYTE       byVehicleKind; 				//车辆种类，0-未知，1客车，2货车
    BYTE       byColorDepth;   				//车身颜色深浅，0-深色，1-浅色  
    BYTE       byColor;   					//车身颜色
    BYTE       byRes1;   					    //保留
    WORD       wLength;   				    //车身长度
    BYTE       byVehicleLogoRecog;   		    //汽车品牌
    BYTE       byVehicleSubLogoRecog[32];  	//车辆子品牌
    BYTE       byVehicleModel;   			    //车辆子品牌年款
    BYTE       byExhaust[256];				//车辆排放信息   
    BYTE       byCustomInfo[256];   			//自定义信息
    BYTE       byRes3[32]; 					//保留

}TRAFFICDATA, *LPTRAFFICDATA;

//设备状态
typedef struct _tagDeviceState
{
    char   	chIP[20];			//终端服务器IP
    DWORD  dwServerStatus;	    //终端服务器状态 0:故障；1:连接正常；2:连接异常
    DWORD  dwCameraNum;			//相机个数
    char   	chCamIP[2000];		//相机IP
    BYTE   byCamStatus[100];	//相机状态 0:故障；1:连接正常；2:连接异常
    BYTE   byVehicleSensorStatus[100];//车检器状态 0:故障；1:连接正常；2:连接异常
    BYTE   byLoopStatus[100];		//线圈状态 0:故障；1:连接正常；2:连接异常
}STDEVSTATE, *LPSTDEVSTATE;

//交通数据回调函数原型
typedef void (CALLBACK* VPR_GetVehicleInfo)(LPTRAFFICDATA pTrafficData);

//设备状态回调函数原型
typedef void (CALLBACK* VPR_GetStatus)(LPSTDEVSTATE pDevState);

#endif