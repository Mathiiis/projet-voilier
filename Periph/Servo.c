#include "stm32f10x.h"
#include "Servo.h"
#include "../Driver1/Timer.h"

int alpha_to_theta(int alpha){
      int theta=0;
      int alphamod = alpha % 180;
      if(alphamod>=0 && alphamod<=45){
            theta=0;
      }           
      if(alphamod>=45){
            theta = ((alphamod-45)*90)/(180-45);
      }

      return theta;
}

float theta_to_DC(int theta){
      float DC = (theta*6/90)+5 ;
      return DC;
            
}

float alpha_to_DC(int alpha){
      float theta = alpha_to_theta(alpha);
      return theta_to_DC(theta)/100;
}

/*
void IT_functionn (void){
      int RC = alpha_to_RC(80);
      MyTimer_PWM(TIM2, 2,RC);
}*/

void MyServoInit(void){

		//int DC = alpha_to_DC(alpha);
		MyTimer_PWM_TIM3_PA6( 3599, 399, 0.05);
			//MyTimer_Base_Init(TIM2, 3599, 399);
      //MyTimer_ActiveIT( TIM2 , 5 , IT_functionn);

}

void SetDC ( float DC){
			 TIM3->CCR1 = (unsigned short)(3599 * DC);

}
