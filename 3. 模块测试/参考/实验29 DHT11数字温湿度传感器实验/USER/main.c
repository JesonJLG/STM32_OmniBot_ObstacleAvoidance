#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "dht11.h"


/************************************************
 ALIENTEK精英STM32开发板实验29
 DHT11数字温湿度传感器 实验
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司
 作者：正点原子 @ALIENTEK
************************************************/

int main(void)
{
    u8 t=0;
    u8 temperature;
    u8 humidity;

    delay_init();	    	 //延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
    uart_init(115200);	 	//串口初始化为115200
    LED_Init();		  		//初始化与LED连接的硬件接口
//    LCD_Init();			   	//初始化LCD
//    POINT_COLOR=RED;		//设置字体为红色
//    LCD_ShowString(30,50,200,16,16,"ELITE STM32");
//    LCD_ShowString(30,70,200,16,16,"DHT11 TEST");
//    LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
//    LCD_ShowString(30,110,200,16,16,"2015/1/16");
    while(DHT11_Init())	//DHT11初始化
    {
        printf("DHT11 Error");
        delay_ms(200);
//        LCD_Fill(30,130,239,130+16,WHITE);
//        delay_ms(200);
    }
//    LCD_ShowString(30,130,200,16,16,"DHT11 OK");
//    POINT_COLOR=BLUE;//设置字体为蓝色
//    LCD_ShowString(30,150,200,16,16,"Temp:  C");
//    LCD_ShowString(30,170,200,16,16,"Humi:  %");
    while(1)
    {
        if(t%10==0)			//每100ms读取一次
        {
            DHT11_Read_Data(&temperature,&humidity);	//读取温湿度值
//            LCD_ShowNum(30+40,150,temperature,2,16);	//显示温度
//            LCD_ShowNum(30+40,170,humidity,2,16);		//显示湿度
			printf("temperature:%d", temperature);
			printf("humidity:%d", humidity);
        }
        delay_ms(10);
        t++;
        if(t==20)
        {
            t=0;
            LED0=!LED0;
        }
    }
}




