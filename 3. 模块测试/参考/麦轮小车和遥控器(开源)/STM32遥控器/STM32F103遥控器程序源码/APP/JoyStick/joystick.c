#include "joystick.h"


u16 ADC_Data[4];//ADC数据存储
int Joy_Lx,Joy_Ly,Joy_Rx,Joy_Ry;
u16 L_Dir,R_Dir;
int Lx_val,Ly_val,Rx_val,Ry_val;
int Key_flag1=0,Key_flag2=0;



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
	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; //PA0~PA3
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//模拟输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;//只使用一个ADC,采用独立模式
	ADC_InitStructure.ADC_ScanConvMode=ENABLE;//扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;//连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//不用外部触发
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;//转换结果右对齐
	ADC_InitStructure.ADC_NbrOfChannel=4;//转换通道1个
	ADC_Init(ADC1,&ADC_InitStructure);//初始化ADC
	
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_DMACmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);//初始化ADC校准寄存器 
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0, 1, ADC_SampleTime_239Cycles5 );
  ADC_RegularChannelConfig(ADC1,ADC_Channel_1, 2, ADC_SampleTime_239Cycles5 );		
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2, 3, ADC_SampleTime_239Cycles5 );
  ADC_RegularChannelConfig(ADC1,ADC_Channel_3, 4, ADC_SampleTime_239Cycles5 );
	
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
	DMA_InitStructure.DMA_BufferSize=4;//传输大小
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
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;              //配置引脚    
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;           //上拉输入    
  GPIO_Init(GPIOB,&GPIO_InitStructure);       //初始化GPIOB

	
	EXTI_InitStructure.EXTI_Line=EXTI_Line0|EXTI_Line1;						//中断线0,1
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;		//中断模式
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//下降沿触发
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;          		//使能中断
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	                        //初始化NVIC寄存器
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级0
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
	JoyStick_DMA_Init();
	JoyStick_Key_Init();
	JoyStick_ADC_Init();
	//ADC1_Init();
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
		Key_flag1=1;
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

/**************************************************
函数名称：EXTI1_IRQHandler(void)
函数功能：外部中断1函数
入口参数：无
返回参数：无
***************************************************/
void EXTI1_IRQHandler(void)//中断服务函数
{
	if(EXTI_GetITStatus(EXTI_Line1)!=RESET)
	{
		LED=~LED;
		Key_flag1=2;
		EXTI_ClearITPendingBit(EXTI_Line1);
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
    if(key_up && (KEY_Z0==1||KEY_Z1==1))
    {
       delay_ms(10);
       key_up=0;
       if(KEY_Z0==1||KEY_Z1==1) 
           return 1;
    }
    else if(KEY_Z0==0||KEY_Z1==0) 
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



/**************************************************
函数名称：JoyStick_Data()
函数功能：摇杆数据转换函数
入口参数：无
返回参数：无
***************************************************/
void JoyStick_Data(void)
{	
	Joy_Lx= (int)((ADC_Data[2]/4095.0)*3.3*100);
	Joy_Ly= (int)((ADC_Data[3]/4095.0)*3.3*100);
	Joy_Rx= (int)((ADC_Data[0]/4095.0)*3.3*100);
	Joy_Ry= (int)((ADC_Data[1]/4095.0)*3.3*100);

	
	if((Joy_Lx>=130&&Joy_Lx<=200)&&(Joy_Ly>=0&&Joy_Ly<=60))L_Dir=0;   //左摇杆上
	if((Joy_Lx>=130&&Joy_Lx<=200)&&(Joy_Ly>=300&&Joy_Ly<=330))L_Dir=1;//左摇杆下
	if((Joy_Lx>=270&&Joy_Lx<=330)&&(Joy_Ly>=130&&Joy_Ly<=200))L_Dir=2;//左摇杆左
	if((Joy_Lx>=0&&Joy_Lx<=60)&&(Joy_Ly>=130&&Joy_Ly<=200))L_Dir=3;   //左摇杆右
	
	if((Joy_Lx>=270&&Joy_Lx<=330)&&(Joy_Ly>=0&&Joy_Ly<=60))L_Dir=4;   //左摇杆左上
	if((Joy_Lx>=0&&Joy_Lx<=60)&&(Joy_Ly>=0&&Joy_Ly<=60))L_Dir=5;      //左摇杆右上
	if((Joy_Lx>=270&&Joy_Lx<=330)&&(Joy_Ly>=270&&Joy_Ly<=330))L_Dir=6;//左摇杆左下
	if((Joy_Lx>=0&&Joy_Lx<=60)&&(Joy_Ly>=270&&Joy_Ly<=330))L_Dir=7;   //左摇杆右下
	
	if((Joy_Rx>=130&&Joy_Rx<=200)&&(Joy_Ry>=0&&Joy_Ry<=60))R_Dir=8;    //右摇杆上
	if((Joy_Rx>=130&&Joy_Rx<=200)&&(Joy_Ry>=270&&Joy_Ry<=330))R_Dir=9; //右摇杆下
	if((Joy_Rx>=270&&Joy_Rx<=330)&&(Joy_Ry>=130&&Joy_Ry<=200))R_Dir=10;//右摇杆左
	if((Joy_Rx>=0&&Joy_Rx<=60)&&(Joy_Ry>=130&&Joy_Ry<=200))R_Dir=11;   //右摇杆右
	
	if((Joy_Rx>=270&&Joy_Rx<=330)&&(Joy_Ry>=0&&Joy_Ry<=60))R_Dir=12;   //右摇杆左上
	if((Joy_Rx>=0&&Joy_Rx<=60)&&(Joy_Ry>=0&&Joy_Ry<=60))R_Dir=13;      //右摇杆右上
	if((Joy_Rx>=270&&Joy_Rx<=330)&&(Joy_Ry>=270&&Joy_Ry<=330))R_Dir=14;//右摇杆左下
	if((Joy_Rx>=0&&Joy_Rx<=60)&&(Joy_Ry>=270&&Joy_Ry<=330))R_Dir=15;   //右摇杆右下
	
	if((Joy_Lx>=130&&Joy_Lx<=200)&&(Joy_Ly>=130&&Joy_Ly<=200))L_Dir=16;   //左摇杆中间
	if((Joy_Rx>=130&&Joy_Rx<=200)&&(Joy_Ry>=130&&Joy_Ry<=200))R_Dir=17;   //右摇杆中间
	
}

/**************************************************
函数名称：JoyStick_OledShow(void)
函数功能：摇杆数据OLED显示函数
入口参数：无
返回参数：无
***************************************************/
void JoyStick_OledShow(void)
{
	Joy_Lx= ADC_Data[2];			
	Joy_Ly= ADC_Data[3];
	Joy_Rx= ADC_Data[0];			
	Joy_Ry= ADC_Data[1];
		
	Joy_Lx= (int)((Joy_Lx/4095.0)*3.3*100);
	Joy_Ly= (int)((Joy_Ly/4095.0)*3.3*100);
	Joy_Rx= (int)((Joy_Rx/4095.0)*3.3*100);
	Joy_Ry= (int)((Joy_Ry/4095.0)*3.3*100);
	
	OLED_DrawLine(5,5,125,5,1);
	OLED_DrawLine(5,5,5,45,1);
	OLED_DrawLine(5,45,125,45,1);
	OLED_DrawLine(125,5,125,45,1);
	
	OLED_ShowString(12,8,"Lx:",16,1);
	OLED_ShowNum(36,8,Joy_Lx,3,16,1);
	OLED_ShowString(68,8,"Ly:",16,1);
	OLED_ShowNum(100,8,Joy_Ly,3,16,1);
	
	OLED_ShowString(12,24,"Rx:",16,1);
	OLED_ShowNum(36,24,Joy_Rx,3,16,1);
	OLED_ShowString(68,24,"Ry:",16,1);
	OLED_ShowNum(100,24,Joy_Ry,3,16,1);
	OLED_Refresh();
//	printf("Joy_Lx=%d,  Joy_Ly=%d\n",Joy_Lx,Joy_Ly);
//	printf("Joy_Rx=%d,  Joy_Ry=%d\n",Joy_Rx,Joy_Ry);
//	printf("\n");
	delay_ms(500);
}


int Map(int val,int in_min,int in_max,int out_min,int out_max)
{
	return (int)(val-in_min)*(out_max-out_min)/(in_max-in_min)+out_min;
}


/**************************************************
函数名称：void JoyStick_DirSend(void)
函数功能：摇杆数据发送函数
入口参数：无
返回参数：无
***************************************************/
void JoyStick_DirSend(void)
{
	
	u8 Joy_TxBuf[20]="0";
	
	L_Dir=100,R_Dir=100;
	
	JoyStick_Data();
	
	OLED_DrawLine(5,5,125,5,1);
	OLED_DrawLine(5,5,5,45,1);
	OLED_DrawLine(5,45,125,45,1);
	OLED_DrawLine(125,5,125,45,1);
	
	OLED_ShowString(12,8,"Lx:",16,1);
	OLED_ShowNum(36,8,Joy_Lx,3,16,1);
	OLED_ShowString(68,8,"Ly:",16,1);
	OLED_ShowNum(100,8,Joy_Ly,3,16,1);
	
	OLED_ShowString(12,24,"Rx:",16,1);
	OLED_ShowNum(36,24,Joy_Rx,3,16,1);
	OLED_ShowString(68,24,"Ry:",16,1);
	OLED_ShowNum(100,24,Joy_Ry,3,16,1);
	OLED_Refresh();
	
	
	Lx_val=Map(Joy_Lx,0,330,10,90);
	Ly_val=Map(Joy_Ly,0,330,10,90);
	Rx_val=Map(Joy_Rx,0,330,10,90);
	Ry_val=Map(Joy_Ry,0,330,10,90);
	
	Joy_TxBuf[0]='L';
	Joy_TxBuf[1]='X';
	Joy_TxBuf[2]=Lx_val/10+'0';
	Joy_TxBuf[3]=Lx_val%10+'0';
	Joy_TxBuf[4]='Y';
	Joy_TxBuf[5]=Ly_val/10+'0';
	Joy_TxBuf[6]=Ly_val%10+'0';
	Joy_TxBuf[7]='R';
	Joy_TxBuf[8]='X';
	Joy_TxBuf[9]= Rx_val/10+'0';
	Joy_TxBuf[10]=Rx_val%10+'0';
	Joy_TxBuf[11]='Y';
	Joy_TxBuf[12]=Ry_val/10+'0';
	Joy_TxBuf[13]=Ry_val%10+'0';
	
	Joy_TxBuf[19]='0';
	
	
	NRF24L01_TX_Mode();
	if(NRF24L01_TxPacket(Joy_TxBuf)==TX_OK)
	{
			LED=~LED;
	}
	else
	{										   	
 		printf("Send failed !\n");	
	}
	
	//printf("L_Dir=%d\n",L_Dir);
	//printf("R_Dir=%d\n",R_Dir);
	//printf((char*)Joy_TxBuf);
	//printf("\n");
	
	delay_ms(10);
}


