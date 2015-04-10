#include <includes.h>

struct FipexOperationParaStr FipexOperationPara;  //8������ָ��

const uint8_t FipexCmdDeft0[6] = {0x7E, 0x0B, 0x00, 0x0B, 0x3C, 0x00};
const uint8_t FipexCmdDeft1[9] = {0x7E, 0x11, 0x03, 0x04, 0x01, 0x00, 0x17, 0xFF, 0xFF};
const uint8_t FipexCmdDeft2[9] = {0x7E, 0x11, 0x03, 0x05, 0x10, 0x0A, 0x0D, 0xFF, 0xFF};
const uint8_t FipexCmdDeft3[9] = {0x7E, 0x11, 0x03, 0x02, 0xC8, 0x00, 0xD8, 0xFF, 0xFF};
const uint8_t FipexCmdDeft4[6] = {0x7E, 0x0C, 0x00, 0x0C, 0x2C, 0x01};
const uint8_t FipexCmdDeft5[6] = {0x7E, 0x20, 0x00, 0x20, 0xFF, 0xFF};
const uint8_t FipexCmdDeft6[6] = {0x7E, 0x21, 0x00, 0x21, 0xFF, 0xFF};


uint8_t FepixPowerOnFlg = 0;





uint8_t bsp_FipexSendCmd(uint8_t *cmd, uint16_t length)
{
	uint8_t command_temp[32] = {0};
	uint16_t length_temp;

	length_temp = length;
	Mem_Copy(command_temp, cmd, length_temp);  //�����ݿ������ֲ�����
	comWaitTxFifoEmpty(COM3); // �ȴ�����3�������
	
	comSendBuf(COM3, command_temp, length);
	return 0;
}

void FipexCmdRead(void)  //��CPU_INTER��ȡ����
{
	#if debug_enable
	printf("Upload Fipex Command from CPU internal Flash\r\n");
	#endif 
	
	bsp_ReadCpuFlash(CurFlashSetor, (uint8_t *)&FipexOperationPara.FipexCmdInfo[0].FipexCmd[0], sizeof(FipexOperationPara));
}

uint8_t FipexCmdUpdate(uint8_t *cmd, uint8_t index, uint8_t length)
{
	FipexOperationPara.FipexCmdInfo[index].CmdLength = length;
	
	Mem_Set(&FipexOperationPara.FipexCmdInfo[index].FipexCmd[0], 0x00, 32);
	Mem_Copy(&FipexOperationPara.FipexCmdInfo[index].FipexCmd[0], cmd, length);
	
	return 0;
}

uint8_t FipexCmdStore(void)
{
	uint8_t TryTime;
	TryTime = 2;
	while(bsp_WriteCpuFlash(CurFlashSetor, (uint8_t *)&FipexOperationPara.FipexCmdInfo[0].FipexCmd[0], sizeof(FipexOperationPara)) && TryTime--);
	
	if(!TryTime)
	{
		#if debug_enable
			printf("Internal Flash program error!\r\n");
		#endif
	}
	return 0;
}

void FipexSetDefaultInfo(void)
{
	FipexCmdUpdate((uint8_t *)FipexCmdDeft0, 0, FipexCmdDeft0[2] + 0x06);
	FipexCmdUpdate((uint8_t *)FipexCmdDeft1, 1, FipexCmdDeft1[2] + 0x06);
	FipexCmdUpdate((uint8_t *)FipexCmdDeft2, 2, FipexCmdDeft2[2] + 0x06);
	FipexCmdUpdate((uint8_t *)FipexCmdDeft3, 3, FipexCmdDeft3[2] + 0x06);
	FipexCmdUpdate((uint8_t *)FipexCmdDeft4, 4, FipexCmdDeft4[2] + 0x06);
	FipexCmdUpdate((uint8_t *)FipexCmdDeft5, 5, FipexCmdDeft5[2] + 0x06);
	FipexCmdUpdate((uint8_t *)FipexCmdDeft6, 6, FipexCmdDeft6[2] + 0x06);
	FipexOperationPara.StartTime = 0x1A56BFC0;
	FipexOperationPara.RepeatTime = 0x0E10;
	FipexOperationPara.CmdCnt = 10;
	
	FipexCmdStore();
}
uint8_t FipexInfoGet(uint8_t* cmd, uint8_t size)
{
	uint8_t i;
	int16_t length = (int16_t)cmd[0]; //ָ���
	uint8_t cmd_cnt = 0;
	uint8_t *cmd_ptr;
	uint8_t *delay_ptr;
	
	
	Mem_Set((uint8_t *)&FipexOperationPara, 0x00, sizeof(FipexOperationPara));
/*******************************************************/	
	for(i=4; i>0; i--)
	{
		FipexOperationPara.StartTime = (FipexOperationPara.StartTime | cmd[i]) << 8;
	}
	length -= 4;
/*******************************************************/
	for(i=6; i>4; i--)
	{
		FipexOperationPara.RepeatTime = (FipexOperationPara.RepeatTime | cmd[i]) << 8;
	}
	length -= 2;
/*******************************************************/	
	cmd_cnt = cmd[7];
	
	
	cmd_ptr = &cmd[8];
	for(i=0; i<cmd_cnt; i++)
	{
		if(*cmd_ptr != 0x7E) //����ͬ����
		{
			#if debug_enable
				printf("Sync error!\r\n");
			#endif		
			return 1;  //���ͬ���ִ���	
		}
		delay_ptr = (cmd_ptr+cmd_ptr[2]+4);
		
		Mem_Copy(&FipexOperationPara.FipexCmdInfo[i].FipexCmd[0], cmd_ptr, cmd_ptr[2]+4);
		FipexOperationPara.FipexCmdInfo[i].CmdLength = cmd_ptr[2]+4;
		FipexOperationPara.FipexCmdInfo[i].CmdDelay = *(delay_ptr + 1);
		FipexOperationPara.FipexCmdInfo[i].CmdDelay = (FipexOperationPara.FipexCmdInfo[i].CmdDelay << 8) | *(delay_ptr);
		
		if(length -  6 - cmd_ptr[2]< 0)
		{
			#if debug_enable
				printf("Size error!\r\n");
			#endif	
			return 2; //�ַ�������

		}
			
		cmd_ptr = delay_ptr + 2;
	}
	
	return 0;
}
	
void FipexInfomationInit(void)  //��FLASH�ж�ȡָ����Ϣ
{
	#if debug_enable
		char i, j;
	#endif
	
	#if debug_enable
	printf("Read Fipex Command from CPU Flash\r\n");
	#endif 
	
	bsp_ReadCpuFlash(CurFlashSetor, (uint8_t *)&FipexOperationPara.FipexCmdInfo[0].FipexCmd[0], sizeof(FipexOperationPara));

	#if debug_enable
	printf("Fipex Command infomation:\r\n");
	
	for(i=0; i<8; i++)
	{
		printf("Cmd: ");
		for(j=0; j<32; j++)
		{
			printf("%x ", FipexOperationPara.FipexCmdInfo[i].FipexCmd[j]);
		}
		printf("||Command Delay: %d\r\n", FipexOperationPara.FipexCmdInfo[i].CmdDelay);
	}
	#endif 
}






void bsp_FipexSwitchInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* ʹ��ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_FIPEX_3V3_PORT | RCC_FIPEX_5V_PORT, ENABLE);

	/* ����GPIO */

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	
	GPIO_InitStructure.GPIO_Pin = GPIO_FIPEX_3V3_EN_PIN;	
	GPIO_Init(GPIO_FIPEX_3V3_EN_PORT, &GPIO_InitStructure);
	
	FIPEX_3V3_DISABLE;  //����3.3V����
	GPIO_InitStructure.GPIO_Pin = GPIO_FIPEX_5V_EN_PIN;	
	GPIO_Init(GPIO_FIPEX_5V_EN_PORT, &GPIO_InitStructure);
	FIPEX_5V_DISABLE;   //����5V����
	
}

uint8_t bsp_FipexPowerOn(void)
{
  uint32_t FipexTimeOut_ms;
	
	FIPEX_3V3_ENABLE;//Fipex 3.3����Դ�ϵ�
	
	FipexTimeOut_ms = 0;
	while((FIPEX_3V3_PIN() != SET) && (((FipexTimeOut_ms++) < (300*1000))))	//����ϵ������Ƿ�����(ʱ������300ms)
	{
		bsp_DelayUS(1);
	}
	if(FipexTimeOut_ms > (290*1000))	//�����ʱtodo
	{
		FIPEX_3V3_DISABLE;
		
		#if debug_enable
		printf("Fipex 3v3 power on fail!\r\n");
		#endif
		
		return 1;  //3.3V�ϵ����
	}
	
	
	BSP_OS_TimeDlyMs(10);  //�ϵ�֮���ӳ�
	
	FIPEX_5V_ENABLE;	//Fipex5����Դ�ϵ�
	
	FipexTimeOut_ms = 0;
	while((FIPEX_5V_PIN() != SET) && (((FipexTimeOut_ms++) < (300*1000))))	//����ϵ������Ƿ�����(ʱ������300ms)
	{
		bsp_DelayUS(1);
	}
	if(FipexTimeOut_ms > (290*1000))	//�����ʱtodo
	{
		FIPEX_5V_DISABLE;
		FIPEX_3V3_DISABLE;
		
		#if debug_enable
		printf("Fipex 5V power on fail!\r\n");
		#endif
		
		return 2;  //5V�ϵ����
	}
	else
	{
		FepixPowerOnFlg = 1;  //FEPIX�ϵ��־λ��λ
		
		#if debug_enable
		printf("Fipex power on successfully!\r\n");
		#endif
	}
	BSP_OS_TimeDlyMs(1000);	//�ȴ�1000ms
	
	
	//bsp_FipexSendCmd(FipexCmd, 8);//���ڷ�������ָ��
	return 0;
}

uint8_t bsp_FipexPowerOff(void)
{
	uint32_t FipexTimeOut_ms;
	
	//���ڷ��͹ر�ָ��
	
	//�ظ���ȷ
	
	FIPEX_5V_DISABLE;//�ر�Fipex 5����Դ����
	
	while((FIPEX_5V_PIN() != RESET) && (((FipexTimeOut_ms++) < (300*1000))))//���5�������Ƿ�ر�(ʱ������200ms)
	{
		bsp_DelayUS(1);
	}
	
	if(FipexTimeOut_ms > 290*1000)
	{
		#if debug_enable
			printf("Fipex power 5V off fail!\r\n");
		#endif
		return 1;
	}
	
	FIPEX_3V3_DISABLE;//�ر�Fipex3.3������
	while((FIPEX_5V_PIN() != RESET) && (((FipexTimeOut_ms++) < (300*1000))))//���3.3�������Ƿ�ر�(ʱ������200ms)
	{
		bsp_DelayUS(1);
	}

	if(FipexTimeOut_ms > 290*1000)
	{
		#if debug_enable
			printf("Fipex power 3V3 off fail!\r\n");
		#endif
		return 2;
	}
	else
	{
		FepixPowerOnFlg = 0;  //FEPIX�ϵ��־λ��λ
		
		#if debug_enable
		printf("Fipex power off successfully!\r\n");
		#endif
	}
	
	return 0;
}

uint8_t bsp_FipexGetCheckSum(unsigned char *cmd, unsigned char* checksum)
{
	unsigned char checksum_temp = 0;
	unsigned char length = 0;

	if (*cmd == 0x7E)  //����ĸ�Ƿ����0x7E
	{
		length = cmd[2];  //cmd[1]��cmd[length+3]����У��
		length += 3;
		while (length)
		{
			checksum_temp ^= cmd[length];
			length--;
		}
		*checksum = checksum_temp;
		return 0;
	}
	else
	{
		return 1;  //֡ͷ����0x7E
	}
	
}
