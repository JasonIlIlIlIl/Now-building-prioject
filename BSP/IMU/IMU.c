#include "IMU.h"
#include "mpu6050.h"
#include "hmc5883.h"
#include "Kalman.h"
#include "bsp_bluetooth.h"
short nacc[3], ngyro[3], nmag[3];
float acc[3],gyro[3],mag[3];

double roll, pitch, yaw; // Roll and pitch are calculated using the accelerometer while yaw is calculated using the magnetometer

double gyroXangle, gyroYangle, gyroZangle; // Angle calculate using the gyro only 只用陀螺仪计算角度
double compAngleX, compAngleY, compAngleZ; // Calculated angle using a complementary filter  用电磁计计算角度
double kalAngleX, kalAngleY, kalAngleZ; // Calculated angle using a Kalman filter    用kalman计算角度

double magGain[3];
struct Kalman kalmanX, kalmanY, kalmanZ; // Create the Kalman instances

void InitAll(void){
		int sum =0;
	/* Set Kalman and gyro starting angle */
		MPU6050ReadAcc(nacc);
		MPU6050ReadGyro(ngyro);
		HMC_Read(nmag);
		acc[0] = nacc[0]/16384.0-0.02323;
		acc[1] = nacc[1]/16384.0-(-0.008);
		acc[2] = nacc[2]/16384.0-(-0.07);
		gyro[0] = ngyro[0]/131.1;
		gyro[1] = ngyro[1]/131.1;
		gyro[2] = ngyro[2]/131.1;
		mag[0] = nmag[0]/252.84 - (0.38);
		mag[1] = nmag[1]/252.84 - (-0.08);
		mag[2] = nmag[2]/252.84 - (-0.06);
		sum += put_char(0xAA);
		sum += put_char(0xAA);
		sum += put_char(0x02);
		sum += put_char(18);
		sum += put_Int16(nacc[0]);
		sum += put_Int16(nacc[1]);
		sum += put_Int16(nacc[2]);
		sum += put_Int16(ngyro[0]);
		sum += put_Int16(ngyro[1]);
		sum += put_Int16(ngyro[2]);
		sum += put_Int16(nmag[0]);
		sum += put_Int16(nmag[1]);
		sum += put_Int16(nmag[2]);
		put_char(sum);
		updatePitchRoll();
		updateYaw();
    
		setAngle(&kalmanX,roll); // First set roll starting angle
		gyroXangle = roll;
		compAngleX = roll;
    
		setAngle(&kalmanY,pitch); // Then pitch
		gyroYangle = pitch;
		compAngleY = pitch;
   
		setAngle(&kalmanZ,yaw); // And finally yaw
		gyroZangle = yaw;
		compAngleZ = yaw;

}

void  updatePitchRoll(void){ 
		// Source: http://www.freescale.com/files/sensors/doc/app_note/AN3461.pdf eq. 25 and eq. 26
		// atan2 outputs the value of -π to π (radians) - see http://en.wikipedia.org/wiki/Atan2
		// It is then converted from radians to degrees
		#ifdef RESTRICT_PITCH // Eq. 25 and 26
		roll = atan2(acc[1],acc[2]) * RAD_TO_DEG;
		pitch = atan(-acc[0] / sqrt(acc[1] * acc[1] + acc[2] * acc[2])) * RAD_TO_DEG;
		#else // Eq. 28 and 29
		roll = atan(acc[1] / sqrt(acc[0] * acc[0] + acc[2] * acc[2])) * RAD_TO_DEG;
		pitch = atan2(-acc[0], acc[2]) * RAD_TO_DEG;
		#endif
}

void  updateYaw(void){
		double rollAngle,pitchAngle,Bfy,Bfx;  
    mag[0] *= -1; // Invert axis - this it done here, as it should be done after the calibration
		mag[2] *= -1;
		
		mag[0] *= magGain[0];
		mag[1] *= magGain[1];
		mag[2] *= magGain[2];



		rollAngle  = kalAngleX * DEG_TO_RAD;
		pitchAngle = kalAngleY * DEG_TO_RAD;
    
		Bfy = mag[2] * sin(rollAngle) - mag[1] * cos(rollAngle);
		Bfx = mag[0] * cos(pitchAngle) + mag[1] * sin(pitchAngle) * sin(rollAngle) + mag[2] * sin(pitchAngle) * cos(rollAngle);
		yaw = atan2(-Bfy, Bfx) * RAD_TO_DEG;

		yaw *= -1;
}

void  func(void){
		int sum=0;
		double gyroXrate,gyroYrate,gyroZrate,dt=0.01;
		/* Update all the IMU values */
		MPU6050ReadAcc(nacc);
		MPU6050ReadGyro(ngyro);
		HMC_Read(nmag);
		acc[0] = nacc[0]/16384.0-0.02323;
		acc[1] = nacc[1]/16384.0-(-0.008);
		acc[2] = nacc[2]/16384.0-(-0.07);
		gyro[0] = ngyro[0]/131.1- 3.783;
		gyro[1] = ngyro[1]/131.1- 3.285;
		gyro[2] = ngyro[2]/131.1- 3.800;
		mag[0] = nmag[0]/252.84 - (0.38);
		mag[1] = nmag[1]/252.84 - (-0.08);
		mag[2] = nmag[2]/252.84 - (-0.06);
		sum += put_char(0xAA);
		sum += put_char(0xAA);
		sum += put_char(0x02);
		sum += put_char(18);
		sum += put_Int16(nacc[0]);
		sum += put_Int16(nacc[1]);
		sum += put_Int16(nacc[2]);
		sum += put_Int16(ngyro[0]);
		sum += put_Int16(ngyro[1]);
		sum += put_Int16(ngyro[2]);
		sum += put_Int16(nmag[0]);
		sum += put_Int16(nmag[1]);
		sum += put_Int16(nmag[2]);
		put_char(sum);
		//    dt = (double)(micros - timer) / 1000; // Calculate delta time
		//    timer = micros;
		//    if(dt<0)dt+=(1<<20);    //时间是周期性的，有可能当前时间小于上次时间，因为这个周期远大于两次积分时间，所以最多相差1<<20
	
		/* Roll and pitch estimation */
		updatePitchRoll();             //用采集的加速计的值计算roll和pitch的值
		gyroXrate = gyro[0];    // Convert to deg/s    把陀螺仪的角加速度按照当初设定的量程转换为°/s
		gyroYrate = gyro[1];     // Convert to deg/s
		
		#ifdef RESTRICT_PITCH        //如果上面有#define RESTRICT_PITCH就采用这种方法计算，防止出现-180和180之间的跳跃
		// This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
		if ((roll < -90 && kalAngleX > 90) || (roll > 90 && kalAngleX < -90)) {
				setAngle(&kalmanX,roll);
				compAngleX = roll;
         kalAngleX = roll;
         gyroXangle = roll;
     } else
     kalAngleX = getAngle(&kalmanX, roll, gyroXrate, dt); // Calculate the angle using a Kalman filter
     
     if (fabs(kalAngleX) > 90)
       gyroYrate = -gyroYrate; // Invert rate, so it fits the restricted accelerometer reading
				kalAngleY = getAngle(&kalmanY,pitch, gyroYrate, dt);
		#else
		// This fixes the transition problem when the accelerometer angle jumps between -180 and 180 degrees
		if ((pitch < -90 && kalAngleY > 90) || (pitch > 90 && kalAngleY < -90)) {
					kalmanY.setAngle(pitch);
					compAngleY = pitch;
					kalAngleY = pitch;
					gyroYangle = pitch;
		} else
				kalAngleY = getAngle(&kalmanY, pitch, gyroYrate, dt); // Calculate the angle using a Kalman filter
   
		if (abs(kalAngleY) > 90)
				gyroXrate = -gyroXrate; // Invert rate, so it fits the restricted accelerometer reading
				kalAngleX = getAngle(&kalmanX, roll, gyroXrate, dt); // Calculate the angle using a Kalman filter
		#endif
    
 
		/* Yaw estimation */
		updateYaw();
		gyroZrate = gyro[2] ; // Convert to deg/s
		// This fixes the transition problem when the yaw angle jumps between -180 and 180 degrees
		if ((yaw < -90 && kalAngleZ > 90) || (yaw > 90 && kalAngleZ < -90)) {
        setAngle(&kalmanZ,yaw);
        compAngleZ = yaw;
				kalAngleZ = yaw;
				gyroZangle = yaw;
     } else
			kalAngleZ = getAngle(&kalmanZ, yaw, gyroZrate, dt); // Calculate the angle using a Kalman filter
     
     
     /* Estimate angles using gyro only */
     gyroXangle += gyroXrate * dt; // Calculate gyro angle without any filter
     gyroYangle += gyroYrate * dt;
     gyroZangle += gyroZrate * dt;
     //gyroXangle += kalmanX.getRate() * dt; // Calculate gyro angle using the unbiased rate from the Kalman filter
     //gyroYangle += kalmanY.getRate() * dt;
     //gyroZangle += kalmanZ.getRate() * dt;
     
     /* Estimate angles using complimentary filter */
     compAngleX = 0.93 * (compAngleX + gyroXrate * dt) + 0.07 * roll; // Calculate the angle using a Complimentary filter
     compAngleY = 0.93 * (compAngleY + gyroYrate * dt) + 0.07 * pitch;
     compAngleZ = 0.93 * (compAngleZ + gyroZrate * dt) + 0.07 * yaw;
     
     // Reset the gyro angles when they has drifted too much
     if (gyroXangle < -180 || gyroXangle > 180)
         gyroXangle = kalAngleX;
     if (gyroYangle < -180 || gyroYangle > 180)
         gyroYangle = kalAngleY;
     if (gyroZangle < -180 || gyroZangle > 180)
         gyroZangle = kalAngleZ;
     
    sum =0;
    sum += put_char(0xAA);
		sum += put_char(0xAA);
		sum += put_char(0x01);
		sum += put_char(12);
		sum += put_Int16(roll*100);
		sum += put_Int16(pitch*100);
		sum += put_Int16(yaw*100);
		sum += put_Int16(0);
		sum += put_Int16(0);
		sum += put_Int16(1);
		put_char(sum);

}

