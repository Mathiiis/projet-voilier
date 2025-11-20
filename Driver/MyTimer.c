#include "stm32f10x.h"
#include "MyTimer.h"

void (* maFonc2) (void) ;

void MyTimer_Base_Init ( TIM_TypeDef * Timer , unsigned short ValARR , unsigned short ValPSC) {
	Timer->ARR = ValARR ; 
	Timer->PSC = ValPSC ;
}



void MyTimer_ActiveIT( TIM_TypeDef * Timer , char Prio, void (*IT_function) (void)) {
	Timer->DIER |= (0x01 << 0) ; //Active l'interruption 
	NVIC->ISER[0] |= 1<<28 ; //Active l'interruption aux yeux du umicro
	NVIC->IPR[28] |= Prio<<4 ; //Donne la priorité de l'interruption
	if (Timer == TIM2) {
	maFonc2 = IT_function ;
	} 
}

void TIM2_IRQHandler(void) { // Fonction deja défini par le constructeur en [WEAK] (= peut etre surchagé comme ici)
		TIM2->SR &= 0xF<<1 ;
	if (maFonc2 != 0) (*maFonc2) () ; //Lance MaFonc2 lorsque SR est levé
	
}


void MyTimer_PWM( TIM_TypeDef * Timer , char Channel ) {
	
	Timer->CCER |= 0x1<<((Channel-1)*4); //Allume la Capture/Compare sur le bon channel
	
	if (Channel == 1 || Channel == 2) {
		Timer->CCMR1 |= 0x6<<(4+(Channel-1)*8) ; //110: PWM mode 1 - In upcounting, channel 1 is active as long as TIMx_CNT<TIMx_CCR1 else inactive
	} else {
		Timer->CCMR2 |= 0x6<<(4+(Channel-3)*8) ;
	}
	MyTimer_PWM_RC(Timer,50) ; // Valeur par défaut
}

void MyTimer_PWM_RC( TIM_TypeDef * Timer, int RC) {

	Timer->CCR1 = (RC * Timer->ARR) / 100 ; // Permet d'entrer la valeur de comparaison pour piloter le rapport cyclique avec ARR
}
