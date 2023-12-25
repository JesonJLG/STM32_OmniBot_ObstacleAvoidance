#ifndef __MPUDATA_H
#define __MPUDATA_H
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "mpu6050.h" //MPU6050Çý¶¯¿â
#include "inv_mpu.h" //ÍÓÂÝÒÇÇý¶¯¿â
#include "inv_mpu_dmp_motion_driver.h" //DMP×ËÌ¬½â¶Á¿â

void MPUInit(void);
void MPU_Read(void);
void SendMpuData(void);
void MpuDataProcessing(void)	;

#endif



