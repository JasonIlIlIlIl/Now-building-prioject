#ifndef __MS5611_H
#define __MS5611_H
#include "stm32f4xx.h"
/*宏定义------------------------------------------------------------------*/
#define MS5611_ADDRESS          0x77     
// 定义MS561101BA内部地址
// registers of the device
#define MS561101BA_D1 0x40
#define MS561101BA_D2 0x50
#define MS561101BA_RESET 0x1E

#define MS561101BA_PROM_BASE_ADDR 0xA0 // by adding ints from 0 to 6 we can read all the prom configuration values. 

// C1 will be at 0xA2 and all the subsequent are multiples of 2
#define MS561101BA_PROM_REG_COUNT 6 // number of registers in the PROM
#define MS561101BA_PROM_REG_SIZE 2 // size in bytes of a prom registry.

#define MS5611_OSR	4 // 0..4
//OSR 0 - Resolution 0.065 mbar, Conversion time 0.60 ms
//OSR 1 - Resolution 0.042 mbar, Conversion time 1.17 ms
//OSR 2 - Resolution 0.027 mbar, Conversion time 2.28 ms
//OSR 3 - Resolution 0.018 mbar, Conversion time 4.54 ms
//OSR 4 - Resolution 0.012 mbar, Conversion time 9.04 ms

extern unsigned short ms5611_C1, ms5611_C2, ms5611_C3, ms5611_C4, ms5611_C5, ms5611_C6;

void MS5611_Init(void);
void MS5611_Convert(long* temperature, long* pressure);


#endif
