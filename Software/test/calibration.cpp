#include "../Include/BlackCore.h"
#include "../Include/BlackADC.h"
#include "../Include/GPIO.h"
#include "../Include/LSM9DS1.h"

#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <pthread.h>

// ******************************************************************************************
// NAMESPACE

using namespace std;
using namespace std::chrono;
using namespace BlackLib;

// ******************************************************************************************
// MAIN

int main()
{
	
	///////////////////////////////
	// Check If Running as Root  //
	///////////////////////////////
	if (getuid() != 0) {
		cout << "Run program as root" << endl;
		return -1;
	}
		
	/////////////////////
	// Initialize ADC  //
	/////////////////////
	BlackADC analog0(AIN0);
	BlackADC analog1(AIN1);
	BlackADC analog2(AIN2);
	BlackADC analog4(AIN4);
	BlackADC analog6(AIN6);
	
	//////////////////////////
	// LSM9DS1 Library Init //
	//////////////////////////
	// Use the LSM9DS1 class to create an object.
	LSM9DS1 imu;
	int16_t gx1, gx2, gx3, gx4, gx5, gx6, gx7;
	int16_t gy1, gy2, gy3, gy4, gy5, gy6, gy7;
	int16_t ax1, ax2, ax3, ax4, ax5, ax6, ax7;
	int16_t ay1, ay2, ay3, ay4, ay5, ay6, ay7;
	
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
	
	///////////////////////
	// Data Storage Init //
	///////////////////////
	ofstream dataFile; //create file stream
	dataFile.open ("Calibration.txt"); //open text file
	dataFile <<"Time,ElevatorDeg,TwistDeg,RudderDeg,Elevator1,Elevator2,Rudder,LeftTT,RightTT,ax1,ax2,ax3,ax4,ax5,ax6,ax7,ay1,ay2,ay3,ay4,ay5,ay6,ay7,gx1,gx2,gx3,gx4,gx5,gx6,gx7,gy1,gy2,gy3,gy4,gy5,gy6,gy7"<<endl;//write colomn headers
	
	////////////////
	// Time Init //
	///////////////
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	auto t2 = duration_cast<duration<double>>(high_resolution_clock::now() - t1).count();
	auto t3 = t2;
	
	///////////////
	// IMU Init //
	//////////////
	ofstream logFile; //create file stream
	logFile.open ("IMUStatus.txt"); //open text file
	for (int i=0; i<7;i=i+1){
		if (!imu.begin()){
			logFile << "Could not intialize IMU" << endl;
		} 
		else{
			logFile << "Init Successful" << endl;
		}
	}
	
	/////////////////////
	// Hold Value Init //
	////////////////////
	float elDeg, TTDeg, RudDeg;
	
	while(1){
		
		if ((t2 - t3)>10.0){
			//Read currently held values
			cout << "Elevator Value: ";
			cin >> elDeg;
			cout << "Torque Tube Value: ";
			cin >> TTDeg;
			cout << "Rudder Value: ";
			cin >> RudDeg;
			t3 = duration_cast<duration<double>>(high_resolution_clock::now() - t1).count();
		}
		
		for (int i=0; i<7;i=i+1){
			imu.read2AccelAxis(1);
			if (i == 0 ){
				ax1 = imu.ax;
				ay1 = imu.ay;
			}
			else if(i == 1){
				ax2 = imu.ax;
				ay2 = imu.ay;
			}
			else if(i == 2){
				ax3 = imu.ax;
				ay3 = imu.ay;
			}
			else if(i == 3){
				ax4 = imu.ax;
				ay4 = imu.ay;
			}
			else if(i == 4){
				ax5 = imu.ax;
				ay5 = imu.ay;
			}
			else if(i == 5){
				ax6 = imu.ax;
				ay6 = imu.ay;
			}
			else if(i == 6){
				ax7 = imu.ax;
				ay7 = imu.ay;
			}
		}
		for (int i=0; i<7;i=i+1){
			imu.read2GyroAxis(1);
			if (i == 0 ){
				gx1 = imu.gx;
				gy1 = imu.gy;
			}
			else if(i == 1){
				gx2 = imu.gx;
				gy2 = imu.gy;
			}
			else if(i == 2){
				gx3 = imu.gx;
				gy3 = imu.gy;
			}
			else if(i == 3){
				gx4 = imu.gx;
				gy4 = imu.gy;
			}
			else if(i == 4){
				gx5 = imu.gx;
				gy5 = imu.gy;
			}
			else if(i == 5){
				gx6 = imu.gx;
				gy6 = imu.gy;
			}
			else if(i == 6){
				gx7 = imu.gx;
				gy7 = imu.gy;
			}
		}
		t2 = duration_cast<duration<double>>(high_resolution_clock::now() - t1).count();
		dataFile <<t2<<","<<elDeg<<","<<TTDeg<<","<<RudDeg<<","<<analog0.getNumericValue()<<","<<analog1.getNumericValue()<<","<<analog2.getNumericValue()<<","<<analog4.getNumericValue()<<","<<analog6.getNumericValue()<<','<<ax1<<','<<ax2<<','<<ax3<<','<<ax4<<','<<ax5<<','<<ax6<<','<<ax7<<','<<ay1<<','<<ay2<<','<<ay3<<','<<ay4<<','<<ay5<<','<<ay6<<','<<ay7<<','<<gx1<<','<<gx2<<','<<gx3<<','<<gx4<<','<<gx5<<','<<gx6<<','<<gx7<<','<<gy1<<','<<gy2<<','<<gy3<<','<<gy4<<','<<gy5<<','<<gy6<<','<<gy7<<endl;
	}
	
	return 0;
}
