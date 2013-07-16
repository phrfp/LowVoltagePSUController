
#include "DIMServerConfigParser.h"
#include "DIMServerInitialiser.h"
#include "EventTimer.h"
#include <Windows.h>
#include <iostream>
#include <cmath>

int main( int argc, char* argv[] )
{
	//bool createConnection = true; //this stops the socket being initaliesed by the cmd handler
	//std::string serverName; //The variable holding the name of the server that is reg. with the DNS server
	//char* ipAddress; // ip address of the device - the port is hard coded in the comm. class as this is fixed
	//char* dnsNode;
	std::string configFileName;
	if(argc != 2 ){
		// Quick explanation if the user does not include any arguments when running the app
		std::cout<<"LOW VOLTAGE DIM SERVER TO USE: .\LowVoltageServer.exe <config file> " <<std::endl;
		exit(1);
	}
	else{
		configFileName = argv[1];
		//serverName = argv[2]; // set the server name to the first arg
		//ipAddress = argv[3]; // set the ipaddress to the value input by the user
		//std::cout<<"STARTING SERVER: "<<serverName<<" TO COMMUNICATE WITH PSU AT IP "<<ipAddress<<std::endl;
	}

	DIMServerConfigParser* myDIMServerConfig = new DIMServerConfigParser();
	bool success = myDIMServerConfig->ConfigurerServer(configFileName);
	
	DIMServerInitialiser* myDIMServerInitialiser = new DIMServerInitialiser();
	myDIMServerInitialiser->StartServer( myDIMServerConfig );
	
	////Add some server updates after a certain time period - say 1 min

	SYSTEMTIME currentTime;
	SYSTEMTIME startTime;

	GetLocalTime(&startTime);

	while(true)
	{
		GetLocalTime(&currentTime);
		double timeDiff = std::fabs( ( (double)currentTime.wMinute - (double)startTime.wMinute) );
		if( timeDiff >=1  ) {
			std::cout<<"Checking LV connection and data values at "<< currentTime.wHour << ":" << currentTime.wMinute << ":" << currentTime.wSecond<<std::endl;
			myDIMServerInitialiser->UpdateCommandHandlers(true);
			GetLocalTime(&startTime);
		}
	}

	delete myDIMServerInitialiser;
	delete myDIMServerConfig;
	/** Create a command handler - this allows the server to take commands from the client 
		See documentation for the list of allowed commands and what they do.
	**/
	//LowVoltageCommand* myLVCmdHandler = new LowVoltageCommand( serverName.c_str() ); // Set the server name
	//if(createConnection == true ) myLVCmdHandler->PowerSupplyIPAddress( ipAddress ); // Pass the ip address to the server which has an association with the lowvolatege comm class and then set the ipaddress for the connection 

	//DIMServErrorHandler* myErrorHandler = new DIMServErrorHandler(); // Create a error handler - add some user handled errors i.e the log file which is used when an event of interest takes place
	//DimServer::addErrorHandler( myErrorHandler ); // The error handler is added to the server

	//DimServer::setDnsNode(dnsNode);
	//DimServer::start( serverName.c_str() ); // The server is started.

	///**
	//Some timing stuff in case the server needs to take command of checking the status of the psu periodically
	//*/

	////EventTimer myEvtTimer(10);
	////myEvtTimer.SetEvtObject( myLVCmdHandler );
	////myEvtTimer.StartTimer();

	////Add some server updates after a certain time period - say 1 min

	//SYSTEMTIME currentTime;
	//SYSTEMTIME startTime;

	//GetLocalTime(&startTime);

	////Run the server until killed
	//while(true)
	//{
	//	GetLocalTime(&currentTime);
	//	if( currentTime.wMinute > startTime.wMinute ){
	//		std::cout<<"Checking LV connection and data values"<<std::endl;
	//		myLVCmdHandler->Update(true);
	//		GetLocalTime(&startTime);
	//	}
	//}
	////clean up - need to check how crl-c terminates a program to ensure the memory is correctly released.
	//delete myLVCmdHandler;
	//delete myErrorHandler;
	return 0;

}

