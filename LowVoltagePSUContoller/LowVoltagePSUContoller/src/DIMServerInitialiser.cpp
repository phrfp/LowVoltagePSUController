#include "DIMServerInitialiser.h"
#include "DIMServErrorHandler.h"
#include "DIMServerConfigParser.h"
#include <string>
#include <dis.hxx>
#include <iostream>

DIMServerInitialiser::DIMServerInitialiser(void)
{
}


DIMServerInitialiser::~DIMServerInitialiser(void)
{

	for(std::vector<LowVoltageCommand*>::iterator iPSUCommandHandles = mvec_lowVoltageCommandHandlers.begin(); iPSUCommandHandles != mvec_lowVoltageCommandHandlers.end(); iPSUCommandHandles++  ){
	
		delete *iPSUCommandHandles;
	}

}

void DIMServerInitialiser::StartServer( DIMServerConfigParser* serverConfig ){
	//Add the handlers
	for(int iPSU = 0; iPSU < serverConfig->NumberOfPSUs(); iPSU++ ){
		std::string tempServerServiceName = serverConfig->ServerName() + "/" + serverConfig->PSUName( iPSU );
		std::cout<<"Service Name:  "<<tempServerServiceName<<std::endl;
		LowVoltageCommand* aLVCmdHandler = new LowVoltageCommand( tempServerServiceName );
		//aLVCmdHandler->PowerSupplyIPAddress( serverConfig->PSUIpAddress( iPSU ) );
		mvec_lowVoltageCommandHandlers.push_back( aLVCmdHandler );
	}
	

	mp_ErrorHandler = new DIMServErrorHandler(); // Create a error handler - add some user handled errors i.e the log file which is used when an event of interest takes place
	DimServer::addErrorHandler( mp_ErrorHandler ); // The error handler is added to the server

	DimServer::setDnsNode( serverConfig->DNSNode().c_str() );
	DimServer::start( serverConfig->ServerName().c_str() ); // The server is started.
}

void DIMServerInitialiser::UpdateCommandHandlers( bool updateInDebug ){

	for(std::vector< LowVoltageCommand* >::iterator iPSU = mvec_lowVoltageCommandHandlers.begin(); iPSU != mvec_lowVoltageCommandHandlers.end(); iPSU++ ){
		
		(*iPSU)->Update(updateInDebug);
	}

}