#include "oleddata.h"
#include "bmp.h"


u8 mode=1;//�˵�ģʽѡ��
u8 mode_flag=1;//�˵�ģʽѡ���־λ
u8 YK_Stop[10]="L16R17";//ң��ģʽ�˳�����
u8 ZL_Stop[10]="MPU10";//����ģʽ�˳�����
u8 Eva_Stop[4]="STOP";//����ģʽ�˳�����
u8 RGB_Stop[4]="STOP";//�ʵ�ģʽ�˳�����
u8 Con_TxBuf[4]="0";//ģʽ�˳�����洢��

u8 RGB_JT=0;//�ʵ�ģʽ��ͷָʾ��λ��
#define left  0x27
#define right 0x26


void OLED_Battery(void)
{
	float val;
	val=Get_ADC(ADC_Channel_4);
	printf("val=%.2f\n",val);
	delay_ms(1000);
}

/**************************************************
�������ƣ�OLED_Progress(void)
�������ܣ���������ʾ����
��ڲ�������
���ز�������
***************************************************/
void OLED_Progress(void)
{
	int i;
	OLED_ShowString(16,48,"Loading",16,1);
	OLED_DrawLine(8,16,125,16,1);
	OLED_DrawLine(8,16,8,32,1);
	OLED_DrawLine(8,32,125,32,1);
	OLED_DrawLine(125,16,125,32,1);
	for(i=8;i<125;i++)
	{
		OLED_DrawLine(i,16,i,32,1);	
		if(i%4==0)OLED_ShowChar(80,48,'.',16,1);
		if(i%8==0)OLED_ShowChar(90,48,'.',16,1);
		if(i%16==0)OLED_ShowChar(100,48,'.',16,1);
		if(i%32==0)OLED_ShowString(80,48,"       ",16,1);	
		OLED_Refresh();
	};
	OLED_Refresh();
}

/**************************************************
�������ƣ�OLED_Face(void)
�������ܣ������˱�����ʾ����
��ڲ�������
���ز�������
***************************************************/
void OLED_Face(void)
{
		OLED_ShowPicture(0,0,128,64,BMP1,0);
		OLED_Refresh();
		delay_ms(1000);
		OLED_ShowPicture(0,0,128,64,BMP2,0);
		OLED_Refresh();
		delay_ms(100);
		OLED_ShowPicture(0,0,128,64,BMP3,0);
		OLED_Refresh();
		delay_ms(100);
		OLED_ShowPicture(0,0,128,64,BMP4,0);
		OLED_Refresh();
		delay_ms(100);
		OLED_ShowPicture(0,0,128,64,BMP5,0);
		OLED_Refresh();
		delay_ms(100);
		OLED_ShowPicture(0,0,128,64,BMP6,0);
		OLED_Refresh();
		delay_ms(100);
		OLED_ShowPicture(0,0,128,64,BMP7,0);
		OLED_Refresh();
		delay_ms(100);
		OLED_ShowPicture(0,0,128,64,BMP8,0);
		OLED_Refresh();
		delay_ms(500);  
}

/**************************************************
�������ƣ�OLED_ShowMove_Icon(u8 mode,u8 Dir)
�������ܣ�OLEDg������ʾ����
��ڲ�����mode:1 ���� 2 ң�� 3 ����  Dir:left right
���ز�������
***************************************************/
void OLED_ShowMove_Icon(u8 mode,u8 Dir)
{
	OLED_WR_Byte(0x2E,OLED_CMD);        //�رչ���
	OLED_WR_Byte(Dir,OLED_CMD);        //ˮƽ��������ҹ��� 26/27
	OLED_WR_Byte(0x00,OLED_CMD);        //�����ֽ�
	OLED_WR_Byte(0x00,OLED_CMD);        //��ʼҳ 0
	OLED_WR_Byte(0x07,OLED_CMD);        //����ʱ����
	OLED_WR_Byte(0x07,OLED_CMD);        //��ֹҳ 7
	OLED_WR_Byte(0x00,OLED_CMD);        //�����ֽ�
	OLED_WR_Byte(0xFF,OLED_CMD);        //�����ֽ�
//	if(mode==0)OLED_ShowPicture(0,0,128,64,BMP_Ly,1);
//	if(mode==1)OLED_ShowPicture(0,0,128,64,BMP_Yk,1);
//	if(mode==2)OLED_ShowPicture(0,0,128,64,BMP_Zl,1);
	if(mode==1)
	{
		OLED_ShowChinese(16,24,0,24,1);//��
		OLED_ShowChinese(40,24,1,24,1);//��
		OLED_ShowChinese(64,24,6,24,1);//ģ
		OLED_ShowChinese(88,24,7,24,1);//ʽ
	}
	if(mode==2)
	{
		OLED_ShowChinese(16,24,2,24,1);//ң
		OLED_ShowChinese(40,24,3,24,1);//��
		OLED_ShowChinese(64,24,6,24,1);//ģ
		OLED_ShowChinese(88,24,7,24,1);//ʽ
	}
	if(mode==3)
	{
		OLED_ShowChinese(16,24,4,24,1);//��
		OLED_ShowChinese(40,24,5,24,1);//��
		OLED_ShowChinese(64,24,6,24,1);//ģ
		OLED_ShowChinese(88,24,7,24,1);//ʽ
	}
	OLED_Refresh();
	OLED_WR_Byte(0x2F,OLED_CMD);        //��������
	delay_ms(300);
}


/**************************************************
�������ƣ�OLED_Roll(u8 mode,u8 dir)
�������ܣ�������ʾ����
��ڲ�����mode:1 ���� 2 ң�� 3 ���� Dir:0 left 1 right
���ز�������
***************************************************/
void OLED_Roll(u8 mode,u8 dir)
{
	int i;
	if(dir==0)
	{
		for(i=0;i<=16;i++)
		{
			if(mode==1)
			{
				OLED_ShowChinese(16-i,24,0,24,1);//��
				OLED_ShowChinese(40-i,24,1,24,1);//��
				OLED_ShowChinese(64-i,24,6,24,1);//ģ
				OLED_ShowChinese(88-i,24,7,24,1);//ʽ
				OLED_Refresh();
			}
			else if(mode==2)
			{
				OLED_ShowChinese(16-i,24,2,24,1);//ң
				OLED_ShowChinese(40-i,24,3,24,1);//��
				OLED_ShowChinese(64-i,24,6,24,1);//ģ
				OLED_ShowChinese(88-i,24,7,24,1);//ʽ
				OLED_Refresh();
			}
			else if(mode==3)
			{
				OLED_ShowChinese(16-i,24,4,24,1);//��
				OLED_ShowChinese(40-i,24,5,24,1);//��
				OLED_ShowChinese(64-i,24,6,24,1);//ģ
				OLED_ShowChinese(88-i,24,7,24,1);//ʽ
				OLED_Refresh();
			}
		}
	}
	
	else if(dir==1)
	{
		for(i=0;i<=16;i++)
		{
			if(mode==1)
			{
				OLED_ShowChinese(16+i,24,0,24,1);//��
				OLED_ShowChinese(40+i,24,1,24,1);//��
				OLED_ShowChinese(64+i,24,6,24,1);//ģ
				OLED_ShowChinese(88+i,24,7,24,1);//ʽ
				OLED_Refresh();
			}
			else if(mode==2)
			{
				OLED_ShowChinese(16+i,24,2,24,1);//ң
				OLED_ShowChinese(40+i,24,3,24,1);//��
				OLED_ShowChinese(64+i,24,6,24,1);//ģ
				OLED_ShowChinese(88+i,24,7,24,1);//ʽ
				OLED_Refresh();
			}
			else if(mode==3)
			{
				OLED_ShowChinese(16+i,24,4,24,1);//��
				OLED_ShowChinese(40+i,24,5,24,1);//��
				OLED_ShowChinese(64+i,24,6,24,1);//ģ
				OLED_ShowChinese(88+i,24,7,24,1);//ʽ
				OLED_Refresh();
			}
		}
	}	
}


/**************************************************
�������ƣ�OLED_Mode(u8 mode)
�������ܣ�OLEDģʽ��ʾ����
��ڲ�����mode:1 ���� 2 ң�� 3 ���� 4 ���� 5 ���� 6 ѭ�� 7 ̰����
���ز�������
***************************************************/
void OLED_Mode(u8 mode)
{
	if(mode==1)
	{
		OLED_ShowChinese(32,48,24,16,1);//��
		OLED_ShowChinese(56,48,24,16,1);//��
		OLED_ShowChinese(80,48,24,16,1);//��
		
		OLED_ShowPicture(40,0,48,48,BMP_LY,1);
		OLED_ShowChinese(20,48,0,16,1);//��
		OLED_ShowChinese(44,48,1,16,1);//��
		OLED_ShowChinese(68,48,6,16,1);//ģ
		OLED_ShowChinese(92,48,7,16,1);//ʽ
	}
	else if(mode==2)
	{
		OLED_ShowChinese(32,48,24,16,1);//��
		OLED_ShowChinese(56,48,24,16,1);//��
		OLED_ShowChinese(80,48,24,16,1);//��
		
		OLED_ShowPicture(40,0,48,48,BMP_YK,1);
		OLED_ShowChinese(20,48,2,16,1);//ң
		OLED_ShowChinese(44,48,3,16,1);//��
		OLED_ShowChinese(68,48,6,16,1);//ģ
		OLED_ShowChinese(92,48,7,16,1);//ʽ
	}
	else if(mode==3)
	{
		OLED_ShowChinese(32,48,24,16,1);//��
		OLED_ShowChinese(56,48,24,16,1);//��
		OLED_ShowChinese(80,48,24,16,1);//��
		
		OLED_ShowPicture(40,0,48,48,BMP_ZL,1);
		OLED_ShowChinese(20,48,4,16,1);//��
		OLED_ShowChinese(44,48,5,16,1);//��
		OLED_ShowChinese(68,48,6,16,1);//ģ
		OLED_ShowChinese(92,48,7,16,1);//ʽ
	}
	else if(mode==4)
	{
		OLED_ShowChinese(32,48,24,16,1);//��
		OLED_ShowChinese(56,48,24,16,1);//��
		OLED_ShowChinese(80,48,24,16,1);//��
		
		OLED_ShowPicture(40,0,48,48,BMP_BZ,1);
		OLED_ShowChinese(20,48,10,16,1);//��
		OLED_ShowChinese(44,48,11,16,1);//��
		OLED_ShowChinese(68,48,6,16,1); //ģ
		OLED_ShowChinese(92,48,7,16,1); //ʽ
	}
	else if(mode==5)
	{
		OLED_ShowChinese(32,48,24,16,1);//��
		OLED_ShowChinese(56,48,24,16,1);//��
		OLED_ShowChinese(80,48,24,16,1);//��
		
		OLED_ShowPicture(40,0,48,48,BMP_GS,1);
		OLED_ShowChinese(20,48,8,16,1);//��
		OLED_ShowChinese(44,48,9,16,1);//��
		OLED_ShowChinese(68,48,6,16,1);//ģ
		OLED_ShowChinese(92,48,7,16,1);//ʽ
	}
	else if(mode==6)
	{
		OLED_ShowChinese(32,48,24,16,1);//��
		OLED_ShowChinese(56,48,24,16,1);//��
		OLED_ShowChinese(80,48,24,16,1);//��
		
		OLED_ShowPicture(40,0,48,48,BMP_CD,1);
		OLED_ShowChinese(20,48,25,16,1);//ѭ
		OLED_ShowChinese(44,48,26,16,1);//��
		OLED_ShowChinese(68,48,6,16,1); //ģ
		OLED_ShowChinese(92,48,7,16,1); //ʽ
	}
	else if(mode==7)
	{
		OLED_ShowChinese(20,48,24,16,1);//��
		OLED_ShowChinese(44,48,24,16,1);//��
		OLED_ShowChinese(68,48,24,16,1); //��
		OLED_ShowChinese(92,48,24,16,1); //��
		
		OLED_ShowPicture(40,0,48,48,BMP_TCS,1);
		OLED_ShowChinese(32,48,21,16,1);//̰
		OLED_ShowChinese(56,48,22,16,1);//��
		OLED_ShowChinese(80,48,23,16,1);//��
	}
	OLED_Refresh();
}

/**************************************************
�������ƣ�OLED_Mode(u8 mode)
�������ܣ�ң����������ʾ����
��ڲ�������
���ز�������
***************************************************/
void OLED_Control(void)
{
	JoyStick_Data();//��ȡң������
	OLED_Mode(mode);//OLED��Ļ�˵�ģʽ��ʾ
	OLED_Refresh();
	if(L_Dir==L_RIGHT)//���ҡ�����Ҵ����ģʽ��ʾ�л�
	{
		mode++;
		OLED_Refresh();
		OLED_Mode(mode);
		while(1)//�˴��Ƿ�ֹҡ��δ��λ���������л���Ļ��ʾ
		{
			JoyStick_Data();
			if(L_Dir==L_STOP&&R_Dir==R_STOP)//ҡ�˴����м�
			{
				break;
			}
		}
	}
	if(L_Dir==L_LEFT)//���ҡ����������ģʽ��ʾ�л�
	{
		mode--;
		OLED_Refresh();
		OLED_Mode(mode);
		while(1)//�˴��Ƿ�ֹҡ��δ��λ���������л���Ļ��ʾ
		{
			JoyStick_Data();
			if(L_Dir==L_STOP&&R_Dir==R_STOP)//ҡ�˴����м�
			{
				break;
			}
		}
	}
	
	
	//��Ļģʽ��ʾ��λ
	if(mode>7)mode=1;
	if(mode<1)mode=7;
	
	if(mode==LY_Mode)//����ģʽ
	{
		if(R_Dir==R_RIGHT)//�ұ�ҡ���Ҵ�
		{
			//��ʾ���� "�������ֻ�"
			OLED_Clear();
			OLED_ShowChinese(24,24,14,16,1);
			OLED_ShowChinese(40,24,15,16,1);
			OLED_ShowChinese(56,24,16,16,1);
			OLED_ShowChinese(72,24,17,16,1);
			OLED_ShowChinese(88,24,18,16,1);
			OLED_Refresh();
			while(1)
			{
				JoyStick_Data();
				NRF24L01_TX_Mode();//NRF����ģʽ��
				Con_TxBuf[0]='L';//����ģʽ����
				Con_TxBuf[1]='Y';
				while(NRF24L01_TxPacket(Con_TxBuf)==TX_OK);//�ȴ����ն˽���
				if(R_Dir==R_LEFT)//��ҡ��ͬʱ��������˳���ǰģʽ
				{
				  OLED_Clear();
					break;
				}
			}
		}
	}
	else if(mode==YK_Mode)//ң��ģʽ
	{
		if(R_Dir==R_RIGHT)//�ұ�ҡ���Ҵ�
		{
			OLED_Clear();
			OLED_Refresh();
			
			while(1)
			{
				JoyStick_Data();
				JoyStick_DirSend();
				MPU_Data();//���������ݴ���ͷ���
				printf("pitch=%.2f\n",pitch);
				if(pitch>=50)//ң����ǰ�����50�ȣ����˳���ǰģʽ
				{
					pitch=0;
					NRF24L01_TX_Mode();
					while(NRF24L01_TxPacket(YK_Stop)==TX_OK);//�ȴ�����ָֹͣ��
				  OLED_Clear();
					break;
				}
				pitch=0;
			}
			pitch=0;
		}
	}
	else if(mode==ZL_Mode)//������Ӧģʽ
	{
		if(R_Dir==R_RIGHT)//�ұ�ҡ���Ҵ�
		{
			OLED_Clear();
			OLED_Refresh();

			while(1)
			{
				JoyStick_Data();//ҡ�����ݴ���
				MPU_Direction();//���������ݴ���ͷ���
				if(R_Dir==R_LEFT)//��ҡ��ͬʱ��������˳���ǰģʽ
				{
					NRF24L01_TX_Mode();
					while(NRF24L01_TxPacket(ZL_Stop)==TX_OK);
				  OLED_Clear();
					break;
				}
			}
		}
	}
	
	else if(mode==BZ_Mode)
	{
		if(R_Dir==R_RIGHT)//�ұ�ҡ���Ҵ�
		{
			OLED_Clear();
			//��ʾ "����ing"
			OLED_ShowChinese(40,24,10,16,1);
			OLED_ShowChinese(56,24,11,16,1);
			OLED_ShowString(72,24,"ing",16,1);
			
			OLED_DrawLine(20,16,108,16,1);
			OLED_DrawLine(20,16,20,48,1);
			OLED_DrawLine(20,48,108,48,1);
			OLED_DrawLine(108,16,108,48,1);
			OLED_Refresh();
			
			while(1)
			{
				JoyStick_Data();
				NRF24L01_TX_Mode();
				Con_TxBuf[0]='B';//����ģʽ����
				Con_TxBuf[1]='Z';
				while(NRF24L01_TxPacket(Con_TxBuf)==TX_OK);
				if(R_Dir==R_LEFT)//��ҡ��ͬʱ��������˳���ǰģʽ
				{
					NRF24L01_TX_Mode();
					while(NRF24L01_TxPacket(Eva_Stop)==TX_OK);
				  OLED_Clear();
					break;
				}
			}
		}
	}
	
	else if(mode==GS_Mode)
	{
		if(R_Dir==R_RIGHT)//�ұ�ҡ���Ҵ�
		{
			OLED_Clear();
			//��ʾ "����ing"
			OLED_ShowChinese(40,24,8,16,1);
			OLED_ShowChinese(56,24,9,16,1);
			OLED_ShowString(72,24,"ing",16,1);
			
			OLED_DrawLine(20,16,108,16,1);
			OLED_DrawLine(20,16,20,48,1);
			OLED_DrawLine(20,48,108,48,1);
			OLED_DrawLine(108,16,108,48,1);
			OLED_Refresh();
			
			while(1)
			{
				JoyStick_Data();
				NRF24L01_TX_Mode();
				Con_TxBuf[0]='G';//����ģʽ����
				Con_TxBuf[1]='S';
				while(NRF24L01_TxPacket(Con_TxBuf)==TX_OK);
				if(R_Dir==R_LEFT)//��ҡ��ͬʱ��������˳���ǰģʽ
				{
				  OLED_Clear();
					break;
				}
			}
		}
	}
	
	else if(mode==RGB_Mode)
	{
		if(R_Dir==R_RIGHT)//�ұ�ҡ���Ҵ�
		{
			OLED_Clear();
			
			while(1)
			{
				if(RGB_JT==0)
				{
					OLED_ShowPicture(10,8,16,16,BMP_JT,1);
					OLED_ShowPicture(10,(8+16),16,16,BMP_KB,1);
					OLED_ShowPicture(10,(8+32),16,16,BMP_KB,1);
					OLED_ShowPicture(10,(8+16),16,16,BMP_YQ,1);
					OLED_ShowPicture(10,(8+32),16,16,BMP_YQ,1);
				}
				else if(RGB_JT==1)
				{
					OLED_ShowPicture(10,8,16,16,BMP_KB,1);
					OLED_ShowPicture(10,8,16,16,BMP_YQ,1);
					OLED_ShowPicture(10,(8+16),16,16,BMP_JT,1);
					OLED_ShowPicture(10,(8+32),16,16,BMP_KB,1);
					OLED_ShowPicture(10,(8+32),16,16,BMP_YQ,1);
				}
				else if(RGB_JT==2)
				{
					OLED_ShowPicture(10,8,16,16,BMP_KB,1);
					OLED_ShowPicture(10,(8+16),16,16,BMP_KB,1);
					OLED_ShowPicture(10,8,16,16,BMP_YQ,1);
					OLED_ShowPicture(10,(8+16),16,16,BMP_YQ,1);
					OLED_ShowPicture(10,(8+32),16,16,BMP_JT,1);
				}
				
				OLED_ShowString(32,8,"1.",16,1);
				OLED_ShowChinese(48,8,27,16,1);
				OLED_ShowChinese((48+16),8,28,16,1);
				OLED_ShowChinese((48+32),8,26,16,1);
				
				OLED_ShowString(32,(8+16),"2.",16,1);
				OLED_ShowChinese(48,(8+16),29,16,1);
				OLED_ShowChinese((48+16),(8+16),30,16,1);
				OLED_ShowChinese((48+32),(8+16),26,16,1);
				
				OLED_ShowString(32,(8+32),"3.",16,1);
				OLED_ShowChinese(48,(8+32),31,16,1);
				OLED_ShowChinese((48+16),(8+32),32,16,1);
				OLED_ShowChinese((48+32),(8+32),26,16,1);
				
				OLED_Refresh();
				
				JoyStick_Data();
				NRF24L01_TX_Mode();
				Con_TxBuf[0]='C';//�ʵ�ģʽ����
				Con_TxBuf[1]='D';
				if(RGB_JT==0)Con_TxBuf[2]='0';
				else if(RGB_JT==1)Con_TxBuf[2]='1';
				else if(RGB_JT==2)Con_TxBuf[2]='2';
				while(NRF24L01_TxPacket(Con_TxBuf)==TX_OK);
				if(R_Dir==R_DOWN)//��ҡ��ͬʱ���´����ͷ�����ƶ�
				{
					RGB_JT++;
					
					if(RGB_JT==3)RGB_JT=0;//��λ
					while(1)//�˴��Ƿ�ֹҡ��δ��λ���������л���Ļ��ʾ
					{
						JoyStick_Data();
						if(L_Dir==L_STOP&&R_Dir==R_STOP)//ҡ�˴����м�
						{
							break;
						}
					}
				}
				if(R_Dir==R_LEFT)//��ҡ��ͬʱ��������˳���ǰģʽ
				{
					NRF24L01_TX_Mode();
					while(NRF24L01_TxPacket(RGB_Stop)==TX_OK);
				  OLED_Clear();
					break;
				}
			}
		}
	}
	
	else if(mode==TCS_Mode)
	{
		if(R_Dir==R_RIGHT)//�ұ�ҡ���Ҵ�
		{
			TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//ʹ�ܼ������ж�
			GUI_Init();//�����ʼ��
			while(1)
			{
				Get_Command();
				Show_Score();
				GUI_Refresh(map);//ˢ�µ�ͼ
				if(R_Dir==R_LEFT)//��ҡ��ͬʱ��������˳���ǰģʽ
				{
					TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);//ʧ�ܼ������ж�
					Data_Clear();
					OLED_Clear();
					break;
				}
				if(GameOver())
				{
					OLED_Clear();
					while(1)
					{
						JoyStick_Data();
						OLED_ShowString(30,24,"Game Over !",16,1);
						OLED_Refresh();
						if(R_Dir==R_LEFT)//��ҡ��ͬʱ��������˳���ǰģʽ
						{
							TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);//ʧ�ܼ������ж�
						  Data_Clear();
							OLED_Clear();
							delay_ms(100);
							break;
						}
					}
				}
			}
		}
	}
	
}




