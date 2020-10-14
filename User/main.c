#include "stm32f4xx.h"
#include "apps.h"

OS_STK		App_Task0Stack[APP_TASK0_STK_SIZE];
OS_STK		App_Task1Stack[APP_TASK1_STK_SIZE];
OS_STK		App_Task2Stack[APP_TASK2_STK_SIZE];


int main(void)
{ 
	BSP_Init();

	OSInit();   
	OSTaskCreate(App_Task0,(void *)0,(OS_STK *)&App_Task0Stack[APP_TASK0_STK_SIZE-1],APP_TASK0_PRIO );
	//OSTaskCreate(App_Task1,(void *)0,(OS_STK *)&App_Task1Stack[APP_TASK1_STK_SIZE-1],APP_TASK1_PRIO );
	//OSTaskCreate(App_Task2,(void *)0,(OS_STK *)&App_Task2Stack[APP_TASK2_STK_SIZE-1],APP_TASK2_PRIO );
	OSStart();	
}



