/*
 
*/
 
// ******************************************************************************************
// INCLUDED LIBRARIES
#include <unistd.h>
#include "GPIO.h"

// ******************************************************************************************
// CLASS DEFINITIONS
class physicalLayer {
	public:
		char apaGetPacket(unsigned char packet[]);
		char apaPutPacket(unsigned char pathOut[],unsigned char payLoadOut[],unsigned char pathOutLength,unsigned char payloadOutLength);
		void init(char);
	private:
		GPIO inGPIO;
		GPIO outGPIO;
		char apaPutChar(GPIO *pin, char c, unsigned char *returnValue);
		char apaGetChar(GPIO *pin, char *c, unsigned char *returnValue);