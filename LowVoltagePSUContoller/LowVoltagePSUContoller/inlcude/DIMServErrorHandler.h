#pragma once
#include <fstream>
#include <dis.hxx>

class DIMServErrorHandler : public DimErrorHandler
{
public:
	DIMServErrorHandler(void);
	virtual ~DIMServErrorHandler(void);
	void errorHandler(int severity, int code, char *msg); // overloaded method to allow user defined error handling
private:
	std::ofstream m_logfile;
};

