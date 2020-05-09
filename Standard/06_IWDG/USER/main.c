#include "include.h"



uint8_t key_val;

int main(void)
{
	delay_init(168); 
	delay_ms(100);
	Key_Init();
	LED_BEEP_Init();
	IWDG_Init();
	
	
	GPIO_ResetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);
	GPIO_SetBits(GPIOF,GPIO_Pin_8);

	
	while(1)
	{
		key_val = Key_Scan();
		if(key_val == 1)
		{
			IWDG_Free();
			delay_ms(100);
		}
	}
}

