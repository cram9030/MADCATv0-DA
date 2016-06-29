#include <iostream>
#include <unistd.h>
#include <fstream>
#include <chrono>
#include <thread>
#include <pthread.h>
#include "../Include/GPIO.h"
#include "../Include/LSM9DS1.h"
using namespace std;
using namespace std::chrono;

// GPIO *Datain, *firstLatchOut, *lastLatchOut;

// void *lookForEdge(void *value) {
// 	GPIO *lastLatch = (GPIO *)value;
// 	while(1) {
// 		lastLatch->waitForEdge();
// 		Datain->streamWrite(GPIO::LOW);
// 		firstLatchOut->waitForEdge();
// 		Datain->streamWrite(GPIO::HIGH);
// 	}
// }

int main() {
	if (getuid() != 0) {
		cout << "Run program as root" << endl;
		return -1;
	}

	// Datain = new GPIO(49);
	// firstLatchOut = new GPIO(117);
	// lastLatchOut = new GPIO(115);
	// Datain->setDirection(GPIO::OUTPUT);
	// firstLatchOut->setDirection(GPIO::INPUT);
	// lastLatchOut->setDirection(GPIO::INPUT);
	// Datain->streamOpen();
	// firstLatchOut->setEdgeType(GPIO::FALLING);
	// lastLatchOut->setEdgeType(GPIO::FALLING);
	// Datain->streamWrite(GPIO::HIGH);

	cout << "Reset Flip Flops then press enter" << endl;
	char temp;
	cin >> temp;

	// pthread_t thread;
	// if(pthread_create(&thread, NULL, &lookForEdge, lastLatchOut)) {
	// 	cout << "Failed to create thread" << endl;
	// 	return 1;
	// }

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

	if (!imu.begin() && !imu.begin() && !imu.begin()){
		cout << "Could not intialize IMU1" << endl;
	} else {
		imu.readAccel();
		cout << "Accel -> ax: " << imu.calcAccel(imu.ax) << " ay: " << imu.calcAccel(imu.ay) << " az: " << imu.calcAccel(imu.az) << endl;
		while (1) {
			//imu.readGyro();
			//imu.read2GyroAxis(1);
			//cout << "Gyro -> gx: " << imu.calcGyro(imu.gx) << " gy: " << imu.calcGyro(imu.gy) << " gz: " << imu.calcGyro(imu.gz) << endl;
			imu.readAccel();
			//imu.read2AccelAxis(1);
			cout << "Accel -> ax: " << imu.calcAccel(imu.ax) << " ay: " << imu.calcAccel(imu.ay) << " az: " << imu.calcAccel(imu.az) << endl;
			usleep(500000);
		}
	}
}