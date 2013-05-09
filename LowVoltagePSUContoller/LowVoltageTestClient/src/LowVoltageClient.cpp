// LowVoltageClient.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <iostream>
#include <dic.hxx>
#include <sstream>
#include <Windows.h>
using namespace std;

class ErrorHandler : public DimErrorHandler
{
	void errorHandler(int severity, int code, char *msg)
	{
		int index = 0;
		char **services;
		if(code){}
		cout << severity << " " << msg << endl;
		services = DimClient::getServerServices();
		cout<< "from "<< DimClient::getServerName() << " services:" << endl;
		while(services[index])
		{
			cout << services[index] << endl;
			index++;
		}
	}
public:
	ErrorHandler() {DimClient::addErrorHandler(this);}
};

class CurrentService : public DimInfo
{
	void infoHandler(){

		std::cout<<"CHECKING CURRENT (CLIENT) "<<std::endl;

		std::cout<<"Current Current: "<< getFloat() <<std::endl;

	}
public:
	CurrentService( std::string serverName ) : DimInfo( (serverName + "/CURRENT_CURRENT").c_str() ,-1){};
};

class VoltageService : public DimInfo
{
	void infoHandler(){

		std::cout<<"CHECKING VOLTAGE (CLIENT)"<<std::endl;
		//float volatge=0;
//		volatge = getFloat();
		std::cout<<"Current Voltage: "<< getFloat() <<std::endl;

	}
public:
	VoltageService( std::string serverName ) : DimInfo( (serverName + "/CURRENT_VOLTAGE").c_str() ,-1) {};
};

class PowerStateService : public DimInfo
{
	void infoHandler(){

		std::cout<<"CHECKING POWER STATE (CLIENT)"<<std::endl;
	//	float volatge=0;
//		volatge = getFloat();
		std::cout<<"Current Power State: "<< getShort() <<std::endl;

	}

public:
	PowerStateService(std::string serverName) : DimInfo( (serverName + "/CURRENT_POWER_STATE").c_str() ,-1) {};
};

int main (int argc, char* argv[])
{
	std::string serverName;

	if( (argc == 1) || ( argc > 2 ) ){

		std::cout<<"Test DIM client: usage .\LowVoltage.exe <serverName> "<<std::endl;
		exit(1);
	}
	else{
		serverName = (argv[1]);
		std::cout<<"Starting Client to test server: "<< serverName <<std::endl;
		
	}

	//ErrorHandler errHandler;
	//StrService servstr;
	VoltageService voltserv( serverName );
	CurrentService currserv( serverName );
	PowerStateService powerserv( serverName );

	while(true)
	{


		sleep(5);

		/*
		 * Test sending voltage command to the server
		 */
		string input_voltage="";
		float voltage=0;

		// This code converts from string to number safely.
		while(true){
			cout << "SET VOLTAGE : ";
			getline(std::cin, input_voltage);
			std::stringstream userInputVoltage( input_voltage );
			if( userInputVoltage >> voltage){
				break;
			}
			std::cout << "Incorrect type entered, try again "<<std::endl;
		}

		DimClient::sendCommand( (serverName + "/SET_VOLTAGE").c_str() ,voltage);

		sleep(5);
		/*
		 * Test sending current command to the sever
		 */
		string input_current="";
		float current=0;

		// This code converts from string to number safely.
		while(true){
			cout << "SET CURRENT : ";
			getline(std::cin, input_current);
			std::stringstream userInputCurrent( input_current );
			if( userInputCurrent >> current){
				break;
			}
			std::cout << "Incorrect type entered, try again "<<std::endl;
		}

		DimClient::sendCommand( (serverName + "/SET_CURRENT").c_str() ,current);

		sleep(5);
		/*
		 * Test sending power command to the sever
		 */
		string input_powerState="";
		short powerState=0;

		// This code converts from string to number safely.
		while(true){
			cout << "SET POWER STATE : ";
			getline(std::cin, input_powerState);
			std::stringstream userInputPowerState( input_powerState );
			if( userInputPowerState >> powerState){
				break;
			}
			std::cout << "Incorrect type entered, try again "<<std::endl;
		}

		DimClient::sendCommand( (serverName + "/SET_POWER_STATE").c_str() ,powerState);

		sleep(5);

		string input_finshedSetup="";
		// This code converts from string to number safely.
		while(true){
			cout << "Are you finished setting up the power supply (y/n) ? : ";
			getline(std::cin, input_finshedSetup);
			
			if( input_finshedSetup == "y"){
				break;
			}
			else if( input_finshedSetup == "n"){
				break;
			}
			std::cout << "Incorrect type entered, try again "<<std::endl;
		}
		if( input_finshedSetup == "y"){
				break;
		}
	}

	/**** Send client into logging mode ****/
	std::cout<<"Start logging"<<std::endl;
	while(true){
		sleep(10);
		std::cout<<"Do refreash"<<std::endl;
		DimClient::sendCommand( (serverName + "/REFREASH_DATA_VARIABLES").c_str() ,"ALL");
	}

	return 0;
}


