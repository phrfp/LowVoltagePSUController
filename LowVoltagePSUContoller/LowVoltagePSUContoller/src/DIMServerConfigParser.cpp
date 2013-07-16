#include "DIMServerConfigParser.h"
#include <fstream>
#include <iostream>
#include <sstream>

DIMServerConfigParser::DIMServerConfigParser(void)
{
}


DIMServerConfigParser::~DIMServerConfigParser(void)
{
}

bool DIMServerConfigParser::ConfigurerServer(std::string configName){


	m_configFile = configName;

	std::fstream temp_confFileStream;

	temp_confFileStream.open(m_configFile, std::fstream::in);

	if( temp_confFileStream.is_open() ){

		std::cout<<"Configuring DIM server using "<< configName << std::endl;

		std::string dns_heading("[DNS Settings]");
		std::string dns_node_heading("DNS_NODE");
		std::string dns_port_heading("DNS_PORT");

		std::string dim_heading("[DIM Server Settings]");
		std::string dim_server_name_heading("SERVER_NAME");
		std::string dim_numberOfPsu_heading("NUM_PSUs");
		std::string dim_PSU_names("PSU_NAMES");

		std::string psu_heading("[PSU Settings]");
		std::string psu_ipAddress_heading("IPADDRESSS");

		bool dnsSettingsFound = false;
		bool dnsNodeFound = false;
		bool dnsPortFound = false;

		bool dimSettingsFound = false;
		bool dimServerName = false;
		bool dimNumPSU = false;
		bool dimPSUNames = false;

		bool psuSettingsFound = false;
		bool psuIPAddressFound = false;

		while(  !temp_confFileStream.eof() ){

			std::string line;
			std::getline(temp_confFileStream, line);
			std::string::size_type headingFound;

			headingFound = line.find( dns_heading );
			if( headingFound != std::string::npos){
				std::cout<<"CHECKING DNS SETTINGS"<<std::endl;	
				dnsSettingsFound = true;
			}

			headingFound = line.find(dns_node_heading);
			if( headingFound != std::string::npos ){
				std::stringstream lineStream(line);
				lineStream.ignore(256,' ');
				std::getline(lineStream, m_dns_node);
				dnsNodeFound = true;
				//std::cout<< m_dns_node <<std::endl;
			}

			headingFound = line.find(dns_port_heading);
			if( headingFound != std::string::npos ){
				std::stringstream lineStream(line);
				lineStream.ignore(256,' ');
				std::getline(lineStream, m_dns_port);
				dnsPortFound = true;
				//std::cout<< m_dns_port <<std::endl;
			}

			headingFound = line.find( dim_heading );
			if( headingFound != std::string::npos){
				std::cout<<"CHECKING DIM SERVER SETTINGS"<<std::endl;
				dimSettingsFound = true;
			}

			headingFound = line.find(dim_server_name_heading);
			if( headingFound != std::string::npos ){
				std::stringstream lineStream(line);
				lineStream.ignore(256,' ');
				std::getline(lineStream, m_serverName);
				dimServerName = true;
				//std::cout<< m_serverName <<std::endl;
			}

			headingFound = line.find(dim_numberOfPsu_heading);
			if( headingFound != std::string::npos ){
				std::stringstream lineStream(line);
				lineStream.ignore(256,' ');
				//std::getline(lineStream, m_serverName);
				lineStream >> m_numOfPSUs;
				dimNumPSU = true;
				//std::cout<< m_numOfPSUs <<std::endl;
			}

			headingFound = line.find(dim_PSU_names);
			if( headingFound != std::string::npos ){
				std::stringstream lineStream(line);
				lineStream.ignore(256,' ');
				m_vectorOfPSUNames.reserve(m_numOfPSUs);
				std::string temp_line_PSU;
				std::getline(lineStream, temp_line_PSU);
				dimPSUNames = true;
				//std::cout<< temp_line_PSU <<std::endl;
				std::string::size_type startChar = 0;
				std::string::size_type endCharPos = 0;
				for(int iNumPSUs=0; iNumPSUs < m_numOfPSUs; iNumPSUs++){
					std::string::size_type endCharPos = temp_line_PSU.find(' ', startChar);
					std::string::size_type tempNameLength =  endCharPos - startChar;
					std::string tempPSUname = temp_line_PSU.substr(startChar, tempNameLength );
					//std::cout<<tempPSUname<<std::endl;
					m_vectorOfPSUNames.push_back(tempPSUname);
					startChar = endCharPos;
					startChar++;
				}

			}

			headingFound = line.find( psu_heading );
			if( headingFound != std::string::npos){
				std::cout<<"CHECKING PSU SETTINGS"<<std::endl;
				psuSettingsFound = true;

			}

			headingFound = line.find(psu_ipAddress_heading);
			if( headingFound != std::string::npos ){
				std::stringstream lineStream(line);
				lineStream.ignore(256,' ');
				m_vectorOfPSUIpAddresss.reserve(m_numOfPSUs);
				std::string temp_line_PSU;
				std::getline(lineStream, temp_line_PSU);
				psuIPAddressFound = true;
				//std::cout<< temp_line_PSU <<std::endl;
				std::string::size_type startChar = 0;
				std::string::size_type endCharPos = 0;
				for(int iNumPSUs=0; iNumPSUs < m_numOfPSUs; iNumPSUs++){
					std::string::size_type endCharPos = temp_line_PSU.find(' ', startChar);
					std::string::size_type tempNameLength =  endCharPos - startChar;
					std::string tempPSUname = temp_line_PSU.substr(startChar, tempNameLength );
					//std::cout<<tempPSUname<<std::endl;
					m_vectorOfPSUIpAddresss.push_back(tempPSUname);
					startChar = endCharPos;
					startChar++;
				}

			}

		}
		if( !dnsSettingsFound || !dimSettingsFound || !psuSettingsFound ){
			if(!dnsSettingsFound){
				std::cout<<"ERROR FINDING DNS SETTINGS TAG. CHECK CONFIG FILE"<<std::endl;
			}
			else if( !dimSettingsFound){
				std::cout<<"ERROR FINDING DIM SETTINGS TAG. CHECK CONFIG FILE"<<std::endl;
			}
			else if(!psuSettingsFound){
				std::cout<<"ERROR FINDING PSU SETTINGS TAG. CHECK CONFIG FILE"<<std::endl;
			}
			exit(1);
		}

	}
	else{

		std::cout<<" FAILED TO OPEN CONFIG FILE: "<<m_configFile<<std::endl;
		exit(1);
	}
	
	temp_confFileStream.close();

	this->PrintCurrentSettings();

	return true;
}


void DIMServerConfigParser::PrintCurrentSettings(){
		

	std::cout<<"CURRENTLY LOADED CONFIGURATION"<<std::endl;
	std::cout<<"DNS SETTINGS"<<std::endl;
	std::cout<<"DNS NODE: "<< m_dns_node <<std::endl;
	std::cout<<"DNS PORT: "<< m_dns_port <<std::endl;
	std::cout<<"DIM SERVER SETTINGS"<<std::endl;
	std::cout<<"DIM SERVER NAME: "<< m_serverName <<std::endl;
	std::cout<<"NUMBER OF PSUs: "<< m_numOfPSUs <<std::endl;
	std::cout<<"NAME OF PSUs "<<std::endl;

	for(std::vector<std::string>::iterator iPSUName = m_vectorOfPSUNames.begin(); iPSUName != m_vectorOfPSUNames.end();  iPSUName++){
		
		std::cout<< *iPSUName <<std::endl;
	
	}
	std::cout<<"PSU SETTINGS "<<std::endl;
	std::cout<<"PSU IP ADDRESS'S "<<std::endl;
	for(std::vector<std::string>::iterator iPSUip = m_vectorOfPSUIpAddresss.begin(); iPSUip != m_vectorOfPSUIpAddresss.end();  iPSUip++){
		
		std::cout<< *iPSUip <<std::endl;
	
	}
}

int DIMServerConfigParser::NumberOfPSUs(){

	return m_numOfPSUs;

}
std::string DIMServerConfigParser::ServerName(){

	return m_serverName;
}
std::string DIMServerConfigParser::DNSNode(){

	return m_dns_node;
}
std::string DIMServerConfigParser::PSUName(int psuNumber){

	return m_vectorOfPSUNames[psuNumber];

}
std::string DIMServerConfigParser::PSUIpAddress(int psuNumber){

	return m_vectorOfPSUIpAddresss[psuNumber];

}