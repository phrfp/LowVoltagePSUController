#include <string>
#include <dis.hxx>
#include "DIMServErrorHandler.h"
#include "LowVoltageCommand.h"
#include "EventTimer.h"
#include <Windows.h>
#include <iostream>
int main( int argc, char* argv[] )
{

	std::string serverName; //The variable holding the name of the server that is reg. with the DNS server
	char* ipAddress; // ip address of the device - the port is hard coded in the comm. class as this is fixed
	if(argc != 3 ){
		// Quick explanation if the user does not include any arguments when running the app
		std::cout<<"LOW VOLTAGE DIM SERVER TO USE: .\LowVoltageServer.exe <serverName> <ip address of PSU> " <<std::endl;
		exit(1);
	}
	else{
		serverName = argv[1]; // set the server name to the first arg
		ipAddress = argv[2]; // set the ipaddress to the value input by the user
		std::cout<<"STARTING SERVER: "<<serverName<<" TO COMMUNICATE WITH PSU AT IP "<<ipAddress<<std::endl;
	}


	/** Create a command handler - this allows the server to take commands from the client 
		See documentation for the list of allowed commands and what they do.
	**/
	LowVoltageCommand* myLVCmdHandler = new LowVoltageCommand( serverName.c_str() ); // Set the server name
	myLVCmdHandler->PowerSupplyIPAddress( ipAddress ); // Pass the ip address to the server which has an association with the lowvolatege comm class and then set the ipaddress for the connection 

	DIMServErrorHandler* myErrorHandler = new DIMServErrorHandler(); // Create a error handler - add some user handled errors i.e the log file which is used when an event of interest takes place
	DimServer::addErrorHandler( myErrorHandler ); // The error handler is added to the server

	DimServer::start( serverName.c_str() ); // The server is started.

	/**
	Some timing stuff in case the server needs to take command of checking the status of the psu periodically
	*/

	//EventTimer myEvtTimer(10);
	//myEvtTimer.SetEvtObject( myLVCmdHandler );
	//myEvtTimer.StartTimer();

	//Add some server updates after a certain time period - say 1 min

	//SYSTEMTIME currentTime;
	//SYSTEMTIME startTime;

	//GetLocalTime(&startTime);

	//Run the server until killed
	while(true)
	{
	/*	GetLocalTime(&currentTime);
		if( currentTime.wMinute > startTime.wMinute ){
			std::cout<<"Checking LV connection and data values"<<std::endl;
			myLVCmdHandler->Update();
			GetLocalTime(&startTime);
		}*/
	}
	//clean up - need to check how crl-c terminates a program to ensure the memory is correctly released.
	delete myLVCmdHandler;
	delete myErrorHandler;
	return 0;

}

