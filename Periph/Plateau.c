#include "Plateau.h"
#include "../Driver2/Pilote_GPIO.h"
#include "../Driver1/Timer.h"


void InitPlateau(void){
    MyTimer_PWM_Init(GPIOB, 6, TIM4, 1, 3599, 0);
	  InitGPIO(GPIOB, 7, OUTPUTPUSHPULL);
}

void SetPlateau(float DC){
	MyTimer_PWM_SetDutyCycle(TIM4, 1, DC);
}



