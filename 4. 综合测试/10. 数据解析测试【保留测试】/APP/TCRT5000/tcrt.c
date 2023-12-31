#include "tcrt.h"

/*ѭ��������*/



void TCRT5000_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//����һ��GPIO_InitTypeDef���͵Ľṹ��
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);//����GPIO�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);//����ADC1ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	
	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4; //PA4
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//ģ������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;            //PB0~PB1
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//ģ������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
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




void Tcrt_Text(void)
{
	
	
}







