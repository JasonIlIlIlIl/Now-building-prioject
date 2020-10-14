#include "bsp_led.h"

void LED_Config(void){
	GPIO_InitTypeDef GPIO_Initstructure;
	RCC_AHB1PeriphClockCmd(LED_CLK,ENABLE);
	GPIO_Initstructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_Initstructure.GPIO_OType=GPIO_OType_PP;
	GPIO_Initstructure.GPIO_Speed=GPIO_Speed_25MHz;
	GPIO_Initstructure.GPIO_Pin=LED_GPIO_PIN;
	GPIO_Init(LED_GPIO_PORT,&GPIO_Initstructure);
}
