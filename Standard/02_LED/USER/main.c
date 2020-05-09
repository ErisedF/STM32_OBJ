#include"stm32f4xx.h"
#include"stm32f4xx_rcc.h"
#include"stm32f4xx_gpio.h"
#include"led.h"

void Delay(__IO uint32_t nCount);

void Delay(__IO uint32_t nCount)
{
	while(nCount--);
}

int main(void)
{
	/*GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);


	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOF, &GPIO_InitStruct);*/
	
	LED_Init();
	
		while(1)
		{
			GPIO_SetBits(GPIOF,  /*BEEP |*/ LED_0 | LED_1);
			Delay(0x7FFFFF);
			GPIO_ResetBits(GPIOF,  /*BEEP |*/ LED_0 | LED_1);
			Delay(0x7FFFFF);
		}
}

