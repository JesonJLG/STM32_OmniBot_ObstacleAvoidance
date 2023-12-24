#include "joystick.h"


u16 ADC_Data[2];//ADC���ݴ洢


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
	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1; //PA0~PA1
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//ģ������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;//ֻʹ��һ��ADC,���ö���ģʽ
	ADC_InitStructure.ADC_ScanConvMode=ENABLE;//ɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;//����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//�����ⲿ����
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;//ת������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel=2;//ת��ͨ��2��
	ADC_Init(ADC1,&ADC_InitStructure);//��ʼ��ADC
	
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_DMACmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);//��ʼ��ADCУ׼�Ĵ��� 
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_0, 1, ADC_SampleTime_239Cycles5 );
  ADC_RegularChannelConfig(ADC1,ADC_Channel_1, 1, ADC_SampleTime_239Cycles5 );		
	
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
	DMA_InitStructure.DMA_BufferSize=2;//�����С
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

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;              //��������    
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;           //��������    
  GPIO_Init(GPIOB,&GPIO_InitStructure);       //��ʼ��GPIOB
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource0); //����GPIOΪ�ж�����
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line0;						//�ж���0
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;		//�ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling;//�½��ش���
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;          		//ʹ���ж�
	
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�0
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
	JoyStick_Key_Init();
	JoyStick_ADC_Init();
	JoyStick_DMA_Init();
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
		EXTI_ClearITPendingBit(EXTI_Line0);
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






