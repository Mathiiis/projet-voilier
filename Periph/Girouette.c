#include "stm32f10x.h"
#include "Girouette.h"
#include "../Driver/Pilote_GPIO.h"
#include "../Driver/MyTimer.h"
#include "../Driver/MyADC.h"
#include "../Driver/MyUSART.h"


void Girouette_Init() {
	
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN ;
	RCC->APB2ENR |= RCC_APB2ENR_IOPBEN ;

	InitGPIO(GPIOB,13,INPUTFLOATING) ;
	InitGPIO(GPIOB,14,INPUTFLOATING) ;
	InitGPIO(GPIOB,15,INPUTFLOATING) ;
	
	MyTimer_Base_Init(TIM1,1440,0) ;
	
	TIM1->CCER |= 0x01<<0 ; //Enable Channel 1
	TIM1->CCER |= 0x01<<4 ; //Enable Channel 2
	
	// Sequence d'instruction pour realiser un compteur en fonction de signaux en quadrature
	
	TIM1->CCMR1 |= 0x01<<0 ; //CC1 channel is configured as input, IC1 is mapped on TI1
	TIM1->CCMR1 |= 0x01<<8 ; //CC2 channel is configured as input, IC2 is mapped on TI2
	
	
	TIM1->CCER |= 0x00<<1 ;
	TIM1->CCER |= 0x00<<3 ;
	TIM1->CCMR1 |= 0x0000<<4 ;
	
	TIM1->CCER |= 0x00<<5 ;
	TIM1->CCER |= 0x00<<7 ;
	TIM1->CCMR1 |= 0x00<<12 ;
	
	TIM1->SMCR |= 0x011 ;
	
	while (!LireBroche(GPIOB,15)) {} //Attends le disque IDX pour mettre l'angle a 0
	MyTimer_Base_Start(TIM1) ;
}

int GirouetteGetAngle() {
		return (TIM1->CNT)/4;
}