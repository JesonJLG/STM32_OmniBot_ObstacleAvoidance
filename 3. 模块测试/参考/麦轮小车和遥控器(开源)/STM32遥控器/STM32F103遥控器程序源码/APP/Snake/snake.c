#include "snake.h"
#include "oled.h"
#include "stdio.h"
#include "string.h"
#include "JoyStick.h"



int map[32][12]={0};//地图大小  y-x
int snake_Grid[32][2]={0};
int length;
int score;
int direction;//蛇的方向
int key=0;
bool eated=false;
int Ui_Show_Time=1;
extern u8 L_Dir;

//数据初始化
void Data_Clear(void)
{
	memset(map,0,sizeof(map));
	memset(snake_Grid,0,sizeof(snake_Grid));
	direction=1;
}

void GUI_Init()//界面初始化
{
	OLED_Clear();
	Snake_Init();
	Food();
	OLED_Refresh();	
}

void GUI_Refresh(int map[32][12])//界面刷新
{
	int i,j,temp;
	for(i=0;i<32;i++)
	{
		for(j=0;j<12;j++)
		{
		   temp=map[i][j];
		   if(temp==2)
		   {
		       Paint_Body(i,j);  
		   }
		   else if(temp==-2)
		   {
		       Paint_Map(i,j);  
		   }
		   else if(temp==-1)
		   {
		       Paint_Food(i,j);  
		   }
	       else if(temp==0)
		   {
		      Paint_Clean(i,j);  
		   } 		  
		}
	}
	OLED_Refresh();	
}

void Creat_map(int map[32][12])//创建地图
{
	int i,j;
	for(i=0;i<12;i++)
	{
		for(j=0;j<32;j++)
		{
  	   if(i==0||i==11)
		   {
		     map[j][i]=-2;
		   }
		   if(j==0||j==31)
		   {
		     map[j][i]=-2;
		   }
		}
	}
}

void Paint_Map(int x,int y)//绘制地图
{
	int i,j;
	for(i=4*y;i<4*y+4;i++)     
	{
	  for(j=4*x;j<4*x+4;j++)
	  {
		    OLED_DrawPoint(j,i+16,1); 
	  }
	}
}


void Paint_Head(int x,int y )//绘制蛇头
{
	int i,j;
	for(i=4*y;i<4*y+4;i++)     
	{
	  for(j=4*x;j<4*x+4;j++)
	  {
		    OLED_DrawPoint(j,i+16,1); 
	  }
	}
}


void Paint_Food(int x,int y )//绘制食物
{
	int i,j;
	for(i=4*y;i<4*y+4;i++)     
	{
	  for(j=4*x;j<4*x+4;j++)
	  {
		
	    if(i==4*y+1||i==4*y+2)
		  {
			
			  OLED_DrawPoint(j,i+16,1);  
			
		  }
	    if(j==4*x+1||j==4*x+2)
		  {
			
			  OLED_DrawPoint(j,i+16,1);  
		  } 
	  }
	}
}


void Paint_Tail(int x,int y )//绘制尾巴
{
	int i,j;
	for(i=4*y;i<4*y+4;i++)     
	{
	  for(j=4*x;j<4*x+4;j++)
	  {
	    if(i==4*y+1||i==4*y+2)
		  {
			  OLED_DrawPoint(j,i+16,1);  
		  }
	    if(j==4*x+1||j==4*x+2||j==4*x+3)
		  {
			  OLED_DrawPoint(j,i+16,1);  
		  } 
	  }
	}
}

void Paint_Body(int x,int y )//绘制身体
{
	int i,j;
	for(i=4*y;i<4*y+4;i++)     
	{
	  for(j=4*x;j<4*x+4;j++)
	  {
			if(i==4*y||i==4*y+3)
			{	
	  		OLED_DrawPoint(j,i+16,1);  
			}
			if(j==4*x||j==4*x+3)
			{
				OLED_DrawPoint(j,i+16,1);  
			}
	  }
  }
}

void Paint_Clean(int x,int y )//清除点
{
	int i,j;
	for(i=4*y;i<4*y+4;i++)     
	{
	  for(j=4*x;j<4*x+4;j++)
	  {
			OLED_ClearPoint(j,i+16);  
	  }
	}
}

void Snake_Init()//蛇及食物初始化
{  
	 int i;
   length=5;
   score=0;
   snake_Grid[0][0]=7;//x坐标,蛇头坐标
   snake_Grid[0][1]=5;//y坐标
   for(i=1;i<5;i++)
    {
        snake_Grid[i][0]=snake_Grid[0][0]-i;
        snake_Grid[i][1]=snake_Grid[0][1];  //给刚开始的蛇身几个初始坐标
    }
  	direction=RIGHT;
    Creat_map(map);
	 
}

void Move()//移动
{
  int i;
	map[snake_Grid[length-1][0]][snake_Grid[length-1][1]]=0;//清除尾巴
	if(eated)               //如果吃到了食物
  {
    length++;
    eated=false;        //设置为false，不然无限变长
  }
  for(i=length-1;i>0;i--)    //从尾巴开始，每一个点的位置等于它前面一个点的位置
  {
      snake_Grid[i][0]=snake_Grid[i-1][0];
      snake_Grid[i][1]=snake_Grid[i-1][1];
  }
  switch(direction)
  {
    case UP:
        snake_Grid[0][1]--;
        break;
    case DOWN:
        snake_Grid[0][1]++;
        break;
    case LEFT:
        snake_Grid[0][0]--;
        break;
    case RIGHT:
        snake_Grid[0][0]++;
        break;
  }
	Grid_Bound();//坐标限定
}


void drawSnake()            //画蛇
{
    int i,x,y;
    for(i=0;i<length;i++)
    {
		  x=snake_Grid[i][0];
		  y=snake_Grid[i][1];
      map[x][y]=2;       
    }
}


void Get_Command()//获取键盘值
{
	JoyStick_Data();
	
	if(L_Dir==L_RIGHT&&direction!=LEFT)direction=RIGHT;         //右
  else if(L_Dir==L_LEFT&&direction!=RIGHT)direction=LEFT;        //左
  else if(L_Dir==L_DOWN&&direction!=UP)direction=DOWN;        //下 
	else if(L_Dir==L_UP&&direction!=DOWN)direction=UP;            //上
}


void  Grid_Bound()//坐标限定
{
  if(snake_Grid[0][0]==31)
		snake_Grid[0][0]=1;
  else if(snake_Grid[0][0]==0)
		snake_Grid[0][0]=30;
  else if(snake_Grid[0][1]==11)
		snake_Grid[0][1]=1;
  else if(snake_Grid[0][1]==0)
		snake_Grid[0][1]=10;  
}


bool GameOver()//游戏结束
{
	bool isGameOver=false;
	int sx=snake_Grid[0][0],sy=snake_Grid[0][1],i;//蛇头坐标
	for(i=1; i<length; i++)    //判断有没有吃到自己
    {
        if(snake_Grid[i][0]==sx&&snake_Grid[i][1]==sy)
            isGameOver=true;
    }
    return isGameOver;
}


int Chek(int i,int j)//检查地图空位
{
	if(map[i][j]!=0)
	{
		return 0;
	}
	return 1;   //是空位就返回1
}


void Food()//生成食物
{
	int i,j;
	do
	{
		i=rand()%30;                 //生成0~H-1之间的一个数
        j=rand()%10;
	
	}
	while(Chek(i,j)==0);
	map[i][j]=-1;//画出食物
}


void Eat_Food()//吃食物
{
	if(map[snake_Grid[0][0]][snake_Grid[0][1]]==-1) //如果蛇头碰到食物，就重新投放食物，并且把食物点重置为0
  {
    eated=true;            //标记已经吃到食物
		score+=1;
    Food();
    map[snake_Grid[0][0]][snake_Grid[0][1]]=0;  //去掉食物
  }
}


void Show_Score()//显示分数
{
	OLED_ShowString(30,0,"Score:",16,1);
	OLED_ShowNum(80,0,score,2,16,1);
}
