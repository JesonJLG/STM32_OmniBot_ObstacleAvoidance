#include "ws2812b.h"

/******************************************

RGB颜色转换网站

https://www.sioe.cn/yingyong/yanse-rgb-16/

RGB->十六进制    255 0 0    #FF0000



红: #FF0000
橙:	#FFA500
黄:	#FFFF00
绿:	#00FF00
浅蓝:#00FFFF
蓝:	#0000FF
粉:	#FFC0CB
紫: #FF00FF	



*******************************************/

float RGB_R,RGB_G,RGB_B;//HSV转RGB的颜色


void RGB_LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
	GPIO_Init(GPIOB, &GPIO_InitStructure);					
	GPIO_SetBits(GPIOB,GPIO_Pin_0);						 
}




void RGB_LED_Write0(void)
{
	RGB_LED_HIGH;
	__nop();__nop();__nop();__nop();__nop();__nop();
	RGB_LED_LOW;
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
}

/********************************************************/
//
/********************************************************/

void RGB_LED_Write1(void)
{
	RGB_LED_HIGH;
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
	RGB_LED_LOW;
	__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();
	__nop();__nop();
}

void RGB_LED_Reset(void)
{
	RGB_LED_LOW;
	delay_us(80);
}

void RGB_LED_Write_Byte(uint8_t byte)
{
	uint8_t i;

	for(i=0;i<8;i++)
		{
			if(byte&0x80)
				{
					RGB_LED_Write1();
			}
			else
				{
					RGB_LED_Write0();
			}
		byte <<= 1;
	}
}

void RGB_LED_Write_24Bits(uint8_t green,uint8_t red,uint8_t blue)
{
	RGB_LED_Write_Byte(green);
	RGB_LED_Write_Byte(red);
	RGB_LED_Write_Byte(blue);
}


//亮灯颜色设定，其他颜色以此类推
void RGB_LED_Red(void)
{
	 uint8_t i;
	//6个LED全彩灯
	for(i=0;i<6;i++)
		{
			RGB_LED_Write_24Bits(0, 0xff, 0);
	}
}

//亮灯颜色设定，其他颜色以此类推
void RGB_LED_Green(void)
{
	 uint8_t i;
	//6个LED全彩灯
	for(i=0;i<6;i++)
		{
			RGB_LED_Write_24Bits(0xff, 0, 0);
	}
}

//亮灯颜色设定，其他颜色以此类推
void RGB_LED_Blue(void)
{
	 uint8_t i;
	//6个LED全彩灯
	for(i=0;i<6;i++)
		{
			RGB_LED_Write_24Bits(0, 0, 0xff);
	}
}

//RGB灯12种颜色改变
void RGB_Circulation(u8 mode)
{
/*
红: #FF0000
橙:	#FFA500
黄:	#FFFF00
绿:	#00FF00
浅蓝:#00FFFF
蓝:	#0000FF
粉:	#FFC0CB
紫: #FF00FF	
*/

	uint8_t i;
	if(mode==1)
	{
		for(i=0;i<6;i++)
		{
			RGB_LED_Write_24Bits(0xFF, 0, 0);
		}
	}
	else if(mode==2)
	{
		for(i=0;i<6;i++)
		{
			RGB_LED_Write_24Bits(0xFF, 0xA5, 0);
		}
	}
	else if(mode==3)
	{
		for(i=0;i<6;i++)
		{
			RGB_LED_Write_24Bits(0xFF, 0xFF, 0);
		}
	}
	else if(mode==4)
	{
		for(i=0;i<6;i++)
	  {
			RGB_LED_Write_24Bits(0, 0xFF, 0);
	  }
	}
	else if(mode==5)
	{
		for(i=0;i<6;i++)
		{
			RGB_LED_Write_24Bits(0, 0xFF, 0xFF);
		}
	}
	else if(mode==6)
	{
		for(i=0;i<6;i++)
		{
			RGB_LED_Write_24Bits(0, 0, 0xFF);
		}
	}
	else if(mode==7)
	{
		for(i=0;i<6;i++)
		{
			RGB_LED_Write_24Bits(0xFF, 0xC0, 0xCB);
		}
	}
	else if(mode==8)
	{
		for(i=0;i<6;i++)
		{
			RGB_LED_Write_24Bits(0xFF, 0, 0xFF);
		}
	}
}



/*********************************************************************************************
RGB转化到HSV的算法:
    max=max(R,G,B)；
    min=min(R,G,B)；
    V=max(R,G,B)；
    S=(max-min)/max；
    if (R = max) H =(G-B)/(max-min)* 60；
    if (G = max) H = 120+(B-R)/(max-min)* 60；
    if (B = max) H = 240 +(R-G)/(max-min)* 60；
    if (H < 0) H = H + 360；
***********************************************************************************************/
void hsv_to_rgb(int h,int s,int v,float *R,float *G,float *B)
{
    float C = 0,X = 0,Y = 0,Z = 0;
    int i=0;
    float H=(float)(h);
    float S=(float)(s)/100.0;
    float V=(float)(v)/100.0;
    if(S == 0)
        *R = *G = *B = V;
    else
    {
        H = H/60;
        i = (int)H;
        C = H - i;
 
        X = V * (1 - S);
        Y = V * (1 - S*C);
        Z = V * (1 - S*(1-C));
        switch(i)
				{
            case 0 : *R = V; *G = Z; *B = X; break;
            case 1 : *R = Y; *G = V; *B = X; break;
            case 2 : *R = X; *G = V; *B = Z; break;
            case 3 : *R = X; *G = Y; *B = V; break;
            case 4 : *R = Z; *G = X; *B = V; break;
            case 5 : *R = V; *G = X; *B = Y; break;
        }
    }
    *R = *R *255;
    *G = *G *255;
    *B = *B *255;
}

