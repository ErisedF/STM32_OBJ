#include "stm32f4xx.h" 
#include "key.h"
#include "led_beep.h"

uint8_t key_val;
uint8_t beep_flag;

int main(void)
{
	delay_init(168);
	Key_Init();
	LED_BEEP_Init();
	
	while(1)
	{
		key_val = Key_Scan();
		delay_ms(6);
		switch(key_val)
		{
			case 0x01:
				beep_flag = BEEP;
			if(beep_flag == 0)
				GPIO_SetBits(GPIOF,GPIO_Pin_8);
			else
				GPIO_ResetBits(GPIOF,GPIO_Pin_8);
			break;
				
		}
	}
}
