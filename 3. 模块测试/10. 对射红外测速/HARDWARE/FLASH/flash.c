#include "flash.h"
#include "delay.h"
#include "usart.h"

//Ҫд�뵽STM32 FLASH���ַ�������
const u8 TEXT_Buffer[]={"STM32F103 FLASH TEST"};
#define SIZE sizeof(TEXT_Buffer)		//���鳤��
#define FLASH_SAVE_ADDR  0X08004000		//����FLASH �����ַ(����Ϊż��������ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)
//ע�ⲻͬ������Ƭ����flash��ƫ�Ƶ�ַ

/***********************************************************
�������ƣ�STMFLASH_ReadHalfWord(u32 faddr)
�������ܣ���ȡָ����ַ�İ���(16λ����)
��ڲ�����faddr  ����ַ(�˵�ַ����Ϊ2�ı���!!)
���ز�������Ӧ����.
************************************************************/
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}
#if STM32_FLASH_WREN	//���ʹ����д   

/**************************************************************************
�������ƣ�STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
�������ܣ�������д��
��ڲ�����WriteAddr:��ʼ��ַ
          pBuffer:����ָ��
					NumToWrite:����(16λ)��   
���ز�������
***************************************************************************/
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//��ַ����2.
	}  
} 


#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else 
#define STM_SECTOR_SIZE	2048
#endif
u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//�����2K�ֽ�

/***********************************************************
�������ƣ�STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
�������ܣ���ָ����ַ��ʼд��ָ�����ȵ�����
��ڲ�����WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
          pBuffer:����ָ��
					NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
���ز�������
************************************************************/
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	u32 secpos;	   //������ַ
	u16 secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	u16 secremain; //������ʣ���ַ(16λ�ּ���)	   
 	u16 i;    
	u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
	if(WriteAddr<STM32_FLASH_BASE||(WriteAddr>=(STM32_FLASH_BASE+1024*STM32_FLASH_SIZE)))return;//�Ƿ���ַ
	FLASH_Unlock();						//����
	offaddr=WriteAddr-STM32_FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/STM_SECTOR_SIZE;			//������ַ  0~127 for STM32F103RBT6
	secoff=(offaddr%STM_SECTOR_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=STM_SECTOR_SIZE/2-secoff;		//����ʣ��ռ��С   
	if(NumToWrite<=secremain)secremain=NumToWrite;//�����ڸ�������Χ
	while(1) 
	{	
		STMFLASH_Read(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(STMFLASH_BUF[secoff+i]!=0XFFFF)break;//��Ҫ����  	  
		}
		if(i<secremain)//��Ҫ����
		{
			FLASH_ErasePage(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE);//�����������
			for(i=0;i<secremain;i++)//����
			{
				STMFLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			STMFLASH_Write_NoCheck(secpos*STM_SECTOR_SIZE+STM32_FLASH_BASE,STMFLASH_BUF,STM_SECTOR_SIZE/2);//д����������  
		}else STMFLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 				   
		if(NumToWrite==secremain)break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff=0;				//ƫ��λ��Ϊ0 	 
		   	pBuffer+=secremain;  	//ָ��ƫ��
			WriteAddr+=secremain;	//д��ַƫ��	   
		   	NumToWrite-=secremain;	//�ֽ�(16λ)���ݼ�
			if(NumToWrite>(STM_SECTOR_SIZE/2))secremain=STM_SECTOR_SIZE/2;//��һ����������д����
			else secremain=NumToWrite;//��һ����������д����
		}	 
	};	
	FLASH_Lock();//����
}
#endif


/*****************************************************************
�������ƣ�STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead) 
�������ܣ���ָ����ַ��ʼ����ָ�����ȵ�����
��ڲ�����ReadAddr:��ʼ��ַ
          pBuffer:����ָ��
					NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
���ز�������
*****************************************************************/
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}


/***********************************************************
�������ƣ�Test_Write(u32 WriteAddr,u16 WriteData)   	
�������ܣ���ָ����ַ��ʼ����ָ�����ȵ�����
��ڲ�����WriteAddr:��ʼ��ַ
          WriteData:Ҫд�������
���ز�������
************************************************************/
void Test_Write(u32 WriteAddr,u16 WriteData)   	
{
	STMFLASH_Write(WriteAddr,&WriteData,1);//д��һ���� 
}


/***********************************************************
�������ƣ�Flash_Text(void)   	
�������ܣ���дFlash����
��ڲ�������
���ز�������
************************************************************/
void Flash_Text(void)
{
	u8 i;
	u8 Read_Buffer[SIZE];
	STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)TEXT_Buffer,SIZE);
  delay_ms(1000);
	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)Read_Buffer,SIZE);
	for(i=0;i<SIZE;i++)
	{
		if(TEXT_Buffer[i]!=Read_Buffer[i])break;
	}
	if(i==SIZE)printf("Read and write success !\n");
}



