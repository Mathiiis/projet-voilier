#include "stm32f10x.h"
#include "Servo.h"
#include "MyTimer.h"






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

int theta_to_RC(int theta){
      int RC = (theta*6/90)+5 ;
      return RC;
            
}

int alpha_to_RC(int alpha){
      int theta = alpha_to_theta(alpha);
      return theta_to_RC(theta);
}

/*
void IT_functionn (void){
      int RC = alpha_to_RC(80);
      MyTimer_PWM(TIM2, 2,RC);
}*/

void MyServoInit(void){

	
		MyTimer_PWM_TIM3_PA6( 3599, 399, 0.1);
			//MyTimer_Base_Init(TIM2, 3599, 399);
      //MyTimer_ActiveIT( TIM2 , 5 , IT_functionn);

}
