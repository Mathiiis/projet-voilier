#include "stm32f10x.h"
#include "Alimentation.h"
#include "../Driver2/Pilote_GPIO.h"
#include "../Driver2/MyTimer.h"
#include "../Driver1/ADC.h"
#include "../Driver1/USART.h"

void ADC1_Init_PA0_Interrupt(void)
{
    /* 1) Activation horloges */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   // GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;   // ADC1

    /* 2) PA0 en mode analogique */
    GPIOA->CRL &= ~(0xF << (0 * 4));      // MODE=00, CNF=00

    /* 3) Configuration ADC */
    ADC1->CR2 |= ADC_CR2_ADON;            // ADC ON
    for(volatile int i=0; i<20000; i++);  // petit délai

    // Long sample time pour stabilité
    ADC1->SMPR2 |= ADC_SMPR2_SMP0_2;      // 55.5 cycles

    /* Séquence: une seule conversion, canal 0 */
    ADC1->SQR1 = 0;
    ADC1->SQR3 = 0;                        // PA0 = Channel 0

    /* 4) Calibration */
    ADC1->CR2 |= ADC_CR2_CAL;
    while (ADC1->CR2 & ADC_CR2_CAL);

    /* 5) Active interruption EOC */
    ADC1->CR1 |= ADC_CR1_EOCIE;

    /* 6) Active ADC interrupt dans NVIC */
    NVIC_EnableIRQ(ADC1_2_IRQn);

    /* 7) Lancer la première conversion */
    ADC1->CR2 |= ADC_CR2_ADON;    // Start
}






