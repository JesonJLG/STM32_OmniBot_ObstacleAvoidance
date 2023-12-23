#include "joystick.h"


u16 ADC_Data[2];//ADC数据存储


/**************************************************
函数名称：JoyStick_ADC_Init()
函数功能：摇杆ADC初始化函数
入口参数：无
返回参数：无
***************************************************/
void JoyStick_ADC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义一个GPIO_InitTypeDef类型的结构体
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//开启GPIO端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);//开启ADC1时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1; //PA0~PA1
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//模拟输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;//只使用一个ADC,采用独立模式
	ADC_InitStructure.ADC_ScanConvMode=ENABLE;//扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;//连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//不用外部触发
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;//转换结果右对齐
	ADC_InitStructure.ADC_NbrOfChannel=2;//转换通道2个
	ADC_Init(ADC1,&ADC_InitStructure);//初始化ADC
	
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_DMACmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);//初始化ADC校准寄存器 
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0, 1, ADC_SampleTime_239Cycles5 );
  ADC_RegularChannelConfig(ADC1,ADC_Channel_1, 1, ADC_SampleTime_239Cycles5 );		
	
	while(ADC_GetResetCalibrationStatus(ADC1));//等待ADC校准寄存器初始化完成
	
	ADC_StartCalibration(ADC1);	 //开启ADC校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//没有采用外部触发，使用软件触发
	
}


/**************************************************
函数名称：JoyStick_DMA_Init()
函数功能：摇杆数据通过DMA传输
入口参数：无
返回参数：无
***************************************************/
void JoyStick_DMA_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//开启DMA时钟
	
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t)&(ADC1->DR);//源数据地址,看成外设
	DMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)&ADC_Data;//目标地址,一个空数组
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;//方向:存储器(外设)到存储器
	DMA_InitStructure.DMA_BufferSize=2;//传输大小
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;//外设地址不递增，外设地址只有一个
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;//内存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;//外设数据单位
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;//内存数据单位
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;//DMA模式，一次Normal，循环Circular
	
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;//优先级:高
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;//使能内存到内存的传输
	
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); //配置DMA通道
	DMA_Cmd(DMA1_Channel1,ENABLE);//使能DMA
	
}

/**************************************************
函数名称：JoyStick_Key_Init()
函数功能：摇杆按键初始化函数
入口参数：无
返回参数：无
***************************************************/
void JoyStick_Key_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;                    //定义GPIO结构体    
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);    //使能GPIOB时钟  

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;              //配置引脚    
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;           //上拉输入    
  GPIO_Init(GPIOB,&GPIO_InitStructure);       //初始化GPIOB
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0); //配置GPIO为中断引脚
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;						//中断线0
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;		//中断模式
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//下降沿触发
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;          		//使能中断
	
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	                        //初始化NVIC寄存器


}


/**************************************************
函数名称：JoyStick_Init()
函数功能：摇杆初始化函数
入口参数：无
返回参数：无
***************************************************/
void JoyStick_Init(void)
{
	JoyStick_Key_Init();
	JoyStick_ADC_Init();
	JoyStick_DMA_Init();
}


/**************************************************
函数名称：EXTI0_IRQHandler(void)
函数功能：外部中断0函数
入口参数：无
返回参数：无
***************************************************/
void EXTI0_IRQHandler(void)//中断服务函数
{
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
	{
		LED=~LED;
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

/**************************************************
函数名称：JoyStick_Key_Scan(int mode)
函数功能：摇杆按键扫描函数
入口参数：mode 0 不支持连续按  1 支持连续按
返回参数：1 按键按下
          0 按键没有按下
***************************************************/
int JoyStick_Key_Scan(int mode)
{
    static u8 key_up=1;
    if(mode) key_up=1;
    if(key_up && (KEY_Z==1))
    {
       delay_ms(10);
       key_up=0;
       if(KEY_Z==1) 
           return 1;
    }
    else if(KEY_Z==0) 
        key_up=1;
    return 0;
}


/**************************************************
函数名称：JoyStick_Text()
函数功能：摇杆测试函数
入口参数：无
返回参数：无
***************************************************/
void JoyStick_Text()
{
		u32 adcx,adcy;
		u8 key;
		key=JoyStick_Key_Scan(0);
	  if(key)
		{
			printf("JoyStick_Key YES!");
			printf("\n");
		}
		adcx= ADC_Data[0];			
		adcy= ADC_Data[1];	
		printf("adcx=%.3f,  adcy=%.3f\r\n", (adcx/4095.0)*3.3 , (adcy/4095.0)*3.3);  
		printf("\r\n");
		delay_ms(500);
}






