// ******************************************************************************************
// INCLUDED LIBRARIES
#include <iostream>
#include <unistd.h>
#include "../Include/APAProtocol.h"

using namespace std;

int main(int argc, char *argv[]){
	
	apaProtocol port0(0);
	apaProtocol port1(1);
	
	port0.apaPort.nextPort = &port1.apaPort;
	port1.apaPort.nextPort = &port0.apaPort;
	
	cout<< argv[1] <<endl;
	port0.apaPort.packet[0] = '{';
	port0.apaPort.packet[1] = '1';
	port0.apaPort.packet[2] = '^';
	port0.apaPort.packet[3] = '|';
	port0.apaPort.packet[0] = 'b';
	port0.apaPort.packet[0] = '}';
	port0.apaMovePacket(&port0.apaPort,&port1.apaPort);
	if(port1.apaPutPacket(&port1.apaPort) != 0)
	{
		while(port1.apaGetPacket(&port1.apaPort) == 0){}
	}
}