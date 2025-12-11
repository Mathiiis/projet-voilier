#include "Plateau.h"
#include "../Driver2/Pilote_GPIO.h"
#include "../Driver1/Timer.h"



void InitPlateau(void)
{
    // PWM sur PB6 (TIM4 CH1), fréquence 20 kHz
    MyTimer_PWM_Init(GPIOB, 6, TIM4, 1, 3599, 0);

    // GPIO PB7 = sens de rotation
    InitGPIO(GPIOB, 7, OUTPUTPUSHPULL);
}

void SetPlateau(float DC)
{
	
		//float DCC = DC*3.6; //arr 3600
    MyTimer_PWM_SetDutyCycle(TIM4, 1, DC);
}

void SetPlateauSens(int sens)
{
    if (sens == 0)
        GPIOB->ODR &= ~(1 << 7);   // Babord
    else
        GPIOB->ODR |= (1 << 7);    // Tribord
}


void USART3_SendChar(char c)
{
    while(!(USART3->SR & USART_SR_TXE)); // Wait until TX buffer empty
    USART3->DR = c;
}

void USART3_SendString(char *s)
{
    while(*s)
        USART3_SendChar(*s++);
}



void USART3_Init_RX_Interrupt(void)
{
    /* 1) Activation des horloges */
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;  // USART3 clock
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;    // GPIOB clock
    RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;    // AFIO clock

    /* 2) Configuration PB11 = USART3_RX (input floating) */
    GPIOB->CRH &= ~(0xF << ((11 - 8) * 4));
    GPIOB->CRH |=  (0x4 << ((11 - 8) * 4));  // CNF=01, MODE=00

		/* 3) PB10 = USART3_TX (Alternate Function Push-Pull, 50 MHz) */
    GPIOB->CRH &= ~(0xF << ((10 - 8) * 4));   
    GPIOB->CRH |=  (0xB << ((10 - 8) * 4));   // CNF=10 (AF PP), MODE=11 (50 MHz)
	
	
    /* 3) Configuration USART3 */
    USART3->CR1 = 0;            
    USART3->BRR = 0x0EA6;       // 9600 bauds @ 36 MHz PCLK1

    USART3->CR1 |= USART_CR1_RE;        // Reception enable
	  USART3->CR1 |= USART_CR1_TE;   // Enable TX

    USART3->CR1 |= USART_CR1_RXNEIE;    // Interrupt enable
    USART3->CR1 |= USART_CR1_UE;        // USART enable

    /* 4) Active interruption dans NVIC */
    NVIC_EnableIRQ(USART3_IRQn);
}





