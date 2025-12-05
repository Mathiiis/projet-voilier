#include "stm32f10x.h"
#include "Alimentation.h"
#include "../Driver2/Pilote_GPIO.h"
#include "../Driver2/MyTimer.h"
#include "../Driver2/MyADC.h"
#include "../Driver1/USART.h"

float GetBatteryVoltage(void)
{
	MyADC_StartConvert();
	float adc = MyADC_GetConversion();
	return adc * 3.3f / 4095.0f * 13.0f; // pont diviseur (47k/3.9k)
}

void Send_Warning(void) {
	float Vbat = GetBatteryVoltage();

	if (Vbat < 10.0f)
			USART2_SendChar('F');
	else
			USART2_SendChar('T');
	
}

void Alimentation_Init() {
	
	RCC->APB2ENR |= (1<<4) | RCC_APB2ENR_IOPAEN ;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN ;

	
	//USART
	InitGPIO(GPIOA,2,OUTPUTALTERNATEPUSHPULL) ;
	InitGPIO(GPIOA,3,INPUTFLOATING) ;
	
	MyTimer_Base_Init(TIM2,35999,199) ; // Formule (1/f_clk_rcc[=souvent 72 MHz]) * (ARR+1) * (PSC+1) = f_horloge_voulue
	MyTimer_ActiveIT(TIM2,3,Send_Warning) ;
	
	MyUSART_Init(USART2);
	MyUSART_SetBaudRate(USART2,9600) ;
	MyUSART_ActiveIT(USART2) ;
	
	//ADC
	InitGPIO(GPIOA,0,INPUTANALOG) ; 
	
	MyADC_Init(ADC1,8);
		
	MyTimer_Base_Start(TIM2) ; // Allume le Timer a la fin pour faire des send apres avoir config l'USART	
}
