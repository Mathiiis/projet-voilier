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

		NVIC_EnableIRQ(USART2_IRQn) ;
	
	
	}

	
	