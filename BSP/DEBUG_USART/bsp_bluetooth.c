#include "bsp_bluetooth.h"

#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

typedef union {unsigned char byte[4];float num;}t_floattobyte;


void Debug_USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* ��һ������ʼ��GPIO */
	 RCC_AHB1PeriphClockCmd(DEBUG_USART_RX_GPIO_CLK|DEBUG_USART_TX_GPIO_CLK,ENABLE);
	
	  /* GPIO��ʼ�� */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  /* ����Tx����Ϊ���ù���  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_TX_PIN  ;  
  GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
	
	  /* ����Rx����Ϊ���ù��� */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = DEBUG_USART_RX_PIN;
  GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	/* GPIO���帴�ó�ʲô */
	 /* ���� PXx �� USARTx_Tx*/
  GPIO_PinAFConfig(DEBUG_USART_RX_GPIO_PORT,DEBUG_USART_RX_SOURCE,DEBUG_USART_RX_AF);
  /*  ���� PXx �� USARTx__Rx*/
  GPIO_PinAFConfig(DEBUG_USART_TX_GPIO_PORT,DEBUG_USART_TX_SOURCE,DEBUG_USART_TX_AF);
	
	/* �ڶ��������ô��ڳ�ʼ���ṹ�� */
	  /* ʹ�� USART ʱ�� */
  RCC_APB2PeriphClockCmd(DEBUG_USART_CLK, ENABLE);
	  /* ���ô�DEBUG_USART ģʽ */
  /* ���������ã�DEBUG_USART_BAUDRATE */
  USART_InitStructure.USART_BaudRate = DEBUG_USART_BAUDRATE;
  /* �ֳ�(����λ+У��λ)��8 */
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  /* ֹͣλ��1��ֹͣλ */
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  /* У��λѡ�񣺲�ʹ��У�� */
  USART_InitStructure.USART_Parity = USART_Parity_No;
  /* Ӳ�������ƣ���ʹ��Ӳ���� */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  /* USARTģʽ���ƣ�ͬʱʹ�ܽ��պͷ��� */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* ���USART��ʼ������ */
  USART_Init(DEBUG_USART, &USART_InitStructure);
	
	
	/* ʹ�ܴ��� */
  USART_Cmd(DEBUG_USART, ENABLE);
}



void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* ����һ���ֽ����ݵ�USART */
	USART_SendData(pUSARTx,ch);
		

	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);	
}



void Usart_SendString( USART_TypeDef * pUSARTx, char *str)
{
	unsigned int k=0;
  do 
  {
      Usart_SendByte( pUSARTx, *(str + k) );
      k++;
  } while(*(str + k)!='\0');
  

  while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET);
}


int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(DEBUG_USART, (uint8_t) ch);
		
		
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}

uint8_t put_char(uint8_t ch){
		USART_SendData(DEBUG_USART, (uint8_t) ch);
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}
int fgetc(FILE *f)
{
		
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(DEBUG_USART);
}


uint8_t put_Int16(uint16_t ch){
		uint8_t sum = 0;
		USART_SendData(DEBUG_USART, BYTE1(ch));
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);
		USART_SendData(DEBUG_USART, BYTE0(ch));
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);
		sum += BYTE1(ch);
		sum += BYTE0(ch);
		return sum;
}

uint8_t put_Float(float ch){
		t_floattobyte floattobyte;
		uint8_t sum = 0;
		floattobyte.num = ch;
		USART_SendData(DEBUG_USART, floattobyte.byte[3]);
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);
		USART_SendData(DEBUG_USART, floattobyte.byte[2]);
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);
		USART_SendData(DEBUG_USART, floattobyte.byte[1]);
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);
		USART_SendData(DEBUG_USART, floattobyte.byte[0]);
		while (USART_GetFlagStatus(DEBUG_USART, USART_FLAG_TXE) == RESET);
		sum += BYTE1(floattobyte.byte[3]);
		sum += BYTE0(floattobyte.byte[2]);
		sum += BYTE1(floattobyte.byte[1]);
		sum += BYTE0(floattobyte.byte[0]);
		return sum;
}




