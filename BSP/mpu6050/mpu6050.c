#include "mpu6050.h"
#include "bsp_bluetooth.h"
#include "bsp_i2c.h"

void MPU6050_Init(void){
    uint8_t data;

    //唤醒MPU6050
    data=0x00;
    Sensor_Write(MPU6050_ADDRESS,MPU6050_RA_PWR_MGMT_1,1,&data);
    
    //写入采样率
    data=0x07;
    Sensor_Write(MPU6050_ADDRESS,MPU6050_RA_SMPLRT_DIV,1,&data); 
    //MPU配置
    data=0x00;
    Sensor_Write(MPU6050_ADDRESS,MPU6050_RA_CONFIG,1,&data);
    //加速度计配置   2G
    data=0x00;
    Sensor_Write(MPU6050_ADDRESS,MPU6050_RA_ACCEL_CONFIG,1,&data);
    //陀螺仪配置   250
    data=0x00;
    Sensor_Write(MPU6050_ADDRESS,MPU6050_RA_GYRO_CONFIG,1,&data);
    //I2C_MST关
    data=0xC0;
    Sensor_Write(MPU6050_ADDRESS,MPU6050_RA_USER_CTRL,1,&data);  
    //打开旁通模式
    data=0x82;
    Sensor_Write(MPU6050_ADDRESS,MPU6050_RA_INT_PIN_CFG,1,&data);
    
}

uint8_t MPU6050ReadID(void)
{
	unsigned char Re = 0;
    Sensor_Read(MPU6050_ADDRESS,MPU6050_RA_WHO_AM_I,1,&Re);    //读器件地址
    
	if(Re != 0x68)
	{
		MPU_ERROR("MPU6050 dectected error!\r\n检测不到MPU6050模块，请检查模块与开发板的接线");
		return 0;
	}
	else
	{
		MPU_INFO("MPU6050 ID = %d\r\n",Re);
		return 1;
	}		
}

void MPU6050ReadAcc(short *accData)
{
    uint8_t buf[6];
    Sensor_Read(MPU6050_ADDRESS,MPU6050_ACC_OUT,6, buf);//读取加速度六字节数据
    accData[0] = (buf[0] << 8) | buf[1];   //x高八位左移和第八位共同组成16位数据
    accData[1] = (buf[2] << 8) | buf[3];   //y
    accData[2] = (buf[4] << 8) | buf[5];   //z
}

void MPU6050ReadGyro(short *gyroData)
{
    u8 buf[6];
    Sensor_Read(MPU6050_ADDRESS,MPU6050_GYRO_OUT,6,buf); //读取陀螺仪六字节数据
    gyroData[0] = (buf[0] << 8) | buf[1];           //x
    gyroData[1] = (buf[2] << 8) | buf[3];           //y 
    gyroData[2] = (buf[4] << 8) | buf[5];           //z
}


void MPU6050_ReturnTemp(float *Temperature)
{
	short temp3;
	u8 buf[2];
	
	Sensor_Read(MPU6050_ADDRESS,MPU6050_RA_TEMP_OUT_H,2,buf);     //读取温度值高8位与低8位共两字节
    temp3= (buf[0] << 8) | buf[1];	//高8位低8位组合成16位数据
	*Temperature=((double) temp3/340.0)+36.53;  //根据手册计算公式得温度

}

