#include"stm32f4xx.h"
#include"key.h"
#include"led_beep.h"


//uint8_t KEY0 = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4);
	
int main(void)
{

	//uint8_t KEY0 = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4);
	u8 Key_Val;
	delay_init(168);
	Key_Init();
	LED_BEEP_Init();
	
	
	while(1)
	{
		Key_Val = Key_Scan(0);
		if(!Key_Val)
		{
			switch(Key_Val)
			{
				case 1:
					GPIO_SetBits(GPIOF,GPIO_Pin_9);
					break;
				/*case 2:
					break;
				case 3:
					break;*/
				case 4:
					if(GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0) == 1)
					{
						GPIO_SetBits(GPIOF,GPIO_Pin_8);
					}
					else
					{
						GPIO_ResetBits(GPIOF,GPIO_Pin_8);
					}
					break;
			}
		}
	
	}
}

