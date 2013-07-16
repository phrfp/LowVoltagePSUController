#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>
#include <string>
/**
	Class used to create a socket and connect it using TCP/IP to a PSU at a ceratin IP address
	then allow messages to be sent to the PSU and replies extracted
*/
/**Could really do with refactoring alot of code is repeated it is only the string going to the PSU that is changing
*/

class LowVoltageCommunication
{
public:
	LowVoltageCommunication(void);
	virtual ~LowVoltageCommunication(void);
	
	/** Sets the IP addess of the device */
	void IpAddressOfServer(std::string ipaddress);

	/** Creates tests the IP address of the PSU and creates a socket and connects it to the PSU */
	bool InitialiseSocket();

	/** connection setup and testing **/
	bool ConnectToDevice();
	
	/** Each element is set seperately there is some room for refactoring i.e one set function that has a flag for if it is voltage or current etc

	/** setting new voltage and reading commands **/
	bool SetVoltage( float newVoltage );
	float VoltageReading();
	float VoltageSetPointReading();

	/** setting new current and reading commands **/
	bool SetCurrent( float newCurrent );
	float CurrentReading();
	float CurrentLimitReading();

	/** setting power state and reading power state **/
	bool SetPowerState( short newPowerState );
	short CurrentPowerState();

	

protected:
	/** carries out parsing of the reply from the PSU 
		The int can be 0 or 1 depending on whether it is parsing the reponse to current/voltage (0) or output (1)
	*/

	float PSUStringParser(char *psuResponse, int queryType);

private:

	/** variables to store the current data values **/
	SOCKET m_socket; //Hold the socket at member varible
	std::string m_ipaddressOfServer; //Hold ip address 

};

