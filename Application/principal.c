#include "stm32f10x.h"
#include "stdio.h"                 

#include "../Driver1/GPIO.h"
#include "../Driver1/Timer.h"
#include "../Driver1/ADC.h"
#include "../Driver1/USART.h"
#include "../Periph/Servo.h"
#include "../Periph/Girouette.h"
#include "../Periph/Alimentation.h"

extern int valeur;
char toto ;
int angle;
float DC;


void System_Clock_Init(void)
{
    // Déjà programmé par défaut, mais propre à rappeler :
    // SYSCLK = 72 MHz via HSE+PLL

    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;   // APB1 = 36 MHz
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;   // APB2 = 72 MHz

    RCC->CFGR |= RCC_CFGR_ADCPRE_DIV6;  // ADC = 12 MHz
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
    System_Clock_Init();

    // --- Initialisation des modules ---
    MyUSART_Init(USART2);
    MyUSART_SetBaudRate(USART2, 9600);

    Alimentation_Init();
    Girouette_Init();
    MyServoInit();
    

    TIM3_Message_Init();  // messages toutes les 3 secondes

    USART2_SendString("Systeme Voilier 2.0 - READY\n");

    while(1)
    {
        // --- Mesure vent ---
        int AngleVent = GirouetteGetAngle();
        float DC = alpha_to_DC(AngleVent);
				SetDC(DC);

        // --- Commande du plateau depuis télécommande ---
        //int cmd = GetRemoteCommand(); 
        //Plateau_SetSpeed(cmd);

        // --- Sécurités ---
        //Check_Roulis();
        //Check_Batterie();
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
}	