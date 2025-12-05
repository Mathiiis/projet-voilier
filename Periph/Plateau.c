#include "Plateau.h"
#include "../Driver2/Pilote_GPIO.h"

// Choix hardware : PWM sur PB6 (TIM4_CH1) + 2 broches de sens PB7 / PB8.
// Ajuster ici si le câblage est différent.
#define PLATEAU_PWM_PORT      GPIOB
#define PLATEAU_PWM_PIN       6      // TIM4_CH1
#define PLATEAU_DIR_PORT      GPIOB
#define PLATEAU_DIR_PIN_A     7      // Sens A
#define PLATEAU_DIR_PIN_B     8      // Sens B

#define PLATEAU_TIMER         TIM4
#define PWM_BASE_FREQ_HZ      20000U  // PWM silencieuse pour moteur DC
#define TIMER_CLK_HZ          72000000U

static int8_t s_plateauSpeed = 0;  // consigne actuelle en pourcentage

static void Plateau_ApplyDirection(int8_t speed)
{
    if(speed > 0)
    {
        SetBroche(PLATEAU_DIR_PORT, PLATEAU_DIR_PIN_A);
        ResetBroche(PLATEAU_DIR_PORT, PLATEAU_DIR_PIN_B);
    }
    else if(speed < 0)
    {
        ResetBroche(PLATEAU_DIR_PORT, PLATEAU_DIR_PIN_A);
        SetBroche(PLATEAU_DIR_PORT, PLATEAU_DIR_PIN_B);
    }
    else
    {
        // Arrêt libre
        ResetBroche(PLATEAU_DIR_PORT, PLATEAU_DIR_PIN_A);
        ResetBroche(PLATEAU_DIR_PORT, PLATEAU_DIR_PIN_B);
    }
}

static void Plateau_ApplyDuty(uint8_t magnitudePercent)
{
    uint32_t arr = PLATEAU_TIMER->ARR;
    uint32_t ccr = ((uint32_t)(arr + 1) * magnitudePercent) / 100U;
    if(ccr > arr) ccr = arr;
    PLATEAU_TIMER->CCR1 = (uint16_t)ccr;
}

void Plateau_Init(void)
{
    // Horloges : GPIOB + TIM4
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;

    // GPIO sens (push-pull) et PWM (AF push-pull)
    InitGPIO(PLATEAU_DIR_PORT, PLATEAU_DIR_PIN_A, OUTPUTPUSHPULL);
    InitGPIO(PLATEAU_DIR_PORT, PLATEAU_DIR_PIN_B, OUTPUTPUSHPULL);
    InitGPIO(PLATEAU_PWM_PORT, PLATEAU_PWM_PIN, OUTPUTALTERNATEPUSHPULL);

    // Base de temps : 72 MHz -> 20 kHz PWM (PSC=71 ; ARR=49)
    PLATEAU_TIMER->PSC = 71;
    PLATEAU_TIMER->ARR = (TIMER_CLK_HZ / ((PLATEAU_TIMER->PSC + 1) * PWM_BASE_FREQ_HZ)) - 1;
    PLATEAU_TIMER->CNT = 0;

    // PWM mode 1 sur CH1
    PLATEAU_TIMER->CCMR1 &= ~(0xFF);
    PLATEAU_TIMER->CCMR1 |= (6 << 4) | (1 << 3); // OC1M=110 (PWM1), OC1PE=1
    PLATEAU_TIMER->CCER  |= TIM_CCER_CC1E;       // sortie active

    // Précharge + génération immédiate
    PLATEAU_TIMER->CR1 |= TIM_CR1_ARPE;
    PLATEAU_TIMER->EGR |= 1;

    Plateau_SetSpeed(0);  // arrêt initial

    PLATEAU_TIMER->CR1 |= TIM_CR1_CEN;
}

void Plateau_SetSpeed(int8_t percent)
{
    if(percent > 100)  percent = 100;
    if(percent < -100) percent = -100;

    s_plateauSpeed = percent;

    uint8_t magnitude = (percent < 0) ? (uint8_t)(-percent) : (uint8_t)percent;
    Plateau_ApplyDirection(percent);
    Plateau_ApplyDuty(magnitude);
}

int8_t Plateau_GetSpeed(void)
{
    return s_plateauSpeed;
}
