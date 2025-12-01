#include "GPIO.h"
#include "stm32f10x.h"
#include "ADC.h"

int valeur;

void MyADC1_Config_PA0(void)
{
    // 1) Horloges GPIOA et ADC1
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   // enable GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;   // enable ADC1

    // 2) PA0 en mode analogique (MODE0=00, CNF0=00)
    GPIOA->CRL &= ~(0xF << (0 * 4));

    // 4) Power-on
    ADC1->CR2 |= ADC_CR2_ADON;            // ADON=1 (power on)
   

    // 5) Séquence: 1 conversion, canal 0 en SQ1
    ADC1->SQR1 = 0;                       // L[3:0]=0 -> 1 conv.
    ADC1->SQR3 = 0;                       // SQ1[4:0]=0 -> CH0

 
	RCC->CFGR |=  RCC_CFGR_ADCPRE_DIV6;   // ADCCLK = PCLK2/6 -> 72/6 = 12 MHz
	
    // 7) Déclenchement par trigger externe = SWSTART
    //    - EXTSEL[2:0] = 111 (SWSTART)
    //    - EXTTRIG = 1 (autorise le déclenchement externe)
    ADC1->CR2 &= ~ADC_CR2_EXTSEL;         // efface EXTSEL[2:0]
    ADC1->CR2 |=  (0x7 << 17);            // EXTSEL=111b (SWSTART)
    ADC1->CR2 |=  ADC_CR2_EXTTRIG;        // EXTTRIG=1


	 // 8) configurer les IT
    // >>> pour activer interruption EOC :
    ADC1->CR1 |= ADC_CR1_EOCIE;          // enable IT fin conversion
    NVIC_EnableIRQ(ADC1_2_IRQn);         // activer dans le NVIC
    NVIC_SetPriority(ADC1_2_IRQn, 5);    // priorité optionnelle

}

	
	
void ADC1_2_IRQHandler(void)
{
    // Vérifie si le flag EOC (End Of Conversion) est levé
    if (ADC1->SR & ADC_SR_EOC)
    {
        // Lecture du résultat (12 bits)
        valeur = (unsigned short)ADC1->DR;

        // ?? La lecture de DR efface automatiquement le flag EOC.
        // ? Ne pas le remettre à 0 manuellement !
    }
	
}


	/*
unsigned short MyADC1_Read_PA0_SWSTART(void){
	 // 8) Démarre la conversion régulière en SW
    ADC1->CR2 |= ADC_CR2_SWSTART;         // SWSTART=1
    // 9) Attend la fin de conversion (EOC)
   // while (((ADC1->SR & ADC_SR_EOC) != 1<<1));     // EOC=1 -> done  methode primale

    // 10) Lit le résultat (12 bits)
    unsigned short value = (unsigned short)ADC1->DR;
    return value;
}*/

void MyADC_Init(ADC_TypeDef * ADC, int ADCPrescaler) {
	
	RCC->CFGR |= ((ADCPrescaler>>2)<<14) ; // Allume ADC prescaler + mettre en &= pour ecraser la valeur deja presente
	
	ADC->CR1 |= 0x01<<5 ; // Allume l'interruption de fin de conversion
	
	ADC->CR2 |= 0x01<<0 ; // ADON -> Active l'ADC puis reecrire un 1 dedans fait commencer la conversion
	ADC->CR2 |= 0x01<<20 ; // Active le trigger de l'ADC en external
	ADC->CR2 |= 0x7<<17 ; // 111 -> Le trigger de l'ADC est le software
	
	NVIC->ISER[0] |= 0x1<<18 ; //Active l'interruption ADC aux yeux du processeur
}

float MyADC_GetConversion() {
	return valeur ;
}

void MyADC_StartConvert() {
	ADC1->CR2 |= 0x01<<22 ; //Debut de la conversion SWSTART
}

