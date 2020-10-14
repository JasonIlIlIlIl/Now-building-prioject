#include "apps.h"
//#define TEST 1
/* IMU Data MPU6050 AND HMC5883 Data*/
extern struct Kalman kalmanX, kalmanY, kalmanZ; // Create the Kalman instances

/* IMU Data MPU6050 AND HMC5883 Data*/
extern short acc[3], gyro[3], mag[3];


extern double roll, pitch, yaw; // Roll and pitch are calculated using the accelerometer while yaw is calculated using the magnetometer

extern double gyroXangle, gyroYangle, gyroZangle; // Angle calculate using the gyro only ֻ�������Ǽ���Ƕ�
extern double compAngleX, compAngleY, compAngleZ; // Calculated angle using a complementary filter  �õ�żƼ���Ƕ�
extern double kalAngleX, kalAngleY, kalAngleZ; // Calculated angle using a Kalman filter    ��kalman����Ƕ�
//uint32_t timer,micros; //��һ��ʱ���뵱ǰʱ��
extern float magOffset[3];
extern double magGain[3];

void App_Task0(void *p_arg){
#ifdef TEST
	short Acel[3];
	short Gyro[3];
	short Hmc[3];
	float rAcel[3],rGyro[3],rHmc[3];
	uint8_t sum=0;
#endif
	OS_CPU_SysTickInit();
	InitAll();
	while(1){
		OSTimeDly(100);
		func();
		
		#ifdef TEST
		
		MPU6050ReadAcc(Acel);
		//printf("Acel:%8d%8d%8d",Acel[0],Acel[1],Acel[2]);
		rAcel[0] = Acel[0]/16384.0-0.02323;
		rAcel[1] = Acel[1]/16384.0-(-0.008);
		rAcel[2] = Acel[2]/16384.0-(-0.07);
		printf("Acel:%6.2f%6.2f%6.2f",rAcel[0],rAcel[1],rAcel[2]);
		MPU6050ReadGyro(Gyro);
		rGyro[0] = Gyro[0]/131.1- 3.783;
		rGyro[1] = Gyro[1]/131.1- 3.285;
		rGyro[2] = Gyro[2]/131.1- 3.800;
		printf(" Gyro:%6.2f%6.2f%6.2f",rGyro[0],rGyro[1],rGyro[2]);
		HMC_Read(Hmc);
		rHmc[0] = Hmc[0]/252.84 - (0.38);
		rHmc[1] = Hmc[1]/252.84 - (-0.08);
		rHmc[2] = Hmc[2]/252.84 - (-0.06);
		printf(" Hmc:%6.2f%6.2f%6.2f\r\n",rHmc[0],rHmc[1],rHmc[2]);
//		sum += put_char(0xAA);
//		sum += put_char(0xAA);
//		sum += put_char(0x02);
//		sum += put_char(18);
//		sum += put_Int16(Acel[0]);
//		sum += put_Int16(Acel[1]);
//		sum += put_Int16(Acel[2]);
//		sum += put_Int16(Gyro[0]);
//		sum += put_Int16(Gyro[1]);
//		sum += put_Int16(Gyro[2]);
//		sum += put_Int16(Hmc[0]);
//		sum += put_Int16(Hmc[1]);
//		sum += put_Int16(Hmc[2]);
//		put_char(sum);
//		sum=0;
		sum += put_char(0xAA);
		sum += put_char(0xAA);
		sum += put_char(0x02);
		sum += put_char(18);
		sum += put_Int16(Acel[0]);
		sum += put_Int16(Acel[1]);
		sum += put_Int16(Acel[2]);
		sum += put_Int16(Gyro[0]);
		sum += put_Int16(Gyro[1]);
		sum += put_Int16(Gyro[2]);
		sum += put_Int16(Hmc[0]);
		sum += put_Int16(Hmc[1]);
		sum += put_Int16(Hmc[2]);
		put_char(sum);
		sum=0;
	#endif
		OSTimeDly(100);
	}
}


void App_Task1(void *p_arg){
	OS_CPU_SysTickInit();	
	while(1)
	{
		OSTimeDly(100);
		printf("hello,Task1\n");
		OSTimeDly(100);
	}
}
void App_Task2(void *p_arg){
	int i=0;
	OS_CPU_SysTickInit();
	while(1){
		OSTimeDly(200);
		LED_ON;
		printf("LED ON %d times",i+1);
		OSTimeDly(200);
		LED_OFF;
		i++;
	}
}

void BSP_Init(void){
	TIM_config();
	Debug_USART_Config();
	GY_Config();
	MPU6050_Init();

	if(MPU6050ReadID()){
		printf("MPU Init SUCCESS.\n");
	}
	HMC_Init();
	LED_Config();
	
}


