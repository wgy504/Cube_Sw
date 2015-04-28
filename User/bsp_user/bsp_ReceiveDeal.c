#include <includes.h>
#include "globalavr.h"

enum INS_STATUS InsState=INS_IDLE;  /* �������״̬��ʼ�� */

uint16_t InsRxCmdCnt = 0;
uint8_t InsRxIndex;
uint8_t InsRxLength;
uint8_t InsRcvErr=0;
uint8_t GT_ConnStat=0;  /* û����λ������ */
uint8_t InsCheckSumError = 0;

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

void InsGetCheckSum(uint8_t *Ptr, uint8_t buffsize, uint8_t *checksum)
{
	uint8_t checksumtemp = 0;
	while(buffsize)
	{
		buffsize--;
		checksumtemp ^= Ptr[buffsize];
		
	}
	*checksum = checksumtemp;
}


void InsSendAck(void)
{
	uint8_t ins_data_temp[10] = {0};
	uint8_t ins_checksum;
	
	ins_data_temp[0] = 0xA5;
  ins_data_temp[1] = 0x5A;
	ins_data_temp[2] = 0x30; //Ackָ����
	ins_data_temp[3] = 0x02; //����
  ins_data_temp[4] = (uint8_t)(InsRxCmdCnt & 0x00FF);  //������
	ins_data_temp[5] = (uint8_t)((InsRxCmdCnt >> 8) & 0x00FF); //������

	InsGetCheckSum(&ins_data_temp[4], ins_data_temp[3], &ins_checksum);
	ins_data_temp[6] = ins_checksum;
	comSendBuf(COM6, ins_data_temp, 7);
}

CPU_INT08U InsDecode(uint8_t *InsBuf)
{
	uint8_t ins_checksum;
	
	InsGetCheckSum(&InsBuf[2], InsBuf[1], &ins_checksum);
	if(ins_checksum != InsBuf[(InsBuf[1] + 2)])  //У�������
	{
		InsCheckSumError++;
		return 1;
	}
	switch(InsBuf[0])
	{
		/* ����ָ�� */
		case INS_CONN_TST:  //ͨ�Ų���ָ��
			GT_ConnStat = 1; //��λ������
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;
		case INS_COMM_SWITCH_CLR:  //ͨ������16Сʱ����ָ��
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;
		/* ��������ָ�� */	
		case INS_DOWN_TEL:
			InsRxCmdCnt++;  //ָ�������1
		break;
		case INS_DOWN_PLD:
			InsRxCmdCnt++;  //ָ�������1
		break;		
		case INS_DOWN_CMD:
			InsRxCmdCnt++;  //ָ�������1
		break;				
		case INS_SD_CLR:
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;				
		case INS_FLASH_RST:
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;				
		case INS_OBC_RST:
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;				
		
		/* ����ָ�� */
		case INS_MTQ_ON:
			SW_MTQ_ENABLE;
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;
		case INS_MTQ_OFF:
			SW_MTQ_DISABLE;
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;
		case INS_GPS_A_ON:
			SW_GPSA_ENABLE;
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;		
		case INS_GPS_A_OFF:
			SW_GPSA_DISABLE;
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;	
		case INS_GPS_B_ON:
			SW_GPSB_ENABLE;
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;		
		case INS_GPS_B_OFF:
			SW_GPSB_DISABLE;
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;			
		case INS_RSV_ON:
			SW_RES_ENABLE;
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;		
		case INS_RSV_OFF:
			SW_RES_DISABLE;
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;			
		case INS_MW_A_ON:
			SW_WHEELA_ENABLE;
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;		
		case INS_MW_A_OFF:
			SW_WHEELA_DISABLE;
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;		
		case INS_MW_B_ON:
			SW_WHEELB_ENABLE;
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;		
		case INS_MW_B_OFF:
			SW_WHEELB_DISABLE;
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;		
		case INS_SLBRD_ON:
			SW_SOLAR_ENABLE;
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;		
		case INS_SLBRD_OFF:
			SW_SOLAR_DISABLE;
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;		
		case INS_USB_ON:
			SW_USB_ENABLE;
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;		
		case INS_USB_OFF:
			SW_USB_DISABLE;
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;		
		case INS_S1_ON:
			SW_S0_ENABLE;
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;		
		case INS_S1_OFF:
			SW_S0_DISABLE;
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;		
		case INS_S2_ON:
			SW_S1_ENABLE;
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;		
		case INS_S2_OFF:
			SW_S1_DISABLE;
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;		
		case INS_S3_ON:
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;		
		case INS_S3_OFF:
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;		
		case INS_S4_ON:
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;		
		case INS_S4_OFF:
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;		
		
		case INS_DET:
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;		
		case INS_STA:
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;		
		case INS_DUMP_FOEV_DIS:
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;		
		case INS_SW_MAG_A:
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;		
		case INS_SW_MAG_B:
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;
		case INS_SW_1200:
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;		
		case INS_SW_9600:
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;		
		case INS_CW_ON:
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;		
		case INS_COM_TRAN_OFF:
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;
		case INS_TIME_IN:
			
			bsp_RTCSet(InsBuf[6],InsBuf[7],InsBuf[8],InsBuf[9],InsBuf[10],InsBuf[11]); //todo
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;
		
		case INS_FIPEX_SCRIPT_IN:
			
			FipexScriptStop();
			FipexInfoGet(&InsBuf[6]);
			FipexScriptStart();  //��ʼFipex�ű�
			InsRxCmdCnt++;  //ָ�������1
			InsSendAck();
		break;
				
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
	
	if(comGetChar(COM6, &response)) //��ȡһ���ַ�
	{
		TestRcv(response);
	}
	
}
