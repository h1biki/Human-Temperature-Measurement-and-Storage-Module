#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

extern unsigned char Uart1_Receive_Data;
extern unsigned char Uart1_Receive_Change;

//如果想串口中断接收，请不要注释以下宏定义
void uart_init(u32 bound);
void UART_SendByte(unsigned char byte);
void UART_SendStr(unsigned char *s);


#endif


