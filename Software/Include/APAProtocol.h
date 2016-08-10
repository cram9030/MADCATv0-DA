/*

*/

// ******************************************************************************************
// INCLUDED LIBRARIES
#include "physicalLayer.h"

// ******************************************************************************************
// DEFINITIONS

#define apa_max_packet 10 // maximum path and payload size
#define apa_timeout 255 // timeout loop count

#define apa_start '{' // packet start
#define apa_pointer '^' // packet path pointer
#define apa_divider '|' // packet payload divider
#define apa_end '}' // packet end
#define apa_escape '\\' // packet end escape

#define apa_flood 'F' // flood destination
#define apa_here 'Z' // terminal destination

#define readusDelay 10 // delay betefore reading bit

// ******************************************************************************************
// STRCUT DEFINITIONS

struct apaPortType {
			unsigned char pathIn[apa_max_packet];
			unsigned char pathOut[apa_max_packet];
			unsigned char pathInLength, pathOutLength;
			unsigned char payloadIn[apa_max_packet];
			unsigned char payloadOut[apa_max_packet];
			unsigned char payloadInLength, payLoadOutLength;
			char destination;
			char id;
			struct apaPortType *nextPort;
			unsigned char packet[2*apa_max_packet+3];
			physicalLayer device;
			apaPortType():id(0),device(0){};
		};
		
// ******************************************************************************************
// CLASS DEFINITIONS

class apaProtocol {
	
	public:
		apaPortType apaPort;
		unsigned char apaProcessPacket(struct apaPortType *port);
		void apaMovePacket(struct apaPortType *port0, struct apaPortType *port1);
		void apaCopyPacket(struct apaPortType *port0, struct apaPortType *port1);
		void apaRoutePacket(struct apaPortType *port);
		void apaReversePath(struct apaPortType *port);
		void apaGetPacket(struct apaPortType *port);
		void apaPutPacket(struct apaPortType *port);
		void apaProtScan(struct apaPortType *port);
		apaProtocol(char);
};