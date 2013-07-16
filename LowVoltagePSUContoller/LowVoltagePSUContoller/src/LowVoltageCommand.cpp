#include "LowVoltageCommand.h"
#include <iostream>
#include "LowVoltageCommunication.h"

LowVoltageCommand::LowVoltageCommand( std::string dimServerName )
{
	/**
	The constructor creates all the cmds supported by the server and the services that are avalible
	*/
	 std::string vcmdName = dimServerName + "/SET_VOLTAGE";
	 mp_voltageCmd = new DimCommand( vcmdName.c_str(), "F", this );

	 std::string icmdName = dimServerName + "/SET_CURRENT";
	 mp_currentCmd = new DimCommand( icmdName.c_str(), "F", this );

	 std::string pcmdName = dimServerName + "/SET_POWER_STATE";
	 mp_powerStateCmd = new DimCommand( pcmdName.c_str(), "S", this );

	 std::string refreashcmdName = dimServerName + "/REFREASH_DATA_VARIABLES";
	 mp_refreashCmd = new DimCommand( refreashcmdName.c_str(), "C", this );

	 std::string queryPSUSettingsCmd = dimServerName + "/QUERY_PSU_SETTINGS";
	 mp_querySettingsCmd = new DimCommand( queryPSUSettingsCmd.c_str(), "C", this );

	 /** Set Defualts **/
	 m_voltage = 0.0;
	 m_current = 0.0;
	 m_voltage_setPoint = 0.0;
	 m_current_limit = 0.0;
	 m_powerState = 0;
	 m_debug_state=true;

	 std::string vserviceName = dimServerName + "/CURRENT_VOLTAGE";
	 mp_voltageService = new DimService( vserviceName.c_str(), m_voltage );

	 std::string iserviceName = dimServerName + "/CURRENT_CURRENT";
	 mp_currentService = new DimService( iserviceName.c_str(), m_current);

	 std::string v_setpoint_serviceName = dimServerName + "/VOLTAGE_SETPOINT";
	 mp__setPoint_voltageService = new DimService( v_setpoint_serviceName.c_str(), m_voltage_setPoint );

	 std::string i_current_limit_serviceName = dimServerName + "/CURRENT_LIMIT_SETPOINT";
	 mp_setPoint_currentService = new DimService( i_current_limit_serviceName.c_str(), m_current_limit);

	 std::string pserviceName = dimServerName + "/CURRENT_POWER_STATE";
	 mp_powerStateService = new DimService( pserviceName.c_str() , m_powerState);

	 mp_lvComLink = new LowVoltageCommunication();
	 

}


LowVoltageCommand::~LowVoltageCommand(void)
{
	/**
	 deletes the associated commands and services
	*/
	 delete mp_voltageCmd; 
	 delete mp_currentCmd; 
	 delete mp_powerStateCmd; 

	 delete mp_voltageService; 
	 delete mp_currentService;
	 delete mp_powerStateService;

	 delete mp_querySettingsCmd;

	 delete mp_lvComLink;

}

void LowVoltageCommand::commandHandler(void)
{
	/// This get called each time a command is issued by the client

	DimCommand *currCmd = getCommand(); // request current command

	/**
	Use else if to select which of the commands has been called
	*/

	if( currCmd == mp_voltageCmd ) 
	{
		// informing the user that the command has been recieved
		std::cout << "Set Voltage to " << currCmd->getFloat() << " received" << std::endl; 
		m_voltage = currCmd->getFloat(); // Get the value the user entered

		/**need to communicate with the power supply**/
		
		if( mp_lvComLink->ConnectToDevice() ){// test link. Currently always returns true - this maybe expanded in future to do some tests so have left it in
			
			// Use the comm link class to message the power supply to set the voltge setpoint 
			if ( mp_lvComLink->SetVoltage( m_voltage ) ){ // returns true if the communication with the PSU is successful
			
			// Some output from the server that tells the user what is being done
				std::cout<<"Get new voltage from PSU (SERVER)"<<std::endl;

				m_voltage = mp_lvComLink->VoltageReading();
				
				if(m_voltage != -999){ // -999 code is returned if the connetion to the psu fails.
					
					std::cout<<"New Voltage Value (SERVER)"<< m_voltage <<std::endl;
					mp_voltageService->updateService( m_voltage ); // updates the DIM service with the new variable
				
				}
				else{
				 /**
				 This should catch any communication error when reading back from the PSU fails - this should probably use the DIM server error handler??
				 */
				}
			}
			else{
				 /**
				 This should catch any communication error when setting the volatage of PSU fails
				 */
			}
		}
		else{
			// Need to handle events where the server cannot connect to the Low voltage
		
		}
	}
	else if( currCmd == mp_currentCmd )
	{
		 // informing the user that the command has been recieved
		std::cout << "Set Current to " << currCmd->getFloat() << " received" << std::endl;
		
		m_current = currCmd->getFloat(); //update the member variable to the request current
		
		if( mp_lvComLink->ConnectToDevice() ){// test link. Returns true - will be modified to do some connections tests at a later date
			
			// Next set new current limit on PSU - returns true if this succeds  
			if (mp_lvComLink->SetCurrent( m_current ) ){
				
				// Update to user that the server is requesting the current 
				std::cout<<"Get new current from PSU (SERVER)"<<std::endl;

				m_current = mp_lvComLink->CurrentReading();

				if( m_current != -999 ){ // Code returned if the server cannot query the PSU

					std::cout<<"New Current Value (SERVER)"<< m_current <<std::endl;
					mp_currentService->updateService( m_current ); // updates the DIM service with the new variable
				}
				else{
					/** TODO - add error handling */
				}
			}
			else{
				/** TODO - add error handling */
			}
		}

		else{
			// Need to handle events where the server cannot connect to the Low voltage
		}
	}
	else if( currCmd == mp_powerStateCmd )
	{
		// informing the user that the command has been recieved
		std::cout << "Set Power Status to " << currCmd->getShort() <<" received" << std::endl; 

		// Added some extra code to ensure the input is either 0 or 1
		if( currCmd->getShort() > 1 || currCmd->getShort() < 0){
			
			/** This may need to change when tying into PVSS */
			std::cout<<"SHORT NEEDS TO EVALUATE TO BOOL"<<std::endl;

		}
		else{
			/**update the current member variable */
			m_powerState = currCmd->getShort();
			
			if( mp_lvComLink->ConnectToDevice() ){// test link. Will add some extra testing to the connection for now returns true.
				
				//Set the power state of the PSU returns true if the link is established and the data is sent
				if( mp_lvComLink->SetPowerState( m_powerState ) ){
					
					// Some ouput server side so the user can see what is happening - could update this so we have a message class and verbosity setting
					std::cout<<"Get new power state from PSU (SERVER)"<<std::endl;
					
					// Query the power supply to get its current output status
					m_powerState = mp_lvComLink->CurrentPowerState();

					if( m_powerState != -999 ){ // -999 is the return code if anything goes wrong when connecting to the PSU

						//User update server side
						std::cout<<"New power state Value (SERVER)"<< m_powerState <<std::endl;
						
						// updates the DIM service with the new variable
						mp_currentService->updateService( m_powerState ); 
					}
					else{
						/** need to add extra error handling if PSU query fails */
					}
				}
				else{
					/** need to add extra error handling if setting the power state fails */
				}
			}
			else{
				// Need to handle events where the server cannot connect to the Low voltage
		
			}
		}
	}
	else if( currCmd == mp_refreashCmd )
	{
		
		//Get refreash command and construct string object
		std::string refreashCommand( mp_refreashCmd->getString() );

		/**Run through options using else if string comparions 
		   Options are: All - i.e query the PSU and update all the current services
						POWERSTATE - check the status of the power output
						VOLTAGE - check the output voltage
						CURRENT - check the output current
		*/

		if( refreashCommand == "ALL" ){
			
			/** open connection to the power supply and query the voltage and current and power state **/ 
			if( mp_lvComLink->ConnectToDevice() ){ // Always retrun true currently - might change to include extra connection checks

				m_voltage = mp_lvComLink->VoltageReading(); //Query the PSU for the current voltage output
				if( m_voltage != -999 ){ //-999 is output code if connection fails
					
					//Update the service with the current value
					mp_voltageService->updateService( m_voltage );
				}
				else{
					/** Need extra error handling - could use the data quality flag so the client can check that the update was successsful */
				}

				// Move onto checking the current
				m_current = mp_lvComLink->CurrentReading();
				if( m_current !=-999 ){// Code for failed connection
					
					//update current service with latest value
					mp_currentService->updateService( m_current ); 
				}
				else{
					//see above
				}

				m_powerState = mp_lvComLink->CurrentPowerState(); // get current state from PSU
				if( m_powerState !=-999 ){//check that the connection was succesful
					mp_powerStateService->updateService( m_powerState ); //update the service
				}
				else{
					//see voltage update comments
				}
			}
		}
		else if( refreashCommand == "POWERSTATE" ){
			//See above for comments on what the next lines do...
			
			if( mp_lvComLink->ConnectToDevice() ){

				m_powerState = mp_lvComLink->CurrentPowerState();	
				
				if( m_powerState != -999 ) {
					mp_powerStateService->updateService( m_powerState );
				}
				else{
					mp_powerStateService->updateService( m_powerState );
				}
			}
			else{
				// add error handling
			}
		
		}
		else if( refreashCommand == "VOLTAGE" ){
			//See above for comments on what the next lines do...
			if( mp_lvComLink->ConnectToDevice() ){
			    m_voltage = mp_lvComLink->VoltageReading();
				if( m_voltage != -999 ){
					mp_voltageService->updateService( m_voltage );
				}
				else{
					//error handling...
				}
			}
			else{
				//error handling
			}

		}
		else if( refreashCommand == "CURRENT" ){
			//See above for comments on what the next lines do...
			if( mp_lvComLink->ConnectToDevice() ){
				
				m_current = mp_lvComLink->CurrentReading();
				if( m_current != -999 ){
					mp_currentService->updateService( m_current ); 
				}
				else{
					//error handling
				}	
			}
			else{
				//error handling...
			}

		}
		else{
			//output if string input is not of the valid form
			std::cout<<"Command not supported the options are: ALL, POWERSTATE, VOLTAGE, CURRENT"<<std::endl;
		}
	}
	else if( currCmd == mp_querySettingsCmd){
		//Get query command and construct string object
		std::string queryCommand( mp_querySettingsCmd->getString() );
		std::cout<<"Query command: "<<queryCommand<<std::endl;
		/*Options are: 
			1) QI0SETPOINT - what is the current limit
			2) QV0SETPOINT - what is the voltage set point
		*/
		if( queryCommand == "QI0SETPOINT"){

			//See above for comments on what the next lines do...
			if( mp_lvComLink->ConnectToDevice() ){
				
				m_current_limit = mp_lvComLink->CurrentLimitReading();
				if( m_current_limit != -999 ){
					mp_setPoint_currentService->updateService( m_current_limit ); 
				}
				else{
					//error handling
				}	
			}
			else{
				//error handling...
			}

		}
		else if( queryCommand == "QV0SETPOINT" ){

			//See above for comments on what the next lines do...
			if( mp_lvComLink->ConnectToDevice() ){
				
				m_voltage_setPoint = mp_lvComLink->VoltageSetPointReading();
				if( m_voltage_setPoint != -999 ){
					mp__setPoint_voltageService->updateService( m_voltage_setPoint ); 
				}
				else{
					//error handling
				}	
			}
			else{
				//error handling...
			}


		}
		else{
			//output if string input is not of the valid form
			std::cout<<"Command not supported the options are: QI0SETPOINT, QV0SETPOINT"<<std::endl;
		}
	}
	else
	{	
		//output if none of the commands are supported
		std::cout << "Requested Command is not supported " << std::endl;
	}
	

}

void  LowVoltageCommand::PowerSupplyIPAddress( std::string ipAddress ){
	/**Member function that sets the ipaddress and also 
		initialises the socket
	*/
 	mp_lvComLink->IpAddressOfServer( ipAddress ); //sets the ipaddress of the PSU
	mp_lvComLink->InitialiseSocket(); // this returns true if the socket is successfully constructed - need to put into here some error handling
}

void LowVoltageCommand::Update( bool debug ){
	/** Server side update that allows all the current services to be queried and updated */
	if( debug ){
		m_voltage++;
		mp_voltageService->updateService( m_voltage );

		m_current++;
		mp_currentService->updateService( m_current ); 

		if( m_powerState == 1){
			mp_powerStateService->updateService( m_powerState );
			m_powerState = 0;
		}
		else{
			mp_powerStateService->updateService( m_powerState );
			m_powerState = 0;
		}

		m_debug_state = false;

	}
	else{
		if( mp_lvComLink->ConnectToDevice() ){//Checks the socket

			m_voltage = mp_lvComLink->VoltageReading();
			if( m_voltage != -999 ){//error code if communication failed
				mp_voltageService->updateService( m_voltage );
			}
			else{
				//error handling
			}

			m_current = mp_lvComLink->CurrentReading();
			if( m_current != -999 ){
				mp_currentService->updateService( m_current ); 
			}
			else{
				// error handling
			}


			if( mp_lvComLink->ConnectToDevice() ){

				m_powerState = mp_lvComLink->CurrentPowerState();
				if( m_powerState != -999 ) {
					mp_powerStateService->updateService( m_powerState );
				}
				else{
					mp_powerStateService->updateService( m_powerState );

				}
			}

		}
	}
}
