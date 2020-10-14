#ifndef __APP_H__
#define __APP_H__

#include "ucos_ii.h"
#include "bsp_bluetooth.h"
#include "bsp_i2c.h"
#include "mpu6050.h"
#include "hmc5883.h"
#include "bsp_led.h"
#include "IMU.h"
#include "TIM_in_out.h"
#include "bsp_led.h"


#define APP_TASK0_STK_SIZE				512
#define APP_TASK0_PRIO						16
#define APP_TASK1_STK_SIZE				512
#define APP_TASK1_PRIO						14
#define APP_TASK2_STK_SIZE				512
#define APP_TASK2_PRIO						11
#define APP_TASK3_STK_SIZE				512
#define APP_TASK3_PRIO						11


extern OS_STK		App_Task0Stack[APP_TASK0_STK_SIZE];
extern OS_STK		App_Task1Stack[APP_TASK1_STK_SIZE];
extern OS_STK		App_Task2Stack[APP_TASK2_STK_SIZE];
void App_Task0(void *p_arg);
void App_Task1(void *p_arg);
void App_Task2(void *p_arg);
void BSP_Init(void);

#endif


