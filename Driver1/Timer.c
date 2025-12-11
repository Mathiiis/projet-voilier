#include "stm32f10x.h"

#include "GPIO.h"
#include "Timer.h"

/////////////////////////////////
// Init Timer2 : pï¿½riode 500 ms //
/////////////////////////////////



void MyTimer_Base_Init( TIM_TypeDef *Timer , unsigned short ValARR , unsigned short ValPSC ){
	  // Activer horloge Timer
		if (Timer == TIM1){
			RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
		} else if (Timer == TIM2){
			RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
		} else if (Timer == TIM3){
			RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
		} else if (Timer == TIM4){
			RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
		}

    // Config PSC et ARR
    Timer->PSC = ValPSC;   
    Timer->ARR = ValARR;   
    Timer->CNT = 0;       // Remise ï¿½ zï¿½ro du compteur

    Timer->EGR |= TIM_EGR_UG; //???
}


// ðŸ”¹ DÃ©claration des pointeurs de fonctions (un par timer)
static void (*ITFunction_TIM1)(void) = 0; //0 pour le null et static juste precaution pour dire on l utilise juste dans ce fichier
static void (*ITFunction_TIM2)(void) = 0;
static void (*ITFunction_TIM3)(void) = 0;
static void (*ITFunction_TIM4)(void) = 0;


void MyTimer_ActiveIT(TIM_TypeDef *Timer, char Prio, void (*IT_function)(void))
{
    // 1ï¸âƒ£ Active lâ€™interruption "Update" (dÃ©bordement)
    Timer->DIER |= 1 << 0;

    // 2ï¸âƒ£ Active lâ€™interruption dans le NVIC selon le timer
    if (Timer == TIM1) {
        NVIC->ISER[0] |= 1 << 25;
        NVIC->IPR[25] |= Prio << 4;
        ITFunction_TIM1 = IT_function; // Sauvegarde du pointeur
    }
    else if (Timer == TIM2) {
        NVIC->ISER[0] |= 1 << 28;
        NVIC->IPR[28] |= Prio << 4;
        ITFunction_TIM2 = IT_function;
    }
    else if (Timer == TIM3) {
        NVIC->ISER[0] |= 1 << 29;
        NVIC->IPR[29] |= Prio << 4;
        ITFunction_TIM3 = IT_function;
    }
    else if (Timer == TIM4) {
        NVIC->ISER[0] |= 1 << 30;
        NVIC->IPR[30] |= Prio << 4;
        ITFunction_TIM4 = IT_function;
    }
}


void TIM1_UP_IRQHandler(void)
{
    if (TIM1->SR & 1) {
        TIM1->SR &= ~1;              // Efface le flag UIF
        if (ITFunction_TIM1 != 0)    // VÃ©rifie que le pointeur est dÃ©fini
            ITFunction_TIM1();       // Appelle la fonction du main
    }
}

void TIM2_IRQHandler(void)
{
    if (TIM2->SR & 1) {
        TIM2->SR &= ~1;
        if (ITFunction_TIM2 != 0)
            ITFunction_TIM2();
    
		}
}

void TIM3_IRQHandler(void)
{
    if (TIM3->SR & 1) {
        TIM3->SR &= ~1;
        if (ITFunction_TIM3 != 0)
            ITFunction_TIM3();
    }
}

void TIM4_IRQHandler(void)
{
    if (TIM4->SR & 1) {
        TIM4->SR &= ~1;
        if (ITFunction_TIM4 != 0)
            ITFunction_TIM4();
    }
}


/**
 * @brief Configure et démarre une PWM sur TIM3_CH1 (PA6)
 * @param ARR : valeur de période
 * @param PSC : prescaler
 * @param DutyCycle : rapport cyclique entre 0.0 et 1.0
 */

void MyTimer_PWM_TIM3_PA6(unsigned short ARR, unsigned short PSC, float DutyCycle)
{
    //  Active les horloges GPIOA et TIM3
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   // GPIOA
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;   // TIM3

    // Configure PA6 en mode Alternate Function Push-Pull
    GPIOA->CRL &= ~(0xF << (6 * 4));      // Efface la config du pin 6
    GPIOA->CRL |= (0xB << (6 * 4));       // Sortie AF Push-Pull, 50 MHz

    //  Configure la base du timer (période et prescaler)
    TIM3->PSC = PSC;
    TIM3->ARR = ARR;
    TIM3->CNT = 0;

    //  Configure le canal 1 en mode PWM1
    TIM3->CCMR1 &= ~(0xFF);
    TIM3->CCMR1 |= (6 << 4);   // OC1M = 110  PWM1
    TIM3->CCMR1 |= (1 << 3);   // OC1PE = 1  preload activé
    TIM3->CCER  |= (1 << 0);   // CC1E = 1  sortie activée

    //  Définit le rapport cyclique
    TIM3->CCR1 = (unsigned short)(ARR * DutyCycle);

    //  Active ARPE + charge les registres
    TIM3->CR1 |= (1 << 7);   // ARPE = 1
    TIM3->EGR |= 1;          // UG = 1

    // 7?? Démarre le timer
    TIM3->CR1 |= TIM_CR1_CEN;
}



void MyTimer_PWM_Init(GPIO_TypeDef* GPIOx, uint8_t Pin,
                      TIM_TypeDef* TIMx, uint8_t Channel,
                      unsigned short ARR, unsigned short PSC)
{
    // 1. Activer horloge GPIO
    if (GPIOx == GPIOA) RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    if (GPIOx == GPIOB) RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    if (GPIOx == GPIOC) RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    // 2. Activer horloge Timer
    if (TIMx == TIM1) RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    if (TIMx == TIM2) RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    if (TIMx == TIM3) RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    if (TIMx == TIM4) RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

    // 3. Configurer le pin GPIO en AF Push-Pull 50 MHz
    if (Pin < 8) {
        GPIOx->CRL &= ~(0xF << (Pin * 4));
        GPIOx->CRL |= (0xB << (Pin * 4));
    } else {
        GPIOx->CRH &= ~(0xF << ((Pin - 8) * 4));
        GPIOx->CRH |= (0xB << ((Pin - 8) * 4));
    }

    // 4. Configurer base timer
    TIMx->PSC = PSC;
    TIMx->ARR = ARR;
    TIMx->CNT = 0;

    // 5. Configurer le canal en PWM1
    switch(Channel)
    {
        case 1:
            TIMx->CCMR1 &= ~(0xFF);
            TIMx->CCMR1 |= (6 << 4);  // OC1M = 110 PWM1
            TIMx->CCMR1 |= (1 << 3);  // OC1PE = 1
            TIMx->CCER |= (1 << 0);   // CC1E
            break;
        case 2:
            TIMx->CCMR1 &= ~(0xFF00);
            TIMx->CCMR1 |= (6 << 12); // OC2M = 110 PWM1
            TIMx->CCMR1 |= (1 << 11); // OC2PE
            TIMx->CCER |= (1 << 4);   // CC2E
            break;
        case 3:
            TIMx->CCMR2 &= ~(0xFF);
            TIMx->CCMR2 |= (6 << 4);
            TIMx->CCMR2 |= (1 << 3);
            TIMx->CCER |= (1 << 8);
            break;
        case 4:
            TIMx->CCMR2 &= ~(0xFF00);
            TIMx->CCMR2 |= (6 << 12);
            TIMx->CCMR2 |= (1 << 11);
            TIMx->CCER |= (1 << 12);
            break;
    }

    // 6. ARPE + chargement registre
    TIMx->CR1 |= TIM_CR1_ARPE;
    TIMx->EGR |= TIM_EGR_UG;

    // 7. D marrer le timer
    TIMx->CR1 |= TIM_CR1_CEN;
}


void MyTimer_PWM_SetDutyCycle(TIM_TypeDef* TIMx, uint8_t Channel, float DutyCycle)
{
    // DutyCycle : 0.0 ? 1.0
    unsigned short ccr = (unsigned short)(TIMx->ARR * (DutyCycle/100));

    switch(Channel)
    {
        case 1: TIMx->CCR1 = ccr; break;
        case 2: TIMx->CCR2 = ccr; break;
        case 3: TIMx->CCR3 = ccr; break;
        case 4: TIMx->CCR4 = ccr; break;
    }
}




