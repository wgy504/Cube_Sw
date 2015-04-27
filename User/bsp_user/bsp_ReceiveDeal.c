#include <includes.h>
#include "globalavr.h"

enum INS_STATUS InsState=INS_IDLE;  /* �������״̬��ʼ�� */


uint8_t InsRxIndex;
uint8_t InsRxLength;
uint8_t InsRcvErr=0;
uint8_t GT_ConnStat=0;  /* û����λ������ */

CPU_INT08U TestRcv(unsigned char rev_data)
{
	OS_ERR err;
	
	switch(InsState)
	{
			case INS_IDLE:  //����״̬
				if(rev_data == 0XA5)
				{
					InsRxIndex = 0;
					InsRxLength = 0;
					InsState = INS_SNYC;
					OSTmrStart((OS_TMR *) &COM_OT_TIMER, (OS_ERR *) &err);   
				}
				else
				{
					InsRxIndex = 0;
					InsRxLength = 0;
					OSTmrStop((OS_TMR *) &COM_OT_TIMER, OS_OPT_TMR_NONE, (OS_TMR_CALLBACK_PTR)NULL, (OS_ERR *) &err); 
				}
			break;
				
			case INS_SNYC:  //ͬ��״̬
				if(rev_data == 0X5A)
				{
					InsState = INS_RCV;
					InsRxIndex = 0;  /* ���������������� */
					InsRxLength = 0;
					OSTmrStart((OS_TMR *) &COM_OT_TIMER, (OS_ERR *) &err);
				}					
			break;
				
			case INS_RCV:  //����״̬
				InsBuf[InsRxIndex] = rev_data;  //0:ָ�� 1:���� 2:���� 3:XOR
				
				if(InsRxIndex == 1)  //��ȡ�����򳤶�
				{
					InsRxLength = rev_data;
				}
				
				if((InsRxLength + 2) == InsRxIndex)
				{
				
					//Mem_Copy(InsBuf, ID_CommandBuf[ID_CommandCnt], InsRxIndex);
					//ID_CommandCnt++;
				
					
					InsDecode(InsBuf);
					
					InsState = INS_IDLE;
					OSTmrStop((OS_TMR *) &COM_OT_TIMER, OS_OPT_TMR_NONE, (OS_TMR_CALLBACK_PTR)NULL, (OS_ERR *) &err); 
				}
				else
				{
					OSTmrStart((OS_TMR *) &COM_OT_TIMER, (OS_ERR *) &err);   
				}
				
				InsRxIndex++;
			break;
				
			default:
				InsRcvErr |= 0x80;
			break;
	}
	return 0;
}



void ComOT_CallBack (OS_TMR *p_tmr, void *p_arg)
{
	InsRxIndex = 0; 
	InsState = INS_IDLE;
	switch(InsState)
	{
			case INS_IDLE:
			{
				InsRcvErr |= 0x40;
			}break;
			case INS_SNYC:
			{
				InsRcvErr |= 0x20;
			}break;
			case INS_RCV:
			{
				InsRcvErr |= 0x10;
			}break;
			default:
				break;
	}
}

CPU_INT16U GetCheckSum(CPU_INT16U *Ptr, uint8_t BufSize)
{
	CPU_INT32U CheckSumTemp;
	CPU_INT16U index;
	
	CheckSumTemp = 0x00000000;
	
	for(index=0; index<BufSize; index++)
		CheckSumTemp+=*(Ptr+index);
	
	return (CPU_INT16U)CheckSumTemp;
}


void InsSendAck(uint8_t cmd)
{
	//uint8_t ins_temp[10] = {0xA5, 0x5A, 0xFF, }
	//UartSend(USART1,OBC_ACK);
}

CPU_INT08U InsDecode(uint8_t *InsBuf)
{
	switch(*InsBuf)
	{
		/* ����ָ�� */
		case INS_CONN_TST:
			GT_ConnStat = 1; //��λ������
			InsSendAck(INS_CONN_TST);
			break;
		case INS_COMM_SWITCH_CLR:
		{
			
			//UartSend(USART1,OBC_ACK);
			break;
		}
		/* ��������ָ�� */	
		case INS_DOWN_TEL:
		{
			//UartSend(USART1,INS_DOWN_TEL);
			break;
		}
		case INS_DOWN_PLD:
		{
			//UartSend(USART1,INS_DOWN_PLD);
			break;
		}			
		case INS_DOWN_CMD:
		{
			//UartSend(USART1,INS_DOWN_CMD);
			break;
		}					
		/* ����ָ�� */
		case INS_GPS_A_ON:
		{
			//UartSend(USART1,INS_GPS_A_ON);
			break;
		}		
		
		default:
		{
			//UartSend(USART1,0xFF);
			break;
		}
	}
	
	return 0;
}


void GndTsRxHandle(void)
{
	uint8_t response;
	
	if(comGetChar(COM1, &response)) //��ȡһ���ַ�
	{
		TestRcv(response);
	}
	
}
