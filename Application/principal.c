#include "stm32f10x.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

#include "../Driver1/GPIO.h"
#include "../Driver1/Timer.h"
#include "../Driver1/ADC.h"
#include "../Driver1/USART.h"
#include "../Periph/Servo.h"
#include "../Periph/Girouette.h"
#include "../Periph/Alimentation.h"
#include "../Periph/Plateau.h"
#include "../Periph/Telecommande.h"

extern int valeur;
extern int ch;
char toto ;
int AngleVent;
float DC;
char sens = 0;      // -1 = bâbord, 0 = stop, +1 = tribord (convention driver télécommande)
char vitesse = 0;   // 0..100 %
 int lastCommand;
int battery_raw;
/*
 * Parse une consigne reçue sur l'USART2 sous la forme d'un entier signé ASCII
 * ex: "-75" ou "40", terminée par \r ou \n.
 * Conserve la dernière consigne valide entre deux réceptions.
 */
 

void CheckBatteryAndSend(void)
{
	
		float bat = GetBatteryVoltage();
    if (bat < 10.0f)
    {
        USART3_SendString("Batterie faible\r\n");
    }
    else
    {
        USART3_SendString("Batterie OK\r\n");
    }
}
void ADC1_2_IRQHandler(void)
{
    if (ADC1->SR & ADC_SR_EOC)
    {
        battery_raw = ADC1->DR;  // Lire efface EOC
        //ADC1->CR2 |= ADC_CR2_ADON;   // Relance une conversion
    }
}

float GetBatteryVoltage(void)
{
    float vADC = (battery_raw * 3.3f) / 4095.0f;
    float vBAT = vADC * 13.0f;   // pont diviseur 1/13
    return vBAT;
}

void SetPlateauCommand(void)
{
    float DC;

    if (lastCommand >= 0)
    {
        SetPlateauSens(1);   // Tribord
        DC = lastCommand;
    }
    else
    {
        SetPlateauSens(0);   // Babord
        DC = -lastCommand;           // Valeur positive
    }
	
    SetPlateau(DC);
}


/*
void Plateau_ReceiveAndUpdate(void){
    int8_t commande = lastCommand;
    float DC;

    if (commande >= 0){
        // Tribord ? PB5 = 1
        GPIOB->ODR |= (1 << 5);
        DC = commande;
    }
    else {
        // Babord ? PB5 = 0
        GPIOB->ODR &= ~(1 << 5);
        DC = -commande;
    }

    SetPlateau(DC);
}*/


void USART3_IRQHandler(void)
{
    // RXNE = un octet est arrivé ?
    if (USART3->SR & USART_SR_RXNE)
    {
        lastCommand = (int8_t)USART3->DR;  // Lire efface RXNE
    }
}

int main(void)
{

    Girouette_Init();
    MyServoInit();
    InitPlateau();
    USART3_Init_RX_Interrupt();
	ADC1_Init_PA0_Interrupt();


    // Initialisation de la télécommande (config USART2, etc.)
    //Telecommande_Init();

    //TIM3_Message_Init();  // messages toutes les 3 secondes


    while(1)
    {
        // --- Mesure vent / asservissement voile ---
        AngleVent = GirouetteGetAngle();
        float DC = alpha_to_DC(AngleVent);
        SetDC(DC);
				
				//SetPlateau(100);
				SetPlateauCommand();
			CheckBatteryAndSend();
			   ADC1->CR2 |= ADC_CR2_ADON;   // Relance une conversion

    }
}

/*
int main ( void )
{
	
	Girouette_Init();
	MyServoInit();
	Alimentation_Init() ;

	while(1){
		angle = GirouetteGetAngle();
		DC = alpha_to_DC(angle);
		SetDC(DC);
	} 
	//RCC->APB2ENR |= (1<<4) | RCC_APB2ENR_IOPAEN ;
   MyTimer_Base_Init(TIM2, 4999, 14399);	


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

	
	// Génère une PWM à 10 kHz avec 50 % de rapport cyclique sur PA6
  // f_PWM = 72 MHz / ((PSC+1) * (ARR+1))
  // Par ex : PSC = 71, ARR = 99 ? 10 kHz
  // MyTimer_PWM_TIM3_PA6(4999, 14399, 0.2f);
	

	


	
	while (1)
	{
				//ADC1->CR2 |= ADC_CR2_SWSTART;   // bit 22 = 1, auto-clear par le HW au démarrage

	}



 // CONFIGURATION MANUELLE DES BROCHES
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
