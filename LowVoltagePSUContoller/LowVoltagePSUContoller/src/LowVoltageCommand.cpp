#include "LowVoltageCommand.h"
#include <iostream>
#include "LowVoltageCommunication.h"

LowVoltageCommand::LowVoltageCommand( std::string dimServerName )
{

	 std::string vcmdName = dimServerName + "/SET_VOLTAGE";
	 mp_voltageCmd = new DimCommand( vcmdName.c_str(), "F", this );

	 std::string icmdName = dimServerName + "/SET_CURRENT";
	 mp_currentCmd = new DimCommand( icmdName.c_str(), "F", this );

	 std::string pcmdName = dimServerName + "/SET_POWER_STATE";
	 mp_powerStateCmd = new DimCommand( pcmdName.c_str(), "S", this );

	 std::string refreashcmdName = dimServerName + "/REFREASH_DATA_VARIABLES";
	 mp_refreashCmd = new DimCommand( refreashcmdName.c_str(), "C", this );

	 /** Set Defualts **/
	 m_voltage = 0.0;
	 m_current = 0.0;
	 m_powerState = 0;

	 std::string vserviceName = dimServerName + "/CURRENT_VOLTAGE";
	 mp_voltageService = new DimService( vserviceName.c_str(), m_voltage );

	 std::string iserviceName = dimServerName + "/CURRENT_CURRENT";
	 mp_currentService = new DimService( iserviceName.c_str(), m_current);

	 std::string pserviceName = dimServerName + "/CURRENT_POWER_STATE";
	 mp_powerStateService = new DimService( pserviceName.c_str() , m_powerState);

	 mp_lvComLink = new LowVoltageCommunication();
	 

}


LowVoltageCommand::~LowVoltageCommand(void)
{

	 delete mp_voltageCmd; 
	 delete mp_currentCmd; 
	 delete mp_powerStateCmd; 

	 delete mp_voltageService; 
	 delete mp_currentService;
	 delete mp_powerStateService;

	 delete mp_lvComLink;

}

void LowVoltageCommand::commandHandler(void)
{
	/// This get called each time a command is issued by the client

	DimCommand *currCmd = getCommand(); // request current command

	if( currCmd == mp_voltageCmd )
	{
		std::cout << "Set Voltage to " << currCmd->getFloat() << " received" << std::endl; // informing the user that the command has been recieved
		m_voltage = currCmd->getFloat();
		/**need to communicate with the power supply**/
		if( mp_lvComLink->ConnectToDevice() ){// test link.
			// Next send voltage update 
			mp_lvComLink->SetVoltage( m_voltage );
			// Request voltage 
			std::cout<<"Get new voltage from PSU (SERVER)"<<std::endl;

			m_voltage = mp_lvComLink->VoltageReading();

			std::cout<<"New Voltage Value (SERVER)"<< m_voltage <<std::endl;

			// Still testing so...

			//  m_voltage = pwrVoltage;

			mp_voltageService->updateService( m_voltage ); // updates the DIM service with the new variable
		}
		else{
			// Need to handle events where the server cannot connect to the Low voltage
		
		}
	}
	else if( currCmd == mp_currentCmd )
	{
		std::cout << "Set Current to " << currCmd->getFloat() << " received" << std::endl; // informing the user that the command has been recieved
		m_current = currCmd->getFloat();
		
		if( mp_lvComLink->ConnectToDevice() ){// test link.
			// Next send voltage update 
			mp_lvComLink->SetCurrent( m_current );
			// Request voltage 
			std::cout<<"Get new current from PSU (SERVER)"<<std::endl;

			m_current = mp_lvComLink->CurrentReading();

			std::cout<<"New Current Value (SERVER)"<< m_current <<std::endl;

			// Still testing so...

			//  m_voltage = pwrVoltage;

			mp_currentService->updateService( m_current ); // updates the DIM service with the new variable
		}
		else{
			// Need to handle events where the server cannot connect to the Low voltage
		
		}
	}
	else if( currCmd == mp_powerStateCmd )
	{
		std::cout << "Set Power Status to " << currCmd->getShort() <<" received" << std::endl; // informing the user that the command has been recieved

		if( currCmd->getShort() > 1 || currCmd->getShort() < 0){
			std::cout<<"SHORT NEEDS TO EVALUATE TO BOOL"<<std::endl;
		}
		else{
			m_powerState = currCmd->getShort();
			
			if( mp_lvComLink->ConnectToDevice() ){// test link.
			// Next send voltage update 
				mp_lvComLink->SetPowerState( m_powerState );
			// Request voltage 
				std::cout<<"Get new power state from PSU (SERVER)"<<std::endl;

				m_powerState = mp_lvComLink->CurrentPowerState();

				std::cout<<"New power state Value (SERVER)"<< m_powerState <<std::endl;

			// Still testing so...

			//  m_voltage = pwrVoltage;

				mp_currentService->updateService( m_powerState ); // updates the DIM service with the new variable
		}
			else{
			// Need to handle events where the server cannot connect to the Low voltage
		
			}
		}
	}
	else if( currCmd == mp_refreashCmd )
	{
		std::string refreashCommand( mp_refreashCmd->getString() );

		if( refreashCommand == "ALL" ){
			/** open connection to the power supply and query the voltage and current and power state **/ 
			if( mp_lvComLink->ConnectToDevice() ){
				m_voltage = mp_lvComLink->VoltageReading();
				mp_voltageService->updateService( m_voltage );
				
				m_current = mp_lvComLink->CurrentReading();
				mp_currentService->updateService( m_current ); 

				m_powerState = mp_lvComLink->CurrentPowerState();
				mp_powerStateService->updateService( m_powerState );
			}
		}
		else if( refreashCommand == "POWERSTATE" ){

			if( mp_lvComLink->ConnectToDevice() ){
				m_powerState = mp_lvComLink->CurrentPowerState();
				mp_powerStateService->updateService( m_powerState );
			}
		
		}
		else if( refreashCommand == "VOLTAGE" ){

			if( mp_lvComLink->ConnectToDevice() ){
			    m_voltage = mp_lvComLink->VoltageReading();
				mp_voltageService->updateService( m_voltage );
			}

		}
		else if( refreashCommand == "CURRENT" ){

			if( mp_lvComLink->ConnectToDevice() ){
				m_current = mp_lvComLink->CurrentReading();
				mp_currentService->updateService( m_current ); 
			}

		}
		else{
			std::cout<<"Command not supported the options are: ALL, POWERSTATE, VOLTAGE, CURRENT"<<std::endl;
		}
	}
	else
	{
		std::cout << "Requested Command is not supported " << std::endl;
	}

}

void  LowVoltageCommand::PowerSupplyIPAddress( char* ipAddress ){
	
	mp_lvComLink->IpAddressOfServer( ipAddress );
	mp_lvComLink->InitialiseSocket();
}
void LowVoltageCommand::Update(){

	if( mp_lvComLink->ConnectToDevice() ){

		m_voltage = mp_lvComLink->VoltageReading();
		mp_voltageService->updateService( m_voltage );
				
		m_current = mp_lvComLink->CurrentReading();
		mp_currentService->updateService( m_current ); 

		m_powerState = mp_lvComLink->CurrentPowerState();
		mp_powerStateService->updateService( m_powerState );
	}

}
