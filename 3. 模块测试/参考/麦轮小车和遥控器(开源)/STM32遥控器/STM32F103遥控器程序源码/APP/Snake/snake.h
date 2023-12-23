#ifndef _SNAKE_H_
#define _SNAKE_H_
#include "sys.h"
#include "stdbool.h"



#define UP 3
#define DOWN 4
#define LEFT 1
#define RIGHT 2

extern int map[32][12];//��ͼ��С
extern int key;
extern int command;
extern bool eated;
extern int score;
extern int Ui_Show_Time;


void GUI_Refresh(int map[32][12]);//����ˢ��
void GUI_Init(void);//�����ʼ��
void Creat_Map(void);//��ʼ����ͼ�߽�
void Move(void);//�ƶ�
void Paint_Map(int x,int y);//���Ƶ�ͼ
void Paint_Head(int x,int y );//������ͷ
void Paint_Tail(int x,int y );//����β��
void Paint_Food(int x,int y );//����ʳ��
void Paint_Body(int x,int y );//��������
void drawSnake(void);            //����
void Snake_Init(void);//�߼�ʳ���ʼ��
void Paint_Clean(int x,int y );//�����
void Get_Command(void );//��ȡ����ֵ
void Grid_Bound(void);//�����޶�
bool GameOver(void);//��Ϸ����
void Food(void);//����ʳ��
int  Chek(int i,int j);//����ͼ��λ
void Eat_Food(void);//��ʳ��
void Show_Score(void);//��ʾ����
void Data_Clear(void);//���ݳ�ʼ��

#endif  


