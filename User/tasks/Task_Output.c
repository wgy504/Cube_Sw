#include  <includes.h>
#include "globalavr.h"

CPU_INT16U PwmCurDutyOld_1;
CPU_INT16U PwmCurDutyOld_2;
CPU_INT16U PwmCurDutyOld_3;
CPU_INT08U PwmPwrStatusOld;
void OUTPUT_TASK(void *p_arg)
{
	char timeout = 0;
	(void)p_arg;
	
	//PwmCurDuty_1 = PwmSetDuty_1;
  //PwmCurDuty_2 = PwmSetDuty_2;
  //PwmCurDuty_3 = PwmSetDuty_3;
	
	while((!MTQ_POWER_PIN()) && timeout < 50)  //��һ�������ȴ���POWER�����ٽ��д���
	{
		timeout++;
		BSP_OS_TimeDlyMs(100);
	}
	
	if(MTQ_POWER_PIN())
	{	
		MTQ_WAKEN;
	}
	
	PwmPwrStatusOld = MTQ_POWER_PIN();  //��Pwm���ŵĵ�ƽֵ��ֵ��PwmOld
	
	while(1)
	{
		
		if(PwmPwrStatusOld != MTQ_POWER_PIN())  //Pwmʹ���źŷ����ı�
		{
			if(MTQ_POWER_PIN())  //��Դ����
			{
				MTQ_WAKEN;   //оƬ����
				
				PwmSetDuty_1 = PwmCurDutyOld_1;
				PwmSetDuty_2 = PwmCurDutyOld_2;
				PwmSetDuty_3 = PwmCurDutyOld_3;
			}
			else   //��Դ�ر�
			{
				PwmCurDutyOld_1 = PwmCurDuty_1;
				PwmCurDutyOld_2 = PwmCurDuty_2;
				PwmCurDutyOld_3 = PwmCurDuty_3;
				PwmCurDuty_1 = 0;
				PwmCurDuty_2 = 0;
				PwmCurDuty_3 = 0;
				bsp_SetPWMDutyCycle(PwmCurDuty_1, 1);
				bsp_SetPWMDutyCycle(PwmCurDuty_2, 2);
				bsp_SetPWMDutyCycle(PwmCurDuty_3, 3);
				
				MTQ_SLEEP;   //�ر�оƬ
			}
		}
		
		
		if(MTQ_POWER_PIN() && MTQ_SLEEP_PIN())  //MTQ��
		{
			if(PwmCurDuty_1 != PwmSetDuty_1)
			{
				PwmCurDuty_1 = PwmSetDuty_1;
				bsp_SetPWMDutyCycle(PwmCurDuty_1, 1);			
			}
			if(PwmCurDuty_2 != PwmSetDuty_2)
			{
				PwmCurDuty_2 = PwmSetDuty_2;
				bsp_SetPWMDutyCycle(PwmCurDuty_2, 2);			
			}
			if(PwmCurDuty_3 != PwmSetDuty_3)
			{
				PwmCurDuty_3 = PwmSetDuty_3;
				bsp_SetPWMDutyCycle(PwmCurDuty_3, 3);
			}
			
		}
		
		
		
		
		
/*			
		if(MotorCurOutput != MotorSetOutput)
		{
			MotorCurOutput = MotorSetOutput;
			MotorChange(MotorSetOutput);
		}
*/	
		PwmPwrStatusOld = MTQ_POWER_PIN();
		BSP_OS_TimeDlyMs(100);
	}
}
