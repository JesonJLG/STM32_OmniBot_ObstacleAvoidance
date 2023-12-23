#ifndef __EXFUNS_H
#define __EXFUNS_H 					   
#include <stm32f10x.h>
#include "ff.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������V3
//FATFS ��չ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/20
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

extern FATFS *fs[_VOLUMES];  
extern FIL *file;	 
extern FIL *ftemp;	 
extern UINT br,bw;
extern FILINFO fileinfo;
extern DIR dir;
extern u8 *fatbuf;//SD�����ݻ�����




//f_typetell���ص����Ͷ���
//���ݱ�FILE_TYPE_TBL���.��exfuns.c���涨��
#define _BIN		0X00	//bin�ļ�
#define _LRC		0X10	//lrc�ļ�
#define _NES		0X20	//nes�ļ�

#define __TEXT		0X30	//.txt�ļ�
#define _C			0X31	//.c�ļ�
#define _H			0X32    //.h�ļ�

#define _MP1		0X40	//MP1�ļ�
#define _MP2		0X41	//MP2�ļ� 
#define _MP3		0X42	//MP3�ļ�
#define _MP4		0X43	//MP4�ļ�
#define _M4A		0X44	//M4A�ļ�
#define _3GP		0X45	//3GP�ļ�
#define _3G2		0X46	//3G2�ļ�
#define _OGG		0X47	//OGG�ļ�
#define _AAC		0X48	//AAC�ļ�
#define _WMA		0X49	//WMA�ļ�
#define _WAV		0X4A	//WAV�ļ�
#define _MID		0X4B	//MIDI�ļ�
#define _FLAC		0X4C	//FLAC�ļ� 
 

#define _BMP		0X50	//bmp�ļ�
#define _JPG		0X51	//jpg�ļ�
#define _JPEG		0X52	//jpeg�ļ�		 
#define _GIF		0X53	//gif�ļ�   

#define _AVI		0X60	//avi�ļ�  

 
u8 exfuns_init(void);							//�����ڴ�
u8 f_typetell(u8 *fname);						//ʶ���ļ�����
u8 exf_getfree(u8 *drv,u32 *total,u32 *free);	//�õ�������������ʣ������
u32 exf_fdsize(u8 *fdname);						//�õ��ļ��д�С
u8* exf_get_src_dname(u8* dpfn);																		   
u8 exf_copy(u8(*fcpymsg)(u8*pname,u8 pct,u8 mode),u8 *psrc,u8 *pdst,u32 totsize,u32 cpdsize,u8 fwmode);	   //�ļ�����
u8 exf_fdcopy(u8(*fcpymsg)(u8*pname,u8 pct,u8 mode),u8 *psrc,u8 *pdst,u32 *totsize,u32 *cpdsize,u8 fwmode);//�ļ��и���	  
#endif


