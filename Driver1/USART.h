#ifndef __piloteUSART_h
#define __piloteUSART_h 
#include "stm32f10x.h"

void MyUSART_Init(USART_TypeDef * USART) ;

void USART2_Init(void) ;

void MyUSART_ActiveIT(USART_TypeDef * USART) ;
	
void MyUSART_SetBaudRate(USART_TypeDef * USART, int baudrate) ;

void USART2_SendChar(char c);

void USART2_SendString(const char *s);

void USART1_SendChar(char c) ;

void USART1_SendString(const char *s) ;

#endif