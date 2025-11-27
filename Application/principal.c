#include "stm32f10x.h"
#include "GPIO.h"
#include "MyTimer.h"
#include "ADC.h"
#include "USART.h"
#include "Servo.h"


int watched_value;

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

void Emission(void)
{
	//ADC1->CR2 |= ADC_CR2_SWSTART;   // bit 22 = 1, auto-clear par le HW au d�marrage c est pour l adc
	USART2->DR= 'a';
}

extern int valeur;


char toto ;
void USART2_IRQHandler(void)
{
	toto = USART2->DR ;
}


int main ( void )
{
	
	Girouette_Init() ;

	MyServoInit();
	//RCC->APB2ENR |= (1<<4) | RCC_APB2ENR_IOPAEN ;
  	/* MyTimer_Base_Init(TIM2, 4999, 14399);	
	

	//InitTimer2();
	//watched_value = TIM2->CNT;

	
	//InitGPIO( GPIOA, 0, OUTPUTPUSHPULL );
	//InitGPIO( GPIOC, 0, INPOUTFLOATING );

	//MyADC1_Config_PA0();

	InitGPIO(GPIOA, 2, OUTPUTPUSHPULL);
	InitGPIO(GPIOA, 3, INPOUTFLOATING);

	
  	MyTimer_ActiveIT(TIM2, 3, Emission);
	
  
	USART2_Init();
	
	TIM2->CR1 |= TIM_CR1_CEN;//faut pas publier cette ligne!!!!!!!
*/
	
	// G�n�re une PWM � 10 kHz avec 50 % de rapport cyclique sur PA6
	// f_PWM = 72 MHz / ((PSC+1) * (ARR+1))
	// Par ex : PSC = 71, ARR = 99 ? 10 kHz
	// MyTimer_PWM_TIM3_PA6(4999, 14399, 0.2f);
	
	while (1)
	{
		//ADC1->CR2 |= ADC_CR2_SWSTART;   // bit 22 = 1, auto-clear par le HW au d�marrage
	}
}


/*// CONFIGURATION MANUELLE DES BROCHES
// 
//  GPIOA pin 0 : push-pull output 2 MHz
	GPIOA->CRL &= ~0xF;
	GPIOA->CRL |= 0x2;

	// GPIOC pin 0 : input pull-up
	GPIOC->CRL &= ~0xF;
	GPIOC->CRL |= 0x8;
	GPIOC->ODR |= 1 << 0;  // pull-up
	code pour push pull et pull up*/
	
	/*//foating et ...
	GPIOA->CRL &= ~0xF; //pushpull
	GPIOA->CRL |= 0x2;
	
	GPIOC->CRL &= ~0xF; //floating
	GPIOC->CRL |= 0x4;
	*/
	