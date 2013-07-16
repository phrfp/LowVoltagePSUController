/**
Class to parse the config file supplied by the user-

Format:

#DNS Settings
DNS_NODE <ipadress>
DNS_PORT <portNumber>

#Server settings
SERVER_NAME <nameOfServer>
NUM_PSUs <numberOfPSUs>
PSU_NAMES <PSU_1_NAME> ... <PSU_N_NAME>

#PSU Settings
IPADDRESSS <PSU_1_IPADDRESS> ... <PSU_N_ADDRESS>


*/
#include <string>
#include <vector>
class LowVoltageCommand;

class DIMServerConfigParser
{
public:
	DIMServerConfigParser(void);
	virtual ~DIMServerConfigParser(void);

	/** extracts the relavent info from the conf file */
	bool ConfigurerServer( std::string configfile ); // false failed to configure the setup - check error log
	void PrintCurrentSettings();

	int NumberOfPSUs();
	std::string ServerName();
	std::string DNSNode();
	std::string PSUName(int psuNumber);
	std::string PSUIpAddress(int psuNumber);

protected:
	/** Start up the server **/
	//bool StartupServer(); // Start up server providing that the configuration stage is successful
	

private:

	bool m_configAndStartUpSuccessFul;
	
	std::string m_configFile;
	std::string m_dns_node;
	std::string m_dns_port;

	std::string m_serverName;
	int m_numOfPSUs;
	std::vector<std::string> m_vectorOfPSUNames;
	std::vector<std::string> m_vectorOfPSUIpAddresss;

};

