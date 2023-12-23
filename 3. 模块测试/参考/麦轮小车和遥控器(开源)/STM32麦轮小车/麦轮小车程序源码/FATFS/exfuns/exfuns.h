#ifndef __EXFUNS_H
#define __EXFUNS_H 					   
#include <stm32f10x.h>
#include "ff.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板V3
//FATFS 扩展代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/20
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

extern FATFS *fs[_VOLUMES];  
extern FIL *file;	 
extern FIL *ftemp;	 
extern UINT br,bw;
extern FILINFO fileinfo;
extern DIR dir;
extern u8 *fatbuf;//SD卡数据缓存区




//f_typetell返回的类型定义
//根据表FILE_TYPE_TBL获得.在exfuns.c里面定义
#define _BIN		0X00	//bin文件
#define _LRC		0X10	//lrc文件
#define _NES		0X20	//nes文件

#define __TEXT		0X30	//.txt文件
#define _C			0X31	//.c文件
#define _H			0X32    //.h文件

#define _MP1		0X40	//MP1文件
#define _MP2		0X41	//MP2文件 
#define _MP3		0X42	//MP3文件
#define _MP4		0X43	//MP4文件
#define _M4A		0X44	//M4A文件
#define _3GP		0X45	//3GP文件
#define _3G2		0X46	//3G2文件
#define _OGG		0X47	//OGG文件
#define _AAC		0X48	//AAC文件
#define _WMA		0X49	//WMA文件
#define _WAV		0X4A	//WAV文件
#define _MID		0X4B	//MIDI文件
#define _FLAC		0X4C	//FLAC文件 
 

#define _BMP		0X50	//bmp文件
#define _JPG		0X51	//jpg文件
#define _JPEG		0X52	//jpeg文件		 
#define _GIF		0X53	//gif文件   

#define _AVI		0X60	//avi文件  

 
u8 exfuns_init(void);							//申请内存
u8 f_typetell(u8 *fname);						//识别文件类型
u8 exf_getfree(u8 *drv,u32 *total,u32 *free);	//得到磁盘总容量和剩余容量
u32 exf_fdsize(u8 *fdname);						//得到文件夹大小
u8* exf_get_src_dname(u8* dpfn);																		   
u8 exf_copy(u8(*fcpymsg)(u8*pname,u8 pct,u8 mode),u8 *psrc,u8 *pdst,u32 totsize,u32 cpdsize,u8 fwmode);	   //文件复制
u8 exf_fdcopy(u8(*fcpymsg)(u8*pname,u8 pct,u8 mode),u8 *psrc,u8 *pdst,u32 *totsize,u32 *cpdsize,u8 fwmode);//文件夹复制	  
#endif


