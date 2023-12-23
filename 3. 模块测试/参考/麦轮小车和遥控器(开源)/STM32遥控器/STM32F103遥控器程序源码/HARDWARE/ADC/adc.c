#include "adc.h"


/**************************************************
�������ƣ�ADC1_Init(void)
�������ܣ�ADC��ʼ������
��ڲ�������
���ز�������
***************************************************/
void ADC1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����һ��GPIO_InitTypeDef���͵Ľṹ��
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//����GPIO�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);//����ADC1ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2; //PA0~PA2
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3; //PA3
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//ģ������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;//ֻʹ��һ��ADC,���ö���ģʽ
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;//��ֹɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;//����ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//�����ⲿ����
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;//ת������Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel=1;//ת��ͨ��1��
	ADC_Init(ADC1,&ADC_InitStructure);//��ʼ��ADC
	
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);//��ʼ��ADCУ׼�Ĵ��� 
	
	while(ADC_GetResetCalibrationStatus(ADC1));//�ȴ�ADCУ׼�Ĵ�����ʼ�����
	
	ADC_StartCalibration(ADC1);	 //����ADCУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//û�в����ⲿ������ʹ���������

}



/**************************************************
�������ƣ�Get_ADC(u8 ch)
�������ܣ���ȡADCת����ĵ�ѹֵ
��ڲ�����ch  ͨ��1~9,ADC_Channel_1~ADC_Channel_9
���ز�����temp_val    ��ѹֵ
***************************************************/
float Get_ADC(u8 ch)
{
	float temp_val;
	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	//ADC1_chͨ��,��һ��ת����239������;��߲���ʱ�������߾�ȷ��
	ADC_RegularChannelConfig(ADC1,ch, 1, ADC_SampleTime_239Cycles5 );				    
  
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������
	
	temp_val = ADC_GetConversionValue(ADC1);

	temp_val = temp_val*(3.3/4096);

	return temp_val;
}


