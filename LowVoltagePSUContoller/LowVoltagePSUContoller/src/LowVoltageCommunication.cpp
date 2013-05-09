#include "LowVoltageCommunication.h"
#include <iostream>
#include <sstream>
#include <string>
#define BUFFERSIZE 512

LowVoltageCommunication::LowVoltageCommunication(void)
{
}


LowVoltageCommunication::~LowVoltageCommunication(void)
{
}

bool LowVoltageCommunication::InitialiseSocket(void){

	WSADATA wsaDat;
	
	if(WSAStartup( MAKEWORD(2,2), &wsaDat) != 0){

		std::cout<<"Error starting up windows sockets API"<<std::endl;
		return false;
	}

	struct addrinfo *result = NULL;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints) );
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	int iresult = getaddrinfo( m_ipaddressOfServer, "9221", &hints, &result); //169.254.227.33 - for inst 1 00-50-C2-F9-56-34, 169.254.30.174 - for inst 2 00-50-C2-F9-56-35
	if(iresult != 0 ){
	
		std::cout<<"Get address failed with error: "<< iresult <<std::endl;
		return false;
		WSACleanup();
	}

	m_socket = INVALID_SOCKET;
	m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol); // set up a tcp / ip socket
	
	if( m_socket == INVALID_SOCKET ){
	
		std::cout<<"Error creating socket"<<std::endl;
		return false;
		WSACleanup();
		
	}

	//try to connect

	if(connect( m_socket, result->ai_addr, result->ai_addrlen ) != 0 ){

		std::cout<<"Error creating connection to PSU"<<std::endl;
		return false;
		WSACleanup();
	}

	return true;

}

void LowVoltageCommunication::IpAddressOfServer(char* ipaddress){

	m_ipaddressOfServer = ipaddress;
}

bool LowVoltageCommunication::ConnectToDevice(void)
{
	

	return true;

}

bool LowVoltageCommunication::SetVoltage( float newVoltage ){

	/**code to change the voltage on the power supply **/

	// send test command

	int sendVal = SOCKET_ERROR;

	std::stringstream voltsetpoint;
	voltsetpoint << "V1 " << newVoltage;

	while(sendVal == SOCKET_ERROR){
			
		sendVal = send(m_socket, voltsetpoint.str().c_str(), strlen(voltsetpoint.str().c_str())+1, 0);
		if( (sendVal ==0) || (sendVal == WSAECONNRESET) || (sendVal == WSAETIMEDOUT) ){
		
			std::cout<<"Error sending command to PSU"<<std::endl;
			return false;
			
		}
	}

	return true;
}

float LowVoltageCommunication::VoltageReading()
{

	char voltageQuery[] = "V1O?";

	int sendVal = SOCKET_ERROR;

	while(sendVal == SOCKET_ERROR){
			
		sendVal = send(m_socket, voltageQuery, strlen(voltageQuery)+1, 0);
		if( (sendVal ==0) || (sendVal == WSAECONNRESET) || (sendVal == WSAETIMEDOUT) ){
		
			std::cout<<"Error sending command to PSU"<<std::endl;
			return -999;
			
		}
	}

	int recVal = SOCKET_ERROR;
	char psuResponse[BUFFERSIZE];

	while(recVal == SOCKET_ERROR){
			
		recVal = recv(m_socket, psuResponse, BUFFERSIZE, 0);

		if( (recVal ==0) || (recVal == WSAECONNRESET) || (recVal == WSAETIMEDOUT) ){
		
			std::cout<<"Error receving reply from PSU"<<std::endl;
			return -999;
			
		}
	}

	float voltage = this->PSUStringParser( psuResponse, 0 );

	std::cout<<"Voltage set to: "<<voltage<<std::endl;

	return voltage;
}

bool LowVoltageCommunication::SetCurrent( float newCurrent ){
	
	int sendVal = SOCKET_ERROR;

	std::stringstream currentsetpoint;
	currentsetpoint << "I1 " << newCurrent;

	while(sendVal == SOCKET_ERROR){
			
		sendVal = send(m_socket, currentsetpoint.str().c_str(), strlen(currentsetpoint.str().c_str())+1, 0);
		if( (sendVal ==0) || (sendVal == WSAECONNRESET) || (sendVal == WSAETIMEDOUT) ){
		
			std::cout<<"Error sending command to PSU"<<std::endl;
			return false;
			
		}
	}

	return true;
}

float LowVoltageCommunication::CurrentReading(){

	char currentQuery[] = "I1O?";

	int sendVal = SOCKET_ERROR;

	while(sendVal == SOCKET_ERROR){
			
		sendVal = send(m_socket, currentQuery, strlen(currentQuery)+1, 0);
		if( (sendVal ==0) || (sendVal == WSAECONNRESET) || (sendVal == WSAETIMEDOUT) ){
		
			std::cout<<"Error sending command to PSU"<<std::endl;
			return -999;
			
		}
	}

	int recVal = SOCKET_ERROR;
	char psuResponse[BUFFERSIZE];

	while(recVal == SOCKET_ERROR){
			
		recVal = recv(m_socket, psuResponse, BUFFERSIZE, 0);

		if( (recVal ==0) || (recVal == WSAECONNRESET) || (recVal == WSAETIMEDOUT) ){
		
			std::cout<<"Error receving reply from PSU"<<std::endl;
			return -999;
			
		}
	}

	float current = this->PSUStringParser( psuResponse, 0 );

	std::cout<<"Current set to: "<<current<<std::endl;

	return current;
}

bool LowVoltageCommunication::SetPowerState( short newPowerState ){
	
	int sendVal = SOCKET_ERROR;

	std::stringstream currentsetpoint;
	currentsetpoint << "OP1 " << newPowerState;

	while(sendVal == SOCKET_ERROR){
			
		sendVal = send(m_socket, currentsetpoint.str().c_str(), strlen(currentsetpoint.str().c_str())+1, 0);
		if( (sendVal ==0) || (sendVal == WSAECONNRESET) || (sendVal == WSAETIMEDOUT) ){
		
			std::cout<<"Error sending command to PSU"<<std::endl;
			return false;
			
		}
	}

	return true;

}
short LowVoltageCommunication::CurrentPowerState(){
	
	char currentQuery[] = "OP1?";

	int sendVal = SOCKET_ERROR;

	while(sendVal == SOCKET_ERROR){
			
		sendVal = send(m_socket, currentQuery, strlen(currentQuery)+1, 0);
		if( (sendVal ==0) || (sendVal == WSAECONNRESET) || (sendVal == WSAETIMEDOUT) ){
		
			std::cout<<"Error sending command to PSU"<<std::endl;
			return -999;
			
		}
	}

	int recVal = SOCKET_ERROR;
	char psuResponse[BUFFERSIZE];

	while(recVal == SOCKET_ERROR){
			
		recVal = recv(m_socket, psuResponse, BUFFERSIZE, 0);

		if( (recVal ==0) || (recVal == WSAECONNRESET) || (recVal == WSAETIMEDOUT) ){
		
			std::cout<<"Error receving reply from PSU"<<std::endl;
			return -999;
			
		}
	}

	float powerState = this->PSUStringParser( psuResponse, 1 );

	powerState = static_cast<short>(powerState);

	std::cout<<"PowerState set to: "<<powerState<<std::endl;

	return powerState;
}

float LowVoltageCommunication::PSUStringParser(char *psuResponse, int queryType ){

	std::string response(psuResponse);
	std::stringstream responseStream(response);

	std::string firstLineRes;

	//Get first line corresponding to the channel identifier and value

	std::getline( responseStream, firstLineRes );

	//std::cout<< firstLineRes << std::endl;
	std::string numStr;
	std::string::size_type len = std::string::npos;
	if(queryType == 0 ){ numStr = firstLineRes.substr(0, (len - 1) );}
	else if( queryType == 1 ){ numStr = firstLineRes; }
	//std::cout<< numStr <<std::endl;

	std::stringstream numSS( numStr );

	float reading =0;

	numSS >> reading;

	//std::cout << reading <<std::endl;

	return reading;
}