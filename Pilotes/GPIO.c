#include "GPIO.h"

#include "stm32f10x.h"

#define OUTPUTPUSHPULL           0x2   // CNF=00, MODE=10 ? 0010b
#define OUTPUTALTERNATEPUSHPULL  0xA   // CNF=10, MODE=10 ? 1010b
#define INPOUTFLOATING           0x4   // CNF=01, MODE=00 ? 0100b
#define INPOUTANALOG             0x0   // CNF=00, MODE=00 ? 0000b


void InitGPIO(GPIO_TypeDef *PORT, char BROCHE, char CONFIG){
	
  if(BROCHE < 8){
        PORT->CRL &= ~(0xF << (BROCHE * 4));
        PORT->CRL |= ((int)CONFIG << (BROCHE * 4));
    } else {
        BROCHE -= 8;
        PORT->CRH &= ~(0xF << (BROCHE * 4));
        PORT->CRH |= ((int)CONFIG << (BROCHE * 4));
    }

}

char LireBroche(GPIO_TypeDef *PORT, char BROCHE){
	
	return (PORT->IDR >> BROCHE) & 0x1 ; 
	
}

void SetBroche(GPIO_TypeDef *PORT, char BROCHE){
	
 	PORT->ODR |= (1 << (BROCHE));
	
}

void ResetBroche(GPIO_TypeDef *PORT, char BROCHE){

		PORT->ODR &= ~(1 << (BROCHE));


}

void MyGPIO_Toggle(GPIO_TypeDef *PORT, char BROCHE)
{
    if (LireBroche(PORT, BROCHE))  // Si la broche est à 1
        ResetBroche(PORT, BROCHE); // → on la met à 0
    else                           // Sinon (elle est à 0)
        SetBroche(PORT, BROCHE);   // → on la met à 1
}












