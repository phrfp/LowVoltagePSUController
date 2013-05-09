#pragma once
#include <dis.hxx>
#include <string>

class LowVoltageCommunication;

class LowVoltageCommand : public DimCommandHandler
{
public:
	void PowerSupplyIPAddress( char* ipAddress );
	LowVoltageCommand( std::string dimServerName );
	void Update();
	virtual ~LowVoltageCommand(void);
	void commandHandler(void); // overloaded method that allows server to recieve commands 

		
private:
	
	/** Dim commands **/
	DimCommand* mp_voltageCmd;
	DimCommand* mp_currentCmd;
	DimCommand* mp_powerStateCmd;
	DimCommand* mp_refreashCmd;
	/** Dim services **/
	DimService* mp_voltageService;
	DimService* mp_currentService;
	DimService* mp_powerStateService;
	/** data members holding the current values **/
	float m_voltage;
	float m_current;
	short m_powerState;

	LowVoltageCommunication* mp_lvComLink; //Comm link to lv part of the has association - odd direction but simplifies the execution

};

