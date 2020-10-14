#ifndef BSP_LED_H
#define BSP_LED_H

#include "stm32f4xx.h"


#define digitalHi(p,i)       {p->BSRRL=i;}
#define digitalLo(p,i)       {p->BSRRH=i;}

#define LED_GPIO_PORT		GPIOA
#define LED_CLK					RCC_AHB1Periph_GPIOA
#define LED_GPIO_PIN		GPIO_Pin_5
#define LED_ON					digitalLo(LED_GPIO_PORT,LED_GPIO_PIN)
#define LED_OFF					digitalHi(LED_GPIO_PORT,LED_GPIO_PIN)
void LED_Config(void);

#endif
