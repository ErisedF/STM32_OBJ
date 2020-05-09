#ifndef __CAP_KEY_H
#define __CAP_KEY_H
#include "sys.h"
#include "delay.h"

//PA5����͵�ƽ�ŵ磬������ֵ���
void TPAD_Reset(void);
//��ʱ��2ͨ��1���벶������
void TIM2_CH1_Cap_Init(u32 arr,u16 psc);
//���ݰ�����ʼ��
void TPAD_Init(void);
//���ݰ���ֵɨ��
u8 TPAD_Scan(void);
//��ȡ���ݰ������ֵ��Сֵ
uint32_t TPAD_Get_MaxVal(void);
uint32_t TPAD_Get_MinVal(void);
uint32_t TPAD_Get_Val(void);

#endif

