#include "nrf24l01.h"
#include "delay.h"
#include "spi.h"
#include "usart.h"

/************************************************************************
NRF24L01��CE->PB13`  CSN->PB14  IRQ->PB15  SCK->PA5  MISO->PA6  MOSI->PA7  
������->PB12
|-----------------|  IRQ   MISO 
|       NRF     ::|  MOSI  SCK                       
|      24L01    ::|  CSN   CE
|-----------------|  VCC   GND
************************************************************************/
  
u8 Tx_buf[10]="abcdefgh";
u8 Rx_buf[10];//NRF���ܻ��������趨��Ϊȫ�ֱ���

const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01};


/**************************************************
�������ƣ�NRF24L01_Init(void)
�������ܣ�NRF24L01��ʼ������
��ڲ�������
���ز�������
***************************************************/
void NRF24L01_Init(void)
{ 	
	GPIO_InitTypeDef GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB�˿�ʱ��
    	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //PB12���� ��ֹ����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);	//��ʼ��ָ��IO
 	GPIO_SetBits(GPIOB,GPIO_Pin_12);//����				
 	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;	//PB13 14 ���� 	  
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��ָ��IO
  GPIO_ResetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_15;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PB15 ��������  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);//PB15����
						 		 
  SPI1_Init();    		//��ʼ��SPI1 
	
  SPI_Cmd(SPI1, DISABLE); // SPI���費ʹ��
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//SPI����
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//ʱ�����յ�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//���ݲ����ڵ�1��ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź����������
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
			 
	NRF24L01_CE=0; 			//ʹ��24L01
	NRF24L01_CSN=1;			//SPIƬѡȡ��  
	 		 	 
}

/**************************************************
�������ƣ�NRF24L01_Check(void)
�������ܣ����NRF24L01�Ƿ����
��ڲ�������
���ز�����0���ɹ�;1��ʧ��
***************************************************/
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 i;
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8); //spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   	 
	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	NRF24L01_Read_Buf(TX_ADDR,buf,5); //����д��ĵ�ַ  
	for(i=0;i<5;i++)if(buf[i]!=0XA5)break;
	if(i!=5)return 1;//���24L01����	
	return 0;		 //��⵽24L01
}	 

/**************************************************
�������ƣ�NRF24L01_Write_Reg(u8 reg,u8 value)
�������ܣ�NRF24L01д�Ĵ���
��ڲ�����reg:ָ���Ĵ�����ַ  value:д���ֵ
���ز�����status  ״ֵ̬
***************************************************/
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;	
  NRF24L01_CSN=0;                 //ʹ��SPI����
  status =SPI1_ReadWriteByte(reg);//���ͼĴ����� 
  SPI1_ReadWriteByte(value);      //д��Ĵ�����ֵ
  NRF24L01_CSN=1;                 //��ֹSPI����	   
  return(status);       			//����״ֵ̬
}


/**************************************************
�������ƣ�NRF24L01_Read_Reg(u8 reg,u8 value)
�������ܣ�NRF24L01���Ĵ���
��ڲ�����reg:ָ���Ĵ�����ַ
���ز�����reg_val  ״ֵ̬
***************************************************/
u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;	    
 	NRF24L01_CSN = 0;          //ʹ��SPI����		
  SPI1_ReadWriteByte(reg);   //���ͼĴ�����
  reg_val=SPI1_ReadWriteByte(0XFF);//��ȡ�Ĵ�������
  NRF24L01_CSN = 1;          //��ֹSPI����		    
  return(reg_val);           //����״ֵ̬
}	

/***********************************************************
�������ƣ�NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
�������ܣ�NRF24L01��ָ��λ�ö���ָ�����ȵ�����
��ڲ�����reg:ָ���Ĵ�����ַ  *pBuf:����ָ��  len:���ݳ���
���ز�����status  �˴ζ�����״̬�Ĵ���ֵ
************************************************************/
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;	       
  NRF24L01_CSN = 0;           //ʹ��SPI����
  status=SPI1_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬   	   
 	for(u8_ctr=0;u8_ctr<len;u8_ctr++)pBuf[u8_ctr]=SPI1_ReadWriteByte(0XFF);//��������
  NRF24L01_CSN=1;       //�ر�SPI����
  return status;        //���ض�����״ֵ̬
}

/***********************************************************
�������ƣ�NRF24L01_Write_Buf(u8 reg,u8 *pBuf,u8 len)
�������ܣ�NRF24L01��ָ��λ��д��ָ�����ȵ�����
��ڲ�����reg:ָ���Ĵ�����ַ  *pBuf:����ָ��  len:���ݳ���
���ز�����status  �˴ζ�����״̬�Ĵ���ֵ
************************************************************/
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	u8 status,u8_ctr;	    
 	NRF24L01_CSN = 0;          //ʹ��SPI����
  status = SPI1_ReadWriteByte(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
  for(u8_ctr=0; u8_ctr<len; u8_ctr++)SPI1_ReadWriteByte(*pBuf++); //д������	 
  NRF24L01_CSN = 1;       //�ر�SPI����
  return status;          //���ض�����״ֵ̬
}	

/***********************************************************
�������ƣ�NRF24L01_TxPacket(u8 *txbuf)
�������ܣ�����NRF24L01����һ������
��ڲ�����txbuf  �����͵������׵�ַ
���ز���������ֵ:TX_OK��������ɣ�0xff���������
************************************************************/
u8 NRF24L01_TxPacket(u8 *txbuf)
{
	u8 sta;
 	SPI1_SetSpeed(SPI_BaudRatePrescaler_8);//spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
	NRF24L01_CE=0;
  NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  32���ֽ�
 	NRF24L01_CE=1;//��������	   
	while(NRF24L01_IRQ!=0);//�ȴ��������
	sta=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&MAX_TX)//�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
		return MAX_TX; 
	}
	if(sta&TX_OK)//�������
	{
		return TX_OK;
	}
	return 0xff;//����ԭ����ʧ��
}


/***********************************************************
�������ƣ�NRF24L01_RxPacket(u8 *rxbuf)
�������ܣ�����NRF24L01����һ������
��ڲ�����rxbuf:�����������׵�ַ
���ز���������ֵ:0��������ɣ��������������
************************************************************/
u8 NRF24L01_RxPacket(u8 *rxbuf)
{
	u8 sta;		    							   
	SPI1_SetSpeed(SPI_BaudRatePrescaler_8); //spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
	sta=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ    	 
	NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&RX_OK)//���յ�����
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
		return 0; 
	}	   
	return 1;//û�յ��κ�����
}			


/***********************************************************
�������ƣ�NRF24L01_RX_Mode(void)
�������ܣ���ʼ��NRF24L01��RXģʽ����RX��ַ,дRX���ݿ��,ѡ��
          RFƵ��,�����ʺ�LNA HCURR����CE��ߺ�,������RXģʽ,
          �����Խ���������
��ڲ�������
���ز�������
************************************************************/	   
void NRF24L01_RX_Mode(void)
{
	  NRF24L01_CE=0;	  
  	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ
	  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);    //ʹ��ͨ��0���Զ�Ӧ��    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ  	 
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);	     //����RFͨ��Ƶ��		  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 	    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);//����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
  	NRF24L01_CE = 1; //CEΪ��,�������ģʽ 
}	

/*****************************************************************
�������ƣ�NRF24L01_TX_Mode(void)
�������ܣ���ʼ��NRF24L01��TXģʽ,����TX�Զ�Ӧ��ĵ�ַ,���TX����
					����,ѡ��RFƵ��,�����ʺ�LNA HCURR,PWR_UP,CRCʹ��,��CE��
					�ߺ�,������RXģʽ,�����Խ���������,CEΪ�ߴ���10us,����
					������.	 	
          
��ڲ�������
���ز�������
*****************************************************************/	   
void NRF24L01_TX_Mode(void)
{														 
	  NRF24L01_CE=0;	    
  	NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
  	NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  

  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
  	NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
  	NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       //����RFͨ��Ϊ40
  	NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	  NRF24L01_CE=1;//CEΪ��,10us����������
}



/***********************************************************
�������ƣ�NRF24L01_Check_detection(void)
�������ܣ����NRF24L01�Ƿ�Ӧ��
��ڲ�������
���ز�������
************************************************************/
void NRF24L01_Check_detection(void)
{
	while(NRF24L01_Check())
	{
		printf("NRF24L01_Check failed !");
		printf("\n");
		delay_ms(1000);
	}
	printf("NRF24L01_Check succeed !");
	printf("\n");
}


/***********************************************************
�������ƣ�NRF24L01_Text(u8 mode)
�������ܣ�NRF24L01�շ�����
��ڲ�����mode 0 ���� ��1 ����
���ز�������
************************************************************/
void NRF24L01_Text(u8 mode)
{
	if(mode==0)
	{
		NRF24L01_RX_Mode();	
		if(NRF24L01_RxPacket(Rx_buf)==0)//һ�����յ���Ϣ,����ʾ����.
		{
			Rx_buf[9]=0;//�����ַ��������� 
			printf((char*)Rx_buf); 
			printf("\n");
		}
		else
		{										   	
 			printf("Receive failed !"); 
			printf("\n");
		}
		delay_ms(1000);	
	}
	if(mode==1)
	{
		NRF24L01_TX_Mode();
		if(NRF24L01_TxPacket(Tx_buf)==TX_OK)
		{
				LED=~LED;
		}
		else
		{										   	
 			printf("Send failed !");
			printf("\n");			
		}
		delay_ms(1000);	
	}
}
