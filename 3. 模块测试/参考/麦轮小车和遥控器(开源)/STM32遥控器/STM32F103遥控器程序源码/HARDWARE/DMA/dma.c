#include "dma.h"
#include "led.h"
#include "delay.h"
#include "usart.h"

//const �ؼ��ֽ����鶨��Ϊ��������
//��ʾ���ݴ�����ڲ���Flash�洢����
const uint32_t Buffer1[10]={0,1,2,3,4,5,6,7,8,9};

//�洢���ڲ���SRAM��
uint32_t Buffer2[10];

 #define SendBuff_Size 10
 u8 SendBuffe[SendBuff_Size];//����һ����������


/**************************************************
�������ƣ�DMA1_Init(void)
�������ܣ�DMA1��ʼ������
��ڲ�������
���ز�������
***************************************************/
void DMA1_Init(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);//����DMAʱ��
	
	
	DMA_InitStructure.DMA_PeripheralBaseAddr=(uint32_t)Buffer1;//Դ���ݵ�ַ,��������
	DMA_InitStructure.DMA_MemoryBaseAddr=(uint32_t)Buffer2;//Ŀ���ַ,һ��������
	DMA_InitStructure.DMA_DIR=DMA_DIR_PeripheralSRC;//����:�洢�����洢��
	DMA_InitStructure.DMA_BufferSize=10;//�����С
	DMA_InitStructure.DMA_PeripheralInc=DMA_PeripheralInc_Enable;//�����ַ����
	DMA_InitStructure.DMA_MemoryInc=DMA_MemoryInc_Enable;//�ڴ��ַ����
	DMA_InitStructure.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Word;//�������ݵ�λ
	DMA_InitStructure.DMA_MemoryDataSize=DMA_MemoryDataSize_Word;//�ڴ����ݵ�λ
	DMA_InitStructure.DMA_Mode=DMA_Mode_Normal;//DMAģʽ��һ��Normal��ѭ��Circular
	
	DMA_InitStructure.DMA_Priority=DMA_Priority_High;//���ȼ�:��
	DMA_InitStructure.DMA_M2M=DMA_M2M_Enable;//ʹ���ڴ浽�ڴ�Ĵ���
	
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); //����DMAͨ��
	DMA_Cmd(DMA1_Channel1,ENABLE);//ʹ��DMA
	
}


/**************************************************
�������ƣ�USART1_DMA1_Init(void)
�������ܣ�USART1_DMA1��ʼ������
��ڲ�������
���ز�������
***************************************************/
void USART1_DMA1_Init(void)
{
	//1.����ṹ��
 	DMA_InitTypeDef DMA_InitStruct;
 	//2������DMAʱ��,USART1_TXʹ��DMA1��ͨ��4
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);//ʹ��DMA1ͨ��4��ʱ��
 	
 	//3�����ݴ�������������ȥ������3����
 	//�����ַ���������ݼĴ�����--Ŀ���ַ
 	DMA_InitStruct.DMA_PeripheralBaseAddr=(u32)&USART1->DR;//�����ַ������USART1_BASE+0x04��
 	//�洢����ַ---Դ��ַ
 	DMA_InitStruct.DMA_MemoryBaseAddr=(uint32_t)SendBuffe;//�洢����ַ��ָ���������׵�ַ��
 	//���򣺴洢�������裨���贮����ΪĿ�ĵأ�
 	DMA_InitStruct.DMA_DIR=DMA_DIR_PeripheralDST;//���䷽ʽ��MTP��
 	//�涨��������Ϊ���ݴ����Ŀ�ĵػ�����Դ�����ݴ��䷽��
 	//DMA_DIR_PeripheralDST	������Ϊ���ݴ����Ŀ�ĵ�
 	//DMA_DIR_PeripheralSRC	������Ϊ���ݴ������Դ
 	
 	
 	//4�������٣���λ�Ƕ���
 	DMA_InitStruct.DMA_BufferSize= SendBuff_Size;//������Ŀ������ĳ��ȣ�
 	
 	//ֻ��һ���������ݼĴ�������Ҫ������������U8����,����һ�δ���һ���ֽ�
 	DMA_InitStruct.DMA_PeripheralInc=DMA_PeripheralInc_Disable ;//�����ַb����Ҫ����
 	DMA_InitStruct.DMA_PeripheralDataSize= DMA_PeripheralDataSize_Byte  ;//���ݿ��8λ��u8���ͣ�һ���ֽ�
 	
 	//����memory��������һ�����飬����һ���������һ�������Ե�ַ�ǵ�����
 	DMA_InitStruct.DMA_MemoryInc=DMA_MemoryInc_Enable ;//�ڴ��ַ����
 	DMA_InitStruct.DMA_MemoryDataSize=DMA_MemoryDataSize_Byte  ;//һ���ֽ�
 	
 	
 	//5������ģʽ�����ȼ�
 	DMA_InitStruct.DMA_Mode=DMA_Mode_Normal ;//����ģʽ������ѭ��ģʽ
 	DMA_InitStruct.DMA_Priority=DMA_Priority_High ;//�����������ȼ�
 	DMA_InitStruct.DMA_M2M=DMA_M2M_Disable  ;//��ʹ��M-Mģʽ
 	DMA_Init(DMA1_Channel4, &DMA_InitStruct);//����1TX��DMAͨ��4
 	//
 	
 	DMA_ClearFlag(DMA1_FLAG_TC4);//�Ƚ������־λ���
 	//6��ʹ��DMA
 	DMA_Cmd(DMA1_Channel4, ENABLE);

}



//�Ƚ�����������
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
�������ƣ�DMA_Text(void)
�������ܣ�DMA���Ժ���
��ڲ�������
���ز�������
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


//��䷢������	
void USART_DMA_Text(void)
{
	u16 i;
 	for(i=0;i<SendBuff_Size;i++)//�������������
 	{
 		SendBuffe[i]='b';
 	}
}


