#include "LowVoltageCommunication.h"
#include <iostream>
#include <sstream>
#include <string>
#define BUFFERSIZE 512 //set the size of the recieved commands

LowVoltageCommunication::LowVoltageCommunication(void)
{
}


LowVoltageCommunication::~LowVoltageCommunication(void)
{
	WSACleanup(); //This might be needed when the object is destroyed...
}

bool LowVoltageCommunication::InitialiseSocket(void){

	WSADATA wsaDat; // holds data relating to winsock
	
	if(WSAStartup( MAKEWORD(2,2), &wsaDat) != 0){ //request version 2_2 of winsock

		std::cout<<"Error starting up windows sockets API"<<std::endl;
		return false;
	}

	struct addrinfo *result = NULL; //use address infor structs to store info relating to the host and the server
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints) ); //set struct to zero?? not sure exactly what this does
	hints.ai_family = AF_UNSPEC; // Set the family to unsecified could set to ipv4 for instance
	hints.ai_socktype = SOCK_STREAM; // sock stream this means it will stream the number of bytes defined by the buffer 
	hints.ai_protocol = IPPROTO_TCP; // Set protol to TCP/IP

	//Get infor from the server - need ip of server the port number the host address infor and the result that is acquried from the server

	int iresult = getaddrinfo( m_ipaddressOfServer, "9221", &hints, &result); //169.254.227.33 - for inst 1 00-50-C2-F9-56-34, 169.254.30.174 - for inst 2 00-50-C2-F9-56-35
	
	if(iresult != 0 ){//if this is unsucessful cleanup - might need to add some message to the server create a set of failure codes
	
		std::cout<<"Get address failed with error: "<< iresult <<std::endl;
		WSACleanup();
		return false;
		
	}

	m_socket = INVALID_SOCKET; // Initialise a socket to invalid
	m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol); // set up socket using the infor from the server
	
	if( m_socket == INVALID_SOCKET ){ // check that the socket has been initialised
	
		std::cout<<"Error creating socket"<<std::endl;
		WSACleanup();
		return false;
		
		
	}

	//try to connect the socket to the psu
	if(connect( m_socket, result->ai_addr, result->ai_addrlen ) != 0 ){

		std::cout<<"Error creating connection to PSU"<<std::endl;
		WSACleanup();
		return false;
		
	}

	return true;

}

void LowVoltageCommunication::IpAddressOfServer(char* ipaddress){

	m_ipaddressOfServer = ipaddress; // set the ip address of the psu
}

bool LowVoltageCommunication::ConnectToDevice(void)
{
	/** function that can be expanded to do further tests on the connection */
	return true;

}

bool LowVoltageCommunication::SetVoltage( float newVoltage ){

	
	//setup a varible to contain the send bytes initialise to error - guessing this is simply 0
	int sendVal = SOCKET_ERROR;

	//use streams to create the command to send to the PSU
	std::stringstream voltsetpoint;
	voltsetpoint << "V1 " << newVoltage; //send the command V1 <newVoltage>

	while(sendVal == SOCKET_ERROR){//checks the send completes or if an errror occurs
			
		//use send to command to change the voltage of the PSU 
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
	// Voltage query command V1O? checkes the output voltage NOT the setpoint
	char voltageQuery[] = "V1O?"; //command to send to the PSU

	int sendVal = SOCKET_ERROR; //use same trick as before to check that the query is sent

	while(sendVal == SOCKET_ERROR){
			
		sendVal = send(m_socket, voltageQuery, strlen(voltageQuery)+1, 0);
		if( (sendVal ==0) || (sendVal == WSAECONNRESET) || (sendVal == WSAETIMEDOUT) ){
		
			std::cout<<"Error sending command to PSU"<<std::endl;
			return -999; // code retruned if communication fails
			
		}
	}

	//Move on to the recieved response
	int recVal = SOCKET_ERROR;
	char psuResponse[BUFFERSIZE]; // variable to hold the response

	while(recVal == SOCKET_ERROR){//need while loop so that the response is picked up as if the recv command does not coinside with the PSU response the message will be lost
			
		recVal = recv(m_socket, psuResponse, BUFFERSIZE, 0);

		if( (recVal ==0) || (recVal == WSAECONNRESET) || (recVal == WSAETIMEDOUT) ){
		
			std::cout<<"Error receving reply from PSU"<<std::endl;
			return -999; //again the error code for failed comm is -999
			
		}
	}

	float voltage = this->PSUStringParser( psuResponse, 0 ); //Parses the message from the PSU and converts it to a float

	//some info for the user
	std::cout<<"Voltage set to: "<<voltage<<std::endl;

	return voltage; //return the voltage
}


bool LowVoltageCommunication::SetCurrent( float newCurrent ){
	
	/**Function works the same way as the setvoltage function see that for comments */

	int sendVal = SOCKET_ERROR;

	std::stringstream currentsetpoint;
	currentsetpoint << "I1 " << newCurrent; // Set the current setpoint in the PSU in amps

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

	/** function works the same as the currentvoltagereading function  see that for comments*/ 

	char currentQuery[] = "I1O?"; // Query for the current output in Amps of the PSU

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
	currentsetpoint << "OP1 " << newPowerState; // Turn the output on (1) and off (0) of the PSU

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
	
	char currentQuery[] = "OP1?"; //Check the whether the PSU has its output turned on or off

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

	powerState = static_cast<short>(powerState); // convert the float to short not the best way - could make the function a template

	std::cout<<"PowerState set to: "<<powerState<<std::endl;

	return powerState;
}

float LowVoltageCommunication::PSUStringParser(char *psuResponse, int queryType ){

	//Create a string and ten a stringstream from the psu response
	std::string response(psuResponse);
	std::stringstream responseStream(response);

	//variable to hold the first line sent back from the PSU
	std::string firstLineRes; 

	//Get first line corresponding to the PSU response - it terminates response with an end of line character that can be used by getline

	std::getline( responseStream, firstLineRes ); // from the response extract the first line

	//std::cout<< firstLineRes << std::endl;
	std::string numStr; //String that contains the numeriacl reponse from the PSU - old intrested in numerical value at the moment...
	std::string::size_type len = std::string::npos; //length of the line

	if(queryType == 0 ){ numStr = firstLineRes.substr(0, (len - 1) );} // if voltage or current is being queried need to remove unit
	else if( queryType == 1 ){ numStr = firstLineRes; } //for power just need the first line
	//std::cout<< numStr <<std::endl;

	std::stringstream numSS( numStr ); //Create a stream object from the number string

	float reading =0;

	numSS >> reading; //write numss into the float 

	//std::cout << reading <<std::endl;

	return reading; //return the float
}