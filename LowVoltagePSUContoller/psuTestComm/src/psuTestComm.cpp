// psuTestComm.cpp : Defines the entry point for the console application.
//


#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <iostream>

int main(int argc, char* argv[])
{
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

	int iresult = getaddrinfo("169.254.227.33", "9221", &hints, &result);
	if(iresult != 0 ){
	
		std::cout<<"Get address failed with error: "<< iresult <<std::endl;
		WSACleanup();
	}

	SOCKET m_socket = INVALID_SOCKET;
	m_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol); // set up a tcp / ip socket
	
	if( m_socket == INVALID_SOCKET ){
	
		std::cout<<"Error creating socket"<<std::endl;
		return false;
		
	}

	//try to connect

	if(connect( m_socket, result->ai_addr, result->ai_addrlen ) != 0 ){

		std::cout<<"Error creating connection to PSU"<<std::endl;
		return false;
	}

	// send test command

	int sendVal = SOCKET_ERROR;

	char idtest[] = "I1 0.750";

	while(sendVal == SOCKET_ERROR){
			
		sendVal = send(m_socket, idtest, strlen(idtest)+1, 0);
		if( (sendVal ==0) || (sendVal == WSAECONNRESET) || (sendVal == WSAETIMEDOUT) ){
		
			std::cout<<"Error sending command to PSU"<<std::endl;
			return false;
			
		}
	}


	char voltageQuery[] = "I1O?";

	sendVal = SOCKET_ERROR;

	while(sendVal == SOCKET_ERROR){
			
		sendVal = send(m_socket, voltageQuery, strlen(voltageQuery)+1, 0);
		if( (sendVal ==0) || (sendVal == WSAECONNRESET) || (sendVal == WSAETIMEDOUT) ){
		
			std::cout<<"Error sending command to PSU"<<std::endl;
			return false;
			
		}
	}

	int recVal = SOCKET_ERROR;
	char psuResponse[512];

	while(recVal == SOCKET_ERROR){
			
		recVal = recv(m_socket, psuResponse, 512, 0);

		if( (recVal ==0) || (recVal == WSAECONNRESET) || (recVal == WSAETIMEDOUT) ){
		
			std::cout<<"Error receving reply from PSU"<<std::endl;
			return false;
			
		}
	}

	std::cout<<"PSU RESPNSE: "<<psuResponse<<std::endl;

	freeaddrinfo(result);
	WSACleanup();
	return 0;
}

