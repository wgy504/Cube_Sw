/*
*********************************************************************************************************
*
*	ģ������ : RTC
*	�ļ����� : bsp_rtc.c
*	��    �� : V1.0
*	˵    �� : RTC�ײ�����
*	
*
*********************************************************************************************************
*/

#ifndef __BSP_RTC_H
#define __BSP_RTC_H

void RTC_TimeShow(void);
void RTC_AlarmShow(void);
void RTC_TimeRegulate(void);
uint8_t USART_Scanf(uint32_t MinValue, uint32_t MaxValue);
void RTC_DateShow(void);
void bsp_InitRTC(void);
void bsp_InitBkpSram(void);
void RTC_Config(void);
void RTC_TimeStampShow(void);
void RTC_SourceSwitch(void);
#endif

