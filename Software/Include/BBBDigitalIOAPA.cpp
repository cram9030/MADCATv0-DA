/*
*/

// ******************************************************************************************
// INCLUDED LIBRARIES
#include "physicalLayer.h"

// ******************************************************************************************
// DEFINITIONS
#define pinIn 79
#define pinOut 77

// ******************************************************************************************
// FUNCTIONS

//bBBDIOApa constructor
void bBBDIOApa::init(char pins){
	GPIO inGPIO(pinIn);
	GPIO outGPIO(pinOut);
	inGPIO.setDirection(INPUT);
	inGPIO.setEdgeType(RISING);
	outGPIO.setDirection(OUTPUT);
}

//////////////////////////////////////////////////////////////////////
// apaPutChar function - this functions takes a given pin and character
//							and sends it out
// input - apaPortType
// output - 0 if everything worked and 1 if something failed
//////////////////////////////////////////////////////////////////////
char bBBDIOApa::apaPutChar(char c, unsigned char *returnValue){
	// Start a clock reading
	// Set Ready Pin
	outGPIO.setValue(HIGH);
	
	// Initialize timeout
	*returnValue = apa_timeout;
	while(1){
		if(0 != inGPIO.getValue())
			break;
		*returnValue -= 1;
		if (*returnValue == 0){
			return 1;
		}
	}
	// Bit 0
	// Set pin to low for start bit
	outGPIO.setValue(LOW);
	// Stay low for the timout length
	*returnValue = apa_timeout;
	while(1){
		if(0 != inGPIO.getValue())
			break;
		*returnValue -= 1;
		if (*returnValue == 0){
			return 1;
		}
	}
	// Bit 1
	// Check to see if the next value should be one or zero and set it repspectively
	if (((c >> 0) & 1) == 0)
		outGPIO.setValue(LOW);
	else
		outGPIO.setValue(HIGH);
	// Loop for timeout
	*returnValue = apa_timeout;
	while(1){
		if(0 != inGPIO.getValue())
			break;
		*returnValue -= 1;
		if (*returnValue == 0){
			return 1;
		}
	}
	
	// Bit 2
	// Check to see if the next value should be one or zero and set it repspectively
	if (((c >> 1) & 1) == 0)
		outGPIO.setValue(LOW);
	else
		outGPIO.setValue(HIGH);
	// Loop for timeout
	*returnValue = apa_timeout;
	while(1){
		if(0 != inGPIO.getValue())
			break;
		*returnValue -= 1;
		if (*returnValue == 0){
			return 1;
		}
	}
	
	// Bit 3
	// Check to see if the next value should be one or zero and set it repspectively
	if (((c >> 2) & 1) == 0)
		outGPIO.setValue(LOW);
	else
		outGPIO.setValue(HIGH);
	// Loop for timeout
	*returnValue = apa_timeout;
	while(1){
		if(0 != inGPIO.getValue())
			break;
		*returnValue -= 1;
		if (*returnValue == 0){
			return 1;
		}
	}
	
	// Bit 4
	// Check to see if the next value should be one or zero and set it repspectively
	if (((c >> 3) & 1) == 0)
		outGPIO.setValue(LOW);
	else
		outGPIO.setValue(HIGH);
	// Loop for timeout
	*returnValue = apa_timeout;
	while(1){
		if(0 != inGPIO.getValue())
			break;
		*returnValue -= 1;
		if (*returnValue == 0){
			return 1;
		}
	}
	
	// Bit 5
	// Check to see if the next value should be one or zero and set it repspectively
	if (((c >> 4) & 1) == 0)
		outGPIO.setValue(LOW);
	else
		outGPIO.setValue(HIGH);
	// Loop for timeout
	*returnValue = apa_timeout;
	while(1){
		if(0 != inGPIO.getValue())
			break;
		*returnValue -= 1;
		if (*returnValue == 0){
			return 1;
		}
	}
	
	// Bit 6
	// Check to see if the next value should be one or zero and set it repspectively
	if (((c >> 5) & 1) == 0)
		outGPIO.setValue(LOW);
	else
		outGPIO.setValue(HIGH);
	// Loop for timeout
	*returnValue = apa_timeout;
	while(1){
		if(0 != inGPIO.getValue())
			break;
		*returnValue -= 1;
		if (*returnValue == 0){
			return 1;
		}
	}
	
	// Bit 7
	// Check to see if the next value should be one or zero and set it repspectively
	if (((c >> 6) & 1) == 0)
		outGPIO.setValue(LOW);
	else
		outGPIO.setValue(HIGH);
	// Loop for timeout
	*returnValue = apa_timeout;
	while(1){
		if(0 != inGPIO.getValue())
			break;
		*returnValue -= 1;
		if (*returnValue == 0){
			return 1;
		}
	}
	
	// Last bit
	// Check to see if the next value should be one or zero and set it repspectively
	if (((c >> 7) & 1) == 0)
		outGPIO.setValue(LOW);
	else
		outGPIO.setValue(HIGH);
	// Loop for timeout
	*returnValue = apa_timeout;
	while(1){
		if(0 != inGPIO.getValue())
			break;
		*returnValue -= 1;
		if (*returnValue == 0){
			return 1;
		}
	}
	outGPIO.setValue(LOW);
	usleep(readusDelay);
	return 0;
}

//////////////////////////////////////////////////////////////////////
// apaGetChar function - this functions takes a given pin and gets the
//							character sent to the pin
// input - apaPortType
// output - 0 if everything worked and 1 if something failed
//////////////////////////////////////////////////////////////////////
char apaGetChar(char *c, unsigned char *returnValue){
	// Initialize the character
	*c = 0;
	
	// Initialize timeout
	*returnValue = apa_timeout;
	// Wait to recieve ready
	while(1){
		if(0 != inGPIO.getValue())
			break;
		*returnValue -= 1;
		if (*returnValue == 0){
			return 1;
		}
	}
	
	// Send start clock signal by putting output pin out
	outGPIO.setValue(HIGH);
	
	// Wait to recieve the start indicator bit
	while(1){
		if(0 != inGPIO.getValue())
			break;
		*returnValue -= 1;
		if (*returnValue == 0){
			return 1;
		}
	}
	
	// Recieve 0 bit
	outGPIO.setValue(LOW);
	usleep(readusDelay);
	if(0 == inGPIO.getValue())
		*c |= (0 << 1);
	else
		*c |= (1 << 1);
	
	// Recieve 1 bit
	outGPIO.setValue(HIGH);
	usleep(readusDelay);
	if(0 == inGPIO.getValue())
		*c |= (0 << 1);
	else
		*c |= (1 << 1);
		
	// Recieve 2 bit
	outGPIO.setValue(LOW);
	usleep(readusDelay);
	if(0 == inGPIO.getValue())
		*c |= (0 << 2);
	else
		*c |= (1 << 2);
	
	// Recieve 3 bit
	outGPIO.setValue(HIGH);
	usleep(readusDelay);
	if(0 == inGPIO.getValue())
		*c |= (0 << 3);
	else
		*c |= (1 << 3);
		
	// Recieve 4 bit
	outGPIO.setValue(LOW);
	usleep(readusDelay);
	if(0 == inGPIO.getValue())
		*c |= (0 << 4);
	else
		*c |= (1 << 4);
	
	// Recieve 5 bit
	outGPIO.setValue(HIGH);
	usleep(readusDelay);
	if(0 == inGPIO.getValue())
		*c |= (0 << 5);
	else
		*c |= (1 << 5);
		
	// Recieve 6 bit
	outGPIO.setValue(LOW);
	usleep(readusDelay);
	if(0 == inGPIO.getValue())
		*c |= (0 << 6);
	else
		*c |= (1 << 6);
	
	// Recieve 7 bit
	outGPIO.setValue(HIGH);
	usleep(readusDelay);
	if(0 == inGPIO.getValue())
		*c |= (0 << 7);
	else
		*c |= (1 << 7);
		
	//Send Stop Clock indicator
	outGPIO.setValue(LOW);
	usleep(readusDelay);
	
	//Return 0 to indicate completion of send
	return 0;
}

//////////////////////////////////////////////////////////////////////
// apaGetPacket function - this functions calls apaGetChar and 
//							collects the characters and puts them into
//							the packet
// input - packet array pointer
// output - returns pathInLength
// calls - apaGetChar
//////////////////////////////////////////////////////////////////////
char apaGetPacket(unsigned char *packet){
	// Initialize characters, returnValue, and count
	char c,index;
	unsigned char returnValue,count;
	
	//Initialize index to zero
	index = 0;
	
	// Set count to timeout
	count = apa_timeout;
	// Get the start symbol
	while(c != apa_start){
		// Get character
		apaGetChar(&c,&returnValue);
		count -= 1;
		// Check to see if time out 
		if ((returnValue == 0) || (count == 0)){
			// Return 0 length
			return 0;
		}
	}
	// Set first packet to start symbol
	packet[0] = apa_start;
	// Incriment index
	index += 1;
	
	// Set count to timeout
	count = apa_timeout;
	// Collect the rest of the packet
	while(c != apa_stop){
		// Get character
		apaGetChar(&c,&returnValue);
		count -= 1;
		if ((returnValue == 0) || (count == 0)){
			// Return 0 length
			return 0;
		}
		// Check for escape character
		if (c == apa_escape){
			// Get escape character
			apaGetChar(&c,&returnValue);
			count -= 1;
			if ((returnValue == 0) || (count == 0)){
				// Return 0 length
				return 0;
			}
			packet[index] = c;
			index += 1;
			// Get next character
			apaGetChar(&c,&returnValue);
			count -= 1;
			if ((returnValue == 0) || (count == 0)){
				// Return 0 length
				return 0;
			}
		}
		// Save Character
		packet[index] = c;
		index += 1;
	}
	//Return the length of packet
	return index;
}

//////////////////////////////////////////////////////////////////////
// apaPutPacket function - this functions calls apaPutChar and 
//							puts it on the output
// input - packet array pointer
// output - returns 0 if successful and 1 if failure
// calls - apaPutChar
//////////////////////////////////////////////////////////////////////
char apaPutPacket(unsigned char *pathOut, unsigned char *payLoadOut, ,unsigned char *pathOutLength,unsigned char *payloadOutLength){
	// Initialize variables
	unsigned char i;
	char c;
	unsigned char returnValue;
	
	// Put start
	apaPutChar(apa_start,&returnValue);
	if (returnValue == 0){
		return 1;
	}
	
	// Put path 
	for (i = 0; i < pathOutLength; ++i){
		apaPutChar(pathOut[i],&returnValue);
		if (returnValue == 0 )
			return 1;
	}
	pathOutLength = 0;
	
	// Put Divider
	apaPutChar(apa_divider, &returnValue);
	if (returnValue == 0){
		return 1;
	}
	
	//Put Payload
	for (i = 0; i < payLoadLength, ++i){
		// Get next char
		c = payLoadOut[i];
		
		// Check if escape is needed
		if ((c == apa_end) || (c == apa_escape)){
			// Put escape in
			apaPutChar(apa_escape, &returnValue);
			if (returnValue == 0){
				return 1;
			}
		}
		// Put character
		apaPutChar(c,&returnValue);
		if (returnValue == 0){
			return 1;
		}
	}
	//Put End
	apaPutChar(apa_end,&returnValue);
	if (returnValue == 0){
		return 1;
	}
	payloadOutLegth = 0;
}