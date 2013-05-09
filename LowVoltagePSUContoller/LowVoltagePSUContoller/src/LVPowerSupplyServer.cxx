#include <string>
#include <dis.hxx>
#include "DIMServErrorHandler.h"
#include "LowVoltageCommand.h"
#include "EventTimer.h"
#include <Windows.h>
#include <iostream>
int main( int argc, char* argv[] )
{



	std::string serverName; //= "LOWVOLTAGE_NINO";
	char* ipAddress;
	if(argc != 3 ){
		std::cout<<"LOW VOLTAGE DIM SERVER TO USE: .\LowVoltageServer.exe <serverName> <ip address of PSU> " <<std::endl;
		exit(1);
	}
	else{
		serverName = argv[1];
		ipAddress = argv[2];
		std::cout<<"STARTING SERVER: "<<serverName<<" TO COMMUNICATE WITH PSU AT IP "<<ipAddress<<std::endl;
	}

	LowVoltageCommand* myLVCmdHandler = new LowVoltageCommand( serverName.c_str() );
	myLVCmdHandler->PowerSupplyIPAddress( ipAddress );

	DIMServErrorHandler* myErrorHandler = new DIMServErrorHandler();
	DimServer::addErrorHandler( myErrorHandler );

	DimServer::start( serverName.c_str() );

	//EventTimer myEvtTimer(10);
	//myEvtTimer.SetEvtObject( myLVCmdHandler );
	//myEvtTimer.StartTimer();

	//Add some server updates after a certain time period - say 1 min

	//SYSTEMTIME currentTime;
	//SYSTEMTIME startTime;

	//GetLocalTime(&startTime);

	while(true)
	{
	/*	GetLocalTime(&currentTime);
		if( currentTime.wMinute > startTime.wMinute ){
			std::cout<<"Checking LV connection and data values"<<std::endl;
			myLVCmdHandler->Update();
			GetLocalTime(&startTime);
		}*/
	}
	return 0;

}

