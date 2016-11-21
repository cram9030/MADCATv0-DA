#include "../Include/BlackCore.h"
#include "../Include/BlackADC.h"

#include <string>
#include <iostream>

using namespace BlackLib;

int main()
{
	BlackADC analog0(AIN0);
	BlackADC analog1(AIN1);
	BlackADC analog2(AIN2);
	BlackADC analog4(AIN4);
	BlackADC analog6(AIN6);
	
	int         valueInt0;
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
    std::cout << "AIN6_flt: " << valueFloat6 << std::endl;
	
	return 0;
}