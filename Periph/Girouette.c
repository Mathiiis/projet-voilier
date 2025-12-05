#include "stm32f10x.h"

#include "Girouette.h"
#include "../Driver1/GPIO.h"
#include "../Driver2/MyTimer.h"


void Girouette_Init() {
    
    // Active les horloges
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN ;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN ; // << OBLIGATOIRE
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN ;

    // A = PA8 (CH1)
    InitGPIO(GPIOA, 8, INPUTFLOATING);

    // B = PA9 (CH2)
    InitGPIO(GPIOA, 9, INPUTFLOATING);

    // I = PB15 (index, pas utilisé ici)
    InitGPIO(GPIOB, 10, INPUTFLOATING);

    // Encoder mode 3 (compte sur TI1 et TI2)
    TIM1->SMCR = TIM_SMCR_SMS_0 | TIM_SMCR_SMS_1;

    // CH1 et CH2 en mode entrée (TI1 + TI2)
    TIM1->CCMR1 = (1 << 0) | (1 << 8);

    // Active CH1 et CH2 (OBLIGATOIRE)
    TIM1->CCER = TIM_CCER_CC1E | TIM_CCER_CC2E;

    // Remise à zéro du compteur
    TIM1->CNT = 0;
		
		//
		TIM1->ARR = 1440 ;

    // Démarre le timer
    TIM1->CR1 |= TIM_CR1_CEN;
}

int GirouetteGetAngle() {
    return TIM1->CNT / 4;  // 1440 ticks ? 360°
}
