#include "gap.h"

u8  TIM2CH1_CAPTURE_STA=0;	//���벶��״̬		    				
u16	TIM2CH1_CAPTURE_VAL;	//���벶��ֵ
u32 temp=0; 


/**************************************************
�������ƣ�TIM2_Gap_Init(u32 arr,u32 psc)
�������ܣ���ʱ��2���벶���ʼ������
��ڲ�����arr ��װ��ֵ    psc   Ԥ��Ƶϵ��
���ز�������
***************************************************/
void TIM2_Gap_Init(u32 arr,u32 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_InitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);     //������ʱ��2ʱ��
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			//Ƶ��50ZMHZ
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;				//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);								//��ʼ��GPIOA
	GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);//����
	
	
	TIM_InitStructure.TIM_Period=arr;                    //�Զ���װ�ؼĴ������ڵ�ֵ        
	TIM_InitStructure.TIM_Prescaler=psc;                 //����Ԥ��Ƶֵ 
	TIM_InitStructure.TIM_ClockDivision=TIM_CKD_DIV1;    //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_InitStructure.TIM_CounterMode=TIM_CounterMode_Up;//TIM���ϼ���ģʽ
	TIM_InitStructure.TIM_RepetitionCounter=0;           //�ظ�������ֵ
	
	TIM_TimeBaseInit(TIM2,&TIM_InitStructure);
	TIM_Cmd(TIM2, ENABLE);                               //ʹ�ܼ�����
	
	
	TIM_ICInitStructure.TIM_Channel=TIM_Channel_1;//ѡ������ˣ�IC1ӳ�䵽TI1��
	TIM_ICInitStructure.TIM_ICFilter=0x00;//�����˲���IC1F=0000 ���˲�
	TIM_ICInitStructure.TIM_ICPolarity=TIM_ICPolarity_Rising;//�����ز���
	TIM_ICInitStructure.TIM_ICPrescaler=TIM_ICPSC_DIV1;//���������Ƶ������Ƶ
	TIM_ICInitStructure.TIM_ICSelection=TIM_ICSelection_DirectTI;//ӳ�䵽TI1��
	TIM_ICInit(TIM2,&TIM_ICInitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);

  TIM_ITConfig(TIM2, TIM_IT_Update|TIM_IT_CC1, ENABLE);   //��������жϣ�����CC1TE�����ж�
	TIM_Cmd(TIM2,ENABLE);//ʹ�ܶ�ʱ��2
		
	
}



////��ʱ��2�жϷ������	 
//void TIM2_IRQHandler(void)
//{ 

// 	if((TIM2CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
//	{	  
//		if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
//		 
//		{	    
//			if(TIM2CH1_CAPTURE_STA&0X40)//�Ѿ����񵽸ߵ�ƽ��
//			{
//				if((TIM2CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
//				{
//					TIM2CH1_CAPTURE_STA|=0X80;//��ǳɹ�������һ��
//					TIM2CH1_CAPTURE_VAL=0XFFFF;
//				}else TIM2CH1_CAPTURE_STA++;
//			}	 
//		}
//	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)//����1���������¼�
//		{	
//			if(TIM2CH1_CAPTURE_STA&0X40)		//����һ���½��� 		
//			{	  			
//				TIM2CH1_CAPTURE_STA|=0X80;		//��ǳɹ�����һ�θߵ�ƽ����
//				TIM2CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);
//		   	TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
//			}else  								//��δ��ʼ,��һ�β���������
//			{
//				TIM2CH1_CAPTURE_STA=0;			//���
//				TIM2CH1_CAPTURE_VAL=0;
//	 			TIM_SetCounter(TIM2,0);
//				TIM2CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
//		   	TIM_OC1PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//CC1P=1 ����Ϊ�½��ز���
//			}		    
//		}			     	    					   
// 	} 
//  TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update); //����жϱ�־λ
//}


/**************************************************
�������ƣ�TIM2_Gap_text(void)
�������ܣ���ʱ��2���벶����Ժ���
��ڲ�������
���ز�������
***************************************************/
void TIM2_Gap_text(void)
{
		delay_ms(10);
		TIM_SetCompare2(TIM2,TIM_GetCapture2(TIM2)+1);

		if(TIM_GetCapture2(TIM2)==300)TIM_SetCompare2(TIM2,0);	
		 		 
 		if(TIM2CH1_CAPTURE_STA&0X80)//�ɹ�������һ��������
		{
			temp=TIM2CH1_CAPTURE_STA&0X3F;
			temp*=65536;//���ʱ���ܺ�
			temp+=TIM2CH1_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
			printf("HIGH:%d us\r\n",temp);//��ӡ�ܵĸߵ�ƽʱ��
			TIM2CH1_CAPTURE_STA=0;//������һ�β���
		}
}


