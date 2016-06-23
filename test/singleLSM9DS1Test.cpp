
// ******************************************************************************************
// INCLUDED LIBRARIES
#include "../Include/LSM9DS1.h"
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>

// ******************************************************************************************
// NAMESPACE

using namespace std;
using namespace std::chrono;

// ******************************************************************************************
// MAIN

int main(){
	///////////////////////
	// Data Storage Init //
	///////////////////////
	ofstream dataFile; //create file stream
	dataFile.open ("testing.txt"); //open text file
	dataFile <<"Time,xAcc,yAcc,zAcc,xGyro,yGyro,zGyro,xMag,yMag,zMag"<<endl;//write colomn headers
	
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
	imu.settings.device.commInterface = IMU_MODE_SPI;
	imu.settings.device.mAddress = LSM9DS1_M;
	imu.settings.device.agAddress = LSM9DS1_AG;
	
	imu.settings.mag.enabled = false;
	
	//Set the inital time by creating a high resolution clock object
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	
	if (!imu.begin()){
		cout << "Could not intialize IMU" << endl;
	}
	else{
		//imu.readAccel();
		//cout << "Accel -> ax: " << imu.calcAccel(imu.ax) << " ay: " << imu.calcAccel(imu.ay) << " az: " << imu.calcAccel(imu.az) << endl;
		while(1){
			if (imu.accelAvailable())
			{
				//imu.readGyro();
				//imu.read2GyroAxis(1);
				//cout << "Gyro -> gx: " << imu.calcGyro(imu.gx) << " gy: " << imu.calcGyro(imu.gy) << " gz: " << imu.calcGyro(imu.gz) << endl;
				imu.readAccel();
				//imu.read2AccelAxis(1);
				//cout << "Accel -> ax: " << imu.calcAccel(imu.ax) << " ay: " << imu.calcAccel(imu.ay) << " az: " << imu.calcAccel(imu.az) << endl;
				cout << "Accel -> ax: " << imu.calcAccel(imu.readAccel(X_AXIS)) << " ay: " << imu.calcAccel(imu.readAccel(Y_AXIS)) << " az: " << imu.calcAccel(imu.readAccel(Z_AXIS)) << endl;
				//imu.readMag();
				//dataFile <<duration_cast<duration<double>>(high_resolution_clock::now() - t1).count()<<","<<imu.readAccel(X_AXIS)<<","<<imu.readAccel(Y_AXIS)<<","<<imu.az<<","<<imu.gx<<","<<imu.gy<<","<<imu.gz<<","<<imu.mx<<","<<imu.my<<","<<imu.mz<<endl;
				std::this_thread::sleep_for (std::chrono::milliseconds(100));
			}
		}
	}
}