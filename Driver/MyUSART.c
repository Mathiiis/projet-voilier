#include "stm32f10x.h"
#include "MyUSART.h"

int ch = 0 ;

void MyUSART_Init(USART_TypeDef * USART) {
	USART->CR1 |= 0x1<<13 ; // Active l'USART
	USART->CR1 |= 0x1<<3 ; // Active le transmitter
	USART->CR1 |= 0x1<<2 ; // Active le receiver
	USART->CR1 |= 0x1<<5 ; // Active l'interruption RXNE (RX not empty)
//	USART->CR1 |= 0x1<<7 ; // Active l'interuption TXE (TX empty)
	USART->CR1 |= 0x0<<12 ; // Defini la taille du message
	
	USART->CR2 |= 0x0<<12 ; // Nombre de bit de stop (=> on change rien = 1 bit de stop)
}

void MyUSART_ActiveIT(USART_TypeDef * USART) {
	NVIC->ISER[1] |= 1<<6 ; //Active l'interruption aux yeux du umicro
}
	
void MyUSART_SetBaudRate(USART_TypeDef * USART, int baudrate) {
	USART->BRR = 36e6 / baudrate ; // Met en place le baudrate
}

void USART2_IRQHandler(void) {
	ch = USART2->DR ;
}