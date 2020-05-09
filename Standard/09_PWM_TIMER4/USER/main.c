#include "include.h"


uint32_t led0_pwm_CCR = 0;
uint8_t	 dir_flag = 1;

int main(void)
{
	delay_init(168); 
	delay_ms(100);
//	Key_Init();
	LED_BEEP_Init();
//	IWDG_Init();
//	WWDG_Init();
//	TIMER3_Init();
	PWM_TIM14_Init();
	
	//GPIO_ResetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);
	//GPIO_SetBits(GPIOF,GPIO_Pin_8);
	delay_ms(300);
	
	while(1)
	{
		delay_ms(100);
		LED1 = 0;
		if(dir_flag == 1)
		{
			led0_pwm_CCR++;
		}
		if(dir_flag == 0)
		{
			led0_pwm_CCR--;
		}
		if(led0_pwm_CCR == 0)
		{
			dir_flag = 1;
		}
		if(led0_pwm_CCR > 299)
		{
			dir_flag = 0;
		}
		
		TIM_SetCompare1(TIM14, led0_pwm_CCR);
	}
	
}

