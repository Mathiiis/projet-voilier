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

/*
 * Parse une consigne reçue sur l'USART2 sous la forme d'un entier signé ASCII
 * ex: "-75" ou "40", terminée par \r ou \n.
 * Conserve la dernière consigne valide entre deux réceptions.
 */
static int8_t Telecommande_GetPlateauCmd(void)
{
    static char buf[8];
    static uint8_t idx = 0;
    static int8_t lastCmd = 0;

    if(USART2->SR & USART_SR_RXNE)
    {
        char c = USART2->DR;

        if(c == '\r' || c == '\n')
        {
            buf[idx] = '\0';
            idx = 0;

            if(buf[0] != '\0')
            {
                int val = atoi(buf);
                if(val > 100)  val = 100;
                if(val < -100) val = -100;
                lastCmd = (int8_t)val;
            }
        }
        else if(idx < sizeof(buf) - 1)
        {
            buf[idx++] = c;
        }
        else
        {
            idx = 0; // overflow -> reset buffer
        }
    }

    return lastCmd;
}


void Send_Telemetry(void)
{
    float Vbat = GetBatteryVoltage();
    int AngleVent = GirouetteGetAngle();

    char msg[128];
    sprintf(msg,
            "a=%3d  Vbat=%.1fV\n",
            AngleVent, Vbat);

    USART2_SendString(msg);
}


void TIM3_Message_Init(void)
{
    MyTimer_Base_Init(TIM3, 30000, 7200-1);
    MyTimer_ActiveIT(TIM3, 2, Send_Telemetry);
    MyTimer_Base_Start(TIM3);
}


int main(void)
{

    Alimentation_Init();
    Girouette_Init();
    MyServoInit();
    InitPlateau();

    // Initialisation de la télécommande (config USART2, etc.)
    Telecommande_Init();



    //TIM3_Message_Init();  // messages toutes les 3 secondes

    USART2_SendString("Systeme Voilier 2.0 - READY\n");

    while(1)
    {
        // --- Mesure vent / asservissement voile ---
        AngleVent = GirouetteGetAngle();
        float DC = alpha_to_DC(AngleVent);
        SetDC(DC);

        // --- Lecture de la consigne plateau via télécommande ---
        Telecommande_GetDirectionVitesse(&sens, &vitesse);

        // Sécurité : clamp de la vitesse 0..100
        if (vitesse < 0) vitesse = 0;
        if (vitesse > 100) vitesse = 100;

        // Gestion sens + vitesse
        if (sens < 0)
        {
            // Bâbord : par ex. PB7 à 0
            ResetBroche(GPIOB, 7);
            SetPlateau((float)vitesse);   // PWM proportionnelle
        }
        else if (sens > 0)
        {
            // Tribord : par ex. PB7 à 1
            SetBroche(GPIOB, 7);
            SetPlateau((float)vitesse);
        }
        else
        {
            // Stop
            SetPlateau(0.0f);
        }
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
