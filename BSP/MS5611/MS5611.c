#include "MS5611.h"
#include "math.h"
#include "bsp_i2c.h"
#include "bsp_bluetooth.h"
#include "ucos_ii.h"

uint16_t ms5611ReadShort(uint8_t address) {
	uint8_t rxBuf[2] = {0,0};
	Sensor_Read(MS5611_ADDRESS, address, 2, rxBuf);
	printf("%d,%d\n",rxBuf[0],rxBuf[1]);
	return (rxBuf[0] << 8) | rxBuf[1];
}


uint32_t ms5611ReadLong(uint8_t address) {
	uint8_t rxBuf[3] = {0,0,0};
	Sensor_Read(MS5611_ADDRESS, address,3,rxBuf);
	printf("\n%d,%d,%d\n",rxBuf[0],rxBuf[1],rxBuf[2]);
	return (rxBuf[0] << 16) | (rxBuf[1] << 8) | rxBuf[2];
}


void ms5611WriteByte(uint8_t data) {
	uint8_t num = 1;
	Sensor_Write(MS5611_ADDRESS, data,1,&num);
}
//----------------------------------------

void MS5611_Init(void) {
	// Reset
	ms5611WriteByte(MS561101BA_RESET);

	OSTimeDly(999);

	// Read calibration data
	ms5611_C1 = ms5611ReadShort(0xA2);
	OSTimeDly(999);
	ms5611_C2 = ms5611ReadShort(0xA4);
	OSTimeDly(999);
	ms5611_C3 = ms5611ReadShort(0xA6);
	OSTimeDly(999);
	ms5611_C4 = ms5611ReadShort(0xA8);
	OSTimeDly(999);
	ms5611_C5 = ms5611ReadShort(0xAA);
	OSTimeDly(999);
	ms5611_C6 = ms5611ReadShort(0xAC);
	OSTimeDly(999);
	printf("C1: %d\n",ms5611_C1);
	printf("C2: %d\n",ms5611_C2);
	printf("C3: %d\n",ms5611_C3);
	printf("C4: %d\n",ms5611_C4);
	printf("C5: %d\n",ms5611_C5);
	printf("C6: %d\n",ms5611_C6);
}

void MS5611_Convert(long* temperature, long* pressure) {

	unsigned long D1, D2;
	long dT, TEMP;
	long long OFF, SENS, OFF2, SENS2, T2;

	// Start Pressure conversion
	ms5611WriteByte(MS561101BA_D1 + MS5611_OSR * 2);
	OSTimeDly(999);
	// Read Pressure data
	D1 = ms5611ReadLong(0x00);
	printf("\n %d \n",D1);
	// Start Temperature conversion
	ms5611WriteByte(MS561101BA_D2 + MS5611_OSR * 2);
	OSTimeDly(999);
	// Read Temperature data
	D2 = ms5611ReadLong(0x00);
	printf("\n %d \n",D2);

	dT = D2 - (ms5611_C5 << 8);
	TEMP = 2000 + (((long long)dT * (long long)ms5611_C6) >> 23);
	OFF = ((long long)ms5611_C2 << 16) + (((long long)ms5611_C4 * (long long)dT) >> 7);
	SENS = ((long long)ms5611_C1 << 15 ) + (((long long)ms5611_C3 * (long long)dT ) >> 8);

	if (TEMP >= 2000) {
		T2 = 0;
		OFF2 = 0;
		SENS2 = 0;
	}
	else if (TEMP < 2000) {
		T2 = (((long long)dT * (long long)dT) >> 31);
		OFF2 = 5 * (((long long)TEMP - 2000) * ((long long)TEMP - 2000)) >> 1;
		SENS2 = 5 * (((long long)TEMP - 2000) * ((long long)TEMP - 2000)) >> 2;
		if (TEMP < -1500 ) {
			OFF2 = OFF2 + 7 * (((long long)TEMP + 1500) * ((long long)TEMP + 1500));
			SENS2 = SENS2 + ((11 *(((long long)TEMP + 1500) * ((long long)TEMP + 1500))) >> 1);
		}
	}

	TEMP = TEMP - T2;
	OFF = OFF - OFF2;
	SENS = SENS - SENS2;
	
	*pressure = (unsigned long) (((((D1 * SENS) >> 21) - OFF)) >> 15);
	*temperature = (long)TEMP/100;
}
