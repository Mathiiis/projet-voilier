#ifndef __myDriver_h
#define __myDriver_h
#include "stm32f10x.h"


/*
*****************************************************************************************
* @brief
* @param -> Paramè t r e sous forme d ’ une s t r u c t u r e ( son a d r e s s e ) c o n t e n a n t l e s
i n f o r m a t i o n s de base
* @Note -> F o n c t i o n à l a n c e r s y s t é matiquement avant d ’ a l l e r p l u s en dé t a i l dans l e s
c o n f p l u s f i n e s (PWM, codeur i n c . . . )
*************************************************************************************************
*/


void MyTimer_Base_Init ( TIM_TypeDef * Timer , unsigned short ValARR , unsigned short ValPSC) ;

#define MyTimer_Base_Start(Timer) (Timer->CR1 |= 0x01 << 0)

#define MyTimer_Base_Stop(Timer) (Timer->CR1 &= 0xF << 1)

/*
*************************************************************************************************
* @brief
* @param : - TIM_TypeDef * Timer : Timer concerne
- char P r i o : de 0 a 15
* @Note : La f o n c t i o n MyTimer_Base_Init d o i t a v o i r e t e l a n c e e au p r e a l a b l e
**************************************************************************************************
*/


void MyTimer_ActiveIT( TIM_TypeDef * Timer , char Prio, void (*IT_function) (void)) ;

/* *
* @brief
* @param . . . .
* @Note A c t i v e l e channel sp é c i f i é suTimerr l e t i m e r sp é c i f i é
* l a g e s t i o n de l a c o n f i g u r a t i o n I /O n ’ e s t pas f a i t e dans c e t t e f o n c t i o n
* n i l e r é g l a g e de l a pé r i o d e de l a PWM (ARR , PSC )
*/
void MyTimer_PWM( TIM_TypeDef * Timer , char Channel ) ;

void MyTimer_PWM_RC( TIM_TypeDef * Timer, int RC) ;
#endif
