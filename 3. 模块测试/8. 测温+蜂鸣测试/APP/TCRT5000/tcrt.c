#include "tcrt.h"

/*循迹传感器*/



void TCRT5000_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;//定义一个GPIO_InitTypeDef类型的结构体
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);//开启GPIO端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);//开启ADC1时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	
	
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4; //PA4
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//模拟输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1;            //PB0~PB1
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;//模拟输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	ADC_InitStructure.ADC_Mode=ADC_Mode_Independent;//只使用一个ADC,采用独立模式
	ADC_InitStructure.ADC_ScanConvMode=DISABLE;//禁止扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode=ENABLE;//连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;//不用外部触发
	ADC_InitStructure.ADC_DataAlign=ADC_DataAlign_Right;//转换结果右对齐
	ADC_InitStructure.ADC_NbrOfChannel=1;//转换通道1个
	ADC_Init(ADC1,&ADC_InitStructure);//初始化ADC
	
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);//初始化ADC校准寄存器 
	
	while(ADC_GetResetCalibrationStatus(ADC1));//等待ADC校准寄存器初始化完成
	
	ADC_StartCalibration(ADC1);	 //开启ADC校准
	
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//没有采用外部触发，使用软件触发
}




void Tcrt_Text(void)
{
	
	
}







