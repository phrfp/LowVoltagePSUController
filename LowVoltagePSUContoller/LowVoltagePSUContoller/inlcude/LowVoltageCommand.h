#pragma once
#include <dis.hxx>
#include <string>

/**
	Command handler class to allow the Dim client to send commands to the server
	with a set of services that allow the server to publish results that the client
	subscries to.
*/

class LowVoltageCommunication; // Forward declaration of the comm class

class LowVoltageCommand : public DimCommandHandler
{
public:
	
	/**Ip address of the psu that is being connected to*/
	void PowerSupplyIPAddress( std::string ipAddress );
	
	/** Constructor that needs the server name to setup commands and services **/
	LowVoltageCommand( std::string dimServerName );
	
	/** Update function that talks to the PSU and then publishs the current values of the PSU */
	//void Update( std::string ServicesToUpdate );

	/** Update function that does not relay on a client to tell it when to refresh */

	void Update(bool debug);

	/** Constructor that destroys all the cmds and services that are built in the construtor */
	virtual ~LowVoltageCommand(void);

	/** overridden method from the DimCommandHandler that is called when the client calls one of the suppored commands */
	void commandHandler(void); // overloaded method that allows server to recieve commands 

		
private:
	
	/** Dim commands **/
	DimCommand* mp_voltageCmd; //Command for setting the voltage
	DimCommand* mp_currentCmd; //Command for setting the current
	DimCommand* mp_powerStateCmd; //Command for turning the power on/off
	DimCommand* mp_refreashCmd; //Command for querying the PSU for the current values of voltage, current and power status
	DimCommand* mp_querySettingsCmd; //Command for querying the PSU for the current settings i.e voltage and current limit
	/** Dim services **/

	DimService* mp_voltageService; //Voltage service
	DimService* mp_currentService; //Current service
	DimService* mp_powerStateService; //Powerstate service

	DimService* mp__setPoint_voltageService;//Voltage setpoint service
	DimService* mp_setPoint_currentService;//current limit setting service

	/** data members holding the current values **/
	float m_voltage;
	float m_current;
	short m_powerState;

	bool m_debug_state;

	float m_voltage_setPoint;
	float m_current_limit;

	LowVoltageCommunication* mp_lvComLink; //Comm link to lv. Part of "has a" association - odd direction but simplifies the execution

};

