#include "stm32f10x.h"
#include "../Driver/Pilote_GPIO.h"
#include "../Driver/MyTimer.h"
#include "../Driver/MyADC.h"
#include "../Driver/MyUSART.h"

/////////////////////////////////
//  fonction 'pas top top'     //
//  d'attente d'environ 250 ms //
/////////////////////////////////
void Attente (void)
{
	for (int i = 0 ; i < 1000000 ; i++){}
}
/////////////////////////////////

/// GPIO
void Toggle() {
		if (LireBroche(GPIOC,8)) {
			ResetBroche(GPIOC,8);
		} else {
			SetBroche(GPIOC,8);
		}
}


/// Timer
void scrutatingUIF() {
		if ((TIM2->SR & 0x01<<0) == 1) { // Dans SR on regarde le registre UIF
			Toggle() ;
			TIM2->SR &= 0xF<<1 ;
		}
}

/// USART
void putChar(char c) {
	USART2->DR = c ;
	//Attention gerer le flag TXE
	/*while (USART2->SR&(1<<7) == (1<<7)) {}*/
}

void send() {
	putChar('a') ;
	
}

int main ( void ) {
	
	/*
	/// RCC => Allumer les périph 
	RCC->APB2ENR |= (1<<4) | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPDEN ;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN ; // Allume le Timer 2
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN ;
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN ;
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN ;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN ;
	
	/// USART
	
	InitGPIO(GPIOA,2,OUTPUTALTERNATEPUSHPULL) ;
	InitGPIO(GPIOA,3,INPUTFLOATING) ;
	
	MyTimer_Base_Init(TIM2,35999,199) ; // Formule (1/f_clk_rcc[=souvent 72 MHz]) * (ARR+1) * (PSC+1) = f_horloge_voulue
	MyTimer_ActiveIT(TIM2,3,send) ;
	
	MyUSART_Init(USART2);
	MyUSART_SetBaudRate(USART2,9600) ;
	MyUSART_ActiveIT(USART2) ;
	
	MyTimer_Base_Start(TIM2) ; // Allume le Timer a la fin pour faire des send apres avoir config l'USART
	
	
	
	/// ADC
	
	InitGPIO(GPIOA,0,0) ; // En input, analog input
	
	MyADC_Init(ADC1,8) ;
	
	MyTimer_Base_Init(TIM2,35999,199) ; 
	MyTimer_ActiveIT(TIM2,3,MyADC_StartConvert) ;
	
	
	InitGPIO(GPIOC,8,2) ;
	MyTimer_Base_Init(TIM2,35999,999) ; // Initialise le fréquence de l'horloge a 0,5Hz
	MyTimer_ActiveIT(TIM2,3,Toggle) ; //Active l'interruption et la rend acceptable pour le processeur
	
	/// Timer avec PWM
	
	//On va utiliser de nouveaux timers et GPIO pour avoir faire du PWM mode
	
	InitGPIO(GPIOA,6,10) ;// A mettre en alternate output push pull (sortie plus dépendante de ODR) et voir quelle pin est map avec le TIM3_Channel qui est disponible pour le PWM mode
	MyTimer_Base_Init(TIM3,11999,119) ;// Faire clignoter a 50Hz 
	MyTimer_PWM(TIM3,1) ; // Init le PWM et map le tout
	
	int RC = 60 ;
	MyTimer_PWM_RC(TIM3,RC) ;
	*/
	
	
	while (1) {
	}
}
