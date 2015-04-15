#include <includes.h>

#define FIPEX_RX_SIZE 256

uint8_t FipexRxBuf[FIPEX_RX_SIZE] = {0};
uint8_t FipexRxPtr = 0;
uint8_t FipexRxSyncFlg = 0;
uint8_t FipexRxLength = 0;


void PLOAD_SAM_TASK(void *p_arg)
{
	uint8_t Fipex_data;
	uint8_t FipexAckTimeout;
	(void)p_arg;

	
	
	while(1)
	{
		switch(FipexStatus)
		{
			case Stop:
				break;
			case PowerOn:  //��ʼ��
				FipexCurTime = 0;
				CurrentScript = 0;
				ScriptAct = 1;
				DelayTime = 0;  //��ʼ�ӳ�Ϊ1ms
				FipexStatus = ScriptRunning;
			case ScriptRunning:
				if(ScriptAct == 1) //�ű�����
				{
					ScriptAct = 0;
					switch (FipexOperationPara.FipexCmdInfo[CurrentScript].FipexCmd[1])
					{
						case OBC_SU_ON:
							if(bsp_FipexPowerOn())
							{
								//�ϵ����
							}
							
							if(FipexOperationPara.FipexCmdInfo[CurrentScript].CmdDelay == 0xFFFF)
								DelayTime = 1;
							else
								DelayTime = FipexOperationPara.FipexCmdInfo[CurrentScript].CmdDelay * 1000;
							break;
							
						case OBC_SU_OFF:
							if(bsp_FipexPowerOff())
							{
								//�������
							}
							
							if(FipexOperationPara.FipexCmdInfo[CurrentScript].CmdDelay == 0xFFFF)
								DelayTime = 1;
							else
							DelayTime = FipexOperationPara.FipexCmdInfo[CurrentScript].CmdDelay * 1000;
							break;
							
						case OBC_SU_END:
							FipexCurTime = 0;
							DelayTime = FipexOperationPara.RepeatTime * 1000;
							FipexStatus = ScriptSleep; //״̬ת��Ϊ�ű�˯�߲���
							break;
						
						case SU_PING:
						case SU_INIT:
						case SU_ID:
						case SU_RSP:
						case SU_SP:
						case SU_HK:
						case SU_DP:
						case SU_STDBY:
						case SU_SC:
						case SU_SM:
						case SU_CAL:
							FipexRxPtr = 0; //����2��������ָ������
							comClearRxFifo(COM3);  //��մ��ڽ���FIFO
							bsp_FipexSendCmd(&FipexOperationPara.FipexCmdInfo[CurrentScript].FipexCmd[0], FipexOperationPara.FipexCmdInfo[CurrentScript].CmdLength);
							
							FipexAckWaitFlg = 1; //����Fipex�ű����ձ�־λ
							FipexRxSyncFlg = 0; //COM3����ͬ��������
							DelayTime = 1;
							break;
					}
				}
			break;
			
				
			case ScriptSleep:	
			break;
			default:
			break;
		}
	
		if(FipexAckWaitFlg)  //δ�յ�SU�غɻش�������
		{
			FipexAckTimeout = 0;
			FipexRxLength = 0;
			while(FipexAckTimeout < 10)  //���մ������ݣ���ʱ����Ϊ10ms��
			{
				if(comGetChar(COM3, &Fipex_data))  //���ڻ�ȡһ������
				{
					FipexAckTimeout = 0; //��ʱ����
					if(!FipexRxSyncFlg)  //δͬ��
					{
						if(Fipex_data == 0x7E)
						{
							FipexRxSyncFlg = 1;  
							FipexRxPtr = 0;   //��ͬ���ַ���֡ͷ
							FipexRxBuf[FipexRxPtr++] = Fipex_data; 
						}
					}
					else  //ͬ�������
					{
						FipexRxBuf[FipexRxPtr++] = Fipex_data; 
						if(FipexRxPtr == 3) //������ݳ���
						{
							FipexRxLength = FipexRxBuf[2]; //����2���ַ��������ݴ���FipexRxLength
						}
						
					}
				}
				else
				{
					FipexAckTimeout++;
				}
				
				BSP_OS_TimeDlyMs(1);
			}
			
			if(FipexAckTimeout == 10)
			{
				//����
			}
			else
			{
				FipexAckHandle(FipexRxBuf);
			}

		}
		
		else  //��ȡ��SU�غɻش�������
		{
			if(FipexCurTime++ > DelayTime) //Fipexʱ�䵽���ӳ�ʱ��
			{
				switch(FipexStatus)
				{
					case Stop:
					case PowerOn:
						break;
					case ScriptRunning:
						if(CurrentScript == FipexOperationPara.CmdCnt - 1)  //�ű�����������󣬽���ű�˯�߽׶�
						{
							FipexCurTime = 0;
							DelayTime = FipexOperationPara.RepeatTime * 1000;
							FipexStatus = ScriptSleep; //״̬ת��Ϊ�ű�˯�߲���
						}
						else  //�ű������л�
						{
							FipexCurTime = 0;
							CurrentScript++;
							ScriptAct = 1;  //�ű�������־λ��λ
						}
						break;
						
					case ScriptSleep:
						FipexStatus = PowerOn;
						break;
				}
				
			}
		}
		BSP_OS_TimeDlyMs(1);
	}
}

void PLOAD_STO_TASK(void *p_arg)
{
	(void)p_arg;
	
	while(1)
	{
		BSP_OS_TimeDlyMs(1000);
	}
}
