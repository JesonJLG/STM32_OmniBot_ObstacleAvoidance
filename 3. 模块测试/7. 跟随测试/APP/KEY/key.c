#include "key.h"
#include "sys.h"
#include "delay.h"


//¾ØÕó°´¼ü³õÊ¼»¯
void Key_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    
    //LINE    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    //ROW
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
    GPIO_Init(GPIOA,&GPIO_InitStructure);
}


uint16_t Key_scan(void)
{
    uint16_t key_val=0;
    uint16_t temp=0;
	
	/*************Scan  1st Line************************/

    GPIOA->ODR=0X00;

    GPIOA->ODR=0XF7;

    if((GPIOA->IDR&0XF0)!=0XF0)
    {
        delay_ms(50);

        if((GPIOA->IDR & 0XF0)!=0XF0)
        {
            temp=(GPIOA->IDR&0XF7);
            switch(temp)
            {
                case 0xE7:  key_val=1;   break;

                case 0xD7:  key_val=2;   break;

                case 0xB7:  key_val=3;   break;

                case 0x77:  key_val=4;   break;

                default:    key_val=0;   break;
            }
        }
    }
    
		/*************Scan  2st Line************************/
    GPIOA->ODR=0X00;

    GPIOA->ODR=0XFB;

    if((GPIOA->IDR&0XF0)!=0XF0)
    {
        delay_ms(50);

        if((GPIOA->IDR & 0XF0)!=0XF0)
        {
            temp=(GPIOA->IDR&0XFB);
            switch(temp)
            {
                case 0xEB:  key_val=5;  break;

                case 0xDB:  key_val=6;  break;

                case 0xBB:  key_val=7;  break;

                case 0x7B:  key_val=8;  break;

                default:    key_val=0;  break;
            }
        }
    }

		/*************Scan  3st Line************************/
    GPIOA->ODR=0X00;

    GPIOA->ODR=0XFD;

    if((GPIOA->IDR&0XF0)!=0XF0)
    {
        delay_ms(50);

        if((GPIOA->IDR & 0XF0)!=0XF0)
        {
            temp=(GPIOA->IDR&0XFD);
            switch(temp)
            {
                case 0xED:  key_val=9;   break;

                case 0xDD:  key_val=10;  break;

                case 0xBD:  key_val=11;  break;

                case 0x7D:  key_val=12;  break;

                default:    key_val=0;   break;
            }
        }
    }

		/*************Scan  4st Line************************/
    GPIOA->ODR=0X00;

    GPIOA->ODR=0XFE;

    if((GPIOA->IDR&0XF0)!=0XF0)
    {
        delay_ms(50);

        if((GPIOA->IDR & 0XF0)!=0XF0)
        {
            temp=(GPIOA->IDR&0XFE);
            switch(temp)
            {
                case 0xEE:  key_val=13;  break;

                case 0xDE:  key_val=14;  break;

                case 0xBE:  key_val=15;  break;

                case 0x7E:  key_val=16;  break;

                default:    key_val=0;   break;
            }
        }
    }

    return key_val;

}
