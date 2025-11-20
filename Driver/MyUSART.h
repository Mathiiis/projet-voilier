#ifndef __myUSART_h
#define __myUSART_h
#include "stm32f10x.h"

extern int ch;

void MyUSART_Init(USART_TypeDef * USART) ;

void MyUSART_ActiveIT(USART_TypeDef * USART) ;
	
void MyUSART_SetBaudRate(USART_TypeDef * USART, int baudrate) ;

#endif
