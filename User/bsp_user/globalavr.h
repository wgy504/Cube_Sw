#ifndef  __GLOBALAVR_H__
#define  __GLOBALAVR_H__

#include <includes.h>


void ADCS_Init_VAR(void);
void EPS_Init_VAR(void);
void bsp_Init_VAR(void);
void VarRov(void);
void VarBak(void);

/* AD����ͨ�� */
#define OBC_5V             0                /* �����5V��Դ���� ��ѹ��1:2 */
#define OBC_SUN1           1                /* ��̫��1ͨ�� */
#define OBC_SUN2           2                /* ��̫��2ͨ�� */
#define OBC_SUN3           4                /* ��̫��3ͨ�� */
#define OBC_SUN4           3                /* ��̫��4ͨ�� */
#define OBC_SUN5           6                /* ��̫��5ͨ�� */
#define OBC_SUN6           5                /* ��̫��6ͨ�� */
#define OBC_TEMP1          7                /* �¶ȴ�����1ͨ�� */
#define OBC_TEMP2          8                /* �¶ȴ�����2ͨ�� */
#define OBC_POWER          9                /* �����3.3V��Դ���� ��ѹ��1:1 */          
#define OBC_EXPAND1        10               /* �����AD��չ1 */
#define OBC_EXPAND2        11               /* �����AD��չ2 */
#define OBC_EXPAND3        12               /* �����AD��չ3 */
#define OBC_EXPAND4        13               /* �����AD��չ4 */
#define OBC_EXPAND5        14               /* �����AD��չ5 */
#define OBC_EXPAND6        15               /* �����AD��չ6 */

/* ���ڽ��ջ�������С */ 
#define BUFFER_SIZE                13               /* ָ����ջ�������С */
#define GPS_MAX_REV_SIZE           800              /* GPS���ջ�������С */

/* ң��洢 */
#define TEL_STO_ADDR               NOR_FLASH_SA0 			/* ң�����ݴ洢�׵�ַ */
#define TEL_STO_RAG                0x00100000   		  /* ң�����ݴ洢��ַ��Χ */

/* GPS�洢 */
#define GPS_STO_ADDR               NOR_FLASH_SA32 			/* GPS���ݴ洢�׵�ַ */
#define GPS_STO_RAG                0x00100000 				/* GPS���ݴ洢��ַ��Χ */

/* �غɴ洢 */
#define PLOAD_STO_ADDR             0x8000
#define PLOAD_STO_RAG              0x80000000
														
/* FLASH�¶ȴ洢�׵�ַ */ 
#define WRITE_READ_ADDR            0x8000
#define GT_RCV_SIZE                30

/* �ⲿFLASH��ƫ�Ƶ�ַ */
#define NOR_FLASH_SA0              0x00000000
#define NOR_FLASH_SA1              0x00008000
#define NOR_FLASH_SA2              0x00010000
#define NOR_FLASH_SA3              0x00018000
#define NOR_FLASH_SA4              0x00020000
#define NOR_FLASH_SA5              0x00028000
#define NOR_FLASH_SA6              0x00030000
#define NOR_FLASH_SA7              0x00038000
#define NOR_FLASH_SA8              0x00040000
#define NOR_FLASH_SA9              0x00048000
#define NOR_FLASH_SA10             0x00050000
#define NOR_FLASH_SA11             0x00058000
#define NOR_FLASH_SA12             0x00060000
#define NOR_FLASH_SA13             0x00068000
#define NOR_FLASH_SA14             0x00070000
#define NOR_FLASH_SA15             0x00078000
#define NOR_FLASH_SA16             0x00080000
#define NOR_FLASH_SA17             0x00088000
#define NOR_FLASH_SA18             0x00090000
#define NOR_FLASH_SA19             0x00098000
#define NOR_FLASH_SA20             0x000A0000
#define NOR_FLASH_SA21             0x000A8000
#define NOR_FLASH_SA22             0x000B0000
#define NOR_FLASH_SA23             0x000B8000
#define NOR_FLASH_SA24             0x000C0000
#define NOR_FLASH_SA25             0x000C8000
#define NOR_FLASH_SA26             0x000D0000
#define NOR_FLASH_SA27             0x000D8000
#define NOR_FLASH_SA28             0x000E0000
#define NOR_FLASH_SA29             0x000E8000
#define NOR_FLASH_SA30             0x000F0000
#define NOR_FLASH_SA31             0x000F8000
#define NOR_FLASH_SA32             0x00100000
#define NOR_FLASH_SA33             0x00108000
#define NOR_FLASH_SA34             0x00110000
#define NOR_FLASH_SA35             0x00118000
#define NOR_FLASH_SA36             0x00120000
#define NOR_FLASH_SA37             0x00128000
#define NOR_FLASH_SA38             0x00130000
#define NOR_FLASH_SA39             0x00138000
#define NOR_FLASH_SA40             0x00140000
#define NOR_FLASH_SA41             0x00148000
#define NOR_FLASH_SA42             0x00150000
#define NOR_FLASH_SA43             0x00158000
#define NOR_FLASH_SA44             0x00160000
#define NOR_FLASH_SA45             0x00168000
#define NOR_FLASH_SA46             0x00170000
#define NOR_FLASH_SA47             0x00178000
#define NOR_FLASH_SA48             0x00180000
#define NOR_FLASH_SA49             0x00188000
#define NOR_FLASH_SA50             0x00190000
#define NOR_FLASH_SA51             0x00198000
#define NOR_FLASH_SA52             0x001A0000
#define NOR_FLASH_SA53             0x001A8000
#define NOR_FLASH_SA54             0x001B0000
#define NOR_FLASH_SA55             0x001B8000
#define NOR_FLASH_SA56             0x001C0000
#define NOR_FLASH_SA57             0x001C8000
#define NOR_FLASH_SA58             0x001D0000
#define NOR_FLASH_SA59             0x001D8000
#define NOR_FLASH_SA60             0x001E0000
#define NOR_FLASH_SA61             0x001E8000
#define NOR_FLASH_SA62             0x001F0000


						
struct OBCBootInfoStr
{
	char BootCnt;             /* ���������������� */
	char BootPINRSTCnt;       /* �����������Ÿ�λ���� */
	char BootPORRSTCnt;       //
	char BootSFTRSTCnt;       /* ��������������λ���� */
	char BootIWDGRSTCnt;      /* ���������������Ź���λ���� */
	char BootWWDGRSTCnt;      /* �����������ڿ��Ź���λ���� */
	char BootLPWRRSTCnt;      
	/*******************/
	char BootRTC_Source;      /* RTCʱ��Դ��0�ⲿ����1�ڲ�����2û������*/
	char BootLSE_Error;       /* �ⲿʱ�Ӵ������ */
	char BootLSI_Error;       /* �ڲ�ʱ�Ӵ������ */
};


struct OBCStoreInfoStr
{
	unsigned char SectorInUse;             /* ����ʹ�ñ�־λ */
	
	struct
	{
		unsigned char Empty             :1; /* ���б�־λ */
		unsigned char Full              :1; /* ������־λ */
		unsigned char Downed            :1; /* ���ȼ� */
	  unsigned char Broke             :1; /* ������ */
		unsigned char EraserTime        :4; /* �������� */
	}SectorInfo[32];
	
};


struct OBCTaskStatus
{
		unsigned char CpuUsage;    
		CPU_STK_SIZE StackFree;    
	  CPU_STK_SIZE StackUsed;
		CPU_TS TimeStamp;           
};

/* �����ⲿ������ʱ��ʱ�� */
extern OS_TMR  GPS_OT_TIMER;
extern OS_TMR  TEST_OT_TIMER;

/* GPS�洢�źŵ� */
extern OS_SEM   SEM_GPS_STO;	
extern OS_SEM   SEM_TEL_STO;
extern OS_SEM   SEM_PLOAD_STO;

/* ����GPS���ջ������ */
extern CPU_INT08U GpsRevBuf[GPS_MAX_REV_SIZE]; 
extern volatile CPU_INT16U GpsRevCnt;
extern NMEA_MSG GpsCurInfo;            
/* �ⲿ�������� */
/* ����ָ�����״̬�� */
extern CPU_INT08U IntBuf[BUFFER_SIZE];            /* ָ����ջ������ */
extern CPU_INT16U InsCnt;                         /* ָ����ռ���ֵ */
extern CPU_INT08U InsBuf_p;                       /* ָ��ȫ��ָ�� */								
extern CPU_INT08U ID_CommandBuf[GT_RCV_SIZE];
extern CPU_INT08U ID_CommandCnt;				

/* �������ģת��״̬�� */																		
extern CPU_INT16U ObcAdValue[16][6];              /* ��Դ��ȡ�� */
extern CPU_INT08U ObcAdErr[16];
extern CPU_INT08U ObcCommErr;
																		
/* ���ϴ洢ָ��״̬�� */
extern CPU_INT32U TelCurPtr;                      /* ��ǰң��洢ָ�� */
extern CPU_INT32U GpsCurPtr;                      /* ��ǰGPS�洢ָ�� */
extern CPU_INT32U PldCurPtr;                      /* ��ǰ�غɴ洢ָ�� */
extern CPU_INT08U TelOverRangeCnt;                /* ң��ָ��Խ����� */
extern CPU_INT08U GpsOverRangeCnt;                /* GPSָ��Խ����� */
extern CPU_INT08U PldOverRangeCnt;                /* �غ�ָ��Խ����� */
extern CPU_INT16U TelCurStrCnt;                   /* ��ǰң��洢�� */
extern CPU_INT16U GpsCurStrCnt;                   /* ��ǰGPS�洢�� */
extern CPU_INT16U PldCurStrCnt;                   /* ��ǰ�غɴ洢�� */
extern struct OBCStoreInfoStr GpsStrInfo;
extern struct OBCStoreInfoStr TelStrInfo;

/* ����ȫ��ʱ����� */
extern RTC_TimeTypeDef   CurTime;                 /* ��ǰ����ʱ�� */
extern RTC_DateTypeDef   CurDate;                 /* ��ǰ�������� */

/* ���Ͽ���״̬��־λ */
extern struct OBCBootInfoStr OBCBootInfo;         /* OBC����״̬��Ϣ */
extern CPU_INT08U LowVolCnt;                      /* ��ԴǷѹ���� */

/* �弶ͨ�ű��� */
extern CPU_INT08U EpsTranOTCnt;                   /* EPS���䳬ʱ���� */
extern CPU_INT08U EpsRevOTCnt;                    /* EPS������ռ��� */
extern CPU_INT08U ComTranOTCnt;                   /* COM���䳬ʱ���� */
extern CPU_INT08U ComRevOTCnt;                    /* COM���䳬ʱ���� */

/* ������������� */
extern CPU_INT32U MotorSetOutput;
extern CPU_INT32U MotorCurOutput;

extern CPU_INT08U MotorCommErr;
extern CPU_INT16U PwmSetDuty_1;
extern CPU_INT16U PwmSetDuty_2;
extern CPU_INT16U PwmSetDuty_3;
extern CPU_INT16U PwmCurDuty_1;
extern CPU_INT16U PwmCurDuty_2;
extern CPU_INT16U PwmCurDuty_3;
/* ���������ļ�ϵͳ */
extern FRESULT f_result;
extern FATFS f_fs;
extern FIL f_file;

/********************δ����**************************/
/* �������� */
extern CPU_INT32U RandNum_32;         /* 32λ������� */
extern CPU_INT32U CurFlashSetor;
extern CPU_INT08U FlashTestErrCnt;
extern CPU_INT08U TFCardTestErrCnt;
extern char DebugFlag;


extern CPU_INT16U EpsAdValue[32][6];
extern CPU_INT08U EpsAdErr[32];
extern CPU_INT08U EpsCommErr;
extern volatile CPU_INT08U BatInC;
extern volatile CPU_INT08U BatOutC;
extern volatile CPU_INT08U BusC;
extern volatile CPU_INT08U ComC;
extern volatile CPU_INT08U V3C;
extern volatile CPU_INT08U V5C; 
extern volatile CPU_INT08U EPSC;
extern volatile CPU_INT08U WhlAC; 
extern volatile CPU_INT08U WhlBC;  
extern volatile CPU_INT08U MtqC;  
extern volatile CPU_INT08U USBC;
extern volatile CPU_INT08U GPSAC; 
extern volatile CPU_INT08U GPSBC; 
extern volatile CPU_INT08U MagC; 
extern volatile CPU_INT08U PldC1;
extern volatile CPU_INT08U PldC2;
extern volatile CPU_INT08U PldC3;
extern volatile CPU_INT08U PldC4;
extern volatile CPU_INT08U PldC5;
extern volatile CPU_INT08U PldC6;
extern volatile CPU_INT08U PldC7;
extern volatile CPU_INT08U CellC1;
extern volatile CPU_INT08U CellC2;
extern volatile CPU_INT08U CellC3;
extern volatile CPU_INT08U CellC4;
extern volatile CPU_INT08U CellC5;
extern volatile CPU_INT08U CellC6;
extern volatile CPU_INT08U SplyV1;
extern volatile CPU_INT08U SplyV2;
extern volatile CPU_INT08U BatV1;
extern volatile CPU_INT08U BatV2;
extern volatile CPU_INT08U V3V1;
extern volatile CPU_INT08U V5V1;
extern volatile CPU_INT08U V3V2;
extern volatile CPU_INT08U V5V2;
extern volatile CPU_INT08U Temp1;
extern volatile CPU_INT08U Temp2;
extern volatile CPU_INT08U Temp3;
extern volatile CPU_INT08U Temp4;
extern volatile CPU_INT08U Temp5;
extern volatile CPU_INT08U Temp6;
extern volatile CPU_INT08U Temp7;
extern volatile CPU_INT08U Temp8;
/* ��??��?a1?��? */
extern volatile CPU_INT16U SwSubSys;
extern volatile CPU_INT16U SwPld;
extern volatile CPU_INT16U FalSubSys;
extern volatile CPU_INT16U FalPld;

/* ��Դ��غ� */

/* ���غ� */
#define SW_WHL_A                   0x8000 
#define SW_WHL_B  								 0x4000
#define SW_MTQ  									 0x2000
#define SW_USB  									 0x1000
#define SW_GPS_A  								 0x0800
#define SW_GPS_B  								 0x0400
#define SW_MAG  									 0x0200
#define SW_BAT_A 								   0x0100
#define SW_BAT_B  						  	 0x0080

#define SW_PLD1                    0x8000 
#define SW_PLD2                    0x4000 
#define SW_PLD3                    0x2000 
#define SW_PLD4                    0x1000 
#define SW_PLD5                    0x0800 
#define SW_PLD6                    0x0400 
#define SW_PLD7                    0x0200 
/* ���Ϻ� */
#define FAL_WHL_A                  0x8000 
#define FAL_WHL_B  								 0x4000
#define FAL_MTQ  									 0x2000
#define FAL_USB  									 0x1000
#define FAL_GPS_A  								 0x0800
#define FAL_GPS_B  								 0x0400
#define FAL_MAG  									 0x0200
#define FAL_BAT_A 								 0x0100
#define FAL_BAT_B  						  	 0x0080
#define FAL_COM  									 0x0040
#define FAL_3V3_1 								 0x0020
#define FAL_5V_1  						  	 0x0010

#define FAL_PLD1                   0x8000 
#define FAL_PLD2                   0x4000 
#define FAL_PLD3                   0x2000 
#define FAL_PLD4                   0x1000 
#define FAL_PLD5                   0x0800 
#define FAL_PLD6                   0x0400 
#define FAL_PLD7                   0x0200 



/* ��վָ�� */
#define INS_CONN_TST           0x01    //ͨ����·����
#define INS_COMM_SWITCH_CLR    0x02    //���ؼ����12Сʱ����
/* ��������ָ�� */
#define INS_DOWN_TEL           0x11    //��������״̬ң������
#define INS_DOWN_PLD           0x12    //����QB50�غ�����
#define INS_DOWN_CMD           0x13    //��������ָ������
/* ���翪��ָ�� */
#define INS_MAG_ON             0x21    //��ǿ�ƿ�
#define INS_MAG_OFF            0x22		 //��ǿ�ƹ�
#define INS_GPS_A_ON           0x23		 //GPSA��
#define INS_GPS_A_OFF          0x24		 //GPSA��
#define INS_GPS_B_ON           0x25		 //GPSB��
#define INS_GPS_B_OFF          0x26		 //GPSB��
#define INS_RSV_ON             0x27		 //Ӧ������ջ���
#define INS_RSV_OFF            0x28		 //Ӧ������ջ���
#define INS_TRAN_ON            0x29		 //Ӧ����������
#define INS_TRAN_OFF           0x2A		 //Ӧ����������
#define INS_GAMA_ON            0x2B		 //gamalink��
#define INS_GAMA_OFF           0x2C		 //gamalink��
#define INS_ANT_ON             0x2D		 //����չ��
#define INS_SLBRD_ON           0x2E		 //����չ��
#define INS_MTQ_ON             0x2F		 //�Ű���
#define INS_MTQ_OFF            0x31		 //�Ű���
#define INS_MW_A_ON            0x32		 //������A��
#define INS_MW_A_OFF           0x33		 //������A��
#define INS_MW_B_ON            0x34		 //������B��
#define INS_MW_B_OFF           0x35		 //������B��
#define INS_PLD_A_ON           0x36    //����A��
#define INS_PLD_A_OFF          0x37		 //����A��
/* �˿�ϵͳָ�� */
#define INS_DET                0x41    //��������
#define INS_STA                0x42		 //��������ʹ��
#define INS_DUMP_FOEV_DIS      0x43    //���������ֹ
#define INS_SW_MAG_A           0x44    //�л�����ǿ��A
#define INS_SW_MAG_B           0x45    //�л�����ǿ��B
/* ͨ��ϵͳָ�� */
#define INS_SW_1200            0x51    //BPSK1200�л�
#define INS_SW_9600            0x52		 //BPSK9600��
#define INS_CW_ON              0x53    //CW��
#define INS_COM_TRAN_OFF       0x54    //ͨ�Ż�������ػ�
/* �����ۺ�ָ�� */
#define INS_SD_CLR             0x61    //SD�����
#define INS_FLASH_RST          0x62    //FLASHָ�뻹ԭ
#define INS_OBC_RST            0x63    //������������

//******����ע��ָ��*****//
/* �˿�ϵͳָ�� */
#define INS_CTL_P_PRA          0x71    //�����ȶ�������ע��
#define INS_CTL_D_PRA          0x72    //�����ȶ�������ע��
#define INS_ZJD_CTL            0x73    //�½�������ϵ��
//**************************
#define INS_DMP_FLAG           0x74    //�����־λ
#define INS_FLT_FLAG           0x75    //������־λ
#define INS_CTL_FLAG           0x76    //���Ʊ�־λ
#define INS_CNT_DMP_FLAG       0x77    //���������
#define INS_CNT_FLT_FLAG       0x78    //����������
#define INS_CNT_CTL_FLAG       0x79    //���Ƽ�����
/**************************/
#define INS_EQUP_INPUT         0x81    //��������״̬���ָ��
#define INS_SD_ADRS            0x82    //����SD����ַ���
#define INS_TEL_ADRS           0x83    //ң��洢ָ����
#define INS_GPS_ADRS           0x84    //GPS�洢ָ����
#define INS_TIME_IN            0x91    //ʱ��ע��
/*********************/
#endif