#ifndef __servo_h
#define __servo_h
#include "stm32f10x.h"

int alpha_to_theta(int alpha);
float theta_to_DC(int theta);
float alpha_to_DC(int alpha);
//void IT_functionn (void);
void MyServoInit(void);
void SetDC ( float DC);


#endif