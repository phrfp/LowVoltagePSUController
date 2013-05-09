#include "DIMServErrorHandler.h"
#include <Windows.h>
#include <iostream>


DIMServErrorHandler::DIMServErrorHandler(void)
{
}


DIMServErrorHandler::~DIMServErrorHandler(void)
{
}

void DIMServErrorHandler::errorHandler(int severity, int code, char *msg)
{
		SYSTEMTIME timeDate;

		GetSystemTime(&timeDate);

		m_logfile.open("log.txt", std::ios::app);
		std::cout<< "Time and Date: "<< timeDate.wDay << ":"<< timeDate.wMonth << ":"<< timeDate.wYear << " " << timeDate.wHour << ":" << timeDate.wMinute << ":" << timeDate.wSecond << " Servrity " << severity << " " << msg << std::endl;
		m_logfile << "Time and Date: "<< timeDate.wDay << ":"<< timeDate.wMonth << ":"<< timeDate.wYear << " " << timeDate.wHour << ":" << timeDate.wMinute << ":" << timeDate.wSecond << " Severity: "<<severity<<" Code: "<<code<<" Message: "<<msg<< std::endl;
		m_logfile.close();
}