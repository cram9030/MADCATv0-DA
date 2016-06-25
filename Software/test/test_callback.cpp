#include <iostream>
#include <unistd.h>
#include <fstream>
#include <chrono>
#include <thread>
#include <pthread.h>
#include "GPIO.h"
#include "MADCATv0-DA/Include/LSM9DS1.h"
using namespace std;
using namespace std::chrono;

GPIO *outGPIO, *myclock, *in2;

void *lookForEdge(void *value) {
	GPIO *myGPIO = (GPIO *)value;
	while(1) {
		myGPIO->waitForEdge();
		outGPIO->streamWrite(GPIO::HIGH);
		in2->waitForEdge();
		outGPIO->streamWrite(GPIO::LOW);
	}
}

// void *lookForOtherEdge(void *value) {
// 	GPIO *myGPIO = (GPIO *)value;
// 	while(1) {
// 		myGPIO->waitForEdge();
// 		usleep(100000);
// 		outGPIO->streamWrite(GPIO::LOW);
// 	}
// }

int init(int num) {
	int i;
	for (i = 0; i < num; i++) {
		myclock->streamWrite(GPIO::HIGH);
		usleep(125000);
		myclock->streamWrite(GPIO::LOW);
		usleep(125000);
	}
	outGPIO->streamWrite(GPIO::HIGH);
	myclock->streamWrite(GPIO::HIGH);
	usleep(125000);
	myclock->streamWrite(GPIO::LOW);
	usleep(125000);
	outGPIO->streamWrite(GPIO::LOW);
}

int main(int argc, char *argv[]) {
	if (getuid() != 0) {
		cout << "Run program as root" << endl;
		return -1;
	}

	if (argc < 2) {
		cout << "USAGE: ./test_callback [number of imus]" << endl;
		return 1;
	}

	int numberIMUS = *argv[1] - '0';

	outGPIO = new GPIO(49);
	myclock = new GPIO(4);
	outGPIO->setDirection(GPIO::OUTPUT);
	myclock->setDirection(GPIO::OUTPUT);
	outGPIO->streamOpen();
	myclock->streamOpen();
	outGPIO->streamWrite(GPIO::LOW);
	myclock->streamWrite(GPIO::LOW);
	init(numberIMUS);
	myclock->~GPIO();
	in2 = new GPIO(117);
	in2->setDirection(GPIO::INPUT);
	in2->setEdgeType(GPIO::RISING);

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

	GPIO inGPIO(115);
	inGPIO.setDirection(GPIO::INPUT);
	inGPIO.setEdgeType(GPIO::FALLING);
	pthread_t thread;
	if(pthread_create(&thread, NULL, &lookForEdge, &inGPIO)) {
		cout << "Failed to create thread" << endl;
		return 1;
	}

	
	// pthread_t thread2;
	// if(pthread_create(&thread2, NULL, &lookForOtherEdge, &in2)) {
	// 	cout << "Failed to create other thread" << endl;
	// 	return 1;
	// }

	if (!imu.begin()){
		cout << "Could not intialize IMU" << endl;
	}else {
		imu.readAccel();
		cout << "Accel -> ax: " << imu.calcAccel(imu.ax) << " ay: " << imu.calcAccel(imu.ay) << " az: " << imu.calcAccel(imu.az) << endl;
		while (1) {
			//int i;
			//for (i = 0; i < numberIMUS; i++) {
				//imu.readGyro();
				//imu.read2GyroAxis(1);
				//cout << "Gyro -> gx: " << imu.calcGyro(imu.gx) << " gy: " << imu.calcGyro(imu.gy) << " gz: " << imu.calcGyro(imu.gz) << endl;
				imu.readAccel();
				//imu.read2AccelAxis(1);
				cout << "Accel -> ax: " << imu.calcAccel(imu.ax) << " ay: " << imu.calcAccel(imu.ay) << " az: " << imu.calcAccel(imu.az) << endl;
				//myclock->streamWrite(GPIO::HIGH);
				//usleep(250000);
				//myclock->streamWrite(GPIO::LOW);
				//usleep(250000);
			//}
			//outGPIO->streamWrite(GPIO::HIGH);
			//usleep(10000);
			//myclock->streamWrite(GPIO::HIGH);
			//usleep(250000);
			//myclock->streamWrite(GPIO::LOW);
			//usleep(250000);
			//outGPIO->streamWrite(GPIO::LOW);
		}
	}
}