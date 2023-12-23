#include "rwdata.h"
#include "malloc.h"
#include "string.h"


#define FILE_MAX_TYPE_NUM		7	//���FILE_MAX_TYPE_NUM������
#define FILE_MAX_SUBT_NUM		4	//���FILE_MAX_SUBT_NUM��С��

 //�ļ������б�
u8*const FILE_TYPE_TBL[FILE_MAX_TYPE_NUM][FILE_MAX_SUBT_NUM]=
{
{"BIN"},			//BIN�ļ�
{"LRC"},			//LRC�ļ�
{"NES"},			//NES�ļ�
{"TXT","C","H"},	//�ı��ļ�
{"WAV","MP3","APE","FLAC"},//֧�ֵ������ļ�
{"BMP","JPG","JPEG","GIF"},//ͼƬ�ļ�
{"AVI"},			//��Ƶ�ļ�
};


///////////////////////////////�����ļ���,ʹ��malloc��ʱ��////////////////////////////////////////////
FATFS *fs[_VOLUMES];//�߼����̹�����.	 
FIL *file;	  		//�ļ�1
FIL *ftemp;	  		//�ļ�2.
UINT br,bw;			//��д����
FILINFO fileinfo;	//�ļ���Ϣ
DIR dir;  			//Ŀ¼

u8 *fatbuf;			//SD�����ݻ�����

/**************************************************
�������ƣ�FATFS_Init(void)
�������ܣ�Ϊexfuns�����ڴ�
��ڲ�������
���ز�����0,�ɹ�   1,ʧ��
***************************************************/
u8 FATFS_Init(void)
{
	u8 i;
	for(i=0;i<_VOLUMES;i++)
	{
		fs[i]=(FATFS*)mymalloc(SRAMIN,sizeof(FATFS));	//Ϊ����i�����������ڴ�	
		if(!fs[i])break;
	}
	file=(FIL*)mymalloc(SRAMIN,sizeof(FIL));		//Ϊfile�����ڴ�
	ftemp=(FIL*)mymalloc(SRAMIN,sizeof(FIL));		//Ϊftemp�����ڴ�
	fatbuf=(u8*)mymalloc(SRAMIN,512);				//Ϊfatbuf�����ڴ�
	if(i==_VOLUMES&&file&&ftemp&&fatbuf)return 0;  //������һ��ʧ��,��ʧ��.
	else return 1;	
}


/**************************************************
�������ƣ�u8 char_upper(u8 c)
�������ܣ���Сд��ĸתΪ��д��ĸ,���������,�򱣳ֲ���.
��ڲ�����c Ҫת�����ַ�
���ز�������д��ĸ
***************************************************/
u8 char_upper(u8 c)
{
	if(c<'A')return c;//����,���ֲ���.
	if(c>='a')return c-0x20;//��Ϊ��д.
	else return c;//��д,���ֲ���
}	

/**************************************************
�������ƣ�FATFS_ScanFilesType(u8 *fname)
�������ܣ������ļ�������
��ڲ�����fname:�ļ���
���ز�����0XFF,��ʾ�޷�ʶ����ļ����ͱ��.
					����,����λ��ʾ��������,����λ��ʾ����С��.
***************************************************/	 
u8 FATFS_ScanFilesType(u8 *fname)
{
	u8 tbuf[5];
	u8 *attr='\0';//��׺��
	u8 i=0,j;
	while(i<250)
	{
		i++;
		if(*fname=='\0')break;//ƫ�Ƶ��������.
		fname++;
	}
	if(i==250)return 0XFF;//������ַ���.
 	for(i=0;i<5;i++)//�õ���׺��
	{
		fname--;
		if(*fname=='.')
		{
			fname++;
			attr=fname;
			break;
		}
  	}
	strcpy((char *)tbuf,(const char*)attr);//copy
 	for(i=0;i<4;i++)tbuf[i]=char_upper(tbuf[i]);//ȫ����Ϊ��д 
	for(i=0;i<FILE_MAX_TYPE_NUM;i++)	//����Ա�
	{
		for(j=0;j<FILE_MAX_SUBT_NUM;j++)//����Ա�
		{
			if(*FILE_TYPE_TBL[i][j]==0)break;//�����Ѿ�û�пɶԱȵĳ�Ա��.
			if(strcmp((const char *)FILE_TYPE_TBL[i][j],(const char *)tbuf)==0)//�ҵ���
			{
				return (i<<4)|j;
			}
		}
	}
	return 0XFF;//û�ҵ�		 			   
}	 


u8 FATFS_ScanFiles(uint8_t *dir, FileNameTypeDef *file)
{
	FRESULT res;
	FILINFO  fileInfo;
	DIR dirs;
	char *fn, i = 0; 

#if _USE_LFN
    static char lfn[_MAX_LFN + 1];   /* Buffer to store the LFN */
    fileInfo.lfname = lfn;
    fileInfo.lfsize = sizeof(lfn);
#endif
	/* ���ļ��� */
	res = f_opendir(&dirs, (const TCHAR*)dir);
	
	if (res == FR_OK)    //�ɹ����ļ��� 
	{    
		while (f_readdir(&dirs, &fileInfo) == FR_OK)   //����˳����ļ��� 
		{ 	 
			if(!fileInfo.fname[0]) 	  //����ļ���Ϊ0,���� 
			{
				break;     
			} 	
			
			/* �ж��ļ����� */								
			if((fileInfo.fattrib == AM_ARC)|(fileInfo.fattrib == AM_DIR)) 
			{

#if _USE_LFN	//Ϊ���ַ�����׼��
            	fn = *fileInfo.lfname ? fileInfo.lfname : fileInfo.fname;
#else
           	 	fn = fileInfo.fname;
#endif

				/* ��ȡ���� */
				i = 0;
	       		while((*fn != '.') && (*fn != '\0')) //��һ������������ѭ��
				{
					file->name[i] = *fn;
					i++;
					fn++;	
				}
                file->name[i] = '\0';

				/* ��ȡ��׺ */
				i = 0;
				while(*fn != '\0')
				{
					file->type[i] = *fn;
					i++;
					fn++;
				}
                file->type[i] = '\0';
                 
				file++;
			}//end of �ļ������ж���
		} //end of while (f_readdir(&dirs, &fileInfo) == FR_OK)//����˳����ļ���
	}
	return res;
}


/**************************************************
�������ƣ�FATFS_RW_Text(void)
�������ܣ��õ�����ʣ������
��ڲ�����drv:���̱��("0:"/"1:")
					total:������	 ����λKB��
					free:ʣ������	 ����λKB��
���ز�����0,����.����,�������
***************************************************/
u8 FATFS_GetFree(u8 *drv,u32 *total,u32 *free)
{
	 FATFS *fs1;
	 u8 res;
   u32 fre_clust=0, fre_sect=0, tot_sect=0;
    //�õ�������Ϣ�����д�����
   res =(u32)f_getfree((const TCHAR*)drv, (DWORD*)&fre_clust, &fs1);
   if(res==0)
	 {											   
	   tot_sect=(fs1->n_fatent-2)*fs1->csize;	//�õ���������
	   fre_sect=fre_clust*fs1->csize;			//�õ�����������	   
#if _MAX_SS!=512				  				//������С����512�ֽ�,��ת��Ϊ512�ֽ�
		tot_sect*=fs1->ssize/512;
		fre_sect*=fs1->ssize/512;
#endif	  
		*total=tot_sect>>1;	//��λΪKB
		*free=fre_sect>>1;	//��λΪKB 
	 }
	return res;
}	


/**************************************************
�������ƣ�FATFS_RW_Text(void)
�������ܣ�FATFS��д����
��ڲ�������
���ز�������
***************************************************/
void FATFS_RW_Text(void)
{
	u8 res;
	char DataBuff[512];
	f_mount(fs[0],"0:",1); 					//����SD�� 	
	
	f_mkdir("0:\\Data");            //���� (Data) �ļ���
	
	res=f_open(file,"0:\\Data\\data.txt",FA_CREATE_ALWAYS|FA_WRITE);//���ļ�
	if(res!=0)printf("Open the failure ! , res=%d\n",res);
	
	res=f_write(file,"abcdefg",100,&bw);//���ļ�д������
	if(res!=0)printf("Write the failure ! , res=%d\n",res);
	f_close(file);                      //�ر��ļ�
	
	
	res=f_open(file,"0:\\Data\\data.txt",FA_READ);//���ļ���ֻ��
	if(res!=0)printf("Open the failure ! , res=%d\n",res);
	res=f_read(file,DataBuff,100,&br);
	if(res!=0)printf("Read the failure ! , res=%d\n",res);
	f_close(file);//�ر��ļ�
	delay_ms(100);
	printf("DataBuff:\n");
	printf(DataBuff);

}


