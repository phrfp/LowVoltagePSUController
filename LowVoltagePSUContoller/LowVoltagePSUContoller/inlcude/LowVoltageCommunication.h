#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <WS2tcpip.h>

class LowVoltageCommunication
{
public:
	LowVoltageCommunication(void);
	virtual ~LowVoltageCommunication(void);
	
	bool InitialiseSocket();

	/** connection setup and testing **/
	bool ConnectToDevice();
	
	/** setting new voltage and reading commands **/
	bool SetVoltage( float newVoltage );
	float VoltageReading();

	/** setting new current and reading commands **/
	bool SetCurrent( float newCurrent );
	float CurrentReading();

	/** setting power state and reading power state **/
	bool SetPowerState( short newPowerState );
	short CurrentPowerState();

	void IpAddressOfServer(char* ipaddress);

protected:
	float PSUStringParser(char *psuResponse, int queryType);

private:

	/** variables to store the current data values **/
	SOCKET m_socket;
	char* m_ipaddressOfServer;
	float m_voltageReading;
	float m_currentReading;
	short m_powerStateReading;
};

