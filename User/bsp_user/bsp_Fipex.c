#include <includes.h>


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



void bsp_FipexSwitchInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	/* ʹ��ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_FIPEX_3V3_PORT | RCC_FIPEX_5V_PORT, ENABLE);

	/* ����GPIO */

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	
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
	
	
	//���ڷ�������ָ��
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
