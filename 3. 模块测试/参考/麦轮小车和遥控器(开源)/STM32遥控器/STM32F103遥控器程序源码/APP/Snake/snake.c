#include "snake.h"
#include "oled.h"
#include "stdio.h"
#include "string.h"
#include "JoyStick.h"



int map[32][12]={0};//��ͼ��С  y-x
int snake_Grid[32][2]={0};
int length;
int score;
int direction;//�ߵķ���
int key=0;
bool eated=false;
int Ui_Show_Time=1;
extern u8 L_Dir;

//���ݳ�ʼ��
void Data_Clear(void)
{
	memset(map,0,sizeof(map));
	memset(snake_Grid,0,sizeof(snake_Grid));
	direction=1;
}

void GUI_Init()//�����ʼ��
{
	OLED_Clear();
	Snake_Init();
	Food();
	OLED_Refresh();	
}

void GUI_Refresh(int map[32][12])//����ˢ��
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

void Creat_map(int map[32][12])//������ͼ
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

void Paint_Map(int x,int y)//���Ƶ�ͼ
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


void Paint_Head(int x,int y )//������ͷ
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


void Paint_Food(int x,int y )//����ʳ��
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


void Paint_Tail(int x,int y )//����β��
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

void Paint_Body(int x,int y )//��������
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

void Paint_Clean(int x,int y )//�����
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

void Snake_Init()//�߼�ʳ���ʼ��
{  
	 int i;
   length=5;
   score=0;
   snake_Grid[0][0]=7;//x����,��ͷ����
   snake_Grid[0][1]=5;//y����
   for(i=1;i<5;i++)
    {
        snake_Grid[i][0]=snake_Grid[0][0]-i;
        snake_Grid[i][1]=snake_Grid[0][1];  //���տ�ʼ����������ʼ����
    }
  	direction=RIGHT;
    Creat_map(map);
	 
}

void Move()//�ƶ�
{
  int i;
	map[snake_Grid[length-1][0]][snake_Grid[length-1][1]]=0;//���β��
	if(eated)               //����Ե���ʳ��
  {
    length++;
    eated=false;        //����Ϊfalse����Ȼ���ޱ䳤
  }
  for(i=length-1;i>0;i--)    //��β�Ϳ�ʼ��ÿһ�����λ�õ�����ǰ��һ�����λ��
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
	Grid_Bound();//�����޶�
}


void drawSnake()            //����
{
    int i,x,y;
    for(i=0;i<length;i++)
    {
		  x=snake_Grid[i][0];
		  y=snake_Grid[i][1];
      map[x][y]=2;       
    }
}


void Get_Command()//��ȡ����ֵ
{
	JoyStick_Data();
	
	if(L_Dir==L_RIGHT&&direction!=LEFT)direction=RIGHT;         //��
  else if(L_Dir==L_LEFT&&direction!=RIGHT)direction=LEFT;        //��
  else if(L_Dir==L_DOWN&&direction!=UP)direction=DOWN;        //�� 
	else if(L_Dir==L_UP&&direction!=DOWN)direction=UP;            //��
}


void  Grid_Bound()//�����޶�
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


bool GameOver()//��Ϸ����
{
	bool isGameOver=false;
	int sx=snake_Grid[0][0],sy=snake_Grid[0][1],i;//��ͷ����
	for(i=1; i<length; i++)    //�ж���û�гԵ��Լ�
    {
        if(snake_Grid[i][0]==sx&&snake_Grid[i][1]==sy)
            isGameOver=true;
    }
    return isGameOver;
}


int Chek(int i,int j)//����ͼ��λ
{
	if(map[i][j]!=0)
	{
		return 0;
	}
	return 1;   //�ǿ�λ�ͷ���1
}


void Food()//����ʳ��
{
	int i,j;
	do
	{
		i=rand()%30;                 //����0~H-1֮���һ����
        j=rand()%10;
	
	}
	while(Chek(i,j)==0);
	map[i][j]=-1;//����ʳ��
}


void Eat_Food()//��ʳ��
{
	if(map[snake_Grid[0][0]][snake_Grid[0][1]]==-1) //�����ͷ����ʳ�������Ͷ��ʳ����Ұ�ʳ�������Ϊ0
  {
    eated=true;            //����Ѿ��Ե�ʳ��
		score+=1;
    Food();
    map[snake_Grid[0][0]][snake_Grid[0][1]]=0;  //ȥ��ʳ��
  }
}


void Show_Score()//��ʾ����
{
	OLED_ShowString(30,0,"Score:",16,1);
	OLED_ShowNum(80,0,score,2,16,1);
}
