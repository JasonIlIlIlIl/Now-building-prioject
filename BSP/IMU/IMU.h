#ifndef __IMU_H
#define __IMU_H


#include "stm32f4xx.h"
#include <math.h>
#define RESTRICT_PITCH // Comment out to restrict roll to ±90deg instead - please read: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf



 
#define RAD_TO_DEG 57.295779513082320876798154814105  // 弧度转角度的转换率
#define DEG_TO_RAD 0.01745329251994329576923690768489 // 角度转弧度的转换率



void  updatePitchRoll(void);                                    //根据加速计刷新Pitch和Roll数据
void  updateYaw(void);                                            //根据磁力计刷新Yaw角
void  InitAll(void);                                            //循环前的初始化
void  func(void);                                                //循环里的内容


#endif
