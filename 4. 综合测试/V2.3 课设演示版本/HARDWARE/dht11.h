#ifndef __DHT11_H
#define __DHT11_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//DHT11数字温湿度传感器驱动代码
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/12
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//////////////////////////////////////////////////////////////////////////////////

#define DHT11_PORT	GPIOD
#define DHT11_DATA	GPIO_Pin_1

//IO方向设置
#define DHT11_IO_IN()  {GPIOD->CRL&=0XFFFFFF0F;GPIOD->CRL|=(u32)8<<4;}
#define DHT11_IO_OUT() {GPIOD->CRL&=0XFFFFFF0F;GPIOD->CRL|=(u32)3<<4;}
////IO操作函数
#define	DHT11_DQ_OUT PDout(1) //数据端口输出（写）
#define	DHT11_DQ_IN  PDin(1)  //数据端口输入（读）


u8 DHT11_Init(void);//初始化DHT11
u8 DHT11_Read_Data(u8* temp, u8* humi);//读取温湿度
u8 DHT11_Read_Byte(void);//读出一个字节
u8 DHT11_Read_Bit(void);//读出一个位
u8 DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11
#endif















