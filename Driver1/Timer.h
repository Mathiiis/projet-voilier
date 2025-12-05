#ifndef __piloteTimer
#define __piloteTimer

#include "stm32f10x.h"
//void InitTimer2(void);

/*
*****************************************************************************************
* @brie f
* @param -> Param� t r e sous forme d � une s t r u c t u r e ( son a d re s se ) con ten an t l e s
i n f o rm a ti o n s de base
* @Note -> F onc ti on � l a n c e r s y s t �matiquement avan t d � a l l e r pl u s en d� t a i l dans l e s
con f pl u s f i n e s (PWM, codeur i n c . . . )
*************************************************************************************************
*/

void MyTimer_ActiveIT(TIM_TypeDef *Timer, char Prio, void (*IT_function)(void));
void MyTimer_PWM_TIM3_PA6(unsigned short ARR, unsigned short PSC, float DutyCycle);

void MyTimer_Base_Init( TIM_TypeDef *Timer , unsigned short ValARR , unsigned short ValPSC ) ;
#define MyTimer_Base_Start( Timer ) ( Timer->CR1 |= TIM_CR1_CEN );
#define MyTimer_Base_Stop( Timer ) ( Timer -> =CR1 &= ~TIM_CR1_CEN);
void MyTimer_PWM_Init(GPIO_TypeDef* GPIOx, uint8_t Pin,
                      TIM_TypeDef* TIMx, uint8_t Channel,
                      unsigned short ARR, unsigned short PSC);


void MyTimer_PWM_SetDutyCycle(TIM_TypeDef* TIMx, uint8_t Channel, float DutyCycle);

#endif
