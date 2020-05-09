#include "include.h"





int main(void)
{
	delay_init(168); 
	delay_ms(100);
//	Key_Init();
	LED_BEEP_Init();
//	IWDG_Init();
	WWDG_Init();
	
	
	GPIO_ResetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);
	//GPIO_SetBits(GPIOF,GPIO_Pin_8);
	delay_ms(300);
	
	while(1)
	{
		LED0 = 1;
	}
	
}

