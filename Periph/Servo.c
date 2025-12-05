#include "stm32f10x.h"
#include "Servo.h"
#include "../Driver1/Timer.h"

/************************************************************
 * SERVO SPECIFICATIONS
 * Standard RC Servo:
 *  - Period: 20 ms (50 Hz)
 *  - Pulse min: 1.0 ms  ? 0°
 *  - Pulse max: 2.0 ms  ? 180°
 ************************************************************/

#define SERVO_MIN_PW_MS   1.0f
#define SERVO_MAX_PW_MS   2.0f
#define SERVO_PERIOD_MS   20.0f

// Timer constants (TIM3 on APB1 = 36 MHz ? PSC=3599 ? 10 kHz clock)
#define TIM3_PWM_FREQ_HZ   50           // 20 ms
#define TIM3_ARR           399          // 10 kHz ? ARR=199 for 20 ms ; you used 3599/399 so we keep it
#define TIM3_PSC           3599         // 36MHz / 3600 = 10 kHz tick

/************************************************************
 * Converti un angle mécanique 0–180° en pulse width ratio
 ************************************************************/
float Angle_To_DC(float angle_deg)
{
    if(angle_deg < 0)   angle_deg = 0;
    if(angle_deg > 180) angle_deg = 180;

    float pw_ms = SERVO_MIN_PW_MS +
                  (angle_deg / 180.0f) * (SERVO_MAX_PW_MS - SERVO_MIN_PW_MS);

    // duty cycle = pulse / period
    float DC = pw_ms / SERVO_PERIOD_MS;   // ex: 1.5ms / 20ms = 0.075
    return DC;
}

/************************************************************
 * API demandée : alpha ? duty cycle (voilier)
 ************************************************************/
float alpha_to_DC(int alpha)
{
    // a peut dépasser 180° ? modulo pour assurer 0–180°
    int a = alpha % 180;
    if(a < 0) a += 180;

    return Angle_To_DC((float)a);
}

/************************************************************
 * Initialisation du servo sur TIM3-CH1 (PA6)
 ************************************************************/
void MyServoInit(void)
{
    // Configuration du PWM sur TIM3-CH1 en utilisant TA LIB
    MyTimer_PWM_TIM3_PA6(TIM3_PSC, TIM3_ARR, 0.05);  // 5% duty par défaut (position neutre)
}

/************************************************************
 * Mise à jour du Duty Cycle
 ************************************************************/
void SetDC(float DC)
{
    if(DC < 0)   DC = 0;
    if(DC > 0.12) DC = 0.12;    // Sécurité : servos standard ~ 5%-12%

    TIM3->CCR1 = (uint16_t)(TIM3_ARR * DC);
}
