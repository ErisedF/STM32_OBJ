#include "include.h"



void IWDG_Init(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);		//IWDGдʹ��
	IWDG_SetPrescaler(IWDG_Prescaler_64);						//����Ԥ��Ƶϵ��
	IWDG_SetReload(0x4E2);													//�������ؼ���ֵ
	IWDG_ReloadCounter();														//װ�����ؼ���ֵ(ι��)
	IWDG_Enable();																	//ʹ�ܶ������Ź�
}



void IWDG_Free(void)
{
	IWDG_ReloadCounter();														//װ�����ؼ���ֵ(ι��)
}
