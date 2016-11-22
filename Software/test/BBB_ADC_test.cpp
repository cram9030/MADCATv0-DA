#include "../Include/BlackCore.h"
#include "../Include/BlackADC.h"

#include <string>
#include <iostream>
#include <chrono>
#include <thread>

// ******************************************************************************************
// NAMESPACE

using namespace std;
using namespace std::chrono;
using namespace BlackLib;

// ******************************************************************************************
// MAIN

int main()
{
	/////////////////////
	// Initialize ADC  //
	/////////////////////
	BlackADC analog0(AIN0);
	BlackADC analog1(AIN1);
	BlackADC analog2(AIN2);
	BlackADC analog4(AIN4);
	BlackADC analog6(AIN6);
	
	///////////////////////
	// Data Storage Init //
	///////////////////////
	ofstream dataFile; //create file stream
	dataFile.open ("testing.txt"); //open text file
	dataFile <<"Time,Elevator1,Elevator2,Rudder,LeftTT,RightTT"<<endl;//write colomn headers
	
	/*int         valueInt0;
	int         valueInt1;
	int         valueInt2;
	int         valueInt4;
	int         valueInt6;
    float       valueFloat0;
	float       valueFloat1;
	float       valueFloat2;
	float       valueFloat4;
	float       valueFloat6;
	
	usleep(1000);
	valueInt0 = analog0.getNumericValue();
	valueInt1 = analog1.getNumericValue();
	valueInt2 = analog2.getNumericValue();
	valueInt4 = analog4.getNumericValue();
	valueInt6 = analog6.getNumericValue();
	
	usleep(1000);
	valueFloat0 = analog0.getConvertedValue( dap3 );
	valueFloat1 = analog1.getConvertedValue( dap3 );
	valueFloat2 = analog2.getConvertedValue( dap3 );
	valueFloat4 = analog4.getConvertedValue( dap3 );
	valueFloat6 = analog6.getConvertedValue( dap3 );
	
	std::cout << "AIN0_int: " << valueInt0 << std::endl;
    std::cout << "AIN0_flt: " << valueFloat0 << std::endl;
	std::cout << "AIN1_int: " << valueInt1 << std::endl;
    std::cout << "AIN1_flt: " << valueFloat1 << std::endl;
	std::cout << "AIN2_int: " << valueInt2 << std::endl;
    std::cout << "AIN2_flt: " << valueFloat2 << std::endl;
	std::cout << "AIN4_int: " << valueInt4 << std::endl;
    std::cout << "AIN4_flt: " << valueFloat4 << std::endl;
	std::cout << "AIN6_int: " << valueInt6 << std::endl;
    std::cout << "AIN6_flt: " << valueFloat6 << std::endl;*/
	
	//Set the inital time by creating a high resolution clock object
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	
	while(1){
		dataFile <<duration_cast<duration<double>>(high_resolution_clock::now() - t1).count()<<","<<analog0.getNumericValue()<<","<<analog1.getNumericValue()<<","<<analog2.getNumericValue()<<","<<analog4.getNumericValue()<<","<<analog6.getNumericValue()<<endl;
	}
	
	return 0;
}