#ifndef _SNAKE_H_
#define _SNAKE_H_
#include "sys.h"
#include "stdbool.h"



#define UP 3
#define DOWN 4
#define LEFT 1
#define RIGHT 2

extern int map[32][12];//地图大小
extern int key;
extern int command;
extern bool eated;
extern int score;
extern int Ui_Show_Time;


void GUI_Refresh(int map[32][12]);//界面刷新
void GUI_Init(void);//界面初始化
void Creat_Map(void);//初始化地图边界
void Move(void);//移动
void Paint_Map(int x,int y);//绘制地图
void Paint_Head(int x,int y );//绘制蛇头
void Paint_Tail(int x,int y );//绘制尾巴
void Paint_Food(int x,int y );//绘制食物
void Paint_Body(int x,int y );//绘制身体
void drawSnake(void);            //画蛇
void Snake_Init(void);//蛇及食物初始化
void Paint_Clean(int x,int y );//清除点
void Get_Command(void );//获取键盘值
void Grid_Bound(void);//坐标限定
bool GameOver(void);//游戏结束
void Food(void);//生成食物
int  Chek(int i,int j);//检查地图空位
void Eat_Food(void);//吃食物
void Show_Score(void);//显示分数
void Data_Clear(void);//数据初始化

#endif  


