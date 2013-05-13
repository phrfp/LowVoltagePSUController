#pragma once
#include <fstream>
#include <dis.hxx>
/**
	class to allow user defined error handling when an error is sent to the server the errorHandler method is called
	and an entry into the log file is made - tested so far on the dns server stopping and starting

	This would be the ideal place to pass errors in communuication with the PSU - maybe ??
*/
class DIMServErrorHandler : public DimErrorHandler
{
public:
	DIMServErrorHandler(void);
	virtual ~DIMServErrorHandler(void);
	void errorHandler(int severity, int code, char *msg); // overloaded method to allow user defined error handling
private:
	//logfile used to store errors
	std::ofstream m_logfile;
};

