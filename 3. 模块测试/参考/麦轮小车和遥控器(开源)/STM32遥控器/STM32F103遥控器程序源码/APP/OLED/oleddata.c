#include "oleddata.h"
#include "bmp.h"


u8 mode=1;//菜单模式选择
u8 mode_flag=1;//菜单模式选择标志位
u8 YK_Stop[10]="L16R17";//遥控模式退出命令
u8 ZL_Stop[10]="MPU10";//重力模式退出命令
u8 Eva_Stop[4]="STOP";//避障模式退出命令
u8 RGB_Stop[4]="STOP";//彩灯模式退出命令
u8 Con_TxBuf[4]="0";//模式退出命令存储区

u8 RGB_JT=0;//彩灯模式箭头指示器位置
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
函数名称：OLED_Progress(void)
函数功能：进度条显示函数
入口参数：无
返回参数：无
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
函数名称：OLED_Face(void)
函数功能：机器人表情显示函数
入口参数：无
返回参数：无
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
函数名称：OLED_ShowMove_Icon(u8 mode,u8 Dir)
函数功能：OLEDg滚动显示函数
入口参数：mode:1 蓝牙 2 遥控 3 重力  Dir:left right
返回参数：无
***************************************************/
void OLED_ShowMove_Icon(u8 mode,u8 Dir)
{
	OLED_WR_Byte(0x2E,OLED_CMD);        //关闭滚动
	OLED_WR_Byte(Dir,OLED_CMD);        //水平向左或者右滚动 26/27
	OLED_WR_Byte(0x00,OLED_CMD);        //虚拟字节
	OLED_WR_Byte(0x00,OLED_CMD);        //起始页 0
	OLED_WR_Byte(0x07,OLED_CMD);        //滚动时间间隔
	OLED_WR_Byte(0x07,OLED_CMD);        //终止页 7
	OLED_WR_Byte(0x00,OLED_CMD);        //虚拟字节
	OLED_WR_Byte(0xFF,OLED_CMD);        //虚拟字节
//	if(mode==0)OLED_ShowPicture(0,0,128,64,BMP_Ly,1);
//	if(mode==1)OLED_ShowPicture(0,0,128,64,BMP_Yk,1);
//	if(mode==2)OLED_ShowPicture(0,0,128,64,BMP_Zl,1);
	if(mode==1)
	{
		OLED_ShowChinese(16,24,0,24,1);//蓝
		OLED_ShowChinese(40,24,1,24,1);//牙
		OLED_ShowChinese(64,24,6,24,1);//模
		OLED_ShowChinese(88,24,7,24,1);//式
	}
	if(mode==2)
	{
		OLED_ShowChinese(16,24,2,24,1);//遥
		OLED_ShowChinese(40,24,3,24,1);//控
		OLED_ShowChinese(64,24,6,24,1);//模
		OLED_ShowChinese(88,24,7,24,1);//式
	}
	if(mode==3)
	{
		OLED_ShowChinese(16,24,4,24,1);//重
		OLED_ShowChinese(40,24,5,24,1);//力
		OLED_ShowChinese(64,24,6,24,1);//模
		OLED_ShowChinese(88,24,7,24,1);//式
	}
	OLED_Refresh();
	OLED_WR_Byte(0x2F,OLED_CMD);        //开启滚动
	delay_ms(300);
}


/**************************************************
函数名称：OLED_Roll(u8 mode,u8 dir)
函数功能：滚动显示函数
入口参数：mode:1 蓝牙 2 遥控 3 重力 Dir:0 left 1 right
返回参数：无
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
				OLED_ShowChinese(16-i,24,0,24,1);//蓝
				OLED_ShowChinese(40-i,24,1,24,1);//牙
				OLED_ShowChinese(64-i,24,6,24,1);//模
				OLED_ShowChinese(88-i,24,7,24,1);//式
				OLED_Refresh();
			}
			else if(mode==2)
			{
				OLED_ShowChinese(16-i,24,2,24,1);//遥
				OLED_ShowChinese(40-i,24,3,24,1);//控
				OLED_ShowChinese(64-i,24,6,24,1);//模
				OLED_ShowChinese(88-i,24,7,24,1);//式
				OLED_Refresh();
			}
			else if(mode==3)
			{
				OLED_ShowChinese(16-i,24,4,24,1);//重
				OLED_ShowChinese(40-i,24,5,24,1);//力
				OLED_ShowChinese(64-i,24,6,24,1);//模
				OLED_ShowChinese(88-i,24,7,24,1);//式
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
				OLED_ShowChinese(16+i,24,0,24,1);//蓝
				OLED_ShowChinese(40+i,24,1,24,1);//牙
				OLED_ShowChinese(64+i,24,6,24,1);//模
				OLED_ShowChinese(88+i,24,7,24,1);//式
				OLED_Refresh();
			}
			else if(mode==2)
			{
				OLED_ShowChinese(16+i,24,2,24,1);//遥
				OLED_ShowChinese(40+i,24,3,24,1);//控
				OLED_ShowChinese(64+i,24,6,24,1);//模
				OLED_ShowChinese(88+i,24,7,24,1);//式
				OLED_Refresh();
			}
			else if(mode==3)
			{
				OLED_ShowChinese(16+i,24,4,24,1);//重
				OLED_ShowChinese(40+i,24,5,24,1);//力
				OLED_ShowChinese(64+i,24,6,24,1);//模
				OLED_ShowChinese(88+i,24,7,24,1);//式
				OLED_Refresh();
			}
		}
	}	
}


/**************************************************
函数名称：OLED_Mode(u8 mode)
函数功能：OLED模式显示函数
入口参数：mode:1 蓝牙 2 遥控 3 重力 4 避障 5 跟随 6 循迹 7 贪吃蛇
返回参数：无
***************************************************/
void OLED_Mode(u8 mode)
{
	if(mode==1)
	{
		OLED_ShowChinese(32,48,24,16,1);//空
		OLED_ShowChinese(56,48,24,16,1);//空
		OLED_ShowChinese(80,48,24,16,1);//空
		
		OLED_ShowPicture(40,0,48,48,BMP_LY,1);
		OLED_ShowChinese(20,48,0,16,1);//蓝
		OLED_ShowChinese(44,48,1,16,1);//牙
		OLED_ShowChinese(68,48,6,16,1);//模
		OLED_ShowChinese(92,48,7,16,1);//式
	}
	else if(mode==2)
	{
		OLED_ShowChinese(32,48,24,16,1);//空
		OLED_ShowChinese(56,48,24,16,1);//空
		OLED_ShowChinese(80,48,24,16,1);//空
		
		OLED_ShowPicture(40,0,48,48,BMP_YK,1);
		OLED_ShowChinese(20,48,2,16,1);//遥
		OLED_ShowChinese(44,48,3,16,1);//控
		OLED_ShowChinese(68,48,6,16,1);//模
		OLED_ShowChinese(92,48,7,16,1);//式
	}
	else if(mode==3)
	{
		OLED_ShowChinese(32,48,24,16,1);//空
		OLED_ShowChinese(56,48,24,16,1);//空
		OLED_ShowChinese(80,48,24,16,1);//空
		
		OLED_ShowPicture(40,0,48,48,BMP_ZL,1);
		OLED_ShowChinese(20,48,4,16,1);//重
		OLED_ShowChinese(44,48,5,16,1);//力
		OLED_ShowChinese(68,48,6,16,1);//模
		OLED_ShowChinese(92,48,7,16,1);//式
	}
	else if(mode==4)
	{
		OLED_ShowChinese(32,48,24,16,1);//空
		OLED_ShowChinese(56,48,24,16,1);//空
		OLED_ShowChinese(80,48,24,16,1);//空
		
		OLED_ShowPicture(40,0,48,48,BMP_BZ,1);
		OLED_ShowChinese(20,48,10,16,1);//避
		OLED_ShowChinese(44,48,11,16,1);//障
		OLED_ShowChinese(68,48,6,16,1); //模
		OLED_ShowChinese(92,48,7,16,1); //式
	}
	else if(mode==5)
	{
		OLED_ShowChinese(32,48,24,16,1);//空
		OLED_ShowChinese(56,48,24,16,1);//空
		OLED_ShowChinese(80,48,24,16,1);//空
		
		OLED_ShowPicture(40,0,48,48,BMP_GS,1);
		OLED_ShowChinese(20,48,8,16,1);//跟
		OLED_ShowChinese(44,48,9,16,1);//随
		OLED_ShowChinese(68,48,6,16,1);//模
		OLED_ShowChinese(92,48,7,16,1);//式
	}
	else if(mode==6)
	{
		OLED_ShowChinese(32,48,24,16,1);//空
		OLED_ShowChinese(56,48,24,16,1);//空
		OLED_ShowChinese(80,48,24,16,1);//空
		
		OLED_ShowPicture(40,0,48,48,BMP_CD,1);
		OLED_ShowChinese(20,48,25,16,1);//循
		OLED_ShowChinese(44,48,26,16,1);//迹
		OLED_ShowChinese(68,48,6,16,1); //模
		OLED_ShowChinese(92,48,7,16,1); //式
	}
	else if(mode==7)
	{
		OLED_ShowChinese(20,48,24,16,1);//空
		OLED_ShowChinese(44,48,24,16,1);//空
		OLED_ShowChinese(68,48,24,16,1); //空
		OLED_ShowChinese(92,48,24,16,1); //空
		
		OLED_ShowPicture(40,0,48,48,BMP_TCS,1);
		OLED_ShowChinese(32,48,21,16,1);//贪
		OLED_ShowChinese(56,48,22,16,1);//吃
		OLED_ShowChinese(80,48,23,16,1);//蛇
	}
	OLED_Refresh();
}

/**************************************************
函数名称：OLED_Mode(u8 mode)
函数功能：遥控器控制显示函数
入口参数：无
返回参数：无
***************************************************/
void OLED_Control(void)
{
	JoyStick_Data();//获取遥感数据
	OLED_Mode(mode);//OLED屏幕菜单模式显示
	OLED_Refresh();
	if(L_Dir==L_RIGHT)//左边摇杆往右打进行模式显示切换
	{
		mode++;
		OLED_Refresh();
		OLED_Mode(mode);
		while(1)//此处是防止摇杆未复位导致连续切换屏幕显示
		{
			JoyStick_Data();
			if(L_Dir==L_STOP&&R_Dir==R_STOP)//摇杆处于中间
			{
				break;
			}
		}
	}
	if(L_Dir==L_LEFT)//左边摇杆往左打进行模式显示切换
	{
		mode--;
		OLED_Refresh();
		OLED_Mode(mode);
		while(1)//此处是防止摇杆未复位导致连续切换屏幕显示
		{
			JoyStick_Data();
			if(L_Dir==L_STOP&&R_Dir==R_STOP)//摇杆处于中间
			{
				break;
			}
		}
	}
	
	
	//屏幕模式显示限位
	if(mode>7)mode=1;
	if(mode<1)mode=7;
	
	if(mode==LY_Mode)//蓝牙模式
	{
		if(R_Dir==R_RIGHT)//右边摇杆右打
		{
			//显示汉字 "请连接手机"
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
				NRF24L01_TX_Mode();//NRF发送模式打开
				Con_TxBuf[0]='L';//蓝牙模式命令
				Con_TxBuf[1]='Y';
				while(NRF24L01_TxPacket(Con_TxBuf)==TX_OK);//等待接收端接收
				if(R_Dir==R_LEFT)//右摇杆同时向左打，则退出当前模式
				{
				  OLED_Clear();
					break;
				}
			}
		}
	}
	else if(mode==YK_Mode)//遥控模式
	{
		if(R_Dir==R_RIGHT)//右边摇杆右打
		{
			OLED_Clear();
			OLED_Refresh();
			
			while(1)
			{
				JoyStick_Data();
				JoyStick_DirSend();
				MPU_Data();//陀螺仪数据处理和发送
				printf("pitch=%.2f\n",pitch);
				if(pitch>=50)//遥控器前倾大于50度，则退出当前模式
				{
					pitch=0;
					NRF24L01_TX_Mode();
					while(NRF24L01_TxPacket(YK_Stop)==TX_OK);//等待发送停止指令
				  OLED_Clear();
					break;
				}
				pitch=0;
			}
			pitch=0;
		}
	}
	else if(mode==ZL_Mode)//重力感应模式
	{
		if(R_Dir==R_RIGHT)//右边摇杆右打
		{
			OLED_Clear();
			OLED_Refresh();

			while(1)
			{
				JoyStick_Data();//摇杆数据处理
				MPU_Direction();//陀螺仪数据处理和发送
				if(R_Dir==R_LEFT)//右摇杆同时向左打，则退出当前模式
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
		if(R_Dir==R_RIGHT)//右边摇杆右打
		{
			OLED_Clear();
			//显示 "避障ing"
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
				Con_TxBuf[0]='B';//避障模式命令
				Con_TxBuf[1]='Z';
				while(NRF24L01_TxPacket(Con_TxBuf)==TX_OK);
				if(R_Dir==R_LEFT)//右摇杆同时向左打，则退出当前模式
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
		if(R_Dir==R_RIGHT)//右边摇杆右打
		{
			OLED_Clear();
			//显示 "跟随ing"
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
				Con_TxBuf[0]='G';//跟随模式命令
				Con_TxBuf[1]='S';
				while(NRF24L01_TxPacket(Con_TxBuf)==TX_OK);
				if(R_Dir==R_LEFT)//右摇杆同时向左打，则退出当前模式
				{
				  OLED_Clear();
					break;
				}
			}
		}
	}
	
	else if(mode==RGB_Mode)
	{
		if(R_Dir==R_RIGHT)//右边摇杆右打
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
				Con_TxBuf[0]='C';//彩灯模式命令
				Con_TxBuf[1]='D';
				if(RGB_JT==0)Con_TxBuf[2]='0';
				else if(RGB_JT==1)Con_TxBuf[2]='1';
				else if(RGB_JT==2)Con_TxBuf[2]='2';
				while(NRF24L01_TxPacket(Con_TxBuf)==TX_OK);
				if(R_Dir==R_DOWN)//右摇杆同时向下打，则箭头向下移动
				{
					RGB_JT++;
					
					if(RGB_JT==3)RGB_JT=0;//限位
					while(1)//此处是防止摇杆未复位导致连续切换屏幕显示
					{
						JoyStick_Data();
						if(L_Dir==L_STOP&&R_Dir==R_STOP)//摇杆处于中间
						{
							break;
						}
					}
				}
				if(R_Dir==R_LEFT)//右摇杆同时向左打，则退出当前模式
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
		if(R_Dir==R_RIGHT)//右边摇杆右打
		{
			TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//使能计数器中断
			GUI_Init();//界面初始化
			while(1)
			{
				Get_Command();
				Show_Score();
				GUI_Refresh(map);//刷新地图
				if(R_Dir==R_LEFT)//右摇杆同时向左打，则退出当前模式
				{
					TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);//失能计数器中断
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
						if(R_Dir==R_LEFT)//右摇杆同时向左打，则退出当前模式
						{
							TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);//失能计数器中断
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




