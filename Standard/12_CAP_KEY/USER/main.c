#include "include.h"




int main(void)
{
	delay_init(168);
	LED_BEEP_Init();
	delay_ms(100);
	TPAD_Init();
	while(1)
	{
		if(TPAD_Scan())
		{
			GPIO_ResetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);
		}
		delay_ms(10);
	}
	
}

