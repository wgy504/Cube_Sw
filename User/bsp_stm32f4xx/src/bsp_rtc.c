#include "bsp.h"
#include "globalavr.h"

/* ѡ��RTC��ʱ��Դ */


/* ���� */
#define RTC_TIMEOUT_US 100000
/* ��������RTC��Ƶ */
__IO uint32_t uwAsynchPrediv = 0;
__IO uint32_t uwSynchPrediv = 0;
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitRTC
*	����˵��: ��ʼ��RTC
*	��    �Σ���
*	�� �� ֵ: ��		        
*********************************************************************************************************
*/
void bsp_InitRTC(void)
{
	#if debug_enable
	if(RCC_GetSYSCLKSource() == 0x00)
		printf("Current system clock:HSI\r\n");
	if(RCC_GetSYSCLKSource() == 0x04)
		printf("Current system clock:HSE\r\n");
	else if(RCC_GetSYSCLKSource() == 0x08)
		printf("Current system clock:PLL\r\n");
	#endif
	//RTC_DeInit();
	RTC_Config();   /* ����RTCģ�� */
	
if (OBCBootInfo.BootCnt == 0)   /* ��һ�ο��� */
{  
	/* ����ϵ縴λ��־�Ƿ����� */
	if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
	{
		/* �����ϵ縴λ */
		OBCBootInfo.BootPORRSTCnt++;
	}
}

else  /* ��N�ο��� */
{
	/* ����ϵ縴λ��־�Ƿ����� */
	if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
	{
		/* �����ϵ縴λ */
		OBCBootInfo.BootPORRSTCnt++;
	}
	
	/* ������Ÿ�λ��־�Ƿ����� */
	else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
	{
		/* �������Ÿ�λ */
		OBCBootInfo.BootPINRSTCnt++;
	}
	else if (RCC_GetFlagStatus(RCC_FLAG_SFTRST) != RESET)
	{
		OBCBootInfo.BootSFTRSTCnt++;
	}
	else if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{
		/* �����������Ź���λ */
    OBCBootInfo.BootIWDGRSTCnt++;
	}
	else if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET)
	{
		/* �������ڿ��Ź���λ */
		OBCBootInfo.BootWWDGRSTCnt++;
	}
	else if (RCC_GetFlagStatus(RCC_FLAG_LPWRRST) != RESET)
	{
		/* �����͵�ѹ��λ */
    OBCBootInfo.BootLPWRRSTCnt++;
	}
}

}

/*
*********************************************************************************************************
*	�� �� ��: RTC_Config
*	����˵��: ��������ʱ�������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RTC_Config(void)
{	
	int RtcTimeout;  /* RTC��ʱ���� */
	RTC_TimeTypeDef  RTC_TimeStructure;
	RTC_InitTypeDef  RTC_InitStructure;
	RTC_DateTypeDef  RTC_DateStructure;
	
	Mem_Set(&RTC_TimeStructure, 0x00, sizeof(RTC_TimeTypeDef));
	Mem_Set(&RTC_InitStructure, 0x00, sizeof(RTC_InitTypeDef));	
	Mem_Set(&RTC_DateStructure, 0x00, sizeof(RTC_DateTypeDef));
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);/* ʹ��PWRʱ�� */
	PWR_BackupAccessCmd(ENABLE);/* �������RTC */
	
	

	if(OBCBootInfo.BootRTC_Source == 0)  /* ѡ��LSE��Ϊʱ��Դ */
	{
		RCC_LSEConfig(RCC_LSE_ON);   	     /* ʹ��LSE����  */
		RtcTimeout = 0;	                   /* �ȴ����� */  

		while((RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET) && ((RtcTimeout++) < RTC_TIMEOUT_US))//20
		{
			bsp_DelayUS(1);             /* STM32�ӳٺ��� */
		}

		if(RtcTimeout > RTC_TIMEOUT_US)  //18              /* �ж��Ƿ��г�ʱ */
		{	
			#if debug_enable
			printf("Init RTC Wrong: Oscillator time out! \r\n");
			#endif
			RCC_LSEConfig(RCC_LSE_OFF);
			OBCBootInfo.BootLSE_Error = 1;
			OBCBootInfo.BootRTC_Source = 1;   /* �Զ�����RTCΪ�ڲ�ʱ�� */
		}
	
		if(OBCBootInfo.BootRTC_Source == 0)
		{
			#if debug_enable
			printf("Enable RTC with LSE \r\n");
			#endif
			RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		/* ѡ��RTCʱ��Դ */
			uwSynchPrediv = 0xFF;
			uwAsynchPrediv = 0x7F;
		}
	}

	if(OBCBootInfo.BootRTC_Source == 1)	/* ѡ��LSI��Ϊʱ��Դ */
	{
		RCC_LSICmd(ENABLE);  /* ʹ���ڲ�ʱ�� */

		RtcTimeout = 0;      /*  �ȴ��ڲ�ʱ���ȶ�*/
		while((RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET) && ((RtcTimeout++) < RTC_TIMEOUT_US))
		{
			__nop();__nop();__nop();__nop();__nop();/* STM32�ӳٺ��� */
		}
		
		if(RtcTimeout > RTC_TIMEOUT_US)
		{
			#if debug_enable
			printf("All Oscillator time out! \r\n");
			#endif
			//RCC_LSICmd(DISABLE);
			OBCBootInfo.BootLSI_Error = 1;
			OBCBootInfo.BootRTC_Source = 2;   /* �Զ�����RTCΪ�ڲ�ʱ�� */
		}
		if(OBCBootInfo.BootRTC_Source == 1)
		{
			#if debug_enable
			printf("Enable RTC with LSI \r\n");
			#endif
			RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);  /* ѡ��RTCʱ��Դ */
			uwSynchPrediv = 0xFF;
			uwAsynchPrediv = 0x7F;
		}
	}
	if((OBCBootInfo.BootRTC_Source == 0)||(OBCBootInfo.BootRTC_Source == 1))
	{

	  RCC_RTCCLKCmd(ENABLE);	/* ʹ��RTCʱ�� */
	  RTC_WaitForSynchro();  /* �ȴ�RTC APB�Ĵ���ͬ�� */
    RTC_TimeStampCmd(RTC_TimeStampEdge_Falling, ENABLE);  /* ʹ��ʱ��� */
		RTC_TimeStampPinSelection(RTC_TamperPin_PC13);	
	  RTC_InitStructure.RTC_AsynchPrediv = uwAsynchPrediv;  /* ����RTC���ݼĴ����ͷ�Ƶ��  */
	  RTC_InitStructure.RTC_SynchPrediv = uwSynchPrediv;
	  RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
	
	  if (RTC_Init(&RTC_InitStructure) == ERROR)  /* ���RTC��ʼ�� */
	  {
			#if debug_enable
			printf("RTC Init wrong \r\n");
			#endif
	  }

	  /* ���������պ����� */
	  RTC_DateStructure.RTC_Year = 0x14;
	  RTC_DateStructure.RTC_Month = RTC_Month_October;
	  RTC_DateStructure.RTC_Date = 0x23;
	  RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Thursday;
	  RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);

	  /* ����ʱ���룬�Լ���ʾ��ʽ */
	  RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
	  RTC_TimeStructure.RTC_Hours   = 0x00;
	  RTC_TimeStructure.RTC_Minutes = 0x00;
	  RTC_TimeStructure.RTC_Seconds = 0x00; 
	  RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);   
	}
}


void bsp_RTCSet(uint8_t hour, uint8_t minute, uint8_t second)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	
	RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
	RTC_TimeStructure.RTC_Hours = hour; 
  RTC_TimeStructure.RTC_Minutes = minute;
  RTC_TimeStructure.RTC_Seconds = second;
	
	if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
	{
		printf("\n\r>> !! RTC Set Time failed. !! <<\n\r");
	} 
}
