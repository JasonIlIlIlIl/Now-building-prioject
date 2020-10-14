#ifndef __IMU_H
#define __IMU_H


#include "stm32f4xx.h"
#include <math.h>
#define RESTRICT_PITCH // Comment out to restrict roll to ��90deg instead - please read: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf



 
#define RAD_TO_DEG 57.295779513082320876798154814105  // ����ת�Ƕȵ�ת����
#define DEG_TO_RAD 0.01745329251994329576923690768489 // �Ƕ�ת���ȵ�ת����



void  updatePitchRoll(void);                                    //���ݼ��ټ�ˢ��Pitch��Roll����
void  updateYaw(void);                                            //���ݴ�����ˢ��Yaw��
void  InitAll(void);                                            //ѭ��ǰ�ĳ�ʼ��
void  func(void);                                                //ѭ���������


#endif
