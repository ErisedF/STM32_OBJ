#include"key.h"

void Key_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}


u8 Key_Scan(u8 mode)
{
	while(1)
	{
	uint8_t KEY_0 = GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4);
	}
/*	static u8 key_up_flag = 1;
	if(mode==1)
		key_up_flag = 1;
	if((key_up_flag == 1)&&(KEY_0 == 0 || KEY_1 == 0 || KEY_2 == 0 || KEY_UP == 1))
	{
		delay_ms(5);
		key_up_flag = 0;
		if (KEY_0 == 0)	return 1;		//response priority
		if (KEY_1 == 0)	return 2;
		if (KEY_2 == 0)	return 3;
		if (KEY_UP == 1)	return 4;
	}
	else	
		key_up_flag = 1;
	return 0;*/
}


	
