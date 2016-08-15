/*
 
*/
 
#ifndef _physicalLayer_H
#define _physicalLayer_H

// ******************************************************************************************
// INCLUDED LIBRARIES
#include <unistd.h>
#include "GPIO.h"

// ******************************************************************************************
// NAMESPACE
using namespace exploringBB;

// ******************************************************************************************
// CLASS DEFINITIONS
class physicalLayer {
	public:
		char apaGetPacket(unsigned char packet[]);
		char apaPutPacket(unsigned char pathOut[],unsigned char payLoadOut[],unsigned char &pathOutLength,unsigned char &payloadOutLength);
		physicalLayer(char,char);
	private:
		GPIO inGPIO;
		GPIO outGPIO;
		char apaPutChar(char c, unsigned char *returnValue);
		char apaGetChar(char *c, unsigned char *returnValue);
};
#endif