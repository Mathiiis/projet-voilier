#ifndef __servo_h
#define __servo_h
#include "stm32f10x.h"

int alpha_to_theta(int alpha);
int theta_to_RC(int theta);
int alpha_to_RC(int alpha);
//void IT_functionn (void);
void MyServoInit(void);


#endif