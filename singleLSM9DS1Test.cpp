
// ******************************************************************************************
// INCLUDED LIBRARIES
#include "Include/LSM9DS1.h"
#include <iostream>

// ******************************************************************************************
// NAMESPACE

using namespace std;

// ******************************************************************************************
// MAIN

int main(){
	//////////////////////////
	// LSM9DS1 Library Init //
	//////////////////////////
	// Use the LSM9DS1 class to create an object.
	LSM9DS1 imu;
	
	///////////////////////
	// Example I2C Setup //
	///////////////////////
	// SDO_XM and SDO_G are both pulled high, so our addresses are:
	#define LSM9DS1_M	0x1E // Would be 0x1C if SDO_M is LOW
	#define LSM9DS1_AG	0x6B // Would be 0x6A if SDO_AG is LOW
	
	// Before initializing the IMU, there are a few settings
	// we may need to adjust. Use the settings struct to set
	// the device's communication mode and addresses:
	imu.settings.device.commInterface = IMU_MODE_I2C;
	imu.settings.device.mAddress = LSM9DS1_M;
	imu.settings.device.agAddress = LSM9DS1_AG;
	
	if (!imu.begin()){
		cout << "Could not intialize IMU" << endl;
	}
	else{
		while(1){
			imu.readGyro();
			cout << "Gyro -> gx: " << imu.calcGyro(imu.gx) << " gy: " << imu.calcGyro(imu.gy) << " gz: " << imu.calcGyro(imu.gz) << endl;
			imu.readAccel();
			cout << "Accel -> ax: " << imu.calcAccel(imu.ax) << " ay: " << imu.calcAccel(imu.ay) << " az: " << imu.calcAccel(imu.az) << endl;
			imu.readMag();
			cout << "Mag -> mx: " << imu.calcMag(imu.mx) << " my: " << imu.calcMag(imu.my) << " mz: " << imu.calcMag(imu.mz) << endl;
		}
	}
}