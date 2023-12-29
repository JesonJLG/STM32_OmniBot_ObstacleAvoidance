#include "beep.h"
#include "delay.h"

//��������ʼ��
void BEEP_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_SetBits(GPIOC, GPIO_Pin_13);//���1���رշ��������
}

//��һ��
void BEEP_Tick(void)
{
    BEEP = 0;   //��
    delay_ms(100);
    BEEP = 1;
    delay_ms(100);
}
//������
void BEEP_2Tick(void)
{
    BEEP_Tick();
    BEEP_Tick();
}
//������
void BEEP_3Tick(void)
{
    BEEP_Tick();
    BEEP_Tick();
    BEEP_Tick();
}
//��һ��
void BEEP_Ring(void)
{
    BEEP = 0;   //��
    delay_ms(200);
    BEEP = 1;
}
