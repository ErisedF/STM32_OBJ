#include "stm32f4xx.h"
#include "key.h"
#include "led_beep.h"
#include "exit.h"

int main(void)
{
	delay_init(168);
	Key_Init();
	LED_BEEP_Init();
	EXITX_Init();
	while(1);


	
}
