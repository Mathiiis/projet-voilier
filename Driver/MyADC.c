#include "stm32f10x.h"
#include "MyADC.h"

int n = 0 ;

void MyADC_Init(ADC_TypeDef * ADC, int ADCPrescaler) {
	
	RCC->CFGR |= ((ADCPrescaler>>2)<<14) ; // Allume ADC prescaler + mettre en &= pour ecraser la valeur deja presente
	
	ADC->CR1 |= 0x01<<5 ; // Allume l'interruption de fin de conversion
	
	ADC->CR2 |= 0x01<<0 ; // ADON -> Active l'ADC puis reecrire un 1 dedans fait commencer la conversion
	ADC->CR2 |= 0x01<<20 ; // Active le trigger de l'ADC en external
	ADC->CR2 |= 0x7<<17 ; // 111 -> Le trigger de l'ADC est le software
	
	NVIC->ISER[0] |= 0x1<<18 ; //Active l'interruption ADC aux yeux du processeur
}
void ADC1_2_IRQHandler(void) {
	n = ADC1->DR ;
}
void MyADC_StartConvert() {
	ADC1->CR2 |= 0x01<<22 ; //Debut de la conversion SWSTART
}