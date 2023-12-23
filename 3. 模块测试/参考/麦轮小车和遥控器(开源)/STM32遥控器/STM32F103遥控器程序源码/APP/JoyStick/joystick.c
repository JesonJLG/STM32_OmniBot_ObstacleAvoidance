#include "joystick.h"


u16 ADC_Data[4];//ADC���ݴ洢
int Joy_Lx,Joy_Ly,Joy_Rx,Joy_Ry;
u16 L_Dir,R_Dir;
int Lx_val,Ly_val,Rx_val,Ry_val;
int Key_flag1=0,Key_flag2=0;



/**************************************************
�������ƣ�JoyStick_ADC_Init()
�������ܣ�ҡ��ADC��ʼ������
��ڲ�������
���ز�������
***************************************************/
void JoyStick_ADC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����һ��GPIO_InitTypeDef���͵Ľṹ��
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//����GPIO�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);//����ADC1ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3; //PA0~PA3
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//ģ������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;//ֻʹ��һ��ADC,���ö���ģʽ
	ADC_InitStructure.ADC_ScanConvMode=ENABLE;//ɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;//����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//�����ⲿ����
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;//ת������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel=4;//ת��ͨ��1��
	ADC_Init(ADC1,&ADC_InitStructure);//��ʼ��ADC
	
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_DMACmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);//��ʼ��ADCУ׼�Ĵ��� 
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0, 1, ADC_SampleTime_239Cycles5 );
  ADC_RegularChannelConfig(ADC1,ADC_Channel_1, 2, ADC_SampleTime_239Cycles5 );		
	ADC_RegularChannelConfig(ADC1,ADC_Channel_2, 3, ADC_SampleTime_239Cycles5 );
  ADC_RegularChannelConfig(ADC1,ADC_Channel_3, 4, ADC_SampleTime_239Cycles5 );
	
	while(ADC_GetResetCalibrationStatus(ADC1));//�ȴ�ADCУ׼�Ĵ�����ʼ�����
	
	ADC_StartCalibration(ADC1);	 //����ADCУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//û�в����ⲿ������ʹ���������
	
}


/**************************************************
�������ƣ�JoyStick_DMA_Init()
�������ܣ�ҡ������ͨ��DMA����
��ڲ�������
���ز�������
***************************************************/
void JoyStick_DMA_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//����DMAʱ��
	
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t)&(ADC1->DR);//Դ���ݵ�ַ,��������
	DMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)&ADC_Data;//Ŀ���ַ,һ��������
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;//����:�洢��(����)���洢��
	DMA_InitStructure.DMA_BufferSize=4;//�����С
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Disable;//�����ַ�������������ַֻ��һ��
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;//�ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;//�������ݵ�λ
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;//�ڴ����ݵ�λ
	DMA_InitStructure.DMA_Mode=DMA_Mode_Circular;//DMAģʽ��һ��Normal��ѭ��Circular
	
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;//���ȼ�:��
	DMA_InitStructure.DMA_M2M=DMA_M2M_Disable;//ʹ���ڴ浽�ڴ�Ĵ���
	
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); //����DMAͨ��
	DMA_Cmd(DMA1_Channel1,ENABLE);//ʹ��DMA
	
}

/**************************************************
�������ƣ�JoyStick_Key_Init()
�������ܣ�ҡ�˰�����ʼ������
��ڲ�������
���ز�������
***************************************************/
void JoyStick_Key_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;                    //����GPIO�ṹ��    
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);    //ʹ��GPIOBʱ��  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;              //��������    
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;           //��������    
  GPIO_Init(GPIOB,&GPIO_InitStructure);       //��ʼ��GPIOB

	
	EXTI_InitStructure.EXTI_Line=EXTI_Line0|EXTI_Line1;						//�ж���0,1
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;		//�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//�½��ش���
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;          		//ʹ���ж�
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                        //��ʼ��NVIC�Ĵ���
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;			//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	                        //��ʼ��NVIC�Ĵ���

}


/**************************************************
�������ƣ�JoyStick_Init()
�������ܣ�ҡ�˳�ʼ������
��ڲ�������
���ز�������
***************************************************/
void JoyStick_Init(void)
{
	JoyStick_DMA_Init();
	JoyStick_Key_Init();
	JoyStick_ADC_Init();
	//ADC1_Init();
}


/**************************************************
�������ƣ�EXTI0_IRQHandler(void)
�������ܣ��ⲿ�ж�0����
��ڲ�������
���ز�������
***************************************************/
void EXTI0_IRQHandler(void)//�жϷ�����
{
	if(EXTI_GetITStatus(EXTI_Line0)!=RESET)
	{
		LED=~LED;
		Key_flag1=1;
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

/**************************************************
�������ƣ�EXTI1_IRQHandler(void)
�������ܣ��ⲿ�ж�1����
��ڲ�������
���ز�������
***************************************************/
void EXTI1_IRQHandler(void)//�жϷ�����
{
	if(EXTI_GetITStatus(EXTI_Line1)!=RESET)
	{
		LED=~LED;
		Key_flag1=2;
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
}

/**************************************************
�������ƣ�JoyStick_Key_Scan(int mode)
�������ܣ�ҡ�˰���ɨ�躯��
��ڲ�����mode 0 ��֧��������  1 ֧��������
���ز�����1 ��������
          0 ����û�а���
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
�������ƣ�JoyStick_Text()
�������ܣ�ҡ�˲��Ժ���
��ڲ�������
���ز�������
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
�������ƣ�JoyStick_Data()
�������ܣ�ҡ������ת������
��ڲ�������
���ز�������
***************************************************/
void JoyStick_Data(void)
{	
	Joy_Lx= (int)((ADC_Data[2]/4095.0)*3.3*100);
	Joy_Ly= (int)((ADC_Data[3]/4095.0)*3.3*100);
	Joy_Rx= (int)((ADC_Data[0]/4095.0)*3.3*100);
	Joy_Ry= (int)((ADC_Data[1]/4095.0)*3.3*100);

	
	if((Joy_Lx>=130&&Joy_Lx<=200)&&(Joy_Ly>=0&&Joy_Ly<=60))L_Dir=0;   //��ҡ����
	if((Joy_Lx>=130&&Joy_Lx<=200)&&(Joy_Ly>=300&&Joy_Ly<=330))L_Dir=1;//��ҡ����
	if((Joy_Lx>=270&&Joy_Lx<=330)&&(Joy_Ly>=130&&Joy_Ly<=200))L_Dir=2;//��ҡ����
	if((Joy_Lx>=0&&Joy_Lx<=60)&&(Joy_Ly>=130&&Joy_Ly<=200))L_Dir=3;   //��ҡ����
	
	if((Joy_Lx>=270&&Joy_Lx<=330)&&(Joy_Ly>=0&&Joy_Ly<=60))L_Dir=4;   //��ҡ������
	if((Joy_Lx>=0&&Joy_Lx<=60)&&(Joy_Ly>=0&&Joy_Ly<=60))L_Dir=5;      //��ҡ������
	if((Joy_Lx>=270&&Joy_Lx<=330)&&(Joy_Ly>=270&&Joy_Ly<=330))L_Dir=6;//��ҡ������
	if((Joy_Lx>=0&&Joy_Lx<=60)&&(Joy_Ly>=270&&Joy_Ly<=330))L_Dir=7;   //��ҡ������
	
	if((Joy_Rx>=130&&Joy_Rx<=200)&&(Joy_Ry>=0&&Joy_Ry<=60))R_Dir=8;    //��ҡ����
	if((Joy_Rx>=130&&Joy_Rx<=200)&&(Joy_Ry>=270&&Joy_Ry<=330))R_Dir=9; //��ҡ����
	if((Joy_Rx>=270&&Joy_Rx<=330)&&(Joy_Ry>=130&&Joy_Ry<=200))R_Dir=10;//��ҡ����
	if((Joy_Rx>=0&&Joy_Rx<=60)&&(Joy_Ry>=130&&Joy_Ry<=200))R_Dir=11;   //��ҡ����
	
	if((Joy_Rx>=270&&Joy_Rx<=330)&&(Joy_Ry>=0&&Joy_Ry<=60))R_Dir=12;   //��ҡ������
	if((Joy_Rx>=0&&Joy_Rx<=60)&&(Joy_Ry>=0&&Joy_Ry<=60))R_Dir=13;      //��ҡ������
	if((Joy_Rx>=270&&Joy_Rx<=330)&&(Joy_Ry>=270&&Joy_Ry<=330))R_Dir=14;//��ҡ������
	if((Joy_Rx>=0&&Joy_Rx<=60)&&(Joy_Ry>=270&&Joy_Ry<=330))R_Dir=15;   //��ҡ������
	
	if((Joy_Lx>=130&&Joy_Lx<=200)&&(Joy_Ly>=130&&Joy_Ly<=200))L_Dir=16;   //��ҡ���м�
	if((Joy_Rx>=130&&Joy_Rx<=200)&&(Joy_Ry>=130&&Joy_Ry<=200))R_Dir=17;   //��ҡ���м�
	
}

/**************************************************
�������ƣ�JoyStick_OledShow(void)
�������ܣ�ҡ������OLED��ʾ����
��ڲ�������
���ز�������
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
�������ƣ�void JoyStick_DirSend(void)
�������ܣ�ҡ�����ݷ��ͺ���
��ڲ�������
���ز�������
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


