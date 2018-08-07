#ifndef VPR_COMMON_H
#define  VPR_COMMON_H


#define DLL_DEC PASCAL

#define BINARY_IMG_SIZE		280

/*
B.4  �����ֵ�
B.4.1  ���ƺ��ַ������壺
chPlate Ϊ��Ч�ĳ������ݣ����硰��A12345��������ʶ������������޳��ơ���
������ʾ��������A12345\0�������޳���\0����
ע����һ�����ֽڵĺ��ֱ�ʾ���ƹ����أ��硰������

chPlateColor��ʾ������ɫ��1���ֽڣ��������£�
0 - ��ɫ��1 - ��ɫ��2 - ��ɫ��3 - ��ɫ��4 - �̰׽���ɫ��5 - ���̽���ɫ��6 - ���׽���ɫ��

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
B.4.2  �������Ͷ���:
0 - ������
1 - 92ʽ���ó�������
2 - ���ó�������
3 - ���¾���������û�����ֳ������ͣ�
4 - 92ʽ�侯��������(����û�����ֳ�������)
5 - ���Ҿ�����������(һ�нṹ)
7 - 02ʽ���Ի���������
8 - ��ɫ˫��β�ƣ������򹫽���β�ƣ�
9 - 04ʽ�¾�������(���нṹ)
10 - ʹ�ݳ�������
11 - һ�нṹ����WJ��������
12 - ���нṹ����WJ��������
13 - ��ɫ1225ũ�ó�
14 - ��ɫ1325ũ�ó�
15 - ��ɫ1325ũ�ó�
16 - Ħ�г�
17 - ����Դ
B.4.3  ������ɫ���壺
0������ɫ��1����ɫ��2����(��)��3����(��)��4����ɫ��5����ɫ��6��������7����ɫ��8����ɫ��9����ɫ��10����ɫ��11.:��ɫ12 : ��ɫ

4.Υ�´��붨��
0������
1018 : ���������ڻ�����������ʻ��(��������)
1019�� ������Υ���涨ʹ��ר�ó�����(Ԥ��)
1208�����������־��ʻ��Υ����ת��Υ��ֱ�С�Υ����ת�ȣ�
1211 : ͨ��·����ֹͣ�ź�ʱ��ͣ��ֹͣ�����ڻ�·���ڵ�(ѹֹͣ�ߣ�Ԥ����ʱ���ṩ)
1229��Υ�������־��Ԥ����Ŀǰû���ṩ����Υ�´��룩
1230����������ֹ����ָʾ��ѹ�����ߵȣ�
1301������
1302��Υ���źŵƣ�����ƣ�
1042�������������涨������ʻ
1303����������ʻ�����涨ʱ��50%����
1603����������ʻ�����涨ʱ��50%
5.������
����(0)�����У�1�����ɶ�����(2)��������(3)��������(4)���ɱ�����(5)
��������
0�����ڣ�1���羯������ƣ��������ϰ汾����Υ�����͵ģ��°汾�ն˳�����Υ�´��루���궨�壩���������ʱ����Υ�������ֶΡ�
7�������ֶ�
�������ͣ�0 - ����������1 - С�ͳ���2 - ���ͳ���3 - ���˴�����4 - ���ֳ�������5 - ���ֳ������� 6 - ����������
������ɫ��ǳ��0 - ��ɫ��1 - ǳɫ
������ɫ��0 - ����ɫ��1 - ��ɫ��2 - ��ɫ��3 - ��ɫ��4 - ��ɫ��5 - ��ɫ��6 - ������7 - ��ɫ��8 - ��ɫ��9 - ��ɫ�� 10 - ��ɫ��11 - ��ɫ��12 - ��ɫ��0xff - δ���г�����ɫʶ��

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

byVehicleLogoRecog����Ʒ�ƣ������б�

VLR_OTHER = 0,    //����
VLR_VOLKSWAGEN = 1,    //����
VLR_BUICK = 2,    //���
VLR_BMW = 3,    //����
VLR_HONDA = 4,    //����
VLR_PEUGEOT = 5,    //����
VLR_TOYOTA = 6,    //����
VLR_FORD = 7,    //����
VLR_NISSAN = 8,    //�ղ�
VLR_AUDI = 9,    //�µ�
VLR_MAZDA = 10,   //���Դ�
VLR_CHEVROLET = 11,   //ѩ����
VLR_CITROEN = 12,   //ѩ����
VLR_HYUNDAI = 13,   //�ִ�
VLR_CHERY = 14,   //����
VLR_KIA = 15,   //����
VLR_ROEWE = 16,   //����
VLR_MITSUBISHI = 17,   //����
VLR_SKODA = 18,   //˹�´�
VLR_GEELY = 19,   //����
VLR_ZHONGHUA = 20,   //�л�
VLR_VOLVO = 21,   //�ֶ���
VLR_LEXUS = 22,   //�׿���˹
VLR_FIAT = 23,   //������
VLR_EMGRAND = 24,   //�ۺ�
VLR_DONGFENG = 25,   //����
VLR_BYD = 26,   //���ǵ�
VLR_SUZUKI = 27,   //��ľ
VLR_JINBEI = 28,   //��
VLR_HAIMA = 29,   //����
VLR_SGMW = 30,   //����
VLR_JAC = 31,   //����
VLR_SUBARU = 32,   //˹��³
VLR_ENGLON = 33,   //Ӣ��
VLR_GREATWALL = 34,   //����
VLR_HAFEI = 35,   //����
VLR_ISUZU = 36,   //��ʮ��
VLR_SOUEAST = 37,   //����
VLR_CHANA = 38,   //����
VLR_FOTON = 39,   //����
VLR_XIALI = 40,   //����
VLR_BENZ = 41,   //����
VLR_FAW = 42,   //һ��
VLR_NAVECO = 43,   //��ά��
VLR_LIFAN = 44,   //����
VLR_BESTURN = 45,   //һ������
VLR_CROWN = 46,   //�ʹ�
VLR_RENAULT = 47,   //��ŵ
VLR_JMC = 48,   //JMC
VLR_MG = 49,   //MG ����
VLR_KAMA = 50,   //����
VLR_ZOTYE = 51,   //��̩
VLR_CHANGHE = 52,   //����
VLR_XMKINGLONG = 53,   //���Ž���
VLR_HUIZHONG = 54,   //�Ϻ�����
VLR_SZKINGLONG = 55,   //���ݽ���
VLR_HIGER = 56,   //����
VLR_YUTONG = 57,   //��ͨ
VLR_CNHTC = 58,   //�й�����
VLR_BEIBEN = 59,   //�����ؿ�
VLR_XINGMA = 60,   //��������
VLR_YUEJIN = 61,   //Ծ��
VLR_HUANGHAI = 62   //�ƺ�
*/



typedef struct _tagTrafficData
{
    DWORD		dwVersion;					//Э��汾�ţ��汾��Ϊ1
    char		chTSIP[20];					//�ն˷�����IP
    char		chPlate[20];				//���ƺ��룬����Ϊ20���ַ�������
    BYTE		chPlateColor;				//������ɫ
    BYTE		byBinImg[BINARY_IMG_SIZE];  //��ֵͼ
    DWORD		dwLaneNum;					//�������
    DWORD		dwSpeed;					//�г��ٶ�
    DWORD		dwPassTime;					//����ʱ��
    DWORD		dwDir;						//�г�����
    DWORD		dwDataType;					//��������
    DWORD		dwDefense;					//Υ�´���
    DWORD		dwCameraID;					//������
    DWORD		dwPlateType;                //��������              
    DWORD       dwVehicleColor;             //������ɫ				
    DWORD	    dwPlateImgLen;				//����ͼ����
    BYTE		*pPlateJpegImg;				//����ͼ
    DWORD		dwCarJpegLen1;				//����JPEGͼ1����
    BYTE		*pCarJpegImg1;			    //����JPEGͼ1
    DWORD		dwCarJpegLen2;				//����JPEGͼ2����
    BYTE		*pCarJpegImg2;				//����JPEGͼ2
    DWORD		dwCarJpegLen3;				//����JPEGͼ3����
    BYTE		*pCarJpegImg3;			    //����JPEGͼ3	

    //��������
    BYTE       byVehicleType; 				//��������
    BYTE       byVehicleKind; 				//�������࣬0-δ֪��1�ͳ���2����
    BYTE       byColorDepth;   				//������ɫ��ǳ��0-��ɫ��1-ǳɫ  
    BYTE       byColor;   					//������ɫ
    BYTE       byRes1;   					    //����
    WORD       wLength;   				    //������
    BYTE       byVehicleLogoRecog;   		    //����Ʒ��
    BYTE       byVehicleSubLogoRecog[32];  	//������Ʒ��
    BYTE       byVehicleModel;   			    //������Ʒ�����
    BYTE       byExhaust[256];				//�����ŷ���Ϣ   
    BYTE       byCustomInfo[256];   			//�Զ�����Ϣ
    BYTE       byRes3[32]; 					//����

}TRAFFICDATA, *LPTRAFFICDATA;

//�豸״̬
typedef struct _tagDeviceState
{
    char   	chIP[20];			//�ն˷�����IP
    DWORD  dwServerStatus;	    //�ն˷�����״̬ 0:���ϣ�1:����������2:�����쳣
    DWORD  dwCameraNum;			//�������
    char   	chCamIP[2000];		//���IP
    BYTE   byCamStatus[100];	//���״̬ 0:���ϣ�1:����������2:�����쳣
    BYTE   byVehicleSensorStatus[100];//������״̬ 0:���ϣ�1:����������2:�����쳣
    BYTE   byLoopStatus[100];		//��Ȧ״̬ 0:���ϣ�1:����������2:�����쳣
}STDEVSTATE, *LPSTDEVSTATE;

//��ͨ���ݻص�����ԭ��
typedef void (CALLBACK* VPR_GetVehicleInfo)(LPTRAFFICDATA pTrafficData);

//�豸״̬�ص�����ԭ��
typedef void (CALLBACK* VPR_GetStatus)(LPSTDEVSTATE pDevState);

#endif