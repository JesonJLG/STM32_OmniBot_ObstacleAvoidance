#include "dma.h"
#include "led.h"
#include "delay.h"
#include "usart.h"

//const 关键字将数组定义为常量类型
//表示数据存放在内部的Flash存储器中
const uint32_t Buffer1[10]={0,1,2,3,4,5,6,7,8,9};

//存储在内部的SRAM中
uint32_t Buffer2[10];

 #define SendBuff_Size 10
 u8 SendBuffe[SendBuff_Size];//定义一个发送数组


/**************************************************
函数名称：DMA1_Init(void)
函数功能：DMA1初始化函数
入口参数：无
返回参数：无
***************************************************/
void DMA1_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//开启DMA时钟
	
	
	DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t)Buffer1;//源数据地址,看成外设
	DMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)Buffer2;//目标地址,一个空数组
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;//方向:存储器到存储器
	DMA_InitStructure.DMA_BufferSize=10;//传输大小
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Enable;//外设地址递增
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;//内存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Word;//外设数据单位
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Word;//内存数据单位
	DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;//DMA模式，一次Normal，循环Circular
	
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;//优先级:高
	DMA_InitStructure.DMA_M2M=DMA_M2M_Enable;//使能内存到内存的传输
	
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); //配置DMA通道
	DMA_Cmd(DMA1_Channel1,ENABLE);//使能DMA
	
}


/**************************************************
函数名称：USART1_DMA1_Init(void)
函数功能：USART1_DMA1初始化函数
入口参数：无
返回参数：无
***************************************************/
void USART1_DMA1_Init(void)
{
	//1.定义结构体
 	DMA_InitTypeDef DMA_InitStruct;
 	//2、配置DMA时钟,USART1_TX使用DMA1的通道4
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//使能DMA1通道4的时钟
 	
 	//3、数据从哪里来到哪里去（配置3个）
 	//外设地址（串口数据寄存器）--目标地址
 	DMA_InitStruct.DMA_PeripheralBaseAddr=(u32)&USART1->DR;//外设地址（或者USART1_BASE+0x04）
 	//存储器地址---源地址
 	DMA_InitStruct.DMA_MemoryBaseAddr=(uint32_t)SendBuffe;//存储器地址（指向了数组首地址）
 	//方向：存储器到外设（外设串口作为目的地）
 	DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralDST;//传输方式（MTP）
 	//规定外设是作为数据传输的目的地还是来源（数据传输方向）
 	//DMA_DIR_PeripheralDST	外设作为数据传输的目的地
 	//DMA_DIR_PeripheralSRC	外设作为数据传输的来源
 	
 	
 	//4、传多少，单位是多少
 	DMA_InitStruct.DMA_BufferSize= SendBuff_Size;//传输数目（数组的长度）
 	
 	//只有一个串口数据寄存器不需要递增，数组是U8类型,所以一次传输一个字节
 	DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable ;//外设地址b不需要递增
 	DMA_InitStruct.DMA_PeripheralDataSize= DMA_PeripheralDataSize_Byte  ;//数据宽度8位，u8类型，一个字节
 	
 	//配置memory（定义了一个数组，发送一个会继续下一个，所以地址是递增）
 	DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable ;//内存地址递增
 	DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte  ;//一个字节
 	
 	
 	//5、配置模式和优先级
 	DMA_InitStruct.DMA_Mode=DMA_Mode_Normal ;//正常模式或者是循环模式
 	DMA_InitStruct.DMA_Priority=DMA_Priority_High ;//共有四种优先级
 	DMA_InitStruct.DMA_M2M=DMA_M2M_Disable  ;//不使用M-M模式
 	DMA_Init(DMA1_Channel4, &DMA_InitStruct);//串口1TX是DMA通道4
 	//
 	
 	DMA_ClearFlag(DMA1_FLAG_TC4);//先将这个标志位清除
 	//6、使能DMA
 	DMA_Cmd(DMA1_Channel4, ENABLE);

}



//比较两数组数据
uint8_t Buffercmp(const uint32_t* pBuffer1,uint32_t* pBuffer2,uint32_t BufferSize)
{
	while(BufferSize--)
	{
		if(*pBuffer1==!*pBuffer2)
			return 0;
		pBuffer1++;
		pBuffer2++;
	}
	return 1;
}

/**************************************************
函数名称：DMA_Text(void)
函数功能：DMA测试函数
入口参数：无
返回参数：无
***************************************************/
void DMA_Text(void)
{
	int i;
	for(i=0;i<10;i++)
	{
		printf("Buffer1[%d]=%d\n",i,Buffer1[i]);
		printf("Buffer2[%d]=%d\n",i,Buffer2[i]);
		printf("\n");
	}
	while(1)
	{
		if(Buffercmp(Buffer1,Buffer2,10)==0)
		{
			LED=~LED;
			delay_ms(500);
		}
		else 
			LED=0;
	}
}


//填充发送数组	
void USART_DMA_Text(void)
{
	u16 i;
 	for(i=0;i<SendBuff_Size;i++)//对数组填充数据
 	{
 		SendBuffe[i]='b';
 	}
}


