#include "GPIO.h"
#include "stm32f10x.h"
#include "USART.h"


void USART2_Init(void)
{
    // 1) Activer les horloges
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   // GPIOA
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN; // USART2

  
    // 2) Régler la vitesse (9600 bauds avec PCLK1 = 36 MHz)
    USART2->BRR = 0xEA6;  // 36 MHz / 9600 ˜ 3750 ? 0xEA6

    //  Mettre les bons bits à 1 dans CR1 :
    USART2->CR1 |= (1 << 13);  // UE = 1 ? USART enable
    USART2->CR1 |= (1 << 3);   // TE = 1 ? Transmitter enable
    USART2->CR1 |= (1 << 2);   // RE = 1 ? Receiver enable
    USART2->CR1 |= (1 << 5);   // RXNEIE = 1 ? interruption réception	
	
	}

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