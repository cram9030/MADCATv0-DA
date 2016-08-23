// ******************************************************************************************
// INCLUDED LIBRARIES
#include <iostream>
#include <unistd.h>
#include "../Include/APAProtocol.h"

using namespace std;

int main(){
	apaProtocol port0(0);
	
	while(port0.apaGetPacket(&port0.apaPort) == 0){}
	cout << port0.apaPort.packet << endl;
	
}